/*
 * ============================================================================
 * QUESTION 15: How does Memory Management work in RTOS?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: High | IMPORTANCE: Critical
 *
 * Memory management is crucial in embedded systems with limited resources.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "RTOS memory management differs from desktop systems - no virtual memory,
 * often no MMU. FreeRTOS provides five heap implementations (heap_1 to heap_5)
 * ranging from simple bump allocator to full malloc/free with coalescence.
 * Best practice in embedded: prefer STATIC ALLOCATION where possible to avoid
 * fragmentation and ensure deterministic behavior. Zephyr supports memory pools,
 * slabs (fixed-size blocks), and kernel heap. Key concerns are fragmentation,
 * deterministic allocation time, and avoiding allocation failures at runtime."
 *
 * ============================================================================
 * MEMORY TYPES IN EMBEDDED SYSTEMS
 * ============================================================================
 *
 *   +------------------+------------------------------------------+
 *   | Memory Type      | Characteristics                          |
 *   +------------------+------------------------------------------+
 *   | STATIC (.data)   | Initialized variables, fixed at compile  |
 *   | BSS (.bss)       | Zero-initialized, fixed at compile       |
 *   | STACK            | Local variables, grows/shrinks per task  |
 *   | HEAP             | Dynamic allocation (malloc/free)         |
 *   +------------------+------------------------------------------+
 *
 *
 *   Memory Layout (typical):
 *
 *   High Address
 *   +------------------+
 *   | Stack (Task 1)   |  ↓ Grows down
 *   +------------------+
 *   | Stack (Task 2)   |  ↓ Grows down
 *   +------------------+
 *   | ...              |
 *   +------------------+
 *   | Heap             |  ↑ Grows up (dynamic)
 *   +------------------+
 *   | .bss             |  Uninitialized globals
 *   +------------------+
 *   | .data            |  Initialized globals
 *   +------------------+
 *   | .text            |  Code (ROM/Flash)
 *   +------------------+
 *   Low Address
 *
 * ============================================================================
 * FREERTOS HEAP IMPLEMENTATIONS
 * ============================================================================
 *
 * heap_1: Allocate Only (No Free)
 * --------------------------------
 *   - Simple bump allocator
 *   - No free() - memory never returned
 *   - Deterministic O(1) allocation
 *   - USE WHEN: Objects created at startup, never deleted
 *
 *   void *pvPortMalloc(size_t size) {
 *       void *ptr = &heap[nextFree];
 *       nextFree += size;
 *       return ptr;
 *   }
 *
 *
 * heap_2: Best Fit (Deprecated)
 * --------------------------------
 *   - Allocate and free supported
 *   - Best-fit algorithm
 *   - No coalescence (adjacent free blocks not merged)
 *   - Can cause fragmentation
 *   - USE WHEN: Tasks/queues created and deleted, same sizes
 *
 *
 * heap_3: Wrapper Around malloc()
 * --------------------------------
 *   - Uses compiler's malloc/free
 *   - Adds thread safety (suspends scheduler)
 *   - Not deterministic
 *   - USE WHEN: Already using malloc, need thread safety
 *
 *
 * heap_4: First Fit with Coalescence
 * --------------------------------
 *   - Allocate and free
 *   - Coalesces adjacent free blocks
 *   - Good balance of features
 *   - Most commonly used
 *   - USE WHEN: General purpose dynamic allocation
 *
 *   Coalescence:
 *   Before: [Used][Free][Free][Used]
 *   After:  [Used][    Free    ][Used]
 *
 *
 * heap_5: Multiple Memory Regions
 * --------------------------------
 *   - Like heap_4 but spans non-contiguous memory
 *   - Useful for multiple RAM regions
 *   - USE WHEN: Multiple RAM banks, external RAM
 *
 * ============================================================================
 * FRAGMENTATION PROBLEM
 * ============================================================================
 *
 * EXTERNAL FRAGMENTATION:
 *
 *   Initial: [                HEAP                          ]
 *
 *   Allocate: [AAA][BBBBB][CCCC][DDDDDD][EEEE]
 *
 *   Free B,D: [AAA][     ][CCCC][      ][EEEE]
 *                   ^           ^
 *                   |           |
 *               5 bytes     6 bytes free
 *
 *   Try to allocate 10 bytes: FAILS!
 *   (Even though 11 bytes total are free, not contiguous)
 *
 *
 * SOLUTIONS:
 * 1. Fixed-size blocks (memory pools/slabs)
 * 2. Coalescence (heap_4/heap_5)
 * 3. Static allocation (avoid dynamic entirely)
 * 4. Allocate once at startup (heap_1)
 *
 * ============================================================================
 * ZEPHYR MEMORY OPTIONS
 * ============================================================================
 *
 * 1. MEMORY SLAB (Fixed-Size Blocks)
 * --------------------------------
 *   - Pre-allocated blocks of same size
 *   - O(1) allocation and free
 *   - No fragmentation possible!
 */

#include <stdio.h>
#include <stdint.h>

/*
// Zephyr Memory Slab
K_MEM_SLAB_DEFINE(my_slab, 64, 10, 4);  // 10 blocks of 64 bytes

void slab_example(void) {
    void *block;

    // Allocate block (O(1), deterministic)
    if (k_mem_slab_alloc(&my_slab, &block, K_NO_WAIT) == 0) {
        // Use block...
        memset(block, 0, 64);

        // Free block
        k_mem_slab_free(&my_slab, &block);
    }
}


// 2. MEMORY POOL (Variable-Size Blocks)
// --------------------------------
K_HEAP_DEFINE(my_heap, 1024);  // 1KB heap

void heap_example(void) {
    void *ptr = k_heap_alloc(&my_heap, 128, K_NO_WAIT);
    if (ptr != NULL) {
        // Use memory...
        k_heap_free(&my_heap, ptr);
    }
}


// 3. KERNEL HEAP
// --------------------------------
// System-wide heap, configured with CONFIG_HEAP_MEM_POOL_SIZE

void kernel_heap_example(void) {
    void *ptr = k_malloc(256);
    if (ptr != NULL) {
        // Use memory...
        k_free(ptr);
    }
}
*/

/*
 * ============================================================================
 * STATIC vs DYNAMIC ALLOCATION
 * ============================================================================
 *
 * STATIC ALLOCATION (Preferred in Embedded):
 *
 *   // FreeRTOS static task creation
 *   StaticTask_t taskBuffer;
 *   StackType_t stackBuffer[256];
 *
 *   TaskHandle_t task = xTaskCreateStatic(
 *       task_func,
 *       "MyTask",
 *       256,              // Stack size
 *       NULL,             // Parameter
 *       2,                // Priority
 *       stackBuffer,      // Stack buffer
 *       &taskBuffer       // Task buffer
 *   );
 *
 *   // Zephyr static thread
 *   K_THREAD_STACK_DEFINE(my_stack, 512);
 *   struct k_thread my_thread;
 *
 *   ADVANTAGES:
 *   + No runtime allocation failure
 *   + No fragmentation
 *   + Deterministic
 *   + Known memory usage at compile time
 *
 *   DISADVANTAGES:
 *   - Less flexible
 *   - Memory wasted if not used
 *
 *
 * DYNAMIC ALLOCATION:
 *
 *   // FreeRTOS dynamic task creation
 *   TaskHandle_t task;
 *   xTaskCreate(task_func, "MyTask", 256, NULL, 2, &task);
 *
 *   ADVANTAGES:
 *   + Flexible
 *   + Memory reused
 *
 *   DISADVANTAGES:
 *   - Can fail at runtime
 *   - Fragmentation risk
 *   - Non-deterministic (depends on heap state)
 *
 * ============================================================================
 * BEST PRACTICES
 * ============================================================================
 *
 * 1. PREFER STATIC ALLOCATION
 *    - Use xTaskCreateStatic, xQueueCreateStatic, etc.
 *    - Memory usage known at compile time
 *
 * 2. IF DYNAMIC, USE MEMORY POOLS
 *    - Fixed-size blocks avoid fragmentation
 *    - Deterministic allocation time
 *
 * 3. ALLOCATE AT STARTUP
 *    - Create all objects during initialization
 *    - No runtime allocation = no runtime failure
 *
 * 4. NEVER FRAGMENT
 *    - Same-size allocations
 *    - Or use heap_4/heap_5 with coalescence
 *
 * 5. MONITOR HEAP USAGE
 *    - xPortGetFreeHeapSize()
 *    - xPortGetMinimumEverFreeHeapSize()
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. MENTION STATIC PREFERENCE
 *    "I prefer static allocation in embedded to avoid runtime failures"
 *
 * 2. KNOW HEAP IMPLEMENTATIONS
 *    "FreeRTOS has 5 heap options, heap_4 is most common for general use"
 *
 * 3. DISCUSS FRAGMENTATION
 *    "Fragmentation is a key concern, memory pools solve this"
 *
 * 4. MENTION DETERMINISM
 *    "For hard real-time, need deterministic allocation time"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "How do you detect memory leaks in RTOS?"
 * A: Monitor xPortGetFreeHeapSize() over time. If it decreases steadily,
 *    there's a leak. Use tools like heap tracing, or implement custom
 *    allocation tracking.
 *
 * Q: "What happens if malloc fails?"
 * A: Returns NULL. Must always check! Many bugs come from unchecked malloc.
 *    In FreeRTOS, can also use configASSERT and malloc failed hook.
 *
 * Q: "When would you use heap_5?"
 * A: Multiple non-contiguous RAM regions. Some MCUs have internal RAM +
 *    external SDRAM. heap_5 combines them into one heap.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 15 ===\n\n");
    printf("Q: How does Memory Management work in RTOS?\n\n");
    printf("FREERTOS HEAP OPTIONS:\n");
    printf("- heap_1: Allocate only (no free)\n");
    printf("- heap_2: Best fit (deprecated)\n");
    printf("- heap_3: Wrapper for malloc\n");
    printf("- heap_4: First fit + coalescence (common)\n");
    printf("- heap_5: Multiple memory regions\n\n");
    printf("ZEPHYR OPTIONS:\n");
    printf("- Memory Slab: Fixed-size blocks, O(1)\n");
    printf("- Memory Pool: Variable-size\n");
    printf("- Kernel Heap: System-wide k_malloc/k_free\n\n");
    printf("BEST PRACTICES:\n");
    printf("1. Prefer STATIC allocation\n");
    printf("2. Use memory pools to avoid fragmentation\n");
    printf("3. Allocate at startup, not runtime\n");
    printf("4. Monitor heap with xPortGetFreeHeapSize()\n");
    return 0;
}
