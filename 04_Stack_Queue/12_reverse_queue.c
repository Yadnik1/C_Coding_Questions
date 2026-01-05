/*
 * ============================================================================
 * PROBLEM: Reverse Queue using Stack
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: MEDIUM
 *
 * Reverse the order of elements in a queue using a stack.
 *
 * ============================================================================
 * APPROACH:
 * ============================================================================
 *
 * 1. Dequeue all elements and push to stack
 * 2. Pop all elements from stack and enqueue back
 *
 * Queue is FIFO, Stack is LIFO → combining reverses order!
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   Queue: [1, 2, 3, 4, 5] (front=1, rear=5)
 *   Stack: []
 *
 *   Step 1: Move to stack
 *   Queue: []
 *   Stack: [1, 2, 3, 4, 5] (top=5)
 *
 *   Step 2: Move back to queue
 *   Queue: [5, 4, 3, 2, 1] (front=5, rear=1)
 *   Stack: []
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(n)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Can you reverse queue without stack?"
 * A1: Yes, using recursion! Dequeue element, reverse rest recursively,
 *     then enqueue the element. Uses O(n) call stack though.
 *
 * -------------------------------------------------------------------------
 * Q2: "Can you reverse queue in O(1) space?"
 * A2: Not with standard queue. But with deque (double-ended queue),
 *     swap front and rear pointers. O(1) operation!
 *
 * -------------------------------------------------------------------------
 * Q3: "What about reversing first K elements?"
 * A3: Push K elements to stack, pop back to queue,
 *     then move remaining n-K elements from front to rear.
 *
 * -------------------------------------------------------------------------
 * Q4: "Why does stack + queue reverse order?"
 * A4: Queue gives FIFO: 1,2,3 → push to stack in order 1,2,3.
 *     Stack gives LIFO: pop gives 3,2,1.
 *     Enqueue to queue: 3,2,1 - REVERSED!
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Define maximum size
// Say: "I'll define the maximum size for queue and stack"
#define MAX_SIZE 100

// Simple Queue
// Say: "I create a simple queue struct for FIFO operations"
typedef struct {
    int data[MAX_SIZE];  // Array to hold queue elements
    int front;           // Front index
    int rear;            // Rear index
} Queue;

// Initialize queue
// Say: "This initializes the queue to empty"
void queueInit(Queue* q) {
    // Set front to 0
    // Say: "I set front to 0"
    q->front = 0;

    // Set rear to -1 for empty
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

// Simple Stack
// Say: "I create a simple stack struct for LIFO operations"
typedef struct {
    int data[MAX_SIZE];  // Array to hold stack elements
    int top;             // Top index
} Stack;

// Initialize stack
// Say: "This initializes the stack to empty"
void stackInit(Stack* s) {
    // Set top to -1
    // Say: "I set top to -1 for empty stack"
    s->top = -1;
}

// Check if stack is empty
// Say: "This checks if the stack is empty"
bool stackIsEmpty(Stack* s) {
    // Empty when top is -1
    // Say: "Stack is empty when top is -1"
    return s->top == -1;
}

// Push element
// Say: "This pushes an element onto the stack"
void push(Stack* s, int val) {
    // Increment top and store
    // Say: "I increment top and store the value"
    s->data[++s->top] = val;
}

// Pop element
// Say: "This pops and returns the top element"
int pop(Stack* s) {
    // Return and decrement
    // Say: "I return the top value and decrement top"
    return s->data[s->top--];
}

// Reverse queue using stack
// Say: "Now I'll implement the main function to reverse a queue using a stack"
void reverseQueue(Queue* q) {
    // Declare stack
    // Say: "I declare a stack to temporarily hold the elements"
    Stack s;

    // Initialize stack
    // Say: "I initialize the stack to empty"
    stackInit(&s);

    // Step 1: Dequeue all and push to stack
    // Say: "First, I'll dequeue all elements and push them onto the stack"
    while (!queueIsEmpty(q)) {
        // Dequeue and push
        // Say: "I dequeue from the queue and push to the stack"
        push(&s, dequeue(q));
    }

    // Reset queue pointers
    // Say: "I reset the queue's front and rear pointers"
    q->front = 0;
    q->rear = -1;

    // Step 2: Pop all and enqueue back
    // Say: "Now I pop all elements from the stack and enqueue them back"
    while (!stackIsEmpty(&s)) {
        // Pop and enqueue
        // Say: "I pop from the stack and enqueue to the queue"
        enqueue(q, pop(&s));
    }
}

// Reverse queue using recursion (no explicit stack)
// Say: "Here's an alternative using recursion, which uses the call stack"
void reverseQueueRecursive(Queue* q) {
    // Base case: if queue is empty
    // Say: "If the queue is empty, I return as the base case"
    if (queueIsEmpty(q)) {
        return;
    }

    // Dequeue front element
    // Say: "I dequeue the front element"
    int front = dequeue(q);

    // Recursively reverse remaining queue
    // Say: "Then I recursively reverse the rest of the queue"
    reverseQueueRecursive(q);

    // Enqueue front element at rear
    // Say: "Finally, I enqueue the front element at the end"
    enqueue(q, front);
}

// Print queue contents
// Say: "This helper function prints the queue"
void printQueue(Queue* q) {
    // Print header
    // Say: "I print a queue header"
    printf("Queue: ");

    // Check if empty
    // Say: "If empty, I print empty and return"
    if (queueIsEmpty(q)) {
        printf("(empty)\n");
        return;
    }

    // Print all elements
    // Say: "I print all elements from front to rear"
    for (int i = q->front; i <= q->rear; i++) {
        // Print element
        // Say: "I print each element"
        printf("%d ", q->data[i]);
    }
    printf("\n");
}

int main() {
    // Say: "Let me demonstrate reversing a queue using a stack"
    printf("=== Reverse Queue using Stack ===\n\n");

    // Declare queue
    Queue q;

    // Method 1: Using stack
    // Say: "First, I'll demonstrate the stack-based approach"
    printf("Method 1: Using Stack\n");

    // Initialize queue
    // Say: "I initialize the queue"
    queueInit(&q);

    // Enqueue elements
    // Say: "Let me enqueue 1 through 5"
    for (int i = 1; i <= 5; i++) {
        // Enqueue i
        // Say: "I enqueue each number"
        enqueue(&q, i);
    }

    // Print before
    // Say: "Here's the queue before reversing"
    printf("Before: ");
    printQueue(&q);

    // Reverse queue
    // Say: "Now I reverse the queue using a stack"
    reverseQueue(&q);

    // Print after
    // Say: "And here's the queue after reversing"
    printf("After:  ");
    printQueue(&q);
    printf("\n");

    // Method 2: Using recursion
    // Say: "Now let me demonstrate the recursive approach"
    printf("Method 2: Using Recursion\n");

    // Initialize queue
    // Say: "I initialize a new queue"
    queueInit(&q);

    // Enqueue elements
    // Say: "I enqueue 10, 20, 30, 40, 50"
    for (int i = 10; i <= 50; i += 10) {
        // Enqueue i
        // Say: "I enqueue each value"
        enqueue(&q, i);
    }

    // Print before
    // Say: "Here's the queue before reversing"
    printf("Before: ");
    printQueue(&q);

    // Reverse using recursion
    // Say: "Now I reverse using the recursive approach"
    reverseQueueRecursive(&q);

    // Print after
    // Say: "And here's the reversed queue"
    printf("After:  ");
    printQueue(&q);
    printf("\n");

    // Print algorithm summary
    // Say: "Here's a summary of both algorithms"
    printf("=== Algorithm ===\n");
    printf("Using Stack:\n");
    printf("  1. Dequeue all → Push to stack\n");
    printf("  2. Pop all → Enqueue back\n\n");
    printf("Using Recursion:\n");
    printf("  1. Dequeue front\n");
    printf("  2. Recursively reverse rest\n");
    printf("  3. Enqueue front at end\n");

    return 0;
}
