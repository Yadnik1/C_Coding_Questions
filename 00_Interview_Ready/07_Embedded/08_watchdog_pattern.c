/*
 * ============================================================================
 * PROBLEM: Watchdog Timer Pattern
 * ============================================================================
 *
 * WHAT IS THIS?
 * A watchdog timer (WDT) is a hardware timer that resets the system if not
 * periodically "fed" (reset) by software. If the software hangs, crashes, or
 * enters an infinite loop, the watchdog expires and forces a system reset,
 * allowing recovery from fault conditions.
 *
 * WHY IS THIS CRITICAL FOR EMBEDDED SYSTEMS?
 * - System Recovery: Automatic reset from software hangs or crashes
 * - Safety Critical: Medical devices, automotive, aerospace require watchdog
 * - Remote Systems: Unattended devices (sensors, IoT) can self-recover
 * - Fault Detection: Detects deadlocks, infinite loops, stack overflows
 * - Certification: Safety standards (IEC 61508, ISO 26262) require watchdog
 * - Reliability: 24/7 operation without manual intervention
 *
 * EXAMPLES:
 * Simple Watchdog:
 *   while(1) {
 *     read_sensors();
 *     process_data();
 *     send_results();
 *     watchdog_feed();  // Must reach here within timeout!
 *   }
 *
 * Task Monitoring:
 *   Each RTOS task checks in periodically
 *   Monitor task only feeds watchdog if ALL tasks checked in
 *   Single hung task -> watchdog reset
 *
 * KEY CONCEPT:
 * Three patterns:
 * 1. Simple: Feed at end of main loop
 * 2. Task Monitoring: Multiple tasks must all check in
 * 3. Window Watchdog: Must feed within time window (not too early OR late)
 *
 * VISUAL:
 *
 *   BASIC WATCHDOG OPERATION:
 *
 *   Time:    0    100   200   300   400   500   600ms
 *            |     |     |     |     |     |     |
 *   Counter: 500   400   300   200   100   500   400  (counts down)
 *                                    ^     ^
 *                                    |     Feed! Counter reset
 *                                    |
 *                             Would reset if not fed!
 *
 *   Normal Operation:
 *   +------+------+------+------+------+------+
 *   | Loop | Loop | Loop | Loop | Loop | Loop |
 *   | FEED | FEED | FEED | FEED | FEED | FEED |
 *   +------+------+------+------+------+------+
 *   Counter never reaches zero -> No reset
 *
 *   Hung System:
 *   +------+------+----------------HUNG---------------
 *   | Loop | Loop | Infinite loop...
 *   | FEED | FEED | (no feed)
 *   +------+------+-----------------------------------
 *   Counter: 500   400   300   200   100    0 -> RESET!
 *
 *
 *   TASK MONITORING PATTERN:
 *
 *   Task 1        Task 2        Task 3       Monitor       Watchdog
 *   ------        ------        ------       -------       --------
 *   [check in] -> [ check in] -> [check in] -> All OK? -> [FEED]
 *       |             |              |            |
 *       v             v              v            v
 *   +-------+    +-------+      +-------+    +--------+
 *   |Sensor |    |Comms  |      |Motor  |    |Check   |
 *   |Task   |    |Task   |      |Task   |    |all live|
 *   +-------+    +-------+      +-------+    +--------+
 *       |             |              |            |
 *       +-------------+--------------+            |
 *                     |                           v
 *                  Shared                    Only feed if
 *                  check-in                  ALL checked in!
 *                  flags
 *
 *   If Task 2 hangs: Monitor sees Task 2 timeout -> No feed -> Reset!
 *
 *
 *   WINDOW WATCHDOG:
 *
 *   Time:     0      50      100     150     200
 *             |-------|-------|-------|-------|
 *             | Early |  VALID WINDOW | Late  |
 *             | Zone  |    (OK here)  | Zone  |
 *             |-------|-------|-------|-------|
 *                     ^               ^
 *                     |               |
 *             Window opens     Window closes
 *
 *   Feed at 30ms:  ERROR! (too early - loop too fast)
 *   Feed at 80ms:  OK! (within window)
 *   Feed at 180ms: ERROR! (too late - loop too slow)
 *
 *   Detects BOTH stuck code AND runaway code!
 *
 *
 *   WRONG VS RIGHT USAGE:
 *
 *   WRONG (defeats purpose):        RIGHT (catches hangs):
 *   +-------------------+           +-------------------+
 *   | void Timer_ISR(){ |           | while(1) {        |
 *   |   WDT_Feed();     | <-BAD!    |   read_sensors(); |
 *   | }                 |           |   process();      |
 *   +-------------------+           |   communicate();  |
 *   ISR always feeds,               |   WDT_Feed();     | <-GOOD!
 *   main loop could                 | }                 |
 *   be completely hung!             +-------------------+
 *                                   Only feeds after ALL
 *                                   tasks complete!
 *
 * ============================================================================
 */

// Watchdog Timer Pattern - ESSENTIAL for embedded system reliability
// Prevents system lockup, ensures recovery from faults

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================
// Simulated Watchdog Timer (Real HW would use registers)
// ============================================================

typedef struct {
    uint32_t timeout_ms;
    uint32_t counter;
    bool enabled;
    bool triggered;
} WatchdogTimer;

static WatchdogTimer wdt = {0};
static uint32_t system_time = 0;

// Say: "Initialize watchdog with timeout period"
void watchdog_init(uint32_t timeout_ms) {
    wdt.timeout_ms = timeout_ms;
    wdt.counter = timeout_ms;
    wdt.enabled = false;
    wdt.triggered = false;
    printf("[WDT] Initialized with %u ms timeout\n", timeout_ms);
}

// Say: "Enable watchdog - starts counting down"
void watchdog_enable(void) {
    wdt.enabled = true;
    wdt.counter = wdt.timeout_ms;
    printf("[WDT] Enabled\n");
}

// Say: "Feed/kick the watchdog - reset counter"
void watchdog_feed(void) {
    if (wdt.enabled) {
        wdt.counter = wdt.timeout_ms;
        // printf("[WDT] Fed - counter reset\n");  // Commented to reduce noise
    }
}

// Say: "Simulated tick - call from systick ISR"
void watchdog_tick(uint32_t elapsed_ms) {
    if (!wdt.enabled || wdt.triggered) return;

    if (wdt.counter <= elapsed_ms) {
        wdt.counter = 0;
        wdt.triggered = true;
        printf("\n[WDT] *** TIMEOUT - SYSTEM RESET ***\n");
        // In real system: trigger reset or recovery
    } else {
        wdt.counter -= elapsed_ms;
    }
}

bool watchdog_triggered(void) {
    return wdt.triggered;
}

// ============================================================
// Task Monitoring Pattern
// ============================================================

// Say: "Monitor multiple tasks for proper execution"
#define MAX_TASKS 4

typedef struct {
    const char *name;
    uint32_t last_checkin;
    uint32_t timeout_ms;
    bool alive;
} TaskMonitor;

static TaskMonitor tasks[MAX_TASKS] = {0};
static int num_tasks = 0;

// Say: "Register a task for monitoring"
int task_monitor_register(const char *name, uint32_t timeout_ms) {
    if (num_tasks >= MAX_TASKS) return -1;

    tasks[num_tasks].name = name;
    tasks[num_tasks].timeout_ms = timeout_ms;
    tasks[num_tasks].last_checkin = system_time;
    tasks[num_tasks].alive = true;

    printf("[MON] Registered task '%s' with %u ms timeout\n", name, timeout_ms);
    return num_tasks++;
}

// Say: "Task checks in - proves it's running"
void task_checkin(int task_id) {
    if (task_id >= 0 && task_id < num_tasks) {
        tasks[task_id].last_checkin = system_time;
        tasks[task_id].alive = true;
    }
}

// Say: "Check all tasks, feed watchdog only if all OK"
bool task_monitor_check(void) {
    bool all_ok = true;

    for (int i = 0; i < num_tasks; i++) {
        uint32_t elapsed = system_time - tasks[i].last_checkin;
        if (elapsed > tasks[i].timeout_ms) {
            tasks[i].alive = false;
            printf("[MON] Task '%s' TIMEOUT (%u ms)\n", tasks[i].name, elapsed);
            all_ok = false;
        }
    }

    // Say: "Only feed watchdog if ALL tasks are healthy"
    if (all_ok) {
        watchdog_feed();
    }

    return all_ok;
}

// ============================================================
// Window Watchdog Pattern
// ============================================================

// Say: "Window WDT: must feed within a time window, not too early or late"
typedef struct {
    uint32_t window_start;  // Say: "Don't feed before this"
    uint32_t window_end;    // Say: "Must feed before this"
    uint32_t counter;
    bool in_window;
} WindowWatchdog;

static WindowWatchdog wwdt = {0};

void window_watchdog_init(uint32_t early_limit, uint32_t late_limit) {
    wwdt.window_start = early_limit;
    wwdt.window_end = late_limit;
    wwdt.counter = 0;
    wwdt.in_window = false;
    printf("[WWDT] Window: %u - %u ms\n", early_limit, late_limit);
}

// Say: "Returns true if feed was in valid window"
bool window_watchdog_feed(void) {
    if (wwdt.counter < wwdt.window_start) {
        printf("[WWDT] ERROR: Fed too early! (%u ms)\n", wwdt.counter);
        return false;
    } else if (wwdt.counter > wwdt.window_end) {
        printf("[WWDT] ERROR: Fed too late! (%u ms)\n", wwdt.counter);
        return false;
    } else {
        printf("[WWDT] Fed in window (%u ms) - OK\n", wwdt.counter);
        wwdt.counter = 0;
        return true;
    }
}

void window_watchdog_tick(uint32_t elapsed_ms) {
    wwdt.counter += elapsed_ms;
}

// ============================================================
// Demo
// ============================================================

void simulate_time(uint32_t ms) {
    system_time += ms;
    watchdog_tick(ms);
    window_watchdog_tick(ms);
}

int main() {
    printf("=== Watchdog Timer Patterns ===\n\n");

    // Basic watchdog demo
    printf("--- Basic Watchdog ---\n");
    watchdog_init(100);  // 100ms timeout
    watchdog_enable();

    printf("Simulating normal operation (feeding every 50ms):\n");
    for (int i = 0; i < 5; i++) {
        simulate_time(50);
        watchdog_feed();
        printf("  t=%u ms - fed\n", system_time);
    }

    // Task monitoring demo
    printf("\n--- Task Monitoring ---\n");
    system_time = 0;  // Reset for demo
    watchdog_init(200);
    watchdog_enable();

    int sensor_task = task_monitor_register("Sensor", 100);
    int comm_task = task_monitor_register("Comm", 150);

    printf("\nNormal operation:\n");
    for (int i = 0; i < 3; i++) {
        simulate_time(50);
        task_checkin(sensor_task);
        task_checkin(comm_task);
        task_monitor_check();
        printf("  t=%u ms - all tasks OK\n", system_time);
    }

    printf("\nSensor task hangs:\n");
    for (int i = 0; i < 3; i++) {
        simulate_time(50);
        // sensor_task NOT checking in!
        task_checkin(comm_task);
        bool ok = task_monitor_check();
        printf("  t=%u ms - %s\n", system_time, ok ? "OK" : "FAULT");
    }

    // Window watchdog demo
    printf("\n--- Window Watchdog ---\n");
    window_watchdog_init(50, 100);  // Feed between 50-100ms

    wwdt.counter = 0;
    simulate_time(30);
    window_watchdog_feed();  // Too early

    wwdt.counter = 0;
    simulate_time(75);
    window_watchdog_feed();  // In window

    wwdt.counter = 0;
    simulate_time(120);
    window_watchdog_feed();  // Too late

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Watchdog timer ensures system recovery from lockups or faults.

 BASIC WATCHDOG:
 - Hardware timer counts down
 - Software must 'feed' (reset) before timeout
 - If not fed in time, triggers reset
 - Catches infinite loops, deadlocks, crashes

 PROPER WATCHDOG USAGE:
 1. Don't just feed in timer ISR (defeats purpose!)
 2. Feed only after verifying system health
 3. Feed at end of main loop, after all tasks run
 4. Use task monitoring for multi-task systems

 TASK MONITORING PATTERN:
 - Each task checks in periodically
 - Monitor verifies all tasks alive
 - Only feed watchdog if ALL tasks healthy
 - Catches individual task hangs

 WINDOW WATCHDOG:
 - Must feed WITHIN a time window
 - Too early = fault (loop running too fast)
 - Too late = fault (loop running too slow)
 - Catches both lockups AND runaway code

 INDEPENDENT WATCHDOG (IWDG):
 - Runs on separate clock
 - Works even if main clock fails
 - Last-resort safety

 EMBEDDED BEST PRACTICES:
 1. Enable watchdog early in boot
 2. Choose appropriate timeout (not too short/long)
 3. Don't disable watchdog in production
 4. Test watchdog actually resets system
 5. Log watchdog resets for debugging

 COMMON MISTAKES:
 - Feeding in ISR (doesn't monitor main loop)
 - Timeout too short (false triggers)
 - Timeout too long (slow recovery)
 - Disabling for debugging, forgetting to re-enable
 - Not testing the reset functionality"
*/
