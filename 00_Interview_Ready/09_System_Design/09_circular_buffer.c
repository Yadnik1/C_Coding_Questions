/*
 * ============================================================================
 *        DESIGN: CIRCULAR BUFFER WITH FULL/EMPTY HANDLING
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★★ (CRITICAL)
 *   - Video frame buffering
 *   - Audio sample buffering
 *   - UART RX/TX buffers
 *   - Sensor data logging
 *   - Event queues
 *
 * ============================================================================
 *                    PROBLEM STATEMENT
 * ============================================================================
 *
 * Need a fixed-size buffer that:
 *   - Works without dynamic memory allocation
 *   - Efficiently reuses memory (wrap around)
 *   - Safe for ISR producer + main consumer (or vice versa)
 *   - Clearly distinguishes FULL from EMPTY state
 *
 * LINEAR BUFFER PROBLEM:
 *
 *   [D][D][D][D][D][D][D][D]  <- Buffer full
 *    ^                    ^
 *   read               write
 *
 *   After reading all: must copy remaining data to start!
 *   Inefficient, complex, interrupts must be disabled
 *
 * CIRCULAR BUFFER SOLUTION:
 *
 *   Treat buffer as a ring - wrap around at end
 *
 *   Write →  [4][5][6][_][_][_][1][2][3]  ← Read
 *                      ^       ^
 *                    head    tail
 *
 * ============================================================================
 *                    THE FULL vs EMPTY PROBLEM
 * ============================================================================
 *
 * When head == tail, is buffer FULL or EMPTY?
 *
 * SOLUTIONS:
 *
 * 1. WASTE ONE SLOT:
 *    - Full when (head + 1) % size == tail
 *    - Empty when head == tail
 *    - Simple, commonly used
 *    - Wastes one element of storage
 *
 * 2. SEPARATE COUNT:
 *    - Track element count separately
 *    - Full when count == size
 *    - Empty when count == 0
 *    - Uses full capacity
 *    - Count access needs synchronization
 *
 * 3. MIRROR INDICATOR:
 *    - Use extra bit to indicate wrap count
 *    - Complex but efficient
 *
 * 4. READ/WRITE COUNTS:
 *    - Track total reads and writes
 *    - Full when writes - reads == size
 *    - Uses full capacity, simple
 *
 * ============================================================================
 *                    DESIGN DECISIONS
 * ============================================================================
 *
 * 1. POWER-OF-2 SIZE:
 *    - Allows & (size-1) instead of % size
 *    - Faster on MCUs without hardware divider
 *    - Recommended for ISR-accessed buffers
 *
 * 2. VOLATILE POINTERS:
 *    - Required when ISR accesses buffer
 *    - Prevents compiler optimization issues
 *
 * 3. LOCK-FREE DESIGN:
 *    - Single producer + single consumer = no locks needed
 *    - Producer only writes head, consumer only writes tail
 *    - Both can read both pointers
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// IMPLEMENTATION 1: WASTE-ONE-SLOT (Simplest, ISR-safe)
// ============================================================================

/*
 * MEMORY LAYOUT:
 *
 *   Empty: head == tail
 *   [_][_][_][_][_][_][_][_]
 *    ^
 *   head,tail
 *
 *   After writes: tail points to oldest, head to next write
 *   [A][B][C][D][_][_][_][_]
 *    ^          ^
 *   tail       head
 *
 *   Full: (head + 1) % size == tail  (one slot wasted)
 *   [A][B][C][D][E][F][G][_]
 *    ^                   ^
 *   tail                head
 *
 *   Can't write to last slot - would make head == tail (looks empty!)
 */

#define RING_SIZE 8  // Must be power of 2 for optimization

typedef struct {
    uint8_t buffer[RING_SIZE];
    volatile uint8_t head;  // Write position
    volatile uint8_t tail;  // Read position
} RingBuffer;

void ring_init(RingBuffer* rb) {
    rb->head = 0;
    rb->tail = 0;
    memset(rb->buffer, 0, RING_SIZE);
}

bool ring_is_empty(RingBuffer* rb) {
    return rb->head == rb->tail;
}

bool ring_is_full(RingBuffer* rb) {
    return ((rb->head + 1) & (RING_SIZE - 1)) == rb->tail;
}

uint8_t ring_count(RingBuffer* rb) {
    return (rb->head - rb->tail) & (RING_SIZE - 1);
}

uint8_t ring_free_space(RingBuffer* rb) {
    return (RING_SIZE - 1) - ring_count(rb);  // -1 for wasted slot
}

// Push one byte (call from producer, e.g., ISR)
bool ring_push(RingBuffer* rb, uint8_t data) {
    if (ring_is_full(rb)) {
        return false;  // Buffer full
    }

    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) & (RING_SIZE - 1);  // Wrap around

    return true;
}

// Pop one byte (call from consumer, e.g., main loop)
bool ring_pop(RingBuffer* rb, uint8_t* data) {
    if (ring_is_empty(rb)) {
        return false;  // Buffer empty
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) & (RING_SIZE - 1);  // Wrap around

    return true;
}

// Peek without removing
bool ring_peek(RingBuffer* rb, uint8_t* data) {
    if (ring_is_empty(rb)) {
        return false;
    }
    *data = rb->buffer[rb->tail];
    return true;
}


// ============================================================================
// IMPLEMENTATION 2: WITH COUNT (Uses full capacity)
// ============================================================================

typedef struct {
    uint8_t buffer[RING_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
    volatile uint8_t count;  // Track element count
} RingBufferCount;

void ring_count_init(RingBufferCount* rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

bool ring_count_push(RingBufferCount* rb, uint8_t data) {
    if (rb->count >= RING_SIZE) {
        return false;  // Full
    }

    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) & (RING_SIZE - 1);
    rb->count++;  // NOTE: Not atomic! Need protection if ISR accesses

    return true;
}

bool ring_count_pop(RingBufferCount* rb, uint8_t* data) {
    if (rb->count == 0) {
        return false;  // Empty
    }

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) & (RING_SIZE - 1);
    rb->count--;  // NOTE: Not atomic!

    return true;
}


// ============================================================================
// IMPLEMENTATION 3: BLOCK-BASED (For DMA, larger data)
// ============================================================================

/*
 * For video/audio, often transfer in blocks, not bytes
 *
 * DMA writes entire block, then signals completion
 * Processor reads entire block at once
 */

#define BLOCK_SIZE 256
#define NUM_BLOCKS 4

typedef struct {
    uint8_t blocks[NUM_BLOCKS][BLOCK_SIZE];
    volatile uint8_t write_block;  // DMA writes here
    volatile uint8_t read_block;   // CPU reads from here
    volatile uint8_t ready_count;  // Blocks ready for reading
} BlockBuffer;

void block_buffer_init(BlockBuffer* bb) {
    bb->write_block = 0;
    bb->read_block = 0;
    bb->ready_count = 0;
}

// Get pointer to next block for writing (called before DMA)
uint8_t* block_buffer_get_write_ptr(BlockBuffer* bb) {
    if (bb->ready_count >= NUM_BLOCKS) {
        return NULL;  // All blocks full
    }
    return bb->blocks[bb->write_block];
}

// Mark block as written (called from DMA complete ISR)
void block_buffer_write_complete(BlockBuffer* bb) {
    bb->write_block = (bb->write_block + 1) % NUM_BLOCKS;
    bb->ready_count++;
}

// Get pointer to next block for reading
uint8_t* block_buffer_get_read_ptr(BlockBuffer* bb) {
    if (bb->ready_count == 0) {
        return NULL;  // No blocks ready
    }
    return bb->blocks[bb->read_block];
}

// Mark block as read (release for writing)
void block_buffer_read_complete(BlockBuffer* bb) {
    bb->read_block = (bb->read_block + 1) % NUM_BLOCKS;
    bb->ready_count--;
}


// ============================================================================
// IMPLEMENTATION 4: GENERIC RING BUFFER (Any element type)
// ============================================================================

#define GENERIC_RING_SIZE 16

typedef struct {
    void* buffer;
    size_t elem_size;
    size_t capacity;
    volatile size_t head;
    volatile size_t tail;
} GenericRing;

void generic_ring_init(GenericRing* gr, void* buffer, size_t elem_size, size_t capacity) {
    gr->buffer = buffer;
    gr->elem_size = elem_size;
    gr->capacity = capacity;
    gr->head = 0;
    gr->tail = 0;
}

bool generic_ring_push(GenericRing* gr, const void* elem) {
    size_t next_head = (gr->head + 1) % gr->capacity;
    if (next_head == gr->tail) {
        return false;  // Full
    }

    uint8_t* dest = (uint8_t*)gr->buffer + (gr->head * gr->elem_size);
    memcpy(dest, elem, gr->elem_size);
    gr->head = next_head;

    return true;
}

bool generic_ring_pop(GenericRing* gr, void* elem) {
    if (gr->head == gr->tail) {
        return false;  // Empty
    }

    uint8_t* src = (uint8_t*)gr->buffer + (gr->tail * gr->elem_size);
    memcpy(elem, src, gr->elem_size);
    gr->tail = (gr->tail + 1) % gr->capacity;

    return true;
}


// ============================================================================
// USAGE EXAMPLE: UART RX BUFFER
// ============================================================================

RingBuffer uart_rx_buffer;

// Called from UART RX interrupt
void uart_rx_isr(uint8_t received_byte) {
    if (!ring_push(&uart_rx_buffer, received_byte)) {
        // Buffer full - byte lost!
        // Could: set overflow flag, or overwrite oldest
        printf("[ISR] UART RX buffer overflow!\n");
    }
}

// Called from main loop to process received data
void uart_process_rx(void) {
    uint8_t byte;
    while (ring_pop(&uart_rx_buffer, &byte)) {
        printf("[MAIN] Received: 0x%02X\n", byte);
        // Process byte...
    }
}


// ============================================================================
// ADVANCED: OVERWRITE MODE (For sensor logging)
// ============================================================================

/*
 * Some applications want to keep LATEST data, dropping oldest
 * Example: GPS position logging - always want current position
 */

typedef struct {
    uint8_t buffer[RING_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} OverwriteRing;

void overwrite_ring_init(OverwriteRing* rb) {
    rb->head = 0;
    rb->tail = 0;
}

// Always succeeds - overwrites oldest if full
void overwrite_ring_push(OverwriteRing* rb, uint8_t data) {
    uint8_t next_head = (rb->head + 1) & (RING_SIZE - 1);

    if (next_head == rb->tail) {
        // Buffer full - advance tail (drop oldest)
        rb->tail = (rb->tail + 1) & (RING_SIZE - 1);
    }

    rb->buffer[rb->head] = data;
    rb->head = next_head;
}


// ============================================================================
// DEMO
// ============================================================================

void print_buffer_state(RingBuffer* rb) {
    printf("  [");
    for (int i = 0; i < RING_SIZE; i++) {
        if (i == rb->head && i == rb->tail) {
            printf("HT");
        } else if (i == rb->head) {
            printf("H");
        } else if (i == rb->tail) {
            printf("T");
        } else {
            printf(" ");
        }
    }
    printf("] count=%d, empty=%d, full=%d\n",
           ring_count(rb), ring_is_empty(rb), ring_is_full(rb));
}

int main() {
    printf("============================================\n");
    printf("   CIRCULAR BUFFER DEMONSTRATION\n");
    printf("============================================\n");

    RingBuffer rb;
    ring_init(&rb);

    printf("\n--- Initial state ---\n");
    print_buffer_state(&rb);

    // Push some data
    printf("\n--- Push 5 bytes ---\n");
    for (uint8_t i = 1; i <= 5; i++) {
        ring_push(&rb, i);
        printf("Pushed %d: ", i);
        print_buffer_state(&rb);
    }

    // Pop some data
    printf("\n--- Pop 3 bytes ---\n");
    uint8_t data;
    for (int i = 0; i < 3; i++) {
        if (ring_pop(&rb, &data)) {
            printf("Popped %d: ", data);
            print_buffer_state(&rb);
        }
    }

    // Fill to capacity
    printf("\n--- Fill to capacity (7 elements, 1 wasted) ---\n");
    while (!ring_is_full(&rb)) {
        static uint8_t val = 10;
        ring_push(&rb, val++);
    }
    print_buffer_state(&rb);

    // Try to push when full
    printf("\n--- Try push when full ---\n");
    bool success = ring_push(&rb, 99);
    printf("Push returned: %s\n", success ? "true" : "false");

    // Demonstrate wrap-around
    printf("\n--- Wrap-around demo ---\n");
    ring_pop(&rb, &data);
    ring_pop(&rb, &data);
    ring_push(&rb, 20);
    ring_push(&rb, 21);
    print_buffer_state(&rb);

    // UART simulation
    printf("\n--- UART RX Simulation ---\n");
    ring_init(&uart_rx_buffer);
    uart_rx_isr('H');
    uart_rx_isr('e');
    uart_rx_isr('l');
    uart_rx_isr('l');
    uart_rx_isr('o');
    uart_process_rx();

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "How do you distinguish full from empty?"
 * A: "Two common approaches:
 *    1. Waste one slot: Full when (head+1)%size == tail
 *       Simple, no extra variable, wastes 1 element
 *    2. Track count: Full when count == size
 *       Uses full capacity, but count needs synchronization
 *
 *    For ISR safety, I prefer waste-one-slot approach."
 *
 * Q: "Why use power-of-2 size?"
 * A: "Allows using bitwise AND instead of modulo:
 *       head = (head + 1) & (SIZE - 1)  // Fast
 *    vs head = (head + 1) % SIZE        // Division, slow on MCU
 *
 *    On Cortex-M0 (no hardware divide), this is ~10x faster."
 *
 * Q: "Is this implementation thread-safe?"
 * A: "For single producer + single consumer: yes, lock-free.
 *    - Producer only writes head
 *    - Consumer only writes tail
 *    - Both can read both (volatile prevents caching)
 *
 *    For multiple producers or consumers: need mutex."
 *
 * Q: "Buffer overflow handling?"
 * A: "Options:
 *    1. Return false, caller handles (my implementation)
 *    2. Block until space available (RTOS with semaphore)
 *    3. Overwrite oldest data (sensor logging)
 *    4. Trigger error/assertion (debug builds)
 *
 *    Choice depends on: Can data be lost? Can producer wait?"
 *
 * ============================================================================
 *                    EDGE CASES
 * ============================================================================
 *
 * 1. SINGLE ELEMENT:
 *    - Push 1 element to empty buffer: head advances
 *    - Pop 1 element: tail advances, buffer empty again
 *    - Works correctly
 *
 * 2. RAPID FILL/DRAIN:
 *    - Producer faster than consumer: eventually fills
 *    - Must handle overflow gracefully
 *
 * 3. EXACTLY FULL:
 *    - With waste-one-slot: can store SIZE-1 elements
 *    - Document this limitation!
 *
 * 4. SIZE = 1:
 *    - With waste-one-slot: usable capacity = 0!
 *    - Minimum useful size = 2
 *
 * 5. WRAP-AROUND BOUNDARY:
 *    - When head or tail wraps from SIZE-1 to 0
 *    - & (SIZE-1) handles this correctly for power-of-2
 *
 * ============================================================================
 *                    PERFORMANCE
 * ============================================================================
 *
 * Operation      | Time Complexity | Notes
 * ---------------|-----------------|---------------------------
 * push           | O(1)            | Single write + pointer update
 * pop            | O(1)            | Single read + pointer update
 * is_empty       | O(1)            | Compare pointers
 * is_full        | O(1)            | Compare pointers
 * count          | O(1)            | Subtraction + mask
 *
 * Memory: SIZE + 2 bytes (for head/tail) per buffer
 *
 * ============================================================================
 */
