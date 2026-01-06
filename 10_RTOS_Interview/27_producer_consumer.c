/*
 * ============================================================================
 * QUESTION 27: Explain the Producer-Consumer Pattern in RTOS
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * One of the most common design patterns in embedded systems.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Producer-Consumer is a pattern where one or more tasks produce data and
 * one or more tasks consume it, connected by a buffer (usually a queue).
 * The producer generates data (e.g., ISR reading sensor) and puts it in
 * a queue. The consumer retrieves and processes data at its own pace.
 * The queue decouples timing - producer can be fast and bursty, consumer
 * can be slower and steady. Key concerns: queue sizing to handle bursts,
 * handling queue full/empty, and deciding blocking behavior for both ends."
 *
 * ============================================================================
 * VISUALIZATION: Producer-Consumer Pattern
 * ============================================================================
 *
 *   BASIC PATTERN:
 *
 *   +----------+      +--------+      +----------+
 *   | Producer | ---> | Queue  | ---> | Consumer |
 *   +----------+      +--------+      +----------+
 *
 *
 *   MULTIPLE PRODUCERS, SINGLE CONSUMER:
 *
 *   +----------+
 *   |Producer 1| ----+
 *   +----------+     |    +--------+      +----------+
 *                    +--> | Queue  | ---> | Consumer |
 *   +----------+     |    +--------+      +----------+
 *   |Producer 2| ----+
 *   +----------+     |
 *                    |
 *   +----------+     |
 *   |Producer 3| ----+
 *   +----------+
 *
 *
 *   REAL-WORLD EXAMPLE: UART Reception
 *
 *   +----------+      +----------+      +-------------+
 *   | UART ISR | ---> | RX Queue | ---> | Parser Task |
 *   | (fast)   |      | (buffer) |      | (slow)      |
 *   +----------+      +----------+      +-------------+
 *
 *   ISR runs at wire speed (115200 baud = 11.5KB/s)
 *   Parser can take variable time per message
 *   Queue absorbs timing differences
 *
 * ============================================================================
 * QUEUE SIZING CONSIDERATIONS
 * ============================================================================
 *
 *   QUEUE TOO SMALL:
 *
 *   Producer: [===]           [==========]     [====]
 *   Queue:    [XXX] FULL!     [XXXXXXXXXX]     [XXXX]
 *   Consumer:      [====]          [========]
 *                   ^                  ^
 *                   |                  |
 *               Data lost!        Data lost!
 *
 *
 *   QUEUE PROPERLY SIZED:
 *
 *   Producer: [===]           [==========]     [====]
 *   Queue:    [XXX][___][___] [XXXXXXXXXX][_]  [XXXX][___]
 *   Consumer:      [====]     [========]       [===========]
 *
 *   Queue absorbs bursts without losing data
 *
 *
 *   SIZING FORMULA:
 *
 *   Queue Size >= (Max Producer Rate - Consumer Rate) × Max Burst Duration
 *
 *   Example:
 *   - Producer: 100 items/sec max
 *   - Consumer: 80 items/sec average
 *   - Max burst: 1 second
 *   - Queue Size >= (100-80) × 1 = 20 items minimum
 *   - Use 2x safety margin: 40 items
 *
 * ============================================================================
 * CODE EXAMPLE: Basic Producer-Consumer
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// ============================================================================
// BASIC PRODUCER-CONSUMER
// ============================================================================

typedef struct {
    uint32_t timestamp;
    int16_t value;
    uint8_t channel;
} SensorData_t;

#define QUEUE_SIZE 20

QueueHandle_t sensor_queue;

// Producer Task: Reads sensor periodically
void producer_task(void *pvParameters) {
    SensorData_t data;
    TickType_t last_wake = xTaskGetTickCount();

    for (;;) {
        // Produce data
        data.timestamp = xTaskGetTickCount();
        data.value = read_adc();
        data.channel = 0;

        // Send to queue
        if (xQueueSend(sensor_queue, &data, pdMS_TO_TICKS(10)) != pdPASS) {
            // Queue full - handle error
            // Options: drop data, overwrite oldest, block longer
            printf("Warning: Queue full, data dropped!\n");
        }

        // Wait for next period
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(10));  // 100 Hz
    }
}

// Consumer Task: Processes data
void consumer_task(void *pvParameters) {
    SensorData_t data;

    for (;;) {
        // Wait for data (blocks until available)
        if (xQueueReceive(sensor_queue, &data, portMAX_DELAY) == pdPASS) {
            // Process data (variable time)
            process_sensor_data(&data);
        }
    }
}

void init_producer_consumer(void) {
    sensor_queue = xQueueCreate(QUEUE_SIZE, sizeof(SensorData_t));

    xTaskCreate(producer_task, "Producer", 256, NULL, 4, NULL);
    xTaskCreate(consumer_task, "Consumer", 512, NULL, 3, NULL);
}


// ============================================================================
// ISR AS PRODUCER
// ============================================================================

// ISR produces, Task consumes (very common pattern)

QueueHandle_t uart_rx_queue;

void UART_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t byte;

    while (UART->SR & UART_SR_RXNE) {
        byte = UART->DR;

        // Send to queue from ISR
        xQueueSendFromISR(uart_rx_queue, &byte, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_consumer_task(void *pvParameters) {
    uint8_t byte;
    uint8_t buffer[128];
    uint8_t index = 0;

    for (;;) {
        // Receive byte from ISR
        if (xQueueReceive(uart_rx_queue, &byte, portMAX_DELAY) == pdPASS) {
            buffer[index++] = byte;

            // Process complete line
            if (byte == '\n' || index >= sizeof(buffer)) {
                process_line(buffer, index);
                index = 0;
            }
        }
    }
}


// ============================================================================
// MULTIPLE PRODUCERS
// ============================================================================

QueueHandle_t event_queue;

typedef struct {
    uint8_t source;
    uint8_t type;
    uint32_t data;
} Event_t;

// Multiple sensors, one queue
void sensor1_task(void *pvParameters) {
    Event_t event = {.source = 1};
    for (;;) {
        event.type = SENSOR_EVENT;
        event.data = read_sensor_1();
        xQueueSend(event_queue, &event, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void sensor2_task(void *pvParameters) {
    Event_t event = {.source = 2};
    for (;;) {
        event.type = SENSOR_EVENT;
        event.data = read_sensor_2();
        xQueueSend(event_queue, &event, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(50));  // Faster rate
    }
}

void button_task(void *pvParameters) {
    Event_t event = {.source = 3};
    for (;;) {
        // Wait for button press (via semaphore from ISR)
        xSemaphoreTake(button_sem, portMAX_DELAY);
        event.type = BUTTON_EVENT;
        event.data = get_button_state();
        xQueueSend(event_queue, &event, portMAX_DELAY);
    }
}

void event_consumer_task(void *pvParameters) {
    Event_t event;
    for (;;) {
        xQueueReceive(event_queue, &event, portMAX_DELAY);

        switch (event.type) {
            case SENSOR_EVENT:
                handle_sensor_event(event.source, event.data);
                break;
            case BUTTON_EVENT:
                handle_button_event(event.data);
                break;
        }
    }
}
*/

/*
 * ============================================================================
 * ZEPHYR PRODUCER-CONSUMER
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>

typedef struct {
    int16_t value;
    uint32_t timestamp;
} DataItem_t;

// Define message queue
K_MSGQ_DEFINE(data_queue, sizeof(DataItem_t), 20, 4);

void zephyr_producer(void *p1, void *p2, void *p3) {
    DataItem_t item;

    while (1) {
        item.value = read_adc();
        item.timestamp = k_uptime_get_32();

        // Non-blocking put with timeout
        if (k_msgq_put(&data_queue, &item, K_MSEC(10)) != 0) {
            printk("Queue full!\n");
        }

        k_sleep(K_MSEC(10));
    }
}

void zephyr_consumer(void *p1, void *p2, void *p3) {
    DataItem_t item;

    while (1) {
        // Blocking get
        if (k_msgq_get(&data_queue, &item, K_FOREVER) == 0) {
            process_data(&item);
        }
    }
}

// Alternative: Use FIFO for variable-size or pointer-based data
K_FIFO_DEFINE(work_fifo);

struct work_item {
    void *fifo_reserved;  // Required by FIFO
    uint8_t data[64];
    size_t len;
};

void fifo_producer(void) {
    struct work_item *item = k_malloc(sizeof(*item));
    if (item) {
        item->len = read_data(item->data, sizeof(item->data));
        k_fifo_put(&work_fifo, item);
    }
}

void fifo_consumer(void) {
    struct work_item *item;
    while (1) {
        item = k_fifo_get(&work_fifo, K_FOREVER);
        process_work(item->data, item->len);
        k_free(item);
    }
}
*/

/*
 * ============================================================================
 * HANDLING BACKPRESSURE
 * ============================================================================
 *
 * When producer is faster than consumer, need strategy:
 *
 * STRATEGY 1: DROP NEW DATA
 * -------------------------
 * Use non-blocking send, drop if full
 *
 *   if (xQueueSend(q, &data, 0) != pdPASS) {
 *       // Drop - acceptable for streaming data
 *       drop_count++;
 *   }
 *
 *
 * STRATEGY 2: DROP OLDEST DATA
 * ----------------------------
 * Overwrite front of queue
 *
 *   xQueueOverwrite(q, &data);  // Only for queue size 1!
 *
 *   // For larger queues, implement circular buffer
 *
 *
 * STRATEGY 3: BLOCK PRODUCER
 * --------------------------
 * Wait until space available
 *
 *   xQueueSend(q, &data, portMAX_DELAY);
 *
 * Careful: ISR cannot block!
 *
 *
 * STRATEGY 4: FLOW CONTROL
 * ------------------------
 * Signal producer to slow down
 *
 *   if (uxQueueSpacesAvailable(q) < threshold) {
 *       signal_producer_to_pause();
 *   }
 *
 * ============================================================================
 * DESIGN CONSIDERATIONS
 * ============================================================================
 *
 * 1. QUEUE ITEM TYPE
 *    - Small items: Copy directly into queue (safest)
 *    - Large items: Use pointers (need memory management)
 *
 *   // Small item - copy
 *   typedef struct {
 *       uint16_t value;
 *       uint8_t status;
 *   } SmallData_t;  // 3 bytes - OK to copy
 *
 *   // Large item - use pointer
 *   typedef struct {
 *       uint8_t buffer[256];
 *       size_t len;
 *   } LargeData_t;  // 257 bytes - use pointer
 *
 *   LargeData_t *ptr = pvPortMalloc(sizeof(LargeData_t));
 *   xQueueSend(q, &ptr, timeout);  // Send pointer
 *   // Consumer must free!
 *
 *
 * 2. PRIORITY ASSIGNMENT
 *    - Producer higher: Ensures data collected even if queue fills
 *    - Consumer higher: Keeps queue from filling
 *    - Usually: Producer = Consumer, or Producer slightly higher
 *
 *
 * 3. ERROR HANDLING
 *    - Queue full: Drop, block, or signal error
 *    - Queue empty: Block or poll
 *    - Memory allocation failure: Critical error
 *
 * ============================================================================
 * PRACTICAL EXAMPLE: Data Logger
 * ============================================================================
 */

/*
// Complete data logger with producer-consumer

typedef struct {
    uint32_t timestamp;
    uint8_t sensor_id;
    float value;
    uint8_t quality;
} LogEntry_t;

#define LOG_QUEUE_SIZE  100
#define LOG_BUFFER_SIZE 512

QueueHandle_t log_queue;

// Multiple producers (sensor tasks)
void sensor_task(void *pvParameters) {
    uint8_t sensor_id = (uint8_t)(uint32_t)pvParameters;
    LogEntry_t entry;

    for (;;) {
        entry.timestamp = xTaskGetTickCount();
        entry.sensor_id = sensor_id;
        entry.value = read_sensor(sensor_id);
        entry.quality = 100;  // 100% quality

        // Non-blocking, report drops
        if (xQueueSend(log_queue, &entry, 0) != pdPASS) {
            // Queue full - lower quality indication
            entry.quality = 0;
            // Try to send anyway (may fail)
            xQueueSend(log_queue, &entry, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Consumer: Logs to SD card (slow!)
void logger_task(void *pvParameters) {
    LogEntry_t entries[10];  // Batch for efficiency
    int count = 0;

    for (;;) {
        // Receive entry
        if (xQueueReceive(log_queue, &entries[count], pdMS_TO_TICKS(1000)) == pdPASS) {
            count++;

            // Batch write for efficiency (SD card writes are slow)
            if (count >= 10 || uxQueueMessagesWaiting(log_queue) == 0) {
                write_to_sd_card(entries, count);
                count = 0;
            }
        } else {
            // Timeout - flush partial batch
            if (count > 0) {
                write_to_sd_card(entries, count);
                count = 0;
            }
        }

        // Monitor queue health
        UBaseType_t waiting = uxQueueMessagesWaiting(log_queue);
        if (waiting > LOG_QUEUE_SIZE * 0.8) {
            printf("Warning: Log queue %.0f%% full\n",
                   (float)waiting / LOG_QUEUE_SIZE * 100);
        }
    }
}
*/

/*
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN THE DECOUPLING
 *    "Queue decouples producer and consumer timing"
 *
 * 2. DISCUSS QUEUE SIZING
 *    "Size based on max burst rate minus consumer rate"
 *
 * 3. MENTION ISR PATTERN
 *    "Very common: ISR produces, task consumes"
 *
 * 4. ADDRESS BACKPRESSURE
 *    "If queue full: drop, block, or flow control"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "What happens if the consumer is faster than the producer?"
 * A: Consumer blocks on empty queue until data arrives. No problem,
 *    just means consumer has idle time.
 *
 * Q: "Can you have multiple consumers?"
 * A: Yes, each item goes to only ONE consumer. Multiple consumers
 *    provide load balancing (work stealing pattern).
 *
 * Q: "How do you ensure FIFO ordering with multiple producers?"
 * A: Queue maintains FIFO for items as they're added. If strict
 *    timestamp ordering needed, consumer must sort.
 *
 * Q: "What if queue item is large?"
 * A: Use pointer instead of copying. Allocate from memory pool,
 *    producer puts pointer in queue, consumer frees after processing.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 27 ===\n\n");
    printf("Q: Explain Producer-Consumer Pattern\n\n");
    printf("PATTERN:\n");
    printf("  [Producer] ---> [Queue] ---> [Consumer]\n\n");
    printf("KEY POINTS:\n");
    printf("1. Queue decouples timing between tasks\n");
    printf("2. Producer can be bursty, consumer steady\n");
    printf("3. Very common: ISR produces, task consumes\n\n");
    printf("QUEUE SIZING:\n");
    printf("  Size >= (MaxProduceRate - ConsumeRate) * BurstDuration\n\n");
    printf("BACKPRESSURE HANDLING:\n");
    printf("- Drop new data (non-blocking send)\n");
    printf("- Drop oldest (circular overwrite)\n");
    printf("- Block producer (wait for space)\n");
    printf("- Flow control (signal to slow down)\n\n");
    printf("DESIGN CHOICES:\n");
    printf("- Small items: Copy into queue\n");
    printf("- Large items: Use pointers\n");
    printf("- Priority: Producer >= Consumer\n");
    return 0;
}
