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

// Keep first occurrence of each value
Node* remove_duplicates(Node* head) {
    // Say: "I iterate with single pointer, skipping duplicate next nodes"
    if (head == NULL) {
        return NULL;
    }

    Node* current = head;

    while (current != NULL && current->next != NULL) {
        // Say: "If next value equals current, skip it"
        if (current->data == current->next->data) {
            Node* duplicate = current->next;
            current->next = current->next->next;
            free(duplicate);
            // Say: "Don't advance current - check new next node"
        } else {
            // Say: "Different value, move to next"
            current = current->next;
        }
    }

    return head;
}

// Variant: Remove ALL nodes that have duplicates (keep only unique)
Node* remove_all_duplicates(Node* head) {
    // Say: "Use dummy node to handle head being a duplicate"
    Node dummy;
    dummy.next = head;
    Node* prev = &dummy;
    Node* current = head;

    while (current != NULL) {
        // Check if current has duplicates
        bool has_duplicate = false;
        while (current->next != NULL && current->data == current->next->data) {
            has_duplicate = true;
            Node* temp = current->next;
            current->next = current->next->next;
            free(temp);
        }

        if (has_duplicate) {
            // Say: "Current itself is a duplicate, remove it"
            prev->next = current->next;
            free(current);
            current = prev->next;
        } else {
            // Say: "Current is unique, advance both pointers"
            prev = current;
            current = current->next;
        }
    }

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
