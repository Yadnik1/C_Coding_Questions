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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why do we only move curr when there's no duplicate?"
 * A1: Because after removing a duplicate, the NEW next might also be a duplicate!
 *
 *     Example: 1 → 1 → 1 → 2
 *     After first removal: 1 → 1 → 2
 *     We need to check again, so we DON'T move curr.
 *
 * -------------------------------------------------------------------------
 * Q2: "What if the list is UNSORTED?"
 * A2: Then duplicates are NOT adjacent. Two approaches:
 *
 *     Approach 1: Hash set (O(n) time, O(n) space)
 *     - Track seen values in a set
 *     - Skip nodes with values already in set
 *
 *     Approach 2: Brute force (O(n²) time, O(1) space)
 *     - For each node, check all subsequent nodes
 *     - Remove matches
 *
 *     For embedded with memory constraints, brute force may be preferred.
 *
 * -------------------------------------------------------------------------
 * Q3: "Should I free the duplicate nodes?"
 * A3: YES! Always free removed nodes to prevent memory leaks:
 *
 *     Node* duplicate = curr->next;
 *     curr->next = curr->next->next;
 *     free(duplicate);  // Critical for embedded systems!
 *
 *     ALWAYS mention memory management in embedded interviews.
 *
 * -------------------------------------------------------------------------
 * Q4: "What about removing ALL occurrences (keep only unique values)?"
 * A4: Different problem! For 1→1→2→3→3, output is 2 (not 1→2→3).
 *
 *     This requires:
 *     - Dummy head (in case first nodes are duplicates)
 *     - Track if current value has duplicates
 *     - Skip ALL nodes with that value
 *
 * -------------------------------------------------------------------------
 * Q5: "Why is the sorted property important?"
 * A5: Sorted means duplicates are ADJACENT!
 *
 *     - Only compare curr with curr->next
 *     - Single pass O(n) is possible
 *     - No extra data structures needed (O(1) space)
 *
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

/*
 * ============================================================================
 * PRINT LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void printList(Node* head):
 *   - "void" = doesn't return anything
 *   - "Node* head" = receives COPY of pointer (safe to modify)
 *
 * while (head):
 *   - Shorthand for "while (head != NULL)"
 *   - Non-NULL pointers evaluate to true in C
 *
 * head = head->next:
 *   - Traversal step - move to next node
 *   - Original caller's pointer unchanged (we have a copy)
 *
 * ============================================================================
 */
// Print list - traverse and print each node's data
// Say: "I'll traverse the list and print each node's value"
void printList(Node* head) {
    // Loop until we reach the end
    // Say: "I loop while head is not NULL"
    while (head) {
        // Print current node's data
        // Say: "I print the current node's data"
        printf("%d", head->data);

        // Print arrow if more nodes exist
        // Say: "If there's a next node, I print an arrow"
        if (head->next) printf(" -> ");

        // Move to next node
        // Say: "I advance head to the next node"
        head = head->next;
    }

    // Print NULL to show end
    // Say: "I print NULL to show the end of the list"
    printf(" -> NULL\n");
}

/*
 * ============================================================================
 * CREATE LIST FROM ARRAY - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Key insight: We need TWO pointers:
 *   - "head" stays at first node (to return it)
 *   - "curr" moves forward as we build
 *
 * If we only used head and moved it, we'd lose the list start!
 *
 * ============================================================================
 */
// Create list from array
// Say: "I'll create a linked list from the given array"
Node* createList(int arr[], int n) {
    // Edge case: empty array
    // Say: "First, I check if the array is empty"
    if (n == 0) return NULL;

    // Create head with first element
    // Say: "I create the head node with the first element"
    Node* head = createNode(arr[0]);

    // curr tracks where to add next node
    // Say: "I use current to track the end of the list"
    Node* curr = head;

    // Add remaining elements
    // Say: "I loop through remaining elements"
    for (int i = 1; i < n; i++) {
        // Create and link
        // Say: "I create a new node and link it"
        curr->next = createNode(arr[i]);

        // Move forward
        // Say: "I move current forward"
        curr = curr->next;
    }

    // Return head (never moved!)
    // Say: "I return head to access the list"
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
