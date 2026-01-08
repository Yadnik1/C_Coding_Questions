/*
 * ============================================================================
 * PROBLEM: Implement Queue using Two Stacks
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Implement a FIFO (First-In-First-Out) queue using only two LIFO stacks.
 * You can only use standard stack operations: push, pop, peek, isEmpty.
 * The challenge is to achieve queue behavior (FIFO) using stack behavior (LIFO).
 *
 * EXAMPLES:
 * - enqueue(1), enqueue(2), enqueue(3)
 *   Input Stack: [1, 2, 3] (3 on top)
 *   Output Stack: []
 *
 * - dequeue() -> Returns 1 (first in, first out!)
 *   How? Transfer all to output: [3, 2, 1] (1 on top), then pop
 *   Input Stack: []
 *   Output Stack: [3, 2] (2 on top now)
 *
 * - enqueue(4)
 *   Input Stack: [4]
 *   Output Stack: [3, 2]
 *
 * - dequeue() -> Returns 2 (from output stack, no transfer needed)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Classic data structure transformation problem
 * - Tests understanding of amortized time complexity analysis
 * - Shows creative thinking - using constraints to achieve different behavior
 * - Common at Google, Amazon, Meta interviews
 * - Inverse problem (Stack using Queues) is also popular
 * - Real application: certain systems only provide stack-like interfaces
 *
 * KEY CONCEPT:
 * Use two stacks: "input" for enqueue, "output" for dequeue.
 * - Enqueue: Always push to input stack - O(1)
 * - Dequeue: Pop from output stack. If output is empty, transfer ALL
 *            elements from input to output first (this reverses order!)
 *
 * The trick: Reversing twice = original order!
 * Input->Output reverses. Popping from Output reverses again = FIFO!
 *
 * Amortized O(1): Each element is pushed/popped at most twice total.
 *
 * VISUAL:
 *
 *   Step 1: enqueue(1), enqueue(2), enqueue(3)
 *
 *   INPUT STACK      OUTPUT STACK
 *   +---+            +---+
 *   | 3 | <- top     |   | (empty)
 *   +---+            +---+
 *   | 2 |
 *   +---+
 *   | 1 |
 *   +---+
 *
 *   Step 2: dequeue() - output empty, so TRANSFER first
 *
 *   Pop 3, push to output     Pop 2, push       Pop 1, push
 *
 *   INPUT    OUTPUT           INPUT  OUTPUT     INPUT  OUTPUT
 *   +---+    +---+            +---+  +---+      +---+  +---+
 *   | 2 |    | 3 |            |   |  | 2 |      |   |  | 1 | <- pop this!
 *   +---+    +---+            +---+  +---+      +---+  +---+
 *   | 1 |                            | 3 |             | 2 |
 *   +---+                            +---+             +---+
 *                                                      | 3 |
 *                                                      +---+
 *
 *   Now pop from OUTPUT -> Returns 1 (FIFO achieved!)
 *
 *   Step 3: enqueue(4) - just push to INPUT
 *
 *   INPUT      OUTPUT
 *   +---+      +---+
 *   | 4 |      | 2 | <- next dequeue returns 2
 *   +---+      +---+
 *              | 3 |
 *              +---+
 *
 * ============================================================================
 */

// Implement Queue using Two Stacks - Popular interview question
// Time: Amortized O(1) per operation, Space: O(n)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;

void stack_init(Stack *s) { s->top = -1; }
bool stack_empty(Stack *s) { return s->top == -1; }
bool stack_full(Stack *s) { return s->top == MAX_SIZE - 1; }
void stack_push(Stack *s, int val) { s->data[++s->top] = val; }
int stack_pop(Stack *s) { return s->data[s->top--]; }
int stack_peek(Stack *s) { return s->data[s->top]; }

// Say: "Queue using two stacks - one for input, one for output"
typedef struct {
    Stack input;   // Say: "Stack for enqueue operations"
    Stack output;  // Say: "Stack for dequeue operations"
} MyQueue;

void queue_init(MyQueue *q) {
    stack_init(&q->input);
    stack_init(&q->output);
}

// Say: "Always push to input stack - O(1)"
void enqueue(MyQueue *q, int value) {
    if (stack_full(&q->input)) {
        printf("Queue full!\n");
        return;
    }
    stack_push(&q->input, value);
}

// Say: "Transfer from input to output if output empty"
void transfer(MyQueue *q) {
    // Say: "Only transfer when output is empty"
    if (stack_empty(&q->output)) {
        // Say: "Move all elements from input to output"
        // Say: "This reverses the order, giving FIFO behavior"
        while (!stack_empty(&q->input)) {
            stack_push(&q->output, stack_pop(&q->input));
        }
    }
}

// Say: "Dequeue from output stack - amortized O(1)"
int dequeue(MyQueue *q) {
    transfer(q);  // Say: "Ensure output has elements"

    if (stack_empty(&q->output)) {
        printf("Queue empty!\n");
        return -1;
    }

    return stack_pop(&q->output);
}

// Say: "Peek at front element"
int peek(MyQueue *q) {
    transfer(q);

    if (stack_empty(&q->output)) {
        printf("Queue empty!\n");
        return -1;
    }

    return stack_peek(&q->output);
}

bool is_empty(MyQueue *q) {
    return stack_empty(&q->input) && stack_empty(&q->output);
}

int main() {
    MyQueue q;
    queue_init(&q);

    printf("Enqueue: 1, 2, 3\n");
    enqueue(&q, 1);
    enqueue(&q, 2);
    enqueue(&q, 3);

    printf("Dequeue: %d\n", dequeue(&q));  // Should be 1
    printf("Peek: %d\n", peek(&q));        // Should be 2

    printf("\nEnqueue: 4, 5\n");
    enqueue(&q, 4);
    enqueue(&q, 5);

    printf("Dequeue all:\n");
    while (!is_empty(&q)) {
        printf("  %d\n", dequeue(&q));  // 2, 3, 4, 5
    }

    printf("\nDequeue from empty:\n");
    dequeue(&q);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use two stacks: input stack for enqueue, output stack for dequeue.

 KEY INSIGHT:
 - Stack reverses order (LIFO)
 - Two reversals = original order (FIFO)
 - Push to input -> Transfer to output -> Pop from output

 OPERATIONS:
 - Enqueue: Always push to input stack - O(1)
 - Dequeue: Pop from output stack - O(1) amortized
            If output empty, transfer all from input first

 WHY AMORTIZED O(1):
 - Each element is pushed/popped at most twice
 - Once to input, once to output
 - Total: 2n operations for n elements = O(1) per element

 EXAMPLE:
 Enqueue 1,2,3:
   input:  [1,2,3] (3 on top)
   output: []

 Dequeue (first time):
   Transfer: pop 3,2,1 from input, push to output
   input:  []
   output: [3,2,1] (1 on top)
   Pop from output: return 1

 ALTERNATIVE - O(1) enqueue, O(n) dequeue:
 - Transfer on every dequeue
 - Simpler but inefficient

 ALTERNATIVE - O(n) enqueue, O(1) dequeue:
 - Transfer on every enqueue
 - Keep output always ready

 COMMON MISTAKES:
 - Transferring even when output not empty
 - Forgetting to check both stacks for empty
 - Not understanding amortized analysis

 FOLLOW-UP: Stack using two queues?
 - Possible but less efficient
 - Either O(n) push or O(n) pop"
*/
