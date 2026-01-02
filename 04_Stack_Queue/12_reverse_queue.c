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
 */

#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100

// Simple Queue
typedef struct {
    int data[MAX_SIZE];
    int front;
    int rear;
} Queue;

void queueInit(Queue* q) {
    q->front = 0;
    q->rear = -1;
}

bool queueIsEmpty(Queue* q) {
    return q->rear < q->front;
}

void enqueue(Queue* q, int val) {
    q->data[++q->rear] = val;
}

int dequeue(Queue* q) {
    return q->data[q->front++];
}

// Simple Stack
typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;

void stackInit(Stack* s) {
    s->top = -1;
}

bool stackIsEmpty(Stack* s) {
    return s->top == -1;
}

void push(Stack* s, int val) {
    s->data[++s->top] = val;
}

int pop(Stack* s) {
    return s->data[s->top--];
}

// Reverse queue using stack
void reverseQueue(Queue* q) {
    Stack s;
    stackInit(&s);

    // Step 1: Dequeue all and push to stack
    while (!queueIsEmpty(q)) {
        push(&s, dequeue(q));
    }

    // Reset queue pointers
    q->front = 0;
    q->rear = -1;

    // Step 2: Pop all and enqueue back
    while (!stackIsEmpty(&s)) {
        enqueue(q, pop(&s));
    }
}

// Reverse queue using recursion (no explicit stack)
void reverseQueueRecursive(Queue* q) {
    if (queueIsEmpty(q)) {
        return;
    }

    // Dequeue front element
    int front = dequeue(q);

    // Recursively reverse remaining queue
    reverseQueueRecursive(q);

    // Enqueue front element at rear
    enqueue(q, front);
}

void printQueue(Queue* q) {
    printf("Queue: ");
    if (queueIsEmpty(q)) {
        printf("(empty)\n");
        return;
    }

    for (int i = q->front; i <= q->rear; i++) {
        printf("%d ", q->data[i]);
    }
    printf("\n");
}

int main() {
    printf("=== Reverse Queue using Stack ===\n\n");

    Queue q;

    // Method 1: Using stack
    printf("Method 1: Using Stack\n");
    queueInit(&q);
    for (int i = 1; i <= 5; i++) {
        enqueue(&q, i);
    }

    printf("Before: ");
    printQueue(&q);

    reverseQueue(&q);

    printf("After:  ");
    printQueue(&q);
    printf("\n");

    // Method 2: Using recursion
    printf("Method 2: Using Recursion\n");
    queueInit(&q);
    for (int i = 10; i <= 50; i += 10) {
        enqueue(&q, i);
    }

    printf("Before: ");
    printQueue(&q);

    reverseQueueRecursive(&q);

    printf("After:  ");
    printQueue(&q);
    printf("\n");

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
