/*
 * ============================================================================
 * PROBLEM: Implement Queue Using Array
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: Very High
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. QUEUE = FIFO (First In, First Out)
 *    - Like line at a counter
 *    - Enqueue: Add to rear
 *    - Dequeue: Remove from front
 *
 * 2. CIRCULAR QUEUE:
 *    - Avoids wasted space
 *    - Use modulo for wrap-around
 *
 * ============================================================================
 * CIRCULAR QUEUE VISUALIZATION:
 * ============================================================================
 *
 *       +---+---+---+---+---+
 *       | A | B | C |   |   |
 *       +---+---+---+---+---+
 *         ^           ^
 *       front       rear
 *
 * After dequeue (remove A):
 *       +---+---+---+---+---+
 *       |   | B | C |   |   |
 *       +---+---+---+---+---+
 *             ^       ^
 *           front   rear
 *
 * After enqueue D, E, F (wrap around):
 *       +---+---+---+---+---+
 *       | F | B | C | D | E |
 *       +---+---+---+---+---+
 *         ^   ^
 *       rear front
 *
 * ============================================================================
 * TIME: O(1) for all operations
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Define maximum queue size
// Say: "I'll define the maximum capacity for our queue"
#define MAX_SIZE 5

// Queue structure with array, front, rear, and count
// Say: "The queue struct has an array, front and rear pointers, and a count"
typedef struct {
    int arr[MAX_SIZE];  // Array to store queue elements
    int front;          // Index of front element
    int rear;           // Index of rear element
    int count;          // Number of elements in queue
} Queue;

// Initialize queue to empty state
// Say: "I'll create an initialization function for the queue"
void initQueue(Queue* q) {
    // Set front to beginning
    // Say: "I set front to 0, the start of the array"
    q->front = 0;

    // Set rear to -1 (no elements yet)
    // Say: "I set rear to -1 to indicate no elements have been added"
    q->rear = -1;

    // Set count to 0
    // Say: "I set count to 0 since the queue is empty"
    q->count = 0;
}

// Check if queue is empty
// Say: "Now I'll implement isEmpty by checking the count"
bool isEmpty(Queue* q) {
    // Queue is empty when count is 0
    // Say: "I return true if count equals 0"
    return q->count == 0;
}

// Check if queue is full
// Say: "I need to check if the queue is full before enqueueing"
bool isFull(Queue* q) {
    // Queue is full when count equals MAX_SIZE
    // Say: "I return true if count equals MAX_SIZE"
    return q->count == MAX_SIZE;
}

// Enqueue element at rear
// Say: "Now I'll implement enqueue to add an element to the rear"
bool enqueue(Queue* q, int value) {
    // Check if queue is full
    // Say: "First I check if the queue is full"
    if (isFull(q)) {
        // Print error message
        // Say: "If full, I print an error and return false"
        printf("Queue Overflow!\n");
        return false;
    }

    // Move rear to next position using circular wrap-around
    // Say: "I move rear forward using modulo to wrap around if needed"
    q->rear = (q->rear + 1) % MAX_SIZE;

    // Store value at rear position
    // Say: "Then I store the value at the new rear position"
    q->arr[q->rear] = value;

    // Increment count
    // Say: "I increment the count to track the new element"
    q->count++;

    // Return success
    // Say: "Finally, I return true to indicate success"
    return true;
}

// Dequeue element from front
// Say: "Now I'll implement dequeue to remove an element from the front"
int dequeue(Queue* q) {
    // Check if queue is empty
    // Say: "First I check if the queue is empty"
    if (isEmpty(q)) {
        // Print error message
        // Say: "If empty, I print an error and return -1"
        printf("Queue Underflow!\n");
        return -1;
    }

    // Get value at front
    // Say: "I save the value at the front position"
    int value = q->arr[q->front];

    // Move front to next position using circular wrap-around
    // Say: "I move front forward using modulo to wrap around"
    q->front = (q->front + 1) % MAX_SIZE;

    // Decrement count
    // Say: "I decrement the count since we removed an element"
    q->count--;

    // Return the dequeued value
    // Say: "Finally, I return the value that was at the front"
    return value;
}

// Peek at front element without removing
// Say: "I'll implement peek to view the front element"
int peek(Queue* q) {
    // Check if queue is empty
    // Say: "I check if the queue is empty"
    if (isEmpty(q)) {
        // Return -1 for empty queue
        // Say: "If empty, I return -1"
        return -1;
    }

    // Return value at front without modification
    // Say: "I return the front value without removing it"
    return q->arr[q->front];
}

int main() {
    // Say: "Let me demonstrate the circular queue implementation"
    printf("=== Queue Using Array (Circular) ===\n\n");

    // Declare queue variable
    // Say: "I'll declare a queue variable"
    Queue q;

    // Initialize the queue
    // Say: "Now I initialize the queue to empty state"
    initQueue(&q);

    // Enqueue some values
    // Say: "Let me enqueue three values: 10, 20, and 30"
    printf("Enqueue: 10, 20, 30\n");
    enqueue(&q, 10);  // Add 10
    enqueue(&q, 20);  // Add 20
    enqueue(&q, 30);  // Add 30

    // Display front element
    // Say: "I'll check what's at the front using peek"
    printf("Front: %d\n", peek(&q));

    // Dequeue one element
    // Say: "Now I'll dequeue one element"
    printf("\nDequeue: %d\n", dequeue(&q));

    // Show new front
    // Say: "Let me check the new front element"
    printf("Front now: %d\n", peek(&q));

    // Enqueue more values
    // Say: "I'll enqueue two more values: 40 and 50"
    printf("\nEnqueue: 40, 50\n");
    enqueue(&q, 40);  // Add 40
    enqueue(&q, 50);  // Add 50

    // Dequeue all elements
    // Say: "Finally, I'll dequeue all elements to show FIFO order"
    printf("\nDequeue all:\n");
    while (!isEmpty(&q)) {
        // Dequeue and print each element
        // Say: "I dequeue each element until the queue is empty"
        printf("Dequeued: %d\n", dequeue(&q));
    }

    return 0;
}
