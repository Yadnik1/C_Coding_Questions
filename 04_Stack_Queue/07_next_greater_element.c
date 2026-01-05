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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why traverse right to left?"
 * A1: We need elements to the RIGHT of current. By going right-to-left,
 *     stack already contains all right-side elements.
 *
 * -------------------------------------------------------------------------
 * Q2: "Why is this O(n) despite nested loop?"
 * A2: Each element is pushed once and popped at most once.
 *     Total operations = 2n = O(n). This is the monotonic stack pattern!
 *
 * -------------------------------------------------------------------------
 * Q3: "What's a monotonic stack?"
 * A3: Stack that maintains elements in sorted order (increasing or decreasing).
 *     Here, decreasing from bottom to top - smallest on top.
 *
 * -------------------------------------------------------------------------
 * Q4: "What about Next Greater Element in circular array?"
 * A4: Traverse array twice (or use 2n-1 iterations with modulo).
 *     Same algorithm, just wrap around to beginning.
 *
 * ============================================================================
 */

#include <stdio.h>

// Define maximum stack size
// Say: "I'll define the maximum size for the stack"
#define MAX_SIZE 100

// Stack structure
// Say: "I create a stack struct to hold integers"
typedef struct {
    int arr[MAX_SIZE];  // Array for stack elements
    int top;            // Top index
} Stack;

// Initialize stack
// Say: "This initializes the stack to empty"
void initStack(Stack* s) {
    // Set top to -1
    // Say: "I set top to -1 for empty stack"
    s->top = -1;
}

// Check if empty
// Say: "This checks if the stack is empty"
int isEmpty(Stack* s) {
    // Return true if top is -1
    // Say: "I return true when top is -1"
    return s->top == -1;
}

// Push value
// Say: "This pushes a value onto the stack"
void push(Stack* s, int val) {
    // Increment top and store
    // Say: "I increment top and store the value"
    s->arr[++s->top] = val;
}

// Pop value
// Say: "This pops and returns the top value"
int pop(Stack* s) {
    // Return and decrement
    // Say: "I return the top value and decrement top"
    return s->arr[s->top--];
}

// Peek at top
// Say: "This returns the top value without removing it"
int peek(Stack* s) {
    // Return top value
    // Say: "I return the value at top"
    return s->arr[s->top];
}

// Find next greater element for each element
// Say: "Now I'll implement the main function to find next greater elements"
void nextGreaterElement(int arr[], int n, int result[]) {
    // Declare stack
    // Say: "I declare a stack to hold candidates"
    Stack s;

    // Initialize stack
    // Say: "I initialize the stack to empty"
    initStack(&s);

    // Process from right to left
    // Say: "I'll traverse the array from right to left"
    for (int i = n - 1; i >= 0; i--) {
        // Pop elements smaller than or equal to current
        // Say: "First I pop all elements from stack that are smaller or equal to current"
        while (!isEmpty(&s) && peek(&s) <= arr[i]) {
            // Pop smaller element
            // Say: "I pop this smaller element since it can't be the answer"
            pop(&s);
        }

        // Assign result
        // Say: "If stack is empty, there's no greater element, so I assign -1"
        result[i] = isEmpty(&s) ? -1 : peek(&s);
        // Say: "Otherwise, the top of stack is the next greater element"

        // Push current element
        // Say: "Finally, I push the current element onto the stack"
        push(&s, arr[i]);
    }
}

// Helper function to print array
// Say: "This is a helper function to print an array nicely"
void printArray(int arr[], int n) {
    // Print opening bracket
    // Say: "I print an opening bracket"
    printf("[");

    // Print each element
    // Say: "Then I print each element with commas"
    for (int i = 0; i < n; i++) {
        // Print element
        printf("%d", arr[i]);

        // Print comma if not last
        // Say: "I add a comma if it's not the last element"
        if (i < n - 1) {
            printf(", ");
        }
    }

    // Print closing bracket
    // Say: "Finally I print the closing bracket and newline"
    printf("]\n");
}

int main() {
    // Say: "Let me demonstrate the next greater element algorithm"
    printf("=== Next Greater Element ===\n\n");

    // First test array
    // Say: "I'll create the first test array"
    int arr[] = {4, 5, 2, 25};

    // Get array size
    // Say: "I calculate the array size"
    int n = 4;

    // Result array
    // Say: "I declare a result array to store answers"
    int result[4];

    // Print input
    // Say: "Let me show the input array"
    printf("Input:  ");
    printArray(arr, n);

    // Find next greater elements
    // Say: "Now I call the function to find next greater elements"
    nextGreaterElement(arr, n, result);

    // Print output
    // Say: "And here's the output"
    printf("Output: ");
    printArray(result, n);

    // Print mapping
    // Say: "Let me also show the element to NGE mapping"
    printf("\nElement → NGE:\n");
    for (int i = 0; i < n; i++) {
        // Print each mapping
        // Say: "I print each element and its next greater element"
        printf("  %d → %d\n", arr[i], result[i]);
    }

    // Second test
    // Say: "Let me try another example"
    int arr2[] = {13, 7, 6, 12};

    // Get size
    // Say: "I get the size of the second array"
    int n2 = sizeof(arr2) / sizeof(arr2[0]);

    // Result array
    // Say: "I declare another result array"
    int result2[4];

    // Print input
    // Say: "Here's the second input"
    printf("\nInput:  ");
    printArray(arr2, n2);

    // Find NGE
    // Say: "I find the next greater elements"
    nextGreaterElement(arr2, n2, result2);

    // Print output
    // Say: "And here's the result"
    printf("Output: ");
    printArray(result2, n2);

    return 0;
}
