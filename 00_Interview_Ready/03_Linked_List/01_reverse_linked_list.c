/*
 * ============================================================================
 * PROBLEM: Reverse a Singly Linked List
 * ============================================================================
 *
 * WHAT IS A LINKED LIST?
 * A linked list is a linear data structure where elements (nodes) are stored
 * in non-contiguous memory locations. Each node contains:
 *   1. DATA: The actual value stored
 *   2. NEXT: A pointer to the next node in the sequence
 *
 * NODE STRUCTURE:
 *   +-------+-------+
 *   | data  | next  | ---> points to next node (or NULL)
 *   +-------+-------+
 *
 * In C, we define it as:
 *   typedef struct Node {
 *       int data;           // Value stored in node
 *       struct Node* next;  // Pointer to next node
 *   } Node;
 *
 * LINKED LIST vs ARRAY:
 *   - Array: Contiguous memory, O(1) access, O(n) insert/delete
 *   - Linked List: Scattered memory, O(n) access, O(1) insert/delete (if at position)
 *
 * WHAT IS THIS PROBLEM?
 * Given the head of a singly linked list, reverse the list so that the last
 * node becomes the first, second-to-last becomes second, and so on.
 * Return the new head (originally the last node).
 *
 * EXAMPLES:
 *   Input:  1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *   Output: 5 -> 4 -> 3 -> 2 -> 1 -> NULL
 *
 *   Input:  1 -> 2 -> NULL
 *   Output: 2 -> 1 -> NULL
 *
 *   Input:  NULL (empty list)
 *   Output: NULL
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 *   - Tests understanding of pointer manipulation
 *   - Foundation for many linked list problems (palindrome check, etc.)
 *   - Shows ability to handle edge cases (empty list, single node)
 *   - Classic problem that every engineer should know
 *   - Often asked as warm-up before harder linked list questions
 *
 * KEY CONCEPT: Three Pointer Technique
 *   Use three pointers to reverse links one by one:
 *   - prev: Points to the already-reversed portion (starts at NULL)
 *   - curr: Current node being processed (starts at head)
 *   - next: Temporarily saves the next node before we break the link
 *
 *   For each node: Save next, reverse link, move pointers forward
 *
 * VISUAL:
 *
 *   Initial State:
 *   NULL    1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *    ^      ^
 *   prev   curr
 *
 *   After Step 1: (reverse 1's link)
 *   NULL <- 1    2 -> 3 -> 4 -> 5 -> NULL
 *           ^    ^
 *          prev curr
 *
 *   After Step 2: (reverse 2's link)
 *   NULL <- 1 <- 2    3 -> 4 -> 5 -> NULL
 *                ^    ^
 *               prev curr
 *
 *   ... continue until curr is NULL ...
 *
 *   Final State:
 *   NULL <- 1 <- 2 <- 3 <- 4 <- 5
 *                               ^
 *                              prev (NEW HEAD!)
 *
 * ============================================================================
 */

/*
 * REVERSE LINKED LIST - Iterative Three Pointer Approach
 *
 * Time Complexity: O(n) - Single pass through the list
 * Space Complexity: O(1) - Only three pointers used
 *
 * Pattern: Three Pointer Technique (prev, curr, next)
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
 * DRY-RUN DIAGRAM: Reverse Linked List 1 -> 2 -> 3 -> 4 -> 5
 * ============================================================================
 *
 * INITIAL STATE:
 * ==============
 *
 *   prev    curr                                    next
 *    |       |                                       |
 *    v       v                                       v
 *   NULL    [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 1: Process node 1
 * ----------------------------------------------------------------------------
 *
 *   Step 1: next = curr->next  (save next before we break the link)
 *
 *   prev    curr    next
 *    |       |       |
 *    v       v       v
 *   NULL    [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *
 *   Step 2: curr->next = prev  (reverse the link - 1 now points to NULL)
 *
 *   prev    curr    next
 *    |       |       |
 *    v       v       v
 *   NULL <--[1]     [2] --> [3] --> [4] --> [5] --> NULL
 *
 *   Step 3: prev = curr  (move prev to current position)
 *
 *           prev    next
 *            |       |
 *            v       v
 *   NULL <--[1]     [2] --> [3] --> [4] --> [5] --> NULL
 *
 *   Step 4: curr = next  (move curr to next position)
 *
 *           prev    curr
 *            |       |
 *            v       v
 *   NULL <--[1]     [2] --> [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 2: Process node 2
 * ----------------------------------------------------------------------------
 *
 *   Step 1: next = curr->next
 *
 *           prev    curr    next
 *            |       |       |
 *            v       v       v
 *   NULL <--[1]     [2] --> [3] --> [4] --> [5] --> NULL
 *
 *   Step 2: curr->next = prev  (2 now points to 1)
 *
 *           prev    curr    next
 *            |       |       |
 *            v       v       v
 *   NULL <--[1] <--[2]     [3] --> [4] --> [5] --> NULL
 *
 *   Step 3: prev = curr
 *   Step 4: curr = next
 *
 *                  prev    curr
 *                   |       |
 *                   v       v
 *   NULL <--[1] <--[2]     [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 3: Process node 3
 * ----------------------------------------------------------------------------
 *
 *   After all steps:
 *
 *                          prev    curr
 *                           |       |
 *                           v       v
 *   NULL <--[1] <--[2] <--[3]     [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 4: Process node 4
 * ----------------------------------------------------------------------------
 *
 *   After all steps:
 *
 *                                  prev    curr
 *                                   |       |
 *                                   v       v
 *   NULL <--[1] <--[2] <--[3] <--[4]     [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 5: Process node 5
 * ----------------------------------------------------------------------------
 *
 *   After all steps:
 *
 *                                          prev    curr
 *                                           |       |
 *                                           v       v
 *   NULL <--[1] <--[2] <--[3] <--[4] <--[5]       NULL
 *
 * ----------------------------------------------------------------------------
 * LOOP ENDS: curr == NULL
 * ----------------------------------------------------------------------------
 *
 *   FINAL STATE: prev points to new head (node 5)
 *
 *                                          prev
 *                                           |
 *                                           v
 *   NULL <--[1] <--[2] <--[3] <--[4] <--[5]
 *                                        ^
 *                                     NEW HEAD!
 *
 *   Return prev (which is 5)
 *
 *   Result: 5 -> 4 -> 3 -> 2 -> 1 -> NULL
 *
 * ============================================================================
 */

Node* reverse_list(Node* head) {
    // Say: "I use three pointers: prev, curr, and next"
    Node* prev = NULL;
    Node* curr = head;
    Node* next = NULL;

    // Say: "I iterate through, reversing each link"
    while (curr != NULL) {
        // Say: "Save next, reverse link, move prev and curr forward"
        next = curr->next;      // Save next node
        curr->next = prev;      // Reverse the link
        prev = curr;            // Move prev forward
        curr = next;            // Move curr forward
    }

    // Say: "When curr becomes NULL, prev points to new head"
    return prev;
}

/* ==================== TEST ==================== */

int main() {
    // Create list: 1 -> 2 -> 3 -> 4 -> 5
    Node* head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);
    head->next->next->next->next = create_node(5);

    printf("Original: ");
    print_list(head);

    head = reverse_list(head);

    printf("Reversed: ");
    print_list(head);

    // Test empty list
    Node* empty = NULL;
    empty = reverse_list(empty);
    printf("Empty reversed: ");
    print_list(empty);

    // Test single node
    Node* single = create_node(42);
    single = reverse_list(single);
    printf("Single reversed: ");
    print_list(single);

    return 0;
}

/*
 * ==================== INTERVIEW EXPLANATION ====================
 *
 * PROBLEM: Reverse a singly linked list in-place.
 *
 * APPROACH - Three Pointer Technique:
 *
 * Initial State:
 *   NULL    1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *    ^      ^
 *   prev   curr
 *
 * Step by Step:
 *   1. Save: next = curr->next (save 2)
 *   2. Reverse: curr->next = prev (1 points to NULL)
 *   3. Move: prev = curr (prev at 1)
 *   4. Move: curr = next (curr at 2)
 *
 * After all iterations:
 *   NULL <- 1 <- 2 <- 3 <- 4 <- 5
 *                               ^
 *                              prev (new head)
 *
 * WHY THREE POINTERS:
 * - prev: Tracks the already-reversed portion
 * - curr: Current node being processed
 * - next: Saves reference before we break the link
 *
 * EDGE CASES TO MENTION:
 * 1. Empty list (head = NULL) -> Return NULL
 * 2. Single node -> Returns same node (no change)
 * 3. Two nodes -> Simple swap
 *
 * COMMON MISTAKES:
 * - Forgetting to save next before reversing link (lose rest of list)
 * - Returning curr instead of prev (curr is NULL at end)
 * - Not handling empty list
 *
 * FOLLOW-UP: Recursive solution?
 * - Base case: head is NULL or single node
 * - Recurse to end, then reverse links on way back
 * - Same O(n) time but O(n) space for call stack
 *
 * INTERVIEW TIP: Draw the pointers and show movement visually!
 */
