/*
 * ============================================================================
 * PROBLEM: Merge Two Sorted Linked Lists
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: Very High
 *
 * Merge two sorted linked lists into one sorted list.
 *
 * Example:
 * Input:  1 -> 2 -> 4 and 1 -> 3 -> 4
 * Output: 1 -> 1 -> 2 -> 3 -> 4 -> 4
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. MERGE TECHNIQUE (same as merge sort):
 *    - Compare heads of both lists
 *    - Take smaller one, advance that pointer
 *    - Repeat until one list exhausted
 *    - Attach remaining list
 *
 * 2. DUMMY HEAD:
 *    - Simplifies code by avoiding null checks
 *    - Return dummy->next as result
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * L1: 1 -> 3 -> 5 -> NULL
 * L2: 2 -> 4 -> 6 -> NULL
 *
 * dummy -> NULL, tail = dummy
 *
 * Step 1: Compare 1 vs 2
 *   1 < 2, take 1
 *   dummy -> 1 -> NULL
 *   L1: 3 -> 5 -> NULL
 *
 * Step 2: Compare 3 vs 2
 *   2 < 3, take 2
 *   dummy -> 1 -> 2 -> NULL
 *   L2: 4 -> 6 -> NULL
 *
 * Step 3: Compare 3 vs 4
 *   3 < 4, take 3
 *   dummy -> 1 -> 2 -> 3 -> NULL
 *
 * Step 4: Compare 5 vs 4
 *   4 < 5, take 4
 *   dummy -> 1 -> 2 -> 3 -> 4 -> NULL
 *
 * Step 5: Compare 5 vs 6
 *   5 < 6, take 5
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *
 * L1 exhausted, attach remaining L2:
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n + m)
 * ============================================================================
 * - n = length of list1, m = length of list2
 * - Each node visited once
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only create dummy node
 * - Reuse existing nodes
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

Node* mergeSortedLists(Node* l1, Node* l2) {
    // Create dummy node on stack
    // Say: "I'll create a dummy node to simplify the merge logic"
    Node dummy;

    // Initialize tail pointer
    // Say: "Tail will track the end of our merged list"
    Node* tail = &dummy;

    // Initialize dummy's next to NULL
    // Say: "Initialize the dummy's next pointer to NULL"
    dummy.next = NULL;

    // Merge while both lists have nodes
    // Say: "I'll compare nodes from both lists and attach the smaller one"
    while (l1 != NULL && l2 != NULL) {
        // Compare current nodes from both lists
        // Say: "Compare the current nodes from both lists"
        if (l1->data <= l2->data) {
            // L1's node is smaller or equal
            // Say: "L1's value is smaller, so attach it to our result"
            tail->next = l1;

            // Move l1 forward
            // Say: "Move l1 forward to its next node"
            l1 = l1->next;
        } else {
            // L2's node is smaller
            // Say: "L2's value is smaller, so attach it to our result"
            tail->next = l2;

            // Move l2 forward
            // Say: "Move l2 forward to its next node"
            l2 = l2->next;
        }

        // Move tail forward
        // Say: "Move tail forward to the node we just added"
        tail = tail->next;
    }

    // Attach remaining nodes from non-empty list
    // Say: "Attach whichever list still has remaining nodes"
    tail->next = (l1 != NULL) ? l1 : l2;

    // Return the merged list (skip dummy)
    // Say: "Return dummy's next which is the head of our merged list"
    return dummy.next;
}

void printList(Node* head) {
    while (head) {
        printf("%d", head->data);
        if (head->next) printf(" -> ");
        head = head->next;
    }
    printf(" -> NULL\n");
}

Node* createList(int arr[], int n) {
    if (n == 0) return NULL;
    Node* head = createNode(arr[0]);
    Node* curr = head;
    for (int i = 1; i < n; i++) {
        curr->next = createNode(arr[i]);
        curr = curr->next;
    }
    return head;
}

int main() {
    printf("=== Merge Two Sorted Lists ===\n\n");

    int arr1[] = {1, 3, 5};
    int arr2[] = {2, 4, 6};

    Node* l1 = createList(arr1, 3);
    Node* l2 = createList(arr2, 3);

    printf("List 1: ");
    printList(l1);
    printf("List 2: ");
    printList(l2);

    Node* merged = mergeSortedLists(l1, l2);
    printf("Merged: ");
    printList(merged);

    return 0;
}
