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

// Find middle node (slow/fast pointer)
Node* getMiddle(Node* head) {
    // Edge case: empty or single node
    // Say: "Handle the base case of empty or single node"
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // Initialize slow pointer
    // Say: "I'll use slow and fast pointers to find the middle"
    Node* slow = head;

    // Initialize fast pointer one ahead for first middle
    // Say: "Fast starts one ahead to get the first middle for even-length lists"
    Node* fast = head->next;

    // Traverse to find middle
    // Say: "Move slow by 1 and fast by 2 until fast reaches the end"
    while (fast != NULL && fast->next != NULL) {
        // Move slow one step
        // Say: "Move slow forward one step"
        slow = slow->next;

        // Move fast two steps
        // Say: "Move fast forward two steps"
        fast = fast->next->next;
    }

    // Return middle node
    // Say: "Return the middle node"
    return slow;
}

// Merge two sorted lists
Node* merge(Node* left, Node* right) {
    // Create dummy node to simplify code
    // Say: "I'll use a dummy node to build the merged list"
    Node dummy;
    dummy.next = NULL;

    // Tail tracks end of merged list
    // Say: "Tail pointer will track the end of our merged list"
    Node* tail = &dummy;

    // Merge while both lists have nodes
    // Say: "Compare nodes from both lists and attach the smaller one"
    while (left != NULL && right != NULL) {
        // Compare current nodes
        // Say: "Compare the current values from both lists"
        if (left->data <= right->data) {
            // Left is smaller
            // Say: "Left's value is smaller, so attach it"
            tail->next = left;

            // Move left forward
            // Say: "Move left forward"
            left = left->next;
        } else {
            // Right is smaller
            // Say: "Right's value is smaller, so attach it"
            tail->next = right;

            // Move right forward
            // Say: "Move right forward"
            right = right->next;
        }

        // Move tail forward
        // Say: "Move tail to the node we just added"
        tail = tail->next;
    }

    // Attach remaining nodes
    // Say: "Attach whichever list still has remaining nodes"
    if (left != NULL) {
        tail->next = left;
    } else {
        tail->next = right;
    }

    // Return merged list
    // Say: "Return the merged list starting from dummy's next"
    return dummy.next;
}

// Merge sort for linked list
Node* mergeSort(Node* head) {
    // Base case: empty or single node is already sorted
    // Say: "Base case: if empty or single node, it's already sorted"
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // Find middle of list
    // Say: "Step 1: Find the middle of the list using slow/fast pointers"
    Node* middle = getMiddle(head);

    // Split into right half
    // Say: "Step 2: Split the list into two halves at the middle"
    Node* rightHalf = middle->next;

    // Split the list
    // Say: "Break the link to split the list"
    middle->next = NULL;

    // Recursively sort left half
    // Say: "Step 3: Recursively sort the left half"
    Node* left = mergeSort(head);

    // Recursively sort right half
    // Say: "Recursively sort the right half"
    Node* right = mergeSort(rightHalf);

    // Merge sorted halves
    // Say: "Step 4: Merge the two sorted halves"
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
