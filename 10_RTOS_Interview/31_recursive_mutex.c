/*
 * ============================================================================
 * QUESTION 31: What is a Recursive Mutex? When do you need it?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: Medium | IMPORTANCE: High
 *
 * Understanding recursive mutex prevents self-deadlock.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "A recursive mutex can be taken multiple times by the SAME task without
 * deadlocking. Each take must be matched with a give. Regular mutex would
 * deadlock if a task tries to take it twice. Use recursive mutex when:
 * (1) Function that takes mutex might be called from another function that
 * already holds it, (2) Recursive function needs mutex protection. In
 * FreeRTOS: xSemaphoreCreateRecursiveMutex() and use the Recursive variants
 * of Take/Give. Avoid if possible - usually indicates design issue."
 *
 * ============================================================================
 * VISUALIZATION: Regular vs Recursive Mutex
 * ============================================================================
 *
 *   REGULAR MUTEX - Self Deadlock:
 *
 *   Task A
 *   ------
 *   Take mutex     -> Success, owns mutex
 *   ... work ...
 *   Take mutex     -> DEADLOCK! (already held, waits forever)
 *   ... never reached ...
 *   Give mutex
 *
 *
 *   RECURSIVE MUTEX - No Deadlock:
 *
 *   Task A
 *   ------
 *   Take mutex     -> Success, count=1
 *   ... work ...
 *   Take mutex     -> Success, count=2 (same task, allowed!)
 *   ... work ...
 *   Give mutex     -> count=1 (not released yet)
 *   Give mutex     -> count=0, mutex released
 *
 *
 *   RECURSIVE MUTEX STATE:
 *
 *   +------------------+
 *   | Recursive Mutex  |
 *   +------------------+
 *   | Owner: Task A    |
 *   | Count: 2         |
 *   +------------------+
 *
 *   Must give() twice to fully release!
 *
 * ============================================================================
 * CODE EXAMPLE
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "semphr.h"

// ============================================================================
// PROBLEM: Self-Deadlock with Regular Mutex
// ============================================================================

SemaphoreHandle_t regular_mutex;

// Function A takes mutex
void function_a(void) {
    xSemaphoreTake(regular_mutex, portMAX_DELAY);

    // Do some work...
    process_data();

    // Calls function B which also needs the mutex
    function_b();  // DEADLOCK! function_b tries to take mutex we hold

    xSemaphoreGive(regular_mutex);
}

void function_b(void) {
    xSemaphoreTake(regular_mutex, portMAX_DELAY);  // Waits forever!
    // ...
    xSemaphoreGive(regular_mutex);
}


// ============================================================================
// SOLUTION: Recursive Mutex
// ============================================================================

SemaphoreHandle_t recursive_mutex;

void init_recursive(void) {
    recursive_mutex = xSemaphoreCreateRecursiveMutex();
}

void function_a_fixed(void) {
    // Note: Use xSemaphoreTakeRecursive, not xSemaphoreTake!
    xSemaphoreTakeRecursive(recursive_mutex, portMAX_DELAY);  // count=1

    process_data();
    function_b_fixed();  // No deadlock!

    xSemaphoreGiveRecursive(recursive_mutex);  // count=0, released
}

void function_b_fixed(void) {
    xSemaphoreTakeRecursive(recursive_mutex, portMAX_DELAY);  // count=2
    do_something();
    xSemaphoreGiveRecursive(recursive_mutex);  // count=1
}


// ============================================================================
// USE CASE: Recursive Function with Mutex
// ============================================================================

SemaphoreHandle_t tree_mutex;

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node_t;

// Recursive tree operation needs mutex protection
int sum_tree_recursive(Node_t *node) {
    if (node == NULL) return 0;

    xSemaphoreTakeRecursive(tree_mutex, portMAX_DELAY);

    int sum = node->value;
    sum += sum_tree_recursive(node->left);   // Recursive call
    sum += sum_tree_recursive(node->right);  // Recursive call

    xSemaphoreGiveRecursive(tree_mutex);

    return sum;
}


// ============================================================================
// USE CASE: API with Internal Calls
// ============================================================================

// Public API - user might call these
void api_set_value(int value);
void api_increment(int delta);
void api_process(void);

SemaphoreHandle_t api_mutex;

void api_set_value(int value) {
    xSemaphoreTakeRecursive(api_mutex, portMAX_DELAY);
    internal_data = value;
    xSemaphoreGiveRecursive(api_mutex);
}

void api_increment(int delta) {
    xSemaphoreTakeRecursive(api_mutex, portMAX_DELAY);
    api_set_value(internal_data + delta);  // Calls another API function!
    xSemaphoreGiveRecursive(api_mutex);
}

void api_process(void) {
    xSemaphoreTakeRecursive(api_mutex, portMAX_DELAY);
    // Multiple internal operations
    api_set_value(0);      // Nested take
    api_increment(10);     // Nested take
    api_increment(5);      // Nested take
    xSemaphoreGiveRecursive(api_mutex);
}
*/

/*
 * ============================================================================
 * ZEPHYR RECURSIVE MUTEX
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>

// Zephyr k_mutex is recursive by default!
K_MUTEX_DEFINE(my_mutex);

void zephyr_function_a(void) {
    k_mutex_lock(&my_mutex, K_FOREVER);  // count=1

    zephyr_function_b();  // No deadlock - k_mutex is recursive!

    k_mutex_unlock(&my_mutex);  // count=0
}

void zephyr_function_b(void) {
    k_mutex_lock(&my_mutex, K_FOREVER);  // count=2 (same thread)
    do_work();
    k_mutex_unlock(&my_mutex);  // count=1
}

// Note: k_mutex always allows recursive locking by same thread!
// This is different from FreeRTOS where you must explicitly create
// a recursive mutex.
*/

/*
 * ============================================================================
 * FREERTOS vs ZEPHYR COMPARISON
 * ============================================================================
 *
 *   +------------------+---------------------------+------------------------+
 *   | Aspect           | FreeRTOS                  | Zephyr                 |
 *   +------------------+---------------------------+------------------------+
 *   | Regular mutex    | xSemaphoreCreateMutex()   | k_mutex (recursive!)   |
 *   | Recursive mutex  | xSemaphoreCreateRecursive | k_mutex (same)         |
 *   | Take API         | xSemaphoreTakeRecursive() | k_mutex_lock()         |
 *   | Give API         | xSemaphoreGiveRecursive() | k_mutex_unlock()       |
 *   | Default behavior | Non-recursive             | Recursive              |
 *   +------------------+---------------------------+------------------------+
 *
 *   IMPORTANT: In Zephyr, k_mutex is ALWAYS recursive!
 *
 * ============================================================================
 * WHEN TO USE RECURSIVE MUTEX
 * ============================================================================
 *
 * LEGITIMATE USE CASES:
 *
 * 1. Library/API with internal calls
 *    - Public functions call each other
 *    - User shouldn't need to know about internal locking
 *
 * 2. Recursive algorithms
 *    - Tree traversal, graph algorithms
 *    - Each recursive call needs protection
 *
 * 3. Callback patterns
 *    - Callback might call back into the library
 *    - Can't predict call chain
 *
 *
 * WHEN TO AVOID:
 *
 * 1. Single function needing protection
 *    - Regular mutex is more efficient
 *
 * 2. When redesign is better
 *    - Often recursive mutex indicates design smell
 *    - Consider separate internal/external functions
 *
 * 3. Performance-critical code
 *    - Recursive mutex has overhead of tracking count
 *
 * ============================================================================
 * BETTER DESIGN ALTERNATIVE
 * ============================================================================
 */

/*
// Instead of recursive mutex, use internal/external function pattern:

// Public (takes mutex)
void api_operation(void) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    api_operation_internal();
    xSemaphoreGive(mutex);
}

// Internal (assumes mutex already held)
static void api_operation_internal(void) {
    // No mutex taking - caller must hold it
    data_process_internal();
    data_validate_internal();
}

// Other public function can also use internal
void api_other(void) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    api_operation_internal();  // Reuses internal function
    do_other_stuff();
    xSemaphoreGive(mutex);
}
*/

/*
 * ============================================================================
 * COMMON MISTAKES
 * ============================================================================
 *
 * MISTAKE 1: Using wrong API
 *
 *   // WRONG: Using regular Take with recursive mutex
 *   SemaphoreHandle_t recursive = xSemaphoreCreateRecursiveMutex();
 *   xSemaphoreTake(recursive, portMAX_DELAY);  // Undefined behavior!
 *
 *   // CORRECT:
 *   xSemaphoreTakeRecursive(recursive, portMAX_DELAY);
 *
 *
 * MISTAKE 2: Mismatched Take/Give count
 *
 *   xSemaphoreTakeRecursive(mutex, portMAX_DELAY);  // count=1
 *   xSemaphoreTakeRecursive(mutex, portMAX_DELAY);  // count=2
 *   xSemaphoreGiveRecursive(mutex);                  // count=1
 *   // Forgot second give - mutex never released!
 *
 *
 * MISTAKE 3: Assuming priority inheritance works
 *
 *   // Priority inheritance DOES work with recursive mutex
 *   // But: it's based on the OUTERMOST owner
 *   // Task B waiting on mutex held by Task A (count=3)
 *   // Task A gets priority boost even though count > 1
 *
 *
 * MISTAKE 4: Different tasks trying recursive pattern
 *
 *   // Task A
 *   xSemaphoreTakeRecursive(mutex, portMAX_DELAY);  // count=1
 *
 *   // Task B (different task!)
 *   xSemaphoreTakeRecursive(mutex, portMAX_DELAY);  // BLOCKS!
 *   // Recursive only works for SAME task!
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN THE PROBLEM IT SOLVES
 *    "Prevents self-deadlock when same task takes mutex twice"
 *
 * 2. MENTION USE CASES
 *    "Recursive algorithms, API libraries with internal calls"
 *
 * 3. DISCUSS ALTERNATIVES
 *    "Often better to redesign with internal/external function pattern"
 *
 * 4. KNOW THE ZEPHYR DIFFERENCE
 *    "Zephyr k_mutex is always recursive, FreeRTOS needs explicit type"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "Does recursive mutex have priority inheritance?"
 * A: Yes, in FreeRTOS recursive mutex supports priority inheritance,
 *    but boost is based on the task holding the outermost lock.
 *
 * Q: "What's the overhead of recursive mutex vs regular?"
 * A: Slight overhead - needs to track owner and count. Regular mutex
 *    is more efficient if you don't need recursion.
 *
 * Q: "Can you convert recursive mutex to regular after creation?"
 * A: No, the type is fixed at creation. Must delete and recreate.
 *
 * Q: "What if I forget a Give?"
 * A: Mutex stays locked forever (or until count reaches 0). Other tasks
 *    will block forever = deadlock.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 31 ===\n\n");
    printf("Q: What is a Recursive Mutex?\n\n");
    printf("RECURSIVE MUTEX:\n");
    printf("- Can be taken multiple times by SAME task\n");
    printf("- Each take increments count\n");
    printf("- Each give decrements count\n");
    printf("- Released only when count reaches 0\n\n");
    printf("PROBLEM IT SOLVES:\n");
    printf("- Prevents self-deadlock\n");
    printf("- Function A calls function B, both need mutex\n\n");
    printf("FREERTOS API:\n");
    printf("- xSemaphoreCreateRecursiveMutex()\n");
    printf("- xSemaphoreTakeRecursive()\n");
    printf("- xSemaphoreGiveRecursive()\n\n");
    printf("ZEPHYR:\n");
    printf("- k_mutex is ALWAYS recursive by default!\n\n");
    printf("BEST PRACTICE:\n");
    printf("- Avoid if possible (often design smell)\n");
    printf("- Use internal/external function pattern instead\n");
    printf("- If needed, ensure Take/Give counts match\n");
    return 0;
}
