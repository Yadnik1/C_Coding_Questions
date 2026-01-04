/*
 * ============================================================================
 * PROBLEM: Find Middle Element of Linked List
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * Find the middle node of a singly linked list.
 * For even length, return the second middle node.
 *
 * Example:
 * Input:  1 -> 2 -> 3 -> 4 -> 5
 * Output: 3
 *
 * Input:  1 -> 2 -> 3 -> 4 -> 5 -> 6
 * Output: 4 (second middle)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. SLOW-FAST POINTER TECHNIQUE:
 *    - Slow moves 1 step
 *    - Fast moves 2 steps
 *    - When fast reaches end, slow is at middle
 *
 * 2. WHY IT WORKS:
 *    - Fast covers 2x distance of slow
 *    - When fast at end, slow at half = middle
 *
 * ============================================================================
 * VISUAL WALKTHROUGH (Odd Length):
 * ============================================================================
 *
 * List: 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *
 * Initial:
 *   slow = 1, fast = 1
 *   S,F
 *   1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *
 * Step 1:
 *   slow = 2 (move 1)
 *   fast = 3 (move 2)
 *
 *   1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *        S    F
 *
 * Step 2:
 *   slow = 3 (move 1)
 *   fast = 5 (move 2)
 *
 *   1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *             S         F
 *
 * Step 3:
 *   fast->next = NULL, STOP
 *
 * slow = 3 = MIDDLE!
 *
 * ============================================================================
 * VISUAL WALKTHROUGH (Even Length):
 * ============================================================================
 *
 * List: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL
 *
 * Initial:
 *   slow = 1, fast = 1
 *
 * Step 1:
 *   slow = 2, fast = 3
 *
 * Step 2:
 *   slow = 3, fast = 5
 *
 * Step 3:
 *   slow = 4, fast = NULL (after moving 2)
 *
 * fast = NULL, STOP
 * slow = 4 = SECOND MIDDLE!
 *
 *   1 -> 2 -> 3 -> 4 -> 5 -> 6
 *             ^    ^
 *           first second
 *           middle middle
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   slow = head
 *   fast = head
 *
 *   while (fast != NULL AND fast->next != NULL):
 *       slow = slow->next
 *       fast = fast->next->next
 *
 *   return slow  // Middle node
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Fast pointer traverses entire list once
 * - Actually n/2 iterations
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only two pointers
 *
 * ============================================================================
 * VARIATIONS:
 * ============================================================================
 *
 * 1. Return first middle for even length:
 *    - Start fast one step ahead: fast = head->next
 *
 * 2. Return middle value (not node):
 *    - Return slow->data
 *
 * 3. Delete middle node:
 *    - Need to track prev pointer too
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

// Find middle (returns second middle for even length)
Node* findMiddle(Node* head) {
    // Edge case: empty list
    // Say: "First check if the list is empty"
    if (head == NULL) return NULL;

    // Initialize slow pointer at head
    // Say: "I'll use the tortoise and hare technique with two pointers"
    Node* slow = head;

    // Initialize fast pointer at head
    // Say: "Both pointers start at the head"
    Node* fast = head;

    // Traverse until fast reaches end
    // Say: "Move slow by 1 and fast by 2 until fast reaches the end"
    while (fast != NULL && fast->next != NULL) {
        // Move slow one step
        // Say: "Slow moves one step forward"
        slow = slow->next;

        // Move fast two steps
        // Say: "Fast moves two steps forward"
        fast = fast->next->next;
    }

    // When fast reaches end, slow is at middle
    // Say: "When fast reaches the end, slow is at the middle"
    return slow;
}

// Find first middle for even length
Node* findFirstMiddle(Node* head) {
    // Edge case: empty list
    // Say: "Handle the edge case of an empty list"
    if (head == NULL) return NULL;

    // Initialize slow at head
    // Say: "For the first middle, I'll start slow at head"
    Node* slow = head;

    // Initialize fast one step ahead
    // Say: "And start fast one step ahead at head->next"
    Node* fast = head->next;

    // Traverse with adjusted starting position
    // Say: "This adjustment makes slow land on the first middle for even-length lists"
    while (fast != NULL && fast->next != NULL) {
        // Move slow one step
        slow = slow->next;

        // Move fast two steps
        fast = fast->next->next;
    }

    // Return first middle
    // Say: "Return slow which points to the first middle"
    return slow;
}

// Print list
void printList(Node* head) {
    while (head != NULL) {
        printf("%d", head->data);
        if (head->next) printf(" -> ");
        head = head->next;
    }
    printf(" -> NULL\n");
}

// Create list from array
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
    printf("=== Find Middle of Linked List ===\n\n");

    // Odd length
    int arr1[] = {1, 2, 3, 4, 5};
    Node* list1 = createList(arr1, 5);

    printf("List (odd): ");
    printList(list1);
    printf("Middle: %d\n\n", findMiddle(list1)->data);

    // Even length
    int arr2[] = {1, 2, 3, 4, 5, 6};
    Node* list2 = createList(arr2, 6);

    printf("List (even): ");
    printList(list2);
    printf("Second middle: %d\n", findMiddle(list2)->data);
    printf("First middle:  %d\n\n", findFirstMiddle(list2)->data);

    // Single element
    int arr3[] = {42};
    Node* list3 = createList(arr3, 1);

    printf("List (single): ");
    printList(list3);
    printf("Middle: %d\n\n", findMiddle(list3)->data);

    // Two elements
    int arr4[] = {1, 2};
    Node* list4 = createList(arr4, 2);

    printf("List (two): ");
    printList(list4);
    printf("Second middle: %d\n", findMiddle(list4)->data);
    printf("First middle:  %d\n", findFirstMiddle(list4)->data);

    return 0;
}
