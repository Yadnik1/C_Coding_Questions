/*
 * ============================================================================
 * PROBLEM: Delete Node Without Head Access (The Tricky One!)
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * You are given access ONLY to the node to be deleted (not the head pointer).
 * Delete this node from the linked list.
 *
 * THE CATCH: In a singly linked list, to delete a node, you normally need
 * access to the PREVIOUS node to update its next pointer. But here you
 * don't have the head, so you can't reach the previous node!
 *
 * EXAMPLES:
 *   Input:  List: 1 -> 2 -> 3 -> 4 -> 5
 *           Given: pointer to node with value 3
 *   Output: 1 -> 2 -> 4 -> 5 (node 3 is "deleted")
 *
 *   Input:  List: 1 -> 2 -> 3
 *           Given: pointer to node with value 2
 *   Output: 1 -> 3
 *
 * LIMITATION:
 *   - CANNOT delete the LAST node with this trick!
 *   - The problem usually guarantees the node is not the tail.
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 *   - Tests creative/lateral thinking
 *   - Shows you can work around constraints
 *   - Demonstrates understanding of what "deletion" really means
 *   - Classic "trick" question that appears frequently
 *   - Good discussion point: limitations and edge cases
 *
 * KEY CONCEPT: Copy and Delete Next (The Trick!)
 *   Since we can't access the previous node, we use a clever workaround:
 *   1. Copy the NEXT node's data into the current node
 *   2. Delete the NEXT node (which we CAN access)
 *   3. From the outside, it looks like current node was deleted!
 *
 *   We're not actually deleting the given node - we're making it
 *   "become" the next node, then deleting the next node.
 *
 * VISUAL:
 *
 *   Delete node with value 3:
 *
 *   Before: 1 -> 2 -> [3] -> 4 -> 5 -> NULL
 *                      ^
 *                   given (want to "delete" this)
 *
 *   Step 1: Copy next node's data (4) into current node
 *           1 -> 2 -> [4] -> 4 -> 5 -> NULL
 *                      ^     ^
 *                   given   next (same data now)
 *
 *   Step 2: Skip over next node
 *           1 -> 2 -> [4] ------> 5 -> NULL
 *                      ^
 *                   given->next = given->next->next
 *
 *   Step 3: Free the old next node (the duplicate 4)
 *
 *   Result: 1 -> 2 -> 4 -> 5 -> NULL
 *           (Looks like 3 was deleted!)
 *
 * WHY CAN'T WE DELETE THE LAST NODE?
 *
 *   Before: 1 -> 2 -> [3] -> NULL
 *                      ^
 *                   given (last node)
 *
 *   - There's no next node to copy data from!
 *   - We can't update node 2's next pointer to NULL
 *   - Node 2 still points to node 3 (dangling reference)
 *
 *   The only workaround: Mark the node as "deleted" with a sentinel
 *   value, and clean up later when you have head access.
 *
 * ============================================================================
 */

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

/*
 * ============================================================================
 * NODE STRUCTURE - Line by Line Explanation
 * ============================================================================
 * Say: "A Node has two parts - data stores the value, next is a pointer
 *       to the next node in the list"
 * ============================================================================
 */
typedef struct Node {
    int data;               // Say: "The value this node holds"
    struct Node* next;      // Say: "Address of the next node in the chain"
} Node;

/*
 * ============================================================================
 * CREATE NODE FUNCTION - Line by Line Explanation
 * ============================================================================
 * Say: "This function allocates memory for a new node and initializes it"
 * ============================================================================
 */
Node* create_node(int data) {
    Node* node = (Node*)malloc(sizeof(Node));   // Allocate on heap
    node->data = data;                          // Store the value
    node->next = NULL;                          // Initialize link to NULL
    return node;                                // Return address of new node
}

/*
 * ============================================================================
 * PRINT LIST FUNCTION - Line by Line Explanation
 * ============================================================================
 * Say: "This traverses the list from head to end, printing each value"
 * ============================================================================
 */
void print_list(Node* head) {
    printf("[");                            // Start output
    while (head) {                          // Until we reach NULL
        printf("%d", head->data);           // Print current value
        if (head->next) printf(" -> ");     // Arrow if not last node
        head = head->next;                  // Move to next node
    }
    printf("]\n");                          // End output
}

/* ==================== SOLUTION ==================== */

/*
 * ============================================================================
 * DRY-RUN DIAGRAM: Delete Node Without Head Access
 * ============================================================================
 *
 * EXAMPLE: Delete node with value 3
 *
 * INPUT: 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *                   ^
 *                 node (we only have access to this!)
 *
 * ============================================================================
 * THE PROBLEM:
 * ============================================================================
 *
 *   Normally to delete node 3, we need to update node 2's next pointer:
 *   node2->next = node3->next
 *
 *   But we don't have access to node 2 (no head pointer)!
 *   We can only access node 3 and everything after it.
 *
 * ============================================================================
 * THE TRICK: Copy data from next node, then delete next node
 * ============================================================================
 *
 * INITIAL STATE:
 *
 *   [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *                    ^       ^
 *                  node   node->next
 *
 * ----------------------------------------------------------------------------
 * STEP 1: Copy next node's data to current node
 * ----------------------------------------------------------------------------
 *   node->data = node->next->data  (copy 4 into node)
 *
 *   [1] --> [2] --> [4] --> [4] --> [5] --> NULL
 *                    ^       ^
 *                  node   node->next
 *                 (now 4)  (original 4)
 *
 * ----------------------------------------------------------------------------
 * STEP 2: Skip over the next node
 * ----------------------------------------------------------------------------
 *   next_node = node->next (save pointer to node with original 4)
 *   node->next = node->next->next (point to 5)
 *
 *   [1] --> [2] --> [4] -----------> [5] --> NULL
 *                    ^        ^
 *                  node    (dangling, will be freed)
 *
 * ----------------------------------------------------------------------------
 * STEP 3: Free the old next node
 * ----------------------------------------------------------------------------
 *   free(next_node)
 *
 *   FINAL RESULT: [1] --> [2] --> [4] --> [5] --> NULL
 *
 *   From the outside, it LOOKS like node 3 was deleted!
 *   (But actually, the original node 3 is still there, just with different data)
 *
 * ============================================================================
 * WHY THIS WORKS:
 * ============================================================================
 *
 *   Before: ... -> [2] -> [3] -> [4] -> [5] -> ...
 *                          ^
 *                      we have this
 *
 *   After:  ... -> [2] -> [4] -> [5] -> ...
 *                          ^
 *                   same memory location, different value!
 *
 *   The node at the original [3] position now contains [4]'s data.
 *   The original [4] node is freed.
 *   Net effect: The list looks like [3] was removed.
 *
 * ============================================================================
 * LIMITATION: Cannot delete the LAST node
 * ============================================================================
 *
 *   [1] --> [2] --> [3] --> NULL
 *                    ^
 *                  node (last node)
 *
 *   - node->next is NULL
 *   - No next node to copy data from!
 *   - Even if we could "delete" it, [2] would still point to [3] (dangling!)
 *   - We CAN'T update [2]->next to NULL because we don't have access to [2]
 *
 * ============================================================================
 */

/*
 * ============================================================================
 * DELETE_NODE FUNCTION - Line by Line Explanation (The Trick!)
 * ============================================================================
 *
 * THE PROBLEM:
 *   - We only have access to the node to delete (not the head!)
 *   - In singly linked list, to delete a node, we normally need PREVIOUS node
 *   - But we can't traverse backwards to find previous!
 *
 * THE TRICK: Copy and Delete Next
 *   Instead of actually deleting this node, we:
 *   1. Copy the NEXT node's data into this node
 *   2. Delete the NEXT node (which we CAN access)
 *   3. From outside, it LOOKS like this node was deleted!
 *
 * VISUAL:
 *   Want to "delete" node with value 3:
 *
 *   Before: 1 -> 2 -> [3] -> 4 -> 5
 *                      ^
 *                   given node
 *
 *   Step 1: Copy next's data (4) into current
 *           1 -> 2 -> [4] -> 4 -> 5
 *
 *   Step 2: Skip over next node
 *           1 -> 2 -> [4] -----> 5
 *
 *   Result: 1 -> 2 -> 4 -> 5  (looks like 3 was deleted!)
 *
 * LIMITATION: Cannot delete the LAST node!
 *   - No next node to copy from
 *   - Would leave dangling pointer from previous
 *
 * ============================================================================
 */
void delete_node(Node* node) {
    /*
     * VALIDATION CHECK
     * -----------------
     * Say: "Can't use this trick if node is NULL or is the last node"
     *
     * Why can't delete last node?
     *   - node->next is NULL, no data to copy
     *   - Previous node still points to us (dangling!)
     *   - We can't update previous->next without access to previous
     */
    if (node == NULL || node->next == NULL) {
        return;  // Say: "Cannot delete last node with this trick"
    }

    /*
     * STEP 1: SAVE POINTER TO NEXT NODE
     * -----------------------------------
     * Say: "Save reference to next node (we'll delete it later)"
     *
     * We need this pointer to:
     *   1. Access next's data
     *   2. Link past next to next->next
     *   3. Free next's memory
     */
    Node* next_node = node->next;  // Say: "next_node = node after given node"

    /*
     * STEP 2: COPY NEXT NODE'S DATA
     * ------------------------------
     * Say: "Copy next node's value into current node"
     *
     * This is the KEY step! We're making current node
     * "become" the next node by copying its data.
     *
     * Before: [3].data = 3, [4].data = 4
     * After:  [3].data = 4  (now contains 4!)
     */
    node->data = next_node->data;  // Say: "Current now has next's value"

    /*
     * STEP 3: SKIP OVER NEXT NODE
     * ----------------------------
     * Say: "Link current node to skip over the old next"
     *
     * Before: current -> next -> next_next
     * After:  current ---------> next_next
     *
     * The 'next' node is now orphaned (not in the list).
     */
    node->next = next_node->next;  // Say: "Skip over the old next node"

    /*
     * STEP 4: FREE THE OLD NEXT NODE
     * -------------------------------
     * Say: "Release memory of the node we skipped"
     *
     * The old next node is no longer in the list.
     * We must free it to avoid memory leak.
     *
     * Note: We're freeing the memory at next_node's address,
     * but logically we "deleted" the current node's original value.
     */
    free(next_node);  // Say: "Free memory of skipped node"

    /*
     * RESULT:
     * --------
     * The list now looks like 'node' was deleted:
     *   - node contains what was in node->next
     *   - node->next is what was in node->next->next
     *   - Original next node is freed
     *
     * From external perspective: node with original value is "gone"
     * Reality: node still exists, but with different data
     */
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
