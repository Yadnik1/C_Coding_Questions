/*
 * ============================================================================
 * PROBLEM: Implement Two Stacks in One Array
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: High
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. APPROACH:
 *    - Stack1 grows from left (index 0)
 *    - Stack2 grows from right (index n-1)
 *    - Meet in middle when full
 *
 * ============================================================================
 * VISUAL REPRESENTATION:
 * ============================================================================
 *
 * +---+---+---+---+---+---+---+---+---+---+
 * | A | B | C |   |   |   |   | Z | Y | X |
 * +---+---+---+---+---+---+---+---+---+---+
 *   0   1   2   3   4   5   6   7   8   9
 *           ^                   ^
 *         top1               top2
 *
 * Stack1: grows →
 * Stack2: grows ←
 *
 * Full when: top1 + 1 == top2
 *
 * ============================================================================
 * TIME: O(1) for all operations
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

#define SIZE 10

typedef struct {
    int arr[SIZE];
    int top1;
    int top2;
} TwoStacks;

void initTwoStacks(TwoStacks* ts) {
    ts->top1 = -1;
    ts->top2 = SIZE;
}

bool isFull(TwoStacks* ts) {
    return ts->top1 + 1 == ts->top2;
}

// Push to stack 1
bool push1(TwoStacks* ts, int value) {
    if (isFull(ts)) {
        printf("Overflow!\n");
        return false;
    }
    ts->arr[++ts->top1] = value;
    return true;
}

// Push to stack 2
bool push2(TwoStacks* ts, int value) {
    if (isFull(ts)) {
        printf("Overflow!\n");
        return false;
    }
    ts->arr[--ts->top2] = value;
    return true;
}

int pop1(TwoStacks* ts) {
    if (ts->top1 == -1) {
        printf("Stack1 Underflow!\n");
        return -1;
    }
    return ts->arr[ts->top1--];
}

int pop2(TwoStacks* ts) {
    if (ts->top2 == SIZE) {
        printf("Stack2 Underflow!\n");
        return -1;
    }
    return ts->arr[ts->top2++];
}

int main() {
    printf("=== Two Stacks in One Array ===\n\n");

    TwoStacks ts;
    initTwoStacks(&ts);

    printf("Push to Stack1: 1, 2, 3\n");
    push1(&ts, 1);
    push1(&ts, 2);
    push1(&ts, 3);

    printf("Push to Stack2: 9, 8, 7\n");
    push2(&ts, 9);
    push2(&ts, 8);
    push2(&ts, 7);

    printf("\nPop from Stack1: %d\n", pop1(&ts));
    printf("Pop from Stack2: %d\n", pop2(&ts));

    printf("\nPop from Stack1: %d\n", pop1(&ts));
    printf("Pop from Stack2: %d\n", pop2(&ts));

    return 0;
}
