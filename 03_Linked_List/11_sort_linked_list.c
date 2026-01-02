/*
 * ============================================================================
 * PROBLEM: Sort Linked List (Merge Sort)
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: HIGH
 *
 * Sort a linked list in O(n log n) time using merge sort.
 *
 * ============================================================================
 * WHY MERGE SORT FOR LINKED LISTS?
 * ============================================================================
 *
 * - Quick sort: Bad because random access is O(n)
 * - Merge sort: Perfect because we only need sequential access
 * - Splitting: Easy with slow/fast pointer to find middle
 * - Merging: Easy with linked structure
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 * 1. Find middle of list (slow/fast pointer)
 * 2. Split into two halves
 * 3. Recursively sort both halves
 * 4. Merge two sorted halves
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   Original: 4 → 2 → 1 → 3
 *
 *   Split:    4 → 2    1 → 3
 *
 *   Split:    4   2    1   3
 *
 *   Merge:    2 → 4    1 → 3
 *
 *   Merge:    1 → 2 → 3 → 4
 *
 * ============================================================================
 * TIME: O(n log n) | SPACE: O(log n) recursion stack
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

// Find middle node (slow/fast pointer)
Node* getMiddle(Node* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    Node* slow = head;
    Node* fast = head->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

// Merge two sorted lists
Node* merge(Node* left, Node* right) {
    // Dummy node to simplify code
    Node dummy;
    dummy.next = NULL;
    Node* tail = &dummy;

    while (left != NULL && right != NULL) {
        if (left->data <= right->data) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    // Attach remaining nodes
    if (left != NULL) {
        tail->next = left;
    } else {
        tail->next = right;
    }

    return dummy.next;
}

// Merge sort for linked list
Node* mergeSort(Node* head) {
    // Base case: empty or single node
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // Find middle
    Node* middle = getMiddle(head);
    Node* rightHalf = middle->next;
    middle->next = NULL;  // Split the list

    // Recursively sort both halves
    Node* left = mergeSort(head);
    Node* right = mergeSort(rightHalf);

    // Merge sorted halves
    return merge(left, right);
}

// Utility functions
void printList(Node* head) {
    while (head != NULL) {
        printf("%d", head->data);
        head = head->next;
        if (head) printf(" → ");
    }
    printf(" → NULL\n");
}

Node* createList(int arr[], int n) {
    if (n == 0) return NULL;

    Node* head = createNode(arr[0]);
    Node* current = head;

    for (int i = 1; i < n; i++) {
        current->next = createNode(arr[i]);
        current = current->next;
    }

    return head;
}

void freeList(Node* head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    printf("=== Sort Linked List (Merge Sort) ===\n\n");

    // Test 1
    int arr1[] = {4, 2, 1, 3};
    Node* list1 = createList(arr1, 4);
    printf("Before: "); printList(list1);
    list1 = mergeSort(list1);
    printf("After:  "); printList(list1);
    freeList(list1);
    printf("\n");

    // Test 2
    int arr2[] = {5, 1, 8, 3, 7, 2, 9};
    Node* list2 = createList(arr2, 7);
    printf("Before: "); printList(list2);
    list2 = mergeSort(list2);
    printf("After:  "); printList(list2);
    freeList(list2);
    printf("\n");

    // Test 3 - already sorted
    int arr3[] = {1, 2, 3, 4, 5};
    Node* list3 = createList(arr3, 5);
    printf("Before: "); printList(list3);
    list3 = mergeSort(list3);
    printf("After:  "); printList(list3);
    freeList(list3);
    printf("\n");

    printf("=== Algorithm ===\n");
    printf("1. Find middle (slow/fast pointer)\n");
    printf("2. Split into two halves\n");
    printf("3. Recursively sort both\n");
    printf("4. Merge sorted halves\n");
    printf("\nTime: O(n log n), Space: O(log n) stack\n");

    return 0;
}
