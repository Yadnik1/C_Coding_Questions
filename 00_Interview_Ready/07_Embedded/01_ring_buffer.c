// Ring Buffer / Circular Buffer - ESSENTIAL for embedded UART/SPI buffers
// Time: O(1) for all operations, Space: O(n)

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 8  // Say: "Power of 2 for efficient modulo"

// Say: "Circular buffer for byte data - common in UART/SPI"
typedef struct {
    uint8_t data[BUFFER_SIZE];
    volatile uint16_t head;   // Say: "Write position - volatile for ISR"
    volatile uint16_t tail;   // Say: "Read position"
    volatile uint16_t count;  // Say: "Number of elements"
} RingBuffer;

// Say: "Initialize buffer - reset all indices"
void ring_buffer_init(RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    memset(rb->data, 0, BUFFER_SIZE);
}

bool ring_buffer_is_empty(RingBuffer *rb) {
    return rb->count == 0;
}

bool ring_buffer_is_full(RingBuffer *rb) {
    return rb->count == BUFFER_SIZE;
}

uint16_t ring_buffer_count(RingBuffer *rb) {
    return rb->count;
}

// Say: "Put byte into buffer - called from ISR typically"
bool ring_buffer_put(RingBuffer *rb, uint8_t byte) {
    if (ring_buffer_is_full(rb)) {
        return false;  // Say: "Buffer overflow - data lost"
    }

    rb->data[rb->head] = byte;
    rb->head = (rb->head + 1) % BUFFER_SIZE;  // Say: "Circular wrap"
    rb->count++;
    return true;
}

// Say: "Get byte from buffer - called from main loop typically"
bool ring_buffer_get(RingBuffer *rb, uint8_t *byte) {
    if (ring_buffer_is_empty(rb)) {
        return false;  // Say: "Buffer empty"
    }

    *byte = rb->data[rb->tail];
    rb->tail = (rb->tail + 1) % BUFFER_SIZE;  // Say: "Circular wrap"
    rb->count--;
    return true;
}

// Say: "Peek at next byte without removing"
bool ring_buffer_peek(RingBuffer *rb, uint8_t *byte) {
    if (ring_buffer_is_empty(rb)) {
        return false;
    }
    *byte = rb->data[rb->tail];
    return true;
}

// Say: "Power of 2 optimization - use AND instead of modulo"
// (rb->head + 1) & (BUFFER_SIZE - 1) is faster than % BUFFER_SIZE

void print_buffer_state(RingBuffer *rb) {
    printf("Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%02X", rb->data[i]);
        if (i < BUFFER_SIZE - 1) printf(" ");
    }
    printf("] head=%d tail=%d count=%d\n", rb->head, rb->tail, rb->count);
}

int main() {
    RingBuffer uart_rx;
    ring_buffer_init(&uart_rx);

    printf("=== Ring Buffer Demo ===\n\n");

    // Simulate UART receive
    printf("Putting bytes: 0xAA, 0xBB, 0xCC, 0xDD\n");
    ring_buffer_put(&uart_rx, 0xAA);
    ring_buffer_put(&uart_rx, 0xBB);
    ring_buffer_put(&uart_rx, 0xCC);
    ring_buffer_put(&uart_rx, 0xDD);
    print_buffer_state(&uart_rx);

    // Read some bytes
    uint8_t byte;
    printf("\nGetting 2 bytes:\n");
    if (ring_buffer_get(&uart_rx, &byte)) printf("  Got: 0x%02X\n", byte);
    if (ring_buffer_get(&uart_rx, &byte)) printf("  Got: 0x%02X\n", byte);
    print_buffer_state(&uart_rx);

    // Add more (wrap around)
    printf("\nPutting 0xEE, 0xFF (wrap around):\n");
    ring_buffer_put(&uart_rx, 0xEE);
    ring_buffer_put(&uart_rx, 0xFF);
    print_buffer_state(&uart_rx);

    // Fill to capacity
    printf("\nFilling buffer to capacity:\n");
    ring_buffer_put(&uart_rx, 0x11);
    ring_buffer_put(&uart_rx, 0x22);
    ring_buffer_put(&uart_rx, 0x33);
    ring_buffer_put(&uart_rx, 0x44);
    print_buffer_state(&uart_rx);

    printf("Is full: %s\n", ring_buffer_is_full(&uart_rx) ? "yes" : "no");
    printf("Try put 0x55: %s\n", ring_buffer_put(&uart_rx, 0x55) ? "success" : "failed (full)");

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Ring buffer is a fixed-size FIFO queue using circular indexing.

 STRUCTURE:
 - Fixed-size array
 - Head pointer (write position)
 - Tail pointer (read position)
 - Count (optional, simplifies full/empty check)

 OPERATIONS:
 - Put: Write to head, advance head circularly
 - Get: Read from tail, advance tail circularly
 - All O(1) time

 CIRCULAR WRAP:
 - index = (index + 1) % BUFFER_SIZE
 - Optimization: if BUFFER_SIZE is power of 2:
   index = (index + 1) & (BUFFER_SIZE - 1)

 WHY VOLATILE:
 - ISR writes to buffer, main loop reads
 - volatile prevents compiler from caching values
 - Ensures changes are visible between contexts

 PRODUCER-CONSUMER PATTERN:
 - ISR = Producer (puts data in buffer)
 - Main loop = Consumer (gets data from buffer)
 - Buffer decouples timing between them

 FULL/EMPTY DETECTION:
 Option 1: Count variable (shown here)
 Option 2: Waste one slot - full when head+1 == tail
 Option 3: Bool flag

 EMBEDDED APPLICATIONS:
 1. UART RX/TX buffers
 2. SPI data buffering
 3. ADC sample buffers
 4. Audio streaming
 5. Sensor data logging
 6. Command queues

 RACE CONDITION CONCERNS:
 - Single producer, single consumer: usually safe
 - Multiple producers/consumers: need mutex/critical section
 - ISR context: disable interrupts during shared access

 COMMON MISTAKES:
 - Not using volatile
 - Wrong full/empty check
 - Not handling wrap-around correctly
 - Buffer size not power of 2 (slower modulo)"
*/
