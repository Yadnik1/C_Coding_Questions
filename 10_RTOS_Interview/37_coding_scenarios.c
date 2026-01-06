/*
 * ============================================================================
 * QUESTION 37: Common RTOS Coding Scenarios (Interview Code Questions)
 * ============================================================================
 *
 * DIFFICULTY: Medium-Hard | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * Common coding problems you might be asked to write in an interview.
 *
 * ============================================================================
 * SCENARIO 1: ISR to Task Communication
 * ============================================================================
 *
 * "Write code to safely pass data from an ISR to a task"
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// SOLUTION 1: Binary Semaphore (for signaling only)
SemaphoreHandle_t data_ready_sem;
volatile uint8_t shared_data;

void UART_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Read data from hardware
    shared_data = UART->DR;
    UART->SR = 0;  // Clear interrupt

    // Signal task
    xSemaphoreGiveFromISR(data_ready_sem, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_task(void *pvParameters) {
    data_ready_sem = xSemaphoreCreateBinary();

    for (;;) {
        // Wait for signal from ISR
        if (xSemaphoreTake(data_ready_sem, portMAX_DELAY) == pdTRUE) {
            uint8_t local_data = shared_data;  // Copy to local
            process_byte(local_data);
        }
    }
}


// SOLUTION 2: Queue (passes data directly)
QueueHandle_t uart_queue;

void UART_IRQHandler_Queue(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t data = UART->DR;

    xQueueSendFromISR(uart_queue, &data, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_task_queue(void *pvParameters) {
    uint8_t data;
    uart_queue = xQueueCreate(64, sizeof(uint8_t));

    for (;;) {
        if (xQueueReceive(uart_queue, &data, portMAX_DELAY) == pdTRUE) {
            process_byte(data);
        }
    }
}
*/

/*
 * ============================================================================
 * SCENARIO 2: Thread-Safe Counter (Atomic Operations)
 * ============================================================================
 *
 * "Implement a thread-safe counter that can be incremented from multiple tasks"
 */

/*
// SOLUTION 1: Using Mutex
typedef struct {
    int32_t value;
    SemaphoreHandle_t mutex;
} SafeCounter_t;

void counter_init(SafeCounter_t *counter, int32_t initial) {
    counter->value = initial;
    counter->mutex = xSemaphoreCreateMutex();
}

void counter_increment(SafeCounter_t *counter) {
    xSemaphoreTake(counter->mutex, portMAX_DELAY);
    counter->value++;
    xSemaphoreGive(counter->mutex);
}

int32_t counter_get(SafeCounter_t *counter) {
    int32_t val;
    xSemaphoreTake(counter->mutex, portMAX_DELAY);
    val = counter->value;
    xSemaphoreGive(counter->mutex);
    return val;
}


// SOLUTION 2: Using Critical Section (faster for simple ops)
volatile int32_t global_counter = 0;

void counter_increment_critical(void) {
    taskENTER_CRITICAL();
    global_counter++;
    taskEXIT_CRITICAL();
}

// SOLUTION 3: Using Atomic (if available)
// __atomic_add_fetch(&global_counter, 1, __ATOMIC_SEQ_CST);
*/

/*
 * ============================================================================
 * SCENARIO 3: Periodic Task
 * ============================================================================
 *
 * "Write a task that executes exactly every 100ms"
 */

/*
void periodic_task(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(100);

    for (;;) {
        // Do work
        read_sensor();
        update_output();

        // Wait for next period (maintains consistent timing!)
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

// WRONG WAY (timing drift):
void periodic_task_wrong(void *pvParameters) {
    for (;;) {
        read_sensor();        // Takes variable time!
        update_output();
        vTaskDelay(pdMS_TO_TICKS(100));  // 100ms FROM NOW, not from start!
    }
}
*/

/*
 * ============================================================================
 * SCENARIO 4: Resource Pool with Counting Semaphore
 * ============================================================================
 *
 * "Implement a pool of 5 resources that multiple tasks can acquire"
 */

/*
#define POOL_SIZE 5

typedef struct {
    void *resources[POOL_SIZE];
    SemaphoreHandle_t count_sem;
    SemaphoreHandle_t mutex;
    uint8_t free_list[POOL_SIZE];
    uint8_t free_count;
} ResourcePool_t;

void pool_init(ResourcePool_t *pool) {
    pool->count_sem = xSemaphoreCreateCounting(POOL_SIZE, POOL_SIZE);
    pool->mutex = xSemaphoreCreateMutex();
    pool->free_count = POOL_SIZE;
    for (int i = 0; i < POOL_SIZE; i++) {
        pool->resources[i] = allocate_resource();
        pool->free_list[i] = i;
    }
}

void* pool_acquire(ResourcePool_t *pool, TickType_t timeout) {
    // Wait for available resource
    if (xSemaphoreTake(pool->count_sem, timeout) != pdTRUE) {
        return NULL;  // Timeout
    }

    // Get resource from pool
    void *resource;
    xSemaphoreTake(pool->mutex, portMAX_DELAY);
    {
        int index = pool->free_list[--pool->free_count];
        resource = pool->resources[index];
    }
    xSemaphoreGive(pool->mutex);

    return resource;
}

void pool_release(ResourcePool_t *pool, void *resource) {
    // Find resource index
    xSemaphoreTake(pool->mutex, portMAX_DELAY);
    {
        for (int i = 0; i < POOL_SIZE; i++) {
            if (pool->resources[i] == resource) {
                pool->free_list[pool->free_count++] = i;
                break;
            }
        }
    }
    xSemaphoreGive(pool->mutex);

    // Signal resource available
    xSemaphoreGive(pool->count_sem);
}
*/

/*
 * ============================================================================
 * SCENARIO 5: Bounded Buffer (Producer-Consumer)
 * ============================================================================
 *
 * "Implement a bounded buffer with multiple producers and consumers"
 */

/*
#define BUFFER_SIZE 10

typedef struct {
    int items[BUFFER_SIZE];
    int head;
    int tail;
    int count;
    SemaphoreHandle_t mutex;
    SemaphoreHandle_t empty_slots;  // Producer waits on this
    SemaphoreHandle_t full_slots;   // Consumer waits on this
} BoundedBuffer_t;

void buffer_init(BoundedBuffer_t *buf) {
    buf->head = 0;
    buf->tail = 0;
    buf->count = 0;
    buf->mutex = xSemaphoreCreateMutex();
    buf->empty_slots = xSemaphoreCreateCounting(BUFFER_SIZE, BUFFER_SIZE);
    buf->full_slots = xSemaphoreCreateCounting(BUFFER_SIZE, 0);
}

void buffer_put(BoundedBuffer_t *buf, int item) {
    // Wait for empty slot
    xSemaphoreTake(buf->empty_slots, portMAX_DELAY);

    // Add item
    xSemaphoreTake(buf->mutex, portMAX_DELAY);
    buf->items[buf->head] = item;
    buf->head = (buf->head + 1) % BUFFER_SIZE;
    buf->count++;
    xSemaphoreGive(buf->mutex);

    // Signal item available
    xSemaphoreGive(buf->full_slots);
}

int buffer_get(BoundedBuffer_t *buf) {
    // Wait for item
    xSemaphoreTake(buf->full_slots, portMAX_DELAY);

    // Get item
    int item;
    xSemaphoreTake(buf->mutex, portMAX_DELAY);
    item = buf->items[buf->tail];
    buf->tail = (buf->tail + 1) % BUFFER_SIZE;
    buf->count--;
    xSemaphoreGive(buf->mutex);

    // Signal slot available
    xSemaphoreGive(buf->empty_slots);

    return item;
}
*/

/*
 * ============================================================================
 * SCENARIO 6: Timeout with Retry
 * ============================================================================
 *
 * "Write a function that waits for data with timeout and retries 3 times"
 */

/*
#define MAX_RETRIES 3
#define RETRY_TIMEOUT_MS 1000

int wait_for_data_with_retry(QueueHandle_t queue, void *data) {
    for (int attempt = 0; attempt < MAX_RETRIES; attempt++) {
        if (xQueueReceive(queue, data, pdMS_TO_TICKS(RETRY_TIMEOUT_MS)) == pdTRUE) {
            return 0;  // Success
        }
        printf("Retry %d/%d\n", attempt + 1, MAX_RETRIES);
    }
    return -1;  // Failed after all retries
}

// With exponential backoff
int wait_with_backoff(QueueHandle_t queue, void *data) {
    TickType_t timeout = pdMS_TO_TICKS(100);  // Start with 100ms

    for (int attempt = 0; attempt < MAX_RETRIES; attempt++) {
        if (xQueueReceive(queue, data, timeout) == pdTRUE) {
            return 0;
        }
        timeout *= 2;  // Double timeout each retry
        printf("Retry %d, next timeout: %lu ticks\n", attempt + 1, timeout);
    }
    return -1;
}
*/

/*
 * ============================================================================
 * SCENARIO 7: Event-Driven State Machine
 * ============================================================================
 *
 * "Implement a simple state machine that handles events from a queue"
 */

/*
typedef enum { STATE_IDLE, STATE_RUNNING, STATE_ERROR } State_t;
typedef enum { EVENT_START, EVENT_STOP, EVENT_ERROR, EVENT_RESET } Event_t;

typedef struct {
    Event_t type;
    void *data;
} EventMsg_t;

QueueHandle_t event_queue;
State_t current_state = STATE_IDLE;

void state_machine_task(void *pvParameters) {
    EventMsg_t event;

    for (;;) {
        // Wait for event
        if (xQueueReceive(event_queue, &event, portMAX_DELAY) != pdTRUE) {
            continue;
        }

        // Process based on state
        switch (current_state) {
            case STATE_IDLE:
                if (event.type == EVENT_START) {
                    current_state = STATE_RUNNING;
                    start_operation();
                }
                break;

            case STATE_RUNNING:
                if (event.type == EVENT_STOP) {
                    current_state = STATE_IDLE;
                    stop_operation();
                } else if (event.type == EVENT_ERROR) {
                    current_state = STATE_ERROR;
                    handle_error();
                }
                break;

            case STATE_ERROR:
                if (event.type == EVENT_RESET) {
                    current_state = STATE_IDLE;
                    reset_error();
                }
                break;
        }
    }
}

// Helper to post events
void post_event(Event_t type) {
    EventMsg_t event = {.type = type, .data = NULL};
    xQueueSend(event_queue, &event, portMAX_DELAY);
}
*/

/*
 * ============================================================================
 * SCENARIO 8: Watchdog Task Monitor
 * ============================================================================
 *
 * "Implement a watchdog that monitors multiple tasks"
 */

/*
#define NUM_TASKS 4
#define CHECK_INTERVAL_MS 500
#define WATCHDOG_TIMEOUT_MS 1000

volatile uint32_t task_checkin_flags = 0;

#define TASK_0_BIT (1 << 0)
#define TASK_1_BIT (1 << 1)
#define TASK_2_BIT (1 << 2)
#define TASK_3_BIT (1 << 3)
#define ALL_TASKS_MASK (TASK_0_BIT | TASK_1_BIT | TASK_2_BIT | TASK_3_BIT)

void task_checkin(uint32_t task_bit) {
    taskENTER_CRITICAL();
    task_checkin_flags |= task_bit;
    taskEXIT_CRITICAL();
}

void watchdog_task(void *pvParameters) {
    hw_watchdog_init(WATCHDOG_TIMEOUT_MS);

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(CHECK_INTERVAL_MS));

        taskENTER_CRITICAL();
        uint32_t flags = task_checkin_flags;
        task_checkin_flags = 0;  // Reset for next cycle
        taskEXIT_CRITICAL();

        if ((flags & ALL_TASKS_MASK) == ALL_TASKS_MASK) {
            // All tasks healthy - kick watchdog
            hw_watchdog_kick();
        } else {
            // Some task didn't check in!
            printf("Missing tasks: 0x%lX\n", ALL_TASKS_MASK & ~flags);
            // Don't kick - let watchdog reset system
        }
    }
}

// Each monitored task calls checkin periodically
void task_0(void *pvParameters) {
    for (;;) {
        do_work();
        task_checkin(TASK_0_BIT);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
*/

/*
 * ============================================================================
 * SCENARIO 9: Priority Ceiling Protocol (Manual)
 * ============================================================================
 *
 * "Implement priority ceiling to prevent priority inversion"
 */

/*
#define CEILING_PRIORITY (configMAX_PRIORITIES - 1)

typedef struct {
    SemaphoreHandle_t mutex;
    UBaseType_t ceiling_priority;
} CeilingMutex_t;

UBaseType_t saved_priority;

void ceiling_mutex_take(CeilingMutex_t *cm) {
    saved_priority = uxTaskPriorityGet(NULL);

    // Raise to ceiling
    vTaskPrioritySet(NULL, cm->ceiling_priority);

    xSemaphoreTake(cm->mutex, portMAX_DELAY);
}

void ceiling_mutex_give(CeilingMutex_t *cm) {
    xSemaphoreGive(cm->mutex);

    // Restore original priority
    vTaskPrioritySet(NULL, saved_priority);
}

// Note: FreeRTOS mutex already has priority inheritance
// This is just to show you understand the concept!
*/

/*
 * ============================================================================
 * SCENARIO 10: Double Buffer for ADC
 * ============================================================================
 *
 * "Implement double buffering for continuous ADC sampling"
 */

/*
#define BUFFER_SIZE 256

typedef struct {
    uint16_t buffer[2][BUFFER_SIZE];
    volatile uint8_t write_buffer;  // ISR writes to this
    volatile uint8_t read_buffer;   // Task reads from this
    SemaphoreHandle_t buffer_ready;
} DoubleBuffer_t;

DoubleBuffer_t adc_buffer;

void adc_init(void) {
    adc_buffer.write_buffer = 0;
    adc_buffer.read_buffer = 1;
    adc_buffer.buffer_ready = xSemaphoreCreateBinary();

    // Configure DMA to fill write_buffer
    configure_dma(&adc_buffer.buffer[0][0], BUFFER_SIZE);
}

// Called when DMA completes filling a buffer
void DMA_IRQHandler(void) {
    BaseType_t wake = pdFALSE;

    // Swap buffers
    uint8_t temp = adc_buffer.write_buffer;
    adc_buffer.write_buffer = adc_buffer.read_buffer;
    adc_buffer.read_buffer = temp;

    // Configure DMA for new write buffer
    configure_dma(&adc_buffer.buffer[adc_buffer.write_buffer][0], BUFFER_SIZE);

    // Signal task
    xSemaphoreGiveFromISR(adc_buffer.buffer_ready, &wake);
    portYIELD_FROM_ISR(wake);
}

void processing_task(void *pvParameters) {
    for (;;) {
        xSemaphoreTake(adc_buffer.buffer_ready, portMAX_DELAY);

        // Process read buffer (safe - DMA is filling the other one)
        uint16_t *data = adc_buffer.buffer[adc_buffer.read_buffer];
        process_adc_data(data, BUFFER_SIZE);
    }
}
*/

/*
 * ============================================================================
 * INTERVIEW CODING TIPS
 * ============================================================================
 *
 * 1. ALWAYS INITIALIZE
 *    - Create semaphore/queue before using
 *    - Check creation return value
 *
 * 2. USE CORRECT API
 *    - FromISR variants in ISR
 *    - Always call portYIELD_FROM_ISR
 *
 * 3. CONSIDER EDGE CASES
 *    - What if queue is full?
 *    - What if timeout occurs?
 *    - What if task is deleted?
 *
 * 4. EXPLAIN YOUR CODE
 *    - Comment critical sections
 *    - Explain synchronization choices
 *    - Discuss alternatives
 *
 * 5. MENTION IMPROVEMENTS
 *    - Error handling
 *    - Edge cases
 *    - Performance optimization
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 37 ===\n\n");
    printf("Common RTOS Coding Scenarios\n\n");
    printf("SCENARIOS COVERED:\n");
    printf("1.  ISR to Task Communication\n");
    printf("2.  Thread-Safe Counter\n");
    printf("3.  Periodic Task (vTaskDelayUntil)\n");
    printf("4.  Resource Pool with Counting Semaphore\n");
    printf("5.  Bounded Buffer (Producer-Consumer)\n");
    printf("6.  Timeout with Retry\n");
    printf("7.  Event-Driven State Machine\n");
    printf("8.  Watchdog Task Monitor\n");
    printf("9.  Priority Ceiling Protocol\n");
    printf("10. Double Buffer for ADC\n\n");
    printf("KEY INTERVIEW TIPS:\n");
    printf("- Always use FromISR variants in ISR\n");
    printf("- Check return values\n");
    printf("- Explain synchronization choices\n");
    printf("- Discuss edge cases and improvements\n");
    return 0;
}
