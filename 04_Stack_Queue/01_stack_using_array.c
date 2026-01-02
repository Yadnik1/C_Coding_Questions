/*
 * ============================================================================
 * PROBLEM: Implement Stack Using Array
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: Very High (MUST KNOW!)
 *
 * Implement a stack with push, pop, peek, isEmpty operations.
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. STACK = LIFO (Last In, First Out)
 *    - Like stack of plates
 *    - Push: Add to top
 *    - Pop: Remove from top
 *
 * 2. ARRAY IMPLEMENTATION:
 *    - Use 'top' index to track top element
 *    - top = -1 means empty
 *    - Push: top++, arr[top] = value
 *    - Pop: return arr[top], top--
 *
 * ============================================================================
 * VISUAL REPRESENTATION:
 * ============================================================================
 *
 * Empty stack (top = -1):
 *   +---+---+---+---+---+
 *   |   |   |   |   |   |
 *   +---+---+---+---+---+
 *     0   1   2   3   4
 *   top = -1
 *
 * After push(10):
 *   +---+---+---+---+---+
 *   |10 |   |   |   |   |
 *   +---+---+---+---+---+
 *     ^
 *   top = 0
 *
 * After push(20):
 *   +---+---+---+---+---+
 *   |10 |20 |   |   |   |
 *   +---+---+---+---+---+
 *         ^
 *       top = 1
 *
 * After pop() returns 20:
 *   +---+---+---+---+---+
 *   |10 |   |   |   |   |
 *   +---+---+---+---+---+
 *     ^
 *   top = 0
 *
 * ============================================================================
 * OPERATIONS COMPLEXITY:
 * ============================================================================
 *
 * | Operation | Time | Space |
 * |-----------|------|-------|
 * | Push      | O(1) | O(1)  |
 * | Pop       | O(1) | O(1)  |
 * | Peek      | O(1) | O(1)  |
 * | isEmpty   | O(1) | O(1)  |
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 100

typedef struct {
    int arr[MAX_SIZE];
    int top;
} Stack;

// Initialize stack
void initStack(Stack* s) {
    s->top = -1;
}

// Check if empty
bool isEmpty(Stack* s) {
    return s->top == -1;
}

// Check if full
bool isFull(Stack* s) {
    return s->top == MAX_SIZE - 1;
}

// Push element
bool push(Stack* s, int value) {
    if (isFull(s)) {
        printf("Stack Overflow!\n");
        return false;
    }
    s->top++;
    s->arr[s->top] = value;
    return true;
}

// Pop element
int pop(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack Underflow!\n");
        return -1;
    }
    int value = s->arr[s->top];
    s->top--;
    return value;
}

// Peek top element
int peek(Stack* s) {
    if (isEmpty(s)) {
        printf("Stack is empty!\n");
        return -1;
    }
    return s->arr[s->top];
}

// Get size
int size(Stack* s) {
    return s->top + 1;
}

int main() {
    printf("=== Stack Using Array ===\n\n");

    Stack s;
    initStack(&s);

    printf("Pushing: 10, 20, 30\n");
    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    printf("Top element: %d\n", peek(&s));
    printf("Stack size: %d\n", size(&s));

    printf("\nPopping elements:\n");
    while (!isEmpty(&s)) {
        printf("Popped: %d\n", pop(&s));
    }

    printf("\nStack empty: %s\n", isEmpty(&s) ? "Yes" : "No");

    return 0;
}
