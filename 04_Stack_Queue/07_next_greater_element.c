/*
 * ============================================================================
 * PROBLEM: Next Greater Element
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: High
 *
 * For each element, find the next greater element to its right.
 *
 * Example:
 * Input:  [4, 5, 2, 25]
 * Output: [5, 25, 25, -1]
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. USE STACK:
 *    - Traverse from right to left
 *    - Stack keeps potential "next greater" candidates
 *    - Pop smaller elements (they can't be answer for future)
 *
 * 2. MONOTONIC STACK:
 *    - Stack maintains decreasing order
 *    - Top is smallest, bottom is largest
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: [4, 5, 2, 25]
 * ============================================================================
 *
 * Process right to left:
 *
 * i=3, arr[3]=25:
 *   Stack empty → NGE = -1
 *   Push 25
 *   Stack: [25]
 *
 * i=2, arr[2]=2:
 *   Top=25 > 2 → NGE = 25
 *   Push 2
 *   Stack: [25, 2]
 *
 * i=1, arr[1]=5:
 *   Top=2 < 5 → Pop 2
 *   Top=25 > 5 → NGE = 25
 *   Push 5
 *   Stack: [25, 5]
 *
 * i=0, arr[0]=4:
 *   Top=5 > 4 → NGE = 5
 *   Push 4
 *   Stack: [25, 5, 4]
 *
 * Result: [5, 25, 25, -1]
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(n)
 * ============================================================================
 */

#include <stdio.h>

#define MAX_SIZE 100

typedef struct {
    int arr[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack* s) { s->top = -1; }
int isEmpty(Stack* s) { return s->top == -1; }
void push(Stack* s, int val) { s->arr[++s->top] = val; }
int pop(Stack* s) { return s->arr[s->top--]; }
int peek(Stack* s) { return s->arr[s->top]; }

void nextGreaterElement(int arr[], int n, int result[]) {
    Stack s;
    initStack(&s);

    // Process from right to left
    for (int i = n - 1; i >= 0; i--) {
        // Pop elements smaller than current
        while (!isEmpty(&s) && peek(&s) <= arr[i]) {
            pop(&s);
        }

        // If stack empty, no greater element
        result[i] = isEmpty(&s) ? -1 : peek(&s);

        // Push current element
        push(&s, arr[i]);
    }
}

void printArray(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    printf("=== Next Greater Element ===\n\n");

    int arr[] = {4, 5, 2, 25};
    int n = 4;
    int result[4];

    printf("Input:  ");
    printArray(arr, n);

    nextGreaterElement(arr, n, result);

    printf("Output: ");
    printArray(result, n);

    printf("\nElement → NGE:\n");
    for (int i = 0; i < n; i++) {
        printf("  %d → %d\n", arr[i], result[i]);
    }

    // Another test
    int arr2[] = {13, 7, 6, 12};
    int n2 = 4;
    int result2[4];

    printf("\nInput:  ");
    printArray(arr2, n2);

    nextGreaterElement(arr2, n2, result2);

    printf("Output: ");
    printArray(result2, n2);

    return 0;
}
