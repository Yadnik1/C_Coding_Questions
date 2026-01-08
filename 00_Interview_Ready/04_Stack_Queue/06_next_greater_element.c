/*
 * ============================================================================
 * PROBLEM: Next Greater Element (NGE)
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given an array of integers, find the Next Greater Element (NGE) for every
 * element. The NGE of an element x is the first element to its RIGHT that is
 * greater than x. If no such element exists, the answer is -1.
 *
 * This is a classic "Monotonic Stack" problem - a stack that maintains elements
 * in sorted (increasing or decreasing) order.
 *
 * EXAMPLES:
 * - Input:  [4, 5, 2, 10, 8]
 *   Output: [5, 10, 10, -1, -1]
 *
 *   Explanation:
 *   4  -> Next greater is 5
 *   5  -> Next greater is 10
 *   2  -> Next greater is 10
 *   10 -> No greater element to the right, so -1
 *   8  -> No greater element to the right, so -1
 *
 * - Input:  [6, 8, 0, 1, 3]
 *   Output: [8, -1, 1, 3, -1]
 *
 * - Input:  [3, 2, 1]
 *   Output: [-1, -1, -1]  (strictly decreasing, no NGE for any)
 *
 * - Input:  [1, 2, 3]
 *   Output: [2, 3, -1]    (strictly increasing)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Introduces the powerful "Monotonic Stack" pattern
 * - O(n) solution is non-obvious (brute force is O(n^2))
 * - Foundation for many harder problems:
 *   - Stock Span Problem
 *   - Largest Rectangle in Histogram
 *   - Daily Temperatures
 *   - Trapping Rain Water
 * - Tests ability to see that each element is processed at most twice
 * - Popular at Amazon, Google, Microsoft, and Meta
 *
 * KEY CONCEPT:
 * Use a Monotonic Decreasing Stack (elements decrease from bottom to top).
 *
 * Process RIGHT to LEFT:
 * 1. For each element, pop all smaller elements (they can't be NGE for anyone)
 * 2. If stack not empty, top is the NGE
 * 3. If stack empty, no NGE exists (-1)
 * 4. Push current element for future elements to use
 *
 * Why O(n)? Each element is pushed once and popped at most once = 2n operations.
 *
 * VISUAL:
 *
 *   Array: [4, 5, 2, 10, 8]
 *   Processing RIGHT to LEFT:
 *
 *   i=4, val=8:  Stack=[]        -> NGE=-1,  push 8   Stack=[8]
 *   i=3, val=10: Stack=[8]       -> 8<10, pop        Stack=[]
 *                                -> NGE=-1,  push 10  Stack=[10]
 *   i=2, val=2:  Stack=[10]      -> 10>2, NGE=10     Stack=[10,2]
 *   i=1, val=5:  Stack=[10,2]    -> 2<5, pop         Stack=[10]
 *                                -> 10>5, NGE=10     Stack=[10,5]
 *   i=0, val=4:  Stack=[10,5]    -> 5>4, NGE=5       Stack=[10,5,4]
 *
 *   Result: [5, 10, 10, -1, -1]
 *
 *   Stack visualization (decreasing from bottom):
 *
 *       +----+
 *       |  4 | <- top (smallest)
 *       +----+
 *       |  5 |
 *       +----+
 *       | 10 | <- bottom (largest)
 *       +----+
 *
 *   Property: For any element, all elements below it in stack are larger,
 *   so the immediate element below (or top after pops) is the NGE!
 *
 * ============================================================================
 */

// Next Greater Element using Stack - Popular monotonic stack problem
// Time: O(n), Space: O(n)

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;

void stack_init(Stack *s) { s->top = -1; }
bool is_empty(Stack *s) { return s->top == -1; }
void push(Stack *s, int val) { s->data[++s->top] = val; }
int pop(Stack *s) { return s->data[s->top--]; }
int peek(Stack *s) { return s->data[s->top]; }

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]");
}

// Say: "Find next greater element for each array element"
// Say: "Result[i] = first element to the right that is greater than arr[i]"
void next_greater_element(int arr[], int n, int result[]) {
    Stack s;
    stack_init(&s);

    // Say: "Process from right to left"
    // Say: "Stack maintains elements in decreasing order from bottom to top"
    for (int i = n - 1; i >= 0; i--) {
        // Say: "Pop elements smaller than current - they can't be NGE for anyone to the left"
        while (!is_empty(&s) && peek(&s) <= arr[i]) {
            pop(&s);
        }

        // Say: "If stack empty, no greater element exists to the right"
        // Say: "Otherwise, stack top is the next greater element"
        result[i] = is_empty(&s) ? -1 : peek(&s);

        // Say: "Push current element for future comparisons"
        push(&s, arr[i]);
    }
}

// Alternative: Left to right approach (also valid)
void next_greater_element_v2(int arr[], int n, int result[]) {
    Stack s;  // Say: "This stack stores INDICES, not values"
    stack_init(&s);

    // Initialize result with -1
    for (int i = 0; i < n; i++) {
        result[i] = -1;
    }

    // Say: "Process left to right, stack holds indices of unresolved elements"
    for (int i = 0; i < n; i++) {
        // Say: "Current element is NGE for all smaller elements in stack"
        while (!is_empty(&s) && arr[peek(&s)] < arr[i]) {
            int idx = pop(&s);
            result[idx] = arr[i];  // Say: "Found NGE for element at idx"
        }
        push(&s, i);  // Say: "Push current index"
    }
    // Say: "Elements remaining in stack have no NGE (result already -1)"
}

int main() {
    int arr[] = {4, 5, 2, 10, 8};
    int n = sizeof(arr) / sizeof(arr[0]);
    int result[n];

    printf("Array:  ");
    print_array(arr, n);
    printf("\n\n");

    next_greater_element(arr, n, result);
    printf("NGE (right to left method):\n");
    for (int i = 0; i < n; i++) {
        printf("  %d -> %d\n", arr[i], result[i]);
    }

    printf("\nNGE (left to right method):\n");
    next_greater_element_v2(arr, n, result);
    for (int i = 0; i < n; i++) {
        printf("  %d -> %d\n", arr[i], result[i]);
    }

    // Test with another array
    int arr2[] = {6, 8, 0, 1, 3};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    int result2[n2];

    printf("\nArray:  ");
    print_array(arr2, n2);
    printf("\n");

    next_greater_element(arr2, n2, result2);
    for (int i = 0; i < n2; i++) {
        printf("  %d -> %d\n", arr2[i], result2[i]);
    }

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use a monotonic stack to find the next greater element for each position.

 PROBLEM:
 For each element, find the first element to its right that is greater.
 If none exists, return -1.

 APPROACH 1 - RIGHT TO LEFT:
 - Process array from right to left
 - Stack maintains candidates for NGE in decreasing order
 - Pop elements <= current (they can't be NGE for elements to the left)
 - Stack top (if exists) is the NGE for current element
 - Push current element for future use

 APPROACH 2 - LEFT TO RIGHT:
 - Stack stores INDICES of unresolved elements
 - When we find a larger element, it's the NGE for all smaller ones in stack
 - Pop and set NGE until stack top is >= current

 WHY O(n) TIME:
 - Each element pushed once, popped at most once
 - Total: 2n operations = O(n)

 EXAMPLE for [4, 5, 2, 10, 8]:
 Right to left:
 - i=4 (8): stack=[], result[4]=-1, push 8
 - i=3 (10): pop 8 (8<10), stack=[], result[3]=-1, push 10
 - i=2 (2): stack=[10], result[2]=10, push 2
 - i=1 (5): pop 2 (2<5), stack=[10], result[1]=10, push 5
 - i=0 (4): stack=[10,5], result[0]=5

 MONOTONIC STACK PATTERN:
 - Maintains elements in sorted order
 - Decreasing stack: finds next greater
 - Increasing stack: finds next smaller

 VARIATIONS:
 - Next Smaller Element
 - Previous Greater Element
 - Stock Span Problem
 - Largest Rectangle in Histogram

 COMMON MISTAKES:
 - Using <= vs < in comparison
 - Storing values vs indices
 - Wrong traversal direction"
*/
