/*
 * ============================================================================
 * QUESTION 17: What are Task Notifications? (FreeRTOS Direct-to-Task)
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: Medium | IMPORTANCE: High
 *
 * Task notifications are a FreeRTOS-specific lightweight signaling mechanism.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Task notifications are a FreeRTOS feature where each task has a built-in
 * 32-bit notification value that can be used for lightweight signaling.
 * They're faster and use less RAM than semaphores or event groups because
 * there's no separate kernel object. A notification can act as a binary
 * semaphore, counting semaphore, event group, or mailbox. The limitation is
 * they're task-to-task only (or ISR-to-task) - you can't broadcast to
 * multiple tasks. Use xTaskNotify() to send, xTaskNotifyWait() to receive."
 *
 * ============================================================================
 * VISUALIZATION: Task Notification vs Semaphore
 * ============================================================================
 *
 *   SEMAPHORE (Traditional):
 *
 *   Task A ----+                   +---- Task B
 *              |                   |
 *              v                   v
 *         +---------------------------+
 *         |   Semaphore Object        |
 *         |   (Separate RAM)          |
 *         |   - Count value           |
 *         |   - Wait list             |
 *         |   - etc.                  |
 *         +---------------------------+
 *
 *   Memory: ~80 bytes for semaphore object
 *   Operations: Create, Give, Take, Delete
 *
 *
 *   TASK NOTIFICATION (Direct):
 *
 *   Task A -----------------------> Task B
 *              Direct notify        +-------------+
 *              (no intermediate     | TCB         |
 *               object!)            | ...         |
 *                                   | Notify Val  |  <- Built into TCB!
 *                                   | Notify State|
 *                                   +-------------+
 *
 *   Memory: 0 extra bytes (uses existing TCB)
 *   Operations: Notify, NotifyWait
 *
 * ============================================================================
 * NOTIFICATION ACTIONS
 * ============================================================================
 *
 *   +----------------------------+----------------------------------------+
 *   | Action                     | Effect on Notification Value           |
 *   +----------------------------+----------------------------------------+
 *   | eNoAction                  | No change, just unblock task           |
 *   | eSetBits                   | OR bits into notification value        |
 *   | eIncrement                 | Increment notification value           |
 *   | eSetValueWithOverwrite     | Set to new value (overwrite)           |
 *   | eSetValueWithoutOverwrite  | Set only if previous was read          |
 *   +----------------------------+----------------------------------------+
 *
 *
 *   EQUIVALENTS:
 *
 *   Task Notification          ==    Traditional Mechanism
 *   -----------------------------------------------------------
 *   eIncrement + Wait          ==    Counting Semaphore
 *   eSetBits + WaitBits        ==    Event Group (limited)
 *   eSetValueWithOverwrite     ==    Mailbox (one item)
 *   eNoAction                  ==    Binary Semaphore (signal only)
 *
 * ============================================================================
 * CODE EXAMPLE: Task Notifications
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "task.h"

// ============================================================================
// PATTERN 1: Binary Semaphore Replacement
// ============================================================================

TaskHandle_t receiver_task_handle;

// Sender (or ISR)
void sender_task(void *pvParameters) {
    for (;;) {
        // Do some work...
        do_work();

        // Notify receiver task (like giving a semaphore)
        xTaskNotifyGive(receiver_task_handle);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Receiver
void receiver_task(void *pvParameters) {
    for (;;) {
        // Wait for notification (like taking a semaphore)
        // Blocks until notification received
        ulTaskNotifyTake(
            pdTRUE,           // Clear count on exit (binary semaphore behavior)
            portMAX_DELAY     // Wait forever
        );

        // Notification received - do something
        handle_event();
    }
}


// ============================================================================
// PATTERN 2: Counting Semaphore Replacement
// ============================================================================

void isr_producer(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Each call increments the notification count
    vTaskNotifyGiveFromISR(receiver_task_handle, &xHigherPriorityTaskWoken);
    vTaskNotifyGiveFromISR(receiver_task_handle, &xHigherPriorityTaskWoken);
    vTaskNotifyGiveFromISR(receiver_task_handle, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void counting_receiver_task(void *pvParameters) {
    for (;;) {
        // Returns the count BEFORE clearing
        // pdFALSE = decrement by 1 (counting semaphore behavior)
        uint32_t count = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);

        printf("Received notification, count was: %lu\n", count);
        // Process one item
    }
}


// ============================================================================
// PATTERN 3: Event Bits (Limited Event Group)
// ============================================================================

#define EVENT_BIT_SENSOR    (1 << 0)
#define EVENT_BIT_UART      (1 << 1)
#define EVENT_BIT_TIMER     (1 << 2)
#define EVENT_ALL_BITS      (EVENT_BIT_SENSOR | EVENT_BIT_UART | EVENT_BIT_TIMER)

void sensor_task(void *pvParameters) {
    for (;;) {
        read_sensor();

        // Set specific bit in receiver's notification value
        xTaskNotify(
            receiver_task_handle,
            EVENT_BIT_SENSOR,    // Value to OR
            eSetBits             // OR into notification value
        );

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void event_receiver_task(void *pvParameters) {
    uint32_t notification_value;

    for (;;) {
        // Wait for any bits to be set
        xTaskNotifyWait(
            0x00,              // Don't clear any bits on entry
            EVENT_ALL_BITS,    // Clear these bits on exit
            &notification_value,  // Receives the notification value
            portMAX_DELAY      // Wait forever
        );

        if (notification_value & EVENT_BIT_SENSOR) {
            handle_sensor_event();
        }
        if (notification_value & EVENT_BIT_UART) {
            handle_uart_event();
        }
        if (notification_value & EVENT_BIT_TIMER) {
            handle_timer_event();
        }
    }
}


// ============================================================================
// PATTERN 4: Mailbox (Pass Value)
// ============================================================================

void producer_with_data(void *pvParameters) {
    uint32_t data_to_send = 0;

    for (;;) {
        data_to_send++;

        // Send value directly to task (overwrite previous)
        xTaskNotify(
            receiver_task_handle,
            data_to_send,             // Value to send
            eSetValueWithOverwrite    // Overwrite any pending value
        );

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void mailbox_receiver_task(void *pvParameters) {
    uint32_t received_value;

    for (;;) {
        xTaskNotifyWait(
            0x00,               // Don't clear on entry
            0xFFFFFFFF,         // Clear all on exit
            &received_value,    // Store received value
            portMAX_DELAY
        );

        printf("Received value: %lu\n", received_value);
    }
}


// ============================================================================
// PATTERN 5: From ISR
// ============================================================================

void UART_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Read data
    uint8_t data = UART->DR;

    // Notify task with the data value
    xTaskNotifyFromISR(
        receiver_task_handle,
        data,
        eSetValueWithOverwrite,
        &xHigherPriorityTaskWoken
    );

    // Or just signal (no data)
    // vTaskNotifyGiveFromISR(receiver_task_handle, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
*/

/*
 * ============================================================================
 * TASK NOTIFICATION API SUMMARY
 * ============================================================================
 *
 *   SENDING:
 *   ---------
 *   xTaskNotify(task, value, action)         - General notification
 *   xTaskNotifyGive(task)                    - Simple increment
 *   xTaskNotifyFromISR(...)                  - From ISR
 *   vTaskNotifyGiveFromISR(task, &woken)     - Simple from ISR
 *
 *
 *   RECEIVING:
 *   ----------
 *   ulTaskNotifyTake(clearOnExit, timeout)   - Get count, optionally clear
 *   xTaskNotifyWait(clearOnEntry, clearOnExit, &value, timeout)  - Full control
 *
 *
 *   QUERY:
 *   ------
 *   xTaskNotifyStateClear(task)              - Clear notification state
 *   ulTaskNotifyValueClear(task, bitsToClear) - Clear specific bits
 *
 * ============================================================================
 * PERFORMANCE COMPARISON
 * ============================================================================
 *
 *   +----------------------+------------------+------------------+
 *   | Operation            | Task Notify      | Semaphore        |
 *   +----------------------+------------------+------------------+
 *   | RAM Usage            | 0 bytes extra    | ~80 bytes        |
 *   | Give/Notify cycles   | ~70 cycles       | ~150 cycles      |
 *   | Take/Wait cycles     | ~80 cycles       | ~180 cycles      |
 *   | Create overhead      | None             | Yes              |
 *   | Delete needed        | No               | Yes              |
 *   +----------------------+------------------+------------------+
 *
 *   Task notifications are ~45% faster and use no additional RAM!
 *
 * ============================================================================
 * LIMITATIONS OF TASK NOTIFICATIONS
 * ============================================================================
 *
 * 1. SINGLE RECEIVER
 *    - Only ONE task can wait on its notification
 *    - Cannot broadcast to multiple tasks (use event groups)
 *
 * 2. NO BLOCKING SENDER
 *    - Sender never blocks (unlike queue full)
 *    - Can overwrite pending notification
 *
 * 3. LIMITED TO 32 BITS
 *    - Only 32-bit value (event groups have 24 usable bits anyway)
 *
 * 4. TASK HANDLE REQUIRED
 *    - Sender must know receiver's task handle
 *    - Less decoupled than queue/semaphore
 *
 * ============================================================================
 * WHEN TO USE TASK NOTIFICATIONS
 * ============================================================================
 *
 * USE NOTIFICATIONS:
 * - ISR to single task signaling
 * - Task to task signaling (known receiver)
 * - When RAM is very limited
 * - When performance is critical
 * - Simple binary/counting semaphore replacement
 *
 * USE TRADITIONAL PRIMITIVES:
 * - Multiple tasks waiting (broadcast)
 * - Unknown/variable receivers
 * - Complex synchronization patterns
 * - Need to block sender (full queue)
 * - Clean abstraction preferred
 *
 * ============================================================================
 * ZEPHYR EQUIVALENT: k_poll
 * ============================================================================
 *
 * Zephyr doesn't have direct task notifications, but k_poll provides similar
 * lightweight signaling:
 *
 * struct k_poll_signal my_signal;
 *
 * // Initialize
 * k_poll_signal_init(&my_signal);
 *
 * // Signal from anywhere
 * k_poll_signal_raise(&my_signal, result_value);
 *
 * // Wait for signal
 * struct k_poll_event events[] = {
 *     K_POLL_EVENT_INITIALIZER(K_POLL_TYPE_SIGNAL, K_POLL_MODE_NOTIFY_ONLY,
 *                              &my_signal),
 * };
 * k_poll(events, 1, K_FOREVER);
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN THE PERFORMANCE BENEFIT
 *    "Task notifications are 45% faster and use zero additional RAM"
 *
 * 2. KNOW THE LIMITATIONS
 *    "Single receiver only, cannot broadcast"
 *
 * 3. COMPARE TO ALTERNATIVES
 *    "For multi-task signaling use event groups, for queuing use queues"
 *
 * 4. MENTION ISR USAGE
 *    "Excellent for ISR-to-task communication"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "When would you NOT use task notifications?"
 * A: When multiple tasks need to wait for the same event (broadcast),
 *    when you need a queue of items, or when sender should block on full.
 *
 * Q: "What happens if you notify a task that already has a pending notification?"
 * A: Depends on action: eSetValueWithOverwrite overwrites, eSetBits ORs,
 *    eSetValueWithoutOverwrite fails, eIncrement increments.
 *
 * Q: "Can a task notify itself?"
 * A: Yes, using xTaskNotify(NULL, ...) or xTaskGetCurrentTaskHandle().
 *    Useful for deferred processing within same task.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 17 ===\n\n");
    printf("Q: What are Task Notifications?\n\n");
    printf("TASK NOTIFICATIONS:\n");
    printf("- FreeRTOS lightweight signaling mechanism\n");
    printf("- Built into TCB (zero extra RAM)\n");
    printf("- 45%% faster than semaphores\n");
    printf("- 32-bit notification value per task\n\n");
    printf("ACTIONS:\n");
    printf("- eNoAction: Just unblock task\n");
    printf("- eSetBits: OR bits into value\n");
    printf("- eIncrement: Add 1 to value\n");
    printf("- eSetValueWithOverwrite: Set value\n\n");
    printf("CAN REPLACE:\n");
    printf("- Binary semaphore (eNoAction)\n");
    printf("- Counting semaphore (eIncrement)\n");
    printf("- Event group bits (eSetBits)\n");
    printf("- Mailbox (eSetValueWithOverwrite)\n\n");
    printf("LIMITATIONS:\n");
    printf("- Single receiver only\n");
    printf("- Cannot broadcast\n");
    printf("- Sender never blocks\n");
    return 0;
}
