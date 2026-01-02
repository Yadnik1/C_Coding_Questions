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
 */

#include <stdio.h>
#include <stdbool.h>

#define QUEUE_SIZE 5

typedef struct {
    int data[QUEUE_SIZE];
    int front;
    int rear;
} CircularQueue;

void queueInit(CircularQueue* q) {
    q->front = 0;
    q->rear = 0;
}

bool queueIsEmpty(CircularQueue* q) {
    return q->front == q->rear;
}

bool queueIsFull(CircularQueue* q) {
    return ((q->rear + 1) % QUEUE_SIZE) == q->front;
}

int queueSize(CircularQueue* q) {
    return (q->rear - q->front + QUEUE_SIZE) % QUEUE_SIZE;
}

bool enqueue(CircularQueue* q, int val) {
    if (queueIsFull(q)) {
        printf("Queue is full!\n");
        return false;
    }

    q->data[q->rear] = val;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    return true;
}

bool dequeue(CircularQueue* q, int* val) {
    if (queueIsEmpty(q)) {
        printf("Queue is empty!\n");
        return false;
    }

    *val = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    return true;
}

bool queuePeek(CircularQueue* q, int* val) {
    if (queueIsEmpty(q)) {
        return false;
    }
    *val = q->data[q->front];
    return true;
}

void printQueue(CircularQueue* q) {
    printf("Queue [front=%d, rear=%d, size=%d]: ",
           q->front, q->rear, queueSize(q));

    if (queueIsEmpty(q)) {
        printf("(empty)\n");
        return;
    }

    int i = q->front;
    while (i != q->rear) {
        printf("%d ", q->data[i]);
        i = (i + 1) % QUEUE_SIZE;
    }
    printf("\n");
}

// Visualize circular structure
void visualizeQueue(CircularQueue* q) {
    printf("\n  Circular View (size=%d):\n", QUEUE_SIZE);
    printf("    [0]:%s\n", q->front <= 0 && q->rear > 0 ? "X" :
                          (0 >= q->front && 0 < q->rear) ||
                          (q->rear < q->front && (0 >= q->front || 0 < q->rear)) ?
                          (char[3]){q->data[0]+'0', 0} : "_");

    for (int i = 0; i < QUEUE_SIZE; i++) {
        bool hasData = false;
        if (q->front < q->rear) {
            hasData = (i >= q->front && i < q->rear);
        } else if (q->front > q->rear) {
            hasData = (i >= q->front || i < q->rear);
        }
        // Not printing full visualization for brevity
    }
    printf("    front=%d, rear=%d\n\n", q->front, q->rear);
}

int main() {
    printf("=== Circular Queue Implementation ===\n\n");

    CircularQueue q;
    queueInit(&q);

    printf("Initial state:\n");
    printQueue(&q);
    printf("\n");

    // Enqueue operations
    printf("Enqueue 1, 2, 3, 4:\n");
    enqueue(&q, 1);
    enqueue(&q, 2);
    enqueue(&q, 3);
    enqueue(&q, 4);
    printQueue(&q);
    printf("\n");

    // Try to enqueue when full (capacity is SIZE-1 = 4)
    printf("Enqueue 5 (should fail - full):\n");
    enqueue(&q, 5);
    printf("\n");

    // Dequeue operations
    printf("Dequeue 2 elements:\n");
    int val;
    dequeue(&q, &val);
    printf("  Dequeued: %d\n", val);
    dequeue(&q, &val);
    printf("  Dequeued: %d\n", val);
    printQueue(&q);
    printf("\n");

    // Enqueue more (wrap around)
    printf("Enqueue 5, 6 (wraps around):\n");
    enqueue(&q, 5);
    enqueue(&q, 6);
    printQueue(&q);
    printf("\n");

    // Dequeue all
    printf("Dequeue all:\n");
    while (!queueIsEmpty(&q)) {
        dequeue(&q, &val);
        printf("  Dequeued: %d\n", val);
    }
    printQueue(&q);

    printf("\n=== Key Formulas ===\n");
    printf("rear = (rear + 1) %% SIZE\n");
    printf("front = (front + 1) %% SIZE\n");
    printf("isEmpty = (front == rear)\n");
    printf("isFull = ((rear + 1) %% SIZE == front)\n");
    printf("size = (rear - front + SIZE) %% SIZE\n");

    return 0;
}
