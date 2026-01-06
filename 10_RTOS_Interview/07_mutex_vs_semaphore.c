/*
 * ============================================================================
 * QUESTION 07: What is the difference between Mutex and Semaphore?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: VERY HIGH | IMPORTANCE: CRITICAL
 *
 * This is THE MOST COMMON synchronization question. Interviewers expect
 * you to clearly articulate the differences and know when to use each.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "A MUTEX provides MUTUAL EXCLUSION - only one task can hold it at a time,
 * and only the owner can release it. It supports priority inheritance.
 * A SEMAPHORE is a SIGNALING mechanism - it can be binary (0/1) or counting
 * (0 to N), and ANY task can signal it. Use mutex for protecting shared
 * resources, use semaphore for signaling between tasks or counting resources.
 * Key difference: mutex has ownership, semaphore doesn't."
 *
 * ============================================================================
 * DETAILED COMPARISON
 * ============================================================================
 *
 * +---------------------------+------------------------+------------------------+
 * | Aspect                    | MUTEX                  | SEMAPHORE              |
 * +---------------------------+------------------------+------------------------+
 * | Purpose                   | Mutual exclusion       | Signaling/counting     |
 * |                           | (protect resource)     | (sync between tasks)   |
 * +---------------------------+------------------------+------------------------+
 * | Ownership                 | YES - only owner can   | NO - any task can      |
 * |                           | release                | give/signal            |
 * +---------------------------+------------------------+------------------------+
 * | Count                     | Binary (0 or 1)        | Binary or counting     |
 * |                           |                        | (0 to N)               |
 * +---------------------------+------------------------+------------------------+
 * | Priority Inheritance      | YES (prevents          | NO                     |
 * |                           | priority inversion)    |                        |
 * +---------------------------+------------------------+------------------------+
 * | Recursive                 | Possible (recursive    | NO (signal increments  |
 * |                           | mutex)                 | count)                 |
 * +---------------------------+------------------------+------------------------+
 * | Use case                  | - Protect shared data  | - Signal events        |
 * |                           | - Critical sections    | - Producer-consumer    |
 * |                           | - One-at-a-time access | - Resource pools       |
 * +---------------------------+------------------------+------------------------+
 * | ISR safe (give)           | NO (has ownership)     | YES (no ownership)     |
 * +---------------------------+------------------------+------------------------+
 *
 * ============================================================================
 * VISUALIZATION: Mutex - Mutual Exclusion
 * ============================================================================
 *
 * MUTEX: Like a bathroom key - only one person at a time, same person unlocks
 *
 *   Task A                 Mutex                  Task B
 *   +------+               +---+                  +------+
 *   |      |---Take()---->| 1 |<---Take()--------|      |
 *   | RUNS |               +---+                  | WAIT |
 *   |      |                ^                     |      |
 *   +------+                |                     +------+
 *         |            Only Task A                    |
 *         |            can Give()!                    |
 *         |                                           |
 *         +----Give()----> Task B gets mutex now <----+
 *
 *   OWNERSHIP: Task A took it, only Task A can give it
 *
 * ============================================================================
 * VISUALIZATION: Semaphore - Signaling
 * ============================================================================
 *
 * BINARY SEMAPHORE: Like a flag - anyone can raise it
 *
 *   ISR (Producer)        Semaphore            Task (Consumer)
 *   +------+               +---+                +------+
 *   |      |---Give()---->| 0 |<---Take()------|      |
 *   | ISR  |               +---+                | WAIT |
 *   |      |                |                   |      |
 *   +------+                v                   +------+
 *                      Now = 1                      |
 *                           |                       |
 *                           +----> Task wakes up <--+
 *
 *   NO OWNERSHIP: ISR gave it, Task takes it (different entities!)
 *
 *
 * COUNTING SEMAPHORE: Like parking spots
 *
 *   Initial: 3 parking spots available
 *
 *   +---+
 *   | 3 |  <-- Three spots available
 *   +---+
 *      |
 *   Car A Take() --> Count = 2
 *   Car B Take() --> Count = 1
 *   Car C Take() --> Count = 0
 *   Car D Take() --> BLOCKS (no spots)
 *      |
 *   Car A Give() --> Count = 1, Car D unblocks!
 *
 * ============================================================================
 * USE CASES: When to Use Which
 * ============================================================================
 *
 * USE MUTEX WHEN:
 * ---------------------------------
 *
 * 1. PROTECTING SHARED DATA
 *    Multiple tasks read/write the same variable
 *
 *    int shared_counter;
 *    SemaphoreHandle_t counter_mutex;
 *
 *    void increment_counter(void) {
 *        xSemaphoreTake(counter_mutex, portMAX_DELAY);
 *        shared_counter++;  // Protected!
 *        xSemaphoreGive(counter_mutex);
 *    }
 *
 * 2. ACCESSING PERIPHERAL
 *    Only one task should use I2C/SPI/UART at a time
 *
 *    void send_i2c_message(uint8_t *data, int len) {
 *        xSemaphoreTake(i2c_mutex, portMAX_DELAY);
 *        i2c_write(data, len);  // Exclusive access
 *        xSemaphoreGive(i2c_mutex);
 *    }
 *
 * 3. WHEN PRIORITY INHERITANCE NEEDED
 *    High/low priority tasks share resource
 *
 *
 * USE SEMAPHORE WHEN:
 * ---------------------------------
 *
 * 1. ISR TO TASK SIGNALING
 *    ISR cannot use mutex (ownership issue)
 *
 *    SemaphoreHandle_t data_ready_sem;
 *
 *    void UART_ISR(void) {
 *        // ISR gives semaphore - no ownership needed!
 *        xSemaphoreGiveFromISR(data_ready_sem, &wake);
 *    }
 *
 *    void uart_task(void *pvParameters) {
 *        for (;;) {
 *            xSemaphoreTake(data_ready_sem, portMAX_DELAY);
 *            process_uart_data();  // Task woken by ISR
 *        }
 *    }
 *
 * 2. COUNTING AVAILABLE RESOURCES
 *    Pool of buffers, connections, etc.
 *
 *    #define BUFFER_COUNT 5
 *    SemaphoreHandle_t buffer_sem;
 *
 *    void init(void) {
 *        buffer_sem = xSemaphoreCreateCounting(BUFFER_COUNT, BUFFER_COUNT);
 *    }
 *
 *    Buffer* get_buffer(void) {
 *        xSemaphoreTake(buffer_sem, portMAX_DELAY);  // Decrement count
 *        return allocate_buffer();
 *    }
 *
 *    void return_buffer(Buffer *buf) {
 *        free_buffer(buf);
 *        xSemaphoreGive(buffer_sem);  // Increment count
 *    }
 *
 * 3. PRODUCER-CONSUMER SYNCHRONIZATION
 *    One task produces, another consumes
 *
 *    Producer: xSemaphoreGive(items_ready);
 *    Consumer: xSemaphoreTake(items_ready, ...);
 *
 * ============================================================================
 * CODE EXAMPLES: FreeRTOS
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// ========== MUTEX EXAMPLE ==========

SemaphoreHandle_t data_mutex;
int shared_data;

void mutex_example_init(void) {
    // Create mutex - HAS priority inheritance
    data_mutex = xSemaphoreCreateMutex();
}

void task_using_mutex(void *pvParameters) {
    for (;;) {
        // Take mutex (blocks if not available)
        if (xSemaphoreTake(data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            // We now OWN the mutex
            shared_data++;

            // Only WE can release it (ownership)
            xSemaphoreGive(data_mutex);
        } else {
            // Timeout - mutex not available
            handle_timeout();
        }
    }
}


// ========== BINARY SEMAPHORE EXAMPLE ==========

SemaphoreHandle_t uart_sem;

void binary_sem_example_init(void) {
    // Binary semaphore - NO priority inheritance
    uart_sem = xSemaphoreCreateBinary();
    // Starts at 0 (not available)
}

void UART_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Clear interrupt, read data...

    // Signal task from ISR (no ownership concept!)
    xSemaphoreGiveFromISR(uart_sem, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_task(void *pvParameters) {
    for (;;) {
        // Wait for ISR to signal
        xSemaphoreTake(uart_sem, portMAX_DELAY);

        // Process UART data
        process_received_data();
    }
}


// ========== COUNTING SEMAPHORE EXAMPLE ==========

#define MAX_CONNECTIONS 10
SemaphoreHandle_t connection_sem;

void counting_sem_init(void) {
    // Counting semaphore: max=10, initial=10
    connection_sem = xSemaphoreCreateCounting(MAX_CONNECTIONS, MAX_CONNECTIONS);
}

int acquire_connection(void) {
    // Try to get a connection slot
    if (xSemaphoreTake(connection_sem, pdMS_TO_TICKS(1000)) == pdTRUE) {
        // Got a slot (count decremented)
        return 0;  // Success
    }
    return -1;  // No slots available
}

void release_connection(void) {
    // Return slot (count incremented)
    xSemaphoreGive(connection_sem);
}
*/

/*
 * ============================================================================
 * CODE EXAMPLES: Zephyr
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>

// ========== MUTEX EXAMPLE ==========

K_MUTEX_DEFINE(data_mutex);  // Static mutex definition
int shared_data;

void zephyr_mutex_task(void *p1, void *p2, void *p3) {
    while (1) {
        // Lock mutex (blocks if unavailable)
        if (k_mutex_lock(&data_mutex, K_MSEC(100)) == 0) {
            shared_data++;
            k_mutex_unlock(&data_mutex);
        }
        k_sleep(K_MSEC(10));
    }
}


// ========== SEMAPHORE EXAMPLE ==========

K_SEM_DEFINE(uart_sem, 0, 1);  // Initial=0, Max=1 (binary)

void zephyr_uart_isr(void *arg) {
    // Signal from ISR
    k_sem_give(&uart_sem);
}

void zephyr_uart_task(void *p1, void *p2, void *p3) {
    while (1) {
        // Wait for signal
        k_sem_take(&uart_sem, K_FOREVER);
        process_uart_data();
    }
}


// ========== COUNTING SEMAPHORE ==========

K_SEM_DEFINE(resource_sem, 5, 5);  // Initial=5, Max=5

void acquire_resource(void) {
    k_sem_take(&resource_sem, K_FOREVER);  // Count--
}

void release_resource(void) {
    k_sem_give(&resource_sem);  // Count++
}
*/

/*
 * ============================================================================
 * RECURSIVE MUTEX
 * ============================================================================
 *
 * Normal mutex: Same task taking twice = DEADLOCK
 * Recursive mutex: Same task can take multiple times, must give same number
 *
 * USE CASE: Function that takes mutex calls another function that also
 *           needs the mutex (nested locking)
 *
 * FreeRTOS:
 *   recursive_mutex = xSemaphoreCreateRecursiveMutex();
 *   xSemaphoreTakeRecursive(recursive_mutex, timeout);
 *   xSemaphoreGiveRecursive(recursive_mutex);
 *
 * Zephyr:
 *   Mutexes are recursive by default (track lock count)
 *
 * WARNING: Recursive mutexes can hide design problems!
 *          Consider refactoring instead of using recursive mutex.
 *
 * ============================================================================
 * COMMON MISTAKES
 * ============================================================================
 *
 * 1. USING SEMAPHORE WHERE MUTEX IS NEEDED
 *
 *    WRONG:
 *    SemaphoreHandle_t sem = xSemaphoreCreateBinary();
 *    xSemaphoreGive(sem);  // Make it available
 *
 *    void protect_resource(void) {
 *        xSemaphoreTake(sem, portMAX_DELAY);
 *        // No priority inheritance!
 *        xSemaphoreGive(sem);
 *    }
 *
 *    RIGHT:
 *    SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
 *
 *    void protect_resource(void) {
 *        xSemaphoreTake(mutex, portMAX_DELAY);
 *        // Has priority inheritance!
 *        xSemaphoreGive(mutex);
 *    }
 *
 *
 * 2. GIVING MUTEX FROM ISR
 *
 *    WRONG:
 *    void ISR(void) {
 *        xSemaphoreGiveFromISR(mutex, &wake);  // Can't! Mutex has owner!
 *    }
 *
 *    RIGHT:
 *    void ISR(void) {
 *        xSemaphoreGiveFromISR(binary_sem, &wake);  // OK for signaling
 *    }
 *
 *
 * 3. FORGETTING TO GIVE MUTEX
 *
 *    void buggy_function(void) {
 *        xSemaphoreTake(mutex, portMAX_DELAY);
 *
 *        if (error_condition) {
 *            return;  // OOPS! Mutex never released! DEADLOCK!
 *        }
 *
 *        xSemaphoreGive(mutex);
 *    }
 *
 *    FIX: Always release in all code paths, or use RAII pattern in C++
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. LEAD WITH OWNERSHIP
 *    "The key difference is OWNERSHIP - mutex has it, semaphore doesn't"
 *
 * 2. MENTION PRIORITY INHERITANCE
 *    "Mutex supports priority inheritance to prevent priority inversion"
 *
 * 3. GIVE USE CASES
 *    "Use mutex for protecting shared data"
 *    "Use semaphore for ISR-to-task signaling"
 *
 * 4. KNOW THE ISR RESTRICTION
 *    "You can give semaphore from ISR, but not mutex"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "Can you use semaphore instead of mutex?"
 * A: Technically yes, but you lose priority inheritance. For protecting
 *    shared resources between tasks of different priorities, always use mutex.
 *
 * Q: "Why can't you give mutex from ISR?"
 * A: Mutex has ownership - only the owner (the task that took it) can give it.
 *    ISR is not a task. Use binary semaphore for ISR signaling.
 *
 * Q: "What happens if you give semaphore multiple times?"
 * A: Binary: Saturates at 1 (no effect after first give)
 *    Counting: Increments each time (up to max count)
 *
 * Q: "What happens if you give mutex without taking?"
 * A: Undefined behavior in most RTOS. FreeRTOS may detect this and fail.
 *
 * Q: "How does mutex know who owns it?"
 * A: TCB pointer stored in mutex structure. On give, compares with
 *    current task's TCB.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 07 ===\n\n");
    printf("Q: What is the difference between Mutex and Semaphore?\n\n");
    printf("KEY DIFFERENCE: OWNERSHIP\n");
    printf("- MUTEX: Has owner (only owner can release)\n");
    printf("- SEMAPHORE: No owner (anyone can signal)\n\n");
    printf("MUTEX:\n");
    printf("- Mutual exclusion (protect shared data)\n");
    printf("- Priority inheritance (prevents inversion)\n");
    printf("- Cannot use from ISR\n\n");
    printf("SEMAPHORE:\n");
    printf("- Signaling (ISR to task)\n");
    printf("- Counting resources (binary or counting)\n");
    printf("- No priority inheritance\n");
    printf("- CAN use from ISR (xSemaphoreGiveFromISR)\n\n");
    printf("RULE OF THUMB:\n");
    printf("- Protecting data? Use MUTEX\n");
    printf("- Signaling events? Use SEMAPHORE\n");
    return 0;
}
