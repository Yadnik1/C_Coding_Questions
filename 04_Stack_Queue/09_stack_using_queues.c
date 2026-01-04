/*
 * ============================================================================
 * PROBLEM: Implement Stack using Two Queues
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: HIGH
 *
 * Implement stack (LIFO) using only queue operations (FIFO).
 *
 * ============================================================================
 * TWO APPROACHES:
 * ============================================================================
 *
 * 1. COSTLY PUSH: O(n) push, O(1) pop
 *    - Push: Enqueue to q2, transfer all from q1 to q2, swap q1/q2
 *    - Pop: Dequeue from q1
 *
 * 2. COSTLY POP: O(1) push, O(n) pop
 *    - Push: Enqueue to q1
 *    - Pop: Transfer n-1 elements to q2, dequeue last from q1, swap
 *
 * ============================================================================
 * VISUALIZATION (Costly Push):
 * ============================================================================
 *
 *   Push 1: q2=[1], q1=[], swap → q1=[1]
 *   Push 2: q2=[2], transfer → q2=[2,1], swap → q1=[2,1]
 *   Push 3: q2=[3], transfer → q2=[3,2,1], swap → q1=[3,2,1]
 *
 *   Pop: dequeue from q1 → returns 3
 *   q1=[2,1]
 *
 *   Pop: dequeue from q1 → returns 2
 *   q1=[1]
 *
 * ============================================================================
 * TIME: O(n) push or pop | SPACE: O(n)
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Define maximum queue size
// Say: "I'll define the maximum size for the queues"
#define MAX_SIZE 100

// Simple Queue implementation
// Say: "I create a simple queue struct for FIFO operations"
typedef struct {
    int data[MAX_SIZE];  // Array to hold queue elements
    int front;           // Front index
    int rear;            // Rear index
} Queue;

// Initialize queue
// Say: "This initializes a queue to empty"
void queueInit(Queue* q) {
    // Set front to 0
    // Say: "I set front to 0"
    q->front = 0;

    // Set rear to -1
    // Say: "I set rear to -1 to indicate empty"
    q->rear = -1;
}

// Check if queue is empty
// Say: "This checks if the queue is empty"
bool queueIsEmpty(Queue* q) {
    // Empty when rear is less than front
    // Say: "Queue is empty when rear is less than front"
    return q->rear < q->front;
}

// Get queue size
// Say: "This calculates the current size of the queue"
int queueSize(Queue* q) {
    // Check if empty
    // Say: "If empty, I return 0"
    if (queueIsEmpty(q)) {
        return 0;
    }

    // Return size
    // Say: "Otherwise I return rear minus front plus 1"
    return q->rear - q->front + 1;
}

// Enqueue element
// Say: "This adds an element to the rear of the queue"
void enqueue(Queue* q, int val) {
    // Increment rear and store
    // Say: "I increment rear and store the value there"
    q->data[++q->rear] = val;
}

// Dequeue element
// Say: "This removes and returns the front element"
int dequeue(Queue* q) {
    // Return front and increment
    // Say: "I return the front element and increment front"
    return q->data[q->front++];
}

// Peek at front
// Say: "This returns the front element without removing it"
int queueFront(Queue* q) {
    // Return front value
    // Say: "I return the value at front"
    return q->data[q->front];
}

// ============================================================================
// Stack using Two Queues (Costly Push)
// ============================================================================

// Stack structure with two queues
// Say: "Now I create a Stack struct that uses two queues"
typedef struct {
    Queue q1;  // Primary queue
    Queue q2;  // Temporary queue for transfers
} StackUsingQueues;

// Initialize stack
// Say: "This initializes the stack by initializing both queues"
void stackInit(StackUsingQueues* s) {
    // Initialize queue 1
    // Say: "I initialize queue 1"
    queueInit(&s->q1);

    // Initialize queue 2
    // Say: "I initialize queue 2"
    queueInit(&s->q2);
}

// Push with O(n) time
// Say: "Now I'll implement push, which has O(n) time complexity"
void stackPush(StackUsingQueues* s, int val) {
    // Enqueue to q2
    // Say: "First I enqueue the new value to q2"
    enqueue(&s->q2, val);

    // Transfer all from q1 to q2
    // Say: "Then I transfer all elements from q1 to q2"
    while (!queueIsEmpty(&s->q1)) {
        // Dequeue from q1 and enqueue to q2
        // Say: "I dequeue from q1 and enqueue to q2, one by one"
        enqueue(&s->q2, dequeue(&s->q1));
    }

    // Swap q1 and q2
    // Say: "Finally I swap q1 and q2 by swapping their contents"
    Queue temp = s->q1;
    s->q1 = s->q2;
    s->q2 = temp;
}

// Pop with O(1) time
// Say: "This pops an element, which is O(1) since we maintain LIFO order in q1"
int stackPop(StackUsingQueues* s) {
    // Check if empty
    // Say: "I check if q1 is empty"
    if (queueIsEmpty(&s->q1)) {
        // Print error
        // Say: "If empty, I print underflow and return -1"
        printf("Stack underflow!\n");
        return -1;
    }

    // Dequeue from q1
    // Say: "I simply dequeue from q1, which gives the top element"
    return dequeue(&s->q1);
}

// Get top element
// Say: "This returns the top element without removing it"
int stackTop(StackUsingQueues* s) {
    // Check if empty
    // Say: "I check if q1 is empty"
    if (queueIsEmpty(&s->q1)) {
        // Return -1
        // Say: "If empty, I return -1"
        return -1;
    }

    // Return front of q1
    // Say: "I return the front of q1, which is the top element"
    return queueFront(&s->q1);
}

// Check if stack is empty
// Say: "This checks if the stack is empty"
bool stackIsEmpty(StackUsingQueues* s) {
    // Check q1
    // Say: "I check if q1 is empty"
    return queueIsEmpty(&s->q1);
}

// ============================================================================
// Stack using ONE Queue (Costly Push)
// ============================================================================

// Stack structure with one queue
// Say: "I can also implement a stack with just one queue"
typedef struct {
    Queue q;  // Single queue
} StackOneQueue;

// Initialize one-queue stack
// Say: "This initializes the one-queue stack"
void stack1Init(StackOneQueue* s) {
    // Initialize queue
    // Say: "I initialize the single queue"
    queueInit(&s->q);
}

// Push with O(n) - rotate queue
// Say: "This push rotates the queue to maintain LIFO order"
void stack1Push(StackOneQueue* s, int val) {
    // Get current size
    // Say: "First I get the current size of the queue"
    int size = queueSize(&s->q);

    // Enqueue new value
    // Say: "I enqueue the new value"
    enqueue(&s->q, val);

    // Rotate: move all previous elements to back
    // Say: "Now I rotate by moving all previous elements to the back"
    for (int i = 0; i < size; i++) {
        // Dequeue and enqueue back
        // Say: "I dequeue from front and enqueue to rear, rotating the queue"
        enqueue(&s->q, dequeue(&s->q));
    }
}

// Pop with O(1)
// Say: "This pops an element in O(1) time"
int stack1Pop(StackOneQueue* s) {
    // Check if empty
    // Say: "I check if the queue is empty"
    if (queueIsEmpty(&s->q)) {
        // Print error
        // Say: "If empty, I print underflow and return -1"
        printf("Stack underflow!\n");
        return -1;
    }

    // Dequeue from front
    // Say: "I dequeue from the front, which is the top element"
    return dequeue(&s->q);
}

int main() {
    // Say: "Let me demonstrate stack using two queues"
    printf("=== Stack using Two Queues ===\n\n");

    // Declare two-queue stack
    // Say: "I declare a stack using two queues"
    StackUsingQueues s;

    // Initialize
    // Say: "I initialize the stack"
    stackInit(&s);

    // Push elements
    // Say: "Let me push 1, 2, and 3"
    printf("Push 1, 2, 3:\n");
    stackPush(&s, 1);  // Push 1
    stackPush(&s, 2);  // Push 2
    stackPush(&s, 3);  // Push 3

    // Show state
    // Say: "The stack now has 3, 2, 1 from top to bottom"
    printf("Stack state (top to bottom): 3, 2, 1\n\n");

    // Pop elements
    // Say: "Now I'll pop all elements"
    printf("Pop operations:\n");
    printf("  Pop: %d\n", stackPop(&s));  // Should be 3
    printf("  Pop: %d\n", stackPop(&s));  // Should be 2
    printf("  Pop: %d\n\n", stackPop(&s));  // Should be 1

    // One-queue stack demo
    // Say: "Now let me demonstrate stack using one queue"
    printf("=== Stack using ONE Queue ===\n\n");

    // Declare one-queue stack
    // Say: "I declare a stack using one queue"
    StackOneQueue s1;

    // Initialize
    // Say: "I initialize it"
    stack1Init(&s1);

    // Push elements
    // Say: "Let me push 10, 20, and 30"
    printf("Push 10, 20, 30:\n");
    stack1Push(&s1, 10);  // Push 10
    stack1Push(&s1, 20);  // Push 20
    stack1Push(&s1, 30);  // Push 30

    // Pop elements
    // Say: "Now I'll pop all elements"
    printf("Pop operations:\n");
    printf("  Pop: %d\n", stack1Pop(&s1));  // Should be 30
    printf("  Pop: %d\n", stack1Pop(&s1));  // Should be 20
    printf("  Pop: %d\n\n", stack1Pop(&s1));  // Should be 10

    // Summary
    // Say: "Here's a summary of the two approaches"
    printf("=== Summary ===\n");
    printf("Two Queues: Push O(n), Pop O(1)\n");
    printf("One Queue:  Push O(n), Pop O(1) (rotate after push)\n");

    return 0;
}
