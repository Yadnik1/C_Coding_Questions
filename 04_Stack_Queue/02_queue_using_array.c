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

#define MAX_SIZE 5

typedef struct {
    int arr[MAX_SIZE];
    int front;
    int rear;
    int count;
} Queue;

void initQueue(Queue* q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

bool isEmpty(Queue* q) {
    return q->count == 0;
}

bool isFull(Queue* q) {
    return q->count == MAX_SIZE;
}

bool enqueue(Queue* q, int value) {
    if (isFull(q)) {
        printf("Queue Overflow!\n");
        return false;
    }
    q->rear = (q->rear + 1) % MAX_SIZE;  // Circular wrap
    q->arr[q->rear] = value;
    q->count++;
    return true;
}

int dequeue(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue Underflow!\n");
        return -1;
    }
    int value = q->arr[q->front];
    q->front = (q->front + 1) % MAX_SIZE;  // Circular wrap
    q->count--;
    return value;
}

int peek(Queue* q) {
    if (isEmpty(q)) return -1;
    return q->arr[q->front];
}

int main() {
    printf("=== Queue Using Array (Circular) ===\n\n");

    Queue q;
    initQueue(&q);

    printf("Enqueue: 10, 20, 30\n");
    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);

    printf("Front: %d\n", peek(&q));

    printf("\nDequeue: %d\n", dequeue(&q));
    printf("Front now: %d\n", peek(&q));

    printf("\nEnqueue: 40, 50\n");
    enqueue(&q, 40);
    enqueue(&q, 50);

    printf("\nDequeue all:\n");
    while (!isEmpty(&q)) {
        printf("Dequeued: %d\n", dequeue(&q));
    }

    return 0;
}
