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
