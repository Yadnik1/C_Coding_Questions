/*
 * ============================================================================
 * QUESTION 04: What is the difference between Preemptive and Cooperative
 *              scheduling?
 * ============================================================================
 *
 * DIFFICULTY: Basic | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * This is a MUST-KNOW question. Interviewers expect a clear, concise answer.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "In PREEMPTIVE scheduling, the kernel can forcibly switch tasks at any time,
 * typically when a higher priority task becomes ready or when a time slice
 * expires. In COOPERATIVE scheduling, tasks must voluntarily yield the CPU -
 * the kernel cannot force a switch. Preemptive scheduling ensures responsiveness
 * (high priority tasks run immediately) but requires careful synchronization.
 * Most RTOS use preemptive scheduling by default, including FreeRTOS and Zephyr."
 *
 * ============================================================================
 * DETAILED COMPARISON
 * ============================================================================
 *
 * +---------------------+------------------------+------------------------+
 * | Aspect              | Preemptive             | Cooperative            |
 * +---------------------+------------------------+------------------------+
 * | Task switch trigger | Kernel decides         | Task decides           |
 * | Responsiveness      | Immediate (high prio)  | Depends on task yield  |
 * | CPU hogging risk    | Low (kernel preempts)  | High (task must yield) |
 * | Synchronization     | Must protect shared    | Simpler (no unexpected |
 * |                     | data carefully         | interruption)          |
 * | Implementation      | More complex kernel    | Simpler kernel         |
 * | Determinism         | Better (bounded resp.) | Unpredictable          |
 * | Use case            | Real-time systems      | Simple systems         |
 * +---------------------+------------------------+------------------------+
 *
 * ============================================================================
 * VISUALIZATION: Preemptive Scheduling
 * ============================================================================
 *
 * Time --->
 *
 * Scenario: Task A (priority 1), Task B (priority 3 - higher)
 *
 *   PREEMPTIVE:
 *   +---------+
 *   | Task A  |  Task A running (priority 1)
 *   +---------+
 *            |
 *            v Event makes Task B ready (priority 3)
 *            |
 *            +---> IMMEDIATE PREEMPTION!
 *                  |
 *                  v
 *                  +---------+---------+---------+
 *                  | Task B  | Task B  | Task B  |  Task B runs to completion
 *                  +---------+---------+---------+
 *                                               |
 *                                               v Task B blocks/completes
 *                                               |
 *                  +----------------------------+
 *                  |
 *                  v
 *   +---------+---------+
 *   | Task A  | Task A  |  Task A continues where it left off
 *   +---------+---------+
 *
 *   KEY: High priority task runs IMMEDIATELY, interrupting low priority task
 *
 * ============================================================================
 * VISUALIZATION: Cooperative Scheduling
 * ============================================================================
 *
 * Time --->
 *
 * Scenario: Same tasks, Task A (priority 1), Task B (priority 3 - higher)
 *
 *   COOPERATIVE:
 *   +---------+---------+---------+---------+---------+
 *   | Task A  | Task A  | Task A  | Task A  | Task A  |
 *   +---------+---------+---------+---------+---------+
 *            |                                       |
 *            v Event makes Task B ready              | Task A finally yields!
 *            |                                       v
 *            | Task B WAITING even though            +---------+---------+
 *            | higher priority!                      | Task B  | Task B  |
 *            | (Task A hasn't yielded)               +---------+---------+
 *
 *   PROBLEM: High priority task must WAIT for low priority task to yield
 *   This is NOT suitable for real-time systems!
 *
 * ============================================================================
 * PREEMPTIVE SCHEDULING IN DETAIL
 * ============================================================================
 *
 * HOW PREEMPTION WORKS:
 *
 * 1. TICK-BASED PREEMPTION (Time Slicing)
 *    - Hardware timer generates periodic interrupts (tick)
 *    - Each tick, scheduler checks if context switch needed
 *    - Enables round-robin among same-priority tasks
 *
 *    Timer Tick
 *        |
 *        v
 *    +-------------------+
 *    | Tick ISR runs     |
 *    +-------------------+
 *        |
 *        v
 *    +-------------------+
 *    | Increment tick    |
 *    | counter           |
 *    +-------------------+
 *        |
 *        v
 *    +-------------------+
 *    | Check delayed     |
 *    | tasks for timeout |
 *    +-------------------+
 *        |
 *        v
 *    +-------------------+       +------------------+
 *    | Higher priority   |--YES->| Request context  |
 *    | task ready?       |       | switch (PendSV)  |
 *    +-------------------+       +------------------+
 *        |
 *        NO
 *        |
 *        v
 *    +-------------------+       +------------------+
 *    | Time slice        |--YES->| Request context  |
 *    | expired? (RR)     |       | switch (PendSV)  |
 *    +-------------------+       +------------------+
 *        |
 *        NO
 *        |
 *        v
 *    Continue current task
 *
 *
 * 2. EVENT-BASED PREEMPTION
 *    - Task unblocks another task (semaphore give, queue send)
 *    - If unblocked task has higher priority, immediate switch
 *
 * ============================================================================
 * COOPERATIVE SCHEDULING: When Is It Used?
 * ============================================================================
 *
 * ADVANTAGES:
 * - No race conditions within a task (can't be interrupted mid-operation)
 * - Simpler debugging (predictable execution)
 * - No critical section protection needed for task-level code
 *
 * DISADVANTAGES:
 * - Responsiveness depends on tasks yielding frequently
 * - One misbehaving task blocks entire system
 * - Not suitable for hard real-time requirements
 *
 * USE CASES:
 * - Simple super-loop systems
 * - Systems where all tasks are trusted/simple
 * - Legacy systems
 * - Some protothreads/coroutine implementations
 *
 * ============================================================================
 * CODE EXAMPLES
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/* FreeRTOS Configuration for Scheduling Mode */
/*
// In FreeRTOSConfig.h:

// PREEMPTIVE (default, recommended for real-time):
#define configUSE_PREEMPTION        1
#define configUSE_TIME_SLICING      1  // Round-robin among same priority

// COOPERATIVE (not recommended for real-time):
#define configUSE_PREEMPTION        0  // Disables preemption
// Tasks must call taskYIELD() to allow other tasks to run
*/

/* Preemptive Example - High priority task runs immediately */
/*
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t data_ready_sem;

// Low priority task (producer)
void low_priority_task(void *pvParameters) {
    for (;;) {
        // Prepare data...
        prepare_sensor_data();

        // Signal high priority task
        xSemaphoreGive(data_ready_sem);
        // HIGH PRIORITY TASK RUNS IMMEDIATELY (preemption)
        // This line doesn't execute until high priority task blocks

        // Continue with more work...
        do_more_work();
    }
}

// High priority task (consumer)
void high_priority_task(void *pvParameters) {
    for (;;) {
        // Wait for data
        xSemaphoreTake(data_ready_sem, portMAX_DELAY);
        // Runs IMMEDIATELY when semaphore is given
        // Even though low_priority_task was in middle of function

        // Process data with guaranteed timing
        process_critical_data();
    }
}
*/

/* Cooperative Example - Must explicitly yield */
/*
#include "FreeRTOS.h"
#include "task.h"

// In cooperative mode, this task would hog CPU!
void badly_designed_task(void *pvParameters) {
    for (;;) {
        // Long computation with no yield points
        for (int i = 0; i < 1000000; i++) {
            complex_calculation(i);
            // Other tasks CANNOT run during this loop!
            // In cooperative mode, must add:
            // taskYIELD(); // Give other tasks a chance
        }
    }
}

// Better design for cooperative mode
void well_designed_cooperative_task(void *pvParameters) {
    for (;;) {
        for (int i = 0; i < 1000; i++) {
            complex_calculation(i);
        }

        // Yield point - allows scheduler to run other tasks
        taskYIELD();

        // Or use a blocking call (also yields):
        // vTaskDelay(0);  // Yields even with 0 delay
    }
}
*/

/* Zephyr Scheduling Configuration */
/*
// In prj.conf:

// Preemptive (default):
CONFIG_PREEMPT_ENABLED=y

// Cooperative threads (Zephyr supports both simultaneously!):
// Use negative priorities for cooperative threads
// Cooperative threads: priority < 0
// Preemptive threads: priority >= 0

K_THREAD_DEFINE(preempt_thread, 512, preempt_fn, NULL, NULL, NULL,
                5, 0, 0);  // Priority 5, preemptive

K_THREAD_DEFINE(coop_thread, 512, coop_fn, NULL, NULL, NULL,
                -1, 0, 0); // Priority -1, cooperative

void coop_fn(void *p1, void *p2, void *p3) {
    while (1) {
        do_work();
        k_yield();  // Must yield in cooperative thread!
    }
}
*/

/*
 * ============================================================================
 * PRIORITY LEVELS AND TIME SLICING
 * ============================================================================
 *
 * TIME SLICING (Round-Robin):
 *
 * Without time slicing (configUSE_TIME_SLICING = 0):
 *   Tasks of same priority run until they block or yield
 *
 *   Task A (Pri 3): |████████████████████|........|████████|
 *   Task B (Pri 3): |....................|████████|........|
 *                                        ^
 *                                   Task A blocks
 *
 * With time slicing (configUSE_TIME_SLICING = 1):
 *   Tasks of same priority alternate every time slice
 *
 *   Task A (Pri 3): |████|....|████|....|████|....|
 *   Task B (Pri 3): |....|████|....|████|....|████|
 *                       ^    ^    ^
 *                   Time slice boundary (1 tick by default)
 *
 * ============================================================================
 * CRITICAL SECTIONS IN PREEMPTIVE SYSTEMS
 * ============================================================================
 *
 * Because preemption can happen at ANY time, shared data needs protection:
 */

/*
volatile int shared_counter = 0;

// WRONG - Race condition in preemptive system!
void increment_counter_unsafe(void) {
    shared_counter++;  // Read-modify-write NOT atomic!
    // Another task could preempt between read and write!
}

// RIGHT - Protected critical section
void increment_counter_safe(void) {
    taskENTER_CRITICAL();  // Disable interrupts (and preemption)
    shared_counter++;
    taskEXIT_CRITICAL();   // Re-enable interrupts
}

// Or use mutex for longer critical sections
void increment_counter_with_mutex(void) {
    xSemaphoreTake(counter_mutex, portMAX_DELAY);
    shared_counter++;
    xSemaphoreGive(counter_mutex);
}
*/

/*
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. DEFINE BOTH CLEARLY
 *    Preemptive: Kernel forces switch
 *    Cooperative: Task must yield
 *
 * 2. EXPLAIN WHY PREEMPTIVE IS PREFERRED FOR RTOS
 *    - Guaranteed response time for high priority tasks
 *    - One task can't hog CPU
 *    - Deterministic behavior
 *
 * 3. MENTION THE TRADEOFF
 *    - Preemptive requires synchronization (mutexes, critical sections)
 *    - Cooperative is simpler but less responsive
 *
 * 4. KNOW YOUR CONFIG OPTIONS
 *    FreeRTOS: configUSE_PREEMPTION
 *    Zephyr: CONFIG_PREEMPT_ENABLED, negative priorities
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "What triggers preemption in FreeRTOS?"
 * A: Two main triggers:
 *    1. Tick interrupt - checks for timeouts and time slicing
 *    2. API calls that make higher priority task ready
 *       (xSemaphoreGive, xQueueSend, xTaskNotifyGive, etc.)
 *
 * Q: "Can you have both preemptive and cooperative tasks?"
 * A: Zephyr supports this! Negative priorities = cooperative.
 *    FreeRTOS: No direct support, but you can make all tasks
 *    same priority with no time slicing for cooperative effect.
 *
 * Q: "What's the overhead of preemption?"
 * A: Context switch time (~1-10us on Cortex-M). Each tick interrupt
 *    also has small overhead. But overhead is usually worth the
 *    determinism for real-time systems.
 *
 * Q: "How do you prevent preemption temporarily?"
 * A: taskENTER_CRITICAL() / taskEXIT_CRITICAL() (disables interrupts)
 *    vTaskSuspendAll() / xTaskResumeAll() (disables scheduler only)
 *    Or use mutexes for protecting shared resources
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 04 ===\n\n");
    printf("Q: Preemptive vs Cooperative Scheduling?\n\n");
    printf("PREEMPTIVE:\n");
    printf("- Kernel forces task switches\n");
    printf("- High priority task runs IMMEDIATELY\n");
    printf("- Requires synchronization for shared data\n");
    printf("- Used by FreeRTOS, Zephyr (default)\n\n");
    printf("COOPERATIVE:\n");
    printf("- Task must voluntarily yield\n");
    printf("- Simple but unpredictable timing\n");
    printf("- One task can block entire system\n");
    printf("- Not suitable for hard real-time\n\n");
    printf("Config: FreeRTOS: configUSE_PREEMPTION=1\n");
    printf("        Zephyr: CONFIG_PREEMPT_ENABLED=y\n");
    return 0;
}
