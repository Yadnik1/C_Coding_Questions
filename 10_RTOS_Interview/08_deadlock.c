/*
 * ============================================================================
 * QUESTION 08: What is Deadlock? How do you prevent it?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: High | IMPORTANCE: Critical
 *
 * Deadlock is a classic concurrency problem. Interviewers want to see
 * you understand the four conditions AND know practical prevention strategies.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Deadlock occurs when two or more tasks are permanently blocked, each waiting
 * for a resource held by the other. Four conditions must ALL be present:
 * mutual exclusion, hold and wait, no preemption, and circular wait.
 * Prevention strategies include: always acquiring locks in the same order,
 * using timeouts instead of infinite waits, acquiring all locks atomically,
 * or using a single lock. The most practical approach is LOCK ORDERING -
 * always acquire mutex A before mutex B in all tasks."
 *
 * ============================================================================
 * VISUALIZATION: Deadlock Scenario
 * ============================================================================
 *
 * THE DEADLY EMBRACE:
 *
 *   Task A                                    Task B
 *   +--------+                               +--------+
 *   | Holds  |                               | Holds  |
 *   |Mutex 1 |                               |Mutex 2 |
 *   +--------+                               +--------+
 *       |                                         |
 *       | Wants                             Wants |
 *       v                                         v
 *   +--------+                               +--------+
 *   |Mutex 2 |<------ BLOCKED -------- -----|Mutex 1 |
 *   +--------+                               +--------+
 *
 *   CIRCULAR WAIT: A waits for B's mutex, B waits for A's mutex
 *   FOREVER BLOCKED - DEADLOCK!
 *
 *
 * TIME SEQUENCE:
 *
 *   Time --->
 *
 *   Task A: [Take M1][=====holds M1=====][Try M2 - BLOCKED FOREVER...]
 *   Task B:      [Take M2][==holds M2==][Try M1 - BLOCKED FOREVER...]
 *
 *           ^         ^              ^
 *           |         |              |
 *           A gets    B gets       DEADLOCK!
 *           M1        M2           Both waiting forever
 *
 * ============================================================================
 * THE FOUR CONDITIONS FOR DEADLOCK (Coffman Conditions)
 * ============================================================================
 *
 * ALL FOUR must be present for deadlock to occur:
 *
 * 1. MUTUAL EXCLUSION
 *    - At least one resource must be held in non-shareable mode
 *    - Only one task can use the resource at a time
 *    Example: Mutex by definition is mutually exclusive
 *
 * 2. HOLD AND WAIT
 *    - Task holding resource is waiting to acquire additional resources
 *    Example: Task A holds M1 while waiting for M2
 *
 * 3. NO PREEMPTION
 *    - Resources cannot be forcibly taken from tasks
 *    - Task must voluntarily release resources
 *    Example: Can't take mutex away from a task
 *
 * 4. CIRCULAR WAIT
 *    - Circular chain of tasks, each waiting for resource held by next
 *    Example: A waits for B, B waits for A
 *
 * BREAK ANY ONE CONDITION → PREVENT DEADLOCK
 *
 * ============================================================================
 * CODE EXAMPLE: Creating a Deadlock
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
// DON'T DO THIS - Deadlock Example

SemaphoreHandle_t mutex1, mutex2;

void task_A(void *pvParameters) {
    for (;;) {
        xSemaphoreTake(mutex1, portMAX_DELAY);  // Get mutex1 first
        printf("Task A has mutex1\n");

        vTaskDelay(1);  // Small delay increases deadlock chance

        xSemaphoreTake(mutex2, portMAX_DELAY);  // Try to get mutex2
        printf("Task A has both mutexes\n");

        // Critical section using both resources

        xSemaphoreGive(mutex2);
        xSemaphoreGive(mutex1);
    }
}

void task_B(void *pvParameters) {
    for (;;) {
        xSemaphoreTake(mutex2, portMAX_DELAY);  // Get mutex2 first (WRONG ORDER!)
        printf("Task B has mutex2\n");

        vTaskDelay(1);  // Small delay increases deadlock chance

        xSemaphoreTake(mutex1, portMAX_DELAY);  // Try to get mutex1 - DEADLOCK!
        printf("Task B has both mutexes\n");

        // Critical section using both resources

        xSemaphoreGive(mutex1);
        xSemaphoreGive(mutex2);
    }
}
*/

/*
 * ============================================================================
 * PREVENTION STRATEGIES
 * ============================================================================
 *
 * STRATEGY 1: LOCK ORDERING (Most Common, Most Effective)
 * ---------------------------------------------------------
 * ALWAYS acquire locks in the SAME ORDER in ALL tasks
 * This breaks the CIRCULAR WAIT condition
 */

/*
// CORRECT - Both tasks acquire in same order (mutex1 first)

void task_A_correct(void *pvParameters) {
    for (;;) {
        xSemaphoreTake(mutex1, portMAX_DELAY);  // Always mutex1 first
        xSemaphoreTake(mutex2, portMAX_DELAY);  // Then mutex2

        // Critical section

        xSemaphoreGive(mutex2);
        xSemaphoreGive(mutex1);
    }
}

void task_B_correct(void *pvParameters) {
    for (;;) {
        xSemaphoreTake(mutex1, portMAX_DELAY);  // Always mutex1 first (SAME ORDER!)
        xSemaphoreTake(mutex2, portMAX_DELAY);  // Then mutex2

        // Critical section

        xSemaphoreGive(mutex2);
        xSemaphoreGive(mutex1);
    }
}
*/

/*
 * VISUALIZATION: Lock Ordering Prevents Deadlock
 *
 * With lock ordering (both tasks: M1 then M2):
 *
 *   Time --->
 *
 *   Task A: [Take M1][Take M2][==work==][Give M2][Give M1]
 *   Task B:          [Try M1 - WAIT.............][Take M1][Take M2]...
 *
 *   No deadlock! B waits for M1, but A will eventually release it.
 */

/*
 * STRATEGY 2: TIMEOUT (Detect and Recover)
 * ---------------------------------------------------------
 * Use timeout instead of infinite wait
 * If timeout, release all locks and retry
 * This breaks the NO PREEMPTION condition (sort of)
 */

/*
void task_with_timeout(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(mutex1, pdMS_TO_TICKS(100)) == pdTRUE) {
            if (xSemaphoreTake(mutex2, pdMS_TO_TICKS(100)) == pdTRUE) {
                // Got both mutexes - do work
                critical_section();

                xSemaphoreGive(mutex2);
                xSemaphoreGive(mutex1);
            } else {
                // Couldn't get mutex2 - release mutex1 and retry
                xSemaphoreGive(mutex1);
                // Optional: add random delay to avoid livelock
                vTaskDelay(pdMS_TO_TICKS(rand() % 10));
            }
        } else {
            // Couldn't get mutex1 - retry
            vTaskDelay(pdMS_TO_TICKS(rand() % 10));
        }
    }
}
*/

/*
 * STRATEGY 3: TRY-LOCK (Non-blocking acquire)
 * ---------------------------------------------------------
 * Try to get lock without blocking
 * If can't get all locks, release and retry
 */

/*
void task_with_trylock(void *pvParameters) {
    for (;;) {
        int got_all = 0;

        // Try to get all locks atomically
        taskENTER_CRITICAL();  // Disable preemption

        if (xSemaphoreTake(mutex1, 0) == pdTRUE) {  // Timeout = 0 (try)
            if (xSemaphoreTake(mutex2, 0) == pdTRUE) {
                got_all = 1;
            } else {
                xSemaphoreGive(mutex1);  // Release and retry
            }
        }

        taskEXIT_CRITICAL();

        if (got_all) {
            critical_section();
            xSemaphoreGive(mutex2);
            xSemaphoreGive(mutex1);
        } else {
            vTaskDelay(1);  // Yield and retry
        }
    }
}
*/

/*
 * STRATEGY 4: SINGLE LOCK (Coarse-grained locking)
 * ---------------------------------------------------------
 * Use one lock for multiple resources
 * Simple but reduces concurrency
 */

/*
SemaphoreHandle_t global_mutex;

void task_single_lock(void *pvParameters) {
    for (;;) {
        xSemaphoreTake(global_mutex, portMAX_DELAY);

        // Access resource1 and resource2 safely
        use_resource1();
        use_resource2();

        xSemaphoreGive(global_mutex);
    }
}

// PROS: Simple, no deadlock possible
// CONS: Less concurrency (tasks accessing different resources still serialize)
*/

/*
 * STRATEGY 5: PRIORITY CEILING (Prevents deadlock + priority inversion)
 * ---------------------------------------------------------
 * Task runs at highest priority that uses the resource
 * No other task can preempt while holding lock
 */

/*
 * ============================================================================
 * LIVELOCK vs DEADLOCK
 * ============================================================================
 *
 * DEADLOCK: Tasks permanently blocked, no progress
 * LIVELOCK: Tasks keep running but make no progress (constantly retry)
 *
 * LIVELOCK EXAMPLE:
 *
 *   Task A: Take M1, try M2, timeout, release M1, retry...
 *   Task B: Take M2, try M1, timeout, release M2, retry...
 *
 *   Both keep trying but neither ever succeeds!
 *
 * PREVENTION: Add random delay before retry
 *
 * ============================================================================
 * DEADLOCK DETECTION
 * ============================================================================
 *
 * RUNTIME DETECTION:
 * 1. Maintain resource allocation graph
 * 2. Periodically check for cycles
 * 3. If cycle found, kill/restart one task
 *
 * STATIC ANALYSIS:
 * 1. Tools analyze code for potential deadlocks
 * 2. Model checking
 * 3. Code review with lock ordering rules
 *
 * FREERTOS DEBUGGING:
 * - vTaskGetTaskState() to check if task is blocked
 * - xSemaphoreGetMutexHolder() to see who holds mutex
 * - Custom trace hooks to log lock acquisitions
 *
 * ============================================================================
 * ZEPHYR DEADLOCK DETECTION
 * ============================================================================
 */

/*
// Zephyr has built-in deadlock detection for mutexes!
// Enable in prj.conf:
// CONFIG_MUTEX_DEADLOCK_DETECTION=y

K_MUTEX_DEFINE(mutex1);
K_MUTEX_DEFINE(mutex2);

void zephyr_deadlock_example(void *p1, void *p2, void *p3) {
    k_mutex_lock(&mutex1, K_FOREVER);

    // If deadlock detection is enabled and another thread
    // is holding mutex2 while waiting for mutex1,
    // Zephyr will detect and report the deadlock!

    k_mutex_lock(&mutex2, K_FOREVER);

    k_mutex_unlock(&mutex2);
    k_mutex_unlock(&mutex1);
}
*/

/*
 * ============================================================================
 * REAL-WORLD EXAMPLE: Dining Philosophers
 * ============================================================================
 *
 *          [Fork 1]
 *     P1             P2
 *  [Fork 5]       [Fork 2]
 *     P5             P3
 *          [Fork 4]
 *             P4
 *          [Fork 3]
 *
 * PROBLEM:
 * - 5 philosophers, 5 forks
 * - Each needs 2 forks to eat
 * - If all pick up left fork first → DEADLOCK
 *
 * SOLUTIONS:
 * 1. Lock ordering: Lower-numbered fork first
 * 2. Waiter: Central arbitrator (single lock)
 * 3. Timeout: Put down fork if can't get second
 * 4. Odd/even: Odd pick left first, even pick right first
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. NAME THE FOUR CONDITIONS
 *    "Mutual exclusion, hold and wait, no preemption, circular wait"
 *
 * 2. EXPLAIN LOCK ORDERING
 *    "The most practical prevention is consistent lock ordering"
 *
 * 3. MENTION TIMEOUTS
 *    "Using timeouts allows detection and recovery"
 *
 * 4. KNOW THE DIFFERENCE
 *    "Deadlock = stuck, Livelock = busy but no progress"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "How would you debug a suspected deadlock?"
 * A: 1. Check task states (are tasks blocked?)
 *    2. Identify which mutexes each task holds/wants
 *    3. Look for circular dependency
 *    4. Use RTOS trace tools to see lock acquisition order
 *    5. In Zephyr, enable CONFIG_MUTEX_DEADLOCK_DETECTION
 *
 * Q: "What's the difference between deadlock and starvation?"
 * A: Deadlock: All involved tasks blocked forever
 *    Starvation: One task never gets resource (others keep taking it)
 *    Example: High priority tasks always beat low priority to mutex
 *
 * Q: "Can deadlock occur with just one task?"
 * A: Yes! If task takes non-recursive mutex twice, it deadlocks with itself.
 *    xSemaphoreTake(mutex);
 *    xSemaphoreTake(mutex);  // DEADLOCK - waiting for itself!
 *
 * Q: "Which deadlock prevention strategy would you use?"
 * A: Lock ordering is most practical. Define order based on address or
 *    logical hierarchy. Document and enforce in code review.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 08 ===\n\n");
    printf("Q: What is Deadlock? How do you prevent it?\n\n");
    printf("DEFINITION:\n");
    printf("Two+ tasks permanently blocked, each waiting for resource\n");
    printf("held by the other.\n\n");
    printf("FOUR CONDITIONS (ALL must be present):\n");
    printf("1. Mutual Exclusion - Resource can't be shared\n");
    printf("2. Hold and Wait - Hold one, wait for another\n");
    printf("3. No Preemption - Can't force release\n");
    printf("4. Circular Wait - A waits for B, B waits for A\n\n");
    printf("PREVENTION STRATEGIES:\n");
    printf("1. LOCK ORDERING - Always acquire in same order (BEST)\n");
    printf("2. TIMEOUT - Don't wait forever, retry\n");
    printf("3. TRY-LOCK - Non-blocking attempt, release and retry\n");
    printf("4. SINGLE LOCK - One lock for all (simple but less concurrent)\n\n");
    printf("DEADLOCK vs LIVELOCK:\n");
    printf("- Deadlock: Stuck forever\n");
    printf("- Livelock: Busy but no progress (constant retry)\n");
    return 0;
}
