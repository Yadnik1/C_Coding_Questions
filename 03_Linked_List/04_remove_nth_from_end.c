/*
 * ============================================================================
 * PROBLEM: Remove Nth Node from End of List
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: High
 *
 * Remove the nth node from the end of a linked list.
 *
 * Example:
 * Input:  1 -> 2 -> 3 -> 4 -> 5, n = 2
 * Output: 1 -> 2 -> 3 -> 5 (removed 4)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. TWO-POINTER GAP TECHNIQUE:
 *    - Maintain n-node gap between two pointers
 *    - When first pointer reaches end, second is at target
 *
 * 2. DUMMY HEAD:
 *    - Use dummy node before head
 *    - Handles edge case of removing first node
 *
 * 3. ONE-PASS SOLUTION:
 *    - Don't need to count length first
 *    - Use gap technique
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * List: 1 -> 2 -> 3 -> 4 -> 5 -> NULL, n = 2
 *
 * Add dummy: D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *
 * Step 1: Advance 'first' by n+1 steps (n=2, so 3 steps)
 *
 *   D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *   ^              ^
 *  second        first
 *
 *   Gap = 3 nodes
 *
 * Step 2: Move both until first reaches NULL
 *
 *   D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *        ^              ^
 *       second        first
 *
 *   D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *             ^              ^
 *            second        first
 *
 *   D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *                  ^              ^
 *                second         first=NULL
 *
 * Step 3: Remove second->next (which is node 4)
 *
 *   second->next = second->next->next
 *   3->next = 5
 *
 *   D -> 1 -> 2 -> 3 ------> 5 -> NULL
 *                            ^
 *                        (4 removed)
 *
 * Result: 1 -> 2 -> 3 -> 5 -> NULL
 *
 * ============================================================================
 * WHY USE DUMMY NODE:
 * ============================================================================
 *
 * Without dummy, removing first node is tricky:
 *
 *   List: 1 -> 2, n = 2 (remove node 1)
 *
 *   With dummy:
 *   D -> 1 -> 2 -> NULL
 *   ^    ^
 *  sec  first (after n+1 steps)
 *
 *   Move until first = NULL:
 *   D -> 1 -> 2 -> NULL
 *   ^              ^
 *  sec           first
 *
 *   sec->next = sec->next->next
 *   D -> 2 -> NULL
 *
 *   Return dummy->next (which is 2)
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   dummy = new Node(0)
 *   dummy->next = head
 *
 *   first = dummy
 *   second = dummy
 *
 *   // Advance first by n+1 steps
 *   for i = 0 to n:
 *       first = first->next
 *
 *   // Move both until first is NULL
 *   while (first != NULL):
 *       first = first->next
 *       second = second->next
 *
 *   // Remove node after second
 *   second->next = second->next->next
 *
 *   return dummy->next
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Single pass through list
 * - L nodes, where L is list length
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only two pointers and dummy node
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
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node* removeNthFromEnd(Node* head, int n) {
    // Create dummy node
    Node* dummy = createNode(0);
    dummy->next = head;

    Node* first = dummy;
    Node* second = dummy;

    // Advance first by n+1 steps
    for (int i = 0; i <= n; i++) {
        first = first->next;
    }

    // Move both until first reaches NULL
    while (first != NULL) {
        first = first->next;
        second = second->next;
    }

    // Remove the node after second
    Node* toDelete = second->next;
    second->next = second->next->next;
    free(toDelete);

    // Get new head and free dummy
    Node* newHead = dummy->next;
    free(dummy);

    return newHead;
}

void printList(Node* head) {
    while (head != NULL) {
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
    printf("=== Remove Nth Node from End ===\n\n");

    // Test 1: Remove 2nd from end
    int arr1[] = {1, 2, 3, 4, 5};
    Node* list1 = createList(arr1, 5);

    printf("Original: ");
    printList(list1);
    printf("Remove 2nd from end\n");

    list1 = removeNthFromEnd(list1, 2);
    printf("Result:   ");
    printList(list1);

    // Test 2: Remove last element
    int arr2[] = {1, 2, 3};
    Node* list2 = createList(arr2, 3);

    printf("\nOriginal: ");
    printList(list2);
    printf("Remove 1st from end (last)\n");

    list2 = removeNthFromEnd(list2, 1);
    printf("Result:   ");
    printList(list2);

    // Test 3: Remove first element
    int arr3[] = {1, 2};
    Node* list3 = createList(arr3, 2);

    printf("\nOriginal: ");
    printList(list3);
    printf("Remove 2nd from end (first)\n");

    list3 = removeNthFromEnd(list3, 2);
    printf("Result:   ");
    printList(list3);

    // Test 4: Single element
    int arr4[] = {1};
    Node* list4 = createList(arr4, 1);

    printf("\nOriginal: ");
    printList(list4);
    printf("Remove 1st from end\n");

    list4 = removeNthFromEnd(list4, 1);
    printf("Result:   ");
    if (list4 == NULL) printf("NULL (empty list)\n");

    return 0;
}
