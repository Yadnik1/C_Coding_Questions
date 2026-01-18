/*
 * ============================================================================
 *                    FreeRTOS BASICS FOR EMBEDDED SYSTEMS
 * ============================================================================
 *
 * CRITICAL FOR IRONSITE:
 *   - Camera capture task
 *   - Sensor reading task
 *   - Wireless communication task
 *   - UI/LED control task
 *   - All running "simultaneously"
 *
 * ============================================================================
 *                    WHY AN RTOS?
 * ============================================================================
 *
 * WITHOUT RTOS (Super loop):
 *
 *   while(1) {
 *       read_sensors();      // Takes 5ms
 *       process_video();     // Takes 50ms - BLOCKING!
 *       check_buttons();     // Missed presses during video!
 *       send_wireless();     // Takes 10ms
 *   }
 *
 *   PROBLEMS:
 *   - One slow task blocks everything
 *   - Timing is unpredictable
 *   - Can't prioritize urgent tasks
 *   - Hard to maintain as system grows
 *
 * WITH RTOS:
 *
 *   Task 1 (High priority): Button handling - responds immediately
 *   Task 2 (Medium priority): Sensor reading - runs periodically
 *   Task 3 (Low priority): Video processing - runs when CPU free
 *   Task 4 (Low priority): Wireless - runs when CPU free
 *
 *   BENEFITS:
 *   - Each task is independent
 *   - Scheduler handles timing
 *   - Priority ensures urgent tasks run first
 *   - Easier to add/modify features
 *
 * ============================================================================
 *                    RTOS CONCEPTS
 * ============================================================================
 *
 * TASK (Thread):
 *   - Independent function that runs "forever" (while(1) loop)
 *   - Has its own stack
 *   - Has a priority
 *   - Can be in states: Running, Ready, Blocked, Suspended
 *
 * SCHEDULER:
 *   - Decides which task runs
 *   - Preemptive: Higher priority task interrupts lower
 *   - Runs at tick rate (typically 1ms)
 *
 * TICK:
 *   - Periodic timer interrupt (configTICK_RATE_HZ)
 *   - Scheduler runs at each tick
 *   - Basis for time delays
 *
 * CONTEXT SWITCH:
 *   - Saving current task state, loading another task
 *   - Fast (few microseconds) but has overhead
 *
 * ============================================================================
 *                    TASK STATES
 * ============================================================================
 *
 *                    +----------+
 *              +---->| RUNNING  |<----+
 *              |     +----------+     |
 *              |          |           |
 *       Scheduled     Preempted    Unblocked
 *              |          |           |
 *              |     +----v-----+     |
 *              +-----+  READY   +-----+
 *                    +----+-----+
 *                         |
 *                    Block (wait for
 *                    event/delay)
 *                         |
 *                    +----v-----+
 *                    | BLOCKED  |  (waiting for semaphore,
 *                    +----------+   queue, delay, event)
 *
 *   RUNNING:  Currently executing (only one task at a time)
 *   READY:    Can run, waiting for scheduler
 *   BLOCKED:  Waiting for something (delay, semaphore, queue)
 *   SUSPENDED: Manually paused (vTaskSuspend)
 *
 * ============================================================================
 *                    CREATING TASKS
 * ============================================================================
 *
 * TASK FUNCTION:
 *
 *   void task_function(void* parameters) {
 *       // Initialize
 *
 *       while(1) {  // Tasks run forever!
 *           // Do work
 *
 *           vTaskDelay(pdMS_TO_TICKS(100));  // Don't hog CPU!
 *       }
 *
 *       // Never reaches here, but good practice:
 *       vTaskDelete(NULL);
 *   }
 *
 * CREATE TASK:
 *
 *   xTaskCreate(
 *       task_function,      // Function pointer
 *       "TaskName",         // Name (for debugging)
 *       256,                // Stack size (words, not bytes!)
 *       NULL,               // Parameters to pass
 *       2,                  // Priority (higher = more important)
 *       &task_handle        // Handle for later reference
 *   );
 *
 * ============================================================================
 *                    DELAYS AND TIMING
 * ============================================================================
 *
 * vTaskDelay(ticks):
 *   - Blocks task for at least 'ticks' tick periods
 *   - Allows other tasks to run
 *   - NOT precise timing (depends when last woke)
 *
 * vTaskDelayUntil(&lastWakeTime, period):
 *   - Precise periodic timing
 *   - Compensates for task execution time
 *   - Use for regular sampling
 *
 *   void sensor_task(void* p) {
 *       TickType_t lastWake = xTaskGetTickCount();
 *
 *       while(1) {
 *           read_sensor();
 *           vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(10));  // Exactly 100Hz
 *       }
 *   }
 *
 * ============================================================================
 *                    QUEUES (Inter-task communication)
 * ============================================================================
 *
 * WHAT: Thread-safe FIFO for passing data between tasks
 *
 * CREATE:
 *   QueueHandle_t queue = xQueueCreate(10, sizeof(SensorData));
 *                                       ^         ^
 *                                   length    item size
 *
 * SEND (from producer task):
 *   SensorData data = {.temp = 25, .humidity = 60};
 *   xQueueSend(queue, &data, pdMS_TO_TICKS(100));
 *                            ^timeout (blocks if full)
 *
 * RECEIVE (from consumer task):
 *   SensorData data;
 *   if (xQueueReceive(queue, &data, pdMS_TO_TICKS(100)) == pdTRUE) {
 *       // Process data
 *   }
 *
 * USE CASE:
 *   - Sensor task puts readings in queue
 *   - Processing task takes from queue
 *   - Decouples producer from consumer
 *
 * ============================================================================
 *                    SEMAPHORES
 * ============================================================================
 *
 * BINARY SEMAPHORE (Signaling):
 *   - Like a flag: taken (0) or given (1)
 *   - ISR signals task, task waits for signal
 *
 *   SemaphoreHandle_t sem = xSemaphoreCreateBinary();
 *
 *   // In ISR:
 *   xSemaphoreGiveFromISR(sem, &higherPrioWoken);
 *
 *   // In task:
 *   if (xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE) {
 *       // ISR signaled us!
 *   }
 *
 * COUNTING SEMAPHORE:
 *   - Can be given multiple times
 *   - Track number of available resources
 *
 * MUTEX (Mutual Exclusion):
 *   - Protects shared resource
 *   - Only one task can hold at a time
 *   - Has priority inheritance (prevents priority inversion)
 *
 *   SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
 *
 *   // Task 1:
 *   xSemaphoreTake(mutex, portMAX_DELAY);
 *   // Access shared resource (e.g., I2C bus)
 *   xSemaphoreGive(mutex);
 *
 *   // Task 2:
 *   xSemaphoreTake(mutex, portMAX_DELAY);  // Blocks until Task 1 releases
 *   // Access shared resource
 *   xSemaphoreGive(mutex);
 *
 * ============================================================================
 *                    ISR SAFE FUNCTIONS
 * ============================================================================
 *
 * IMPORTANT: Regular FreeRTOS functions can't be called from ISR!
 *
 * Use these instead:
 *   xSemaphoreGiveFromISR()     instead of xSemaphoreGive()
 *   xQueueSendFromISR()         instead of xQueueSend()
 *   xTaskNotifyFromISR()        instead of xTaskNotify()
 *
 * Pattern:
 *
 *   void UART_IRQHandler(void) {
 *       BaseType_t higherPrioWoken = pdFALSE;
 *
 *       uint8_t data = UART->DR;
 *       xQueueSendFromISR(rx_queue, &data, &higherPrioWoken);
 *
 *       portYIELD_FROM_ISR(higherPrioWoken);  // Context switch if needed
 *   }
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// FreeRTOS types (simplified for demo)
typedef void* TaskHandle_t;
typedef void* QueueHandle_t;
typedef void* SemaphoreHandle_t;
typedef uint32_t TickType_t;
typedef int BaseType_t;

#define pdTRUE  1
#define pdFALSE 0
#define pdMS_TO_TICKS(ms) (ms)
#define portMAX_DELAY 0xFFFFFFFF

// ============================================================================
//                    EXAMPLE: IRONSITE-LIKE MULTI-TASK SYSTEM
// ============================================================================

/*
 * IRONSITE WEARABLE CAMERA TASKS:
 *
 * +----------------+   +----------------+   +----------------+
 * |  CAMERA TASK   |   |  SENSOR TASK   |   | WIRELESS TASK  |
 * |  Priority: 3   |   |  Priority: 2   |   |  Priority: 1   |
 * +-------+--------+   +-------+--------+   +-------+--------+
 *         |                    |                    |
 *         v                    v                    v
 *   +-----------+        +-----------+        +-----------+
 *   |Frame Queue|        |Sensor Queue|       |TX Queue   |
 *   +-----------+        +-----------+        +-----------+
 *         |                    |                    |
 *         v                    v                    v
 * +----------------+   +----------------+   +----------------+
 * | STORAGE TASK   |   | PROCESSING     |   | UPLOAD TASK    |
 * | Priority: 1    |   | TASK Prio: 2   |   | Priority: 0    |
 * +----------------+   +----------------+   +----------------+
 *
 */

// Task priorities
#define PRIORITY_BUTTON     4  // Highest - user input
#define PRIORITY_CAMERA     3  // High - can't miss frames
#define PRIORITY_SENSOR     2  // Medium - periodic sampling
#define PRIORITY_WIRELESS   1  // Lower - can be delayed
#define PRIORITY_STORAGE    1  // Lower - background
#define PRIORITY_IDLE       0  // Lowest

// Data structures
typedef struct {
    uint32_t timestamp;
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
} IMUData;

typedef struct {
    uint32_t frame_number;
    uint32_t timestamp;
    uint8_t* data_ptr;
    uint16_t width, height;
} FrameInfo;

// Simulated queues
#define QUEUE_SIZE 10
static IMUData imu_queue[QUEUE_SIZE];
static int imu_queue_count = 0;

static FrameInfo frame_queue[QUEUE_SIZE];
static int frame_queue_count = 0;

// Simulated semaphores
static bool button_semaphore = false;
static bool i2c_mutex = true;  // true = available

// ============================================================================
//                    TASK IMPLEMENTATIONS
// ============================================================================

void button_task(void* params) {
    printf("[BUTTON TASK] Started (Priority %d)\n", PRIORITY_BUTTON);

    while(1) {
        // Wait for button press (semaphore from ISR)
        printf("[BUTTON] Waiting for press...\n");
        // xSemaphoreTake(button_sem, portMAX_DELAY);

        printf("[BUTTON] Button pressed! Toggling recording\n");

        // Simulate delay
        // vTaskDelay(pdMS_TO_TICKS(50));  // Debounce
    }
}

void camera_task(void* params) {
    printf("[CAMERA TASK] Started (Priority %d)\n", PRIORITY_CAMERA);

    uint32_t frame_number = 0;
    // TickType_t lastWake = xTaskGetTickCount();

    while(1) {
        // Capture frame (simulated)
        FrameInfo frame = {
            .frame_number = frame_number++,
            .timestamp = 0,  // xTaskGetTickCount()
            .width = 640,
            .height = 480
        };

        printf("[CAMERA] Captured frame %lu\n", (unsigned long)frame.frame_number);

        // Send to processing queue
        if (frame_queue_count < QUEUE_SIZE) {
            frame_queue[frame_queue_count++] = frame;
        } else {
            printf("[CAMERA] WARNING: Frame queue full, dropping frame!\n");
        }

        // Run at 30 FPS
        // vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(33));
        break;  // Demo: run once
    }
}

void sensor_task(void* params) {
    printf("[SENSOR TASK] Started (Priority %d)\n", PRIORITY_SENSOR);

    // TickType_t lastWake = xTaskGetTickCount();

    while(1) {
        // Take I2C mutex
        printf("[SENSOR] Taking I2C mutex...\n");
        // xSemaphoreTake(i2c_mutex, portMAX_DELAY);

        // Read IMU (simulated)
        IMUData data = {
            .timestamp = 0,
            .accel_x = 100, .accel_y = -50, .accel_z = 980,
            .gyro_x = 5, .gyro_y = -3, .gyro_z = 1
        };

        printf("[SENSOR] Read IMU: ax=%d ay=%d az=%d\n",
               data.accel_x, data.accel_y, data.accel_z);

        // Release I2C mutex
        // xSemaphoreGive(i2c_mutex);

        // Send to queue
        if (imu_queue_count < QUEUE_SIZE) {
            imu_queue[imu_queue_count++] = data;
        }

        // Run at 100 Hz
        // vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(10));
        break;  // Demo: run once
    }
}

void processing_task(void* params) {
    printf("[PROCESSING TASK] Started (Priority %d)\n", PRIORITY_SENSOR);

    while(1) {
        // Wait for sensor data
        printf("[PROCESS] Waiting for sensor data...\n");

        if (imu_queue_count > 0) {
            IMUData data = imu_queue[--imu_queue_count];

            // Check for fall
            int32_t magnitude = data.accel_x * data.accel_x +
                               data.accel_y * data.accel_y +
                               data.accel_z * data.accel_z;

            printf("[PROCESS] Processing IMU, magnitude^2 = %ld\n", (long)magnitude);

            if (magnitude < 100000) {  // Arbitrary threshold
                printf("[PROCESS] POTENTIAL FALL DETECTED!\n");
            }
        }

        break;  // Demo: run once
    }
}

void wireless_task(void* params) {
    printf("[WIRELESS TASK] Started (Priority %d)\n", PRIORITY_WIRELESS);

    while(1) {
        printf("[WIRELESS] Checking for data to send...\n");

        // Check connection status
        // Send queued data
        // Handle incoming commands

        // vTaskDelay(pdMS_TO_TICKS(100));
        break;  // Demo: run once
    }
}

// ============================================================================
//                    COMMON PATTERNS
// ============================================================================

/*
 * PATTERN 1: ISR to Task signaling
 *
 *   SemaphoreHandle_t data_ready = xSemaphoreCreateBinary();
 *
 *   void ADC_IRQHandler(void) {
 *       adc_value = ADC->DR;
 *       xSemaphoreGiveFromISR(data_ready, &woken);
 *       portYIELD_FROM_ISR(woken);
 *   }
 *
 *   void adc_task(void* p) {
 *       while(1) {
 *           xSemaphoreTake(data_ready, portMAX_DELAY);
 *           process(adc_value);
 *       }
 *   }
 */

/*
 * PATTERN 2: Producer-Consumer with Queue
 *
 *   QueueHandle_t sensor_queue = xQueueCreate(10, sizeof(SensorData));
 *
 *   void producer_task(void* p) {
 *       while(1) {
 *           SensorData data = read_sensor();
 *           xQueueSend(sensor_queue, &data, portMAX_DELAY);
 *           vTaskDelay(pdMS_TO_TICKS(10));
 *       }
 *   }
 *
 *   void consumer_task(void* p) {
 *       SensorData data;
 *       while(1) {
 *           xQueueReceive(sensor_queue, &data, portMAX_DELAY);
 *           process(data);
 *       }
 *   }
 */

/*
 * PATTERN 3: Mutex for shared resource
 *
 *   SemaphoreHandle_t i2c_mutex = xSemaphoreCreateMutex();
 *
 *   void read_sensor_1(void) {
 *       xSemaphoreTake(i2c_mutex, portMAX_DELAY);
 *       i2c_read(SENSOR1_ADDR, ...);
 *       xSemaphoreGive(i2c_mutex);
 *   }
 *
 *   void read_sensor_2(void) {
 *       xSemaphoreTake(i2c_mutex, portMAX_DELAY);
 *       i2c_read(SENSOR2_ADDR, ...);
 *       xSemaphoreGive(i2c_mutex);
 *   }
 */

// ============================================================================
//                    DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   FreeRTOS BASICS DEMO\n");
    printf("============================================\n\n");

    // Simulate creating tasks
    printf("Creating tasks...\n\n");

    // Simulate task execution
    button_task(NULL);
    camera_task(NULL);
    sensor_task(NULL);
    processing_task(NULL);
    wireless_task(NULL);

    printf("\n============================================\n");
    printf("   FreeRTOS KEY POINTS FOR INTERVIEW\n");
    printf("============================================\n");
    printf("- Task = independent thread with own stack\n");
    printf("- Scheduler runs highest priority READY task\n");
    printf("- vTaskDelay() releases CPU, allows other tasks\n");
    printf("- Queue = thread-safe FIFO for passing data\n");
    printf("- Semaphore = signaling between ISR and task\n");
    printf("- Mutex = protect shared resources\n");
    printf("- Use FromISR() variants in interrupt handlers\n");
    printf("- Stack overflow is common bug - size carefully!\n");

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW Q&A
 * ============================================================================
 *
 * Q: What is an RTOS and why use one?
 * A: "An RTOS provides multitasking with deterministic timing. Instead of
 *    a super loop where one function blocks everything, each task runs
 *    independently. The scheduler ensures high priority tasks run first.
 *    For Ironsite, we need camera capture, sensor reading, and wireless
 *    all running 'simultaneously' with predictable timing."
 *
 * Q: Explain task states in FreeRTOS.
 * A: "Running is the task currently executing - only one at a time.
 *    Ready means the task can run but is waiting for the scheduler.
 *    Blocked means waiting for something - a delay, semaphore, or queue.
 *    The scheduler always picks the highest priority Ready task."
 *
 * Q: What's the difference between a semaphore and mutex?
 * A: "A semaphore is for signaling - typically ISR gives it, task takes it.
 *    A mutex is for mutual exclusion - protects shared resources like I2C.
 *    Mutex has priority inheritance to prevent priority inversion - if a
 *    low priority task holds the mutex, it temporarily gets boosted."
 *
 * Q: How do you communicate between tasks?
 * A: "Queues are the main mechanism - thread-safe FIFO for passing data.
 *    Producer task sends, consumer task receives. For simple signaling
 *    without data, binary semaphores work. For shared resources, mutex."
 *
 * Q: What's priority inversion and how to prevent it?
 * A: "When a high priority task waits for a resource held by low priority
 *    task, and medium priority task preempts the low priority one. The
 *    high priority task is stuck. FreeRTOS mutex has priority inheritance
 *    - the low priority task temporarily inherits high priority until
 *    it releases the mutex."
 *
 * Q: How do you call FreeRTOS functions from an ISR?
 * A: "Use the FromISR variants - xQueueSendFromISR, xSemaphoreGiveFromISR.
 *    They don't block and return whether a context switch is needed.
 *    Call portYIELD_FROM_ISR at the end to switch to the woken task."
 *
 * Q: What's a common FreeRTOS bug?
 * A: "Stack overflow - each task has fixed stack and if it overflows,
 *    memory gets corrupted. Enable configCHECK_FOR_STACK_OVERFLOW for
 *    debugging. Also forgetting to yield - tasks must call vTaskDelay
 *    or block on something, otherwise lower priority tasks starve."
 *
 * ============================================================================
 */
