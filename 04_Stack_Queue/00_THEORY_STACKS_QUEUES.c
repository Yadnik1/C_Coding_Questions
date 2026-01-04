/*
 * ============================================================================
 *                    STACKS AND QUEUES - COMPLETE THEORY
 * ============================================================================
 *
 * This file contains everything you need to know about Stacks and Queues
 * for embedded/firmware interviews.
 *
 * ============================================================================
 *                              STACK
 * ============================================================================
 *
 * WHAT IS A STACK?
 * ----------------
 * A stack is a linear data structure that follows LIFO principle.
 *
 * LIFO = Last In, First Out
 *
 * Real-world examples:
 *   - Stack of plates (take from top, add to top)
 *   - Stack of books
 *   - Undo operation in text editor
 *   - Browser back button
 *   - Function call stack in programs!
 *
 * ============================================================================
 * STACK VISUALIZATION:
 * ============================================================================
 *
 *     +-------+
 *     |  30   |  <-- TOP (Last added, first to remove)
 *     +-------+
 *     |  20   |
 *     +-------+
 *     |  10   |  <-- BOTTOM (First added, last to remove)
 *     +-------+
 *
 *     PUSH 40:           POP:
 *     +-------+          +-------+
 *     |  40   | <--NEW   |       |  (30 removed)
 *     +-------+          +-------+
 *     |  30   |          |  20   | <--NEW TOP
 *     +-------+          +-------+
 *     |  20   |          |  10   |
 *     +-------+          +-------+
 *     |  10   |
 *     +-------+
 *
 * ============================================================================
 * STACK OPERATIONS:
 * ============================================================================
 *
 *   Operation   | Description              | Time
 *   ------------|--------------------------|------
 *   push(x)     | Add element to top       | O(1)
 *   pop()       | Remove element from top  | O(1)
 *   peek/top()  | View top element         | O(1)
 *   isEmpty()   | Check if stack empty     | O(1)
 *   isFull()    | Check if stack full      | O(1)
 *
 * ============================================================================
 * STACK IMPLEMENTATION USING ARRAY:
 * ============================================================================
 *
 *   Array:  [10, 20, 30, _, _, _]
 *   Index:    0   1   2  3  4  5
 *                     ^
 *                   top = 2
 *
 *   - top = -1 means stack is empty
 *   - top = MAX_SIZE - 1 means stack is full
 *   - Push: top++, then arr[top] = value
 *   - Pop: return arr[top], then top--
 *
 * ============================================================================
 * STACK IN EMBEDDED SYSTEMS - WHY IT MATTERS:
 * ============================================================================
 *
 * 1. FUNCTION CALL STACK:
 *    - Every function call pushes return address
 *    - Local variables stored on stack
 *    - Stack overflow = crash!
 *
 *    void funcA() {
 *        funcB();  // Push return address
 *    }
 *    void funcB() {
 *        funcC();  // Push return address
 *    }
 *
 *    Stack during funcC():
 *    +------------------+
 *    | funcC locals     |
 *    +------------------+
 *    | return to funcB  |
 *    +------------------+
 *    | funcB locals     |
 *    +------------------+
 *    | return to funcA  |
 *    +------------------+
 *    | funcA locals     |
 *    +------------------+
 *
 * 2. INTERRUPT HANDLING:
 *    - CPU pushes registers on stack before ISR
 *    - Pops them after ISR returns
 *    - Nested interrupts = deeper stack
 *
 * 3. EXPRESSION EVALUATION:
 *    - Compilers use stacks for parsing
 *    - Postfix evaluation uses stack
 *
 * ============================================================================
 * COMMON STACK INTERVIEW QUESTIONS:
 * ============================================================================
 *
 * 1. Balanced Parentheses - VERY COMMON
 * 2. Reverse a string using stack
 * 3. Implement queue using two stacks
 * 4. Min stack (get minimum in O(1))
 * 5. Next greater element
 * 6. Implement stack using linked list
 *
 * ============================================================================
 * STACK OVERFLOW vs UNDERFLOW:
 * ============================================================================
 *
 * OVERFLOW:
 *   - Trying to push when stack is full
 *   - In embedded: can corrupt memory!
 *   - Prevention: always check before push
 *
 * UNDERFLOW:
 *   - Trying to pop when stack is empty
 *   - Returns garbage or crashes
 *   - Prevention: always check isEmpty()
 *
 * ============================================================================
 *                              QUEUE
 * ============================================================================
 *
 * WHAT IS A QUEUE?
 * ----------------
 * A queue is a linear data structure that follows FIFO principle.
 *
 * FIFO = First In, First Out
 *
 * Real-world examples:
 *   - Line at a ticket counter
 *   - Print job queue
 *   - Message queues in RTOS
 *   - Keyboard buffer
 *   - Network packet queue
 *
 * ============================================================================
 * QUEUE VISUALIZATION:
 * ============================================================================
 *
 *   FRONT                              REAR
 *     |                                  |
 *     v                                  v
 *   +----+----+----+----+----+----+----+----+
 *   | 10 | 20 | 30 | 40 |    |    |    |    |
 *   +----+----+----+----+----+----+----+----+
 *     ^                   ^
 *   DEQUEUE             ENQUEUE
 *   (remove)            (add)
 *
 *   First In (10) = First Out
 *
 * ============================================================================
 * QUEUE OPERATIONS:
 * ============================================================================
 *
 *   Operation   | Description              | Time
 *   ------------|--------------------------|------
 *   enqueue(x)  | Add element to rear      | O(1)
 *   dequeue()   | Remove element from front| O(1)
 *   front()     | View front element       | O(1)
 *   isEmpty()   | Check if queue empty     | O(1)
 *   isFull()    | Check if queue full      | O(1)
 *
 * ============================================================================
 * LINEAR QUEUE PROBLEM:
 * ============================================================================
 *
 * Simple linear queue wastes space:
 *
 *   After some enqueue/dequeue:
 *   +----+----+----+----+----+
 *   |    |    | 30 | 40 | 50 |
 *   +----+----+----+----+----+
 *     ^    ^    ^              ^
 *   wasted  front            rear
 *
 *   Positions 0,1 are wasted even though queue isn't "full"!
 *
 * ============================================================================
 * CIRCULAR QUEUE - THE SOLUTION:
 * ============================================================================
 *
 * Wrap around using modulo!
 *
 *   Logical view:
 *         +---+
 *        /  0  \
 *       4       1
 *       |       |
 *       3-------2
 *
 *   After rear reaches end, it wraps to 0:
 *     rear = (rear + 1) % SIZE
 *     front = (front + 1) % SIZE
 *
 *   Example with SIZE=5:
 *   +----+----+----+----+----+
 *   | 60 |    | 30 | 40 | 50 |
 *   +----+----+----+----+----+
 *     ^    ^    ^
 *   rear  empty front
 *
 *   rear wrapped around to index 0!
 *
 * ============================================================================
 * QUEUE IN EMBEDDED SYSTEMS - WHY IT MATTERS:
 * ============================================================================
 *
 * 1. RTOS MESSAGE QUEUES:
 *    - Tasks communicate via queues
 *    - Producer-consumer pattern
 *    - Thread-safe with proper locking
 *
 *    Task A (Producer)     Queue        Task B (Consumer)
 *        |                  |                |
 *        |---enqueue(msg)-->|                |
 *        |                  |---dequeue()--->|
 *
 * 2. UART/SERIAL BUFFERS:
 *    - Receive buffer is a queue
 *    - ISR enqueues, main loop dequeues
 *    - Prevents data loss
 *
 *    UART ISR          RX Buffer        Main Loop
 *       |                 |                |
 *       |--enqueue(byte)->|                |
 *       |                 |--dequeue()--->|
 *
 * 3. EVENT QUEUES:
 *    - Button presses, sensor events
 *    - Process in order received
 *
 * 4. DMA DESCRIPTORS:
 *    - Chain of transfer descriptors
 *    - Hardware processes in FIFO order
 *
 * ============================================================================
 * TYPES OF QUEUES:
 * ============================================================================
 *
 * 1. LINEAR QUEUE:
 *    - Simple, but wastes space
 *    - Rarely used in practice
 *
 * 2. CIRCULAR QUEUE:
 *    - Efficient use of space
 *    - Most common in embedded
 *
 * 3. PRIORITY QUEUE:
 *    - Elements have priority
 *    - Higher priority dequeued first
 *    - Used in RTOS schedulers
 *
 * 4. DOUBLE-ENDED QUEUE (DEQUE):
 *    - Insert/remove from both ends
 *    - Combines stack and queue
 *
 * ============================================================================
 * COMMON QUEUE INTERVIEW QUESTIONS:
 * ============================================================================
 *
 * 1. Implement queue using array (circular)
 * 2. Implement queue using two stacks
 * 3. Implement stack using two queues
 * 4. Sliding window maximum
 * 5. First non-repeating character in stream
 *
 * ============================================================================
 * STACK vs QUEUE COMPARISON:
 * ============================================================================
 *
 *   Feature        | Stack          | Queue
 *   ---------------|----------------|----------------
 *   Order          | LIFO           | FIFO
 *   Insert         | Push (top)     | Enqueue (rear)
 *   Remove         | Pop (top)      | Dequeue (front)
 *   Access point   | One end (top)  | Two ends
 *   Use case       | Undo, parsing  | Scheduling, buffers
 *   Embedded use   | Call stack     | Message queues
 *
 * ============================================================================
 * KEY FORMULAS FOR CIRCULAR QUEUE:
 * ============================================================================
 *
 *   // Move to next position (with wrap-around)
 *   next_rear = (rear + 1) % SIZE
 *   next_front = (front + 1) % SIZE
 *
 *   // Check if full (one slot always empty as sentinel)
 *   isFull = (rear + 1) % SIZE == front
 *
 *   // Check if empty
 *   isEmpty = front == rear
 *
 *   // Number of elements
 *   count = (rear - front + SIZE) % SIZE
 *
 * ============================================================================
 * INTERVIEW TIP:
 * ============================================================================
 *
 * When asked about stacks/queues in embedded context, mention:
 *
 * 1. "Stack is used for function calls and interrupt handling"
 * 2. "Queue is used for RTOS message passing and UART buffers"
 * 3. "Circular queue is preferred to avoid wasted space"
 * 4. "Need to handle overflow/underflow to prevent crashes"
 * 5. "In multi-threaded, need mutex/semaphore for thread safety"
 *
 * ============================================================================
 * SEARCHING & SORTING - WHAT YOU REALLY NEED:
 * ============================================================================
 *
 * FOR EMBEDDED/FIRMWARE INTERVIEWS:
 *
 * MUST KNOW:
 *   1. Binary Search - O(log n), most asked!
 *   2. One simple sort (Insertion or Selection) - O(n²)
 *
 * GOOD TO KNOW (concepts only):
 *   3. Quick Sort - O(n log n) average
 *   4. Merge Sort - stable, used in libraries
 *
 * RARELY ASKED:
 *   - Heap Sort, Radix Sort, Counting Sort
 *   - These are more for CS interviews
 *
 * WHY BINARY SEARCH IS CRITICAL:
 *   - Lookup tables in embedded (ADC calibration, etc.)
 *   - Finding values in sorted sensor data
 *   - Configuration parameter search
 *
 * WHY SIMPLE SORTS ARE ENOUGH:
 *   - Small data sets in embedded (< 100 elements usually)
 *   - O(n²) is fine for small n
 *   - Simple to implement, less bug-prone
 *   - Insertion sort is O(n) for nearly sorted data!
 *
 * ============================================================================
 */

// This is a theory file - see other files for implementations
// Key files to study:
//   01_stack_using_array.c
//   02_queue_using_array.c
//   04_balanced_parentheses.c (most common interview question!)

#include <stdio.h>

int main() {
    // Say: "Let me demonstrate the key concepts of stacks and queues"
    printf("=== Stack & Queue Theory ===\n\n");

    // Explain stack operations
    // Say: "A stack follows LIFO - Last In, First Out principle"
    printf("STACK (LIFO):\n");
    printf("  - Push: Add to top\n");
    printf("  - Pop: Remove from top\n");
    printf("  - Used for: Function calls, undo, parsing\n\n");

    // Explain queue operations
    // Say: "A queue follows FIFO - First In, First Out principle"
    printf("QUEUE (FIFO):\n");
    printf("  - Enqueue: Add to rear\n");
    printf("  - Dequeue: Remove from front\n");
    printf("  - Used for: Buffers, message queues, scheduling\n\n");

    // Explain embedded context
    // Say: "In embedded systems, these are critical for system operation"
    printf("EMBEDDED CONTEXT:\n");
    printf("  - Stack: Call stack, interrupt handling\n");
    printf("  - Queue: UART buffers, RTOS messages\n\n");

    // Direct user to implementation files
    // Say: "See the individual problem files for actual implementations"
    printf("See individual problem files for implementations!\n");

    return 0;
}
