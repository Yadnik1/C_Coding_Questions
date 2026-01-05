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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why not just use a stack to compare?"
 * A1: Stack approach works but uses O(n) space:
 *     - Push all elements to stack
 *     - Traverse again, compare with popped elements
 *
 *     Our approach: O(1) space by reversing in-place!
 *     ALWAYS mention: "For embedded, O(1) space is preferred"
 *
 * -------------------------------------------------------------------------
 * Q2: "Do you need to restore the list after checking?"
 * A2: Depends on requirements! To restore:
 *     - After comparison, reverse the second half again
 *     - Reconnect to first half
 *
 *     In interviews, ASK: "Should I restore the original list?"
 *
 * -------------------------------------------------------------------------
 * Q3: "How does this handle odd-length lists?"
 * A3: Works perfectly! For 1 -> 2 -> 3 -> 2 -> 1:
 *     - Middle is 3
 *     - First half: 1 -> 2
 *     - Second half: 2 -> 1 (reversed)
 *     - Middle element (3) is ignored - palindromes ignore center!
 *
 * -------------------------------------------------------------------------
 * Q4: "What's the trick to finding the correct middle?"
 * A4: For palindrome check, we want slow to stop at:
 *     - Odd length: exactly middle (skip it)
 *     - Even length: end of first half
 *
 *     Use: while (fast->next && fast->next->next)
 *     This makes slow stop at the END of first half.
 *
 * -------------------------------------------------------------------------
 * Q5: "Can you do this recursively?"
 * A5: Yes, but it's tricky and uses O(n) stack space:
 *
 *     Node* left;
 *     bool check(Node* right) {
 *         if (right == NULL) return true;
 *         if (!check(right->next)) return false;
 *         if (left->data != right->data) return false;
 *         left = left->next;
 *         return true;
 *     }
 *
 *     Not recommended for embedded (stack overflow risk)!
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * ============================================================================
 * NODE STRUCTURE - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * typedef struct Node {
 *     int data;           // PAYLOAD: The actual value stored
 *     struct Node* next;  // LINK: Pointer to the next node
 * } Node;
 *
 * - "typedef" creates an alias so we can write "Node" instead of "struct Node"
 * - "int data" stores the value (can be any type: int, char, float, etc.)
 * - "struct Node* next" points to the next node (NULL if last node)
 * - We use "struct Node*" inside because typedef isn't complete yet
 *
 * ============================================================================
 */
typedef struct Node {
    int data;           // The data field stores the actual value (payload)
    struct Node* next;  // Pointer to the next node in the list (or NULL)
} Node;

/*
 * ============================================================================
 * CREATE NODE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * malloc(sizeof(Node)): Allocates memory on HEAP for one Node
 * node->data = data:    Stores the parameter value using -> operator
 * node->next = NULL:    Initializes pointer (CRITICAL to avoid bugs)
 * return node:          Returns pointer to heap-allocated node
 *
 * ============================================================================
 */
Node* createNode(int data) {
    // Allocate memory on HEAP for one Node structure
    // Say: "I allocate memory for a new node using malloc"
    Node* node = (Node*)malloc(sizeof(Node));

    // Store the data value in the node
    // Say: "I store the data value in the node's data field"
    node->data = data;

    // Initialize next pointer to NULL (not connected yet)
    // Say: "I set next to NULL since this node isn't linked to anything yet"
    node->next = NULL;

    // Return pointer to the new node
    // Say: "I return the pointer to the newly created node"
    return node;
}

Node* reverseList(Node* head) {
    // Initialize previous pointer to NULL
    // Say: "I'll reverse the list using three pointers"
    Node* prev = NULL;

    // Traverse and reverse
    // Say: "Iterate through and reverse each link"
    while (head) {
        // Save next node
        // Say: "Save the next node before changing pointers"
        Node* next = head->next;

        // Reverse the link
        // Say: "Reverse the current node's pointer"
        head->next = prev;

        // Move prev forward
        // Say: "Move prev to current"
        prev = head;

        // Move head forward
        // Say: "Move to next node"
        head = next;
    }

    // Return new head
    // Say: "Return the new head of reversed list"
    return prev;
}

bool isPalindrome(Node* head) {
    // Edge case: empty or single node is palindrome
    // Say: "First handle edge cases - empty or single node is a palindrome"
    if (head == NULL || head->next == NULL) return true;

    // Find middle of list
    // Say: "Step 1: Find the middle using slow and fast pointers"
    Node* slow = head;
    Node* fast = head;

    // Use fast->next->next to find first middle for even length
    // Say: "Move slow by 1 and fast by 2 to find the middle"
    while (fast->next && fast->next->next) {
        // Move slow one step
        slow = slow->next;

        // Move fast two steps
        fast = fast->next->next;
    }

    // Reverse second half starting from slow->next
    // Say: "Step 2: Reverse the second half of the list"
    Node* secondHalf = reverseList(slow->next);

    // Keep pointer to first half
    // Say: "Keep a pointer to the first half starting from head"
    Node* firstHalf = head;

    // Compare both halves
    // Say: "Step 3: Compare the first half with the reversed second half"
    while (secondHalf) {
        // Check if values match
        // Say: "Compare corresponding values from both halves"
        if (firstHalf->data != secondHalf->data) {
            // Mismatch found
            // Say: "Found a mismatch, so it's not a palindrome"
            return false;
        }

        // Move to next node in first half
        // Say: "Move to next node in first half"
        firstHalf = firstHalf->next;

        // Move to next node in second half
        // Say: "Move to next node in second half"
        secondHalf = secondHalf->next;
    }

    // All values matched
    // Say: "All values matched, so it's a palindrome"
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
