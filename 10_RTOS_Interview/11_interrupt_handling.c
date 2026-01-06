/*
 * ============================================================================
 * QUESTION 11: How do you handle Interrupts in an RTOS?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: VERY HIGH | IMPORTANCE: CRITICAL
 *
 * ISR handling is fundamental to embedded systems. This is asked in almost
 * every embedded interview. You must know the deferred interrupt pattern!
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "In RTOS, interrupt handlers should be SHORT - typically just acknowledge
 * the hardware and signal a task to do the actual work. This is called
 * DEFERRED INTERRUPT HANDLING. The ISR uses 'FromISR' API variants like
 * xSemaphoreGiveFromISR() or xQueueSendFromISR() to wake a task. These APIs
 * never block and return whether a context switch is needed. At the end of
 * ISR, we call portYIELD_FROM_ISR() if a higher priority task was woken.
 * This keeps ISR latency low and allows complex processing in task context."
 *
 * ============================================================================
 * THE GOLDEN RULE
 * ============================================================================
 *
 *   ╔═══════════════════════════════════════════════════════════════════╗
 *   ║  ISR: FAST, SIMPLE, NON-BLOCKING                                  ║
 *   ║  TASK: COMPLEX PROCESSING, CAN BLOCK                              ║
 *   ╚═══════════════════════════════════════════════════════════════════╝
 *
 * ============================================================================
 * VISUALIZATION: Deferred Interrupt Handling
 * ============================================================================
 *
 * BAD: Everything in ISR (Long ISR)
 *
 *   Time --->
 *   ┌─────────────────────────────────────────────┐
 *   │                    ISR                       │
 *   │ [Read HW][Process][Calculate][Store][Reply] │  OTHER INTERRUPTS DELAYED!
 *   └─────────────────────────────────────────────┘
 *                    ^
 *                    |
 *            Long ISR = BAD
 *            - Blocks other interrupts
 *            - Increases latency
 *            - Can cause missed deadlines
 *
 *
 * GOOD: Deferred Processing (Short ISR)
 *
 *   Time --->
 *   ┌────────────┐
 *   │    ISR     │
 *   │ [Read][Sig]│---> Signal task
 *   └────────────┘
 *         |
 *         v
 *   ┌──────────────────────────────────────────┐
 *   │              Handler Task                 │
 *   │ [Process][Calculate][Store][Reply]       │  Runs when scheduled
 *   └──────────────────────────────────────────┘
 *
 *   Short ISR = GOOD
 *   - Other interrupts can fire
 *   - Low latency
 *   - Complex work in task context
 *
 * ============================================================================
 * RULES FOR ISR IN RTOS
 * ============================================================================
 *
 * 1. KEEP IT SHORT
 *    - Acknowledge hardware
 *    - Read essential data
 *    - Signal task
 *    - Return
 *
 * 2. USE "FromISR" API VARIANTS
 *    WRONG: xSemaphoreGive(sem)        // Can cause context switch issues!
 *    RIGHT: xSemaphoreGiveFromISR(sem, &pxHigherPriorityTaskWoken)
 *
 * 3. NEVER BLOCK IN ISR
 *    NEVER: xSemaphoreTake(sem, portMAX_DELAY)  // ISR will hang!
 *    NEVER: vTaskDelay()                         // Task API in ISR!
 *    NEVER: printf() without care                // May block!
 *
 * 4. CHECK IF CONTEXT SWITCH NEEDED
 *    portYIELD_FROM_ISR(xHigherPriorityTaskWoken)
 *
 * 5. USE CORRECT INTERRUPT PRIORITY
 *    FreeRTOS: Priority must be <= configMAX_SYSCALL_INTERRUPT_PRIORITY
 *    Higher numeric = lower priority on ARM Cortex-M (confusing!)
 *
 * ============================================================================
 * CODE EXAMPLE: FreeRTOS
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// Semaphore for signaling
SemaphoreHandle_t uart_rx_sem;

// Queue for data
QueueHandle_t uart_rx_queue;

// ============================================================================
// PATTERN 1: Binary Semaphore Signaling
// ============================================================================

void UART_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Step 1: Acknowledge interrupt (hardware specific)
    UART_ClearInterrupt();

    // Step 2: Read data (minimal work)
    uint8_t data = UART_ReadByte();

    // Step 3: Store data (simple buffer or queue)
    uart_rx_buffer[uart_rx_head++] = data;

    // Step 4: Signal task using FromISR variant
    xSemaphoreGiveFromISR(uart_rx_sem, &xHigherPriorityTaskWoken);

    // Step 5: Request context switch if needed
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Handler task - does the complex processing
void uart_handler_task(void *pvParameters) {
    for (;;) {
        // Wait for ISR signal
        xSemaphoreTake(uart_rx_sem, portMAX_DELAY);

        // Complex processing here (parsing, protocol handling, etc.)
        process_uart_data();
    }
}


// ============================================================================
// PATTERN 2: Queue Data Transfer
// ============================================================================

typedef struct {
    uint8_t data[16];
    uint8_t length;
    uint32_t timestamp;
} UartPacket;

void UART_DMA_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    UART_ClearDMAInterrupt();

    // Prepare packet
    UartPacket packet;
    packet.length = UART_GetReceivedCount();
    memcpy(packet.data, UART_RX_Buffer, packet.length);
    packet.timestamp = xTaskGetTickCountFromISR();

    // Send to queue (never blocks in ISR)
    xQueueSendFromISR(uart_rx_queue, &packet, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_processor_task(void *pvParameters) {
    UartPacket packet;

    for (;;) {
        // Wait for data from ISR
        if (xQueueReceive(uart_rx_queue, &packet, portMAX_DELAY) == pdTRUE) {
            // Process packet (can take time, use blocking APIs, etc.)
            parse_protocol(&packet);
        }
    }
}


// ============================================================================
// PATTERN 3: Task Notification (Fastest)
// ============================================================================

TaskHandle_t handler_task_handle;

void GPIO_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    GPIO_ClearInterrupt();

    // Direct-to-task notification (faster than semaphore)
    vTaskNotifyGiveFromISR(handler_task_handle, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void gpio_handler_task(void *pvParameters) {
    for (;;) {
        // Wait for notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Handle GPIO event
        handle_button_press();
    }
}
*/

/*
 * ============================================================================
 * CODE EXAMPLE: Zephyr
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>

K_SEM_DEFINE(uart_sem, 0, 1);
K_MSGQ_DEFINE(uart_queue, sizeof(UartPacket), 10, 4);

// Zephyr ISR
void uart_isr(const struct device *dev, void *user_data) {
    if (uart_irq_update(dev) && uart_irq_rx_ready(dev)) {
        uint8_t data;
        uart_fifo_read(dev, &data, 1);

        // Signal semaphore (no FromISR needed in Zephyr - it's automatic!)
        k_sem_give(&uart_sem);

        // Or send to queue
        // k_msgq_put(&uart_queue, &data, K_NO_WAIT);
    }
}

void uart_handler_thread(void *p1, void *p2, void *p3) {
    while (1) {
        k_sem_take(&uart_sem, K_FOREVER);
        process_uart_data();
    }
}

// Setup ISR
void uart_init(void) {
    const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

    uart_irq_callback_set(uart, uart_isr);
    uart_irq_rx_enable(uart);
}
*/

/*
 * ============================================================================
 * INTERRUPT PRIORITY CONFIGURATION
 * ============================================================================
 *
 * ARM Cortex-M Interrupt Priorities:
 *
 *   IMPORTANT: Lower number = HIGHER priority!
 *
 *   Priority 0:  Highest (reserved for fault handlers)
 *   Priority 1:  Very high
 *   ...
 *   Priority 15: Lowest (on 4-bit priority MCU)
 *
 *
 * FreeRTOS configurable priority levels:
 *
 *   +------------------+------------------------------------------+
 *   | Priority Range   | Can Call FreeRTOS?                       |
 *   +------------------+------------------------------------------+
 *   | 0 to (N-1)       | NO - too high, will corrupt RTOS state   |
 *   | N to 15          | YES - configMAX_SYSCALL_INTERRUPT_PRIORITY|
 *   +------------------+------------------------------------------+
 *
 *   Where N = configMAX_SYSCALL_INTERRUPT_PRIORITY (typically 5 or 11)
 *
 *
 * Example FreeRTOSConfig.h:
 *
 *   #define configPRIO_BITS                      4
 *   #define configLIBRARY_LOWEST_INTERRUPT_PRIORITY      15
 *   #define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5
 *
 *   ISRs with priority 0-4: Cannot use FreeRTOS APIs!
 *   ISRs with priority 5-15: Can use FreeRTOS "FromISR" APIs
 *
 *
 * Setting interrupt priority:
 *
 *   NVIC_SetPriority(UART_IRQn, 5);  // Priority 5, can use FreeRTOS
 *   NVIC_SetPriority(FAST_IRQn, 2);  // Priority 2, CANNOT use FreeRTOS!
 *
 * ============================================================================
 * COMMON ISR MISTAKES
 * ============================================================================
 *
 * MISTAKE 1: Blocking in ISR
 *
 *   void BAD_ISR(void) {
 *       xSemaphoreTake(mutex, portMAX_DELAY);  // WRONG! ISR will hang!
 *       // ...
 *       xSemaphoreGive(mutex);
 *   }
 *
 *
 * MISTAKE 2: Using wrong API
 *
 *   void BAD_ISR(void) {
 *       xSemaphoreGive(sem);  // WRONG! Not FromISR version!
 *   }
 *
 *
 * MISTAKE 3: Not yielding when needed
 *
 *   void BAD_ISR(void) {
 *       BaseType_t wake;
 *       xSemaphoreGiveFromISR(sem, &wake);
 *       // WRONG! Forgot to yield! High priority task won't run immediately!
 *   }
 *
 *   void GOOD_ISR(void) {
 *       BaseType_t wake = pdFALSE;
 *       xSemaphoreGiveFromISR(sem, &wake);
 *       portYIELD_FROM_ISR(wake);  // RIGHT!
 *   }
 *
 *
 * MISTAKE 4: Wrong interrupt priority
 *
 *   NVIC_SetPriority(UART_IRQn, 3);  // Priority 3
 *   // configMAX_SYSCALL_INTERRUPT_PRIORITY = 5
 *   // 3 < 5, so this ISR CANNOT call FreeRTOS APIs!
 *
 * ============================================================================
 * INTERRUPT LATENCY
 * ============================================================================
 *
 * DEFINITION: Time from interrupt trigger to ISR start
 *
 * Components:
 * 1. Hardware latency (pipeline flush, context save)
 * 2. Higher priority ISRs executing
 * 3. Critical sections (interrupts disabled)
 *
 *
 * MINIMIZING LATENCY:
 *
 * 1. Keep critical sections short
 *    taskENTER_CRITICAL();
 *    // Minimal code here!
 *    taskEXIT_CRITICAL();
 *
 * 2. Use appropriate interrupt priorities
 *    Critical ISRs get higher priority
 *
 * 3. Use BASEPRI instead of PRIMASK on Cortex-M
 *    Allows high-priority interrupts during "critical" sections
 *
 * ============================================================================
 * NESTED INTERRUPTS
 * ============================================================================
 *
 * ARM Cortex-M supports nested interrupts by default:
 *
 *   Low priority ISR executing
 *        |
 *        v
 *   ┌────────────────────┐
 *   │ Low Priority ISR   │
 *   │  ...doing work...  │──── High priority interrupt arrives!
 *   │                    │
 *   │  ┌──────────────┐  │
 *   │  │ High Pri ISR │  │<── Preempts low priority ISR
 *   │  │              │  │
 *   │  └──────────────┘  │
 *   │                    │<── Resumes low priority ISR
 *   │  ...continue...    │
 *   └────────────────────┘
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN THE PATTERN
 *    "Short ISR signals task, task does complex work"
 *
 * 2. MENTION FromISR
 *    "Always use FromISR variants in interrupt handlers"
 *
 * 3. KNOW THE PRIORITY RULES
 *    "ISR priority must be >= configMAX_SYSCALL_INTERRUPT_PRIORITY"
 *
 * 4. DISCUSS YIELD
 *    "portYIELD_FROM_ISR ensures woken task runs immediately"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "Why not do all processing in ISR?"
 * A: ISRs block other interrupts, increase latency, can cause missed
 *    deadlines. Also can't use blocking APIs in ISR.
 *
 * Q: "What's the difference between xSemaphoreGive and xSemaphoreGiveFromISR?"
 * A: FromISR never blocks, doesn't call scheduler directly, uses special
 *    parameter to indicate if context switch is needed.
 *
 * Q: "How do you handle a very high-rate interrupt?"
 * A: Use DMA to batch data, use circular buffer, use interrupt coalescing,
 *    or move ISR to higher priority (but can't use FreeRTOS APIs).
 *
 * Q: "What happens if you call wrong API in ISR?"
 * A: Undefined behavior. May corrupt RTOS state, cause crashes,
 *    priority inversion, or seemingly random bugs.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 11 ===\n\n");
    printf("Q: How do you handle Interrupts in RTOS?\n\n");
    printf("THE GOLDEN RULE:\n");
    printf("ISR = Short, Fast, Non-blocking\n");
    printf("Task = Complex processing, can block\n\n");
    printf("DEFERRED INTERRUPT PATTERN:\n");
    printf("1. ISR: Acknowledge HW, read data\n");
    printf("2. ISR: Signal task (xSemaphoreGiveFromISR)\n");
    printf("3. ISR: Yield if needed (portYIELD_FROM_ISR)\n");
    printf("4. Task: Wait for signal, do complex work\n\n");
    printf("CRITICAL RULES:\n");
    printf("- Use 'FromISR' API variants in ISR\n");
    printf("- NEVER block in ISR\n");
    printf("- Check/set interrupt priority correctly\n");
    printf("- Always call portYIELD_FROM_ISR at end\n");
    return 0;
}
