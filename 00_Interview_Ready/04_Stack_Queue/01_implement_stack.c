/*
 * ============================================================================
 * PROBLEM: Implement Stack using Array
 * ============================================================================
 *
 * WHAT IS A STACK?
 * A Stack is a linear data structure that follows the LIFO (Last In, First Out)
 * principle. Think of it like a stack of plates - you can only add or remove
 * plates from the top. The last plate you put on is the first one you take off.
 *
 * Key operations:
 * - push(x): Add element x to the top
 * - pop(): Remove and return the top element
 * - peek()/top(): Return the top element without removing it
 * - isEmpty(): Check if stack is empty
 * - size(): Return number of elements
 *
 * WHAT IS THIS PROBLEM?
 * Design and implement a stack data structure using a fixed-size array. You need
 * to handle all basic operations efficiently and manage edge cases like overflow
 * (pushing when full) and underflow (popping when empty).
 *
 * EXAMPLES:
 * - push(10), push(20), push(30) -> Stack: [10, 20, 30] (30 is top)
 * - pop() -> Returns 30, Stack: [10, 20]
 * - peek() -> Returns 20, Stack unchanged: [10, 20]
 * - isEmpty() -> Returns false
 * - size() -> Returns 2
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Foundation for many advanced algorithms (DFS, backtracking, expression parsing)
 * - Tests understanding of data structure internals vs just using library functions
 * - Common in embedded systems where dynamic memory allocation is avoided
 * - Shows ability to handle edge cases (overflow/underflow)
 * - Gateway to harder problems like Min Stack, stack-based parsing
 *
 * KEY CONCEPT:
 * Use an array with a 'top' index that tracks the position of the topmost element.
 * - top = -1 means empty stack
 * - top = MAX_SIZE - 1 means full stack
 * - Push: Increment top, then store element
 * - Pop: Get element at top, then decrement top
 *
 * VISUAL:
 *
 *   Initial         After push      After push      After pop
 *   (empty)         push(10)        push(20)        pop() -> 20
 *
 *   +-----+         +-----+         +-----+         +-----+
 *   |     | top=-1  |     |         | 20  | <-top   |     |
 *   +-----+         +-----+         +-----+         +-----+
 *   |     |         | 10  | <-top   | 10  |         | 10  | <-top
 *   +-----+         +-----+         +-----+         +-----+
 *   |     |         |     |         |     |         |     |
 *   +-----+         +-----+         +-----+         +-----+
 *
 *   top index:       top=0           top=1           top=0
 *   size:            size=1          size=2          size=1
 *
 * ============================================================================
 */

// Implement Stack using Array - ESSENTIAL data structure
// Time: O(1) for push/pop/peek, Space: O(n)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 100

// Say: "Stack is LIFO - Last In First Out"
typedef struct {
    int data[MAX_SIZE];
    int top;  // Say: "Index of top element, -1 when empty"
} Stack;

// Say: "Initialize stack with top = -1 (empty)"
void stack_init(Stack *s) {
    s->top = -1;
}

// Say: "Check if stack is empty - top is -1"
bool is_empty(Stack *s) {
    return s->top == -1;
}

// Say: "Check if stack is full - top reached max index"
bool is_full(Stack *s) {
    return s->top == MAX_SIZE - 1;
}

// Say: "Push - increment top, then store element"
bool push(Stack *s, int value) {
    // Say: "Always check for overflow before pushing"
    if (is_full(s)) {
        printf("Stack Overflow!\n");
        return false;
    }
    s->top++;                // Say: "First increment top"
    s->data[s->top] = value; // Say: "Then store at new top"
    return true;
}

// Say: "Pop - return element at top, then decrement"
int pop(Stack *s) {
    // Say: "Always check for underflow before popping"
    if (is_empty(s)) {
        printf("Stack Underflow!\n");
        return -1;  // Say: "Error indicator"
    }
    int value = s->data[s->top];  // Say: "Get value at top"
    s->top--;                      // Say: "Decrement top"
    return value;
}

// Say: "Peek - return top element without removing"
int peek(Stack *s) {
    if (is_empty(s)) {
        printf("Stack is empty!\n");
        return -1;
    }
    return s->data[s->top];  // Say: "Just read, don't modify top"
}

// Say: "Get number of elements"
int size(Stack *s) {
    return s->top + 1;  // Say: "top is index, size is top + 1"
}

int main() {
    Stack s;
    stack_init(&s);

    printf("Push: 10, 20, 30\n");
    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    printf("Top element: %d\n", peek(&s));
    printf("Stack size: %d\n", size(&s));

    printf("\nPop: %d\n", pop(&s));
    printf("Pop: %d\n", pop(&s));
    printf("Top after pops: %d\n", peek(&s));

    printf("\nIs empty: %s\n", is_empty(&s) ? "yes" : "no");

    printf("Pop last: %d\n", pop(&s));
    printf("Is empty: %s\n", is_empty(&s) ? "yes" : "no");

    // Test underflow
    printf("\nAttempt pop on empty stack:\n");
    pop(&s);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Stack is LIFO - Last In, First Out. Like a stack of plates.

 OPERATIONS:
 - push(x): Add element to top - O(1)
 - pop(): Remove and return top - O(1)
 - peek(): Return top without removing - O(1)
 - isEmpty(): Check if empty - O(1)
 - size(): Number of elements - O(1)

 IMPLEMENTATION CHOICES:
 1. Array-based (shown here):
    - Fixed size, simple implementation
    - top index tracks position
    - Must handle overflow

 2. Linked List-based:
    - Dynamic size
    - Push/pop at head
    - More memory per element (node overhead)

 WHY TOP = -1 FOR EMPTY:
 - top represents index of top element
 - When empty, no valid index, so -1
 - size = top + 1 always works

 COMMON MISTAKES:
 - Forgetting overflow/underflow checks
 - Wrong order: increment then store vs store then increment
 - Returning garbage on error instead of signaling

 EMBEDDED CONSIDERATIONS:
 - Fixed array avoids malloc overhead
 - Stack overflow can crash embedded systems
 - Often use circular buffer for efficiency

 STACK APPLICATIONS:
 1. Function call stack (local variables, return addresses)
 2. Expression evaluation (postfix, infix to postfix)
 3. Parentheses matching
 4. Undo functionality
 5. DFS traversal
 6. Backtracking algorithms"
*/
