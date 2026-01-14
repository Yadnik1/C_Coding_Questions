/*
 * ============================================================================
 *        DESIGN: WATCHDOG RECOVERY SEQUENCE
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★★ (CRITICAL)
 *   - 8-hour unattended operation
 *   - Must recover from firmware hangs
 *   - Cannot lose user's video footage
 *   - Field devices need self-healing
 *
 * ============================================================================
 *                    WATCHDOG TIMER BASICS
 * ============================================================================
 *
 * CONCEPT:
 *   - Hardware timer counts down continuously
 *   - Firmware must "feed" (reset) timer before it reaches zero
 *   - If timer reaches zero: hardware forces system reset
 *   - Catches: infinite loops, deadlocks, runaway code
 *
 * TIMELINE:
 *
 *   Normal Operation:
 *   WDT ████████░░░░ feed ████████░░░░ feed ████████░░░░ feed
 *                                                          ↑
 *                                                    never reaches 0
 *
 *   Hung Code:
 *   WDT ████████░░░░ ░░░░ ░░░░ ░░░░░░░░ RESET!
 *                    ↑                    ↑
 *              code stuck          WDT expired
 *
 * TYPES:
 *   1. Independent Watchdog (IWDG): Runs from separate clock, most reliable
 *   2. Window Watchdog (WWDG): Feed only within time window
 *   3. Software Watchdog: Timer-based, can be disabled (less reliable)
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// WATCHDOG HARDWARE ABSTRACTION
// ============================================================================

#define WDT_TIMEOUT_MS 5000  // 5 seconds

typedef enum {
    RESET_REASON_POWER_ON,
    RESET_REASON_WATCHDOG,
    RESET_REASON_SOFTWARE,
    RESET_REASON_BROWNOUT,
    RESET_REASON_EXTERNAL,
    RESET_REASON_UNKNOWN
} ResetReason;

// Simulated hardware
static bool wdt_enabled = false;
static uint32_t wdt_counter = 0;
static ResetReason last_reset = RESET_REASON_POWER_ON;

void hal_wdt_init(uint32_t timeout_ms) {
    wdt_enabled = true;
    wdt_counter = timeout_ms;
    printf("[WDT] Initialized, timeout=%u ms\n", timeout_ms);
}

void hal_wdt_feed(void) {
    if (wdt_enabled) {
        wdt_counter = WDT_TIMEOUT_MS;
        printf("[WDT] Fed (reset counter)\n");
    }
}

ResetReason hal_get_reset_reason(void) {
    return last_reset;
}

void hal_clear_reset_reason(void) {
    last_reset = RESET_REASON_UNKNOWN;
}

// ============================================================================
// WATCHDOG MANAGER
// ============================================================================

/*
 * MULTI-TASK WATCHDOG:
 *   Each task must check in periodically.
 *   Main watchdog only fed if ALL tasks healthy.
 *   Identifies which task is stuck.
 */

#define MAX_WDT_TASKS 8

typedef struct {
    const char* name;
    uint32_t last_checkin;
    uint32_t max_interval_ms;
    bool enabled;
    bool healthy;
} WdtTask;

typedef struct {
    WdtTask tasks[MAX_WDT_TASKS];
    uint8_t task_count;
    uint32_t current_time;
    uint8_t failed_task;  // Which task failed (for diagnosis)
} WdtManager;

WdtManager wdt_mgr = {0};

int wdt_register_task(const char* name, uint32_t max_interval_ms) {
    if (wdt_mgr.task_count >= MAX_WDT_TASKS) return -1;

    int id = wdt_mgr.task_count++;
    wdt_mgr.tasks[id].name = name;
    wdt_mgr.tasks[id].max_interval_ms = max_interval_ms;
    wdt_mgr.tasks[id].last_checkin = 0;
    wdt_mgr.tasks[id].enabled = true;
    wdt_mgr.tasks[id].healthy = true;

    printf("[WDT] Registered task '%s' (max interval: %u ms)\n",
           name, max_interval_ms);
    return id;
}

void wdt_task_checkin(int task_id) {
    if (task_id < 0 || task_id >= wdt_mgr.task_count) return;

    wdt_mgr.tasks[task_id].last_checkin = wdt_mgr.current_time;
    wdt_mgr.tasks[task_id].healthy = true;
}

bool wdt_check_all_tasks(void) {
    bool all_healthy = true;

    for (int i = 0; i < wdt_mgr.task_count; i++) {
        WdtTask* task = &wdt_mgr.tasks[i];

        if (!task->enabled) continue;

        uint32_t elapsed = wdt_mgr.current_time - task->last_checkin;

        if (elapsed > task->max_interval_ms) {
            task->healthy = false;
            all_healthy = false;
            wdt_mgr.failed_task = i;
            printf("[WDT] Task '%s' UNHEALTHY (no checkin for %u ms)\n",
                   task->name, elapsed);
        }
    }

    return all_healthy;
}

void wdt_manager_update(uint32_t current_time_ms) {
    wdt_mgr.current_time = current_time_ms;

    if (wdt_check_all_tasks()) {
        hal_wdt_feed();  // All tasks OK, feed hardware watchdog
    } else {
        printf("[WDT] NOT feeding - task '%s' stuck!\n",
               wdt_mgr.tasks[wdt_mgr.failed_task].name);
        // Let hardware watchdog reset us
    }
}

// ============================================================================
// RESET RECOVERY SEQUENCE
// ============================================================================

/*
 * ON WATCHDOG RESET:
 *   1. Identify it was a watchdog reset
 *   2. Increment crash counter
 *   3. Save crash info for diagnosis
 *   4. Attempt safe recovery
 *   5. If too many crashes: enter safe mode
 */

#define MAX_WDT_RESETS_BEFORE_SAFE_MODE 3

// Stored in persistent memory (RTC RAM or flash)
typedef struct {
    uint32_t magic;
    uint8_t wdt_reset_count;
    uint8_t failed_task_id;
    uint32_t last_reset_timestamp;
    uint32_t total_wdt_resets;
} PersistentCrashInfo;

#define CRASH_INFO_MAGIC 0xCRASHED

PersistentCrashInfo crash_info = {0};

void load_crash_info(void) {
    // In real code: load from RTC RAM or flash
    if (crash_info.magic != CRASH_INFO_MAGIC) {
        // First boot or corrupted - initialize
        memset(&crash_info, 0, sizeof(crash_info));
        crash_info.magic = CRASH_INFO_MAGIC;
    }
}

void save_crash_info(void) {
    // In real code: save to RTC RAM or flash
}

typedef enum {
    BOOT_MODE_NORMAL,
    BOOT_MODE_SAFE,
    BOOT_MODE_RECOVERY
} BootMode;

BootMode determine_boot_mode(void) {
    load_crash_info();

    ResetReason reason = hal_get_reset_reason();

    if (reason == RESET_REASON_WATCHDOG) {
        crash_info.wdt_reset_count++;
        crash_info.total_wdt_resets++;
        crash_info.failed_task_id = wdt_mgr.failed_task;

        printf("[BOOT] Watchdog reset detected! Count: %d\n",
               crash_info.wdt_reset_count);

        if (crash_info.wdt_reset_count >= MAX_WDT_RESETS_BEFORE_SAFE_MODE) {
            printf("[BOOT] Too many WDT resets - entering SAFE MODE\n");
            return BOOT_MODE_SAFE;
        }

        save_crash_info();
        return BOOT_MODE_RECOVERY;
    }

    // Normal boot - clear crash counter
    crash_info.wdt_reset_count = 0;
    save_crash_info();

    return BOOT_MODE_NORMAL;
}

void handle_safe_mode(void) {
    printf("\n=== SAFE MODE ===\n");
    printf("- Disable non-essential features\n");
    printf("- Enable telemetry for diagnosis\n");
    printf("- Wait for user intervention or OTA fix\n");

    // In real code:
    // - Disable camera, complex processing
    // - Keep basic connectivity for remote diagnosis
    // - Blink error LED pattern
    // - Upload crash logs
}

void handle_recovery_mode(void) {
    printf("\n=== RECOVERY MODE ===\n");
    printf("- Initialize essential hardware only\n");
    printf("- Skip feature that may have caused crash\n");
    printf("- Monitor closely for stability\n");

    // In real code:
    // - Skip initialization of suspected problem area
    // - Reduce processing load
    // - Increase watchdog check frequency
}

// ============================================================================
// CRASH DIAGNOSTICS
// ============================================================================

void log_crash_diagnostics(void) {
    printf("\n=== CRASH DIAGNOSTICS ===\n");
    printf("Total WDT resets: %u\n", crash_info.total_wdt_resets);
    printf("Recent consecutive: %d\n", crash_info.wdt_reset_count);
    printf("Last failed task: %d\n", crash_info.failed_task_id);

    // In real code: include
    // - Stack trace (if available)
    // - Last known program counter
    // - Memory usage at time of crash
    // - Active interrupts
}

// ============================================================================
// WINDOW WATCHDOG (More sophisticated)
// ============================================================================

/*
 * WINDOW WATCHDOG:
 *   - Must feed within a time window, not just before timeout
 *   - Catches both stuck code AND too-fast loops
 *
 *   |------ Window ------|
 *   |  Early  |   OK   |
 *   |  RESET  |  FEED  | RESET
 *   |---------|--------|------
 *   0       min      max  timeout
 *
 *   Feed too early: suspicious, might be runaway loop
 *   Feed in window: normal operation
 *   Feed too late: stuck code
 */

typedef struct {
    uint32_t window_start_ms;
    uint32_t window_end_ms;
    uint32_t last_feed;
    bool window_violation;
} WindowWatchdog;

void wwdt_init(WindowWatchdog* wwdt, uint32_t min_ms, uint32_t max_ms) {
    wwdt->window_start_ms = min_ms;
    wwdt->window_end_ms = max_ms;
    wwdt->last_feed = 0;
    wwdt->window_violation = false;
}

bool wwdt_feed(WindowWatchdog* wwdt, uint32_t current_time) {
    uint32_t elapsed = current_time - wwdt->last_feed;

    if (elapsed < wwdt->window_start_ms) {
        printf("[WWDT] TOO EARLY! Elapsed: %u ms (min: %u)\n",
               elapsed, wwdt->window_start_ms);
        wwdt->window_violation = true;
        return false;
    }

    if (elapsed > wwdt->window_end_ms) {
        printf("[WWDT] TOO LATE! Elapsed: %u ms (max: %u)\n",
               elapsed, wwdt->window_end_ms);
        wwdt->window_violation = true;
        return false;
    }

    wwdt->last_feed = current_time;
    printf("[WWDT] Fed OK at %u ms (elapsed: %u)\n", current_time, elapsed);
    return true;
}

// ============================================================================
// DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   WATCHDOG RECOVERY DEMO\n");
    printf("============================================\n");

    // Initialize hardware watchdog
    hal_wdt_init(WDT_TIMEOUT_MS);

    // Check boot mode
    BootMode mode = determine_boot_mode();

    switch (mode) {
        case BOOT_MODE_SAFE:
            handle_safe_mode();
            return 0;
        case BOOT_MODE_RECOVERY:
            handle_recovery_mode();
            break;
        case BOOT_MODE_NORMAL:
            printf("[BOOT] Normal boot\n");
            break;
    }

    // Register tasks
    int task_main = wdt_register_task("main_loop", 1000);
    int task_sensor = wdt_register_task("sensor", 500);
    int task_comm = wdt_register_task("communication", 2000);

    // Simulate normal operation
    printf("\n--- Normal Operation ---\n");
    for (uint32_t time = 0; time < 2000; time += 100) {
        wdt_task_checkin(task_main);
        if (time % 200 == 0) wdt_task_checkin(task_sensor);
        if (time % 500 == 0) wdt_task_checkin(task_comm);

        wdt_manager_update(time);
    }

    // Simulate stuck sensor task
    printf("\n--- Simulating Stuck Task ---\n");
    for (uint32_t time = 2000; time < 4000; time += 100) {
        wdt_task_checkin(task_main);
        // sensor task NOT checking in!
        if (time % 500 == 0) wdt_task_checkin(task_comm);

        wdt_manager_update(time);
    }

    // Log diagnostics
    log_crash_diagnostics();

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "How do you design a watchdog strategy for a complex system?"
 * A: "Multi-level approach:
 *    1. Hardware watchdog: Ultimate safety net, can't be disabled
 *    2. Task-level watchdog: Each task checks in, identifies culprit
 *    3. Window watchdog: Catches runaway loops too
 *
 *    Only feed hardware WDT if all tasks are healthy.
 *    Log which task failed for post-crash diagnosis."
 *
 * Q: "What do you do after a watchdog reset?"
 * A: "Recovery sequence:
 *    1. Identify reset cause (read reset reason register)
 *    2. Increment crash counter (persistent storage)
 *    3. If too many crashes: enter safe mode
 *    4. Log crash info for remote diagnosis
 *    5. Attempt graceful restart
 *    6. Clear crash counter after stable operation"
 *
 * Q: "When should you NOT feed the watchdog?"
 * A: "1. During flash erase (long blocking operation)
 *       - Either increase timeout or feed during operation
 *    2. During sleep
 *       - Disable WDT or use RTC-based WDT
 *    3. In hard fault handler
 *       - Let it reset for clean recovery"
 *
 * ============================================================================
 */
