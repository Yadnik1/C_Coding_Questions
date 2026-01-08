/*
 * ============================================================================
 * PROBLEM: Implement Queue using Circular Array
 * ============================================================================
 *
 * WHAT IS A QUEUE?
 * A Queue is a linear data structure that follows the FIFO (First In, First Out)
 * principle. Think of it like a line at a store - the first person to join the
 * line is the first person to be served. Elements are added at the rear (back)
 * and removed from the front.
 *
 * Key operations:
 * - enqueue(x): Add element x to the rear
 * - dequeue(): Remove and return the front element
 * - peek()/front(): Return the front element without removing it
 * - isEmpty(): Check if queue is empty
 * - size(): Return number of elements
 *
 * WHAT IS THIS PROBLEM?
 * Design and implement a queue using a circular array. A circular array wraps
 * around when indices reach the end, allowing efficient reuse of space after
 * dequeue operations. Without circular implementation, dequeuing wastes space
 * at the beginning of the array.
 *
 * EXAMPLES:
 * - enqueue(10), enqueue(20), enqueue(30) -> Queue: [10, 20, 30] (10 is front, 30 is rear)
 * - dequeue() -> Returns 10, Queue: [20, 30]
 * - enqueue(40) -> Queue: [20, 30, 40]
 * - peek() -> Returns 20, Queue unchanged
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Foundation for BFS traversal, task scheduling, buffering
 * - Critical in embedded systems (UART buffers, DMA, ISR communication)
 * - Tests understanding of circular/ring buffer concept (modulo arithmetic)
 * - Common in RTOS for ready queues and message passing
 * - Shows ability to optimize space utilization
 *
 * KEY CONCEPT:
 * Use modulo operation to wrap array indices:
 * - next_index = (current_index + 1) % MAX_SIZE
 * - Use a 'count' variable to distinguish full vs empty (both have front == rear)
 *
 * VISUAL:
 *
 *   CIRCULAR ARRAY CONCEPT:
 *
 *   Linear view:   [_, _, _, _, _]  (5 slots, indices 0-4)
 *                   ^front    ^rear
 *
 *   Circular view:
 *                    0
 *                  /   \
 *                4       1
 *                |       |
 *                3 ----- 2
 *
 *   After enqueue(10, 20, 30):
 *
 *   [10, 20, 30, _, _]
 *    ^front      ^rear
 *
 *   After dequeue() twice:
 *
 *   [_, _, 30, _, _]
 *          ^front/rear
 *
 *   After enqueue(40, 50, 60) - WRAP AROUND:
 *
 *   [60, _, 30, 40, 50]
 *       ^   ^front
 *       rear (wrapped to index 0)
 *
 * ============================================================================
 */

// Implement Queue using Circular Array - ESSENTIAL data structure
// Time: O(1) for enqueue/dequeue, Space: O(n)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 5

// Say: "Queue is FIFO - First In First Out"
typedef struct {
    int data[MAX_SIZE];
    int front;  // Say: "Index of first element"
    int rear;   // Say: "Index of last element"
    int count;  // Say: "Number of elements (simplifies full/empty check)"
} Queue;

// Say: "Initialize queue - front and rear at 0, count is 0"
void queue_init(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

bool is_empty(Queue *q) {
    return q->count == 0;
}

bool is_full(Queue *q) {
    return q->count == MAX_SIZE;
}

// Say: "Enqueue - add to rear"
bool enqueue(Queue *q, int value) {
    if (is_full(q)) {
        printf("Queue Overflow!\n");
        return false;
    }
    // Say: "Circular increment: wrap around using modulo"
    q->rear = (q->rear + 1) % MAX_SIZE;
    q->data[q->rear] = value;
    q->count++;
    return true;
}

// Say: "Dequeue - remove from front"
int dequeue(Queue *q) {
    if (is_empty(q)) {
        printf("Queue Underflow!\n");
        return -1;
    }
    int value = q->data[q->front];
    // Say: "Circular increment for front"
    q->front = (q->front + 1) % MAX_SIZE;
    q->count--;
    return value;
}

// Say: "Peek front element without removing"
int peek_front(Queue *q) {
    if (is_empty(q)) {
        printf("Queue is empty!\n");
        return -1;
    }
    return q->data[q->front];
}

int size(Queue *q) {
    return q->count;
}

// Say: "Print queue from front to rear"
void print_queue(Queue *q) {
    printf("Queue: [");
    for (int i = 0; i < q->count; i++) {
        int idx = (q->front + i) % MAX_SIZE;
        printf("%d", q->data[idx]);
        if (i < q->count - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    Queue q;
    queue_init(&q);

    printf("Enqueue: 10, 20, 30, 40, 50\n");
    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);
    enqueue(&q, 40);
    enqueue(&q, 50);
    print_queue(&q);

    printf("\nAttempt enqueue 60 (should overflow):\n");
    enqueue(&q, 60);

    printf("\nDequeue: %d\n", dequeue(&q));
    printf("Dequeue: %d\n", dequeue(&q));
    print_queue(&q);

    printf("\nEnqueue 60, 70 (circular wrap):\n");
    enqueue(&q, 60);
    enqueue(&q, 70);
    print_queue(&q);

    printf("\nFront element: %d\n", peek_front(&q));
    printf("Queue size: %d\n", size(&q));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Queue is FIFO - First In, First Out. Like a line at a store.

 OPERATIONS:
 - enqueue(x): Add to rear - O(1)
 - dequeue(): Remove from front - O(1)
 - peek(): Return front without removing - O(1)
 - isEmpty(): Check if empty - O(1)
 - size(): Number of elements - O(1)

 WHY CIRCULAR QUEUE:
 - Simple array wastes space after dequeues
 - Circular uses modulo to wrap around
 - Formula: next_index = (index + 1) % MAX_SIZE

 FULL vs EMPTY DETECTION:
 Option 1 (shown): Use count variable
 - Simple: full when count == MAX_SIZE
 - Empty when count == 0

 Option 2: Waste one slot
 - Full when (rear + 1) % MAX_SIZE == front
 - Empty when front == rear
 - Wastes one array slot

 COMMON MISTAKES:
 - Forgetting circular wrap (modulo)
 - Confusing full and empty conditions
 - Off-by-one errors in rear initialization

 QUEUE APPLICATIONS:
 1. BFS traversal
 2. Task scheduling (RTOS ready queue)
 3. Interrupt handling (ISR to task communication)
 4. Buffering (UART RX/TX buffers)
 5. Print spooler

 EMBEDDED CONSIDERATIONS:
 - Circular buffers are memory efficient
 - Often used for DMA transfers
 - Producer-consumer pattern with ISRs
 - Watch for race conditions (ISR vs main loop)"
*/
