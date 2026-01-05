/*
 * ============================================================================
 * PROBLEM: Circular Queue Implementation
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH (Embedded)
 *
 * Implement a circular queue (ring buffer) with fixed size.
 *
 * ============================================================================
 * WHY CIRCULAR QUEUE?
 * ============================================================================
 *
 * Linear queue wastes space:
 *   After dequeue operations, front moves right, space on left is wasted.
 *
 * Circular queue reuses space:
 *   When rear reaches end, it wraps around to beginning.
 *
 * ============================================================================
 * KEY FORMULAS:
 * ============================================================================
 *
 *   rear = (rear + 1) % SIZE      // Move rear forward
 *   front = (front + 1) % SIZE    // Move front forward
 *   isEmpty = (front == rear)     // When pointing to same position
 *   isFull = ((rear + 1) % SIZE == front)  // Keep one slot empty
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   Circular view (size = 5):
 *
 *        [0]
 *       /   \
 *     [4]   [1]
 *       \   /
 *     [3]─[2]
 *
 *   Initial: front=0, rear=0 (empty)
 *
 *   Enqueue 1,2,3:
 *        [0:1]
 *       /     \
 *     [4]     [1:2]
 *       \     /
 *     [3]─[2:3]
 *     front=0, rear=3
 *
 *   Dequeue (returns 1):
 *        [0:_]
 *       /     \
 *     [4]     [1:2]
 *       \     /
 *     [3]─[2:3]
 *     front=1, rear=3
 *
 * ============================================================================
 * TIME: O(1) all operations | SPACE: O(n)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why keep one slot empty?"
 * A1: To distinguish full from empty! If we use all slots, front==rear could
 *     mean empty OR full. By wasting one slot, ((rear+1)%SIZE==front) means
 *     full, and (front==rear) means empty.
 *
 * -------------------------------------------------------------------------
 * Q2: "Can we use all slots without wasting one?"
 * A2: Yes! Add a 'count' variable or 'isFull' flag. But this adds complexity
 *     and extra storage. The wasted slot approach is simpler and common.
 *
 * -------------------------------------------------------------------------
 * Q3: "Why is circular queue important in embedded systems?"
 * A3: Ring buffers are EVERYWHERE in embedded: UART receive buffers, audio
 *     streaming, sensor data logging, producer-consumer patterns. Fixed size,
 *     no dynamic allocation, O(1) operations - perfect for real-time!
 *
 * -------------------------------------------------------------------------
 * Q4: "How would you make this thread-safe?"
 * A4: For single producer/single consumer: atomic reads/writes often enough.
 *     For multiple producers/consumers: use mutex or lock-free techniques
 *     with memory barriers. Volatile keyword alone is NOT sufficient!
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Define queue size
// Say: "I'll define the size for our circular queue"
#define QUEUE_SIZE 5

// Circular queue structure
// Say: "I create a circular queue struct with front, rear, and data array"
typedef struct {
    int data[QUEUE_SIZE];  // Array to store queue elements
    int front;             // Front index
    int rear;              // Rear index
} CircularQueue;

// Initialize queue
// Say: "This initializes the circular queue to empty"
void queueInit(CircularQueue* q) {
    // Set front to 0
    // Say: "I set front to 0"
    q->front = 0;

    // Set rear to 0 for empty
    // Say: "I set rear to 0, same as front, to indicate empty"
    q->rear = 0;
}

// Check if queue is empty
// Say: "This checks if the queue is empty"
bool queueIsEmpty(CircularQueue* q) {
    // Empty when front equals rear
    // Say: "Queue is empty when front equals rear"
    return q->front == q->rear;
}

// Check if queue is full
// Say: "This checks if the queue is full"
bool queueIsFull(CircularQueue* q) {
    // Full when next position of rear equals front
    // Say: "Queue is full when rear plus 1 modulo SIZE equals front"
    return ((q->rear + 1) % QUEUE_SIZE) == q->front;
}

// Get queue size
// Say: "This calculates the current number of elements"
int queueSize(CircularQueue* q) {
    // Calculate size using modulo arithmetic
    // Say: "I calculate size as rear minus front plus SIZE, all modulo SIZE"
    return (q->rear - q->front + QUEUE_SIZE) % QUEUE_SIZE;
}

// Enqueue element
// Say: "Now I'll implement enqueue to add an element to the rear"
bool enqueue(CircularQueue* q, int val) {
    // Check if queue is full
    // Say: "First I check if the queue is full"
    if (queueIsFull(q)) {
        // Print error
        // Say: "If full, I print an error and return false"
        printf("Queue is full!\n");
        return false;
    }

    // Store value at rear
    // Say: "I store the value at the current rear position"
    q->data[q->rear] = val;

    // Move rear forward with wrap-around
    // Say: "Then I move rear forward using modulo for circular wrap"
    q->rear = (q->rear + 1) % QUEUE_SIZE;

    // Return success
    // Say: "I return true to indicate success"
    return true;
}

// Dequeue element
// Say: "This removes and returns the front element"
bool dequeue(CircularQueue* q, int* val) {
    // Check if queue is empty
    // Say: "First I check if the queue is empty"
    if (queueIsEmpty(q)) {
        // Print error
        // Say: "If empty, I print an error and return false"
        printf("Queue is empty!\n");
        return false;
    }

    // Get value at front
    // Say: "I get the value at the front position"
    *val = q->data[q->front];

    // Move front forward with wrap-around
    // Say: "Then I move front forward using modulo"
    q->front = (q->front + 1) % QUEUE_SIZE;

    // Return success
    // Say: "I return true to indicate success"
    return true;
}

// Peek at front element
// Say: "This returns the front element without removing it"
bool queuePeek(CircularQueue* q, int* val) {
    // Check if empty
    // Say: "I check if the queue is empty"
    if (queueIsEmpty(q)) {
        // Return false
        // Say: "If empty, I return false"
        return false;
    }

    // Get front value
    // Say: "I get the value at front and return true"
    *val = q->data[q->front];
    return true;
}

// Print queue contents
// Say: "This helper function prints the queue contents"
void printQueue(CircularQueue* q) {
    // Print queue info
    // Say: "I print the front, rear, and size information"
    printf("Queue [front=%d, rear=%d, size=%d]: ",
           q->front, q->rear, queueSize(q));

    // Check if empty
    // Say: "If the queue is empty, I print empty and return"
    if (queueIsEmpty(q)) {
        printf("(empty)\n");
        return;
    }

    // Print all elements from front to rear
    // Say: "I iterate from front to rear, wrapping around with modulo"
    int i = q->front;
    while (i != q->rear) {
        // Print element
        // Say: "I print each element"
        printf("%d ", q->data[i]);

        // Move to next with wrap-around
        // Say: "And move to the next position using modulo"
        i = (i + 1) % QUEUE_SIZE;
    }
    printf("\n");
}

// Visualize circular structure
// Say: "This provides a visual representation of the circular nature"
void visualizeQueue(CircularQueue* q) {
    // Print header
    // Say: "I print a header showing the circular view"
    printf("\n  Circular View (size=%d):\n", QUEUE_SIZE);

    // Simple visualization showing front and rear
    // Say: "I show the current front and rear positions"
    printf("    front=%d, rear=%d\n\n", q->front, q->rear);
}

int main() {
    // Say: "Let me demonstrate the circular queue implementation"
    printf("=== Circular Queue Implementation ===\n\n");

    // Declare queue
    // Say: "I declare a circular queue variable"
    CircularQueue q;

    // Initialize
    // Say: "I initialize the queue to empty"
    queueInit(&q);

    // Print initial state
    // Say: "Here's the initial empty state"
    printf("Initial state:\n");
    printQueue(&q);
    printf("\n");

    // Enqueue operations
    // Say: "Let me enqueue four elements: 1, 2, 3, and 4"
    printf("Enqueue 1, 2, 3, 4:\n");
    enqueue(&q, 1);  // Enqueue 1
    enqueue(&q, 2);  // Enqueue 2
    enqueue(&q, 3);  // Enqueue 3
    enqueue(&q, 4);  // Enqueue 4
    printQueue(&q);
    printf("\n");

    // Try to enqueue when full
    // Say: "Now I'll try to enqueue when the queue is full"
    printf("Enqueue 5 (should fail - full):\n");
    enqueue(&q, 5);  // Should fail
    printf("\n");

    // Dequeue operations
    // Say: "Let me dequeue two elements"
    printf("Dequeue 2 elements:\n");
    int val;
    dequeue(&q, &val);  // Dequeue first
    printf("  Dequeued: %d\n", val);
    dequeue(&q, &val);  // Dequeue second
    printf("  Dequeued: %d\n", val);
    printQueue(&q);
    printf("\n");

    // Enqueue more (wrap around)
    // Say: "Now I'll enqueue two more, which will wrap around"
    printf("Enqueue 5, 6 (wraps around):\n");
    enqueue(&q, 5);  // Enqueue 5
    enqueue(&q, 6);  // Enqueue 6
    printQueue(&q);
    printf("\n");

    // Dequeue all
    // Say: "Finally, I'll dequeue all remaining elements"
    printf("Dequeue all:\n");
    while (!queueIsEmpty(&q)) {
        // Dequeue and print
        // Say: "I dequeue each element until the queue is empty"
        dequeue(&q, &val);
        printf("  Dequeued: %d\n", val);
    }
    printQueue(&q);

    // Print key formulas
    // Say: "Here are the key formulas for circular queues"
    printf("\n=== Key Formulas ===\n");
    printf("rear = (rear + 1) %% SIZE\n");
    printf("front = (front + 1) %% SIZE\n");
    printf("isEmpty = (front == rear)\n");
    printf("isFull = ((rear + 1) %% SIZE == front)\n");
    printf("size = (rear - front + SIZE) %% SIZE\n");

    return 0;
}
