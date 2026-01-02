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

#define MAX_SIZE 100

// Simple Queue implementation
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

int queueSize(Queue* q) {
    if (queueIsEmpty(q)) return 0;
    return q->rear - q->front + 1;
}

void enqueue(Queue* q, int val) {
    q->data[++q->rear] = val;
}

int dequeue(Queue* q) {
    return q->data[q->front++];
}

int queueFront(Queue* q) {
    return q->data[q->front];
}

// ============================================================================
// Stack using Two Queues (Costly Push)
// ============================================================================

typedef struct {
    Queue q1;
    Queue q2;
} StackUsingQueues;

void stackInit(StackUsingQueues* s) {
    queueInit(&s->q1);
    queueInit(&s->q2);
}

// O(n) push
void stackPush(StackUsingQueues* s, int val) {
    // Enqueue to q2
    enqueue(&s->q2, val);

    // Transfer all from q1 to q2
    while (!queueIsEmpty(&s->q1)) {
        enqueue(&s->q2, dequeue(&s->q1));
    }

    // Swap q1 and q2
    Queue temp = s->q1;
    s->q1 = s->q2;
    s->q2 = temp;
}

// O(1) pop
int stackPop(StackUsingQueues* s) {
    if (queueIsEmpty(&s->q1)) {
        printf("Stack underflow!\n");
        return -1;
    }
    return dequeue(&s->q1);
}

int stackTop(StackUsingQueues* s) {
    if (queueIsEmpty(&s->q1)) {
        return -1;
    }
    return queueFront(&s->q1);
}

bool stackIsEmpty(StackUsingQueues* s) {
    return queueIsEmpty(&s->q1);
}

// ============================================================================
// Stack using ONE Queue (Costly Push)
// ============================================================================

typedef struct {
    Queue q;
} StackOneQueue;

void stack1Init(StackOneQueue* s) {
    queueInit(&s->q);
}

// O(n) push - rotate queue to put new element at front
void stack1Push(StackOneQueue* s, int val) {
    int size = queueSize(&s->q);
    enqueue(&s->q, val);

    // Rotate: move all previous elements to back
    for (int i = 0; i < size; i++) {
        enqueue(&s->q, dequeue(&s->q));
    }
}

// O(1) pop
int stack1Pop(StackOneQueue* s) {
    if (queueIsEmpty(&s->q)) {
        printf("Stack underflow!\n");
        return -1;
    }
    return dequeue(&s->q);
}

int main() {
    printf("=== Stack using Two Queues ===\n\n");

    StackUsingQueues s;
    stackInit(&s);

    printf("Push 1, 2, 3:\n");
    stackPush(&s, 1);
    stackPush(&s, 2);
    stackPush(&s, 3);
    printf("Stack state (top to bottom): 3, 2, 1\n\n");

    printf("Pop operations:\n");
    printf("  Pop: %d\n", stackPop(&s));
    printf("  Pop: %d\n", stackPop(&s));
    printf("  Pop: %d\n\n", stackPop(&s));

    printf("=== Stack using ONE Queue ===\n\n");

    StackOneQueue s1;
    stack1Init(&s1);

    printf("Push 10, 20, 30:\n");
    stack1Push(&s1, 10);
    stack1Push(&s1, 20);
    stack1Push(&s1, 30);

    printf("Pop operations:\n");
    printf("  Pop: %d\n", stack1Pop(&s1));
    printf("  Pop: %d\n", stack1Pop(&s1));
    printf("  Pop: %d\n\n", stack1Pop(&s1));

    printf("=== Summary ===\n");
    printf("Two Queues: Push O(n), Pop O(1)\n");
    printf("One Queue:  Push O(n), Pop O(1) (rotate after push)\n");

    return 0;
}
