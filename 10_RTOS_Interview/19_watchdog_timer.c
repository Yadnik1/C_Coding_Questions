/*
 * ============================================================================
 * QUESTION 19: What is a Watchdog Timer? How is it used in RTOS?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * Watchdog timers are essential for system reliability in embedded systems.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "A watchdog timer is a hardware counter that must be periodically 'fed' or
 * 'kicked' by software. If the software fails to reset the watchdog within
 * its timeout period, the watchdog triggers a system reset. This recovers
 * from software hangs, infinite loops, or deadlocks. In RTOS, the challenge
 * is monitoring multiple tasks - you can't just kick the watchdog from one
 * task because other tasks might be stuck. The solution is a dedicated
 * watchdog task that only kicks when ALL monitored tasks have checked in."
 *
 * ============================================================================
 * VISUALIZATION: Watchdog Operation
 * ============================================================================
 *
 *   NORMAL OPERATION (Software healthy):
 *
 *   Watchdog Counter
 *   (Timeout: 1000ms)
 *
 *   |████████| 1000ms
 *   |███████ | 875ms
 *   |██████  | 750ms
 *   |█████   | 625ms
 *   |████    | 500ms   <- Software kicks (resets to 1000ms)
 *   |████████| 1000ms
 *   |███████ | 875ms
 *   |██████  | 750ms
 *   |█████   | 625ms
 *   |████    | 500ms   <- Software kicks again
 *   |████████| 1000ms
 *   ...continues...
 *
 *
 *   FAILURE CASE (Software hung):
 *
 *   |████████| 1000ms
 *   |███████ | 875ms
 *   |██████  | 750ms
 *   |█████   | 625ms
 *   |████    | 500ms   <- Software should kick here but doesn't!
 *   |███     | 375ms
 *   |██      | 250ms
 *   |█       | 125ms
 *   |        | 0ms     <- TIMEOUT! Watchdog triggers RESET!
 *
 *   **** SYSTEM RESET ****
 *
 *   System reboots and recovers from hang
 *
 * ============================================================================
 * MULTI-TASK WATCHDOG PATTERN
 * ============================================================================
 *
 *   PROBLEM: Simple watchdog kick from main loop doesn't work in RTOS
 *
 *   BAD APPROACH:
 *   -------------
 *
 *   void task_A(void) {
 *       while(1) {
 *           do_work();
 *           kick_watchdog();  // If Task B hangs, watchdog doesn't know!
 *       }
 *   }
 *
 *
 *   GOOD APPROACH: Watchdog Monitor Task
 *   ------------------------------------
 *
 *   +-----------+    +-----------+    +-----------+    +-----------+
 *   |  Task A   |    |  Task B   |    |  Task C   |    |  Task D   |
 *   +-----------+    +-----------+    +-----------+    +-----------+
 *         |               |               |               |
 *         | check_in()    | check_in()    | check_in()    | check_in()
 *         v               v               v               v
 *   +-------------------------------------------------------------+
 *   |                    Watchdog Monitor Task                     |
 *   |                                                              |
 *   |   Check-in Flags:  [A:✓]  [B:✓]  [C:✓]  [D:✓]               |
 *   |                                                              |
 *   |   If ALL checked in within window:                          |
 *   |       -> Kick hardware watchdog                             |
 *   |       -> Clear all flags                                    |
 *   |   Else:                                                     |
 *   |       -> DON'T kick (let system reset)                      |
 *   +-------------------------------------------------------------+
 *                              |
 *                              v
 *                    +------------------+
 *                    | Hardware Watchdog|
 *                    |  (in MCU)        |
 *                    +------------------+
 *
 * ============================================================================
 * CODE EXAMPLE: Multi-Task Watchdog System
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
#include "FreeRTOS.h"
#include "task.h"

// ============================================================================
// WATCHDOG MONITOR IMPLEMENTATION
// ============================================================================

#define NUM_MONITORED_TASKS 4
#define WATCHDOG_TIMEOUT_MS 1000   // Hardware watchdog timeout
#define CHECK_PERIOD_MS     500    // How often to verify check-ins

// Bit flags for task check-ins
#define TASK_A_BIT  (1 << 0)
#define TASK_B_BIT  (1 << 1)
#define TASK_C_BIT  (1 << 2)
#define TASK_D_BIT  (1 << 3)
#define ALL_TASKS   (TASK_A_BIT | TASK_B_BIT | TASK_C_BIT | TASK_D_BIT)

static volatile uint32_t task_checkin_flags = 0;
static SemaphoreHandle_t checkin_mutex;

// Called by monitored tasks to indicate they're alive
void watchdog_checkin(uint32_t task_bit) {
    if (xSemaphoreTake(checkin_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        task_checkin_flags |= task_bit;
        xSemaphoreGive(checkin_mutex);
    }
}

// Hardware-specific watchdog functions
extern void hardware_watchdog_init(uint32_t timeout_ms);
extern void hardware_watchdog_kick(void);

// Watchdog monitor task
void watchdog_task(void *pvParameters) {
    TickType_t last_wake = xTaskGetTickCount();

    // Initialize hardware watchdog
    hardware_watchdog_init(WATCHDOG_TIMEOUT_MS);

    for (;;) {
        // Wait for check period
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(CHECK_PERIOD_MS));

        // Check if all tasks have checked in
        if (xSemaphoreTake(checkin_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            uint32_t flags = task_checkin_flags;

            if ((flags & ALL_TASKS) == ALL_TASKS) {
                // All tasks healthy - kick the watchdog
                hardware_watchdog_kick();

                // Clear flags for next check period
                task_checkin_flags = 0;
            } else {
                // Some task(s) didn't check in!
                // Log which tasks are stuck
                printf("Watchdog: Missing check-ins from: ");
                if (!(flags & TASK_A_BIT)) printf("A ");
                if (!(flags & TASK_B_BIT)) printf("B ");
                if (!(flags & TASK_C_BIT)) printf("C ");
                if (!(flags & TASK_D_BIT)) printf("D ");
                printf("\n");

                // DON'T kick watchdog - let system reset!
            }

            xSemaphoreGive(checkin_mutex);
        }
    }
}


// ============================================================================
// MONITORED TASK EXAMPLES
// ============================================================================

void task_A(void *pvParameters) {
    for (;;) {
        // Do normal work
        process_sensor_data();

        // Check in with watchdog monitor
        watchdog_checkin(TASK_A_BIT);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void task_B(void *pvParameters) {
    for (;;) {
        // Blocking operation (e.g., waiting for queue)
        uint8_t data;
        if (xQueueReceive(my_queue, &data, pdMS_TO_TICKS(400)) == pdTRUE) {
            process_data(data);
        }

        // Still check in even if queue was empty
        // (Indicates task is alive and not hung)
        watchdog_checkin(TASK_B_BIT);
    }
}


// ============================================================================
// INITIALIZATION
// ============================================================================

void init_watchdog_system(void) {
    checkin_mutex = xSemaphoreCreateMutex();

    // Create watchdog monitor at high priority
    xTaskCreate(
        watchdog_task,
        "Watchdog",
        256,
        NULL,
        configMAX_PRIORITIES - 1,  // High priority
        NULL
    );
}
*/

/*
 * ============================================================================
 * HARDWARE WATCHDOG EXAMPLES
 * ============================================================================
 */

/*
// ============================================================================
// STM32 Independent Watchdog (IWDG)
// ============================================================================

#include "stm32f4xx.h"

void hardware_watchdog_init_stm32(uint32_t timeout_ms) {
    // Enable write access to IWDG registers
    IWDG->KR = 0x5555;

    // Set prescaler (divide by 256)
    IWDG->PR = IWDG_PR_PR_2 | IWDG_PR_PR_1;  // /256

    // Set reload value
    // IWDG clock = LSI = 32kHz, /256 = 125Hz = 8ms per tick
    uint32_t reload = (timeout_ms / 8) - 1;
    if (reload > 0xFFF) reload = 0xFFF;
    IWDG->RLR = reload;

    // Start watchdog
    IWDG->KR = 0xCCCC;
}

void hardware_watchdog_kick_stm32(void) {
    // Reload the watchdog counter
    IWDG->KR = 0xAAAA;
}


// ============================================================================
// Zephyr Watchdog API
// ============================================================================

#include <zephyr/drivers/watchdog.h>

const struct device *wdt;
int wdt_channel_id;

void zephyr_watchdog_init(void) {
    wdt = DEVICE_DT_GET(DT_ALIAS(watchdog0));
    if (!device_is_ready(wdt)) {
        printk("Watchdog device not ready\n");
        return;
    }

    struct wdt_timeout_cfg wdt_config = {
        .window.min = 0,
        .window.max = 1000,  // 1 second timeout
        .callback = NULL,    // NULL = reset on timeout
        .flags = WDT_FLAG_RESET_SOC,
    };

    wdt_channel_id = wdt_install_timeout(wdt, &wdt_config);
    if (wdt_channel_id < 0) {
        printk("Watchdog install error\n");
        return;
    }

    wdt_setup(wdt, WDT_OPT_PAUSE_HALTED_BY_DBG);
}

void zephyr_watchdog_kick(void) {
    wdt_feed(wdt, wdt_channel_id);
}
*/

/*
 * ============================================================================
 * ADVANCED: WINDOW WATCHDOG
 * ============================================================================
 *
 *   STANDARD WATCHDOG:
 *   - Must kick BEFORE timeout
 *   - Can kick anytime before timeout
 *
 *        0%            50%            100%
 *        |--------------|--------------|
 *        ^              ^              ^
 *        |  Can kick    |   Can kick   | Timeout
 *        |<-------------|------------->|  RESET!
 *
 *
 *   WINDOW WATCHDOG:
 *   - Must kick WITHIN a specific window
 *   - Too early OR too late = reset!
 *   - Catches runaway code that kicks too fast
 *
 *        0%      25%           75%      100%
 *        |--------|-------------|--------|
 *        ^        ^             ^        ^
 *        | Too    |   Valid     | Too    | Timeout
 *        | early! |   window    | late!  |
 *        | RESET  |   OK        | RESET  | RESET
 *
 *
 *   WHY WINDOW WATCHDOG?
 *   - Runaway loop might kick watchdog constantly
 *   - With window watchdog, kicking too fast = reset
 *   - Better detection of software faults
 *
 * ============================================================================
 * WATCHDOG TIMEOUT CALCULATION
 * ============================================================================
 *
 *   FACTORS TO CONSIDER:
 *
 *   1. Worst-case task execution time
 *   2. Blocking operations (queue waits, etc.)
 *   3. Lower priority task delays
 *   4. Interrupt latency
 *
 *   FORMULA:
 *
 *   Timeout > (Max_execution_time + Max_blocking_time + Safety_margin)
 *
 *   EXAMPLE:
 *   - Max task execution: 200ms
 *   - Max queue wait: 300ms
 *   - Safety margin: 200ms
 *   - Timeout = 200 + 300 + 200 = 700ms
 *   - Use 1000ms for extra safety
 *
 * ============================================================================
 * WATCHDOG BEST PRACTICES
 * ============================================================================
 *
 * 1. NEVER DISABLE IN PRODUCTION
 *    - Some developers disable for debugging
 *    - Must re-enable before release!
 *
 * 2. MONITOR ALL CRITICAL TASKS
 *    - Not just main loop
 *    - Include communication, control, safety tasks
 *
 * 3. APPROPRIATE TIMEOUT
 *    - Too short: false resets
 *    - Too long: slow recovery from hangs
 *
 * 4. LOG RESET REASON
 *    - Most MCUs have reset reason register
 *    - Log whether reset was watchdog, power-on, etc.
 *
 * 5. CAREFUL WITH BLOCKING
 *    - Tasks with long waits need special handling
 *    - Consider separate timeouts per task
 *
 * 6. TEST WATCHDOG BEHAVIOR
 *    - Intentionally cause hang to verify reset works
 *    - Part of system validation
 *
 * ============================================================================
 * DETECTING WATCHDOG RESET
 * ============================================================================
 */

/*
// STM32 Example: Check reset cause
void check_reset_cause(void) {
    if (RCC->CSR & RCC_CSR_IWDGRSTF) {
        printf("RESET CAUSE: Independent Watchdog\n");
        // Log error, possibly enter safe mode
    }
    else if (RCC->CSR & RCC_CSR_WWDGRSTF) {
        printf("RESET CAUSE: Window Watchdog\n");
    }
    else if (RCC->CSR & RCC_CSR_PORRSTF) {
        printf("RESET CAUSE: Power-on Reset\n");
    }
    else if (RCC->CSR & RCC_CSR_SFTRSTF) {
        printf("RESET CAUSE: Software Reset\n");
    }

    // Clear reset flags
    RCC->CSR |= RCC_CSR_RMVF;
}


// Zephyr Example
#include <zephyr/fatal.h>

void check_reset_cause_zephyr(void) {
    uint32_t cause = sys_reboot_cause_get();

    switch (cause) {
        case SYS_REBOOT_WARM:
            printk("Warm reboot\n");
            break;
        case SYS_REBOOT_COLD:
            printk("Cold reboot\n");
            break;
        // Platform-specific causes...
    }
}
*/

/*
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN THE PURPOSE
 *    "Recovers from software hangs that would otherwise require manual reset"
 *
 * 2. DESCRIBE MULTI-TASK CHALLENGE
 *    "Can't just kick from one task - need to monitor all critical tasks"
 *
 * 3. MENTION WINDOW WATCHDOG
 *    "Catches runaway code that kicks too frequently"
 *
 * 4. DISCUSS TIMEOUT SELECTION
 *    "Balance between fast recovery and avoiding false resets"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "What's the difference between hardware and software watchdog?"
 * A: Hardware watchdog is in MCU peripheral, runs independently of CPU.
 *    Software watchdog is implemented in code, can be fooled by hung task.
 *    Hardware watchdog is more reliable - use for critical systems.
 *
 * Q: "How do you handle tasks with variable-length blocking operations?"
 * A: Options: (1) Use maximum possible block time for timeout calculation,
 *    (2) Check in before and after blocking, (3) Use separate watchdog
 *    channels with different timeouts per task.
 *
 * Q: "Can watchdog detect priority inversion?"
 * A: Yes! If low-priority task holds resource and high-priority task is
 *    blocked too long, it won't check in, triggering watchdog reset.
 *
 * Q: "What about debugging with watchdog enabled?"
 * A: Most MCUs can freeze watchdog during debug halt (DBGMCU settings).
 *    Zephyr: WDT_OPT_PAUSE_HALTED_BY_DBG flag.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 19 ===\n\n");
    printf("Q: What is a Watchdog Timer?\n\n");
    printf("WATCHDOG TIMER:\n");
    printf("- Hardware counter that must be periodically 'fed'\n");
    printf("- If not fed in time -> system RESET\n");
    printf("- Recovers from software hangs and deadlocks\n\n");
    printf("MULTI-TASK WATCHDOG:\n");
    printf("- Can't just kick from one task\n");
    printf("- Use watchdog monitor task\n");
    printf("- All tasks must check in\n");
    printf("- Only kick if ALL checked in\n\n");
    printf("WINDOW WATCHDOG:\n");
    printf("- Must kick within time window\n");
    printf("- Too early OR too late = reset\n");
    printf("- Catches runaway code\n\n");
    printf("BEST PRACTICES:\n");
    printf("- Never disable in production\n");
    printf("- Monitor all critical tasks\n");
    printf("- Log reset reason\n");
    printf("- Test watchdog behavior\n");
    return 0;
}
