/*
 * ============================================================================
 * QUESTION 34: FreeRTOS API Quick Reference (Interview Cheat Sheet)
 * ============================================================================
 *
 * DIFFICULTY: Reference | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * Quick reference for commonly asked FreeRTOS APIs in interviews.
 *
 * ============================================================================
 * TASK MANAGEMENT
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "task.h"

// ============================================================================
// TASK CREATION
// ============================================================================

// Dynamic allocation
BaseType_t xTaskCreate(
    TaskFunction_t pvTaskCode,      // Task function
    const char *pcName,             // Task name (for debug)
    uint16_t usStackDepth,          // Stack size in WORDS (not bytes!)
    void *pvParameters,             // Parameter to task
    UBaseType_t uxPriority,         // Priority (0 = lowest)
    TaskHandle_t *pxCreatedTask     // Handle output (can be NULL)
);

// Static allocation (no heap)
TaskHandle_t xTaskCreateStatic(
    TaskFunction_t pvTaskCode,
    const char *pcName,
    uint32_t ulStackDepth,
    void *pvParameters,
    UBaseType_t uxPriority,
    StackType_t *puxStackBuffer,    // Pre-allocated stack
    StaticTask_t *pxTaskBuffer      // Pre-allocated TCB
);

// Example:
void my_task(void *pvParameters) {
    int id = (int)pvParameters;
    for (;;) {
        do_work(id);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void create_tasks(void) {
    xTaskCreate(my_task, "Task1", 256, (void*)1, 2, NULL);
    xTaskCreate(my_task, "Task2", 256, (void*)2, 3, NULL);
}


// ============================================================================
// TASK CONTROL
// ============================================================================

void vTaskDelay(TickType_t xTicksToDelay);
    // Delay for N ticks FROM NOW

void vTaskDelayUntil(TickType_t *pxPreviousWakeTime, TickType_t xTimeIncrement);
    // Delay until specific tick (for periodic tasks)

void vTaskSuspend(TaskHandle_t xTask);
    // Suspend task (NULL = self)

void vTaskResume(TaskHandle_t xTask);
    // Resume suspended task

BaseType_t xTaskResumeFromISR(TaskHandle_t xTask);
    // Resume from ISR

void vTaskDelete(TaskHandle_t xTask);
    // Delete task (NULL = self)

void vTaskPrioritySet(TaskHandle_t xTask, UBaseType_t uxNewPriority);
    // Change priority

UBaseType_t uxTaskPriorityGet(TaskHandle_t xTask);
    // Get priority


// ============================================================================
// TASK INFO
// ============================================================================

TaskHandle_t xTaskGetCurrentTaskHandle(void);
    // Get current task's handle

char* pcTaskGetName(TaskHandle_t xTask);
    // Get task name

UBaseType_t uxTaskGetStackHighWaterMark(TaskHandle_t xTask);
    // Get minimum free stack ever

UBaseType_t uxTaskGetNumberOfTasks(void);
    // Get total task count

TickType_t xTaskGetTickCount(void);
    // Get current tick count

TickType_t xTaskGetTickCountFromISR(void);
    // Get tick count from ISR
*/

/*
 * ============================================================================
 * QUEUE MANAGEMENT
 * ============================================================================
 */

/*
#include "queue.h"

// Creation
QueueHandle_t xQueueCreate(UBaseType_t uxQueueLength, UBaseType_t uxItemSize);
QueueHandle_t xQueueCreateStatic(...);

// Send (add to back)
BaseType_t xQueueSend(QueueHandle_t xQueue, const void *pvItemToQueue,
                      TickType_t xTicksToWait);
BaseType_t xQueueSendFromISR(QueueHandle_t xQueue, const void *pvItemToQueue,
                              BaseType_t *pxHigherPriorityTaskWoken);

// Send to front
BaseType_t xQueueSendToFront(QueueHandle_t xQueue, const void *pvItemToQueue,
                              TickType_t xTicksToWait);
BaseType_t xQueueSendToFrontFromISR(...);

// Receive
BaseType_t xQueueReceive(QueueHandle_t xQueue, void *pvBuffer,
                         TickType_t xTicksToWait);
BaseType_t xQueueReceiveFromISR(QueueHandle_t xQueue, void *pvBuffer,
                                 BaseType_t *pxHigherPriorityTaskWoken);

// Peek (don't remove)
BaseType_t xQueuePeek(QueueHandle_t xQueue, void *pvBuffer,
                      TickType_t xTicksToWait);

// Query
UBaseType_t uxQueueMessagesWaiting(QueueHandle_t xQueue);
UBaseType_t uxQueueSpacesAvailable(QueueHandle_t xQueue);

// Reset
BaseType_t xQueueReset(QueueHandle_t xQueue);

// Delete
void vQueueDelete(QueueHandle_t xQueue);


// EXAMPLE:
QueueHandle_t data_queue;

void queue_example(void) {
    // Create queue for 10 items of 4 bytes each
    data_queue = xQueueCreate(10, sizeof(uint32_t));

    // Send
    uint32_t value = 42;
    xQueueSend(data_queue, &value, pdMS_TO_TICKS(100));

    // Receive
    uint32_t received;
    if (xQueueReceive(data_queue, &received, portMAX_DELAY) == pdPASS) {
        printf("Received: %lu\n", received);
    }
}
*/

/*
 * ============================================================================
 * SEMAPHORE/MUTEX
 * ============================================================================
 */

/*
#include "semphr.h"

// Binary Semaphore
SemaphoreHandle_t xSemaphoreCreateBinary(void);
    // Initial value: 0 (empty)

// Counting Semaphore
SemaphoreHandle_t xSemaphoreCreateCounting(UBaseType_t uxMaxCount,
                                            UBaseType_t uxInitialCount);

// Mutex (with priority inheritance)
SemaphoreHandle_t xSemaphoreCreateMutex(void);

// Recursive Mutex
SemaphoreHandle_t xSemaphoreCreateRecursiveMutex(void);

// Take (acquire)
BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait);
BaseType_t xSemaphoreTakeRecursive(SemaphoreHandle_t xMutex,
                                    TickType_t xTicksToWait);

// Give (release)
BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore);
BaseType_t xSemaphoreGiveRecursive(SemaphoreHandle_t xMutex);

// From ISR
BaseType_t xSemaphoreGiveFromISR(SemaphoreHandle_t xSemaphore,
                                  BaseType_t *pxHigherPriorityTaskWoken);
BaseType_t xSemaphoreTakeFromISR(SemaphoreHandle_t xSemaphore,
                                  BaseType_t *pxHigherPriorityTaskWoken);

// Query
UBaseType_t uxSemaphoreGetCount(SemaphoreHandle_t xSemaphore);


// EXAMPLE - Binary Semaphore for signaling:
SemaphoreHandle_t data_ready;

void isr_handler(void) {
    BaseType_t wake = pdFALSE;
    xSemaphoreGiveFromISR(data_ready, &wake);
    portYIELD_FROM_ISR(wake);
}

void task(void *p) {
    data_ready = xSemaphoreCreateBinary();
    for (;;) {
        xSemaphoreTake(data_ready, portMAX_DELAY);
        process_data();
    }
}


// EXAMPLE - Mutex for protection:
SemaphoreHandle_t i2c_mutex;

void i2c_transfer(void) {
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    // Only one task can access I2C
    i2c_write(...);
    xSemaphoreGive(i2c_mutex);
}
*/

/*
 * ============================================================================
 * EVENT GROUPS
 * ============================================================================
 */

/*
#include "event_groups.h"

// Creation
EventGroupHandle_t xEventGroupCreate(void);

// Set bits
EventBits_t xEventGroupSetBits(EventGroupHandle_t xEventGroup,
                                const EventBits_t uxBitsToSet);
BaseType_t xEventGroupSetBitsFromISR(EventGroupHandle_t xEventGroup,
                                      const EventBits_t uxBitsToSet,
                                      BaseType_t *pxHigherPriorityTaskWoken);

// Wait for bits
EventBits_t xEventGroupWaitBits(
    EventGroupHandle_t xEventGroup,
    const EventBits_t uxBitsToWaitFor,  // Which bits to wait for
    const BaseType_t xClearOnExit,       // Clear bits after wait?
    const BaseType_t xWaitForAllBits,    // Wait for ALL or ANY?
    TickType_t xTicksToWait
);

// Clear bits
EventBits_t xEventGroupClearBits(EventGroupHandle_t xEventGroup,
                                  const EventBits_t uxBitsToClear);

// Sync (set bits and wait)
EventBits_t xEventGroupSync(EventGroupHandle_t xEventGroup,
                            const EventBits_t uxBitsToSet,
                            const EventBits_t uxBitsToWaitFor,
                            TickType_t xTicksToWait);


// EXAMPLE:
#define BIT_SENSOR (1 << 0)
#define BIT_UART   (1 << 1)
#define BIT_TIMER  (1 << 2)

EventGroupHandle_t events;

void event_example(void) {
    events = xEventGroupCreate();

    // Wait for ANY of these bits
    EventBits_t bits = xEventGroupWaitBits(events,
        BIT_SENSOR | BIT_UART,  // Wait for these
        pdTRUE,                  // Clear on exit
        pdFALSE,                 // ANY (not ALL)
        portMAX_DELAY);

    if (bits & BIT_SENSOR) handle_sensor();
    if (bits & BIT_UART) handle_uart();
}
*/

/*
 * ============================================================================
 * TASK NOTIFICATIONS
 * ============================================================================
 */

/*
// Simple notify (increment)
BaseType_t xTaskNotifyGive(TaskHandle_t xTaskToNotify);
void vTaskNotifyGiveFromISR(TaskHandle_t xTaskToNotify,
                            BaseType_t *pxHigherPriorityTaskWoken);

// Take (wait for notification)
uint32_t ulTaskNotifyTake(BaseType_t xClearCountOnExit,
                          TickType_t xTicksToWait);

// Advanced notify
BaseType_t xTaskNotify(TaskHandle_t xTaskToNotify,
                       uint32_t ulValue,
                       eNotifyAction eAction);
    // eAction: eNoAction, eSetBits, eIncrement,
    //          eSetValueWithOverwrite, eSetValueWithoutOverwrite

BaseType_t xTaskNotifyFromISR(...);

// Wait with value
BaseType_t xTaskNotifyWait(uint32_t ulBitsToClearOnEntry,
                           uint32_t ulBitsToClearOnExit,
                           uint32_t *pulNotificationValue,
                           TickType_t xTicksToWait);


// EXAMPLE - Simple signaling:
TaskHandle_t worker;

void signal_worker(void) {
    xTaskNotifyGive(worker);  // Fastest way to signal!
}

void worker_task(void *p) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        do_work();
    }
}
*/

/*
 * ============================================================================
 * SOFTWARE TIMERS
 * ============================================================================
 */

/*
#include "timers.h"

// Creation
TimerHandle_t xTimerCreate(
    const char *pcTimerName,
    TickType_t xTimerPeriod,
    UBaseType_t uxAutoReload,    // pdTRUE = periodic, pdFALSE = one-shot
    void *pvTimerID,
    TimerCallbackFunction_t pxCallbackFunction
);

// Control
BaseType_t xTimerStart(TimerHandle_t xTimer, TickType_t xTicksToWait);
BaseType_t xTimerStop(TimerHandle_t xTimer, TickType_t xTicksToWait);
BaseType_t xTimerReset(TimerHandle_t xTimer, TickType_t xTicksToWait);
BaseType_t xTimerChangePeriod(TimerHandle_t xTimer, TickType_t xNewPeriod,
                               TickType_t xTicksToWait);

// From ISR
BaseType_t xTimerStartFromISR(...);
BaseType_t xTimerStopFromISR(...);
BaseType_t xTimerResetFromISR(...);

// Query
BaseType_t xTimerIsTimerActive(TimerHandle_t xTimer);
void *pvTimerGetTimerID(TimerHandle_t xTimer);


// EXAMPLE:
void timer_callback(TimerHandle_t xTimer) {
    printf("Timer fired!\n");
}

void timer_example(void) {
    TimerHandle_t timer = xTimerCreate(
        "MyTimer",
        pdMS_TO_TICKS(500),  // 500ms
        pdTRUE,               // Auto-reload (periodic)
        NULL,
        timer_callback
    );
    xTimerStart(timer, 0);
}
*/

/*
 * ============================================================================
 * CRITICAL SECTIONS
 * ============================================================================
 */

/*
// Disable/enable interrupts (from task)
taskENTER_CRITICAL();
// Critical code
taskEXIT_CRITICAL();

// From ISR
UBaseType_t uxSavedInterruptStatus;
uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
// Critical code
taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);

// Suspend/resume scheduler (ISRs still run!)
vTaskSuspendAll();
// Code that can't be preempted by other tasks
xTaskResumeAll();
*/

/*
 * ============================================================================
 * HELPER MACROS
 * ============================================================================
 */

/*
// Convert milliseconds to ticks
pdMS_TO_TICKS(ms)

// True/False
pdTRUE   // 1
pdFALSE  // 0

// Pass/Fail
pdPASS   // 1
pdFAIL   // 0

// Priorities
tskIDLE_PRIORITY           // 0
configMAX_PRIORITIES - 1   // Highest

// Timeout
portMAX_DELAY              // Wait forever
0                          // Don't wait (return immediately)
*/

/*
 * ============================================================================
 * COMMON PATTERNS
 * ============================================================================
 *
 * 1. ISR to Task signaling:
 *    ISR: xSemaphoreGiveFromISR() + portYIELD_FROM_ISR()
 *    Task: xSemaphoreTake(sem, portMAX_DELAY)
 *
 * 2. Periodic task:
 *    TickType_t last = xTaskGetTickCount();
 *    for (;;) {
 *        do_work();
 *        vTaskDelayUntil(&last, pdMS_TO_TICKS(100));
 *    }
 *
 * 3. Mutex protection:
 *    xSemaphoreTake(mutex, portMAX_DELAY);
 *    // access resource
 *    xSemaphoreGive(mutex);
 *
 * 4. Producer-Consumer:
 *    Producer: xQueueSend(q, &data, timeout)
 *    Consumer: xQueueReceive(q, &data, portMAX_DELAY)
 *
 * ============================================================================
 * INTERVIEW QUICK ANSWERS
 * ============================================================================
 *
 * Q: Create a task?
 * A: xTaskCreate(func, "name", stack, param, priority, &handle)
 *
 * Q: Signal from ISR to task?
 * A: xSemaphoreGiveFromISR() + portYIELD_FROM_ISR()
 *
 * Q: Protect shared resource?
 * A: xSemaphoreCreateMutex(), Take/Give around access
 *
 * Q: Pass data between tasks?
 * A: xQueueCreate(), xQueueSend(), xQueueReceive()
 *
 * Q: Wait for multiple events?
 * A: xEventGroupWaitBits() with ANY or ALL
 *
 * Q: Periodic task with consistent timing?
 * A: vTaskDelayUntil() (not vTaskDelay!)
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 34 ===\n\n");
    printf("FreeRTOS API Quick Reference\n\n");
    printf("TASK:\n");
    printf("  xTaskCreate(), vTaskDelay(), vTaskDelayUntil()\n");
    printf("  vTaskSuspend(), vTaskResume(), vTaskDelete()\n\n");
    printf("QUEUE:\n");
    printf("  xQueueCreate(), xQueueSend(), xQueueReceive()\n");
    printf("  xQueueSendFromISR(), xQueueReceiveFromISR()\n\n");
    printf("SEMAPHORE:\n");
    printf("  xSemaphoreCreateBinary(), xSemaphoreCreateMutex()\n");
    printf("  xSemaphoreTake(), xSemaphoreGive()\n");
    printf("  xSemaphoreGiveFromISR()\n\n");
    printf("EVENT GROUP:\n");
    printf("  xEventGroupCreate(), xEventGroupSetBits()\n");
    printf("  xEventGroupWaitBits(bits, clear, waitAll, timeout)\n\n");
    printf("TASK NOTIFY:\n");
    printf("  xTaskNotifyGive(), ulTaskNotifyTake()\n");
    printf("  xTaskNotify(task, value, action)\n\n");
    printf("TIMER:\n");
    printf("  xTimerCreate(), xTimerStart(), xTimerStop()\n\n");
    printf("CRITICAL:\n");
    printf("  taskENTER_CRITICAL(), taskEXIT_CRITICAL()\n");
    printf("  vTaskSuspendAll(), xTaskResumeAll()\n");
    return 0;
}
