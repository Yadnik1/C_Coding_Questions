/*
 * ============================================================================
 * QUESTION 02: What is the difference between Task, Thread, and Process?
 * ============================================================================
 *
 * DIFFICULTY: Basic | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * Interviewers ask this to verify you understand fundamental OS concepts
 * and how they apply differently in RTOS vs GPOS environments.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "In RTOS terminology, a TASK is essentially the same as a thread - it's
 * an independent execution context with its own stack but sharing memory
 * space with other tasks. A PROCESS, in contrast, has its own memory space
 * (protected by MMU). Most RTOS like FreeRTOS don't have true processes
 * because embedded systems typically lack an MMU. All 'tasks' in FreeRTOS
 * share the same address space, which is efficient but requires careful
 * programming to avoid memory corruption."
 *
 * ============================================================================
 * DETAILED COMPARISON
 * ============================================================================
 *
 * +------------------+--------------------+--------------------+------------------+
 * | Aspect           | Process (GPOS)     | Thread (GPOS)      | Task (RTOS)      |
 * +------------------+--------------------+--------------------+------------------+
 * | Memory Space     | Own (protected)    | Shared with proc   | Shared (global)  |
 * | MMU Required     | Yes                | Yes (in process)   | No               |
 * | Creation Cost    | Heavy (~ms)        | Medium (~100us)    | Light (~us)      |
 * | Context Switch   | Heavy (TLB flush)  | Medium             | Light (~1-10us)  |
 * | Communication    | IPC (pipes, etc.)  | Shared memory      | Shared memory    |
 * | Crash Isolation  | Full (protected)   | None in process    | None             |
 * | Resources        | Own file handles   | Shared in process  | Shared globally  |
 * | Stack            | Large (MB)         | Medium (KB-MB)     | Small (100B-KB)  |
 * | Scheduling       | OS scheduler       | OS scheduler       | RTOS scheduler   |
 * +------------------+--------------------+--------------------+------------------+
 *
 * ============================================================================
 * VISUALIZATION: Memory Layout
 * ============================================================================
 *
 * GENERAL PURPOSE OS (with MMU):
 * ===============================
 *
 *   Process A                      Process B
 *   (Own address space)            (Own address space)
 *   +------------------+           +------------------+
 *   | Code (.text)     |           | Code (.text)     |
 *   +------------------+           +------------------+
 *   | Data (.data)     |           | Data (.data)     |
 *   +------------------+           +------------------+
 *   | Heap             |           | Heap             |
 *   +------------------+           +------------------+
 *   |                  |           |                  |
 *   | Thread 1 Stack   |           | Thread 1 Stack   |
 *   +------------------+           +------------------+
 *   | Thread 2 Stack   |           | Thread 2 Stack   |
 *   +------------------+           +------------------+
 *
 *   MMU provides PROTECTION - Process A cannot access Process B's memory
 *   Threads within same process SHARE code, data, heap
 *
 *
 * RTOS (typically no MMU):
 * ========================
 *
 *   Single Flat Address Space
 *   +------------------------------------------+
 *   | Code (.text) - Shared by ALL tasks       |
 *   +------------------------------------------+
 *   | Data (.data/.bss) - Shared, be careful!  |
 *   +------------------------------------------+
 *   | Heap (optional) - Shared                 |
 *   +------------------------------------------+
 *   | Task 1 Stack (e.g., 256 bytes)           |
 *   +------------------------------------------+
 *   | Task 2 Stack (e.g., 512 bytes)           |
 *   +------------------------------------------+
 *   | Task 3 Stack (e.g., 256 bytes)           |
 *   +------------------------------------------+
 *   | Kernel Data                              |
 *   +------------------------------------------+
 *
 *   NO PROTECTION - Any task can access any memory!
 *   Bugs in one task can corrupt data for all tasks!
 *
 * ============================================================================
 * TASK CONTROL BLOCK (TCB)
 * ============================================================================
 *
 * Every task has a TCB that stores its context:
 *
 *   +----------------------------------+
 *   | Task Control Block (TCB)         |
 *   +----------------------------------+
 *   | Task Name (debug)                |
 *   +----------------------------------+
 *   | Stack Pointer (current position) |
 *   +----------------------------------+
 *   | Stack Base (start of stack)      |
 *   +----------------------------------+
 *   | Stack Size                       |
 *   +----------------------------------+
 *   | Priority (current)               |
 *   +----------------------------------+
 *   | Base Priority (original)         |
 *   +----------------------------------+
 *   | Task State (Ready/Blocked/etc)   |
 *   +----------------------------------+
 *   | Event Flags                      |
 *   +----------------------------------+
 *   | Timeout value                    |
 *   +----------------------------------+
 *   | Next/Prev pointers (list links)  |
 *   +----------------------------------+
 *
 * ============================================================================
 * CONTEXT SWITCH: What Gets Saved
 * ============================================================================
 *
 * When switching from Task A to Task B:
 *
 * 1. SAVE Task A's context:
 *    - CPU registers (R0-R15 on ARM)
 *    - Program Counter (PC)
 *    - Status Register (CPSR/xPSR)
 *    - Stack Pointer (SP)
 *
 * 2. UPDATE Task A's TCB with saved SP
 *
 * 3. SELECT next task (scheduler)
 *
 * 4. RESTORE Task B's context from its TCB/stack
 *
 * ARM Cortex-M Context (pushed automatically on exception):
 *
 *   Higher Address
 *   +------------------+
 *   | xPSR             |  <- Pushed first
 *   +------------------+
 *   | Return Address   |
 *   +------------------+
 *   | LR (R14)         |
 *   +------------------+
 *   | R12              |
 *   +------------------+
 *   | R3               |
 *   +------------------+
 *   | R2               |
 *   +------------------+
 *   | R1               |
 *   +------------------+
 *   | R0               |  <- Hardware saves R0-R3, R12, LR, PC, xPSR
 *   +------------------+
 *   | R4               |  <- Software saves R4-R11
 *   +------------------+
 *   | R5               |
 *   +------------------+
 *   | R6               |
 *   +------------------+
 *   | R7               |
 *   +------------------+
 *   | R8               |
 *   +------------------+
 *   | R9               |
 *   +------------------+
 *   | R10              |
 *   +------------------+
 *   | R11              |  <- Pushed last
 *   +------------------+
 *   Lower Address (SP points here)
 *
 * ============================================================================
 * CODE EXAMPLE: Task Creation
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/* FreeRTOS Task Creation */
/*
#include "FreeRTOS.h"
#include "task.h"

// Task function signature - takes void* parameter, returns void
void my_task_function(void *pvParameters) {
    // Cast parameter if needed
    int task_id = (int)pvParameters;

    for (;;) {
        // Task code here
        printf("Task %d running\n", task_id);

        // Yield or delay to allow other tasks to run
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Tasks should NEVER return!
    // If they do, call vTaskDelete(NULL) to delete themselves
}

void create_tasks(void) {
    TaskHandle_t task_handle;
    BaseType_t result;

    // Create a task
    result = xTaskCreate(
        my_task_function,       // Task function
        "MyTask",               // Task name (for debugging)
        configMINIMAL_STACK_SIZE, // Stack size (in words, not bytes!)
        (void *)1,              // Parameter passed to task
        tskIDLE_PRIORITY + 1,   // Priority (higher = more important)
        &task_handle            // Handle for later reference
    );

    if (result != pdPASS) {
        // Task creation failed - probably out of memory
        printf("Failed to create task!\n");
    }
}
*/

/* Zephyr Task (Thread) Creation */
/*
#include <zephyr/kernel.h>

#define MY_STACK_SIZE 512
#define MY_PRIORITY 5

// Define stack at compile time (Zephyr typically uses static allocation)
K_THREAD_STACK_DEFINE(my_stack, MY_STACK_SIZE);
struct k_thread my_thread_data;

void my_thread_function(void *p1, void *p2, void *p3) {
    int thread_id = (int)p1;

    while (1) {
        printk("Thread %d running\n", thread_id);
        k_sleep(K_MSEC(1000));
    }
}

void create_threads(void) {
    k_tid_t tid;

    // Create thread
    tid = k_thread_create(
        &my_thread_data,        // Thread data structure
        my_stack,               // Stack area
        MY_STACK_SIZE,          // Stack size
        my_thread_function,     // Entry function
        (void *)1, NULL, NULL,  // Three parameters (p1, p2, p3)
        MY_PRIORITY,            // Priority (lower = higher priority in Zephyr!)
        0,                      // Options
        K_NO_WAIT               // Start immediately
    );

    // Set thread name for debugging
    k_thread_name_set(tid, "my_thread");
}

// Alternative: Compile-time thread definition
K_THREAD_DEFINE(
    my_static_thread,           // Thread ID
    MY_STACK_SIZE,              // Stack size
    my_thread_function,         // Entry point
    (void *)2, NULL, NULL,      // Parameters
    MY_PRIORITY,                // Priority
    0,                          // Options
    0                           // Delay before starting
);
*/

/*
 * ============================================================================
 * KEY DIFFERENCES: FreeRTOS vs Zephyr Task Creation
 * ============================================================================
 *
 * +------------------------+-------------------------+------------------------+
 * | Aspect                 | FreeRTOS                | Zephyr                 |
 * +------------------------+-------------------------+------------------------+
 * | Stack allocation       | Runtime (heap) or       | Compile-time (static)  |
 * |                        | static with xTaskCreate | preferred              |
 * |                        | Static()                |                        |
 * +------------------------+-------------------------+------------------------+
 * | Stack size units       | WORDS (4 bytes on 32b)  | BYTES                  |
 * +------------------------+-------------------------+------------------------+
 * | Priority values        | Higher = more important | Lower = more important |
 * +------------------------+-------------------------+------------------------+
 * | Parameters             | 1 (void*)               | 3 (void*, void*, void*)|
 * +------------------------+-------------------------+------------------------+
 * | Naming                 | Task                    | Thread                 |
 * +------------------------+-------------------------+------------------------+
 *
 * ============================================================================
 * IMPORTANT: Why RTOS Tasks Share Memory
 * ============================================================================
 *
 * ADVANTAGES of shared memory (no MMU):
 * 1. Fast context switch (no TLB flush)
 * 2. Easy data sharing between tasks
 * 3. Smaller footprint (no MMU tables)
 * 4. Lower hardware requirements
 *
 * DISADVANTAGES:
 * 1. NO protection - bugs can corrupt any memory
 * 2. Security concerns - all tasks can access all data
 * 3. Harder to isolate faults
 *
 * MITIGATION STRATEGIES:
 * 1. Careful code review and testing
 * 2. Static analysis tools
 * 3. Memory protection units (MPU) on some Cortex-M chips
 * 4. Zephyr's memory domains (software-enforced isolation)
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. KNOW THE TERMINOLOGY
 *    FreeRTOS calls them "tasks", Zephyr calls them "threads"
 *    Both are essentially the same concept
 *
 * 2. MENTION THE MMU DIFFERENCE
 *    This shows you understand hardware implications
 *
 * 3. DISCUSS IMPLICATIONS
 *    "Since tasks share memory, I'm careful about global variables
 *    and use mutexes for shared data"
 *
 * 4. STACK SIZE AWARENESS
 *    "I calculate stack requirements carefully because RTOS stacks
 *    are small and overflow causes hard-to-debug crashes"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "How do you determine stack size for a task?"
 * A: Consider: local variables, function call depth, ISR usage,
 *    worst-case printf buffer, RTOS objects. Use stack watermarking
 *    to monitor actual usage. FreeRTOS: uxTaskGetStackHighWaterMark()
 *
 * Q: "What happens if a task overflows its stack?"
 * A: Corrupts adjacent memory (next task's stack, heap, or kernel data).
 *    Usually causes crashes or mysterious behavior. Hard to debug!
 *    Enable stack overflow checking: configCHECK_FOR_STACK_OVERFLOW
 *
 * Q: "Can you have processes in an RTOS?"
 * A: Some RTOS support MPU-based memory protection (Zephyr user mode,
 *    FreeRTOS MPU ports). These provide limited process-like isolation
 *    but not full virtual memory.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 02 ===\n\n");
    printf("Q: What is the difference between Task, Thread, and Process?\n\n");
    printf("Key Points:\n");
    printf("1. TASK (RTOS) = THREAD: Independent execution, shared memory\n");
    printf("2. PROCESS: Own memory space, requires MMU (not in most RTOS)\n");
    printf("3. RTOS tasks share memory - fast but no protection\n");
    printf("4. FreeRTOS: 'task', Zephyr: 'thread' - same concept\n");
    printf("5. Context switch saves CPU registers to task's stack\n");
    printf("6. TCB (Task Control Block) stores task metadata\n");
    return 0;
}
