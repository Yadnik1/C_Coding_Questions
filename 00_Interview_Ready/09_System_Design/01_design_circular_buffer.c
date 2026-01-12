/*
 * ============================================================================
 * SYSTEM DESIGN: Circular Buffer (Ring Buffer)
 * ============================================================================
 *
 * WHAT IS IT?
 * Fixed-size buffer that wraps around. When full, new data overwrites oldest.
 * Used everywhere in embedded: UART RX/TX, audio buffers, logging.
 *
 * KEY OPERATIONS:
 *   push() - Add element to buffer
 *   pop()  - Remove oldest element
 *   full() - Check if buffer is full
 *   empty()- Check if buffer is empty
 *
 * TIME COMPLEXITY: O(1) for all operations
 * SPACE COMPLEXITY: O(n) where n = buffer size
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define BUFFER_SIZE 8

typedef struct {
    uint8_t data[BUFFER_SIZE];
    int head;    // Where to write next
    int tail;    // Where to read next
    int count;   // Number of elements
} CircularBuffer;

void cb_init(CircularBuffer* cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

bool cb_is_empty(CircularBuffer* cb) {
    return cb->count == 0;
}

bool cb_is_full(CircularBuffer* cb) {
    return cb->count == BUFFER_SIZE;
}

bool cb_push(CircularBuffer* cb, uint8_t value) {
    if (cb_is_full(cb)) return false;              // Buffer full
    cb->data[cb->head] = value;                    // Write at head
    cb->head = (cb->head + 1) % BUFFER_SIZE;       // Wrap around
    cb->count++;
    return true;
}

bool cb_pop(CircularBuffer* cb, uint8_t* value) {
    if (cb_is_empty(cb)) return false;             // Buffer empty
    *value = cb->data[cb->tail];                   // Read from tail
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;       // Wrap around
    cb->count--;
    return true;
}

int main() {
    CircularBuffer cb;
    cb_init(&cb);

    // Push some values
    for (int i = 1; i <= 5; i++) {
        cb_push(&cb, i * 10);
        printf("Pushed %d, count=%d\n", i * 10, cb.count);
    }

    // Pop some values
    uint8_t val;
    while (cb_pop(&cb, &val)) {
        printf("Popped %d, count=%d\n", val, cb.count);
    }

    return 0;
}

/*
 * ============================================================================
 * DRY RUN: Push 10, 20, 30 then Pop twice
 * ============================================================================
 *
 * Buffer size = 8, initially: head=0, tail=0, count=0
 *
 * PUSH 10:
 *   data[0] = 10, head = (0+1)%8 = 1, count = 1
 *   Buffer: [10, _, _, _, _, _, _, _]
 *            ^tail  ^head
 *
 * PUSH 20:
 *   data[1] = 20, head = (1+1)%8 = 2, count = 2
 *   Buffer: [10, 20, _, _, _, _, _, _]
 *            ^tail      ^head
 *
 * PUSH 30:
 *   data[2] = 30, head = (2+1)%8 = 3, count = 3
 *   Buffer: [10, 20, 30, _, _, _, _, _]
 *            ^tail          ^head
 *
 * POP (returns 10):
 *   value = data[0] = 10, tail = (0+1)%8 = 1, count = 2
 *   Buffer: [_, 20, 30, _, _, _, _, _]
 *               ^tail   ^head
 *
 * POP (returns 20):
 *   value = data[1] = 20, tail = (1+1)%8 = 2, count = 1
 *   Buffer: [_, _, 30, _, _, _, _, _]
 *                  ^tail ^head
 *
 * WRAP AROUND EXAMPLE (if we fill buffer):
 *   After pushing 8 elements: head wraps to 0
 *   head = (7+1)%8 = 0
 *
 * ============================================================================
 * INTERVIEW TIPS:
 * - Use modulo (%) for wrap-around: (index + 1) % SIZE
 * - Track count separately OR use (head == tail) with full flag
 * - Common in UART ISR: push in ISR, pop in main loop
 * ============================================================================
 */
