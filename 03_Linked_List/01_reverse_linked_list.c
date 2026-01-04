/*
 * ============================================================================
 * PROBLEM: Reverse a Linked List
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH (MUST KNOW!)
 *
 * Reverse a singly linked list in-place.
 *
 * Example:
 * Input:  1 -> 2 -> 3 -> 4 -> 5 -> NULL
 * Output: 5 -> 4 -> 3 -> 2 -> 1 -> NULL
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. LINKED LIST BASICS:
 *    - Each node has: data + pointer to next
 *    - Head points to first node
 *    - Last node points to NULL
 *
 * 2. THREE POINTER TECHNIQUE:
 *    - prev: tracks previous node (starts NULL)
 *    - curr: current node being processed
 *    - next: temporarily stores next node
 *
 * 3. REVERSE DIRECTION:
 *    - Change curr->next to point to prev
 *    - Move all pointers one step forward
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * Original: 1 -> 2 -> 3 -> NULL
 *
 * Initial state:
 *   prev = NULL
 *   curr = 1
 *   next = ?
 *
 *   NULL   1 -> 2 -> 3 -> NULL
 *    ^     ^
 *   prev  curr
 *
 * Step 1: Save next, reverse link
 *   next = curr->next (save 2)
 *   curr->next = prev (1 points to NULL)
 *   prev = curr (prev moves to 1)
 *   curr = next (curr moves to 2)
 *
 *   NULL <- 1    2 -> 3 -> NULL
 *           ^    ^
 *          prev curr
 *
 * Step 2: Repeat
 *   next = 3
 *   curr->next = prev (2 points to 1)
 *   prev = 2
 *   curr = 3
 *
 *   NULL <- 1 <- 2    3 -> NULL
 *                ^    ^
 *               prev curr
 *
 * Step 3: Repeat
 *   next = NULL
 *   curr->next = prev (3 points to 2)
 *   prev = 3
 *   curr = NULL
 *
 *   NULL <- 1 <- 2 <- 3
 *                     ^
 *                    prev (new head!)
 *
 * curr is NULL, STOP
 * Return prev as new head
 *
 * Result: 3 -> 2 -> 1 -> NULL
 *
 * ============================================================================
 * POINTER MOVEMENT VISUALIZATION:
 * ============================================================================
 *
 *   +------+    +------+    +------+
 *   |  1   | -> |  2   | -> |  3   | -> NULL
 *   +------+    +------+    +------+
 *
 *   Before each iteration:
 *   1. Save: next = curr->next
 *   2. Reverse: curr->next = prev
 *   3. Move prev: prev = curr
 *   4. Move curr: curr = next
 *
 *   +------+    +------+    +------+
 *   |  1   | <- |  2   | <- |  3   |
 *   +------+    +------+    +------+
 *   NULL <-          new head ->
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   prev = NULL
 *   curr = head
 *
 *   while (curr != NULL):
 *       next = curr->next     // Save
 *       curr->next = prev     // Reverse
 *       prev = curr           // Move prev
 *       curr = next           // Move curr
 *
 *   return prev  // New head
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Visit each node exactly once
 * - Constant operations per node
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only three pointers
 * - In-place reversal
 *
 * ============================================================================
 * RECURSIVE VERSION:
 * ============================================================================
 *
 *   Node* reverseRecursive(Node* head) {
 *       if (head == NULL || head->next == NULL)
 *           return head;
 *
 *       Node* newHead = reverseRecursive(head->next);
 *       head->next->next = head;
 *       head->next = NULL;
 *
 *       return newHead;
 *   }
 *
 *   Space: O(n) due to recursion stack
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Node structure
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Iterative reversal (PREFERRED)
Node* reverseIterative(Node* head) {
    // Initialize previous pointer to NULL
    // Say: "I'll use three pointers: prev, curr, and next"
    Node* prev = NULL;

    // Current pointer starts at head
    // Say: "Current starts at the head of the list"
    Node* curr = head;

    // Next pointer for temporary storage
    // Say: "Next will temporarily store the node ahead"
    Node* next = NULL;

    // Traverse until we reach the end
    // Say: "I'll iterate through the list until curr becomes NULL"
    while (curr != NULL) {
        // Save the next node before we change pointers
        // Say: "First, save the next node so we don't lose the rest of the list"
        next = curr->next;

        // Reverse the current node's pointer
        // Say: "Now reverse the link by pointing current's next to prev"
        curr->next = prev;

        // Move prev forward one step
        // Say: "Move prev forward to current node"
        prev = curr;

        // Move curr forward one step
        // Say: "Move curr forward to the next node we saved earlier"
        curr = next;
    }

    // When loop ends, prev points to new head
    // Say: "When we exit the loop, prev is pointing to the new head"
    return prev;
}

// Recursive reversal
Node* reverseRecursive(Node* head) {
    // Base case: empty list or single node
    // Say: "Base case: if empty or single node, just return it"
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // Recursively reverse rest of list
    // Say: "Recursively reverse everything after the current node"
    Node* newHead = reverseRecursive(head->next);

    // Make next node point back to current
    // Say: "Make the next node point back to me"
    head->next->next = head;

    // Current node becomes tail, points to NULL
    // Say: "Current node is now at the end, so point to NULL"
    head->next = NULL;

    // Return the new head
    // Say: "Return the new head which came from recursion"
    return newHead;
}

// Print linked list
void printList(Node* head) {
    Node* curr = head;
    while (curr != NULL) {
        printf("%d", curr->data);
        if (curr->next != NULL) {
            printf(" -> ");
        }
        curr = curr->next;
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
    printf("=== Reverse Linked List ===\n\n");

    // Test iterative
    int arr1[] = {1, 2, 3, 4, 5};
    Node* list1 = createList(arr1, 5);

    printf("Original: ");
    printList(list1);

    list1 = reverseIterative(list1);
    printf("Reversed (iterative): ");
    printList(list1);

    // Test recursive
    int arr2[] = {10, 20, 30};
    Node* list2 = createList(arr2, 3);

    printf("\nOriginal: ");
    printList(list2);

    list2 = reverseRecursive(list2);
    printf("Reversed (recursive): ");
    printList(list2);

    // Single element
    int arr3[] = {42};
    Node* list3 = createList(arr3, 1);

    printf("\nSingle element: ");
    printList(list3);
    list3 = reverseIterative(list3);
    printf("Reversed: ");
    printList(list3);

    return 0;
}
