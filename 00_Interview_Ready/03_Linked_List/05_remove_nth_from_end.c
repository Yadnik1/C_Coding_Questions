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

Node* remove_nth_from_end(Node* head, int n) {
    // Say: "I use a dummy node to handle edge case of removing head"
    Node dummy;
    dummy.next = head;

    Node* fast = &dummy;
    Node* slow = &dummy;

    // Say: "First, move fast pointer n+1 steps ahead"
    for (int i = 0; i <= n; i++) {
        if (fast == NULL) {
            return head;  // n is larger than list length
        }
        fast = fast->next;
    }

    // Say: "Now move both until fast reaches end"
    // Say: "The gap ensures slow stops at node BEFORE the one to delete"
    while (fast != NULL) {
        fast = fast->next;
        slow = slow->next;
    }

    // Say: "slow->next is the node to remove"
    Node* to_delete = slow->next;
    slow->next = slow->next->next;
    free(to_delete);

    return dummy.next;
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
