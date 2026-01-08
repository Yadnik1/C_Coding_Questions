/*
 * ============================================================================
 * PROBLEM: Remove Duplicates from Sorted Linked List
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given the head of a SORTED linked list, delete all duplicate values,
 * keeping only the first occurrence of each value. The list should remain
 * sorted after removing duplicates.
 *
 * KEY INSIGHT: Since the list is SORTED, all duplicates are ADJACENT!
 * This makes the problem much simpler than unsorted lists.
 *
 * EXAMPLES:
 *   Input:  1 -> 1 -> 2 -> 3 -> 3 -> 3 -> 4
 *   Output: 1 -> 2 -> 3 -> 4
 *
 *   Input:  5 -> 5 -> 5 (all same)
 *   Output: 5
 *
 *   Input:  1 -> 2 -> 3 (no duplicates)
 *   Output: 1 -> 2 -> 3 (unchanged)
 *
 *   VARIANT (Remove ALL occurrences of duplicated values):
 *   Input:  1 -> 1 -> 2 -> 3 -> 3 -> 4
 *   Output: 2 -> 4 (only unique values kept)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 *   - Tests understanding of sorted data properties
 *   - Simple but shows pointer manipulation skills
 *   - Often followed by harder variant: remove ALL duplicates
 *   - Memory management: freeing deleted nodes
 *   - Compare with unsorted list approach (harder)
 *
 * KEY CONCEPT: Skip Adjacent Duplicates
 *   Since list is sorted, duplicates are next to each other.
 *   For each node, check if next node has same value.
 *   If yes, skip over it. If no, move to next node.
 *
 *   IMPORTANT: After deleting, don't advance! Check new next node
 *   (it might also be a duplicate).
 *
 * VISUAL:
 *
 *   1 -> 1 -> 2 -> 3 -> 3 -> 3 -> 4
 *   ^
 *  curr
 *
 *   curr->data == curr->next->data? (1 == 1) YES!
 *   Skip: curr->next = curr->next->next
 *
 *   1 -> 2 -> 3 -> 3 -> 3 -> 4
 *   ^
 *  curr
 *
 *   curr->data == curr->next->data? (1 == 2) NO
 *   Advance: curr = curr->next
 *
 *   1 -> 2 -> 3 -> 3 -> 3 -> 4
 *        ^
 *       curr
 *
 *   Continue until end...
 *
 *   Final: 1 -> 2 -> 3 -> 4
 *
 * VARIANT: Remove ALL Nodes with Duplicates
 *
 *   Input:  1 -> 1 -> 2 -> 3 -> 3 -> 4
 *   Output: 2 -> 4
 *
 *   +-------+    +---+    +---+    +---+
 *   | dummy | -> | 2 | -> | 4 | -> NULL
 *   +-------+    +---+    +---+
 *
 *   Need dummy node because head might be removed (1 is duplicated)
 *   Need prev pointer to reconnect after removing all copies
 *
 * ============================================================================
 */

/*
 * REMOVE DUPLICATES FROM SORTED LIST - Skip Duplicates
 *
 * Time Complexity: O(n) - Single pass through the list
 * Space Complexity: O(1) - Only pointer manipulation
 *
 * Pattern: Skip Duplicates with Single Pointer
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
 * DRY-RUN DIAGRAM: Remove Duplicates from Sorted Linked List
 * ============================================================================
 *
 * INPUT: 1 -> 1 -> 2 -> 3 -> 3 -> 3 -> 4 -> NULL
 *
 * INITIAL STATE:
 *
 *   curr
 *    |
 *    v
 *   [1] --> [1] --> [2] --> [3] --> [3] --> [3] --> [4] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 1: curr->data (1) == curr->next->data (1)? YES - DUPLICATE!
 * ----------------------------------------------------------------------------
 *   Skip the duplicate:
 *   duplicate = curr->next (node with value 1)
 *   curr->next = curr->next->next
 *   free(duplicate)
 *
 *   DO NOT advance curr (check new next node)
 *
 *   curr
 *    |
 *    v
 *   [1] --> [2] --> [3] --> [3] --> [3] --> [4] --> NULL
 *            ^
 *       new next (was curr->next->next)
 *
 * ----------------------------------------------------------------------------
 * ITERATION 2: curr->data (1) == curr->next->data (2)? NO - DIFFERENT
 * ----------------------------------------------------------------------------
 *   Advance curr:
 *   curr = curr->next (move to 2)
 *
 *           curr
 *            |
 *            v
 *   [1] --> [2] --> [3] --> [3] --> [3] --> [4] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 3: curr->data (2) == curr->next->data (3)? NO - DIFFERENT
 * ----------------------------------------------------------------------------
 *   Advance curr:
 *   curr = curr->next (move to 3)
 *
 *                   curr
 *                    |
 *                    v
 *   [1] --> [2] --> [3] --> [3] --> [3] --> [4] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 4: curr->data (3) == curr->next->data (3)? YES - DUPLICATE!
 * ----------------------------------------------------------------------------
 *   Skip the duplicate:
 *   curr->next = curr->next->next
 *
 *   DO NOT advance curr
 *
 *                   curr
 *                    |
 *                    v
 *   [1] --> [2] --> [3] --> [3] --> [4] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 5: curr->data (3) == curr->next->data (3)? YES - DUPLICATE!
 * ----------------------------------------------------------------------------
 *   Skip the duplicate:
 *   curr->next = curr->next->next
 *
 *   DO NOT advance curr
 *
 *                   curr
 *                    |
 *                    v
 *   [1] --> [2] --> [3] --> [4] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 6: curr->data (3) == curr->next->data (4)? NO - DIFFERENT
 * ----------------------------------------------------------------------------
 *   Advance curr:
 *   curr = curr->next (move to 4)
 *
 *                           curr
 *                            |
 *                            v
 *   [1] --> [2] --> [3] --> [4] --> NULL
 *
 * ----------------------------------------------------------------------------
 * LOOP EXITS: curr->next == NULL
 * ----------------------------------------------------------------------------
 *
 *   FINAL RESULT: 1 -> 2 -> 3 -> 4 -> NULL
 *
 * ============================================================================
 * KEY INSIGHT: Why we DON'T advance after deletion
 * ============================================================================
 *
 *   Consider: [1] --> [1] --> [1] --> [2]
 *
 *   If we advance after deleting:
 *   - Delete first duplicate: [1] --> [1] --> [2], advance to second [1]
 *   - Now we skip comparing first [1] with new second [1]
 *   - BUG: We'd end up with [1] --> [1] --> [2]
 *
 *   By NOT advancing, we re-check the same curr against its new next!
 *
 * ============================================================================
 */

/*
 * ============================================================================
 * REMOVE_DUPLICATES FUNCTION - Line by Line Explanation
 * ============================================================================
 *
 * STRATEGY: Skip adjacent duplicates (works because list is SORTED!)
 *   - For each node, check if next node has same value
 *   - If same: skip the next node (delete it)
 *   - If different: move to next node
 *
 * KEY INSIGHT: Since list is SORTED, all duplicates are ADJACENT!
 *   [1, 1, 2, 3, 3, 3, 4] - duplicates are next to each other
 *   So we only need to compare current with next, not search entire list.
 *
 * IMPORTANT: After deleting, DON'T advance! The new next might also be duplicate.
 *
 * ============================================================================
 */
Node* remove_duplicates(Node* head) {
    /*
     * NULL CHECK
     * -----------
     * Say: "Empty list has no duplicates to remove"
     */
    if (head == NULL) {
        return NULL;  // Say: "Nothing to process"
    }

    /*
     * INITIALIZE TRAVERSAL POINTER
     * -----------------------------
     * Say: "Start at head, will iterate through the list"
     */
    Node* current = head;  // Say: "current tracks our position"

    /*
     * MAIN LOOP: Process each node
     * -----------------------------
     * Say: "Continue while current and next both exist"
     *
     * Condition: current != NULL && current->next != NULL
     *   - current != NULL: We're not past the end
     *   - current->next != NULL: There's a next node to compare with
     */
    while (current != NULL && current->next != NULL) {
        /*
         * DUPLICATE CHECK
         * ----------------
         * Say: "Compare current value with next value"
         *
         * If equal: next is a duplicate, remove it
         * If different: move to next node
         */
        if (current->data == current->next->data) {
            /*
             * FOUND DUPLICATE - Remove the next node
             * ---------------------------------------
             * Say: "Next node is duplicate, skip over it"
             *
             * Steps:
             * 1. Save pointer to duplicate (to free memory)
             * 2. Link current to skip over duplicate
             * 3. Free the duplicate node's memory
             */
            Node* duplicate = current->next;     // Say: "Save pointer to duplicate"
            current->next = current->next->next; // Say: "Skip over duplicate"
            free(duplicate);                     // Say: "Free duplicate's memory"

            /*
             * IMPORTANT: DON'T advance current!
             * -----------------------------------
             * Say: "Stay at current to check new next node"
             *
             * Why? The new next might ALSO be a duplicate!
             * Example: 1 -> 1 -> 1 -> 2
             *          ^
             *       current
             *
             * After removing first duplicate: 1 -> 1 -> 2
             *                                  ^
             *                               current
             *
             * We need to check again! 1 == 1, another duplicate!
             */
        } else {
            /*
             * NO DUPLICATE - Move to next node
             * ---------------------------------
             * Say: "Values differ, advance to next"
             *
             * Only advance when current and next are different.
             */
            current = current->next;  // Say: "Move to next node"
        }
    }

    return head;  // Say: "Return head of de-duplicated list"
}

/*
 * ============================================================================
 * REMOVE_ALL_DUPLICATES FUNCTION - Line by Line (Harder Variant!)
 * ============================================================================
 *
 * DIFFERENT PROBLEM: Remove ALL occurrences of duplicated values!
 *   Input:  1 -> 1 -> 2 -> 3 -> 3 -> 4
 *   Output: 2 -> 4  (only values that appear ONCE)
 *
 * WHY IS THIS HARDER?
 *   - Head might be removed (if head value is duplicated)
 *   - Need to track if ANY duplicates were found for a value
 *   - Need prev pointer to reconnect after removing all copies
 *
 * STRATEGY:
 *   1. Use dummy node (head might be removed)
 *   2. For each value, delete ALL adjacent duplicates
 *   3. If any duplicates found, delete the first occurrence too
 *   4. Use prev pointer to maintain list connectivity
 *
 * ============================================================================
 */
Node* remove_all_duplicates(Node* head) {
    /*
     * DUMMY NODE SETUP
     * -----------------
     * Say: "Dummy node handles case where head is removed"
     *
     * Problem: If head value (1) is duplicated, we need to remove head!
     * Solution: Create dummy node that points to head.
     *           At the end, return dummy.next (new head).
     *
     * MEMORY NOTE:
     *   Node dummy;        <- Stack-allocated struct (use '.')
     *   Node* prev = &dummy; <- Pointer to dummy (use '->')
     */
    Node dummy;            // Say: "Stack struct, NOT a pointer!"
    dummy.next = head;     // Say: "Use '.' because dummy IS the struct"
    Node* prev = &dummy;   // Say: "prev POINTS TO dummy, use '->'"
    Node* current = head;  // Say: "current traverses the list"

    /*
     * MAIN LOOP: Process each unique value
     * -------------------------------------
     * Say: "Continue until we've checked all nodes"
     */
    while (current != NULL) {
        /*
         * PHASE 1: Remove all adjacent duplicates of current value
         * ----------------------------------------------------------
         * Say: "Delete any nodes with same value as current"
         *
         * has_duplicate tracks if we found ANY duplicates.
         * If true, we must also delete current itself later.
         */
        bool has_duplicate = false;  // Say: "Track if duplicates found"

        /*
         * Inner loop: Delete adjacent duplicates
         * ---------------------------------------
         * Say: "While next exists and has same value, delete it"
         */
        while (current->next != NULL && current->data == current->next->data) {
            has_duplicate = true;  // Say: "Found at least one duplicate"

            /*
             * DELETE THE DUPLICATE
             * ---------------------
             * Same as remove_duplicates: save, skip, free
             */
            Node* temp = current->next;         // Say: "Save duplicate"
            current->next = current->next->next; // Say: "Skip duplicate"
            free(temp);                          // Say: "Free memory"
        }
        // Say: "All adjacent duplicates of current value removed"

        /*
         * PHASE 2: Handle current node itself
         * ------------------------------------
         */
        if (has_duplicate) {
            /*
             * CURRENT IS ALSO DUPLICATE - Remove it!
             * ---------------------------------------
             * Say: "Current's value appeared multiple times, remove it too"
             *
             * prev->next skips over current to current->next
             *
             * Example: After phase 1, we have:
             *   prev -> [1] -> [2] -> ...
             *            ^
             *         current (value 1 had duplicates)
             *
             * After: prev -> [2] -> ...
             */
            prev->next = current->next;  // Say: "prev skips over current"
            free(current);               // Say: "Free current node"
            current = prev->next;        // Say: "Move to next value"
            // Say: "DON'T advance prev! It already points to next section"
        } else {
            /*
             * CURRENT IS UNIQUE - Keep it!
             * -----------------------------
             * Say: "No duplicates, advance both pointers"
             */
            prev = current;           // Say: "prev moves to current"
            current = current->next;  // Say: "current moves forward"
        }
    }

    /*
     * RETURN NEW HEAD
     * -----------------
     * Say: "dummy.next is the new head (might be different from original!)"
     *
     * Use '.' because dummy is a struct, not a pointer.
     */
    return dummy.next;
}

/* ==================== TEST ==================== */

int main() {
    // Test case 1: Multiple duplicates
    // 1 -> 1 -> 2 -> 3 -> 3 -> 3 -> 4
    Node* list1 = create_node(1);
    list1->next = create_node(1);
    list1->next->next = create_node(2);
    list1->next->next->next = create_node(3);
    list1->next->next->next->next = create_node(3);
    list1->next->next->next->next->next = create_node(3);
    list1->next->next->next->next->next->next = create_node(4);

    printf("Original: ");
    print_list(list1);
    list1 = remove_duplicates(list1);
    printf("Remove duplicates (keep first): ");
    print_list(list1);

    // Test case 2: All same values
    Node* list2 = create_node(5);
    list2->next = create_node(5);
    list2->next->next = create_node(5);

    printf("\nOriginal: ");
    print_list(list2);
    list2 = remove_duplicates(list2);
    printf("After removal: ");
    print_list(list2);

    // Test case 3: No duplicates
    Node* list3 = create_node(1);
    list3->next = create_node(2);
    list3->next->next = create_node(3);

    printf("\nOriginal: ");
    print_list(list3);
    list3 = remove_duplicates(list3);
    printf("After removal: ");
    print_list(list3);

    // Test case 4: Remove ALL duplicates variant
    // 1 -> 1 -> 2 -> 3 -> 3 -> 4
    // Should become: 2 -> 4 (only unique values kept)
    Node* list4 = create_node(1);
    list4->next = create_node(1);
    list4->next->next = create_node(2);
    list4->next->next->next = create_node(3);
    list4->next->next->next->next = create_node(3);
    list4->next->next->next->next->next = create_node(4);

    printf("\nOriginal: ");
    print_list(list4);
    list4 = remove_all_duplicates(list4);
    printf("Remove ALL duplicates (keep only unique): ");
    print_list(list4);

    // Test case 5: Single node
    Node* list5 = create_node(1);
    printf("\nSingle node: ");
    print_list(list5);
    list5 = remove_duplicates(list5);
    printf("After removal: ");
    print_list(list5);

    return 0;
}

/*
 * ==================== INTERVIEW EXPLANATION ====================
 *
 * PROBLEM: Remove duplicate values from a SORTED linked list.
 *
 * KEY INSIGHT: Since list is sorted, duplicates are adjacent!
 *
 * APPROACH - Skip Duplicates:
 *
 * Visual:
 *   1 -> 1 -> 2 -> 3 -> 3 -> 4
 *   ^
 *  curr
 *
 *   curr->data == curr->next->data (1 == 1)
 *   Skip: curr->next = curr->next->next
 *
 *   1 -> 2 -> 3 -> 3 -> 4
 *   ^
 *  curr
 *
 *   curr->data != curr->next->data (1 != 2)
 *   Advance: curr = curr->next
 *
 *   Continue until end...
 *
 * WHY NOT ADVANCE AFTER DELETION:
 * - After removing duplicate, check new next node
 * - It might also be a duplicate (e.g., 1 -> 1 -> 1)
 * - Only advance when values differ
 *
 * VARIANT: Remove ALL Nodes with Duplicates
 *
 * [1,1,2,3,3,4] -> [2,4]
 *
 * Differences:
 * - Need to track if ANY duplicates found for current value
 * - Remove ALL nodes with that value (including first occurrence)
 * - Need dummy node (head might be removed)
 * - Need prev pointer to reconnect
 *
 * EDGE CASES:
 * 1. Empty list -> Return NULL
 * 2. Single node -> No duplicates possible
 * 3. All same values -> Return single node (or NULL for variant)
 * 4. No duplicates -> Return unchanged
 * 5. Duplicates at head
 * 6. Duplicates at tail
 *
 * UNSORTED LIST APPROACH:
 * - Can't use adjacency trick
 * - Option 1: Sort first O(n log n), then remove O(n)
 * - Option 2: Hash set O(n) time, O(n) space
 * - Option 3: Nested loop O(n^2) time, O(1) space
 *
 * COMMON MISTAKES:
 * - Advancing pointer after deletion (may miss consecutive dupes)
 * - Forgetting to free deleted nodes (memory leak)
 * - Not handling head being a duplicate (for variant)
 *
 * INTERVIEW TIP:
 * - Ask: "Is the list sorted?" (changes approach significantly)
 * - Ask: "Remove duplicates or remove ALL occurrences?"
 * - Mention memory management (free deleted nodes)
 */
