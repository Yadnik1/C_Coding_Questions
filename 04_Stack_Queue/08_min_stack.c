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

// Define maximum stack size
// Say: "I'll define the maximum capacity for the stack"
#define MAX_SIZE 100

// MinStack structure with two parallel stacks
// Say: "I create a MinStack struct with two arrays: one for values, one for minimums"
typedef struct {
    int mainStack[MAX_SIZE];  // Main stack for actual values
    int minStack[MAX_SIZE];   // Parallel stack to track minimum at each level
    int top;                  // Shared top index for both stacks
} MinStack;

// Initialize min stack
// Say: "This initializes the min stack to empty"
void initMinStack(MinStack* ms) {
    // Set top to -1
    // Say: "I set top to -1 for an empty stack"
    ms->top = -1;
}

// Check if stack is empty
// Say: "This checks if the stack is empty"
int isEmpty(MinStack* ms) {
    // Return true if top is -1
    // Say: "I return true when top is -1"
    return ms->top == -1;
}

// Push value onto min stack
// Say: "Now I'll implement push which maintains the minimum at each level"
void push(MinStack* ms, int val) {
    // Increment top
    // Say: "First I increment top to make room for the new element"
    ms->top++;

    // Store value in main stack
    // Say: "I store the value in the main stack"
    ms->mainStack[ms->top] = val;

    // Update min stack
    // Say: "Now I need to update the min stack"
    if (ms->top == 0 || val < ms->minStack[ms->top - 1]) {
        // New minimum found
        // Say: "If this is the first element or less than previous min, I push it"
        ms->minStack[ms->top] = val;
    } else {
        // Current minimum still holds
        // Say: "Otherwise, I carry forward the previous minimum"
        ms->minStack[ms->top] = ms->minStack[ms->top - 1];
    }
}

// Pop value from min stack
// Say: "This pops the top element from both stacks"
int pop(MinStack* ms) {
    // Check if empty
    // Say: "First I check if the stack is empty"
    if (isEmpty(ms)) {
        // Return error value
        // Say: "If empty, I return INT_MIN as an error code"
        return INT_MIN;
    }

    // Return and decrement
    // Say: "I return the top value and decrement top, removing from both stacks"
    return ms->mainStack[ms->top--];
}

// Get top element
// Say: "This returns the top element without removing it"
int top(MinStack* ms) {
    // Check if empty
    // Say: "I check if the stack is empty"
    if (isEmpty(ms)) {
        // Return error value
        // Say: "If empty, I return INT_MIN"
        return INT_MIN;
    }

    // Return top value from main stack
    // Say: "I return the top value from the main stack"
    return ms->mainStack[ms->top];
}

// Get minimum element in O(1)
// Say: "This returns the minimum element in constant time"
int getMin(MinStack* ms) {
    // Check if empty
    // Say: "I check if the stack is empty"
    if (isEmpty(ms)) {
        // Return error value
        // Say: "If empty, I return INT_MIN"
        return INT_MIN;
    }

    // Return top of min stack
    // Say: "I simply return the top of the min stack, which is the current minimum"
    return ms->minStack[ms->top];
}

int main() {
    // Say: "Let me demonstrate the min stack implementation"
    printf("=== Min Stack ===\n\n");

    // Declare min stack
    // Say: "I declare a MinStack variable"
    MinStack ms;

    // Initialize
    // Say: "I initialize the stack to empty"
    initMinStack(&ms);

    // Push 5
    // Say: "Let me push 5 and check the minimum"
    printf("Push 5: ");
    push(&ms, 5);
    printf("Min = %d\n", getMin(&ms));

    // Push 3
    // Say: "Now I push 3, which becomes the new minimum"
    printf("Push 3: ");
    push(&ms, 3);
    printf("Min = %d\n", getMin(&ms));

    // Push 7
    // Say: "I push 7, but the minimum remains 3"
    printf("Push 7: ");
    push(&ms, 7);
    printf("Min = %d\n", getMin(&ms));

    // Push 2
    // Say: "I push 2, which becomes the new minimum"
    printf("Push 2: ");
    push(&ms, 2);
    printf("Min = %d\n", getMin(&ms));

    // Pop 2
    // Say: "Now I'll pop, which removes 2"
    printf("\nPop %d: ", pop(&ms));
    printf("Min = %d\n", getMin(&ms));

    // Pop 7
    // Say: "I pop again, removing 7"
    printf("Pop %d: ", pop(&ms));
    printf("Min = %d\n", getMin(&ms));

    // Pop 3
    // Say: "I pop again, removing 3"
    printf("Pop %d: ", pop(&ms));
    printf("Min = %d\n", getMin(&ms));

    return 0;
}
