/*
 * ============================================================================
 * PROBLEM: Min Stack - Get Minimum in O(1)
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: High
 *
 * Design a stack that supports push, pop, top, and getMin in O(1) time.
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. TWO STACKS APPROACH:
 *    - Main stack for values
 *    - Min stack tracks minimum at each level
 *
 * 2. SINGLE STACK APPROACH:
 *    - Store encoded values
 *    - When new min, store: 2*val - oldMin
 *
 * ============================================================================
 * TWO STACKS VISUALIZATION:
 * ============================================================================
 *
 * Push sequence: 5, 3, 7, 2
 *
 * After push(5):
 *   Main: [5]    Min: [5]
 *
 * After push(3):
 *   Main: [5,3]  Min: [5,3]  (3 < 5, push 3)
 *
 * After push(7):
 *   Main: [5,3,7]  Min: [5,3,3]  (7 > 3, push current min)
 *
 * After push(2):
 *   Main: [5,3,7,2]  Min: [5,3,3,2]  (2 < 3, push 2)
 *
 * getMin() → peek Min stack → 2
 *
 * After pop():
 *   Main: [5,3,7]  Min: [5,3,3]
 *   getMin() → 3
 *
 * ============================================================================
 * TIME: O(1) for all operations
 * SPACE: O(n) for auxiliary min stack
 * ============================================================================
 */

#include <stdio.h>
#include <limits.h>

#define MAX_SIZE 100

typedef struct {
    int mainStack[MAX_SIZE];
    int minStack[MAX_SIZE];
    int top;
} MinStack;

void initMinStack(MinStack* ms) {
    ms->top = -1;
}

int isEmpty(MinStack* ms) {
    return ms->top == -1;
}

void push(MinStack* ms, int val) {
    ms->top++;
    ms->mainStack[ms->top] = val;

    // Track minimum
    if (ms->top == 0 || val < ms->minStack[ms->top - 1]) {
        ms->minStack[ms->top] = val;
    } else {
        ms->minStack[ms->top] = ms->minStack[ms->top - 1];
    }
}

int pop(MinStack* ms) {
    if (isEmpty(ms)) return INT_MIN;
    return ms->mainStack[ms->top--];
}

int top(MinStack* ms) {
    if (isEmpty(ms)) return INT_MIN;
    return ms->mainStack[ms->top];
}

int getMin(MinStack* ms) {
    if (isEmpty(ms)) return INT_MIN;
    return ms->minStack[ms->top];
}

int main() {
    printf("=== Min Stack ===\n\n");

    MinStack ms;
    initMinStack(&ms);

    printf("Push 5: ");
    push(&ms, 5);
    printf("Min = %d\n", getMin(&ms));

    printf("Push 3: ");
    push(&ms, 3);
    printf("Min = %d\n", getMin(&ms));

    printf("Push 7: ");
    push(&ms, 7);
    printf("Min = %d\n", getMin(&ms));

    printf("Push 2: ");
    push(&ms, 2);
    printf("Min = %d\n", getMin(&ms));

    printf("\nPop %d: ", pop(&ms));
    printf("Min = %d\n", getMin(&ms));

    printf("Pop %d: ", pop(&ms));
    printf("Min = %d\n", getMin(&ms));

    printf("Pop %d: ", pop(&ms));
    printf("Min = %d\n", getMin(&ms));

    return 0;
}
