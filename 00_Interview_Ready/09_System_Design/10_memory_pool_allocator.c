/*
 * ============================================================================
 *        DESIGN: MEMORY POOL ALLOCATOR
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★☆ (HIGH)
 *   - 8-hour continuous operation without memory leaks
 *   - Video frame buffers, sensor data buffers
 *   - Deterministic allocation for real-time constraints
 *   - No heap fragmentation over time
 *
 * ============================================================================
 *                    WHY NOT USE malloc()/free()?
 * ============================================================================
 *
 * PROBLEMS WITH STANDARD HEAP:
 *   1. FRAGMENTATION: After many alloc/free cycles, heap becomes fragmented
 *      - Can't allocate large blocks even with enough total free memory
 *      - Critical failure for 8-hour operation
 *
 *   2. NON-DETERMINISTIC: malloc() time varies wildly
 *      - Best case: O(1) if free block available
 *      - Worst case: O(n) searching free list
 *      - Unacceptable for real-time audio/video
 *
 *   3. MEMORY OVERHEAD: malloc() needs bookkeeping per allocation
 *      - 8-16 bytes overhead per allocation
 *      - Significant for many small allocations
 *
 *   4. THREAD SAFETY: Standard malloc() may need locks
 *      - Priority inversion possible
 *      - Deadlock risk
 *
 * MEMORY POOL ADVANTAGES:
 *   - O(1) allocation and deallocation GUARANTEED
 *   - Zero fragmentation (fixed-size blocks)
 *   - Predictable memory usage
 *   - No metadata overhead per allocation
 *   - Easy to make thread-safe (atomic operations)
 *
 * ============================================================================
 *                    POOL TYPES
 * ============================================================================
 *
 * 1. FIXED-SIZE POOL (Most common for embedded):
 *    - All blocks same size
 *    - O(1) alloc/free using free list
 *    - Perfect for: sensor samples, network packets, video frames
 *
 * 2. VARIABLE-SIZE POOL:
 *    - Multiple fixed pools of different sizes
 *    - Route allocation to appropriate pool
 *    - Example: 32-byte pool + 128-byte pool + 512-byte pool
 *
 * 3. SLAB ALLOCATOR:
 *    - Pools for specific object types
 *    - Preserves object construction state
 *    - Used in Linux kernel
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

// ============================================================================
// APPROACH 1: SIMPLE FREE LIST POOL
// ============================================================================

/*
 * CONCEPT:
 *   - Pre-allocate array of fixed-size blocks
 *   - Maintain linked list of free blocks
 *   - Alloc: Pop from free list (O(1))
 *   - Free: Push to free list (O(1))
 *
 * MEMORY LAYOUT:
 *   +--------+--------+--------+--------+--------+
 *   | Block0 | Block1 | Block2 | Block3 | Block4 |
 *   +--------+--------+--------+--------+--------+
 *       ↓
 *   Free List: Block0 → Block2 → Block4 → NULL
 *   (Blocks 1 and 3 are allocated)
 *
 * TRICK: Store next pointer INSIDE the free block itself!
 *   - When block is free, we can use its memory for the pointer
 *   - When allocated, user gets the whole block
 *   - Zero overhead!
 */

#define POOL_BLOCK_SIZE 64
#define POOL_BLOCK_COUNT 16

typedef union PoolBlock {
    union PoolBlock* next;          // When free: pointer to next free block
    uint8_t data[POOL_BLOCK_SIZE];  // When allocated: user data
} PoolBlock;

typedef struct {
    PoolBlock blocks[POOL_BLOCK_COUNT];
    PoolBlock* free_list;
    uint32_t allocated_count;
    uint32_t high_water_mark;       // Max ever allocated (for monitoring)
} SimplePool;

void simple_pool_init(SimplePool* pool) {
    pool->free_list = NULL;
    pool->allocated_count = 0;
    pool->high_water_mark = 0;

    // Link all blocks into free list
    for (int i = POOL_BLOCK_COUNT - 1; i >= 0; i--) {
        pool->blocks[i].next = pool->free_list;
        pool->free_list = &pool->blocks[i];
    }

    printf("[POOL] Initialized: %d blocks x %d bytes = %zu total\n",
           POOL_BLOCK_COUNT, POOL_BLOCK_SIZE,
           (size_t)POOL_BLOCK_COUNT * POOL_BLOCK_SIZE);
}

void* simple_pool_alloc(SimplePool* pool) {
    if (pool->free_list == NULL) {
        printf("[POOL] ALLOCATION FAILED - pool exhausted!\n");
        return NULL;  // Pool exhausted
    }

    // Pop from free list
    PoolBlock* block = pool->free_list;
    pool->free_list = block->next;

    pool->allocated_count++;
    if (pool->allocated_count > pool->high_water_mark) {
        pool->high_water_mark = pool->allocated_count;
    }

    // Clear block (optional, but good for debugging)
    memset(block->data, 0, POOL_BLOCK_SIZE);

    return block->data;
}

void simple_pool_free(SimplePool* pool, void* ptr) {
    if (ptr == NULL) return;

    // Validate pointer is within pool (defensive programming)
    PoolBlock* block = (PoolBlock*)ptr;
    if (block < pool->blocks || block >= &pool->blocks[POOL_BLOCK_COUNT]) {
        printf("[POOL] ERROR: Freeing invalid pointer!\n");
        return;
    }

    // Push to free list
    block->next = pool->free_list;
    pool->free_list = block;
    pool->allocated_count--;
}

uint32_t simple_pool_available(SimplePool* pool) {
    return POOL_BLOCK_COUNT - pool->allocated_count;
}

// ============================================================================
// APPROACH 2: BITMAP-BASED POOL
// ============================================================================

/*
 * CONCEPT:
 *   - Use bitmap to track which blocks are allocated
 *   - Bit = 0: free, Bit = 1: allocated
 *   - Alloc: Find first zero bit, set it (O(1) with CLZ instruction)
 *   - Free: Clear bit (O(1))
 *
 * ADVANTAGES:
 *   - Can allocate consecutive blocks for larger objects
 *   - Better cache behavior (sequential search)
 *   - Easy to visualize/debug
 *
 * DISADVANTAGES:
 *   - Slightly slower than free list for single block alloc
 *   - Bitmap overhead (1 bit per block)
 */

#define BITMAP_POOL_BLOCKS 32
#define BITMAP_BLOCK_SIZE 128

typedef struct {
    uint8_t storage[BITMAP_POOL_BLOCKS * BITMAP_BLOCK_SIZE];
    uint32_t bitmap;  // 32 bits = 32 blocks
    uint32_t allocated_count;
} BitmapPool;

void bitmap_pool_init(BitmapPool* pool) {
    pool->bitmap = 0;  // All blocks free
    pool->allocated_count = 0;
    memset(pool->storage, 0, sizeof(pool->storage));
    printf("[BITMAP] Initialized: %d blocks x %d bytes\n",
           BITMAP_POOL_BLOCKS, BITMAP_BLOCK_SIZE);
}

// Find first zero bit (Count Leading Zeros approach)
static int find_free_bit(uint32_t bitmap) {
    if (bitmap == 0xFFFFFFFF) return -1;  // All allocated

    // Simple linear search (can use __builtin_clz for optimization)
    for (int i = 0; i < 32; i++) {
        if ((bitmap & (1U << i)) == 0) {
            return i;
        }
    }
    return -1;
}

void* bitmap_pool_alloc(BitmapPool* pool) {
    int bit = find_free_bit(pool->bitmap);
    if (bit < 0) {
        printf("[BITMAP] ALLOCATION FAILED - pool exhausted!\n");
        return NULL;
    }

    // Mark as allocated
    pool->bitmap |= (1U << bit);
    pool->allocated_count++;

    return &pool->storage[bit * BITMAP_BLOCK_SIZE];
}

void bitmap_pool_free(BitmapPool* pool, void* ptr) {
    if (ptr == NULL) return;

    // Calculate block index from pointer
    ptrdiff_t offset = (uint8_t*)ptr - pool->storage;
    int bit = offset / BITMAP_BLOCK_SIZE;

    if (bit < 0 || bit >= BITMAP_POOL_BLOCKS) {
        printf("[BITMAP] ERROR: Invalid pointer!\n");
        return;
    }

    // Mark as free
    pool->bitmap &= ~(1U << bit);
    pool->allocated_count--;
}

// Allocate N consecutive blocks (for larger objects)
void* bitmap_pool_alloc_consecutive(BitmapPool* pool, int count) {
    if (count <= 0 || count > BITMAP_POOL_BLOCKS) return NULL;

    // Find 'count' consecutive free bits
    uint32_t mask = (1U << count) - 1;  // e.g., count=3 -> mask=0b111

    for (int i = 0; i <= BITMAP_POOL_BLOCKS - count; i++) {
        if ((pool->bitmap & (mask << i)) == 0) {
            // Found! Mark all as allocated
            pool->bitmap |= (mask << i);
            pool->allocated_count += count;
            return &pool->storage[i * BITMAP_BLOCK_SIZE];
        }
    }

    printf("[BITMAP] Cannot allocate %d consecutive blocks\n", count);
    return NULL;
}

// ============================================================================
// APPROACH 3: MULTI-SIZE POOL (Variable-Size Allocator)
// ============================================================================

/*
 * CONCEPT:
 *   - Multiple pools with different block sizes
 *   - Route allocation to smallest pool that fits
 *   - Reduces internal fragmentation
 *
 * EXAMPLE:
 *   - Pool A: 32-byte blocks (for small structures)
 *   - Pool B: 128-byte blocks (for medium buffers)
 *   - Pool C: 512-byte blocks (for large buffers)
 *
 * REQUEST 100 bytes → routes to Pool B (128 bytes)
 * REQUEST 20 bytes → routes to Pool A (32 bytes)
 */

#define MULTI_POOL_SIZES 3

typedef struct {
    void* storage;
    void* free_list;
    size_t block_size;
    size_t block_count;
    size_t allocated;
} SubPool;

typedef struct {
    SubPool pools[MULTI_POOL_SIZES];

    // Static storage for each pool
    uint8_t storage_small[16 * 32];    // 16 x 32 bytes
    uint8_t storage_medium[8 * 128];   // 8 x 128 bytes
    uint8_t storage_large[4 * 512];    // 4 x 512 bytes
} MultiSizePool;

static void init_subpool(SubPool* pool, void* storage, size_t block_size, size_t count) {
    pool->storage = storage;
    pool->block_size = block_size;
    pool->block_count = count;
    pool->allocated = 0;

    // Build free list
    pool->free_list = NULL;
    uint8_t* ptr = (uint8_t*)storage;
    for (int i = count - 1; i >= 0; i--) {
        void** block = (void**)(ptr + i * block_size);
        *block = pool->free_list;
        pool->free_list = block;
    }
}

void multi_pool_init(MultiSizePool* mp) {
    init_subpool(&mp->pools[0], mp->storage_small, 32, 16);
    init_subpool(&mp->pools[1], mp->storage_medium, 128, 8);
    init_subpool(&mp->pools[2], mp->storage_large, 512, 4);

    printf("[MULTI] Initialized pools: 32x16, 128x8, 512x4\n");
}

void* multi_pool_alloc(MultiSizePool* mp, size_t size) {
    // Find smallest pool that fits
    for (int i = 0; i < MULTI_POOL_SIZES; i++) {
        SubPool* pool = &mp->pools[i];

        if (size <= pool->block_size && pool->free_list != NULL) {
            // Allocate from this pool
            void* block = pool->free_list;
            pool->free_list = *(void**)block;
            pool->allocated++;

            printf("[MULTI] Allocated %zu bytes from %zu-byte pool\n",
                   size, pool->block_size);
            return block;
        }
    }

    printf("[MULTI] ALLOCATION FAILED for size %zu\n", size);
    return NULL;
}

void multi_pool_free(MultiSizePool* mp, void* ptr) {
    if (ptr == NULL) return;

    // Find which pool this belongs to
    for (int i = 0; i < MULTI_POOL_SIZES; i++) {
        SubPool* pool = &mp->pools[i];
        uint8_t* start = (uint8_t*)pool->storage;
        uint8_t* end = start + pool->block_count * pool->block_size;

        if ((uint8_t*)ptr >= start && (uint8_t*)ptr < end) {
            // Return to this pool's free list
            *(void**)ptr = pool->free_list;
            pool->free_list = ptr;
            pool->allocated--;
            return;
        }
    }

    printf("[MULTI] ERROR: Pointer doesn't belong to any pool!\n");
}

// ============================================================================
// APPROACH 4: RING BUFFER POOL (For Streaming Data)
// ============================================================================

/*
 * CONCEPT:
 *   - Specialized for producer-consumer patterns
 *   - Allocate in order, free in order (FIFO)
 *   - Perfect for: video frames, audio buffers, network packets
 *
 * ADVANTAGE:
 *   - Zero fragmentation (always allocate at tail)
 *   - Very fast (just pointer manipulation)
 *   - Natural flow control (producer waits if pool full)
 *
 * CONSTRAINT:
 *   - Must free in same order as allocated
 *   - Not suitable for random allocation patterns
 */

#define RING_POOL_SIZE 4096
#define RING_BLOCK_SIZE 256

typedef struct {
    uint8_t buffer[RING_POOL_SIZE];
    uint32_t head;  // Next allocation position
    uint32_t tail;  // Next free position
    uint32_t count; // Allocated blocks
} RingPool;

void ring_pool_init(RingPool* pool) {
    pool->head = 0;
    pool->tail = 0;
    pool->count = 0;
    printf("[RING] Initialized: %d bytes, %d-byte blocks\n",
           RING_POOL_SIZE, RING_BLOCK_SIZE);
}

void* ring_pool_alloc(RingPool* pool) {
    uint32_t max_blocks = RING_POOL_SIZE / RING_BLOCK_SIZE;

    if (pool->count >= max_blocks) {
        printf("[RING] FULL - waiting for consumer\n");
        return NULL;
    }

    void* ptr = &pool->buffer[pool->head];
    pool->head = (pool->head + RING_BLOCK_SIZE) % RING_POOL_SIZE;
    pool->count++;

    return ptr;
}

void ring_pool_free(RingPool* pool) {
    if (pool->count == 0) {
        printf("[RING] ERROR: Freeing from empty pool!\n");
        return;
    }

    // Just advance tail (FIFO order enforced)
    pool->tail = (pool->tail + RING_BLOCK_SIZE) % RING_POOL_SIZE;
    pool->count--;
}

// ============================================================================
// THREAD-SAFE POOL (Lock-Free with CAS)
// ============================================================================

/*
 * For multi-threaded environments, pool operations must be atomic.
 *
 * APPROACH 1: Mutex/Spinlock
 *   - Simple but can cause priority inversion
 *   - Acceptable for non-RT tasks
 *
 * APPROACH 2: Lock-Free with Compare-And-Swap (CAS)
 *   - Uses atomic CPU instructions
 *   - No blocking, no priority inversion
 *   - More complex implementation
 *
 * PSEUDO-CODE for lock-free alloc:
 *
 * void* lockfree_alloc(Pool* pool) {
 *     while (1) {
 *         void* head = pool->free_list;  // Atomic read
 *         if (head == NULL) return NULL;
 *
 *         void* next = *(void**)head;
 *
 *         // Atomically: if free_list still == head, set to next
 *         if (CAS(&pool->free_list, head, next)) {
 *             return head;  // Success!
 *         }
 *         // CAS failed, another thread modified - retry
 *     }
 * }
 *
 * On ARM Cortex-M, use LDREX/STREX instructions.
 * On x86, use cmpxchg instruction.
 * In GCC, use __sync_bool_compare_and_swap() or __atomic_*.
 */

// Simulated atomic operations (use real atomics in production)
#ifdef __GNUC__
#define ATOMIC_CAS(ptr, expected, desired) \
    __sync_bool_compare_and_swap(ptr, expected, desired)
#else
// Fallback for demo (NOT THREAD-SAFE!)
#define ATOMIC_CAS(ptr, expected, desired) \
    (*(ptr) == (expected) ? (*(ptr) = (desired), 1) : 0)
#endif

// ============================================================================
// MEMORY POOL MONITORING AND DIAGNOSTICS
// ============================================================================

typedef struct {
    uint32_t total_allocs;
    uint32_t total_frees;
    uint32_t failed_allocs;
    uint32_t high_water_mark;
    uint32_t current_usage;
} PoolStats;

void pool_print_stats(const char* name, PoolStats* stats, size_t pool_size) {
    printf("\n=== %s Statistics ===\n", name);
    printf("Total allocations: %u\n", stats->total_allocs);
    printf("Total frees:       %u\n", stats->total_frees);
    printf("Failed allocs:     %u\n", stats->failed_allocs);
    printf("Current usage:     %u blocks\n", stats->current_usage);
    printf("High water mark:   %u blocks\n", stats->high_water_mark);
    printf("Pool utilization:  %.1f%%\n",
           100.0 * stats->high_water_mark / pool_size);

    if (stats->total_allocs != stats->total_frees + stats->current_usage) {
        printf("WARNING: Possible memory leak detected!\n");
    }
}

// ============================================================================
// DEMO AND TESTING
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   MEMORY POOL ALLOCATOR DEMO\n");
    printf("============================================\n");

    // Demo 1: Simple Free List Pool
    printf("\n--- Simple Free List Pool ---\n");
    SimplePool simple;
    simple_pool_init(&simple);

    void* ptrs[5];
    for (int i = 0; i < 5; i++) {
        ptrs[i] = simple_pool_alloc(&simple);
        printf("Allocated block %d at %p, available: %u\n",
               i, ptrs[i], simple_pool_available(&simple));
    }

    // Free some blocks
    simple_pool_free(&simple, ptrs[1]);
    simple_pool_free(&simple, ptrs[3]);
    printf("After freeing 1 and 3, available: %u\n", simple_pool_available(&simple));

    // Allocate again (reuses freed blocks)
    void* p = simple_pool_alloc(&simple);
    printf("Re-allocated at %p\n", p);

    // Demo 2: Bitmap Pool
    printf("\n--- Bitmap Pool ---\n");
    BitmapPool bitmap;
    bitmap_pool_init(&bitmap);

    void* bp1 = bitmap_pool_alloc(&bitmap);
    void* bp2 = bitmap_pool_alloc(&bitmap);
    printf("Allocated at %p and %p\n", bp1, bp2);
    printf("Bitmap state: 0x%08X\n", bitmap.bitmap);

    // Allocate consecutive blocks
    void* consecutive = bitmap_pool_alloc_consecutive(&bitmap, 4);
    printf("Allocated 4 consecutive blocks at %p\n", consecutive);
    printf("Bitmap state: 0x%08X\n", bitmap.bitmap);

    // Demo 3: Multi-Size Pool
    printf("\n--- Multi-Size Pool ---\n");
    MultiSizePool multi;
    multi_pool_init(&multi);

    void* small = multi_pool_alloc(&multi, 20);   // Goes to 32-byte pool
    void* medium = multi_pool_alloc(&multi, 100); // Goes to 128-byte pool
    void* large = multi_pool_alloc(&multi, 400);  // Goes to 512-byte pool

    multi_pool_free(&multi, medium);
    void* reuse = multi_pool_alloc(&multi, 50);   // Reuses from 128-byte pool

    // Demo 4: Ring Pool
    printf("\n--- Ring Pool (Producer-Consumer) ---\n");
    RingPool ring;
    ring_pool_init(&ring);

    // Producer allocates frames
    printf("Producer allocating...\n");
    for (int i = 0; i < 5; i++) {
        void* frame = ring_pool_alloc(&ring);
        if (frame) {
            sprintf((char*)frame, "Frame %d", i);
            printf("  Produced: %s\n", (char*)frame);
        }
    }

    // Consumer frees frames (must be in order!)
    printf("Consumer processing...\n");
    for (int i = 0; i < 3; i++) {
        printf("  Consumed frame %d\n", i);
        ring_pool_free(&ring);
    }

    printf("Ring pool: %u blocks in use\n", ring.count);

    // Summary
    printf("\n============================================\n");
    printf("   MEMORY POOL COMPARISON\n");
    printf("============================================\n");
    printf("| Approach      | Alloc | Free | Consecutive | Thread-Safe |\n");
    printf("|---------------|-------|------|-------------|-------------|\n");
    printf("| Free List     | O(1)  | O(1) | No          | With CAS    |\n");
    printf("| Bitmap        | O(n)* | O(1) | Yes         | With atomic |\n");
    printf("| Multi-Size    | O(k)  | O(k) | No          | Per-pool    |\n");
    printf("| Ring Buffer   | O(1)  | O(1) | N/A (FIFO)  | Easy        |\n");
    printf("* O(1) with hardware CLZ instruction\n");

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "Why use a memory pool instead of malloc()?"
 * A: "Three main reasons:
 *    1. Deterministic timing - O(1) guaranteed, critical for real-time
 *    2. Zero fragmentation - fixed blocks can't fragment
 *    3. Predictable usage - know exact memory footprint at compile time
 *
 *    For Ironsite's 8-hour operation, heap fragmentation could cause
 *    allocation failures hours into operation. Pool prevents this."
 *
 * Q: "How does the free list trick work?"
 * A: "When a block is free, we store the 'next free' pointer inside the
 *    block itself. This costs zero extra memory because:
 *    - Free blocks aren't being used for anything
 *    - When allocated, user gets the whole block
 *    - Just need block size >= sizeof(void*)
 *
 *    It's like temporary storage - we borrow the space when not in use."
 *
 * Q: "How would you handle multiple block sizes?"
 * A: "Multi-pool approach:
 *    - Create separate pools for each size class (32, 128, 512 bytes)
 *    - Route allocation to smallest pool that fits
 *    - Trade-off: some internal waste vs. flexibility
 *
 *    Alternative: Buddy allocator (splits/merges power-of-2 blocks)"
 *
 * Q: "How do you make the pool thread-safe?"
 * A: "Lock-free with Compare-And-Swap:
 *    1. Read current free_list head
 *    2. Prepare new head value
 *    3. CAS: atomically update only if unchanged
 *    4. If CAS fails, retry (another thread modified)
 *
 *    On Cortex-M: LDREX/STREX instructions
 *    Avoids priority inversion that mutexes can cause."
 *
 * Q: "What's the trade-off between pool size and memory waste?"
 * A: "Internal fragmentation vs. flexibility:
 *    - Larger blocks: More waste per allocation, but simpler
 *    - Smaller blocks: Less waste, but may not fit all requests
 *    - Multiple sizes: Best fit, but more complex
 *
 *    Profile your application to find the right size classes.
 *    For video frames: use exact frame size.
 *    For general use: power-of-2 sizes are common."
 *
 * Q: "How do you handle pool exhaustion?"
 * A: "Several strategies:
 *    1. Return NULL - caller must handle (simplest, most common)
 *    2. Block/wait - for producer-consumer with flow control
 *    3. Emergency pool - small reserve for critical allocations
 *    4. Preempt lower priority - reclaim from less important tasks
 *
 *    Also: monitor high-water mark to size pool appropriately."
 *
 * ============================================================================
 *                    IRONSITE-SPECIFIC CONSIDERATIONS
 * ============================================================================
 *
 * VIDEO FRAME BUFFERS:
 *   - Fixed frame size (e.g., 640x480 = 307KB)
 *   - Ring pool perfect for frame pipeline
 *   - 3-5 frames typically sufficient (producer/consumer)
 *
 * SENSOR DATA:
 *   - Small fixed packets (e.g., 32-byte IMU samples)
 *   - Simple free list pool
 *   - Size for burst capture (100+ samples)
 *
 * NETWORK BUFFERS:
 *   - Variable packet sizes
 *   - Multi-size pool (64/256/1500 bytes)
 *   - Or: use MTU-sized pool for simplicity
 *
 * AUDIO BUFFERS:
 *   - Fixed-size chunks (e.g., 512 samples)
 *   - Ring pool for streaming
 *   - Double-buffer: one filling, one processing
 *
 * ============================================================================
 */
