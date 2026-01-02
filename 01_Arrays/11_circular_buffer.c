/*
 * ============================================================================
 * PROBLEM: Circular Buffer (Ring Buffer) Implementation
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: VERY HIGH (Embedded)
 *
 * This is THE most important data structure for embedded systems!
 * Used in: UART RX/TX buffers, audio buffers, sensor data logging,
 *          DMA descriptors, interrupt-safe producer-consumer patterns
 *
 * ============================================================================
 * WHAT IS A CIRCULAR BUFFER?
 * ============================================================================
 *
 * A fixed-size buffer that wraps around when it reaches the end.
 * - Producer adds data at 'head' (write pointer)
 * - Consumer reads data from 'tail' (read pointer)
 * - When pointer reaches end, it wraps to beginning
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 * Linear view of circular buffer (size = 8):
 *
 *   +---+---+---+---+---+---+---+---+
 *   | A | B | C |   |   |   |   |   |
 *   +---+---+---+---+---+---+---+---+
 *     0   1   2   3   4   5   6   7
 *             ^   ^
 *           tail head
 *           (read) (write)
 *
 * After more writes (D, E, F, G, H):
 *
 *   +---+---+---+---+---+---+---+---+
 *   | A | B | C | D | E | F | G | H |
 *   +---+---+---+---+---+---+---+---+
 *     ^                           ^
 *   head                        tail
 *   (next write wraps!)
 *
 * After wrap-around (write I, J):
 *
 *   +---+---+---+---+---+---+---+---+
 *   | I | J | C | D | E | F | G | H |
 *   +---+---+---+---+---+---+---+---+
 *         ^   ^
 *       head tail
 *
 * Circular view:
 *
 *        [0]
 *       /   \
 *     [7]   [1]
 *     |       |
 *     [6]   [2]
 *       \   /
 *     [5]-[4]-[3]
 *
 * ============================================================================
 * KEY FORMULAS (MEMORIZE THESE!):
 * ============================================================================
 *
 *   // Move pointer with wrap-around
 *   head = (head + 1) % SIZE;
 *   tail = (tail + 1) % SIZE;
 *
 *   // Check if empty
 *   isEmpty = (head == tail);
 *
 *   // Check if full (keep one slot empty as sentinel)
 *   isFull = ((head + 1) % SIZE == tail);
 *
 *   // Count of elements
 *   count = (head - tail + SIZE) % SIZE;
 *
 * ============================================================================
 * WHY KEEP ONE SLOT EMPTY?
 * ============================================================================
 *
 * Problem: How to distinguish full from empty?
 *   - Empty: head == tail
 *   - Full:  head == tail  <-- SAME CONDITION!
 *
 * Solutions:
 *   1. Keep one slot empty (most common)
 *      - Full: (head + 1) % SIZE == tail
 *      - Wastes 1 slot, but simple
 *
 *   2. Use a separate count variable
 *      - No wasted space
 *      - Extra variable to maintain
 *
 *   3. Use a boolean flag
 *      - Set when last op was write
 *      - Clear when last op was read
 *
 * ============================================================================
 * EMBEDDED USE CASES:
 * ============================================================================
 *
 * 1. UART RECEIVE BUFFER:
 *    - ISR writes received bytes to buffer (producer)
 *    - Main loop reads and processes (consumer)
 *    - Prevents data loss during busy periods
 *
 *    UART_ISR() {
 *        byte = UART_RX_REG;
 *        if (!buffer_full()) {
 *            buffer_write(byte);
 *        }
 *    }
 *
 * 2. AUDIO STREAMING:
 *    - DMA fills buffer with ADC samples
 *    - DSP processes samples
 *    - Double-buffering variant common
 *
 * 3. SENSOR DATA LOGGING:
 *    - Continuous sensor readings
 *    - Keep last N samples for averaging/filtering
 *
 * ============================================================================
 * TIME: O(1) for all operations | SPACE: O(n) where n = buffer size
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 8  // Must be >= 2

// ============================================================================
// IMPLEMENTATION 1: Using Empty Slot as Sentinel
// ============================================================================

typedef struct {
    unsigned char data[BUFFER_SIZE];
    int head;  // Write position
    int tail;  // Read position
} CircularBuffer;

void buffer_init(CircularBuffer* cb) {
    cb->head = 0;
    cb->tail = 0;
    memset(cb->data, 0, BUFFER_SIZE);
}

bool buffer_is_empty(CircularBuffer* cb) {
    return cb->head == cb->tail;
}

bool buffer_is_full(CircularBuffer* cb) {
    return ((cb->head + 1) % BUFFER_SIZE) == cb->tail;
}

int buffer_count(CircularBuffer* cb) {
    return (cb->head - cb->tail + BUFFER_SIZE) % BUFFER_SIZE;
}

// Returns true on success, false if buffer full
bool buffer_write(CircularBuffer* cb, unsigned char byte) {
    if (buffer_is_full(cb)) {
        return false;  // Buffer overflow!
    }

    cb->data[cb->head] = byte;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    return true;
}

// Returns true on success, false if buffer empty
bool buffer_read(CircularBuffer* cb, unsigned char* byte) {
    if (buffer_is_empty(cb)) {
        return false;  // Buffer underflow!
    }

    *byte = cb->data[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    return true;
}

// Peek without removing
bool buffer_peek(CircularBuffer* cb, unsigned char* byte) {
    if (buffer_is_empty(cb)) {
        return false;
    }
    *byte = cb->data[cb->tail];
    return true;
}

// ============================================================================
// IMPLEMENTATION 2: Using Count Variable (No Wasted Slot)
// ============================================================================

typedef struct {
    unsigned char data[BUFFER_SIZE];
    int head;
    int tail;
    int count;  // Track number of elements
} CircularBufferWithCount;

void buffer2_init(CircularBufferWithCount* cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

bool buffer2_is_empty(CircularBufferWithCount* cb) {
    return cb->count == 0;
}

bool buffer2_is_full(CircularBufferWithCount* cb) {
    return cb->count == BUFFER_SIZE;
}

bool buffer2_write(CircularBufferWithCount* cb, unsigned char byte) {
    if (buffer2_is_full(cb)) {
        return false;
    }

    cb->data[cb->head] = byte;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->count++;
    return true;
}

bool buffer2_read(CircularBufferWithCount* cb, unsigned char* byte) {
    if (buffer2_is_empty(cb)) {
        return false;
    }

    *byte = cb->data[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->count--;
    return true;
}

// ============================================================================
// IMPLEMENTATION 3: Overwrite Mode (for continuous logging)
// ============================================================================

typedef struct {
    unsigned char data[BUFFER_SIZE];
    int head;
    int count;
} OverwriteBuffer;

void buffer3_init(OverwriteBuffer* cb) {
    cb->head = 0;
    cb->count = 0;
}

// Always succeeds - overwrites oldest if full
void buffer3_write(OverwriteBuffer* cb, unsigned char byte) {
    cb->data[cb->head] = byte;
    cb->head = (cb->head + 1) % BUFFER_SIZE;

    if (cb->count < BUFFER_SIZE) {
        cb->count++;
    }
    // If full, oldest data is silently overwritten
}

// Get the nth oldest element (0 = oldest)
bool buffer3_get(OverwriteBuffer* cb, int n, unsigned char* byte) {
    if (n >= cb->count) {
        return false;
    }

    int tail = (cb->head - cb->count + BUFFER_SIZE) % BUFFER_SIZE;
    int index = (tail + n) % BUFFER_SIZE;
    *byte = cb->data[index];
    return true;
}

// ============================================================================
// VISUALIZATION HELPER
// ============================================================================

void print_buffer(CircularBuffer* cb) {
    printf("Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (cb->data[i] != 0) {
            printf("%c", cb->data[i]);
        } else {
            printf("_");
        }
        if (i < BUFFER_SIZE - 1) printf("|");
    }
    printf("]\n");
    printf("         ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i == cb->tail && i == cb->head) {
            printf("HT");
        } else if (i == cb->head) {
            printf("H ");
        } else if (i == cb->tail) {
            printf("T ");
        } else {
            printf("  ");
        }
    }
    printf("\n");
    printf("Count: %d, Empty: %d, Full: %d\n\n",
           buffer_count(cb), buffer_is_empty(cb), buffer_is_full(cb));
}

int main() {
    printf("=== Circular Buffer (Ring Buffer) ===\n\n");

    CircularBuffer cb;
    buffer_init(&cb);

    printf("Initial state:\n");
    print_buffer(&cb);

    // Write some data
    printf("Writing A, B, C...\n");
    buffer_write(&cb, 'A');
    buffer_write(&cb, 'B');
    buffer_write(&cb, 'C');
    print_buffer(&cb);

    // Read one
    unsigned char byte;
    buffer_read(&cb, &byte);
    printf("Read: %c\n", byte);
    print_buffer(&cb);

    // Fill it up
    printf("Writing D, E, F, G, H...\n");
    buffer_write(&cb, 'D');
    buffer_write(&cb, 'E');
    buffer_write(&cb, 'F');
    buffer_write(&cb, 'G');
    buffer_write(&cb, 'H');
    print_buffer(&cb);

    // Try to write when full
    printf("Try writing I (should fail - buffer full):\n");
    bool success = buffer_write(&cb, 'I');
    printf("Write success: %d\n\n", success);

    // Read all
    printf("Reading all data:\n");
    while (buffer_read(&cb, &byte)) {
        printf("  Read: %c\n", byte);
    }
    printf("\n");
    print_buffer(&cb);

    // =========================================
    printf("=== Key Formulas Summary ===\n\n");
    printf("// Advance pointer with wrap-around\n");
    printf("ptr = (ptr + 1) %% SIZE;\n\n");
    printf("// Check empty\n");
    printf("isEmpty = (head == tail);\n\n");
    printf("// Check full (sentinel method)\n");
    printf("isFull = ((head + 1) %% SIZE == tail);\n\n");
    printf("// Element count\n");
    printf("count = (head - tail + SIZE) %% SIZE;\n");

    return 0;
}
