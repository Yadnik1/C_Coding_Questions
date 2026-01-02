/*
 * ============================================================================
 * PROBLEM: Remove Duplicates from Sorted List
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: High
 *
 * Example:
 * Input:  1 -> 1 -> 2 -> 3 -> 3 -> NULL
 * Output: 1 -> 2 -> 3 -> NULL
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * List is SORTED - duplicates are adjacent!
 * Compare current with next, skip if same.
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * 1 -> 1 -> 2 -> 3 -> 3 -> NULL
 * ^
 * curr
 *
 * Step 1: curr(1) == next(1)
 *   Skip next: curr->next = curr->next->next
 *   1 -> 2 -> 3 -> 3 -> NULL
 *   ^
 *
 * Step 2: curr(1) != next(2)
 *   Move: curr = curr->next
 *   1 -> 2 -> 3 -> 3 -> NULL
 *        ^
 *
 * Step 3: curr(2) != next(3)
 *   Move forward
 *   1 -> 2 -> 3 -> 3 -> NULL
 *             ^
 *
 * Step 4: curr(3) == next(3)
 *   Skip next
 *   1 -> 2 -> 3 -> NULL
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
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

Node* removeDuplicates(Node* head) {
    Node* curr = head;

    while (curr != NULL && curr->next != NULL) {
        if (curr->data == curr->next->data) {
            // Skip duplicate
            Node* duplicate = curr->next;
            curr->next = curr->next->next;
            free(duplicate);
        } else {
            curr = curr->next;
        }
    }

    return head;
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
    printf("=== Remove Duplicates from Sorted List ===\n\n");

    int arr1[] = {1, 1, 2, 3, 3};
    Node* list1 = createList(arr1, 5);

    printf("Original: ");
    printList(list1);

    list1 = removeDuplicates(list1);
    printf("After:    ");
    printList(list1);

    int arr2[] = {1, 1, 1, 1, 1};
    Node* list2 = createList(arr2, 5);

    printf("\nOriginal: ");
    printList(list2);

    list2 = removeDuplicates(list2);
    printf("After:    ");
    printList(list2);

    return 0;
}
