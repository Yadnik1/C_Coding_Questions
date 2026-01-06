/*
 * ============================================================================
 * QUESTION 06: What is Priority Inversion? How do you solve it?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: VERY HIGH | IMPORTANCE: CRITICAL
 *
 * This is one of the MOST ASKED questions in embedded systems interviews.
 * The Mars Pathfinder incident made this famous. Be ready to explain in depth!
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Priority inversion occurs when a high-priority task is blocked waiting
 * for a resource held by a low-priority task, while a medium-priority task
 * runs instead. This inverts the intended priority order. The classic solution
 * is PRIORITY INHERITANCE, where the low-priority task temporarily inherits
 * the high-priority task's priority while holding the resource. FreeRTOS
 * mutexes support this automatically. The Mars Pathfinder bug was caused
 * by priority inversion - the fix was enabling priority inheritance."
 *
 * ============================================================================
 * THE PROBLEM VISUALIZED
 * ============================================================================
 *
 * SCENARIO:
 * - Task H: High priority (e.g., critical sensor reading)
 * - Task M: Medium priority (e.g., data processing)
 * - Task L: Low priority (e.g., logging)
 * - Shared resource: Protected by mutex
 *
 * WITHOUT PRIORITY INHERITANCE (THE BUG):
 *
 * Time --->
 *
 *   Task H (High):    ........[BLOCKED.....................][RUNS]
 *   Task M (Medium):  ........[RUNS][RUNS][RUNS][RUNS][RUNS]
 *   Task L (Low):     [RUNS]...................................[RUNS]
 *                     ^       ^                              ^
 *                     |       |                              |
 *                     L takes H wants mutex                  L finally
 *                     mutex   (BLOCKED!)                     releases mutex
 *
 * WHAT HAPPENED:
 * 1. Task L acquires mutex
 * 2. Task H preempts L, needs mutex, BLOCKS
 * 3. Task M (doesn't need mutex) preempts L
 * 4. Task M runs for a LONG time
 * 5. Task L never gets CPU to release mutex
 * 6. Task H waits INDEFINITELY (unbounded!)
 *
 * PRIORITY IS INVERTED:
 * - H should run before M
 * - But M is running while H waits!
 * - L is effectively running at H's priority level (blocking H)
 *
 * ============================================================================
 * THE SOLUTION: PRIORITY INHERITANCE
 * ============================================================================
 *
 * WITH PRIORITY INHERITANCE:
 *
 * Time --->
 *
 *   Task H (High):    ........[BLOCKED][RUNS]
 *   Task M (Medium):  ........
 *   Task L (Low→High):[RUNS]..[RUNS at H priority!][RUNS]
 *                     ^       ^                    ^
 *                     |       |                    |
 *                     L takes H wants mutex        L releases mutex
 *                     mutex   L inherits H's       L returns to low priority
 *                             priority!
 *
 * WHAT HAPPENS NOW:
 * 1. Task L acquires mutex
 * 2. Task H preempts L, needs mutex, BLOCKS
 * 3. L INHERITS H's priority (temporarily)
 * 4. L now runs at HIGH priority - M cannot preempt!
 * 5. L quickly finishes and releases mutex
 * 6. H runs immediately
 *
 * BOUNDED BLOCKING: H only waits for L's critical section, not for M!
 *
 * ============================================================================
 * MARS PATHFINDER INCIDENT (1997)
 * ============================================================================
 *
 * Real-world example that made priority inversion famous:
 *
 * THE SYSTEM:
 * - VxWorks RTOS on Mars Pathfinder spacecraft
 * - Information bus (shared resource)
 * - High-priority: Bus management task
 * - Medium-priority: Communication task
 * - Low-priority: Meteorological task
 *
 * THE BUG:
 * 1. Meteo task acquired bus mutex
 * 2. Bus management task needed bus, blocked
 * 3. Communication task (medium) ran for extended periods
 * 4. Bus management task missed deadline
 * 5. Watchdog timer triggered system RESET
 * 6. Spacecraft kept resetting every few hours!
 *
 * THE FIX:
 * - Enable priority inheritance on the mutex
 * - Uploaded fix from Earth to Mars!
 *
 * ============================================================================
 * CODE EXAMPLES
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/* FreeRTOS Priority Inheritance */
/*
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// IMPORTANT: Use xSemaphoreCreateMutex() for priority inheritance
// xSemaphoreCreateBinary() does NOT support priority inheritance!

SemaphoreHandle_t mutex;

void setup(void) {
    // Mutex AUTOMATICALLY supports priority inheritance in FreeRTOS
    mutex = xSemaphoreCreateMutex();

    // For comparison, binary semaphore does NOT:
    // sem = xSemaphoreCreateBinary();  // No inheritance!
}

// Low priority task
void low_priority_task(void *pvParameters) {
    for (;;) {
        // Take mutex
        xSemaphoreTake(mutex, portMAX_DELAY);

        // If high priority task blocks on this mutex,
        // THIS task's priority is automatically raised!
        do_work_with_shared_resource();

        // Release mutex (priority returns to original)
        xSemaphoreGive(mutex);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// High priority task
void high_priority_task(void *pvParameters) {
    for (;;) {
        // If mutex is held by lower priority task,
        // that task's priority is raised to match ours
        xSemaphoreTake(mutex, portMAX_DELAY);

        critical_work_with_shared_resource();

        xSemaphoreGive(mutex);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
*/

/* Zephyr Priority Inheritance */
/*
#include <zephyr/kernel.h>

// Zephyr mutexes support priority inheritance by default
K_MUTEX_DEFINE(my_mutex);

void low_priority_thread(void *p1, void *p2, void *p3) {
    while (1) {
        k_mutex_lock(&my_mutex, K_FOREVER);

        // Priority automatically boosted if higher priority
        // thread is waiting
        do_work();

        k_mutex_unlock(&my_mutex);  // Priority returns to normal

        k_sleep(K_MSEC(100));
    }
}

void high_priority_thread(void *p1, void *p2, void *p3) {
    while (1) {
        k_mutex_lock(&my_mutex, K_FOREVER);

        critical_work();

        k_mutex_unlock(&my_mutex);

        k_sleep(K_MSEC(10));
    }
}
*/

/*
 * ============================================================================
 * TYPES OF PRIORITY INVERSION SOLUTIONS
 * ============================================================================
 *
 * 1. PRIORITY INHERITANCE PROTOCOL (PIP)
 *    ---------------------------------
 *    - Task inherits highest priority of tasks waiting for mutex
 *    - Simple, widely implemented (FreeRTOS, Zephyr)
 *    - Can cause "chained blocking" (task inherits multiple times)
 *
 *    VISUALIZATION:
 *
 *    Before:   Task L (Pri 1) holds mutex
 *              Task H (Pri 10) waits for mutex
 *
 *    After:    Task L temporarily runs at Pri 10
 *              When mutex released, L returns to Pri 1
 *
 *
 * 2. PRIORITY CEILING PROTOCOL (PCP)
 *    ---------------------------------
 *    - Each mutex has a "ceiling" priority
 *    - Ceiling = highest priority of any task that might use it
 *    - Task taking mutex immediately gets ceiling priority
 *    - Prevents deadlock and chained blocking
 *
 *    VISUALIZATION:
 *
 *    Mutex ceiling = 10 (Task H might use it)
 *
 *    When L takes mutex:
 *    - L immediately runs at Pri 10 (not waiting for H)
 *    - No other task with Pri < 10 can preempt
 *    - Guarantees bounded blocking
 *
 *
 * 3. PRIORITY CEILING EMULATION (OSEK/VDX)
 *    ---------------------------------
 *    - Immediate priority ceiling (like PCP)
 *    - Used in automotive OSEK standard
 *    - Task runs at ceiling from the moment it takes mutex
 *
 *
 * COMPARISON:
 * +------------------------+------------------+------------------+
 * | Aspect                 | Inheritance      | Ceiling          |
 * +------------------------+------------------+------------------+
 * | Priority boost timing  | When contention  | Immediately      |
 * | Complexity             | Lower            | Higher           |
 * | Deadlock prevention    | No               | Yes              |
 * | Chained blocking       | Possible         | Prevented        |
 * | RTOS support           | FreeRTOS, Zephyr | Less common      |
 * +------------------------+------------------+------------------+
 *
 * ============================================================================
 * NESTED PRIORITY INHERITANCE
 * ============================================================================
 *
 * What if L holds Mutex A, M holds Mutex B, and H needs both?
 *
 * SCENARIO:
 *   Task L (Pri 1) holds Mutex A
 *   Task M (Pri 5) holds Mutex B
 *   Task H (Pri 10) needs Mutex A, then Mutex B
 *
 * CHAINED BLOCKING:
 *
 *   1. H tries Mutex A, blocked by L
 *   2. L inherits Pri 10
 *   3. L finishes, gives Mutex A
 *   4. H gets Mutex A
 *   5. H tries Mutex B, blocked by M
 *   6. M inherits Pri 10
 *   7. M finishes, gives Mutex B
 *   8. H finally runs
 *
 * H's total blocking time = L's critical section + M's critical section
 * This is why critical sections should be SHORT!
 *
 * ============================================================================
 * BEST PRACTICES TO AVOID PRIORITY INVERSION
 * ============================================================================
 *
 * 1. ALWAYS use mutexes (not binary semaphores) for resource protection
 *    Mutexes have priority inheritance; semaphores don't
 *
 * 2. KEEP critical sections SHORT
 *    Less time holding mutex = less blocking
 *
 * 3. AVOID nested mutexes if possible
 *    Can cause chained blocking and deadlock
 *
 * 4. CONSIDER lock-free algorithms
 *    Atomic operations, wait-free queues
 *
 * 5. DESIGN task priorities carefully
 *    Tasks sharing resources should have similar priorities
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN THE THREE TASKS
 *    High, Medium, Low - Medium is the key to the problem!
 *
 * 2. MENTION MARS PATHFINDER
 *    Shows you know real-world implications
 *
 * 3. EXPLAIN THE SOLUTION CLEARLY
 *    "L inherits H's priority while holding the mutex"
 *
 * 4. KNOW THE DIFFERENCE
 *    Mutex (has inheritance) vs Semaphore (no inheritance)
 *
 * 5. DISCUSS ALTERNATIVES
 *    Priority ceiling, lock-free algorithms
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "Why doesn't binary semaphore have priority inheritance?"
 * A: Semaphores have no "owner" - any task can give() them. Inheritance
 *    needs to know WHO to boost. Mutexes track owner, semaphores don't.
 *
 * Q: "What's the difference between bounded and unbounded inversion?"
 * A: Bounded: H waits only for L's critical section (with inheritance)
 *    Unbounded: H waits for L + M + ... (without inheritance)
 *
 * Q: "Can priority inheritance cause deadlock?"
 * A: No, inheritance doesn't prevent deadlock - that's a separate issue.
 *    Deadlock: L waits for M's mutex, M waits for L's mutex.
 *    Priority ceiling can prevent deadlock.
 *
 * Q: "What if multiple high-priority tasks wait?"
 * A: L inherits the HIGHEST priority among all waiting tasks.
 *    When mutex released, highest priority waiter runs first.
 *
 * Q: "How do you configure priority inheritance in FreeRTOS?"
 * A: It's automatic for mutexes created with xSemaphoreCreateMutex()
 *    Set configUSE_MUTEXES = 1 in FreeRTOSConfig.h
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 06 ===\n\n");
    printf("Q: What is Priority Inversion? How do you solve it?\n\n");
    printf("THE PROBLEM:\n");
    printf("- High priority task blocked by LOW priority task (holding mutex)\n");
    printf("- MEDIUM priority task runs instead of HIGH\n");
    printf("- Priority order is INVERTED (M runs, H waits)\n\n");
    printf("THE SOLUTION: PRIORITY INHERITANCE\n");
    printf("- Low task inherits High's priority while holding mutex\n");
    printf("- Medium cannot preempt (Low is now High priority)\n");
    printf("- Low finishes quickly, releases mutex\n");
    printf("- High runs immediately\n\n");
    printf("KEY POINTS:\n");
    printf("- Use MUTEX (has inheritance), not semaphore\n");
    printf("- Mars Pathfinder bug: spacecraft kept resetting!\n");
    printf("- Keep critical sections SHORT\n");
    printf("- FreeRTOS: xSemaphoreCreateMutex() has it automatically\n");
    printf("- Zephyr: k_mutex has it by default\n");
    return 0;
}
