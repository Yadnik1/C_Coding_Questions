/*
 * ============================================================================
 * QUESTION 10: What is the Tick Rate? How does timing work in RTOS?
 * ============================================================================
 *
 * DIFFICULTY: Basic | FREQUENCY: High | IMPORTANCE: Critical
 *
 * Understanding tick rate is fundamental to RTOS timing behavior.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "The tick rate is the frequency of the RTOS system timer interrupt, typically
 * 100Hz to 1000Hz. Each tick, the scheduler checks for timed-out delays and
 * may perform context switches. The tick rate determines timing RESOLUTION -
 * with 1000Hz (1ms tick), you can't have delays shorter than 1ms. Higher tick
 * rates give better resolution but increase overhead. FreeRTOS defaults to
 * 1000Hz. The tradeoff is precision vs CPU overhead from frequent interrupts."
 *
 * ============================================================================
 * VISUALIZATION: Tick-Based Timing
 * ============================================================================
 *
 * Tick Rate: 1000 Hz (1ms per tick)
 *
 *   Time (ms): 0    1    2    3    4    5    6    7    8    9   10
 *              |    |    |    |    |    |    |    |    |    |    |
 *   Tick #:    0    1    2    3    4    5    6    7    8    9   10
 *              ^    ^    ^    ^    ^    ^    ^    ^    ^    ^    ^
 *              |    |    |    |    |    |    |    |    |    |    |
 *            Tick Tick Tick Tick Tick Tick Tick Tick Tick Tick Tick
 *            ISR  ISR  ISR  ISR  ISR  ISR  ISR  ISR  ISR  ISR  ISR
 *
 *
 *   vTaskDelay(5):  Task blocks for 5 ticks (5ms)
 *
 *   Task A: [RUN].....[BLOCKED for 5 ticks].....[RUN]
 *   Ticks:    0    1    2    3    4    5    6
 *                  ^                        ^
 *                  |                        |
 *              vTaskDelay(5)           Task wakes up
 *              called here
 *
 * ============================================================================
 * TICK RATE CONSIDERATIONS
 * ============================================================================
 *
 * +------------------+-------------------+-------------------+
 * | Tick Rate        | Resolution        | Overhead          |
 * +------------------+-------------------+-------------------+
 * | 10 Hz (100ms)    | Very coarse       | Very low          |
 * | 100 Hz (10ms)    | Coarse            | Low               |
 * | 1000 Hz (1ms)    | Fine (typical)    | Moderate          |
 * | 10000 Hz (0.1ms) | Very fine         | High              |
 * +------------------+-------------------+-------------------+
 *
 * HIGHER TICK RATE:
 * + Better timing resolution
 * + Faster response to timeouts
 * + More accurate delays
 * - More CPU overhead (more ISR executions)
 * - More power consumption
 *
 * LOWER TICK RATE:
 * + Less CPU overhead
 * + Better for battery-powered devices
 * - Coarse timing resolution
 * - Longer minimum delay
 *
 * ============================================================================
 * CONFIGURATION
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
// FreeRTOS Configuration (FreeRTOSConfig.h)
#define configTICK_RATE_HZ      1000    // 1000 Hz = 1ms tick
#define configCPU_CLOCK_HZ      72000000 // CPU frequency (for timer setup)

// Derived values
// Tick period in ms: 1000 / configTICK_RATE_HZ = 1ms
// Ticks per ms: configTICK_RATE_HZ / 1000 = 1


// Zephyr Configuration (prj.conf)
CONFIG_SYS_CLOCK_TICKS_PER_SEC=1000  # 1000 Hz tick rate
*/

/*
 * ============================================================================
 * DELAY FUNCTIONS
 * ============================================================================
 *
 * vTaskDelay(ticks):
 * - Delay for AT LEAST 'ticks' number of ticks
 * - Actual delay depends on when called within tick period
 *
 * vTaskDelayUntil(&lastWake, period):
 * - Delay until specific tick count
 * - Used for PERIODIC tasks with consistent period
 * - Accounts for execution time
 *
 * ============================================================================
 * vTaskDelay vs vTaskDelayUntil
 * ============================================================================
 *
 * vTaskDelay(10) - Delay 10 ticks FROM NOW
 *
 *   Task execution times vary:
 *
 *   |--5ms--|     |--8ms--|     |--3ms--|     |--6ms--|
 *   [EXEC] delay  [EXEC]  delay [EXEC]  delay [EXEC]
 *          10ms          10ms          10ms
 *
 *   Period varies: 15ms, 18ms, 13ms, 16ms (NOT consistent!)
 *
 *
 * vTaskDelayUntil(&lastWake, 10) - Delay until specific tick
 *
 *   |--5ms--|     |--8ms--|     |--3ms--|     |--6ms--|
 *   [EXEC]  5ms   [EXEC]  2ms   [EXEC]  7ms   [EXEC]
 *   delay         delay         delay
 *
 *   Period is EXACTLY 10ms every time!
 *
 * ============================================================================
 * CODE EXAMPLES
 * ============================================================================
 */

/*
// Convert milliseconds to ticks
#define pdMS_TO_TICKS(ms) ((TickType_t)(((ms) * configTICK_RATE_HZ) / 1000))

// FreeRTOS Examples

// Simple delay - waits AT LEAST 100ms
void simple_delay_task(void *pvParameters) {
    for (;;) {
        do_work();
        vTaskDelay(pdMS_TO_TICKS(100));  // 100ms delay
    }
}


// Periodic task with consistent timing
void periodic_task(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(50);  // 50ms period

    for (;;) {
        do_periodic_work();

        // Wait until 50ms since last wake (NOT 50ms from now)
        vTaskDelayUntil(&lastWakeTime, period);
    }
}


// Get current tick count
void timing_example(void) {
    TickType_t start = xTaskGetTickCount();

    do_something();

    TickType_t end = xTaskGetTickCount();
    TickType_t elapsed = end - start;

    printf("Elapsed: %u ticks (%u ms)\n",
           elapsed,
           elapsed * portTICK_PERIOD_MS);
}
*/

/*
// Zephyr Examples

void zephyr_delay_task(void *p1, void *p2, void *p3) {
    while (1) {
        do_work();
        k_sleep(K_MSEC(100));  // 100ms delay
    }
}

void zephyr_periodic_task(void *p1, void *p2, void *p3) {
    int64_t next_wake = k_uptime_get();

    while (1) {
        do_work();

        next_wake += 50;  // 50ms period
        k_sleep(K_TIMEOUT_ABS_MS(next_wake));  // Wake at absolute time
    }
}

// Get uptime
void zephyr_timing_example(void) {
    int64_t start = k_uptime_get();  // milliseconds

    do_something();

    int64_t elapsed = k_uptime_get() - start;
    printk("Elapsed: %lld ms\n", elapsed);
}
*/

/*
 * ============================================================================
 * TIMING JITTER AND ACCURACY
 * ============================================================================
 *
 * JITTER: Variation in timing from ideal
 *
 * Sources of jitter:
 * 1. Tick resolution (can't be more precise than 1 tick)
 * 2. Higher priority tasks preempting
 * 3. ISR execution time
 * 4. Critical sections disabling interrupts
 *
 *
 * TICK PHASE:
 *
 *   Ideal 10ms delay:
 *
 *   Tick:   0         1         2         3         4
 *           |---------|---------|---------|---------|
 *               ^                             ^
 *               |                             |
 *           Call vTaskDelay(3)           Task wakes (3 ticks later)
 *           0.3ms into tick              Could be 2.7ms to 3.7ms actual!
 *
 *   Actual delay: Between (N-1)*tick_period and N*tick_period
 *   For vTaskDelay(3) with 1ms tick: 2ms to 3ms actual delay
 *
 *
 * WORST CASE JITTER:
 *
 *   Task calls vTaskDelay(1):
 *
 *   Best case:  Called right after tick → almost full tick delay
 *   Worst case: Called right before tick → almost no delay!
 *
 *   Solution: For 1ms minimum delay, call vTaskDelay(2)
 *
 * ============================================================================
 * TICKLESS IDLE (Low Power)
 * ============================================================================
 *
 * Normal operation:
 * - Tick ISR runs every tick (e.g., every 1ms)
 * - Even if all tasks blocked, tick ISR still runs
 * - Wastes power!
 *
 * Tickless idle:
 * - When all tasks blocked, calculate next wake time
 * - Stop tick timer, enter low power mode
 * - Set wake timer for when next task needs to run
 * - Wake up, update tick count, resume
 *
 *
 * VISUALIZATION:
 *
 *   Normal:
 *   Tick: ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑  ↑
 *   CPU:  ▄__▄__▄__▄__▄__▄__▄__▄__▄__▄__▄__▄__▄__▄__▄__▄__
 *         Wake up every tick even if nothing to do!
 *
 *   Tickless:
 *   Tick: ↑        ↑                    ↑
 *   CPU:  ▄________▄____________________▄________________
 *         Only wake when needed!
 *
 *
 * FreeRTOS Configuration:
 *   #define configUSE_TICKLESS_IDLE    1
 *
 * Zephyr Configuration:
 *   CONFIG_PM=y
 *   CONFIG_PM_DEVICE=y
 *
 * ============================================================================
 * SOFTWARE TIMERS
 * ============================================================================
 *
 * One-shot timer: Fires once after delay
 * Auto-reload timer: Fires repeatedly at interval
 */

/*
// FreeRTOS Timer Example

TimerHandle_t my_timer;

// Timer callback (runs in timer task context, NOT ISR)
void timer_callback(TimerHandle_t xTimer) {
    printf("Timer fired!\n");
    // Can call most FreeRTOS APIs here (not FromISR versions)
}

void timer_example(void) {
    // Create one-shot timer, 500ms period
    my_timer = xTimerCreate(
        "MyTimer",                  // Name
        pdMS_TO_TICKS(500),        // Period
        pdFALSE,                   // One-shot (pdTRUE for auto-reload)
        NULL,                      // Timer ID
        timer_callback             // Callback function
    );

    // Start timer
    xTimerStart(my_timer, 0);

    // Change period
    xTimerChangePeriod(my_timer, pdMS_TO_TICKS(1000), 0);

    // Stop timer
    xTimerStop(my_timer, 0);

    // Delete timer
    xTimerDelete(my_timer, 0);
}
*/

/*
// Zephyr Timer Example

void timer_handler(struct k_timer *timer) {
    printk("Timer fired!\n");
}

K_TIMER_DEFINE(my_timer, timer_handler, NULL);

void zephyr_timer_example(void) {
    // Start one-shot timer, 500ms delay
    k_timer_start(&my_timer, K_MSEC(500), K_NO_WAIT);

    // Start periodic timer, 100ms initial delay, 200ms period
    k_timer_start(&my_timer, K_MSEC(100), K_MSEC(200));

    // Stop timer
    k_timer_stop(&my_timer);
}
*/

/*
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. KNOW TYPICAL VALUES
 *    "Tick rate is typically 100Hz to 1000Hz, with 1000Hz being common"
 *
 * 2. EXPLAIN RESOLUTION TRADEOFF
 *    "Higher tick rate = better resolution but more CPU overhead"
 *
 * 3. KNOW THE DIFFERENCE
 *    "vTaskDelay delays FROM NOW, vTaskDelayUntil delays TO specific time"
 *
 * 4. MENTION TICKLESS
 *    "For battery-powered devices, tickless idle saves power"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "What tick rate would you choose for a battery-powered sensor?"
 * A: Lower rate (100Hz) to reduce overhead and power consumption.
 *    Use tickless idle mode. Accept coarser timing resolution.
 *
 * Q: "How do you measure elapsed time in ticks?"
 * A: TickType_t start = xTaskGetTickCount();
 *    // ... work ...
 *    TickType_t elapsed = xTaskGetTickCount() - start;
 *
 * Q: "What if you need sub-millisecond timing?"
 * A: Use hardware timer directly, not RTOS tick. RTOS tick is for
 *    task scheduling, not high-precision timing.
 *
 * Q: "What happens on tick counter overflow?"
 * A: FreeRTOS handles overflow correctly in delay calculations.
 *    TickType_t is typically 32-bit, overflows after ~49 days at 1kHz.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 10 ===\n\n");
    printf("Q: What is Tick Rate? How does timing work?\n\n");
    printf("TICK RATE:\n");
    printf("- Frequency of RTOS system timer interrupt\n");
    printf("- Typically 100Hz to 1000Hz\n");
    printf("- Determines timing RESOLUTION\n\n");
    printf("TRADEOFF:\n");
    printf("- Higher rate: Better precision, more overhead\n");
    printf("- Lower rate: Less overhead, coarser timing\n\n");
    printf("DELAY FUNCTIONS:\n");
    printf("- vTaskDelay(N): Delay N ticks FROM NOW\n");
    printf("- vTaskDelayUntil(): Delay TO specific tick (periodic)\n\n");
    printf("POWER SAVING:\n");
    printf("- Tickless idle: Stop tick timer when all tasks blocked\n");
    printf("- Wake only when needed\n\n");
    printf("CONFIG:\n");
    printf("- FreeRTOS: configTICK_RATE_HZ (default 1000)\n");
    printf("- Zephyr: CONFIG_SYS_CLOCK_TICKS_PER_SEC\n");
    return 0;
}
