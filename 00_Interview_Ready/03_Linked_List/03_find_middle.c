/*
 * ============================================================================
 * PROBLEM: Find the Middle Node of a Linked List
 * ============================================================================
 *
 * WHAT IS A LINKED LIST? (Quick Recap)
 * A chain of nodes where each node contains data and a pointer to the next
 * node. Unlike arrays, we cannot access the middle element directly - we
 * must traverse from the head.
 *
 * WHAT IS THIS PROBLEM?
 * Given the head of a singly linked list, return the middle node.
 * If there are two middle nodes (even length), return the SECOND middle.
 *
 * EXAMPLES:
 *   Input:  1 -> 2 -> 3 -> 4 -> 5 (odd length: 5 nodes)
 *   Output: Node with value 3
 *                    ^
 *                  middle
 *
 *   Input:  1 -> 2 -> 3 -> 4 -> 5 -> 6 (even length: 6 nodes)
 *   Output: Node with value 4 (second middle)
 *                       ^
 *             first    second
 *             middle   middle
 *
 *   Input:  1 (single node)
 *   Output: Node with value 1 (it's the middle)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 *   - Foundation for many problems: merge sort, palindrome check
 *   - Tests the elegant slow/fast pointer technique
 *   - Simple but shows understanding of pointer manipulation
 *   - Often combined with other operations (reverse, split)
 *   - Interviewer may ask: "What about first middle for even length?"
 *
 * KEY CONCEPT: Slow/Fast Pointer (Tortoise and Hare)
 *   Use two pointers starting at head:
 *   - Slow: Moves 1 step per iteration
 *   - Fast: Moves 2 steps per iteration
 *
 *   When fast reaches the end, slow is at the middle!
 *   Why? Fast covers twice the distance, so slow covers half.
 *
 * VISUAL:
 *
 *   Odd length: 1 -> 2 -> 3 -> 4 -> 5
 *
 *   Start:  S,F at 1
 *   Step 1: S at 2, F at 3
 *   Step 2: S at 3, F at 5
 *   Step 3: F->next is NULL, STOP
 *   Result: S = 3 (middle)
 *
 *   Even length: 1 -> 2 -> 3 -> 4 -> 5 -> 6
 *
 *   Start:  S,F at 1
 *   Step 1: S at 2, F at 3
 *   Step 2: S at 3, F at 5
 *   Step 3: S at 4, F at NULL (past end)
 *   Result: S = 4 (second middle)
 *
 *   +---+    +---+    +---+    +---+    +---+    +---+
 *   | 1 | -> | 2 | -> | 3 | -> | 4 | -> | 5 | -> | 6 | -> NULL
 *   +---+    +---+    +---+    +---+    +---+    +---+
 *                       ^        ^
 *                    1st mid  2nd mid (returned)
 *
 * VARIANT: First Middle for Even Length
 *   Change loop condition to check fast->next->next
 *   This stops slow one position earlier
 *
 * ============================================================================
 */

/*
 * FIND MIDDLE OF LINKED LIST - Slow/Fast Pointer
 *
 * Time Complexity: O(n) - Single pass (fast covers n, slow covers n/2)
 * Space Complexity: O(1) - Only two pointers
 *
 * Pattern: Slow/Fast Pointer Technique
 */

#include <stdio.h>
#include <stdlib.h>

/* ==================== HELPER FUNCTIONS ==================== */

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

void print_list(Node* head) {
    printf("[");
    while (head) {
        printf("%d", head->data);
        if (head->next) printf(" -> ");
        head = head->next;
    }
    printf("]\n");
}

/* ==================== SOLUTION ==================== */

/*
 * ============================================================================
 * DRY-RUN DIAGRAM: Find Middle of Linked List
 * ============================================================================
 *
 * EXAMPLE 1: Odd Length (5 nodes): 1 -> 2 -> 3 -> 4 -> 5
 * ======================================================
 *
 * INITIAL STATE:
 *
 *   slow, fast
 *       |
 *       v
 *      [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 1:
 * ----------------------------------------------------------------------------
 *   Check: fast != NULL (true) && fast->next != NULL (true) -> enter loop
 *   slow = slow->next (move to 2)
 *   fast = fast->next->next (move to 3)
 *
 *              slow          fast
 *               |             |
 *               v             v
 *      [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 2:
 * ----------------------------------------------------------------------------
 *   Check: fast != NULL (true) && fast->next != NULL (true) -> enter loop
 *   slow = slow->next (move to 3)
 *   fast = fast->next->next (move to 5)
 *
 *                      slow            fast
 *                       |               |
 *                       v               v
 *      [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 3:
 * ----------------------------------------------------------------------------
 *   Check: fast != NULL (true) && fast->next != NULL (false - 5->next is NULL)
 *   LOOP EXITS
 *
 *   slow is at [3] -> Return slow (middle node!)
 *
 *                      slow
 *                       |
 *                       v
 *      [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *                       ^
 *                    MIDDLE!
 *
 * ============================================================================
 * EXAMPLE 2: Even Length (6 nodes): 1 -> 2 -> 3 -> 4 -> 5 -> 6
 * ============================================================
 *
 * INITIAL STATE:
 *
 *   slow, fast
 *       |
 *       v
 *      [1] --> [2] --> [3] --> [4] --> [5] --> [6] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 1:
 * ----------------------------------------------------------------------------
 *   slow = slow->next (move to 2)
 *   fast = fast->next->next (move to 3)
 *
 *              slow          fast
 *               |             |
 *               v             v
 *      [1] --> [2] --> [3] --> [4] --> [5] --> [6] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 2:
 * ----------------------------------------------------------------------------
 *   slow = slow->next (move to 3)
 *   fast = fast->next->next (move to 5)
 *
 *                      slow            fast
 *                       |               |
 *                       v               v
 *      [1] --> [2] --> [3] --> [4] --> [5] --> [6] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 3:
 * ----------------------------------------------------------------------------
 *   slow = slow->next (move to 4)
 *   fast = fast->next->next (move to NULL - past end)
 *
 *                              slow                    fast
 *                               |                       |
 *                               v                       v
 *      [1] --> [2] --> [3] --> [4] --> [5] --> [6] --> NULL
 *
 *   Check: fast != NULL (false) -> LOOP EXITS
 *
 *   slow is at [4] -> Return slow (SECOND middle!)
 *
 *      [1] --> [2] --> [3] --> [4] --> [5] --> [6] --> NULL
 *                       ^       ^
 *                    1st mid  2nd mid (returned)
 *
 * ============================================================================
 * WHY IT WORKS:
 * ============================================================================
 *
 *   - fast moves at 2x the speed of slow
 *   - When fast reaches the end:
 *     - fast has traveled n nodes
 *     - slow has traveled n/2 nodes
 *   - slow is at the middle!
 *
 *   For n=5: fast at position 5 (end), slow at position 3 (middle)
 *   For n=6: fast at position NULL (past end), slow at position 4 (2nd middle)
 *
 * ============================================================================
 */

// Returns second middle for even-length lists
Node* find_middle(Node* head) {
    // Say: "I use slow/fast pointers - when fast reaches end, slow is at middle"
    if (head == NULL) {
        return NULL;
    }

    Node* slow = head;
    Node* fast = head;

    // Say: "Fast moves 2x speed of slow"
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;          // Move slow by 1
        fast = fast->next->next;    // Move fast by 2
    }

    // Say: "Slow is now at middle (second middle for even length)"
    return slow;
}

// Variant: Returns first middle for even-length lists
Node* find_first_middle(Node* head) {
    // Say: "For first middle, I check if fast can move 2 MORE steps"
    if (head == NULL) {
        return NULL;
    }

    Node* slow = head;
    Node* fast = head;

    // Different condition: check 2 steps ahead
    while (fast->next != NULL && fast->next->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

/* ==================== TEST ==================== */

int main() {
    // Test odd-length list: 1 -> 2 -> 3 -> 4 -> 5
    Node* odd_list = create_node(1);
    odd_list->next = create_node(2);
    odd_list->next->next = create_node(3);
    odd_list->next->next->next = create_node(4);
    odd_list->next->next->next->next = create_node(5);

    printf("Odd list: ");
    print_list(odd_list);
    printf("Middle: %d\n\n", find_middle(odd_list)->data);

    // Test even-length list: 1 -> 2 -> 3 -> 4 -> 5 -> 6
    Node* even_list = create_node(1);
    even_list->next = create_node(2);
    even_list->next->next = create_node(3);
    even_list->next->next->next = create_node(4);
    even_list->next->next->next->next = create_node(5);
    even_list->next->next->next->next->next = create_node(6);

    printf("Even list: ");
    print_list(even_list);
    printf("Second middle: %d\n", find_middle(even_list)->data);
    printf("First middle: %d\n\n", find_first_middle(even_list)->data);

    // Test single node
    Node* single = create_node(42);
    printf("Single node: ");
    print_list(single);
    printf("Middle: %d\n\n", find_middle(single)->data);

    // Test two nodes
    Node* two = create_node(1);
    two->next = create_node(2);
    printf("Two nodes: ");
    print_list(two);
    printf("Second middle: %d\n", find_middle(two)->data);
    printf("First middle: %d\n", find_first_middle(two)->data);

    return 0;
}

/*
 * ==================== INTERVIEW EXPLANATION ====================
 *
 * PROBLEM: Find the middle node of a linked list.
 *
 * APPROACH - Slow/Fast Pointer:
 *
 * For odd length (1 -> 2 -> 3 -> 4 -> 5):
 *   Step 0: slow=1, fast=1
 *   Step 1: slow=2, fast=3
 *   Step 2: slow=3, fast=5
 *   Step 3: fast->next is NULL, stop
 *   Result: slow = 3 (middle)
 *
 * For even length (1 -> 2 -> 3 -> 4 -> 5 -> 6):
 *   Step 0: slow=1, fast=1
 *   Step 1: slow=2, fast=3
 *   Step 2: slow=3, fast=5
 *   Step 3: slow=4, fast=NULL (loop exits)
 *   Result: slow = 4 (second middle)
 *
 * WHY IT WORKS:
 * - Fast moves at 2x the speed of slow
 * - When fast reaches end, slow has covered half the distance
 * - For n nodes: fast covers n, slow covers n/2
 *
 * FIRST vs SECOND MIDDLE (for even length):
 * - [1,2,3,4,5,6] has two middles: 3 and 4
 * - Standard solution returns 4 (second middle)
 * - To get 3 (first middle), modify loop condition
 *
 * WHY SECOND MIDDLE IS PREFERRED:
 * - Useful for "split list in half" problems
 * - For merge sort: second half starts at middle
 * - For palindrome check: second half to reverse
 *
 * ALTERNATIVE APPROACH (two-pass):
 * 1. Count nodes (O(n))
 * 2. Traverse to n/2 (O(n/2))
 * Total: O(n) time, O(1) space - same complexity but two passes
 *
 * EDGE CASES:
 * 1. Empty list -> Return NULL
 * 2. Single node -> Return that node
 * 3. Two nodes -> Return second (or first with variant)
 *
 * COMMON MISTAKES:
 * - Off-by-one error in loop condition
 * - Not specifying which middle for even length
 * - Null pointer dereference for empty list
 *
 * APPLICATIONS:
 * 1. Merge Sort on linked list (split in half)
 * 2. Palindrome checking (find middle, reverse second half)
 * 3. Finding nth node from end (related pattern)
 *
 * INTERVIEW TIP: Always ask "which middle for even length?"
 */
