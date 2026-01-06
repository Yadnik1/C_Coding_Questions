/*
 * ============================================================================
 * QUESTION 18: What are Software Timers? How do they work?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: High | IMPORTANCE: High
 *
 * Software timers are essential for timed operations without dedicated tasks.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Software timers allow you to execute a callback function at a set time in
 * the future without dedicating a task to waiting. They run in the context of
 * the RTOS timer daemon task (tmr svc in FreeRTOS), not in an ISR. This means
 * timer callbacks can use most RTOS APIs but shouldn't block. Timers can be
 * one-shot (fire once) or auto-reload (periodic). They're NOT real-time
 * accurate - they depend on tick resolution and timer task priority. For
 * precise timing, use hardware timers instead."
 *
 * ============================================================================
 * VISUALIZATION: Timer Architecture
 * ============================================================================
 *
 *   FREERTOS TIMER SYSTEM:
 *
 *   +-----------------+    Timer Commands    +------------------+
 *   | Application     | ------------------> | Timer Command    |
 *   | Tasks           |   (via queue)        | Queue            |
 *   +-----------------+                      +------------------+
 *                                                    |
 *                                                    v
 *                                            +------------------+
 *                                            | Timer Daemon     |
 *                                            | Task (tmr svc)   |
 *                                            | Priority: config |
 *                                            +------------------+
 *                                                    |
 *              +-------------------+-----------------+------------------+
 *              |                   |                 |                  |
 *              v                   v                 v                  v
 *        +-----------+      +-----------+     +-----------+      +-----------+
 *        | Timer 1   |      | Timer 2   |     | Timer 3   |      | Timer N   |
 *        | One-shot  |      | Periodic  |     | One-shot  |      | Periodic  |
 *        | 100ms     |      | 50ms      |     | 500ms     |      | 1000ms    |
 *        +-----------+      +-----------+     +-----------+      +-----------+
 *
 *
 *   TIMER LIST (Sorted by expiry time):
 *
 *   Now: 1000 ticks
 *
 *   +--------+    +--------+    +--------+    +--------+
 *   |Timer B | -> |Timer A | -> |Timer C | -> |Timer D | -> NULL
 *   |Exp:1050|    |Exp:1100|    |Exp:1200|    |Exp:1500|
 *   +--------+    +--------+    +--------+    +--------+
 *       ^
 *       |
 *   Next to expire
 *
 * ============================================================================
 * ONE-SHOT vs AUTO-RELOAD
 * ============================================================================
 *
 *   ONE-SHOT TIMER:
 *   ---------------
 *
 *   Start         Expire
 *     |             |
 *     v             v
 *   --+-------------+---------------------------------->
 *     |<-- Period ->|
 *                   |
 *                Callback()
 *                   |
 *                 DONE (timer stops)
 *
 *
 *   AUTO-RELOAD (Periodic) TIMER:
 *   ----------------------------
 *
 *   Start    Expire     Expire     Expire     Expire
 *     |        |          |          |          |
 *     v        v          v          v          v
 *   --+--------+----------+----------+----------+----->
 *     |<-Period->|<-Period->|<-Period->|<-Period->|
 *              |          |          |          |
 *           Callback() Callback() Callback() Callback()
 *              |          |          |          |
 *           Continues until explicitly stopped...
 *
 * ============================================================================
 * CODE EXAMPLE: FreeRTOS Software Timers
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

// ============================================================================
// BASIC TIMER USAGE
// ============================================================================

TimerHandle_t my_timer;

// Timer callback function
// Runs in timer daemon task context (NOT ISR!)
void my_timer_callback(TimerHandle_t xTimer) {
    // Get timer ID if needed
    uint32_t timer_id = (uint32_t)pvTimerGetTimerID(xTimer);

    printf("Timer %lu fired!\n", timer_id);

    // Can use most FreeRTOS APIs here
    // But don't block for long - blocks other timers!

    // Example: Signal a task
    xSemaphoreGive(my_semaphore);

    // Example: Send to queue
    xQueueSend(my_queue, &data, 0);  // Don't block (0 timeout)
}

void create_timer_example(void) {
    // Create a one-shot timer (fires once)
    my_timer = xTimerCreate(
        "MyTimer",                  // Timer name (for debugging)
        pdMS_TO_TICKS(500),         // Period: 500ms
        pdFALSE,                    // One-shot (pdTRUE for auto-reload)
        (void *)1,                  // Timer ID (user data)
        my_timer_callback           // Callback function
    );

    if (my_timer == NULL) {
        // Timer creation failed (out of memory)
        printf("Failed to create timer!\n");
        return;
    }

    // Start the timer
    // Block up to 100ms if command queue is full
    if (xTimerStart(my_timer, pdMS_TO_TICKS(100)) != pdPASS) {
        printf("Failed to start timer!\n");
    }
}


// ============================================================================
// PERIODIC TIMER (Auto-reload)
// ============================================================================

TimerHandle_t periodic_timer;

void periodic_callback(TimerHandle_t xTimer) {
    static uint32_t count = 0;
    count++;
    printf("Periodic timer: count = %lu\n", count);

    // This will be called every 100ms until stopped
}

void create_periodic_timer(void) {
    periodic_timer = xTimerCreate(
        "PeriodicTimer",
        pdMS_TO_TICKS(100),         // 100ms period
        pdTRUE,                     // AUTO-RELOAD (periodic)
        NULL,
        periodic_callback
    );

    xTimerStart(periodic_timer, 0);
}


// ============================================================================
// TIMER CONTROL OPERATIONS
// ============================================================================

void timer_operations(void) {
    // Stop timer
    xTimerStop(my_timer, pdMS_TO_TICKS(100));

    // Reset timer (restart period from now)
    xTimerReset(my_timer, pdMS_TO_TICKS(100));

    // Change period (and optionally start)
    xTimerChangePeriod(my_timer, pdMS_TO_TICKS(200), pdMS_TO_TICKS(100));

    // Delete timer (free resources)
    xTimerDelete(my_timer, pdMS_TO_TICKS(100));

    // Get remaining time until expiry
    TickType_t remaining = xTimerGetExpiryTime(my_timer) - xTaskGetTickCount();

    // Check if timer is active
    if (xTimerIsTimerActive(my_timer) == pdTRUE) {
        printf("Timer is running\n");
    }
}


// ============================================================================
// TIMER FROM ISR
// ============================================================================

void GPIO_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Clear interrupt
    GPIO_ClearInterrupt();

    // Reset debounce timer from ISR
    xTimerResetFromISR(debounce_timer, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


// ============================================================================
// PRACTICAL EXAMPLE: Button Debounce
// ============================================================================

TimerHandle_t debounce_timer;
volatile uint8_t button_pressed = 0;

void debounce_callback(TimerHandle_t xTimer) {
    // Timer expired - button press is valid
    if (GPIO_ReadPin(BUTTON_PIN) == 0) {  // Still pressed
        button_pressed = 1;
        xSemaphoreGive(button_semaphore);
    }
}

void button_isr(void) {
    BaseType_t woken = pdFALSE;

    // Each press resets the debounce timer
    // Only fires if no activity for 50ms
    xTimerResetFromISR(debounce_timer, &woken);

    portYIELD_FROM_ISR(woken);
}

void init_debounce(void) {
    debounce_timer = xTimerCreate(
        "Debounce",
        pdMS_TO_TICKS(50),          // 50ms debounce time
        pdFALSE,                    // One-shot
        NULL,
        debounce_callback
    );
}


// ============================================================================
// PRACTICAL EXAMPLE: Watchdog-like Timeout
// ============================================================================

TimerHandle_t comm_timeout_timer;

void comm_timeout_callback(TimerHandle_t xTimer) {
    printf("Communication timeout! No data received.\n");
    handle_comm_failure();
}

void data_received_handler(void) {
    // Reset timer every time data arrives
    // If no data for 5 seconds, timeout callback fires
    xTimerReset(comm_timeout_timer, 0);
}

void init_comm_timeout(void) {
    comm_timeout_timer = xTimerCreate(
        "CommTimeout",
        pdMS_TO_TICKS(5000),        // 5 second timeout
        pdFALSE,                    // One-shot
        NULL,
        comm_timeout_callback
    );

    xTimerStart(comm_timeout_timer, 0);
}
*/

/*
 * ============================================================================
 * ZEPHYR SOFTWARE TIMERS
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>

// ============================================================================
// ZEPHYR TIMER - Static Definition
// ============================================================================

void my_timer_expiry(struct k_timer *timer) {
    // Expiry function - runs in ISR context!
    // Keep it short, use k_work for longer processing
    printk("Timer expired!\n");
}

void my_timer_stop(struct k_timer *timer) {
    // Optional: Called when timer is stopped
    printk("Timer stopped\n");
}

// Define timer at compile time
K_TIMER_DEFINE(my_zephyr_timer, my_timer_expiry, my_timer_stop);

void use_zephyr_timer(void) {
    // Start one-shot timer (100ms delay, no period)
    k_timer_start(&my_zephyr_timer, K_MSEC(100), K_NO_WAIT);

    // Start periodic timer (50ms initial delay, 100ms period)
    k_timer_start(&my_zephyr_timer, K_MSEC(50), K_MSEC(100));

    // Stop timer
    k_timer_stop(&my_zephyr_timer);

    // Get remaining time
    uint32_t remaining = k_timer_remaining_get(&my_zephyr_timer);

    // Get status (number of times expired since last read)
    uint32_t status = k_timer_status_get(&my_zephyr_timer);

    // Synchronously wait for timer
    k_timer_status_sync(&my_zephyr_timer);
}


// ============================================================================
// ZEPHYR WORK QUEUE (Better for complex processing)
// ============================================================================

// For complex timer callbacks, use work queue
struct k_work my_work;

void work_handler(struct k_work *work) {
    // This runs in system work queue thread (not ISR)
    // Can do longer processing here
    do_complex_processing();
}

void timer_expiry_submit_work(struct k_timer *timer) {
    // Timer expiry (ISR context) - submit work
    k_work_submit(&my_work);
}

K_TIMER_DEFINE(work_timer, timer_expiry_submit_work, NULL);

void init_work_timer(void) {
    k_work_init(&my_work, work_handler);
    k_timer_start(&work_timer, K_MSEC(100), K_MSEC(100));
}
*/

/*
 * ============================================================================
 * FREERTOS vs ZEPHYR TIMER COMPARISON
 * ============================================================================
 *
 *   +------------------------+-------------------------+------------------------+
 *   | Aspect                 | FreeRTOS                | Zephyr                 |
 *   +------------------------+-------------------------+------------------------+
 *   | Callback context       | Timer daemon TASK       | ISR context!           |
 *   | Blocking in callback   | Avoid but possible      | NEVER (ISR!)           |
 *   | Complex processing     | In callback (carefully) | Use k_work             |
 *   | Configuration          | configUSE_TIMERS        | CONFIG_KERNEL_TIMERS   |
 *   | Create API             | xTimerCreate()          | K_TIMER_DEFINE         |
 *   | Start API              | xTimerStart()           | k_timer_start()        |
 *   +------------------------+-------------------------+------------------------+
 *
 *   KEY DIFFERENCE:
 *   - FreeRTOS: Callback runs in daemon task (can use most APIs)
 *   - Zephyr: Callback runs in ISR context (very limited APIs!)
 *
 * ============================================================================
 * TIMER DAEMON TASK CONFIGURATION (FreeRTOS)
 * ============================================================================
 *
 *   In FreeRTOSConfig.h:
 *
 *   #define configUSE_TIMERS                1    // Enable timers
 *   #define configTIMER_TASK_PRIORITY       2    // Timer task priority
 *   #define configTIMER_QUEUE_LENGTH        10   // Command queue size
 *   #define configTIMER_TASK_STACK_DEPTH    256  // Timer task stack
 *
 *
 *   PRIORITY CONSIDERATION:
 *
 *   If timer task priority is LOW:
 *   - Timers may not fire on time if higher priority tasks are running
 *   - Good for non-critical timeouts
 *
 *   If timer task priority is HIGH:
 *   - Timers fire more accurately
 *   - But may delay other tasks
 *   - Timer callbacks should be SHORT
 *
 * ============================================================================
 * ACCURACY AND JITTER
 * ============================================================================
 *
 *   SOFTWARE TIMER ACCURACY DEPENDS ON:
 *
 *   1. Tick resolution (configTICK_RATE_HZ)
 *      - 1000Hz tick = 1ms resolution
 *      - Can't be more accurate than 1 tick
 *
 *   2. Timer task priority
 *      - Higher priority = less delay
 *
 *   3. Timer callback duration
 *      - Long callback delays other timers
 *
 *   4. System load
 *      - High CPU usage affects timing
 *
 *
 *   EXAMPLE JITTER:
 *
 *   Requested: 100ms timer
 *   Tick: 1ms
 *
 *   Actual timing:  |---99ms---|---101ms---|---100ms---|---102ms---|
 *                            +/-2ms jitter typical
 *
 *   For PRECISE timing, use HARDWARE TIMERS directly!
 *
 * ============================================================================
 * COMMON USE CASES
 * ============================================================================
 *
 * 1. BUTTON DEBOUNCING
 *    - One-shot timer resets on each press
 *    - Only fires if stable for N ms
 *
 * 2. COMMUNICATION TIMEOUTS
 *    - Reset on each received packet
 *    - Fires if no data for N seconds
 *
 * 3. PERIODIC POLLING
 *    - Check sensor every N ms
 *    - Update display every N ms
 *
 * 4. LED BLINKING
 *    - Toggle LED every N ms
 *
 * 5. DELAYED OPERATIONS
 *    - Turn off backlight after N seconds of inactivity
 *
 * 6. RETRY MECHANISMS
 *    - Retry failed operation after N ms
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. KNOW THE CONTEXT
 *    "FreeRTOS callbacks run in timer task, Zephyr in ISR context"
 *
 * 2. DISCUSS ACCURACY LIMITATIONS
 *    "Software timers aren't real-time accurate, limited by tick resolution"
 *
 * 3. MENTION COMMON USES
 *    "Debouncing, timeouts, periodic polling"
 *
 * 4. CALLBACK BEST PRACTICES
 *    "Keep callbacks short, don't block, use semaphores for longer work"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "What's the minimum timer period?"
 * A: One tick period (e.g., 1ms at 1000Hz). Setting period to 0 is invalid.
 *
 * Q: "Can timer callbacks block?"
 * A: In FreeRTOS, avoid but technically possible (blocks other timers).
 *    In Zephyr, NEVER - it's ISR context.
 *
 * Q: "How many timers can you have?"
 * A: Limited by available RAM for timer structures and command queue depth.
 *    Each FreeRTOS timer is ~40 bytes.
 *
 * Q: "What happens if timer fires while callback is still running?"
 * A: Timer daemon handles one callback at a time. If a periodic timer's
 *    callback takes longer than the period, callbacks queue up.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 18 ===\n\n");
    printf("Q: What are Software Timers?\n\n");
    printf("SOFTWARE TIMERS:\n");
    printf("- Execute callback after specified time\n");
    printf("- No dedicated task needed\n");
    printf("- One-shot or auto-reload (periodic)\n\n");
    printf("FREERTOS:\n");
    printf("- xTimerCreate(), xTimerStart(), xTimerStop()\n");
    printf("- Callbacks run in timer daemon TASK\n");
    printf("- Can use most FreeRTOS APIs\n\n");
    printf("ZEPHYR:\n");
    printf("- K_TIMER_DEFINE(), k_timer_start()\n");
    printf("- Callbacks run in ISR context!\n");
    printf("- Use k_work for complex processing\n\n");
    printf("ACCURACY:\n");
    printf("- Limited by tick resolution\n");
    printf("- Not suitable for precise timing\n");
    printf("- Use hardware timers for accuracy\n\n");
    printf("USE CASES:\n");
    printf("- Button debouncing\n");
    printf("- Communication timeouts\n");
    printf("- Periodic polling\n");
    printf("- LED blinking\n");
    return 0;
}
