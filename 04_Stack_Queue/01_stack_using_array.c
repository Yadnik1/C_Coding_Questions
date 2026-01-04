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

// Define maximum stack size
// Say: "I'll define the maximum capacity of our stack"
#define MAX_SIZE 100

// Stack structure with array and top pointer
// Say: "The stack struct contains an array for data and a top index"
typedef struct {
    int arr[MAX_SIZE];  // Array to store elements
    int top;            // Index of top element (-1 when empty)
} Stack;

// Initialize stack to empty state
// Say: "First, I'll create an initialization function that sets top to -1"
void initStack(Stack* s) {
    // Set top to -1 to indicate empty stack
    // Say: "I set top to -1, which represents an empty stack"
    s->top = -1;
}

// Check if stack is empty
// Say: "Now I'll implement isEmpty to check if the stack has no elements"
bool isEmpty(Stack* s) {
    // Stack is empty when top is -1
    // Say: "I return true if top equals -1, meaning the stack is empty"
    return s->top == -1;
}

// Check if stack is full
// Say: "I need to check if the stack is full before pushing"
bool isFull(Stack* s) {
    // Stack is full when top reaches MAX_SIZE - 1
    // Say: "I return true if top equals MAX_SIZE minus 1, meaning no more space"
    return s->top == MAX_SIZE - 1;
}

// Push element onto stack
// Say: "Now I'll implement push to add an element to the top of the stack"
bool push(Stack* s, int value) {
    // First check if stack is full
    // Say: "First I check if the stack is full to avoid overflow"
    if (isFull(s)) {
        // Print error message if stack is full
        // Say: "If it's full, I print an error and return false"
        printf("Stack Overflow!\n");
        return false;
    }

    // Increment top first
    // Say: "I increment top to point to the next available position"
    s->top++;

    // Store value at new top position
    // Say: "Then I store the value at this new top position"
    s->arr[s->top] = value;

    // Return success
    // Say: "Finally, I return true to indicate successful push"
    return true;
}

// Pop element from stack
// Say: "Now I'll implement pop to remove and return the top element"
int pop(Stack* s) {
    // Check if stack is empty
    // Say: "First I check if the stack is empty to avoid underflow"
    if (isEmpty(s)) {
        // Print error message if stack is empty
        // Say: "If it's empty, I print an error and return -1"
        printf("Stack Underflow!\n");
        return -1;
    }

    // Get value at top
    // Say: "I save the value at the top position"
    int value = s->arr[s->top];

    // Decrement top to remove element
    // Say: "Then I decrement top, effectively removing the element"
    s->top--;

    // Return the popped value
    // Say: "Finally, I return the value that was at the top"
    return value;
}

// Peek at top element without removing
// Say: "I'll implement peek to view the top element without removing it"
int peek(Stack* s) {
    // Check if stack is empty
    // Say: "First I check if the stack is empty"
    if (isEmpty(s)) {
        // Print error message
        // Say: "If empty, I print an error and return -1"
        printf("Stack is empty!\n");
        return -1;
    }

    // Return value at top without modifying top
    // Say: "I return the value at top without changing the top index"
    return s->arr[s->top];
}

// Get current size of stack
// Say: "I'll implement size to count how many elements are in the stack"
int size(Stack* s) {
    // Size is top + 1 (since top starts at -1 for empty)
    // Say: "The size is top plus 1, since top is -1 when empty"
    return s->top + 1;
}

int main() {
    // Say: "Let me demonstrate the stack implementation"
    printf("=== Stack Using Array ===\n\n");

    // Declare stack variable
    // Say: "I'll declare a stack variable"
    Stack s;

    // Initialize the stack
    // Say: "Now I initialize the stack to empty state"
    initStack(&s);

    // Push some values
    // Say: "Let me push three values: 10, 20, and 30"
    printf("Pushing: 10, 20, 30\n");
    push(&s, 10);   // Push 10
    push(&s, 20);   // Push 20
    push(&s, 30);   // Push 30

    // Display top element
    // Say: "I'll check what's at the top using peek"
    printf("Top element: %d\n", peek(&s));

    // Display stack size
    // Say: "Let me also check the current size of the stack"
    printf("Stack size: %d\n", size(&s));

    // Pop all elements
    // Say: "Now I'll pop all elements to demonstrate the LIFO order"
    printf("\nPopping elements:\n");
    while (!isEmpty(&s)) {
        // Pop and print each element
        // Say: "I pop each element one by one until the stack is empty"
        printf("Popped: %d\n", pop(&s));
    }

    // Check if empty
    // Say: "Finally, I'll verify that the stack is now empty"
    printf("\nStack empty: %s\n", isEmpty(&s) ? "Yes" : "No");

    return 0;
}
