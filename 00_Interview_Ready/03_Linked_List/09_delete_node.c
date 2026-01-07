/*
 * DELETE NODE (Without Head Access) - Copy Next Node's Data
 *
 * Time Complexity: O(1) - Just copy and relink
 * Space Complexity: O(1) - No extra space
 *
 * Pattern: Copy and Delete Next
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

// Note: Cannot delete the LAST node with this approach
void delete_node(Node* node) {
    // Say: "I can't access previous node, so I copy next node's data into this one"

    // Say: "This trick only works if node is NOT the last node"
    if (node == NULL || node->next == NULL) {
        // Can't delete last node without head access
        return;
    }

    // Say: "Copy the next node's data to current node"
    Node* next_node = node->next;
    node->data = next_node->data;

    // Say: "Point current to skip over next node"
    node->next = next_node->next;

    // Say: "Free the old next node"
    free(next_node);

    // Say: "Effectively, we deleted next but made current look like next"
}

/* ==================== TEST ==================== */

int main() {
    // Create list: 1 -> 2 -> 3 -> 4 -> 5
    Node* head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);
    head->next->next->next->next = create_node(5);

    printf("Original list: ");
    print_list(head);

    // Delete node with value 3 (we only have pointer to this node)
    Node* node_to_delete = head->next->next;  // Node with value 3
    printf("Deleting node with value: %d\n", node_to_delete->data);

    delete_node(node_to_delete);

    printf("After deletion: ");
    print_list(head);

    // Delete another node (value 2)
    node_to_delete = head->next;  // Node with value 2
    printf("\nDeleting node with value: %d\n", node_to_delete->data);

    delete_node(node_to_delete);

    printf("After deletion: ");
    print_list(head);

    // Demonstrate limitation: Cannot delete last node
    printf("\n--- LIMITATION DEMO ---\n");
    Node* list2 = create_node(1);
    list2->next = create_node(2);
    list2->next->next = create_node(3);

    printf("List: ");
    print_list(list2);

    Node* last_node = list2->next->next;  // Node with value 3 (last)
    printf("Attempting to delete last node (value 3)...\n");
    delete_node(last_node);  // This won't work

    printf("After attempt: ");
    print_list(list2);
    printf("(Last node cannot be deleted with this method)\n");

    return 0;
}

/*
 * ==================== INTERVIEW EXPLANATION ====================
 *
 * PROBLEM: Delete a node given only access to that node (no head pointer).
 *
 * THE TRICK - Copy and Delete Next:
 *
 * Since we can't access the previous node, we can't relink.
 * Instead, we COPY the next node's data and delete the next node!
 *
 * Visual:
 *   Before: 1 -> 2 -> [3] -> 4 -> 5    (delete node with 3)
 *                      ^
 *                    given
 *
 *   Step 1: Copy next's data
 *           1 -> 2 -> [4] -> 4 -> 5    (node now has 4)
 *
 *   Step 2: Skip next node
 *           1 -> 2 -> [4] -----> 5
 *
 *   Result: 1 -> 2 -> 4 -> 5            (looks like 3 was deleted)
 *
 * KEY INSIGHT:
 * - We're not actually deleting the given node
 * - We're making it look like the next node
 * - Then deleting the next node
 * - From outside, it appears the given node was deleted
 *
 * LIMITATION - Cannot Delete Last Node:
 * - No next node to copy from
 * - Would leave dangling pointer from previous node
 * - Only workaround: Mark as deleted (if values allow sentinel)
 *
 * TIME COMPLEXITY: O(1)
 * - No traversal needed
 * - Just copy and relink
 *
 * EDGE CASES:
 * 1. Node is last -> Cannot delete (limitation)
 * 2. Node is NULL -> Handle gracefully
 * 3. Single node list -> Cannot delete
 * 4. Node has complex data -> Need deep copy
 *
 * POTENTIAL ISSUES:
 * 1. If external code holds pointer to next node, it becomes invalid
 * 2. If node contains non-copyable data (file handles, etc.)
 * 3. Memory: Original node persists, next node is freed
 *
 * INTERVIEW DISCUSSION POINTS:
 *
 * Q: What if someone has a pointer to the next node?
 * A: They'll have a dangling pointer after deletion
 *
 * Q: What about the last node?
 * A: Cannot be deleted without head access. Could mark as "deleted"
 *    with a sentinel value if requirements allow.
 *
 * Q: Is this a true deletion?
 * A: Logically yes, physically no. The given node survives,
 *    next node is freed. Values shift.
 *
 * ALTERNATIVE (if last node deletion required):
 * - Store head pointer globally (not recommended)
 * - Use doubly linked list
 * - Mark last node as "deleted" and clean up later
 *
 * INTERVIEW TIP:
 * - Immediately mention the "last node" limitation
 * - Shows you understand edge cases
 * - Ask if last node deletion is required
 */
