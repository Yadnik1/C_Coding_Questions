/*
 * ============================================================================
 * PROBLEM: Implement Queue Using Two Stacks
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: High
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. TWO STACKS:
 *    - Stack1 for enqueue
 *    - Stack2 for dequeue
 *
 * 2. TRANSFER WHEN NEEDED:
 *    - Move all from Stack1 to Stack2 (reverses order)
 *    - Then pop from Stack2
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * Enqueue 1, 2, 3:
 *
 *   Stack1:     Stack2:
 *   +---+       +---+
 *   | 3 |       |   |
 *   +---+       +---+
 *   | 2 |       |   |
 *   +---+       +---+
 *   | 1 |       |   |
 *   +---+       +---+
 *
 * Dequeue (need 1):
 *   Transfer Stack1 → Stack2
 *
 *   Stack1:     Stack2:
 *   +---+       +---+
 *   |   |       | 1 | ← pop this!
 *   +---+       +---+
 *   |   |       | 2 |
 *   +---+       +---+
 *   |   |       | 3 |
 *   +---+       +---+
 *
 *   Pop from Stack2 → returns 1 ✓
 *
 * ============================================================================
 * TIME: Amortized O(1) for dequeue
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Define maximum stack size
// Say: "I'll define the maximum size for our stacks"
#define MAX_SIZE 100

// Stack structure
// Say: "I create a basic stack struct"
typedef struct {
    int arr[MAX_SIZE];  // Array to hold elements
    int top;            // Top index
} Stack;

// Initialize stack
// Say: "This initializes a stack to empty"
void initStack(Stack* s) {
    // Set top to -1
    // Say: "I set top to -1 for empty"
    s->top = -1;
}

// Check if stack is empty
// Say: "This checks if the stack is empty"
bool isEmpty(Stack* s) {
    // Return true if top is -1
    // Say: "I return true when top is -1"
    return s->top == -1;
}

// Push to stack
// Say: "This pushes a value onto the stack"
void push(Stack* s, int val) {
    // Increment top and store
    // Say: "I increment top and store the value"
    s->arr[++s->top] = val;
}

// Pop from stack
// Say: "This pops and returns the top value"
int pop(Stack* s) {
    // Return top value and decrement
    // Say: "I return the top value and decrement top"
    return s->arr[s->top--];
}

// Peek at top
// Say: "This peeks at the top value without removing it"
int peek(Stack* s) {
    // Return top value
    // Say: "I return the value at top"
    return s->arr[s->top];
}

// Queue structure using two stacks
// Say: "Now I create a Queue struct that uses two stacks"
typedef struct {
    Stack s1;  // Stack for enqueue operations
    Stack s2;  // Stack for dequeue operations
} Queue;

// Initialize queue
// Say: "This initializes the queue by initializing both stacks"
void initQueue(Queue* q) {
    // Initialize stack 1
    // Say: "I initialize stack 1"
    initStack(&q->s1);

    // Initialize stack 2
    // Say: "And I initialize stack 2"
    initStack(&q->s2);
}

// Enqueue operation
// Say: "Now I'll implement enqueue which adds to the back of the queue"
void enqueue(Queue* q, int val) {
    // Simply push to stack 1
    // Say: "I just push the value onto stack 1 - that's it for enqueue"
    push(&q->s1, val);
}

// Dequeue operation
// Say: "Now I'll implement dequeue which removes from the front"
int dequeue(Queue* q) {
    // Check if stack 2 is empty
    // Say: "First I check if stack 2 is empty"
    if (isEmpty(&q->s2)) {
        // Transfer all elements from s1 to s2
        // Say: "If it's empty, I need to transfer all elements from s1 to s2"
        while (!isEmpty(&q->s1)) {
            // Pop from s1 and push to s2
            // Say: "I pop from s1 and push to s2, which reverses the order"
            push(&q->s2, pop(&q->s1));
        }
    }

    // Check if both stacks are empty
    // Say: "Now I check if s2 is still empty, meaning the queue is empty"
    if (isEmpty(&q->s2)) {
        // Queue is empty
        // Say: "If s2 is empty, the queue has no elements, so I print error"
        printf("Queue empty!\n");
        return -1;
    }

    // Pop from stack 2
    // Say: "I pop from s2, which gives us the front element"
    return pop(&q->s2);
}

// Get front element
// Say: "This function returns the front element without removing it"
int front(Queue* q) {
    // Check if s2 is empty
    // Say: "If s2 is empty, I need to transfer from s1"
    if (isEmpty(&q->s2)) {
        // Transfer from s1 to s2
        // Say: "I transfer all elements from s1 to s2"
        while (!isEmpty(&q->s1)) {
            // Pop and push
            // Say: "Popping from s1 and pushing to s2"
            push(&q->s2, pop(&q->s1));
        }
    }

    // Return front if not empty
    // Say: "If s2 is not empty, I peek at the top"
    return isEmpty(&q->s2) ? -1 : peek(&q->s2);
}

int main() {
    // Say: "Let me demonstrate a queue implemented using two stacks"
    printf("=== Queue Using Two Stacks ===\n\n");

    // Declare queue
    // Say: "I declare a queue variable"
    Queue q;

    // Initialize
    // Say: "I initialize the queue"
    initQueue(&q);

    // Enqueue elements
    // Say: "Let me enqueue 1, 2, and 3"
    printf("Enqueue: 1, 2, 3\n");
    enqueue(&q, 1);  // Enqueue 1
    enqueue(&q, 2);  // Enqueue 2
    enqueue(&q, 3);  // Enqueue 3

    // Dequeue first element
    // Say: "Now I'll dequeue, which should return 1"
    printf("Dequeue: %d\n", dequeue(&q));

    // Dequeue second element
    // Say: "I dequeue again, which should return 2"
    printf("Dequeue: %d\n", dequeue(&q));

    // Enqueue more elements
    // Say: "Let me enqueue two more: 4 and 5"
    printf("\nEnqueue: 4, 5\n");
    enqueue(&q, 4);  // Enqueue 4
    enqueue(&q, 5);  // Enqueue 5

    // Dequeue remaining
    // Say: "Now I'll dequeue all remaining elements"
    printf("Dequeue: %d\n", dequeue(&q));  // Should be 3
    printf("Dequeue: %d\n", dequeue(&q));  // Should be 4
    printf("Dequeue: %d\n", dequeue(&q));  // Should be 5

    return 0;
}
