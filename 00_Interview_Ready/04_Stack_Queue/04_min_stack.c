/*
 * ============================================================================
 * PROBLEM: Min Stack - Get Minimum in O(1) Time
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Design a stack that supports push, pop, top, and retrieving the minimum
 * element, ALL in constant O(1) time. The challenge is that when you pop
 * elements, the minimum might change, so you need to track the minimum at
 * each state of the stack.
 *
 * Operations to implement:
 * - push(x): Push element x onto stack - O(1)
 * - pop(): Remove the top element - O(1)
 * - top(): Get the top element - O(1)
 * - getMin(): Retrieve the minimum element - O(1)
 *
 * EXAMPLES:
 * - push(5)  -> Stack: [5],       Min: 5
 * - push(3)  -> Stack: [5,3],     Min: 3
 * - push(7)  -> Stack: [5,3,7],   Min: 3
 * - push(2)  -> Stack: [5,3,7,2], Min: 2
 * - getMin() -> Returns 2
 * - pop()    -> Stack: [5,3,7],   Min: 3
 * - getMin() -> Returns 3 (minimum changed after pop!)
 * - pop()    -> Stack: [5,3],     Min: 3
 * - pop()    -> Stack: [5],       Min: 5
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Tests creative thinking about auxiliary data structures
 * - Common pattern: trading space for time complexity
 * - Foundation for more complex problems (Max Stack, MinQueue)
 * - Shows understanding that stack state can be "saved" at each level
 * - Popular at FAANG companies (Google, Amazon, Microsoft)
 *
 * KEY CONCEPT:
 * Maintain TWO parallel stacks:
 * 1. Main data stack - stores actual values
 * 2. Min stack - stores the minimum value at each level
 *
 * When pushing: min[top] = minimum of (new value, previous min)
 * When popping: both stacks shrink together, revealing previous min
 *
 * VISUAL:
 *
 *   Push sequence: 5, 3, 7, 2
 *
 *   Data Stack:     Min Stack:       getMin()
 *   +---+           +---+
 *   | 2 | <- top    | 2 | <- top     returns 2
 *   +---+           +---+
 *   | 7 |           | 3 |            (min at this level was 3)
 *   +---+           +---+
 *   | 3 |           | 3 |
 *   +---+           +---+
 *   | 5 |           | 5 |
 *   +---+           +---+
 *
 *   After pop() - remove 2:
 *
 *   Data Stack:     Min Stack:       getMin()
 *   +---+           +---+
 *   | 7 | <- top    | 3 | <- top     returns 3 (previous min restored!)
 *   +---+           +---+
 *   | 3 |           | 3 |
 *   +---+           +---+
 *   | 5 |           | 5 |
 *   +---+           +---+
 *
 *   The key insight: min[i] remembers what the minimum was
 *   when the stack had exactly i+1 elements!
 *
 * ============================================================================
 */

// Min Stack - Get minimum element in O(1) time
// Time: O(1) for all operations, Space: O(n)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_SIZE 100

// Say: "I use two stacks - one for data, one to track minimums"
typedef struct {
    int data[MAX_SIZE];    // Say: "Main data stack"
    int min[MAX_SIZE];     // Say: "Auxiliary stack tracking min at each level"
    int top;
} MinStack;

void min_stack_init(MinStack *s) {
    s->top = -1;
}

bool is_empty(MinStack *s) {
    return s->top == -1;
}

bool is_full(MinStack *s) {
    return s->top == MAX_SIZE - 1;
}

void push(MinStack *s, int value) {
    if (is_full(s)) {
        printf("Stack Overflow!\n");
        return;
    }

    s->top++;
    s->data[s->top] = value;

    // Say: "Track minimum at each level"
    if (s->top == 0) {
        // Say: "First element is the minimum"
        s->min[s->top] = value;
    } else {
        // Say: "New min is smaller of: current value or previous min"
        int prev_min = s->min[s->top - 1];
        s->min[s->top] = (value < prev_min) ? value : prev_min;
    }
}

int pop(MinStack *s) {
    if (is_empty(s)) {
        printf("Stack Underflow!\n");
        return INT_MIN;
    }

    int value = s->data[s->top];
    s->top--;  // Say: "Both stacks shrink together"
    return value;
}

int peek(MinStack *s) {
    if (is_empty(s)) {
        return INT_MIN;
    }
    return s->data[s->top];
}

// Say: "Get minimum in O(1) - just read min stack top"
int get_min(MinStack *s) {
    if (is_empty(s)) {
        printf("Stack is empty!\n");
        return INT_MIN;
    }
    return s->min[s->top];
}

int main() {
    MinStack s;
    min_stack_init(&s);

    printf("Push 5:\n");
    push(&s, 5);
    printf("  Min: %d, Top: %d\n", get_min(&s), peek(&s));

    printf("Push 3:\n");
    push(&s, 3);
    printf("  Min: %d, Top: %d\n", get_min(&s), peek(&s));

    printf("Push 7:\n");
    push(&s, 7);
    printf("  Min: %d, Top: %d\n", get_min(&s), peek(&s));

    printf("Push 2:\n");
    push(&s, 2);
    printf("  Min: %d, Top: %d\n", get_min(&s), peek(&s));

    printf("Push 8:\n");
    push(&s, 8);
    printf("  Min: %d, Top: %d\n", get_min(&s), peek(&s));

    printf("\n--- Pop operations ---\n");
    printf("Pop: %d, Min after: %d\n", pop(&s), get_min(&s));
    printf("Pop: %d, Min after: %d\n", pop(&s), get_min(&s));
    printf("Pop: %d, Min after: %d\n", pop(&s), get_min(&s));
    printf("Pop: %d, Min after: %d\n", pop(&s), get_min(&s));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I maintain an auxiliary stack that tracks the minimum at each level.

 APPROACH:
 - Two parallel arrays: data[] and min[]
 - min[i] stores the minimum of all elements from 0 to i
 - When pushing: min[top] = min(value, min[top-1])
 - When popping: both stacks shrink together
 - get_min(): just return min[top] in O(1)

 WHY THIS WORKS:
 - At any stack state, min[top] holds current minimum
 - As we pop, we go back to previous state's minimum
 - No need to search through stack

 EXAMPLE:
 Push sequence: 5, 3, 7, 2, 8
 data: [5, 3, 7, 2, 8]
 min:  [5, 3, 3, 2, 2]

 Pop 8: min = 2
 Pop 2: min = 3
 Pop 7: min = 3
 Pop 3: min = 5

 ALTERNATIVE - Single stack with encoding:
 - Store (value, current_min) as pair
 - Or use math trick: push 2*value - min when new min found

 SPACE OPTIMIZATION:
 - Only push to min stack when new minimum found
 - Pop from min stack only when popping current minimum
 - Reduces space in practice but worst case still O(n)

 COMMON MISTAKES:
 - Not handling empty stack in get_min
 - Wrong comparison when pushing (> vs <)
 - Forgetting to update min stack on pop

 TIME/SPACE:
 - All operations O(1) time
 - O(n) extra space for min stack"
*/
