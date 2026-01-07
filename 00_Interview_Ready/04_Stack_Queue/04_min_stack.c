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
