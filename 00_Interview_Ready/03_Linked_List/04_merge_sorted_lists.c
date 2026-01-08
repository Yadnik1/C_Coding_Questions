/*
 * ============================================================================
 * PROBLEM: Merge Two Sorted Linked Lists
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given the heads of two sorted linked lists, merge them into one sorted
 * list. The merged list should be made by splicing together the nodes of
 * the first two lists (not creating new nodes).
 *
 * EXAMPLES:
 *   Input:  List 1: 1 -> 3 -> 5
 *           List 2: 2 -> 4 -> 6
 *   Output: 1 -> 2 -> 3 -> 4 -> 5 -> 6
 *
 *   Input:  List 1: 1 -> 2 -> 4
 *           List 2: 1 -> 3 -> 4
 *   Output: 1 -> 1 -> 2 -> 3 -> 4 -> 4 (duplicates preserved)
 *
 *   Input:  List 1: NULL (empty)
 *           List 2: 1 -> 2
 *   Output: 1 -> 2 (return the non-empty list)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 *   - Core operation in Merge Sort for linked lists
 *   - Tests two-pointer technique with different data structures
 *   - Shows understanding of the "dummy head" pattern
 *   - Foundation for "Merge K Sorted Lists" (harder variant)
 *   - Real-world: merging sorted data streams
 *
 * KEY CONCEPT: Two Pointer Merge with Dummy Head
 *   Use a dummy node to simplify edge cases:
 *   - No special handling for "first node"
 *   - Always have a valid "tail" to append to
 *   - Return dummy.next as the actual head
 *
 *   Compare heads of both lists, take the smaller one,
 *   advance that list's pointer, repeat until one list is empty,
 *   then attach the remaining list.
 *
 * VISUAL:
 *
 *   l1: 1 -> 3 -> 5
 *       ^
 *   l2: 2 -> 4 -> 6
 *       ^
 *
 *   dummy -> ?     (tail points to dummy)
 *
 *   Step 1: 1 < 2, take 1
 *   dummy -> 1     l1 advances to 3
 *
 *   Step 2: 3 > 2, take 2
 *   dummy -> 1 -> 2     l2 advances to 4
 *
 *   Step 3: 3 < 4, take 3
 *   dummy -> 1 -> 2 -> 3     l1 advances to 5
 *
 *   Step 4: 5 > 4, take 4
 *   dummy -> 1 -> 2 -> 3 -> 4     l2 advances to 6
 *
 *   Step 5: 5 < 6, take 5
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5     l1 is now NULL
 *
 *   Step 6: l1 is NULL, attach remaining l2
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> 6
 *
 *   Return: dummy.next = 1 (actual merged head)
 *
 * WHY DUMMY HEAD?
 *   +-------+
 *   | dummy | -> merged list starts here
 *   +-------+
 *       ^
 *      tail (always points to last node of merged list)
 *
 *   Without dummy: Need "if (head == NULL)" check for first insertion
 *   With dummy: tail->next = newNode always works!
 *
 * ============================================================================
 */

/*
 * MERGE TWO SORTED LINKED LISTS - Two Pointer Merge
 *
 * Time Complexity: O(n + m) - Visit each node once
 * Space Complexity: O(1) - Reuse existing nodes
 *
 * Pattern: Two Pointer Merge with Dummy Head
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
 * DRY-RUN DIAGRAM: Merge Two Sorted Lists
 * ============================================================================
 *
 * INPUT:
 *   l1: [1] --> [3] --> [5] --> NULL
 *   l2: [2] --> [4] --> [6] --> NULL
 *
 * INITIAL STATE:
 *
 *   dummy --> NULL
 *     ^
 *    tail
 *
 *   l1: [1] --> [3] --> [5] --> NULL
 *        ^
 *       l1
 *
 *   l2: [2] --> [4] --> [6] --> NULL
 *        ^
 *       l2
 *
 * ----------------------------------------------------------------------------
 * ITERATION 1: Compare l1->data (1) vs l2->data (2)
 * ----------------------------------------------------------------------------
 *   1 <= 2, so attach l1 (node 1)
 *
 *   tail->next = l1 (dummy points to 1)
 *   l1 = l1->next (l1 moves to 3)
 *   tail = tail->next (tail moves to 1)
 *
 *   dummy --> [1]
 *              ^
 *             tail
 *
 *   l1: [3] --> [5] --> NULL
 *        ^
 *       l1
 *
 *   l2: [2] --> [4] --> [6] --> NULL
 *        ^
 *       l2
 *
 * ----------------------------------------------------------------------------
 * ITERATION 2: Compare l1->data (3) vs l2->data (2)
 * ----------------------------------------------------------------------------
 *   3 > 2, so attach l2 (node 2)
 *
 *   tail->next = l2 (1 points to 2)
 *   l2 = l2->next (l2 moves to 4)
 *   tail = tail->next (tail moves to 2)
 *
 *   dummy --> [1] --> [2]
 *                      ^
 *                     tail
 *
 *   l1: [3] --> [5] --> NULL
 *   l2: [4] --> [6] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 3: Compare l1->data (3) vs l2->data (4)
 * ----------------------------------------------------------------------------
 *   3 <= 4, so attach l1 (node 3)
 *
 *   dummy --> [1] --> [2] --> [3]
 *                              ^
 *                             tail
 *
 *   l1: [5] --> NULL
 *   l2: [4] --> [6] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 4: Compare l1->data (5) vs l2->data (4)
 * ----------------------------------------------------------------------------
 *   5 > 4, so attach l2 (node 4)
 *
 *   dummy --> [1] --> [2] --> [3] --> [4]
 *                                      ^
 *                                     tail
 *
 *   l1: [5] --> NULL
 *   l2: [6] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 5: Compare l1->data (5) vs l2->data (6)
 * ----------------------------------------------------------------------------
 *   5 <= 6, so attach l1 (node 5)
 *
 *   dummy --> [1] --> [2] --> [3] --> [4] --> [5]
 *                                              ^
 *                                             tail
 *
 *   l1: NULL (exhausted!)
 *   l2: [6] --> NULL
 *
 * ----------------------------------------------------------------------------
 * LOOP EXITS: l1 == NULL
 * ----------------------------------------------------------------------------
 *   Attach remaining l2: tail->next = l2
 *
 *   dummy --> [1] --> [2] --> [3] --> [4] --> [5] --> [6] --> NULL
 *
 *   Return dummy.next = [1]
 *
 *   RESULT: 1 -> 2 -> 3 -> 4 -> 5 -> 6
 *
 * ============================================================================
 */

Node* merge_sorted_lists(Node* l1, Node* l2) {
    // Say: "I use a dummy head to simplify edge cases"
    Node dummy;
    dummy.next = NULL;
    Node* tail = &dummy;

    // Say: "Compare heads, attach smaller one, advance that pointer"
    while (l1 != NULL && l2 != NULL) {
        if (l1->data <= l2->data) {
            // Say: "l1 is smaller, attach it"
            tail->next = l1;
            l1 = l1->next;
        } else {
            // Say: "l2 is smaller, attach it"
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }

    // Say: "Attach remaining nodes (one list might be longer)"
    if (l1 != NULL) {
        tail->next = l1;
    } else {
        tail->next = l2;
    }

    // Say: "Return dummy.next, which is the real head"
    return dummy.next;
}

/* ==================== TEST ==================== */

int main() {
    // Test case 1: Normal merge
    // List 1: 1 -> 3 -> 5
    Node* l1 = create_node(1);
    l1->next = create_node(3);
    l1->next->next = create_node(5);

    // List 2: 2 -> 4 -> 6
    Node* l2 = create_node(2);
    l2->next = create_node(4);
    l2->next->next = create_node(6);

    printf("List 1: ");
    print_list(l1);
    printf("List 2: ");
    print_list(l2);

    Node* merged = merge_sorted_lists(l1, l2);
    printf("Merged: ");
    print_list(merged);

    // Test case 2: Lists with different lengths
    Node* l3 = create_node(1);
    l3->next = create_node(2);

    Node* l4 = create_node(3);
    l4->next = create_node(4);
    l4->next->next = create_node(5);
    l4->next->next->next = create_node(6);

    printf("\nList 3: ");
    print_list(l3);
    printf("List 4: ");
    print_list(l4);
    printf("Merged: ");
    print_list(merge_sorted_lists(l3, l4));

    // Test case 3: One empty list
    Node* l5 = create_node(1);
    l5->next = create_node(2);

    printf("\nList 5: ");
    print_list(l5);
    printf("Empty list: ");
    print_list(NULL);
    printf("Merged: ");
    print_list(merge_sorted_lists(l5, NULL));

    // Test case 4: Lists with duplicates
    Node* l6 = create_node(1);
    l6->next = create_node(2);
    l6->next->next = create_node(2);

    Node* l7 = create_node(1);
    l7->next = create_node(3);

    printf("\nList 6: ");
    print_list(l6);
    printf("List 7: ");
    print_list(l7);
    printf("Merged: ");
    print_list(merge_sorted_lists(l6, l7));

    return 0;
}

/*
 * ==================== INTERVIEW EXPLANATION ====================
 *
 * PROBLEM: Merge two sorted linked lists into one sorted list.
 *
 * APPROACH - Two Pointer with Dummy Head:
 *
 * Visual:
 *   l1: 1 -> 3 -> 5
 *   l2: 2 -> 4 -> 6
 *
 *   dummy -> ?
 *     ^
 *    tail
 *
 *   Step 1: 1 < 2, attach 1:  dummy -> 1
 *   Step 2: 3 > 2, attach 2:  dummy -> 1 -> 2
 *   Step 3: 3 < 4, attach 3:  dummy -> 1 -> 2 -> 3
 *   ... continue until one list is exhausted
 *   Final step: Attach remaining nodes
 *
 * WHY DUMMY HEAD:
 * - Eliminates special case for first node
 * - No need to check "is head NULL?"
 * - tail->next always valid
 * - Return dummy.next (skip dummy itself)
 *
 * WHY STACK-ALLOCATED DUMMY (Node dummy vs Node* dummy):
 * - No malloc needed, no memory leak
 * - Simpler cleanup
 * - Valid in this scope since we only need dummy.next
 *
 * EDGE CASES:
 * 1. Both lists empty -> Return NULL
 * 2. One list empty -> Return the other
 * 3. Lists of different lengths -> Handle remaining
 * 4. Duplicate values -> Both get included (stable merge)
 *
 * COMMON MISTAKES:
 * - Forgetting to advance tail pointer
 * - Not handling remaining elements after loop
 * - Memory leak if using malloc for dummy
 * - Returning &dummy instead of dummy.next
 *
 * RECURSIVE ALTERNATIVE:
 *   Node* merge(l1, l2) {
 *       if (!l1) return l2;
 *       if (!l2) return l1;
 *       if (l1->data <= l2->data) {
 *           l1->next = merge(l1->next, l2);
 *           return l1;
 *       } else {
 *           l2->next = merge(l1, l2->next);
 *           return l2;
 *       }
 *   }
 * - Elegant but O(n+m) space for recursion stack
 *
 * APPLICATIONS:
 * 1. Merge Sort on linked lists
 * 2. Merging K sorted lists (divide and conquer)
 * 3. External sorting (merge sorted chunks)
 *
 * FOLLOW-UP: Merge K sorted lists?
 * - Approach 1: Merge pairs iteratively O(N log k)
 * - Approach 2: Min-heap of k list heads O(N log k)
 */
