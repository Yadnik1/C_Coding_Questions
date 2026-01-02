/*
 * ============================================================================
 * PROBLEM: Check if Linked List is Palindrome
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: High
 *
 * Example:
 * Input:  1 -> 2 -> 2 -> 1 -> NULL
 * Output: true (palindrome)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. APPROACH:
 *    - Find middle using slow/fast
 *    - Reverse second half
 *    - Compare both halves
 *
 * 2. COMBINES THREE CONCEPTS:
 *    - Find middle
 *    - Reverse list
 *    - Compare lists
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: 1 -> 2 -> 2 -> 1
 * ============================================================================
 *
 * Step 1: Find middle
 *   1 -> 2 -> 2 -> 1 -> NULL
 *        ^    ^
 *       slow fast
 *
 *   Middle = 2 (second node)
 *
 * Step 2: Reverse second half (from middle->next)
 *   First half:  1 -> 2
 *   Second half: 2 -> 1 → reversed → 1 -> 2
 *
 * Step 3: Compare
 *   First half:  1 -> 2
 *   Second half: 1 -> 2
 *   Match! → PALINDROME
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

Node* reverseList(Node* head) {
    Node* prev = NULL;
    while (head) {
        Node* next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    return prev;
}

bool isPalindrome(Node* head) {
    if (head == NULL || head->next == NULL) return true;

    // Find middle
    Node* slow = head;
    Node* fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // Reverse second half
    Node* secondHalf = reverseList(slow->next);
    Node* firstHalf = head;

    // Compare
    while (secondHalf) {
        if (firstHalf->data != secondHalf->data) {
            return false;
        }
        firstHalf = firstHalf->next;
        secondHalf = secondHalf->next;
    }

    return true;
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
    printf("=== Palindrome Linked List ===\n\n");

    int arr1[] = {1, 2, 2, 1};
    Node* list1 = createList(arr1, 4);
    printf("List: ");
    printList(list1);
    printf("Palindrome: %s\n\n", isPalindrome(list1) ? "YES" : "NO");

    int arr2[] = {1, 2, 3, 2, 1};
    Node* list2 = createList(arr2, 5);
    printf("List: ");
    printList(list2);
    printf("Palindrome: %s\n\n", isPalindrome(list2) ? "YES" : "NO");

    int arr3[] = {1, 2, 3};
    Node* list3 = createList(arr3, 3);
    printf("List: ");
    printList(list3);
    printf("Palindrome: %s\n", isPalindrome(list3) ? "YES" : "NO");

    return 0;
}
