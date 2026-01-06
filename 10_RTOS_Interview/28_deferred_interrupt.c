/*
 * ============================================================================
 * QUESTION 28: What is Deferred Interrupt Processing? Why use it?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * Deferred interrupt handling is a fundamental RTOS design pattern.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Deferred interrupt processing means keeping ISR code minimal - just
 * acknowledge the interrupt, save critical data, and signal a task to do
 * the actual work. The ISR takes microseconds, the task does the heavy
 * processing. Benefits: (1) Lower interrupt latency for other interrupts,
 * (2) Task can use all RTOS APIs including blocking, (3) Easier to debug
 * and test, (4) Priority can be managed by scheduler. In FreeRTOS, use
 * xSemaphoreGiveFromISR or xQueueSendFromISR to signal the task."
 *
 * ============================================================================
 * VISUALIZATION: Deferred vs Direct Processing
 * ============================================================================
 *
 *   DIRECT PROCESSING IN ISR (BAD for complex work):
 *
 *   Interrupt ↓
 *   Time:     |=====================================|
 *             |  ISR does ALL work (long!)          |
 *             |  - Read data                        |
 *             |  - Parse protocol                   |
 *             |  - Calculate response               |
 *             |  - Send response                    |
 *             |=====================================|
 *
 *   During this time:
 *   - All lower-priority interrupts blocked
 *   - Scheduler cannot run
 *   - Other tasks starved
 *
 *
 *   DEFERRED PROCESSING (GOOD):
 *
 *   Interrupt ↓
 *   Time:     |===|                     |====================|
 *             |ISR|                     | Task processing    |
 *             |   |                     |                    |
 *             |===|                     |====================|
 *               |                            ^
 *               |  Signal (semaphore)        |
 *               +-----> Task wakes ----------+
 *
 *   ISR: ~10us (minimal)
 *   Task: Can take longer, preemptible, uses RTOS APIs
 *
 *
 *   TIMING COMPARISON:
 *
 *   Direct (50us ISR):
 *   IRQ1 |████████████████████████████████████████|
 *   IRQ2            ↓ Blocked until IRQ1 done!
 *        |          [DELAYED]                      |████|
 *
 *   Deferred (5us ISR):
 *   IRQ1 |████|
 *   IRQ2      |████|  <- Minimal delay
 *   Task1         |██████████████████|
 *   Task2                  |███████|  <- Can interleave
 *
 * ============================================================================
 * THE PATTERN
 * ============================================================================
 *
 *   +-------------------+
 *   | Interrupt Handler |
 *   +-------------------+
 *           |
 *           | 1. Clear interrupt flag
 *           | 2. Read hardware (save data)
 *           | 3. Signal task (semaphore/queue)
 *           | 4. Request context switch if needed
 *           v
 *   +-------------------+
 *   | Handler Task      |
 *   +-------------------+
 *           |
 *           | 1. Wait for signal
 *           | 2. Process data
 *           | 3. Take actions (can block!)
 *           | 4. Loop back to wait
 *           v
 *
 * ============================================================================
 * CODE EXAMPLE: Deferred Interrupt Processing
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// ============================================================================
// PATTERN 1: Binary Semaphore Signaling
// ============================================================================

SemaphoreHandle_t uart_rx_sem;

// ISR - Minimal work
void UART_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // 1. Clear interrupt source
    uint32_t status = UART->SR;
    UART->SR = 0;  // Clear flags

    // 2. Signal task (very fast)
    xSemaphoreGiveFromISR(uart_rx_sem, &xHigherPriorityTaskWoken);

    // 3. Request context switch if needed
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Handler Task - Heavy processing
void uart_handler_task(void *pvParameters) {
    for (;;) {
        // Wait for interrupt signal
        xSemaphoreTake(uart_rx_sem, portMAX_DELAY);

        // Now do all the heavy work
        // Can use any RTOS API, can block, can take time
        uint8_t buffer[256];
        int len = uart_read_buffer(buffer, sizeof(buffer));

        if (len > 0) {
            // Parse protocol (complex, variable time)
            Message_t msg;
            if (parse_protocol(buffer, len, &msg) == 0) {
                // Handle message (may involve blocking APIs)
                handle_message(&msg);
            }
        }
    }
}


// ============================================================================
// PATTERN 2: Queue with Data
// ============================================================================

#define RX_QUEUE_SIZE 64

typedef struct {
    uint8_t data;
    uint8_t error_flags;
} UartRxItem_t;

QueueHandle_t uart_rx_queue;

// ISR - Save data and signal
void UART_IRQHandler_Queue(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    UartRxItem_t item;

    // Read all available data
    while (UART->SR & UART_SR_RXNE) {
        item.data = UART->DR;
        item.error_flags = (UART->SR & UART_SR_ERRORS);

        // Send to queue (non-blocking, drop if full)
        xQueueSendFromISR(uart_rx_queue, &item, &xHigherPriorityTaskWoken);
    }

    // Clear any errors
    UART->SR = 0;

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Handler Task
void uart_handler_task_queue(void *pvParameters) {
    UartRxItem_t item;
    uint8_t message_buffer[256];
    int index = 0;

    for (;;) {
        // Wait for data from ISR
        if (xQueueReceive(uart_rx_queue, &item, portMAX_DELAY) == pdPASS) {
            // Check for errors
            if (item.error_flags) {
                handle_uart_error(item.error_flags);
                index = 0;  // Reset on error
                continue;
            }

            // Build message
            message_buffer[index++] = item.data;

            // Check for end of message
            if (item.data == '\n' || index >= sizeof(message_buffer)) {
                // Process complete message
                process_message(message_buffer, index);
                index = 0;
            }
        }
    }
}


// ============================================================================
// PATTERN 3: Task Notification (Most Efficient)
// ============================================================================

TaskHandle_t uart_task_handle;

// ISR using task notification
void UART_IRQHandler_Notify(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Clear interrupt
    UART->SR = 0;

    // Notify task directly (fastest method!)
    vTaskNotifyGiveFromISR(uart_task_handle, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_handler_task_notify(void *pvParameters) {
    for (;;) {
        // Wait for notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Process
        process_uart_event();
    }
}


// ============================================================================
// PATTERN 4: Event Group for Multiple Events
// ============================================================================

#define EVENT_UART_RX   (1 << 0)
#define EVENT_UART_TX   (1 << 1)
#define EVENT_UART_ERR  (1 << 2)
#define EVENT_ALL       (EVENT_UART_RX | EVENT_UART_TX | EVENT_UART_ERR)

EventGroupHandle_t uart_events;

void UART_IRQHandler_Events(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EventBits_t bits_to_set = 0;

    uint32_t status = UART->SR;

    if (status & UART_SR_RXNE) bits_to_set |= EVENT_UART_RX;
    if (status & UART_SR_TC)   bits_to_set |= EVENT_UART_TX;
    if (status & UART_SR_ERR)  bits_to_set |= EVENT_UART_ERR;

    UART->SR = 0;

    xEventGroupSetBitsFromISR(uart_events, bits_to_set, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_handler_task_events(void *pvParameters) {
    EventBits_t bits;

    for (;;) {
        bits = xEventGroupWaitBits(uart_events, EVENT_ALL,
                                    pdTRUE, pdFALSE, portMAX_DELAY);

        if (bits & EVENT_UART_RX) handle_rx();
        if (bits & EVENT_UART_TX) handle_tx_complete();
        if (bits & EVENT_UART_ERR) handle_error();
    }
}
*/

/*
 * ============================================================================
 * ZEPHYR DEFERRED INTERRUPT
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>

// ============================================================================
// Using Semaphore
// ============================================================================

K_SEM_DEFINE(uart_sem, 0, 1);

void uart_isr(const struct device *dev, void *user_data) {
    // Minimal ISR work
    uart_irq_update(dev);

    if (uart_irq_rx_ready(dev)) {
        k_sem_give(&uart_sem);
    }
}

void uart_thread(void *p1, void *p2, void *p3) {
    while (1) {
        k_sem_take(&uart_sem, K_FOREVER);

        // Heavy processing
        uint8_t data;
        while (uart_fifo_read(dev, &data, 1) > 0) {
            process_byte(data);
        }
    }
}


// ============================================================================
// Using Work Queue (Recommended for Zephyr)
// ============================================================================

struct k_work uart_work;

void uart_work_handler(struct k_work *work) {
    // Runs in system work queue thread
    // Can use blocking APIs
    process_uart_data();
}

void uart_isr_work(const struct device *dev, void *user_data) {
    uart_irq_update(dev);

    if (uart_irq_rx_ready(dev)) {
        // Submit work item to system work queue
        k_work_submit(&uart_work);
    }
}

void init_deferred(void) {
    k_work_init(&uart_work, uart_work_handler);
}


// ============================================================================
// Using Delayed Work (for non-urgent processing)
// ============================================================================

struct k_work_delayable debounce_work;

void gpio_isr(const struct device *dev, struct gpio_callback *cb,
              gpio_port_pins_t pins) {
    // Schedule work with delay (debounce)
    k_work_reschedule(&debounce_work, K_MSEC(50));
}

void debounce_handler(struct k_work *work) {
    // Called 50ms after last interrupt
    handle_button_press();
}
*/

/*
 * ============================================================================
 * COMPARISON: ISR METHODS
 * ============================================================================
 *
 *   +-------------------+-----------+----------+------------------------+
 *   | Method            | Overhead  | Latency  | Use Case               |
 *   +-------------------+-----------+----------+------------------------+
 *   | Task Notification | Lowest    | Lowest   | Simple signal, 1 task  |
 *   | Binary Semaphore  | Low       | Low      | Simple signal          |
 *   | Queue             | Medium    | Low      | Pass data to task      |
 *   | Event Group       | Medium    | Low      | Multiple event types   |
 *   | Stream Buffer     | Higher    | Low      | Continuous byte stream |
 *   +-------------------+-----------+----------+------------------------+
 *
 * ============================================================================
 * WHEN TO USE DEFERRED PROCESSING
 * ============================================================================
 *
 * ALWAYS DEFER WHEN:
 * - Processing takes > 10-20 microseconds
 * - Need to call blocking RTOS APIs
 * - Complex protocol parsing
 * - State machine processing
 * - Flash/EEPROM writes
 * - Logging or debug output
 *
 * OK TO PROCESS IN ISR:
 * - Simple flag set
 * - Single register read/write
 * - Fast hardware acknowledgment
 * - DMA setup for short transfer
 * - Time-critical bit manipulation
 *
 * ============================================================================
 * HANDLER TASK PRIORITY
 * ============================================================================
 *
 * HIGH PRIORITY handler:
 * + Fast response to interrupt
 * + Minimal data loss
 * - May delay other important tasks
 *
 * MEDIUM PRIORITY handler:
 * + Balanced responsiveness
 * + Other tasks can interleave
 * - Potential for queue buildup
 *
 * Rule of thumb:
 * - Time-critical peripherals: High priority
 * - Logging, status: Low priority
 * - Communication: Medium-High priority
 *
 * ============================================================================
 * COMMON MISTAKES
 * ============================================================================
 *
 * MISTAKE 1: Too much work in ISR
 *
 *   void UART_IRQHandler(void) {
 *       // BAD: Complex parsing in ISR!
 *       parse_json(uart_buffer, 1024);
 *       update_database(parsed_data);
 *       send_http_response();
 *   }
 *
 *
 * MISTAKE 2: Blocking in ISR
 *
 *   void UART_IRQHandler(void) {
 *       // CRASH: Can't take semaphore in ISR!
 *       xSemaphoreTake(mutex, portMAX_DELAY);
 *   }
 *
 *
 * MISTAKE 3: Forgetting xHigherPriorityTaskWoken
 *
 *   void UART_IRQHandler(void) {
 *       xSemaphoreGiveFromISR(sem, NULL);  // Works but...
 *       // Task won't switch immediately!
 *   }
 *
 *   // Correct:
 *   void UART_IRQHandler(void) {
 *       BaseType_t wake = pdFALSE;
 *       xSemaphoreGiveFromISR(sem, &wake);
 *       portYIELD_FROM_ISR(wake);  // Immediate switch!
 *   }
 *
 *
 * MISTAKE 4: Handler task too low priority
 *
 *   // ISR fast but task never runs because always preempted
 *   // Result: Queue fills up, data lost
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN THE WHY
 *    "Keep ISR short to maintain interrupt responsiveness"
 *
 * 2. DESCRIBE THE PATTERN
 *    "ISR signals task, task does heavy work"
 *
 * 3. MENTION SIGNALING OPTIONS
 *    "Semaphore, queue, or task notification"
 *
 * 4. DISCUSS BENEFITS
 *    "Can use blocking APIs, easier debug, priority managed by scheduler"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "How do you decide what to do in ISR vs task?"
 * A: ISR: Only time-critical hardware interaction. Anything that can
 *    wait even 100us goes to task. Rule: ISR < 10-20us.
 *
 * Q: "What if you lose data while task is processing?"
 * A: Use queue with sufficient depth. Monitor queue usage. Consider
 *    DMA for high-speed peripherals. Increase task priority.
 *
 * Q: "Can multiple ISRs signal the same task?"
 * A: Yes, use event group with different bits per ISR. Task checks
 *    which events occurred and handles each.
 *
 * Q: "What's portYIELD_FROM_ISR?"
 * A: Requests context switch at end of ISR if a higher-priority task
 *    became ready. Essential for immediate task response.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 28 ===\n\n");
    printf("Q: What is Deferred Interrupt Processing?\n\n");
    printf("CONCEPT:\n");
    printf("- ISR does minimal work (acknowledge, save data, signal)\n");
    printf("- Task does heavy processing (parsing, blocking, complex logic)\n\n");
    printf("THE PATTERN:\n");
    printf("  1. ISR clears interrupt flag\n");
    printf("  2. ISR saves critical data (if any)\n");
    printf("  3. ISR signals task (semaphore/queue/notify)\n");
    printf("  4. ISR calls portYIELD_FROM_ISR()\n");
    printf("  5. Task wakes and processes\n\n");
    printf("SIGNALING OPTIONS:\n");
    printf("- Task Notification: Fastest, simplest\n");
    printf("- Binary Semaphore: Simple signal\n");
    printf("- Queue: Pass data to task\n");
    printf("- Event Group: Multiple event types\n\n");
    printf("BENEFITS:\n");
    printf("- Lower interrupt latency\n");
    printf("- Can use blocking RTOS APIs in task\n");
    printf("- Easier to debug and test\n");
    printf("- Priority managed by scheduler\n");
    return 0;
}
