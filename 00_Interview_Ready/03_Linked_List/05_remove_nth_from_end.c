/*
 * ============================================================================
 * PROBLEM: Remove Nth Node From End of List
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given the head of a linked list, remove the nth node from the END of the
 * list and return the head. Do this in ONE PASS (not counting nodes first).
 *
 * EXAMPLES:
 *   Input:  1 -> 2 -> 3 -> 4 -> 5, n = 2
 *                          ^
 *                    2nd from end (remove this)
 *   Output: 1 -> 2 -> 3 -> 5
 *
 *   Input:  1 -> 2 -> 3, n = 3 (remove head)
 *           ^
 *     3rd from end
 *   Output: 2 -> 3
 *
 *   Input:  1, n = 1 (single node, remove it)
 *   Output: NULL (empty list)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 *   - Tests the "gap technique" with two pointers
 *   - Requires careful handling of edge cases (removing head)
 *   - Shows understanding of single-pass algorithms
 *   - Demonstrates the power of maintaining pointer distances
 *   - Common follow-up: "Can you do it in one pass?"
 *
 * KEY CONCEPT: Two Pointer with Fixed Gap
 *   The trick: Maintain a gap of (n+1) between two pointers.
 *   When the fast pointer reaches NULL, the slow pointer is
 *   right BEFORE the node to delete (so we can update next pointer).
 *
 *   Why n+1 gap? We need slow to stop at the PREVIOUS node
 *   to be able to do: slow->next = slow->next->next
 *
 * VISUAL:
 *
 *   Remove 2nd from end in: 1 -> 2 -> 3 -> 4 -> 5
 *
 *   Step 1: Create gap of n+1 = 3 (fast moves 3 steps)
 *
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *     ^                ^
 *    slow             fast    (gap of 3 nodes)
 *
 *   Step 2: Move both until fast reaches NULL
 *
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *                      ^              ^
 *                     slow           fast
 *
 *   Step 3: slow->next is the node to remove (4)
 *           slow->next = slow->next->next
 *
 *   Result: 1 -> 2 -> 3 -> 5
 *
 * WHY DUMMY NODE?
 *   Handle the case when we need to remove the HEAD.
 *   Example: Remove 5th from end in a 5-node list = remove head
 *
 *   Without dummy: Special case needed
 *   With dummy: Same logic works for all cases
 *
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *     ^                               ^
 *    slow                            fast
 *
 *   slow->next = 1 (the head), which gets removed
 *   Return dummy.next = 2 (new head)
 *
 * ============================================================================
 */

/*
 * REMOVE NTH NODE FROM END - Two Pointer with Gap
 *
 * Time Complexity: O(n) - Single pass through the list
 * Space Complexity: O(1) - Only two pointers
 *
 * Pattern: Two Pointer with Fixed Gap
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
 * DRY-RUN DIAGRAM: Remove Nth Node From End
 * ============================================================================
 *
 * EXAMPLE: Remove 2nd from end in: 1 -> 2 -> 3 -> 4 -> 5
 * (n = 2, so we remove node with value 4)
 *
 * INITIAL STATE:
 *
 *   dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *     ^
 *   slow, fast
 *
 * ----------------------------------------------------------------------------
 * PHASE 1: Move fast n+1 (3) steps ahead to create gap
 * ----------------------------------------------------------------------------
 *
 *   Step 1 (i=0): fast = fast->next (moves to 1)
 *
 *   dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *     ^        ^
 *   slow     fast
 *
 *   Step 2 (i=1): fast = fast->next (moves to 2)
 *
 *   dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *     ^               ^
 *   slow            fast
 *
 *   Step 3 (i=2): fast = fast->next (moves to 3)
 *
 *   dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *     ^                        ^
 *   slow                     fast
 *
 *   GAP = 3 nodes between slow and fast
 *
 * ----------------------------------------------------------------------------
 * PHASE 2: Move both until fast reaches NULL
 * ----------------------------------------------------------------------------
 *
 *   Iteration 1:
 *   fast = fast->next (moves to 4)
 *   slow = slow->next (moves to 1)
 *
 *   dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *              ^                       ^
 *            slow                    fast
 *
 *   Iteration 2:
 *   fast = fast->next (moves to 5)
 *   slow = slow->next (moves to 2)
 *
 *   dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *                      ^                       ^
 *                    slow                    fast
 *
 *   Iteration 3:
 *   fast = fast->next (moves to NULL)
 *   slow = slow->next (moves to 3)
 *
 *   dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *                              ^                       ^
 *                            slow                    fast
 *
 *   fast == NULL -> LOOP EXITS
 *
 * ----------------------------------------------------------------------------
 * PHASE 3: Remove slow->next (which is node 4)
 * ----------------------------------------------------------------------------
 *
 *   slow->next = [4]  <-- This is the node to delete (2nd from end!)
 *
 *   Before: slow -> [3] --> [4] --> [5]
 *
 *   slow->next = slow->next->next
 *
 *   After:  slow -> [3] -----------> [5]
 *                           ^
 *                      Node 4 removed!
 *
 *   RESULT: dummy --> [1] --> [2] --> [3] --> [5] --> NULL
 *
 *   Return dummy.next = [1]
 *
 * ============================================================================
 * EDGE CASE: Removing the Head (n = list length)
 * ============================================================================
 *
 *   List: 1 -> 2 -> 3  (n = 3, remove 3rd from end = head)
 *
 *   After Phase 1 (move fast 4 steps):
 *
 *   dummy --> [1] --> [2] --> [3] --> NULL
 *     ^                               ^
 *   slow                            fast
 *
 *   fast is already NULL -> Phase 2 loop never runs
 *
 *   slow->next = [1] (the head!)
 *   slow->next = slow->next->next = [2]
 *
 *   dummy --> [2] --> [3] --> NULL
 *
 *   Return dummy.next = [2] (new head)
 *
 * ============================================================================
 */

/*
 * ============================================================================
 * REMOVE NTH FROM END FUNCTION - Line by Line Explanation
 * ============================================================================
 */
Node* remove_nth_from_end(Node* head, int n) {
    /*
     * Say: "I use a dummy node to handle edge case of removing head"
     *
     * WHY DUMMY NODE?
     * - If we need to remove the HEAD (nth from end = list length)
     * - Without dummy, we'd need special case logic
     * - With dummy, slow stops at dummy when removing head
     * - dummy->next = head->next skips the old head cleanly
     */

    Node dummy;
    /*    ^^^^^^^^^
     *    STACK-ALLOCATED struct (NOT a pointer!)
     *    Lives on the stack, no malloc needed
     *    We use '.' to access its members
     *
     *    WHY "Node dummy" and not "Node* dummy"?
     *    - We want an actual struct on the stack, not a pointer
     *    - Stack allocation is automatic (no malloc/free needed)
     *    - The struct lives until this function returns
     */

    dummy.next = head;
    /*    ^^^^^^^^^^^
     *    Use '.' because dummy IS the struct (not a pointer)
     *    dummy.next now points to the actual head of the list
     *
     *    Visualization:
     *    dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
     *
     *    REMEMBER: '.' for struct, '->' for pointer
     *    - Node x;    -> use x.member
     *    - Node* x;   -> use x->member
     */

    Node* fast = &dummy;
    /*    ^^^^^^^^^^^^^^
     *    fast is a POINTER to the dummy struct
     *    &dummy = "address of dummy" (the & operator gets the address)
     *
     *    Now fast holds the memory address where dummy lives
     *    We can use fast-> because fast IS a pointer
     *
     *    fast
     *     |
     *     v
     *    dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
     */

    Node* slow = &dummy;
    /*    ^^^^^^^^^^^^^^
     *    slow is ALSO a pointer to the dummy struct
     *    Both start at the same position (dummy)
     *
     *    slow, fast
     *        |
     *        v
     *       dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
     */

    for (int i = 0; i <= n; i++) {
        /*    ^^^^^^^^^^^^^^^^^
         *    Say: "First, move fast pointer n+1 steps ahead"
         *
         *    WHY n+1 STEPS (i <= n means n+1 iterations)?
         *    - We need slow to stop at the node BEFORE the one to delete
         *    - To delete node X, we need access to node X-1
         *    - Gap of n+1 ensures slow lands at the predecessor
         *
         *    For n=2 (remove 2nd from end):
         *    - i=0: fast moves to [1]
         *    - i=1: fast moves to [2]
         *    - i=2: fast moves to [3]
         *    Now there's a gap of 3 between slow and fast
         */

        if (fast == NULL) {
            /*    ^^^^^^^^^^^^
             *    Edge case: n is larger than list length
             *    fast went off the end before finishing n+1 steps
             *    Return original head unchanged (invalid n)
             */
            return head;
        }

        fast = fast->next;
        /*    ^^^^^^^^^^^^^
         *    Move fast one step forward
         *    Use '->' because fast is a POINTER
         *
         *    fast->next means:
         *    "go to the node fast points to, get its 'next' field"
         */
    }
    /*
     * After the loop, fast is n+1 steps ahead of slow:
     *
     * For n=2 (remove 2nd from end) in list [1,2,3,4,5]:
     *
     *    slow                    fast
     *     |                        |
     *     v                        v
     *    dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
     *
     *    Gap = 3 nodes (dummy, 1, 2) between slow and fast
     */

    while (fast != NULL) {
        /*    ^^^^^^^^^^^^^
         *    Say: "Now move both until fast reaches end"
         *    Say: "The gap ensures slow stops at node BEFORE the one to delete"
         *
         *    Move both pointers together, maintaining the gap
         *    When fast hits NULL, slow will be at the predecessor of target
         */

        fast = fast->next;
        /*    ^^^^^^^^^^^^^
         *    Move fast one step forward
         */

        slow = slow->next;
        /*    ^^^^^^^^^^^^^
         *    Move slow one step forward
         *    The gap between them stays constant!
         */
    }
    /*
     * After the loop (for n=2):
     *
     *                              slow            fast
     *                               |               |
     *                               v               v
     *    dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> NULL
     *                                       ^
     *                                 This is the target (2nd from end)
     *                                 slow->next points to it!
     */

    Node* to_delete = slow->next;
    /*    ^^^^^^^^^^^^^^^^^^^^^^^^
     *    Say: "slow->next is the node to remove"
     *
     *    Save pointer to the node we're about to remove
     *    We need this to free() its memory later
     *
     *    to_delete points to [4] (the 2nd from end)
     */

    slow->next = slow->next->next;
    /*    ^^^^^^^^^^^^^^^^^^^^^^^^^
     *    Skip over the node to delete!
     *
     *    slow->next = slow->next->next means:
     *    - slow->next was pointing to [4]
     *    - slow->next->next is [5]
     *    - Now slow->next points directly to [5], skipping [4]
     *
     *    Before: [3] --> [4] --> [5]
     *    After:  [3] ----------> [5]
     *                     ^
     *                  [4] is now orphaned (will be freed)
     */

    free(to_delete);
    /*    ^^^^^^^^^^^^^
     *    Release the memory of the deleted node
     *    Prevents memory leak!
     *
     *    IMPORTANT: Always free dynamically allocated memory
     *    when you're done with it
     */

    return dummy.next;
    /*     ^^^^^^^^^^
     *     Return the new head of the list
     *     Use '.' because dummy is a struct, not a pointer
     *
     *     Why dummy.next and not just head?
     *     - If we removed the original head, head is now invalid (freed)
     *     - dummy.next correctly points to the new head
     *
     *     Example: Remove 5th from end in [1,2,3,4,5] (remove head):
     *     - dummy.next was [1]
     *     - After deletion, dummy.next is [2]
     *     - Return [2] as new head (not the old head [1] which is freed!)
     */
}

/* ==================== TEST ==================== */

int main() {
    // Test case 1: Remove 2nd from end in [1,2,3,4,5]
    Node* list1 = create_node(1);
    list1->next = create_node(2);
    list1->next->next = create_node(3);
    list1->next->next->next = create_node(4);
    list1->next->next->next->next = create_node(5);

    printf("Original: ");
    print_list(list1);
    printf("Remove 2nd from end (value 4)\n");
    list1 = remove_nth_from_end(list1, 2);
    printf("Result:   ");
    print_list(list1);

    // Test case 2: Remove last node
    Node* list2 = create_node(1);
    list2->next = create_node(2);
    list2->next->next = create_node(3);

    printf("\nOriginal: ");
    print_list(list2);
    printf("Remove 1st from end (value 3)\n");
    list2 = remove_nth_from_end(list2, 1);
    printf("Result:   ");
    print_list(list2);

    // Test case 3: Remove first node (head)
    Node* list3 = create_node(1);
    list3->next = create_node(2);
    list3->next->next = create_node(3);

    printf("\nOriginal: ");
    print_list(list3);
    printf("Remove 3rd from end (head, value 1)\n");
    list3 = remove_nth_from_end(list3, 3);
    printf("Result:   ");
    print_list(list3);

    // Test case 4: Single node list
    Node* list4 = create_node(1);
    printf("\nOriginal: ");
    print_list(list4);
    printf("Remove 1st from end (only node)\n");
    list4 = remove_nth_from_end(list4, 1);
    printf("Result:   ");
    print_list(list4);

    return 0;
}

/*
 * ==================== INTERVIEW EXPLANATION ====================
 *
 * PROBLEM: Remove the nth node from the end of a linked list.
 *
 * APPROACH - Two Pointer with Fixed Gap:
 *
 * Key Insight: Create a gap of n+1 between two pointers.
 * When fast reaches NULL, slow is at the node BEFORE target.
 *
 * Visual for removing 2nd from end in [1,2,3,4,5]:
 *
 *   Step 1: Move fast n+1 (3) steps
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *     ^                ^
 *    slow             fast
 *
 *   Step 2: Move both until fast is NULL
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *                      ^              ^
 *                     slow           fast
 *
 *   Step 3: Remove slow->next (which is 4)
 *   Result: 1 -> 2 -> 3 -> 5
 *
 * WHY N+1 GAP (not N):
 * - We need to stop at the node BEFORE the one to delete
 * - To delete node X, we need access to node X-1
 * - Gap of n+1 positions slow at the correct predecessor
 *
 * WHY DUMMY NODE:
 * - Edge case: removing head (nth from end = list length)
 * - Without dummy, we'd need special logic for this case
 * - Dummy ensures slow always has a valid previous position
 *
 * TWO-PASS ALTERNATIVE:
 * 1. Count total nodes (n)
 * 2. Traverse to node (n - k) to remove kth from end
 * - Same O(n) time but two passes
 *
 * EDGE CASES:
 * 1. Remove head (n = list length)
 * 2. Remove tail (n = 1)
 * 3. Single node list
 * 4. n > list length (invalid input)
 *
 * COMMON MISTAKES:
 * - Gap of n instead of n+1 (stops at wrong node)
 * - Forgetting dummy node (fails when removing head)
 * - Not handling n > list length
 * - Memory leak (not freeing deleted node)
 *
 * INTERVIEW TIP:
 * - Ask: "Is n always valid?" (1 <= n <= list length)
 * - Draw the gap on whiteboard!
 *
 * RELATED PROBLEMS:
 * - Find nth node from end (same approach, just return)
 * - Find middle (fast moves 2x, related technique)
 */
