/*
 * ============================================================================
 * QUESTION 32: What are Memory Pools? Why use them instead of malloc?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: High | IMPORTANCE: Critical
 *
 * Memory pools are essential for deterministic embedded systems.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Memory pools are pre-allocated blocks of fixed-size memory. Unlike malloc
 * which searches for free space and can fragment, memory pools provide O(1)
 * constant-time allocation and deallocation. Benefits: (1) Deterministic
 * timing - crucial for real-time, (2) No fragmentation - all blocks same
 * size, (3) Known memory usage at compile time, (4) No allocation failures
 * at runtime if pool properly sized. In FreeRTOS, use heap for variable-size
 * or implement custom pool. Zephyr has k_mem_slab for fixed-size blocks."
 *
 * ============================================================================
 * VISUALIZATION: malloc vs Memory Pool
 * ============================================================================
 *
 *   MALLOC (Variable Size, Fragmentation Risk):
 *
 *   Heap:  [AAA][BBBB][CC][DDDDD][E][FFFFFF]...[free]
 *
 *   After some free/alloc cycles:
 *
 *   Heap:  [AAA][    ][CC][     ][E][      ]...[free]
 *              ^         ^         ^
 *              |         |         |
 *         Small holes - fragmented!
 *
 *   malloc(1000 bytes) -> FAILS!
 *   (Even though total free > 1000, not contiguous)
 *
 *   Time: O(n) - searches free list
 *
 *
 *   MEMORY POOL (Fixed Size, No Fragmentation):
 *
 *   Pool:  [BLK][BLK][BLK][BLK][BLK][BLK][BLK][BLK]
 *           |    |    |    |    |    |    |    |
 *          [U]  [F]  [U]  [F]  [F]  [U]  [F]  [F]
 *           ^         ^              ^
 *           Used      Used           Used
 *
 *   Free list: [2] -> [4] -> [5] -> [7] -> [8] -> NULL
 *
 *   Allocate: Take head of free list  -> O(1)!
 *   Free:     Add to head of free list -> O(1)!
 *
 *   No fragmentation - always can allocate if block available
 *
 * ============================================================================
 * CODE EXAMPLE: Simple Memory Pool
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// SIMPLE MEMORY POOL IMPLEMENTATION
// ============================================================================

#define POOL_BLOCK_SIZE  64
#define POOL_NUM_BLOCKS  10

typedef struct MemoryPool {
    uint8_t memory[POOL_NUM_BLOCKS][POOL_BLOCK_SIZE];
    uint8_t free_list[POOL_NUM_BLOCKS];  // Indices of free blocks
    int free_count;
} MemoryPool_t;

MemoryPool_t my_pool;

void pool_init(MemoryPool_t *pool) {
    // All blocks start free
    for (int i = 0; i < POOL_NUM_BLOCKS; i++) {
        pool->free_list[i] = i;
    }
    pool->free_count = POOL_NUM_BLOCKS;
}

void* pool_alloc(MemoryPool_t *pool) {
    if (pool->free_count == 0) {
        return NULL;  // Pool exhausted
    }

    // Take from free list - O(1)!
    int index = pool->free_list[--pool->free_count];
    return pool->memory[index];
}

void pool_free(MemoryPool_t *pool, void *ptr) {
    // Calculate block index from pointer
    int index = ((uint8_t*)ptr - (uint8_t*)pool->memory) / POOL_BLOCK_SIZE;

    // Return to free list - O(1)!
    pool->free_list[pool->free_count++] = index;
}

/*
 * ============================================================================
 * FREERTOS: Custom Memory Pool with Static Allocation
 * ============================================================================
 */

/*
#include "FreeRTOS.h"
#include "semphr.h"

// Thread-safe memory pool for RTOS

#define MSG_POOL_BLOCK_SIZE  128
#define MSG_POOL_NUM_BLOCKS  20

typedef struct {
    uint8_t data[MSG_POOL_BLOCK_SIZE];
} PoolBlock_t;

typedef struct {
    PoolBlock_t blocks[MSG_POOL_NUM_BLOCKS];
    QueueHandle_t free_queue;  // Queue of free block pointers
} ThreadSafePool_t;

ThreadSafePool_t message_pool;

void message_pool_init(void) {
    // Create queue to hold free block pointers
    message_pool.free_queue = xQueueCreate(MSG_POOL_NUM_BLOCKS,
                                           sizeof(PoolBlock_t*));

    // Initialize all blocks as free
    for (int i = 0; i < MSG_POOL_NUM_BLOCKS; i++) {
        PoolBlock_t *ptr = &message_pool.blocks[i];
        xQueueSend(message_pool.free_queue, &ptr, 0);
    }
}

PoolBlock_t* message_pool_alloc(TickType_t timeout) {
    PoolBlock_t *block = NULL;
    xQueueReceive(message_pool.free_queue, &block, timeout);
    return block;  // NULL if timeout
}

void message_pool_free(PoolBlock_t *block) {
    xQueueSend(message_pool.free_queue, &block, 0);
}

// Usage
void producer_task(void *pvParameters) {
    for (;;) {
        // Allocate from pool (blocks if empty)
        PoolBlock_t *msg = message_pool_alloc(portMAX_DELAY);

        if (msg != NULL) {
            // Fill message
            sprintf((char*)msg->data, "Message %lu", xTaskGetTickCount());

            // Send pointer through queue
            xQueueSend(message_queue, &msg, portMAX_DELAY);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void consumer_task(void *pvParameters) {
    PoolBlock_t *msg;

    for (;;) {
        // Receive message pointer
        if (xQueueReceive(message_queue, &msg, portMAX_DELAY) == pdPASS) {
            // Process message
            printf("Received: %s\n", msg->data);

            // Return to pool
            message_pool_free(msg);
        }
    }
}
*/

/*
 * ============================================================================
 * ZEPHYR MEMORY SLAB (Built-in Memory Pool)
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>

// Define memory slab at compile time
// 20 blocks of 64 bytes each, 4-byte aligned
K_MEM_SLAB_DEFINE(my_slab, 64, 20, 4);

void zephyr_pool_example(void) {
    void *block;

    // Allocate - O(1), deterministic!
    if (k_mem_slab_alloc(&my_slab, &block, K_NO_WAIT) == 0) {
        // Use block
        memset(block, 0, 64);

        // Free - O(1)
        k_mem_slab_free(&my_slab, &block);
    } else {
        printk("Pool exhausted!\n");
    }
}

// Check pool status
void print_slab_status(void) {
    uint32_t used = k_mem_slab_num_used_get(&my_slab);
    uint32_t free = k_mem_slab_num_free_get(&my_slab);

    printk("Slab: %u used, %u free\n", used, free);
}


// Multiple pools for different sizes
K_MEM_SLAB_DEFINE(small_slab, 32, 50, 4);   // 50 x 32 bytes
K_MEM_SLAB_DEFINE(medium_slab, 128, 20, 4); // 20 x 128 bytes
K_MEM_SLAB_DEFINE(large_slab, 512, 5, 4);   // 5 x 512 bytes

void* allocate_best_fit(size_t size) {
    void *block = NULL;

    if (size <= 32) {
        k_mem_slab_alloc(&small_slab, &block, K_NO_WAIT);
    } else if (size <= 128) {
        k_mem_slab_alloc(&medium_slab, &block, K_NO_WAIT);
    } else if (size <= 512) {
        k_mem_slab_alloc(&large_slab, &block, K_NO_WAIT);
    }

    return block;
}
*/

/*
 * ============================================================================
 * MEMORY POOL vs MALLOC COMPARISON
 * ============================================================================
 *
 *   +---------------------+-------------------+------------------------+
 *   | Aspect              | Memory Pool       | malloc/free            |
 *   +---------------------+-------------------+------------------------+
 *   | Allocation time     | O(1) constant     | O(n) varies            |
 *   | Fragmentation       | None              | Can be severe          |
 *   | Memory usage        | Fixed at compile  | Variable at runtime    |
 *   | Failure handling    | Predictable       | Can fail anytime       |
 *   | Size flexibility    | Fixed block size  | Any size               |
 *   | Overhead per block  | None or minimal   | Header per allocation  |
 *   | Real-time safe      | Yes               | No (non-deterministic) |
 *   +---------------------+-------------------+------------------------+
 *
 * ============================================================================
 * USE CASES FOR MEMORY POOLS
 * ============================================================================
 *
 * 1. MESSAGE BUFFERS
 *    - Fixed-size messages between tasks
 *    - Pool of message structures
 *
 * 2. NETWORK PACKETS
 *    - Protocol frames are fixed size
 *    - Need predictable allocation
 *
 * 3. SENSOR DATA BUFFERS
 *    - Sampling produces fixed-size samples
 *    - Pool for sample storage
 *
 * 4. DMA BUFFERS
 *    - DMA needs contiguous aligned memory
 *    - Pre-allocate pool of DMA buffers
 *
 * 5. STATE MACHINE OBJECTS
 *    - FSM instances are same size
 *    - Create/destroy frequently
 *
 * ============================================================================
 * POOL SIZING CONSIDERATIONS
 * ============================================================================
 *
 * FORMULA:
 *
 *   Pool Size >= Max concurrent allocations + Safety margin
 *
 * EXAMPLE - UART Message Pool:
 *
 *   - Max messages in flight: 10 (in queue)
 *   - Processing time: 5ms average
 *   - Arrival rate: 100 msg/sec max
 *   - Burst: 20 messages in 100ms
 *
 *   Pool Size = 10 (queue) + 20 (burst buffer) + 5 (safety) = 35 blocks
 *
 *
 * MONITORING:
 *
 *   Track high water mark:
 *   - How many blocks were ever allocated simultaneously
 *   - If near pool size, increase pool
 *
 * ============================================================================
 * ADVANCED: MULTIPLE POOL SIZES
 * ============================================================================
 */

/*
// Handle different allocation sizes efficiently

typedef struct {
    struct k_mem_slab *slab;
    size_t block_size;
} PoolEntry_t;

PoolEntry_t pools[] = {
    {&small_slab, 32},
    {&medium_slab, 128},
    {&large_slab, 512},
};

void* pool_malloc(size_t size) {
    // Find smallest pool that fits
    for (int i = 0; i < sizeof(pools)/sizeof(pools[0]); i++) {
        if (size <= pools[i].block_size) {
            void *block;
            if (k_mem_slab_alloc(pools[i].slab, &block, K_NO_WAIT) == 0) {
                return block;
            }
        }
    }
    return NULL;  // No suitable pool or all full
}

void pool_free(void *ptr) {
    // Determine which pool this came from
    // (Need to track this - could use header or address range check)
}
*/

/*
 * ============================================================================
 * COMMON MISTAKES
 * ============================================================================
 *
 * MISTAKE 1: Pool too small
 *
 *   // Under high load, pool exhausted
 *   // Solution: Monitor usage, increase size
 *
 *
 * MISTAKE 2: Block size mismatch
 *
 *   // Pool block: 64 bytes
 *   // Actual need: 72 bytes
 *   // Result: Memory corruption!
 *
 *
 * MISTAKE 3: Double free
 *
 *   void *block = pool_alloc(&pool);
 *   pool_free(&pool, block);
 *   pool_free(&pool, block);  // BUG! Double free corrupts free list
 *
 *
 * MISTAKE 4: Using freed block
 *
 *   void *block = pool_alloc(&pool);
 *   pool_free(&pool, block);
 *   memcpy(block, data, size);  // BUG! Block may be reallocated!
 *
 *
 * MISTAKE 5: Not protecting pool in multi-threaded code
 *
 *   // Without mutex, concurrent alloc/free corrupts free list
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN DETERMINISM
 *    "O(1) allocation time, crucial for real-time"
 *
 * 2. CONTRAST WITH MALLOC
 *    "No fragmentation, predictable memory usage"
 *
 * 3. MENTION USE CASES
 *    "Message buffers, network packets, DMA buffers"
 *
 * 4. DISCUSS SIZING
 *    "Size based on max concurrent allocations plus margin"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "How do you handle different allocation sizes?"
 * A: Multiple pools for different sizes (small, medium, large).
 *    Or use heap for rare large allocations, pool for common sizes.
 *
 * Q: "What if pool runs out?"
 * A: Options: (1) Block until available, (2) Return NULL and handle,
 *    (3) Drop oldest item. Depends on application requirements.
 *
 * Q: "How do you detect pool leaks?"
 * A: Track allocation count. If it grows over time without returning
 *    to baseline, there's a leak. Log allocations in debug build.
 *
 * Q: "Can memory pools have priority?"
 * A: Not inherently, but you can implement priority by having separate
 *    pools per priority level, or using queue with priority ordering.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 32 ===\n\n");
    printf("Q: What are Memory Pools?\n\n");
    printf("MEMORY POOLS:\n");
    printf("- Pre-allocated fixed-size blocks\n");
    printf("- O(1) allocation and free\n");
    printf("- No fragmentation\n");
    printf("- Deterministic timing\n\n");
    printf("vs MALLOC:\n");
    printf("- malloc: O(n), variable time, fragmentation risk\n");
    printf("- Pool: O(1), constant time, no fragmentation\n\n");
    printf("ZEPHYR API:\n");
    printf("- K_MEM_SLAB_DEFINE(name, block_size, num_blocks, align)\n");
    printf("- k_mem_slab_alloc(&slab, &block, timeout)\n");
    printf("- k_mem_slab_free(&slab, &block)\n\n");
    printf("USE CASES:\n");
    printf("- Message buffers\n");
    printf("- Network packets\n");
    printf("- DMA buffers\n");
    printf("- Sensor data\n\n");

    // Demo simple pool
    pool_init(&my_pool);
    printf("Pool initialized: %d blocks of %d bytes\n\n",
           POOL_NUM_BLOCKS, POOL_BLOCK_SIZE);

    void *block1 = pool_alloc(&my_pool);
    void *block2 = pool_alloc(&my_pool);
    printf("Allocated 2 blocks\n");
    printf("Free count: %d\n", my_pool.free_count);

    pool_free(&my_pool, block1);
    printf("Freed 1 block\n");
    printf("Free count: %d\n", my_pool.free_count);

    return 0;
}
