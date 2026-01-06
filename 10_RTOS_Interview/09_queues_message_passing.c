/*
 * ============================================================================
 * QUESTION 09: How do Queues work in RTOS? When would you use them?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: High | IMPORTANCE: Critical
 *
 * Queues are the PRIMARY inter-task communication mechanism in RTOS.
 * Understanding them is essential for embedded systems interviews.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Queues in RTOS provide thread-safe message passing between tasks. They're
 * FIFO buffers where one task sends data and another receives it. The key
 * advantage is DECOUPLING - sender and receiver don't need to synchronize
 * directly. Queues can block tasks when full (send) or empty (receive),
 * making them perfect for producer-consumer patterns. In FreeRTOS, queues
 * copy data by value, so the original can be modified after sending. Queues
 * are also safe to use from ISRs with the 'FromISR' variants."
 *
 * ============================================================================
 * VISUALIZATION: Queue Operation
 * ============================================================================
 *
 *   PRODUCER                QUEUE (FIFO)                CONSUMER
 *   (Task A)                                            (Task B)
 *
 *   +--------+     Send      +---+---+---+---+    Receive    +--------+
 *   | Data 1 |-------------->| 1 | 2 | 3 |   |-------------->| Data 1 |
 *   +--------+               +---+---+---+---+               +--------+
 *                             ^           ^
 *                             |           |
 *                            Tail       Head
 *                            (write)    (read)
 *
 *
 * QUEUE STATES:
 *
 *   EMPTY:          +---+---+---+---+     Receive blocks (waits for data)
 *                   |   |   |   |   |
 *                   +---+---+---+---+
 *
 *   PARTIAL:        +---+---+---+---+     Send and Receive both work
 *                   | A | B |   |   |
 *                   +---+---+---+---+
 *
 *   FULL:           +---+---+---+---+     Send blocks (waits for space)
 *                   | A | B | C | D |
 *                   +---+---+---+---+
 *
 * ============================================================================
 * WHY USE QUEUES?
 * ============================================================================
 *
 * 1. DECOUPLING
 *    - Producer doesn't need to know about consumer
 *    - Different speeds: fast producer, slow consumer (buffering)
 *
 * 2. THREAD SAFETY
 *    - Built-in synchronization (no manual mutex needed)
 *    - Safe for multiple producers/consumers
 *
 * 3. ISR TO TASK COMMUNICATION
 *    - ISR can send to queue without blocking
 *    - Task processes data at its own pace
 *
 * 4. DATA INTEGRITY
 *    - Data is COPIED (FreeRTOS default) - no pointer issues
 *    - Or passed by reference (for large data)
 *
 * ============================================================================
 * FREERTOS QUEUE EXAMPLE
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Define message structure
typedef struct {
    uint8_t sensor_id;
    int32_t value;
    uint32_t timestamp;
} SensorData;

// Queue handle
QueueHandle_t sensor_queue;

void queue_example_init(void) {
    // Create queue: 10 items, each sizeof(SensorData) bytes
    sensor_queue = xQueueCreate(10, sizeof(SensorData));

    if (sensor_queue == NULL) {
        // Queue creation failed - out of memory
        printf("Failed to create queue!\n");
    }
}


// PRODUCER TASK - Reads sensors and sends data to queue
void sensor_task(void *pvParameters) {
    SensorData data;

    for (;;) {
        // Read sensor
        data.sensor_id = 1;
        data.value = read_sensor();
        data.timestamp = xTaskGetTickCount();

        // Send to queue (blocks if full, up to 100ms)
        if (xQueueSend(sensor_queue, &data, pdMS_TO_TICKS(100)) == pdTRUE) {
            // Data sent successfully
        } else {
            // Queue full for 100ms - handle overflow
            printf("Queue full! Data dropped.\n");
        }

        vTaskDelay(pdMS_TO_TICKS(10));  // Read every 10ms
    }
}


// CONSUMER TASK - Receives data from queue and processes it
void processing_task(void *pvParameters) {
    SensorData received_data;

    for (;;) {
        // Receive from queue (blocks until data available)
        if (xQueueReceive(sensor_queue, &received_data, portMAX_DELAY) == pdTRUE) {
            // Process received data
            printf("Sensor %d: %d at tick %u\n",
                   received_data.sensor_id,
                   received_data.value,
                   received_data.timestamp);
        }
    }
}


// ISR TO TASK COMMUNICATION
void ADC_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    SensorData isr_data;

    isr_data.sensor_id = 0;
    isr_data.value = ADC_read();
    isr_data.timestamp = xTaskGetTickCountFromISR();

    // Send from ISR (never blocks!)
    xQueueSendFromISR(sensor_queue, &isr_data, &xHigherPriorityTaskWoken);

    // Yield if higher priority task was woken
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
*/

/*
 * ============================================================================
 * ZEPHYR QUEUE EXAMPLE
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>

typedef struct {
    uint8_t sensor_id;
    int32_t value;
    uint32_t timestamp;
} SensorData;

// Define message queue: 10 messages, aligned properly
K_MSGQ_DEFINE(sensor_msgq, sizeof(SensorData), 10, 4);


// PRODUCER
void sensor_thread(void *p1, void *p2, void *p3) {
    SensorData data;

    while (1) {
        data.sensor_id = 1;
        data.value = read_sensor();
        data.timestamp = k_uptime_get_32();

        // Put message in queue (blocks if full)
        int ret = k_msgq_put(&sensor_msgq, &data, K_MSEC(100));
        if (ret != 0) {
            printk("Queue full!\n");
        }

        k_sleep(K_MSEC(10));
    }
}


// CONSUMER
void processing_thread(void *p1, void *p2, void *p3) {
    SensorData data;

    while (1) {
        // Get message from queue (blocks until available)
        if (k_msgq_get(&sensor_msgq, &data, K_FOREVER) == 0) {
            printk("Sensor %d: %d\n", data.sensor_id, data.value);
        }
    }
}


// ISR example
void adc_isr(void *arg) {
    SensorData data;
    data.sensor_id = 0;
    data.value = adc_read();
    data.timestamp = k_uptime_get_32();

    // Put from ISR (use K_NO_WAIT - ISR can't block!)
    k_msgq_put(&sensor_msgq, &data, K_NO_WAIT);
}
*/

/*
 * ============================================================================
 * QUEUE TYPES AND VARIANTS
 * ============================================================================
 *
 * 1. FIFO QUEUE (Default)
 *    First In, First Out
 *    xQueueSend() / xQueueReceive()
 *
 *    Send order: A, B, C
 *    Receive order: A, B, C
 *
 *
 * 2. LIFO QUEUE (Stack-like)
 *    Last In, First Out
 *    xQueueSendToFront() - puts at front
 *
 *    Send order: A, B, C (using SendToFront)
 *    Receive order: C, B, A
 *
 *    USE CASE: Priority messages that should be processed first
 *
 *
 * 3. QUEUE SET (FreeRTOS)
 *    Wait on multiple queues simultaneously
 *    Like select() in Unix
 *
 *    QueueSetHandle_t set = xQueueCreateSet(20);
 *    xQueueAddToSet(queue1, set);
 *    xQueueAddToSet(queue2, set);
 *
 *    // Wait for any queue to have data
 *    QueueSetMemberHandle_t member = xQueueSelectFromSet(set, portMAX_DELAY);
 *    if (member == queue1) {
 *        xQueueReceive(queue1, &data, 0);
 *    }
 *
 *
 * 4. MAILBOX (Zephyr)
 *    Synchronous message passing
 *    Sender blocks until receiver gets message
 *
 *    K_MBOX_DEFINE(my_mailbox);
 *
 *    // Sender waits for receiver
 *    k_mbox_put(&my_mailbox, &tx_msg, K_FOREVER);
 *
 *    // Receiver waits for sender
 *    k_mbox_get(&my_mailbox, &rx_msg, NULL, K_FOREVER);
 *
 * ============================================================================
 * QUEUE vs SEMAPHORE vs MUTEX
 * ============================================================================
 *
 * +------------------+--------------------+--------------------+--------------------+
 * | Aspect           | Queue              | Semaphore          | Mutex              |
 * +------------------+--------------------+--------------------+--------------------+
 * | Purpose          | Pass DATA          | SIGNAL events      | PROTECT resources  |
 * | Data transfer    | Yes (copies data)  | No (just signal)   | No                 |
 * | Multiple items   | Yes (buffered)     | Counting only      | No (binary)        |
 * | Blocking         | Both send/receive  | Take only          | Take only          |
 * | ISR safe         | Yes (FromISR)      | Yes (FromISR)      | No                 |
 * | Priority inherit | No                 | No                 | Yes                |
 * +------------------+--------------------+--------------------+--------------------+
 *
 * ============================================================================
 * COPY BY VALUE vs BY REFERENCE
 * ============================================================================
 *
 * COPY BY VALUE (FreeRTOS default):
 * - Queue stores a COPY of the data
 * - Original can be modified/freed after send
 * - Safe and simple
 * - Overhead for large data
 *
 *   SensorData data;
 *   data.value = 42;
 *   xQueueSend(queue, &data, timeout);  // COPY made
 *   data.value = 99;  // OK! Queue has separate copy
 *
 *
 * COPY BY REFERENCE (Pointer queue):
 * - Queue stores POINTER to data
 * - No copy overhead
 * - Must manage memory carefully!
 *
 *   // Queue of pointers
 *   QueueHandle_t ptr_queue = xQueueCreate(10, sizeof(SensorData*));
 *
 *   SensorData *pData = malloc(sizeof(SensorData));
 *   pData->value = 42;
 *   xQueueSend(ptr_queue, &pData, timeout);  // Pointer copied
 *   // Don't free pData until consumer is done!
 *
 *   // Consumer
 *   SensorData *received;
 *   xQueueReceive(ptr_queue, &received, portMAX_DELAY);
 *   use(received->value);
 *   free(received);  // Consumer frees memory
 *
 * ============================================================================
 * QUEUE SIZING CONSIDERATIONS
 * ============================================================================
 *
 * Queue depth (number of items) depends on:
 *
 * 1. PRODUCER/CONSUMER SPEED RATIO
 *    - If producer faster: need deeper queue to buffer bursts
 *    - If consumer faster: shallow queue is fine
 *
 * 2. WORST-CASE BURST
 *    - How many messages can arrive before consumer runs?
 *    - Consider ISR frequency
 *
 * 3. MEMORY CONSTRAINTS
 *    - Each slot = item_size bytes
 *    - Total = depth * item_size + queue overhead
 *
 * 4. LATENCY REQUIREMENTS
 *    - Deeper queue = higher max latency
 *    - Real-time: may prefer smaller queue + flow control
 *
 *
 * EXAMPLE CALCULATION:
 *
 *   Sensor ISR: 1000 Hz (every 1ms)
 *   Processing task: runs at 100 Hz (every 10ms)
 *   Item size: 12 bytes
 *
 *   Worst case: 10 samples arrive between processing runs
 *   Queue depth: at least 10 (add margin: 15-20)
 *   Memory: 20 * 12 = 240 bytes + overhead
 *
 * ============================================================================
 * COMMON MISTAKES
 * ============================================================================
 *
 * 1. BLOCKING IN ISR
 *
 *    WRONG:
 *    void ISR(void) {
 *        xQueueSend(queue, &data, portMAX_DELAY);  // ISR can't block!
 *    }
 *
 *    RIGHT:
 *    void ISR(void) {
 *        xQueueSendFromISR(queue, &data, &wake);  // Never blocks
 *    }
 *
 *
 * 2. WRONG SIZE IN xQueueReceive
 *
 *    WRONG:
 *    int received;
 *    xQueueReceive(queue_of_structs, &received, ...);  // Wrong size!
 *
 *    RIGHT:
 *    MyStruct received;
 *    xQueueReceive(queue, &received, ...);  // Correct type
 *
 *
 * 3. NOT CHECKING RETURN VALUE
 *
 *    WRONG:
 *    xQueueSend(queue, &data, 0);  // Ignoring return!
 *    use_data_assuming_sent();
 *
 *    RIGHT:
 *    if (xQueueSend(queue, &data, 0) != pdTRUE) {
 *        handle_queue_full();
 *    }
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN THE BASICS
 *    "FIFO buffer for task-to-task communication"
 *
 * 2. MENTION COPY SEMANTICS
 *    "FreeRTOS copies data by value - safe but has overhead"
 *
 * 3. DISCUSS ISR USAGE
 *    "Use xQueueSendFromISR - never block in ISR"
 *
 * 4. COMPARE WITH SEMAPHORE
 *    "Queue passes DATA, semaphore just signals"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "What happens if queue is full and you send?"
 * A: With timeout: blocks until space available or timeout
 *    With 0 timeout: returns immediately with failure
 *    FromISR: never blocks, returns failure if full
 *
 * Q: "How do you handle a slow consumer?"
 * A: Options:
 *    1. Increase queue depth (buffer more)
 *    2. Drop oldest data (use SendToFront to overwrite)
 *    3. Flow control (producer checks queue space)
 *    4. Increase consumer priority
 *
 * Q: "Can multiple tasks send to same queue?"
 * A: Yes! Queue operations are atomic. Multiple producers and
 *    multiple consumers are both supported.
 *
 * Q: "What's the overhead of queue operations?"
 * A: Typical: 1-10 microseconds on Cortex-M
 *    Includes copy, list operations, potential context switch
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 09 ===\n\n");
    printf("Q: How do Queues work? When would you use them?\n\n");
    printf("WHAT IS A QUEUE:\n");
    printf("- FIFO buffer for passing data between tasks\n");
    printf("- Thread-safe (built-in synchronization)\n");
    printf("- Blocks sender if full, receiver if empty\n\n");
    printf("WHEN TO USE:\n");
    printf("- Producer-consumer patterns\n");
    printf("- ISR to task communication\n");
    printf("- Decoupling tasks with different speeds\n\n");
    printf("KEY POINTS:\n");
    printf("- FreeRTOS: xQueueCreate(), xQueueSend(), xQueueReceive()\n");
    printf("- Zephyr: K_MSGQ_DEFINE(), k_msgq_put(), k_msgq_get()\n");
    printf("- Use FromISR variants in interrupt handlers\n");
    printf("- Data is COPIED by value (FreeRTOS default)\n\n");
    printf("QUEUE vs SEMAPHORE:\n");
    printf("- Queue: Passes DATA\n");
    printf("- Semaphore: Just SIGNALS (no data)\n");
    return 0;
}
