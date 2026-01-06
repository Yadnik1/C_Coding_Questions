/*
 * ============================================================================
 * QUESTION 24: What IPC mechanisms are available in RTOS?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: High | IMPORTANCE: High
 *
 * Inter-Process/Inter-Task Communication is essential for multi-tasking.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "RTOS provides several IPC mechanisms: Queues for passing data between tasks
 * with buffering and blocking; Semaphores for signaling (binary) or counting
 * resources; Mutexes for mutual exclusion with priority inheritance; Event
 * Groups for waiting on multiple conditions (ANY/ALL); Task Notifications for
 * lightweight direct-to-task signaling; and Mailboxes for fixed-size message
 * passing. Each has different use cases - queues for data transfer, semaphores
 * for signaling, mutexes for resource protection. Choice depends on whether
 * you need to pass data, just signal, or protect resources."
 *
 * ============================================================================
 * VISUALIZATION: IPC Mechanisms Overview
 * ============================================================================
 *
 *   +----------------+-------------------+----------------------------------+
 *   | Mechanism      | Primary Purpose   | Data Transfer?                   |
 *   +----------------+-------------------+----------------------------------+
 *   | Queue          | Data passing      | Yes (any size, buffered)         |
 *   | Semaphore      | Signaling/Counting| No (just count)                  |
 *   | Mutex          | Resource locking  | No (just lock state)             |
 *   | Event Group    | Multi-condition   | No (just bit flags)              |
 *   | Task Notify    | Lightweight signal| Minimal (32-bit value)           |
 *   | Mailbox        | Fixed-size msg    | Yes (pointer to data)            |
 *   | Stream Buffer  | Byte stream       | Yes (continuous bytes)           |
 *   | Message Buffer | Variable messages | Yes (variable-size messages)     |
 *   +----------------+-------------------+----------------------------------+
 *
 *
 *   DECISION FLOWCHART:
 *
 *   Need to pass data? ──Yes──> How much?
 *         │                        │
 *        No                  Fixed size ──> Queue or Mailbox
 *         │                        │
 *         v                  Variable ────> Message Buffer
 *   Need to wait for           │
 *   multiple conditions? ──Yes──> Byte stream ──> Stream Buffer
 *         │
 *        No
 *         │
 *         v
 *   Need resource ──Yes──> Mutex (with priority inheritance)
 *   protection?
 *         │
 *        No
 *         │
 *         v
 *   Simple signaling ──> Semaphore or Task Notification
 *
 * ============================================================================
 * 1. QUEUES
 * ============================================================================
 *
 *   Most versatile IPC - buffers data between producer and consumer.
 *
 *   Producer                Queue                    Consumer
 *   --------               ------                   --------
 *   xQueueSend() ──────>  [Item1]
 *                         [Item2]  ─────────────>  xQueueReceive()
 *                         [Item3]
 *                         [     ]  <- Space for more
 *
 *   Features:
 *   - FIFO ordering (or priority ordering)
 *   - Blocking send/receive with timeout
 *   - Multiple producers/consumers supported
 *   - ISR-safe with FromISR variants
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "queue.h"

// Create queue
QueueHandle_t data_queue;
data_queue = xQueueCreate(10, sizeof(SensorData_t));  // 10 items

// Producer task
void producer_task(void *pvParameters) {
    SensorData_t data;
    for (;;) {
        data.value = read_sensor();
        data.timestamp = xTaskGetTickCount();

        // Send to queue (block if full)
        if (xQueueSend(data_queue, &data, pdMS_TO_TICKS(100)) != pdPASS) {
            // Queue full for 100ms - handle error
        }
    }
}

// Consumer task
void consumer_task(void *pvParameters) {
    SensorData_t data;
    for (;;) {
        // Receive from queue (block if empty)
        if (xQueueReceive(data_queue, &data, portMAX_DELAY) == pdPASS) {
            process_data(&data);
        }
    }
}

// From ISR
void sensor_isr(void) {
    SensorData_t data;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    data.value = SENSOR->DATA;
    xQueueSendFromISR(data_queue, &data, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
*/

/*
 * ============================================================================
 * 2. SEMAPHORES
 * ============================================================================
 *
 *   BINARY SEMAPHORE: Signaling (0 or 1)
 *
 *   ISR/Task A                                   Task B
 *   ----------                                   ------
 *   xSemaphoreGive() ──────> [1] ──────────────> xSemaphoreTake()
 *                                                (was waiting, now runs)
 *
 *   COUNTING SEMAPHORE: Resource counting (0 to N)
 *
 *   Available resources: [████░░░░░░] 4/10
 *
 *   Task takes:    [███░░░░░░░] 3/10
 *   Task takes:    [██░░░░░░░░] 2/10
 *   Task gives:    [███░░░░░░░] 3/10
 */

/*
// Binary semaphore for signaling
SemaphoreHandle_t data_ready_sem;
data_ready_sem = xSemaphoreCreateBinary();

// Counting semaphore for resource pool
SemaphoreHandle_t resource_sem;
resource_sem = xSemaphoreCreateCounting(10, 10);  // Max 10, initial 10

// Signal from ISR
void data_ready_isr(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(data_ready_sem, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Wait in task
void processing_task(void *pvParameters) {
    for (;;) {
        // Wait for data ready signal
        xSemaphoreTake(data_ready_sem, portMAX_DELAY);
        process_new_data();
    }
}
*/

/*
 * ============================================================================
 * 3. MUTEXES
 * ============================================================================
 *
 *   For protecting shared resources with priority inheritance.
 *
 *   Task A (High Pri)    Mutex    Task B (Low Pri)
 *   ----------------     -----    ----------------
 *                                 Take mutex ────> [Locked by B]
 *   Want mutex ──────────────────> [WAITING]
 *                        B's priority boosted to A's!
 *                                 Release mutex ──> [Available]
 *   Acquire mutex <────────────── [Locked by A]
 */

/*
SemaphoreHandle_t i2c_mutex;
i2c_mutex = xSemaphoreCreateMutex();

void i2c_transfer(uint8_t *data, size_t len) {
    // Only one task can use I2C at a time
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    {
        i2c_start();
        i2c_write(data, len);
        i2c_stop();
    }
    xSemaphoreGive(i2c_mutex);
}
*/

/*
 * ============================================================================
 * 4. EVENT GROUPS
 * ============================================================================
 *
 *   Wait for multiple conditions (ANY or ALL).
 *
 *   Event Bits:  [0][0][0][0][1][0][1][0]
 *                          ^       ^
 *                          |       |
 *                       SENSOR  UART_RX
 *
 *   Wait for ANY: Wake if SENSOR OR UART_RX set
 *   Wait for ALL: Wake only if SENSOR AND UART_RX both set
 */

/*
#include "event_groups.h"

#define EVENT_SENSOR_READY  (1 << 0)
#define EVENT_UART_RX       (1 << 1)
#define EVENT_TIMEOUT       (1 << 2)

EventGroupHandle_t system_events;
system_events = xEventGroupCreate();

// Set event from ISR
void uart_rx_isr(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(system_events, EVENT_UART_RX,
                               &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Wait for multiple events
void handler_task(void *pvParameters) {
    EventBits_t bits;
    for (;;) {
        // Wait for ANY of these events
        bits = xEventGroupWaitBits(
            system_events,
            EVENT_SENSOR_READY | EVENT_UART_RX | EVENT_TIMEOUT,
            pdTRUE,      // Clear bits on exit
            pdFALSE,     // Wait for ANY (not ALL)
            portMAX_DELAY
        );

        if (bits & EVENT_SENSOR_READY) handle_sensor();
        if (bits & EVENT_UART_RX) handle_uart();
        if (bits & EVENT_TIMEOUT) handle_timeout();
    }
}
*/

/*
 * ============================================================================
 * 5. TASK NOTIFICATIONS (FreeRTOS-specific)
 * ============================================================================
 *
 *   Lightweight, direct task-to-task signaling.
 *   Faster than semaphores, zero additional RAM.
 *
 *   Task A ─────────────────────────> Task B
 *            xTaskNotifyGive()        ulTaskNotifyTake()
 */

/*
TaskHandle_t worker_task_handle;

// Signal task
void master_task(void *pvParameters) {
    for (;;) {
        prepare_work();
        xTaskNotifyGive(worker_task_handle);  // Signal worker
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Wait for signal
void worker_task(void *pvParameters) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // Wait for signal
        do_work();
    }
}
*/

/*
 * ============================================================================
 * 6. STREAM AND MESSAGE BUFFERS (FreeRTOS)
 * ============================================================================
 *
 *   STREAM BUFFER: Continuous byte stream (like pipe)
 *
 *   Writer ────> [AABBCCDDEE........] ────> Reader
 *                 ^              ^
 *                 |              |
 *              Head           Tail
 *
 *   MESSAGE BUFFER: Discrete variable-length messages
 *
 *   Writer ────> [Len][Message1][Len][Message2][...] ────> Reader
 *
 *   Note: Single writer, single reader only!
 */

/*
#include "stream_buffer.h"
#include "message_buffer.h"

// Stream buffer for UART
StreamBufferHandle_t uart_stream;
uart_stream = xStreamBufferCreate(256, 1);  // 256 bytes, trigger at 1 byte

void uart_rx_isr(void) {
    uint8_t byte = UART->DR;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xStreamBufferSendFromISR(uart_stream, &byte, 1, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_task(void *pvParameters) {
    uint8_t buffer[64];
    size_t received;
    for (;;) {
        received = xStreamBufferReceive(uart_stream, buffer, sizeof(buffer),
                                        portMAX_DELAY);
        process_uart_data(buffer, received);
    }
}


// Message buffer for variable-length messages
MessageBufferHandle_t msg_buffer;
msg_buffer = xMessageBufferCreate(512);

void send_message(void) {
    char msg[] = "Hello, World!";
    xMessageBufferSend(msg_buffer, msg, strlen(msg) + 1, portMAX_DELAY);
}

void receive_messages(void *pvParameters) {
    char buffer[128];
    size_t len;
    for (;;) {
        len = xMessageBufferReceive(msg_buffer, buffer, sizeof(buffer),
                                    portMAX_DELAY);
        printf("Received: %s (len=%d)\n", buffer, len);
    }
}
*/

/*
 * ============================================================================
 * ZEPHYR IPC MECHANISMS
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>

// ============================================================================
// Zephyr Message Queue (k_msgq)
// ============================================================================

K_MSGQ_DEFINE(sensor_msgq, sizeof(SensorData_t), 10, 4);

void zephyr_producer(void) {
    SensorData_t data;
    while (1) {
        data.value = read_sensor();
        k_msgq_put(&sensor_msgq, &data, K_FOREVER);
        k_sleep(K_MSEC(100));
    }
}

void zephyr_consumer(void) {
    SensorData_t data;
    while (1) {
        k_msgq_get(&sensor_msgq, &data, K_FOREVER);
        process_data(&data);
    }
}


// ============================================================================
// Zephyr Semaphore (k_sem)
// ============================================================================

K_SEM_DEFINE(data_sem, 0, 1);  // Initial 0, max 1 (binary)

void zephyr_signal(void) {
    k_sem_give(&data_sem);
}

void zephyr_wait(void) {
    k_sem_take(&data_sem, K_FOREVER);
    handle_event();
}


// ============================================================================
// Zephyr Mutex (k_mutex)
// ============================================================================

K_MUTEX_DEFINE(resource_mutex);

void use_shared_resource(void) {
    k_mutex_lock(&resource_mutex, K_FOREVER);
    // Access shared resource
    k_mutex_unlock(&resource_mutex);
}


// ============================================================================
// Zephyr Pipe (k_pipe) - Stream of bytes
// ============================================================================

K_PIPE_DEFINE(my_pipe, 256, 4);  // 256 bytes, 4-byte alignment

void pipe_writer(void) {
    char data[] = "Hello";
    size_t written;
    k_pipe_put(&my_pipe, data, sizeof(data), &written, sizeof(data), K_FOREVER);
}

void pipe_reader(void) {
    char buffer[64];
    size_t read;
    k_pipe_get(&my_pipe, buffer, sizeof(buffer), &read, 1, K_FOREVER);
}


// ============================================================================
// Zephyr Mailbox (k_mbox) - Message with metadata
// ============================================================================

K_MBOX_DEFINE(my_mbox);

void mailbox_sender(void) {
    struct k_mbox_msg msg;
    char data[] = "Important message";

    msg.info = 0;
    msg.size = sizeof(data);
    msg.tx_data = data;
    msg.tx_target_thread = K_ANY;

    k_mbox_put(&my_mbox, &msg, K_FOREVER);
}

void mailbox_receiver(void) {
    struct k_mbox_msg msg;
    char buffer[64];

    msg.info = 0;
    msg.size = sizeof(buffer);
    msg.rx_source_thread = K_ANY;

    k_mbox_get(&my_mbox, &msg, buffer, K_FOREVER);
    printk("Received: %s\n", buffer);
}
*/

/*
 * ============================================================================
 * COMPARISON TABLE
 * ============================================================================
 *
 *   +-------------+----------+----------+-----------+-----------+----------+
 *   | Feature     | Queue    | Semaphore| Mutex     | Event Grp | Notify   |
 *   +-------------+----------+----------+-----------+-----------+----------+
 *   | Data        | Yes      | No       | No        | No        | 32-bit   |
 *   | Buffering   | Yes      | Count    | No        | Bits      | No       |
 *   | ISR safe    | FromISR  | FromISR  | NO!       | SetFromISR| FromISR  |
 *   | Priority Inh| No       | No       | YES       | No        | No       |
 *   | Multi-wait  | No       | No       | No        | YES       | No       |
 *   | Overhead    | High     | Medium   | Medium    | Medium    | Low      |
 *   +-------------+----------+----------+-----------+-----------+----------+
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. KNOW THE USE CASES
 *    "Queue for data, semaphore for signaling, mutex for protection"
 *
 * 2. MENTION ISR SAFETY
 *    "Mutex cannot be used from ISR, use semaphore or queue FromISR"
 *
 * 3. DISCUSS PRIORITY INHERITANCE
 *    "Only mutex has priority inheritance, semaphore doesn't"
 *
 * 4. EXPLAIN WHEN TO USE EACH
 *    "Task notification for simple signals, queue when data needed"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "When would you choose queue over message buffer?"
 * A: Queue for fixed-size items with multiple producers/consumers.
 *    Message buffer for variable-length, single producer/consumer only.
 *
 * Q: "Can you send a pointer through a queue?"
 * A: Yes, but be careful! The pointed data must outlive the message.
 *    Better to copy data for safety, or use memory pool.
 *
 * Q: "What's the difference between k_msgq and k_mbox in Zephyr?"
 * A: k_msgq is fixed-size, copies data. k_mbox is pointer-based,
 *    supports synchronous transfer where sender waits for receiver.
 *
 * Q: "Which IPC has lowest overhead?"
 * A: Task notifications - no kernel object creation, built into TCB.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 24 ===\n\n");
    printf("Q: What IPC mechanisms are available in RTOS?\n\n");
    printf("IPC MECHANISMS:\n\n");
    printf("1. QUEUE:\n");
    printf("   - Data passing with buffering\n");
    printf("   - Multiple producers/consumers\n");
    printf("   - ISR-safe (FromISR)\n\n");
    printf("2. SEMAPHORE:\n");
    printf("   - Binary: signaling\n");
    printf("   - Counting: resource pool\n");
    printf("   - ISR-safe (FromISR)\n\n");
    printf("3. MUTEX:\n");
    printf("   - Resource protection\n");
    printf("   - Priority inheritance\n");
    printf("   - NOT ISR-safe!\n\n");
    printf("4. EVENT GROUP:\n");
    printf("   - Multi-condition wait (ANY/ALL)\n");
    printf("   - Broadcast to all waiters\n\n");
    printf("5. TASK NOTIFICATION:\n");
    printf("   - Lightweight signaling\n");
    printf("   - Zero overhead (built into TCB)\n\n");
    printf("6. STREAM/MESSAGE BUFFER:\n");
    printf("   - Byte stream or variable messages\n");
    printf("   - Single producer/consumer\n");
    return 0;
}
