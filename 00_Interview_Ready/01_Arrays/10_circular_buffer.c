// Circular Buffer (Ring Buffer) - Essential for embedded systems
// Time: O(1) for all operations, Space: O(n) for buffer storage

#include <stdio.h>
#include <stdbool.h>

// Say: "Fixed size buffer - common in embedded where memory is limited"
#define BUFFER_SIZE 5

// Say: "Struct to encapsulate all buffer state"
typedef struct {
    int buffer[BUFFER_SIZE];  // Say: "The actual data storage"
    int head;                 // Say: "Write position - where next element goes"
    int tail;                 // Say: "Read position - where next element comes from"
    int count;                // Say: "Current number of elements in buffer"
} CircularBuffer;

// Say: "Initialize buffer to empty state"
void cb_init(CircularBuffer *cb) {
    cb->head = 0;   // Say: "Start writing at position 0"
    cb->tail = 0;   // Say: "Start reading at position 0"
    cb->count = 0;  // Say: "Buffer starts empty"
    // Say: "No need to clear buffer array - we track valid data with count"
}

// Say: "Check if buffer is full - cannot push more data"
bool cb_is_full(CircularBuffer *cb) {
    return cb->count == BUFFER_SIZE;
    // Say: "Using count makes this check simple and reliable"
}

// Say: "Check if buffer is empty - cannot pop data"
bool cb_is_empty(CircularBuffer *cb) {
    return cb->count == 0;
    // Say: "Using count avoids the head==tail ambiguity problem"
}

// Say: "Push (enqueue) - add element to buffer"
bool cb_push(CircularBuffer *cb, int data) {
    // Say: "Check for overflow before writing"
    if (cb_is_full(cb)) {
        return false;  // Say: "Buffer full - reject the push"
    }

    // Say: "Write data at head position"
    cb->buffer[cb->head] = data;

    // Say: "Advance head with wraparound using modulo"
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    // Say: "Modulo makes the buffer circular - wraps from end to beginning"

    cb->count++;  // Say: "Increment element count"
    return true;  // Say: "Success"
}

// Say: "Pop (dequeue) - remove and return oldest element"
bool cb_pop(CircularBuffer *cb, int *data) {
    // Say: "Check for underflow before reading"
    if (cb_is_empty(cb)) {
        return false;  // Say: "Buffer empty - nothing to pop"
    }

    // Say: "Read data from tail position (oldest element)"
    *data = cb->buffer[cb->tail];

    // Say: "Advance tail with wraparound"
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;

    cb->count--;  // Say: "Decrement element count"
    return true;  // Say: "Success"
}

// Say: "Peek - view front element without removing it"
bool cb_peek(CircularBuffer *cb, int *data) {
    if (cb_is_empty(cb)) {
        return false;
    }
    *data = cb->buffer[cb->tail];  // Say: "Just read, don't advance tail"
    return true;
}

// Say: "Helper to visualize buffer contents"
void cb_print(CircularBuffer *cb) {
    printf("Buffer [%d/%d]: ", cb->count, BUFFER_SIZE);
    // Say: "Start from tail, print count elements with wraparound"
    int idx = cb->tail;
    for (int i = 0; i < cb->count; i++) {
        printf("%d ", cb->buffer[idx]);
        idx = (idx + 1) % BUFFER_SIZE;  // Say: "Circular traversal"
    }
    printf("\n");
}

int main() {
    CircularBuffer cb;
    cb_init(&cb);  // Say: "Always initialize before use"

    // Say: "Demonstrate push operations"
    printf("Pushing 10, 20, 30...\n");
    cb_push(&cb, 10);
    cb_push(&cb, 20);
    cb_push(&cb, 30);
    cb_print(&cb);

    // Say: "Demonstrate pop operation"
    int val;
    cb_pop(&cb, &val);
    printf("Popped: %d\n", val);
    cb_print(&cb);

    // Say: "Demonstrate circular behavior"
    printf("Pushing 40, 50, 60...\n");
    cb_push(&cb, 40);
    cb_push(&cb, 50);
    cb_push(&cb, 60);
    cb_print(&cb);

    // Say: "Try to push when full"
    printf("Push 70 when full: %s\n", cb_push(&cb, 70) ? "success" : "failed");

    return 0;
}

/*
INTERVIEW EXPLANATION:
"A circular buffer is a fixed-size FIFO queue that wraps around.

 Key Components:
 1. buffer[] - fixed size array for data storage
 2. head - write pointer (where new data goes)
 3. tail - read pointer (where old data comes from)
 4. count - current number of elements

 Why use count instead of just head/tail?
 - Head == tail is ambiguous: empty or full?
 - Count eliminates this ambiguity
 - Alternative: waste one slot (full when head+1 == tail)

 The Circular Magic - Modulo Operation:
 - head = (head + 1) % SIZE
 - When head reaches SIZE, it wraps to 0
 - This creates the 'ring' behavior without any special cases

 Operations (all O(1)):
 - push: write at head, advance head
 - pop: read at tail, advance tail
 - is_full: count == SIZE
 - is_empty: count == 0

 Visual representation for SIZE=5:
 [0][1][2][3][4]
      T     H     (tail=1, head=3, count=2, elements at [1],[2])

 After push:
 [0][1][2][3][4]
      T        H  (tail=1, head=4, count=3)

 Real-world Embedded Uses:
 - UART receive/transmit buffers
 - Audio sample buffers (producer-consumer)
 - Sensor data logging
 - Command queues between ISR and main loop

 Thread-safety considerations:
 - For single producer/single consumer: often lock-free
 - For multiple producers/consumers: need synchronization
 - In embedded ISR context: disable interrupts or use atomics

 Overwrite vs Reject on Full:
 - This implementation rejects new data when full
 - Alternative: overwrite oldest data (useful for streaming)

 Power-of-2 Optimization:
 - If SIZE is power of 2, use: head & (SIZE-1) instead of modulo
 - Bitwise AND is faster than modulo on many processors"
*/
