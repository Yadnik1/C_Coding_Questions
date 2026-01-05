/*
 * ============================================================================
 * PROBLEM: Merge Two Sorted Linked Lists
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: Very High
 *
 * Merge two sorted linked lists into one sorted list.
 *
 * Example:
 * Input:  1 -> 2 -> 4 and 1 -> 3 -> 4
 * Output: 1 -> 1 -> 2 -> 3 -> 4 -> 4
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. MERGE TECHNIQUE (same as merge sort):
 *    - Compare heads of both lists
 *    - Take smaller one, advance that pointer
 *    - Repeat until one list exhausted
 *    - Attach remaining list
 *
 * 2. DUMMY HEAD:
 *    - Simplifies code by avoiding null checks
 *    - Return dummy->next as result
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * L1: 1 -> 3 -> 5 -> NULL
 * L2: 2 -> 4 -> 6 -> NULL
 *
 * dummy -> NULL, tail = dummy
 *
 * Step 1: Compare 1 vs 2
 *   1 < 2, take 1
 *   dummy -> 1 -> NULL
 *   L1: 3 -> 5 -> NULL
 *
 * Step 2: Compare 3 vs 2
 *   2 < 3, take 2
 *   dummy -> 1 -> 2 -> NULL
 *   L2: 4 -> 6 -> NULL
 *
 * Step 3: Compare 3 vs 4
 *   3 < 4, take 3
 *   dummy -> 1 -> 2 -> 3 -> NULL
 *
 * Step 4: Compare 5 vs 4
 *   4 < 5, take 4
 *   dummy -> 1 -> 2 -> 3 -> 4 -> NULL
 *
 * Step 5: Compare 5 vs 6
 *   5 < 6, take 5
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *
 * L1 exhausted, attach remaining L2:
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n + m)
 * ============================================================================
 * - n = length of list1, m = length of list2
 * - Each node visited once
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only create dummy node
 * - Reuse existing nodes
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why use a dummy node here?"
 * A1: Without dummy, we'd need special handling for the first node:
 *     - Which list has smaller first element?
 *     - Set that as head, then continue
 *
 *     With dummy:
 *     - Always append to tail->next
 *     - Return dummy->next at end
 *     - Cleaner, uniform code!
 *
 * -------------------------------------------------------------------------
 * Q2: "What if one list is empty?"
 * A2: The algorithm handles it naturally:
 *     - While loop condition fails immediately
 *     - tail->next = (non-empty list)
 *     - Works perfectly!
 *
 *     You can add explicit check: if (!l1) return l2; if (!l2) return l1;
 *
 * -------------------------------------------------------------------------
 * Q3: "Can you do this recursively?"
 * A3: Yes! Recursive version:
 *
 *     Node* merge(Node* l1, Node* l2) {
 *         if (!l1) return l2;
 *         if (!l2) return l1;
 *
 *         if (l1->data <= l2->data) {
 *             l1->next = merge(l1->next, l2);
 *             return l1;
 *         } else {
 *             l2->next = merge(l1, l2->next);
 *             return l2;
 *         }
 *     }
 *
 *     Time: O(n+m), Space: O(n+m) stack - NOT preferred for embedded!
 *
 * -------------------------------------------------------------------------
 * Q4: "How would you merge K sorted lists?"
 * A4: This is a common follow-up! Three approaches:
 *
 *     1. Merge pairs repeatedly: O(N log k) where N = total nodes
 *     2. Use min-heap: O(N log k) - optimal!
 *     3. Merge one by one: O(Nk) - slowest
 *
 *     For interviews, mention the heap approach as optimal.
 *
 * -------------------------------------------------------------------------
 * Q5: "Is this stable? (preserves relative order of equal elements)"
 * A5: Yes! When l1->data == l2->data:
 *     - We take from l1 first (using <=)
 *     - This preserves original order of equal elements
 *     - Important for sorting algorithms!
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

Node* mergeSortedLists(Node* l1, Node* l2) {
    // Create dummy node on stack
    // Say: "I'll create a dummy node to simplify the merge logic"
    Node dummy;

    // Initialize tail pointer
    // Say: "Tail will track the end of our merged list"
    Node* tail = &dummy;

    // Initialize dummy's next to NULL
    // Say: "Initialize the dummy's next pointer to NULL"
    dummy.next = NULL;

    // Merge while both lists have nodes
    // Say: "I'll compare nodes from both lists and attach the smaller one"
    while (l1 != NULL && l2 != NULL) {
        // Compare current nodes from both lists
        // Say: "Compare the current nodes from both lists"
        if (l1->data <= l2->data) {
            // L1's node is smaller or equal
            // Say: "L1's value is smaller, so attach it to our result"
            tail->next = l1;

            // Move l1 forward
            // Say: "Move l1 forward to its next node"
            l1 = l1->next;
        } else {
            // L2's node is smaller
            // Say: "L2's value is smaller, so attach it to our result"
            tail->next = l2;

            // Move l2 forward
            // Say: "Move l2 forward to its next node"
            l2 = l2->next;
        }

        // Move tail forward
        // Say: "Move tail forward to the node we just added"
        tail = tail->next;
    }

    // Attach remaining nodes from non-empty list
    // Say: "Attach whichever list still has remaining nodes"
    tail->next = (l1 != NULL) ? l1 : l2;

    // Return the merged list (skip dummy)
    // Say: "Return dummy's next which is the head of our merged list"
    return dummy.next;
}

/*
 * ============================================================================
 * PRINT LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void printList(Node* head):
 *   - "void" means function doesn't return anything (just prints)
 *   - "Node* head" receives a COPY of the pointer (not the original)
 *
 * while (head):
 *   - Shorthand for "while (head != NULL)"
 *   - In C, any non-NULL pointer evaluates to true
 *
 * head = head->next:
 *   - Move head to next node (traversal step)
 *   - Original pointer in caller is UNCHANGED
 *
 * ============================================================================
 */
// Print list - traverse and print each node's data
// Say: "I'll traverse the list and print each node's value"
void printList(Node* head) {
    // Loop until we reach the end (NULL)
    // Say: "I loop while head is not NULL"
    while (head) {
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

    // Print NULL to show list termination
    // Say: "I print NULL to show the end of the list"
    printf(" -> NULL\n");
}

/*
 * ============================================================================
 * CREATE LIST FROM ARRAY - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* createList(int arr[], int n):
 *   - Returns pointer to HEAD of newly created list
 *   - "int arr[]" receives the array (decays to pointer)
 *   - "int n" is the number of elements
 *
 * WHY TWO POINTERS (head and curr):
 *   - "head" stays at the first node (so we can return it)
 *   - "curr" moves forward as we build the list
 *
 * ============================================================================
 */
// Create list from array
// Say: "I'll create a linked list from the given array"
Node* createList(int arr[], int n) {
    // Edge case: empty array returns NULL
    // Say: "First, I check if the array is empty"
    if (n == 0) return NULL;

    // Create head node with first element
    // Say: "I create the head node with the first element"
    Node* head = createNode(arr[0]);

    // curr tracks the end of our list as we build
    // Say: "I use a current pointer to track where to add new nodes"
    Node* curr = head;

    // Add remaining elements
    // Say: "I loop through the rest of the array"
    for (int i = 1; i < n; i++) {
        // Create and link new node
        // Say: "I create a new node and link it"
        curr->next = createNode(arr[i]);

        // Move curr to new node
        // Say: "I move current forward"
        curr = curr->next;
    }

    // Return head
    // Say: "I return head to access the entire list"
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
 * NOTE FOR MERGED LISTS:
 *   - After merging, l1 and l2's nodes are REUSED in merged list
 *   - Only free the merged list, NOT the original l1/l2 pointers!
 *   - Freeing l1 or l2 after merge would cause double-free!
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
    printf("=== Merge Two Sorted Lists ===\n\n");

    int arr1[] = {1, 3, 5};
    int arr2[] = {2, 4, 6};

    Node* l1 = createList(arr1, 3);
    Node* l2 = createList(arr2, 3);

    printf("List 1: ");
    printList(l1);
    printf("List 2: ");
    printList(l2);

    Node* merged = mergeSortedLists(l1, l2);
    printf("Merged: ");
    printList(merged);

    /*
     * ============================================================================
     * MEMORY CLEANUP - IMPORTANT FOR INTERVIEWS!
     * ============================================================================
     * IMPORTANT: After merge, all nodes from l1 and l2 are now in merged list.
     * We only free the merged list - freeing l1 or l2 would be DOUBLE FREE!
     * Say: "I only free the merged list since it contains all the original nodes"
     * ============================================================================
     */
    freeList(merged);
    // Do NOT free l1 or l2 - their nodes are already freed as part of merged!

    printf("\n=== Memory freed successfully ===\n");

    return 0;
}
