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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why grow from opposite ends?"
 * A1: Maximizes space utilization! If both grew same direction,
 *     one stack might overflow while space exists for the other.
 *     This way, total elements can use ALL available space.
 *
 * -------------------------------------------------------------------------
 * Q2: "Can you implement 3 stacks in one array?"
 * A2: More complex! Options:
 *     - Fixed division (waste space)
 *     - Dynamic: middle stack uses flexible boundaries
 *     - Array of records with next pointers (linked list in array)
 *
 * -------------------------------------------------------------------------
 * Q3: "How do you check if array is full?"
 * A3: top1 + 1 == top2 means pointers have met.
 *     No more space for either stack!
 *
 * -------------------------------------------------------------------------
 * Q4: "What's the embedded systems use case?"
 * A4: Limited memory! Two related data structures sharing one buffer.
 *     Example: TX and RX buffers in same memory region.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Define array size
// Say: "I'll define the size for our shared array"
#define SIZE 10

// Structure for two stacks
// Say: "I create a struct that holds one array and two top pointers"
typedef struct {
    int arr[SIZE];  // Shared array for both stacks
    int top1;       // Top index for stack 1 (grows right)
    int top2;       // Top index for stack 2 (grows left)
} TwoStacks;

// Initialize both stacks
// Say: "This function initializes both stacks to empty"
void initTwoStacks(TwoStacks* ts) {
    // Set top1 to -1 (before first position)
    // Say: "I set top1 to -1, before the first index"
    ts->top1 = -1;

    // Set top2 to SIZE (after last position)
    // Say: "I set top2 to SIZE, after the last index"
    ts->top2 = SIZE;
}

// Check if array is full
// Say: "This checks if both stacks have met in the middle"
bool isFull(TwoStacks* ts) {
    // Full when tops are adjacent
    // Say: "The array is full when top1 plus 1 equals top2"
    return ts->top1 + 1 == ts->top2;
}

// Push to stack 1
// Say: "Now I'll implement push for stack 1, which grows from the left"
bool push1(TwoStacks* ts, int value) {
    // Check if full
    // Say: "First I check if the array is full"
    if (isFull(ts)) {
        // Print error
        // Say: "If full, I print overflow and return false"
        printf("Overflow!\n");
        return false;
    }

    // Increment top1 and insert
    // Say: "I increment top1 and store the value there"
    ts->arr[++ts->top1] = value;

    // Return success
    // Say: "I return true to indicate success"
    return true;
}

// Push to stack 2
// Say: "Now I'll implement push for stack 2, which grows from the right"
bool push2(TwoStacks* ts, int value) {
    // Check if full
    // Say: "First I check if the array is full"
    if (isFull(ts)) {
        // Print error
        // Say: "If full, I print overflow and return false"
        printf("Overflow!\n");
        return false;
    }

    // Decrement top2 and insert
    // Say: "I decrement top2 and store the value there"
    ts->arr[--ts->top2] = value;

    // Return success
    // Say: "I return true to indicate success"
    return true;
}

// Pop from stack 1
// Say: "This pops an element from stack 1"
int pop1(TwoStacks* ts) {
    // Check if stack1 is empty
    // Say: "I check if stack1 is empty by seeing if top1 is -1"
    if (ts->top1 == -1) {
        // Print error
        // Say: "If empty, I print underflow and return -1"
        printf("Stack1 Underflow!\n");
        return -1;
    }

    // Return value and decrement top1
    // Say: "I return the value at top1 and decrement it"
    return ts->arr[ts->top1--];
}

// Pop from stack 2
// Say: "This pops an element from stack 2"
int pop2(TwoStacks* ts) {
    // Check if stack2 is empty
    // Say: "I check if stack2 is empty by seeing if top2 equals SIZE"
    if (ts->top2 == SIZE) {
        // Print error
        // Say: "If empty, I print underflow and return -1"
        printf("Stack2 Underflow!\n");
        return -1;
    }

    // Return value and increment top2
    // Say: "I return the value at top2 and increment it back"
    return ts->arr[ts->top2++];
}

int main() {
    // Say: "Let me demonstrate two stacks in one array"
    printf("=== Two Stacks in One Array ===\n\n");

    // Declare two stacks structure
    // Say: "I declare a TwoStacks variable"
    TwoStacks ts;

    // Initialize
    // Say: "I initialize both stacks to empty"
    initTwoStacks(&ts);

    // Push to stack 1
    // Say: "Let me push 1, 2, 3 to stack 1"
    printf("Push to Stack1: 1, 2, 3\n");
    push1(&ts, 1);  // Push 1
    push1(&ts, 2);  // Push 2
    push1(&ts, 3);  // Push 3

    // Push to stack 2
    // Say: "Now I'll push 9, 8, 7 to stack 2"
    printf("Push to Stack2: 9, 8, 7\n");
    push2(&ts, 9);  // Push 9
    push2(&ts, 8);  // Push 8
    push2(&ts, 7);  // Push 7

    // Pop from stack 1
    // Say: "Let me pop one element from stack 1"
    printf("\nPop from Stack1: %d\n", pop1(&ts));

    // Pop from stack 2
    // Say: "And pop one element from stack 2"
    printf("Pop from Stack2: %d\n", pop2(&ts));

    // Pop again from stack 1
    // Say: "I'll pop another from stack 1"
    printf("\nPop from Stack1: %d\n", pop1(&ts));

    // Pop again from stack 2
    // Say: "And another from stack 2"
    printf("Pop from Stack2: %d\n", pop2(&ts));

    return 0;
}
