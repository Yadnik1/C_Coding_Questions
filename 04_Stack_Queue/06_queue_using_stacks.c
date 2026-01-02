/*
 * ============================================================================
 * PROBLEM: Implement Queue Using Two Stacks
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: High
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. TWO STACKS:
 *    - Stack1 for enqueue
 *    - Stack2 for dequeue
 *
 * 2. TRANSFER WHEN NEEDED:
 *    - Move all from Stack1 to Stack2 (reverses order)
 *    - Then pop from Stack2
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * Enqueue 1, 2, 3:
 *
 *   Stack1:     Stack2:
 *   +---+       +---+
 *   | 3 |       |   |
 *   +---+       +---+
 *   | 2 |       |   |
 *   +---+       +---+
 *   | 1 |       |   |
 *   +---+       +---+
 *
 * Dequeue (need 1):
 *   Transfer Stack1 → Stack2
 *
 *   Stack1:     Stack2:
 *   +---+       +---+
 *   |   |       | 1 | ← pop this!
 *   +---+       +---+
 *   |   |       | 2 |
 *   +---+       +---+
 *   |   |       | 3 |
 *   +---+       +---+
 *
 *   Pop from Stack2 → returns 1 ✓
 *
 * ============================================================================
 * TIME: Amortized O(1) for dequeue
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100

typedef struct {
    int arr[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack* s) { s->top = -1; }
bool isEmpty(Stack* s) { return s->top == -1; }
void push(Stack* s, int val) { s->arr[++s->top] = val; }
int pop(Stack* s) { return s->arr[s->top--]; }
int peek(Stack* s) { return s->arr[s->top]; }

typedef struct {
    Stack s1;  // For enqueue
    Stack s2;  // For dequeue
} Queue;

void initQueue(Queue* q) {
    initStack(&q->s1);
    initStack(&q->s2);
}

void enqueue(Queue* q, int val) {
    push(&q->s1, val);
}

int dequeue(Queue* q) {
    // If s2 empty, transfer from s1
    if (isEmpty(&q->s2)) {
        while (!isEmpty(&q->s1)) {
            push(&q->s2, pop(&q->s1));
        }
    }

    if (isEmpty(&q->s2)) {
        printf("Queue empty!\n");
        return -1;
    }

    return pop(&q->s2);
}

int front(Queue* q) {
    if (isEmpty(&q->s2)) {
        while (!isEmpty(&q->s1)) {
            push(&q->s2, pop(&q->s1));
        }
    }
    return isEmpty(&q->s2) ? -1 : peek(&q->s2);
}

int main() {
    printf("=== Queue Using Two Stacks ===\n\n");

    Queue q;
    initQueue(&q);

    printf("Enqueue: 1, 2, 3\n");
    enqueue(&q, 1);
    enqueue(&q, 2);
    enqueue(&q, 3);

    printf("Dequeue: %d\n", dequeue(&q));
    printf("Dequeue: %d\n", dequeue(&q));

    printf("\nEnqueue: 4, 5\n");
    enqueue(&q, 4);
    enqueue(&q, 5);

    printf("Dequeue: %d\n", dequeue(&q));
    printf("Dequeue: %d\n", dequeue(&q));
    printf("Dequeue: %d\n", dequeue(&q));

    return 0;
}
