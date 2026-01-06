/*
 * ============================================================================
 * QUESTION 03: What are the different task states in an RTOS?
 * ============================================================================
 *
 * DIFFICULTY: Basic | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * Understanding task states is fundamental to debugging RTOS applications.
 * Interviewers want to see you can trace why a task isn't running.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "In an RTOS, tasks typically exist in four main states: RUNNING (actively
 * executing on CPU), READY (can run but waiting for CPU), BLOCKED (waiting
 * for an event like a semaphore or delay), and SUSPENDED (explicitly paused).
 * The scheduler moves tasks between these states based on priorities and
 * events. Only one task can be RUNNING at a time on a single-core system."
 *
 * ============================================================================
 * TASK STATE DIAGRAM
 * ============================================================================
 *
 *                         TASK CREATED
 *                              |
 *                              v
 *                    +------------------+
 *   vTaskResume() -->|      READY       |<-- Timeout/Event
 *         ^          +------------------+
 *         |                   |
 *         |              Scheduled (highest priority ready)
 *         |                   v
 *   +-------------+   +------------------+
 *   |  SUSPENDED  |   |     RUNNING      | <-- Only ONE task at a time
 *   +-------------+   +------------------+
 *         ^                   |
 *         |       +-----------+-----------+
 *   vTaskSuspend()|           |           |
 *                 v           v           v
 *           Preempted    vTaskDelay()  Wait for
 *           (higher pri  vTaskDelay   mutex/sem/
 *            became      Until()      queue/etc
 *            ready)           |           |
 *                 |           v           v
 *                 |    +------------------+
 *                 +--->|     BLOCKED      |
 *                      +------------------+
 *                              |
 *                              v
 *                    Timeout or Event occurs
 *                              |
 *                              v
 *                         Back to READY
 *
 *
 *                      +------------------+
 *   vTaskDelete() ---->|     DELETED      | (memory freed)
 *                      +------------------+
 *
 * ============================================================================
 * DETAILED STATE DESCRIPTIONS
 * ============================================================================
 *
 * 1. RUNNING
 *    ---------------------------------
 *    - Task is CURRENTLY executing on the CPU
 *    - Only ONE task can be in this state (per CPU core)
 *    - Has full access to CPU resources
 *    - Will stay running until:
 *      a) It blocks (waits for something)
 *      b) It yields (voluntarily gives up CPU)
 *      c) It's preempted (higher priority task becomes ready)
 *      d) Its time slice expires (if round-robin enabled)
 *
 * 2. READY
 *    ---------------------------------
 *    - Task CAN run but is NOT currently running
 *    - Waiting for scheduler to give it CPU time
 *    - Multiple tasks can be in READY state
 *    - Organized by priority in ready queue
 *    - Highest priority READY task runs next
 *
 *    Ready Queue Example (FreeRTOS):
 *    +----------+     +----------+     +----------+
 *    | Task A   |---->| Task B   |---->| Task C   |
 *    | Pri: 3   |     | Pri: 3   |     | Pri: 3   |
 *    +----------+     +----------+     +----------+
 *         Priority 3 Ready List (Round-Robin within same priority)
 *
 *    +----------+     +----------+
 *    | Task D   |---->| Task E   |
 *    | Pri: 2   |     | Pri: 2   |
 *    +----------+     +----------+
 *         Priority 2 Ready List
 *
 * 3. BLOCKED
 *    ---------------------------------
 *    - Task is WAITING for something to happen
 *    - Cannot run until event occurs or timeout expires
 *    - Common blocking reasons:
 *      a) vTaskDelay() - waiting for time to pass
 *      b) xSemaphoreTake() - waiting for semaphore
 *      c) xQueueReceive() - waiting for queue message
 *      d) xEventGroupWaitBits() - waiting for event flags
 *      e) ulTaskNotifyTake() - waiting for notification
 *
 *    Blocked tasks are in different lists based on what they're waiting for:
 *    - Delayed List (sorted by wake time)
 *    - Semaphore Wait List
 *    - Queue Wait List
 *    - Event Group Wait List
 *
 * 4. SUSPENDED
 *    ---------------------------------
 *    - Task is EXPLICITLY paused by vTaskSuspend()
 *    - Will NOT run until vTaskResume() is called
 *    - Different from BLOCKED:
 *      * BLOCKED: waiting for specific event/timeout
 *      * SUSPENDED: indefinitely paused, no automatic wake
 *    - Use cases:
 *      a) Temporarily disable a task
 *      b) Debug/testing
 *      c) Power saving (stop background tasks)
 *
 * 5. DELETED
 *    ---------------------------------
 *    - Task no longer exists
 *    - Resources (stack, TCB) returned to system
 *    - In FreeRTOS, idle task frees deleted task memory
 *    - Cannot be resumed - must create new task
 *
 * ============================================================================
 * FREERTOS vs ZEPHYR STATE NAMES
 * ============================================================================
 *
 * +------------------+--------------------+--------------------+
 * | Concept          | FreeRTOS           | Zephyr             |
 * +------------------+--------------------+--------------------+
 * | Currently running| Running            | Running            |
 * | Can run          | Ready              | Ready              |
 * | Waiting          | Blocked            | Pending            |
 * | Paused           | Suspended          | Suspended          |
 * | Not started      | -                  | Not Started        |
 * | Ended            | Deleted            | Dead/Terminated    |
 * +------------------+--------------------+--------------------+
 *
 * Zephyr additional states:
 * - QUEUED: Thread is in a queue (like semaphore wait queue)
 * - DUMMY: Placeholder for special threads
 *
 * ============================================================================
 * STATE TRANSITIONS: What Triggers Them
 * ============================================================================
 *
 * READY -> RUNNING:
 *   Scheduler selects this task (highest priority ready)
 *   Trigger: Context switch (tick interrupt, yield, unblock)
 *
 * RUNNING -> READY:
 *   1. Preemption: Higher priority task became ready
 *   2. Time slice expired (round-robin)
 *   3. taskYIELD() called
 *
 * RUNNING -> BLOCKED:
 *   Task called a blocking API:
 *   - vTaskDelay(ticks)
 *   - xSemaphoreTake(sem, timeout)
 *   - xQueueReceive(queue, &data, timeout)
 *   - xEventGroupWaitBits(...)
 *
 * BLOCKED -> READY:
 *   1. Event occurred (semaphore given, data in queue)
 *   2. Timeout expired
 *
 * RUNNING -> SUSPENDED:
 *   vTaskSuspend() called (on self or by another task)
 *
 * SUSPENDED -> READY:
 *   vTaskResume() called from another task or ISR
 *
 * ANY -> DELETED:
 *   vTaskDelete() called
 *
 * ============================================================================
 * CODE EXAMPLES
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/* FreeRTOS State Transitions */
/*
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

TaskHandle_t task_handle;
SemaphoreHandle_t sem;

void demo_task(void *pvParameters) {
    for (;;) {
        // Currently in RUNNING state

        // Transition: RUNNING -> BLOCKED (waiting for semaphore)
        // Will wait up to 1000 ticks, then return even without semaphore
        if (xSemaphoreTake(sem, pdMS_TO_TICKS(1000)) == pdTRUE) {
            // Got the semaphore, do work
            process_data();

            // Give semaphore back
            xSemaphoreGive(sem);
        } else {
            // Timeout occurred
            handle_timeout();
        }

        // Transition: RUNNING -> BLOCKED (timed delay)
        vTaskDelay(pdMS_TO_TICKS(100));
        // After 100ms, moves to READY, then RUNNING when scheduled
    }
}

void control_task(void *pvParameters) {
    for (;;) {
        // Suspend demo_task
        // demo_task: RUNNING/READY/BLOCKED -> SUSPENDED
        vTaskSuspend(task_handle);
        printf("Task suspended\n");

        vTaskDelay(pdMS_TO_TICKS(5000));

        // Resume demo_task
        // demo_task: SUSPENDED -> READY
        vTaskResume(task_handle);
        printf("Task resumed\n");

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// Check task state (for debugging)
void print_task_state(TaskHandle_t task) {
    eTaskState state = eTaskGetState(task);

    switch (state) {
        case eRunning:
            printf("Task is RUNNING\n");
            break;
        case eReady:
            printf("Task is READY\n");
            break;
        case eBlocked:
            printf("Task is BLOCKED\n");
            break;
        case eSuspended:
            printf("Task is SUSPENDED\n");
            break;
        case eDeleted:
            printf("Task is DELETED\n");
            break;
        case eInvalid:
            printf("Invalid task handle\n");
            break;
    }
}
*/

/* Zephyr State Transitions */
/*
#include <zephyr/kernel.h>

K_THREAD_STACK_DEFINE(demo_stack, 512);
struct k_thread demo_thread;
k_tid_t demo_tid;

K_SEM_DEFINE(my_sem, 0, 1);  // Initial count 0, max count 1

void demo_thread_fn(void *p1, void *p2, void *p3) {
    while (1) {
        // Transition: RUNNING -> PENDING (waiting for semaphore)
        if (k_sem_take(&my_sem, K_MSEC(1000)) == 0) {
            // Got semaphore
            process_data();
            k_sem_give(&my_sem);
        } else {
            // Timeout (-EAGAIN returned)
            handle_timeout();
        }

        // Transition: RUNNING -> PENDING (sleep)
        k_sleep(K_MSEC(100));
    }
}

void control_thread_fn(void *p1, void *p2, void *p3) {
    while (1) {
        // Suspend demo thread
        k_thread_suspend(demo_tid);
        printk("Thread suspended\n");

        k_sleep(K_SECONDS(5));

        // Resume demo thread
        k_thread_resume(demo_tid);
        printk("Thread resumed\n");

        k_sleep(K_SECONDS(5));
    }
}

// In Zephyr, use thread analyzer for state info
// CONFIG_THREAD_ANALYZER=y
// thread_analyzer_print();
*/

/*
 * ============================================================================
 * DEBUGGING TIP: Why Is My Task Not Running?
 * ============================================================================
 *
 * CHECKLIST:
 *
 * 1. Is the task CREATED?
 *    - Check return value of xTaskCreate()
 *    - Verify TaskHandle is not NULL
 *
 * 2. Is the task READY but never runs?
 *    - Higher priority task hogging CPU (not yielding/blocking)
 *    - Priority too low
 *
 * 3. Is the task BLOCKED?
 *    - Check what it's waiting for
 *    - Semaphore never given?
 *    - Queue always empty?
 *    - Infinite timeout?
 *
 * 4. Is the task SUSPENDED?
 *    - Someone called vTaskSuspend()
 *    - Never called vTaskResume()
 *
 * 5. Is the task DELETED?
 *    - Was vTaskDelete() called?
 *    - Check task handle validity
 *
 * FREERTOS DEBUG TOOLS:
 * - vTaskList() - prints all tasks and states
 * - vTaskGetRunTimeStats() - CPU usage per task
 * - uxTaskGetStackHighWaterMark() - stack usage
 * - eTaskGetState() - get specific task state
 *
 * ============================================================================
 * VISUALIZATION: Scheduler Decision Process
 * ============================================================================
 *
 *   Tick Interrupt or Event
 *            |
 *            v
 *   +------------------+
 *   | Wake up blocked  |  Move tasks from Blocked to Ready
 *   | tasks if timeout |  if their timeout expired
 *   | expired          |
 *   +------------------+
 *            |
 *            v
 *   +------------------+
 *   | Check if higher  |  Compare current running task
 *   | priority task is |  with highest priority in Ready queue
 *   | now Ready        |
 *   +------------------+
 *            |
 *      +-----+-----+
 *      |           |
 *      v           v
 *     YES          NO
 *      |           |
 *      v           v
 *   Context     Continue
 *   Switch      current task
 *      |
 *      v
 *   Save current task context
 *   Load new task context
 *   New task runs
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. DRAW THE STATE DIAGRAM
 *    "Let me draw the state transitions..." - shows clear thinking
 *
 * 2. EXPLAIN BLOCKED vs SUSPENDED
 *    Common interview question: "What's the difference?"
 *    BLOCKED: Automatic wake on event/timeout
 *    SUSPENDED: Manual wake only (vTaskResume)
 *
 * 3. MENTION DEBUGGING
 *    "When a task isn't running, I first check its state..."
 *
 * 4. DISCUSS SCHEDULER ROLE
 *    "The scheduler moves tasks between Ready and Running based on priority"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "What happens if all tasks are blocked?"
 * A: The IDLE task runs. It's always READY (lowest priority).
 *    IDLE task can do housekeeping (free deleted task memory)
 *    or put CPU in low-power mode (tickless idle).
 *
 * Q: "Can a task suspend itself?"
 * A: Yes! vTaskSuspend(NULL) or vTaskSuspend(xTaskGetCurrentTaskHandle())
 *    Task will stay suspended until another task calls vTaskResume()
 *
 * Q: "What's the maximum number of tasks?"
 * A: Limited by RAM (each task needs TCB + stack).
 *    FreeRTOS: configMAX_PRIORITIES limits priority levels, not task count.
 *    Practical limit: dozens to hundreds depending on hardware.
 *
 * Q: "Can a blocked task be suspended?"
 * A: Yes! The task will be both blocked and suspended.
 *    When the event occurs, it moves to SUSPENDED (not READY).
 *    Must call vTaskResume() to make it READY.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 03 ===\n\n");
    printf("Q: What are the different task states in an RTOS?\n\n");
    printf("Main States:\n");
    printf("1. RUNNING  - Executing on CPU (only one per core)\n");
    printf("2. READY    - Can run, waiting for scheduler\n");
    printf("3. BLOCKED  - Waiting for event/timeout\n");
    printf("4. SUSPENDED - Explicitly paused, needs vTaskResume()\n");
    printf("5. DELETED  - Task destroyed, resources freed\n\n");
    printf("Key Differences:\n");
    printf("- BLOCKED: Auto-wake on event/timeout\n");
    printf("- SUSPENDED: Only manual wake (vTaskResume)\n");
    return 0;
}
