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

Node* removeDuplicates(Node* head) {
    // Start at head
    // Say: "I'll traverse the list starting from the head"
    Node* curr = head;

    // Traverse until end
    // Say: "For each node, I'll check if it's equal to the next node"
    while (curr != NULL && curr->next != NULL) {
        // Check if current equals next
        // Say: "Compare current node's value with next node's value"
        if (curr->data == curr->next->data) {
            // Found duplicate, skip it
            // Say: "Found a duplicate, so I'll skip the next node"

            // Save duplicate node
            // Say: "Save the duplicate node so we can free it"
            Node* duplicate = curr->next;

            // Skip the duplicate
            // Say: "Point current's next to the node after the duplicate"
            curr->next = curr->next->next;

            // Free duplicate node
            // Say: "Free the memory of the duplicate node"
            free(duplicate);
        } else {
            // Not a duplicate, move forward
            // Say: "No duplicate found, move to the next node"
            curr = curr->next;
        }
    }

    // Return head (unchanged)
    // Say: "Return the head of the modified list"
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
