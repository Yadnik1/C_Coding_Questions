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

/*
 * ============================================================================
 * REMOVE DUPLICATES FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* removeDuplicates(Node* head):
 *   - Returns "Node*" = pointer to head of modified list (head itself unchanged)
 *   - "Node* head" = pointer to the first node of a SORTED list
 *   - Purpose: Remove duplicate values, keeping only first occurrence
 *   - PREREQUISITE: List must be SORTED (duplicates are adjacent)
 *
 * ALGORITHM: Compare Adjacent Nodes
 * ---------------------------------
 * Since the list is SORTED, duplicates are always ADJACENT.
 * For each node, compare with next node:
 *   - If same value: skip (and free) the next node
 *   - If different: move to next node
 *
 * WHY THIS WORKS FOR SORTED LISTS:
 * ---------------------------------
 * In a sorted list, all duplicates are consecutive:
 *   [1, 1, 2, 3, 3] → duplicates 1,1 and 3,3 are adjacent
 *
 * If the list were unsorted:
 *   [1, 3, 1, 2, 3] → duplicates not adjacent, this algorithm fails!
 *   Would need hash set (O(n) space) or O(n²) nested loop
 *
 * KEY INSIGHT: WHY WE DON'T MOVE CURR AFTER REMOVING?
 * ---------------------------------
 * After removing a duplicate, the NEW next might ALSO be a duplicate!
 *
 * Example: 1 → 1 → 1 → 2
 *   At curr=1, next=1: Remove next → 1 → 1 → 2
 *   Still at curr=1, next=1 (new next!): Remove again → 1 → 2
 *   Now curr=1, next=2: Different, NOW move forward
 *
 * If we moved curr immediately, we'd miss the second duplicate!
 *
 * VISUALIZATION:
 * ---------------------------------
 * Initial: 1 → 1 → 2 → 3 → 3 → NULL
 *          ^
 *         curr
 *
 * Step 1: curr(1) == next(1) → Remove next
 *   1 → 2 → 3 → 3 → NULL
 *   ^
 *  curr (stays! check again)
 *
 * Step 2: curr(1) != next(2) → Move forward
 *   1 → 2 → 3 → 3 → NULL
 *       ^
 *      curr
 *
 * Step 3: curr(2) != next(3) → Move forward
 *   1 → 2 → 3 → 3 → NULL
 *           ^
 *          curr
 *
 * Step 4: curr(3) == next(3) → Remove next
 *   1 → 2 → 3 → NULL
 *           ^
 *          curr (stays, but next is NULL)
 *
 * Step 5: curr->next == NULL → Loop ends
 *
 * Result: 1 → 2 → 3 → NULL
 *
 * MEMORY MANAGEMENT - CRITICAL FOR EMBEDDED!
 * ---------------------------------
 * We MUST free removed nodes to prevent memory leaks.
 * The removed nodes are no longer in the list but still allocated!
 *
 * TIME COMPLEXITY: O(n)
 * ---------------------------------
 * - Each node visited at most once
 * - Even though we don't always advance curr, each node can only be
 *   removed once, so total operations still O(n)
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 * - Only one pointer (curr) regardless of list size
 * - No extra data structures!
 *
 * ============================================================================
 */
// Remove duplicates from a SORTED linked list
// Say: "I'll compare adjacent nodes and remove duplicates since the list is sorted"
Node* removeDuplicates(Node* head) {
    // =========================================================================
    // INITIALIZE: Start traversal from head
    // =========================================================================
    // Say: "I start at the head of the list"
    // WHY curr not head directly? We could use head, but curr is clearer
    // Also, using curr preserves head for return
    Node* curr = head;

    // =========================================================================
    // MAIN LOOP: Compare each node with its next neighbor
    // =========================================================================
    // Say: "I traverse, comparing each node with the next one"
    // WHY curr != NULL? Handle empty list case
    // WHY curr->next != NULL? Need a next node to compare with
    while (curr != NULL && curr->next != NULL) {
        // Check if current value equals next value (duplicate found!)
        // Say: "I compare current node's value with the next node's value"
        // WHY compare data? We're looking for duplicate VALUES, not addresses
        if (curr->data == curr->next->data) {
            // =========================================================
            // DUPLICATE FOUND: Remove the next node
            // =========================================================
            // Say: "I found a duplicate - the next node has the same value"

            // Save pointer to duplicate node before unlinking
            // Say: "I save the duplicate node so I can free its memory"
            // WHY save? After curr->next changes, we'd lose access to free it
            Node* duplicate = curr->next;

            // Skip over the duplicate by relinking
            // Say: "I skip the duplicate by pointing curr's next past it"
            // WHY this works? curr → dup → X becomes curr → X
            curr->next = curr->next->next;

            // Free the duplicate node (critical for embedded systems!)
            // Say: "I free the memory of the removed duplicate node"
            // WHY free? Prevent memory leak - node is unreachable but allocated
            free(duplicate);

            // IMPORTANT: Do NOT move curr forward here!
            // Say: "I stay at curr to check if the new next is also a duplicate"
            // WHY stay? The NEW next might also be a duplicate of curr
        } else {
            // =========================================================
            // NO DUPLICATE: Safe to move forward
            // =========================================================
            // Say: "No duplicate here, so I move to the next node"
            // WHY move only here? We've confirmed curr != curr->next
            curr = curr->next;
        }
    }

    // =========================================================================
    // RETURN: Head is unchanged (we only modified links, not head itself)
    // =========================================================================
    // Say: "I return the head of the modified list"
    // WHY head unchanged? We never reassigned head, only modified internal links
    // The list has fewer nodes now but still starts at the same head
    return head;
}

/*
 * ============================================================================
 * PRINT LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void printList(Node* head):
 *   - "void" = Function doesn't return anything (just prints to console)
 *   - "Node* head" = Receives a COPY of the head pointer (pass by value)
 *   - We can safely modify this copy without affecting caller's pointer
 *
 * WHY USE head DIRECTLY (no separate traversal pointer)?
 * ---------------------------------
 * - "head" is a LOCAL COPY of the pointer passed in
 * - Modifying head inside the function does NOT affect caller's pointer
 * - This is more concise - no need for extra "curr" variable
 * - Both approaches work; this is the cleaner style
 *
 * while (head != NULL):
 *   - Loop continues while head points to a valid node
 *   - When head becomes NULL, we've printed all nodes
 *   - NULL marks the end of a properly terminated linked list
 *
 * printf("%d", head->data):
 *   - Access the data field of the node head points to
 *   - %d prints the integer value
 *   - "->" is shorthand for (*head).data
 *
 * if (head->next) printf(" -> "):
 *   - Shorthand for "if (head->next != NULL)"
 *   - In C, non-NULL pointers evaluate to TRUE
 *   - Only print arrow if there's another node
 *   - Cleaner output: no trailing arrow after last node
 *
 * head = head->next:
 *   - Move to the next node in the list
 *   - This is the TRAVERSAL step
 *   - Original caller's head pointer is UNCHANGED
 *
 * printf(" -> NULL\n"):
 *   - Show that the list properly terminates
 *   - Visual indicator of list end
 *   - \n adds newline for clean output
 *
 * ============================================================================
 */
// Print list - traverse and print each node's data
// Say: "I'll traverse the list and print each node's value"
void printList(Node* head) {
    // Loop until we reach the end (NULL)
    // Say: "I loop while head is not NULL"
    // WHY use head directly? It's a local copy, won't affect caller
    while (head != NULL) {
        // Print current node's data
        // Say: "I print the current node's data"
        printf("%d", head->data);

        // Print arrow if there's a next node
        // Say: "If there's a next node, I print an arrow"
        if (head->next) printf(" -> ");

        // Move to next node
        // Say: "I advance head to the next node"
        head = head->next;
    }

    // Print NULL to show end of list
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

/*
 * ============================================================================
 * FREE LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void freeList(Node* head):
 *   - "void" = Function doesn't return anything
 *   - "Node* head" = Takes pointer to the first node
 *   - Purpose: Deallocate ALL memory used by the linked list
 *
 * WHY WE NEED THIS FUNCTION:
 *   - Every malloc() MUST have a corresponding free()
 *   - Without freeing, we get MEMORY LEAKS
 *   - Memory leaks can crash long-running programs
 *   - In interviews, mentioning memory cleanup shows professionalism
 *
 * NOTE FOR REMOVE DUPLICATES:
 *   - removeDuplicates() already frees duplicate nodes internally
 *   - After that function, only unique nodes remain
 *   - freeList() frees those remaining unique nodes
 *
 * ============================================================================
 */
// Free all nodes in the linked list to prevent memory leaks
// Say: "I'll free all nodes by saving next before freeing current"
void freeList(Node* head) {
    // Temporary pointer to save next node before freeing
    // Say: "I need a temp pointer to save the next node before freeing"
    Node* temp;

    // Loop through all nodes
    // Say: "I loop through each node until the end"
    while (head != NULL) {
        // Save the next pointer BEFORE freeing (critical!)
        // Say: "I save the next pointer before freeing the current node"
        temp = head->next;

        // Free the current node
        // Say: "I free the current node, returning its memory to the heap"
        free(head);

        // Move to the next node using saved pointer
        // Say: "I move to the next node using the saved pointer"
        head = temp;
    }
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

    /*
     * ============================================================================
     * MEMORY CLEANUP - IMPORTANT FOR INTERVIEWS!
     * ============================================================================
     * NOTE: removeDuplicates() already frees duplicate nodes internally.
     * We only need to free the remaining unique nodes.
     * Say: "Finally, I free the remaining unique nodes"
     * ============================================================================
     */
    freeList(list1);
    freeList(list2);

    printf("\n=== Memory freed successfully ===\n");

    return 0;
}
