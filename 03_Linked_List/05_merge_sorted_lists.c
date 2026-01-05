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

/*
 * ============================================================================
 * MERGE SORTED LISTS FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* mergeSortedLists(Node* l1, Node* l2):
 *   - Returns "Node*" = pointer to the head of merged list
 *   - "Node* l1" = pointer to first sorted list
 *   - "Node* l2" = pointer to second sorted list
 *   - Purpose: Merge two SORTED lists into one SORTED list
 *   - IMPORTANT: This REUSES existing nodes (doesn't create new ones!)
 *
 * ALGORITHM: Two-Pointer Merge (same as Merge Sort's merge step)
 * ---------------------------------
 * 1. Compare heads of both lists
 * 2. Take the smaller node, attach to result
 * 3. Advance the pointer of the list we took from
 * 4. Repeat until one list is exhausted
 * 5. Attach remaining nodes (already sorted!)
 *
 * WHY USE STACK-ALLOCATED DUMMY NODE?
 * ---------------------------------
 * - "Node dummy;" allocates on STACK (not heap)
 * - No need to malloc/free - automatic cleanup when function returns
 * - Dummy simplifies code: always append to tail->next
 * - Without dummy: need special handling for first node
 *
 * COMPARISON: Heap vs Stack Dummy
 * ---------------------------------
 * Stack (what we use):  Node dummy;           → auto cleanup
 * Heap (alternative):   Node* dummy = malloc() → must free()
 *
 * Stack is preferred here - cleaner and no memory leak risk!
 *
 * WHY USE "<=" INSTEAD OF "<"?
 * ---------------------------------
 * Using <= makes the algorithm STABLE:
 * - Elements from l1 come first when equal
 * - Preserves relative order of equal elements
 * - Important when merge sort uses this function!
 *
 * VISUALIZATION:
 * ---------------------------------
 * L1: 1 -> 3 -> 5 -> NULL
 * L2: 2 -> 4 -> 6 -> NULL
 *
 * Initial: dummy -> NULL, tail = dummy
 *
 * Step 1: Compare 1 vs 2, take 1
 *   dummy -> 1, tail -> 1
 *   L1: 3 -> 5 -> NULL
 *
 * Step 2: Compare 3 vs 2, take 2
 *   dummy -> 1 -> 2, tail -> 2
 *   L2: 4 -> 6 -> NULL
 *
 * Step 3: Compare 3 vs 4, take 3
 *   dummy -> 1 -> 2 -> 3, tail -> 3
 *
 * Step 4: Compare 5 vs 4, take 4
 *   dummy -> 1 -> 2 -> 3 -> 4, tail -> 4
 *
 * Step 5: Compare 5 vs 6, take 5
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5, tail -> 5
 *   L1: NULL (exhausted!)
 *
 * Attach remaining L2:
 *   dummy -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL
 *
 * Return dummy.next = 1 -> 2 -> 3 -> 4 -> 5 -> 6
 *
 * EDGE CASES HANDLED AUTOMATICALLY:
 * ---------------------------------
 * - l1 empty: while loop never runs, tail->next = l2
 * - l2 empty: while loop never runs, tail->next = l1
 * - Both empty: while loop never runs, tail->next = NULL
 *
 * TIME COMPLEXITY: O(n + m)
 * ---------------------------------
 * - n = length of l1, m = length of l2
 * - Each node visited exactly once
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 * - Only one stack-allocated dummy node
 * - No new nodes created - we REUSE existing nodes!
 * - This is called "in-place" merging
 *
 * ============================================================================
 */
// Merge two sorted lists into one sorted list (in-place, reuses nodes)
// Say: "I'll merge two sorted lists by comparing nodes and attaching smaller ones"
Node* mergeSortedLists(Node* l1, Node* l2) {
    // =========================================================================
    // STEP 1: CREATE STACK-ALLOCATED DUMMY NODE
    // =========================================================================
    // Say: "I create a dummy node on the stack to simplify building the result"
    // WHY stack? Automatic cleanup, no need to free, cleaner code
    // WHY dummy? Avoids special case for first node - always append to tail->next
    Node dummy;

    // =========================================================================
    // STEP 2: INITIALIZE TAIL POINTER
    // =========================================================================
    // Say: "Tail pointer will track the end of our merged list"
    // WHY tail? We need to know where to append the next node
    // WHY point to dummy? First node will be attached to dummy.next via tail->next
    Node* tail = &dummy;

    // Initialize dummy's next to NULL (good practice)
    // Say: "I initialize dummy's next to NULL"
    // WHY initialize? Safety - in case we return early or both lists are empty
    dummy.next = NULL;

    // =========================================================================
    // STEP 3: MAIN MERGE LOOP - Compare and attach smaller node
    // =========================================================================
    // Say: "I'll compare nodes from both lists and attach the smaller one"
    // WHY both conditions? We can only compare when BOTH lists have nodes
    // When one is exhausted, we attach the remaining list directly
    while (l1 != NULL && l2 != NULL) {
        // Compare current nodes from both lists
        // Say: "I compare the current values from both lists"
        // WHY <=? Makes algorithm STABLE (preserves order of equal elements)
        if (l1->data <= l2->data) {
            // L1's node is smaller or equal - take it
            // Say: "L1's value is smaller or equal, so I attach it to our result"
            // WHY attach? We're building a new sorted list by relinking existing nodes
            tail->next = l1;

            // Move l1 forward (we've "consumed" this node)
            // Say: "I move l1 forward to its next node"
            // WHY advance? This node is now in our result, get the next candidate
            l1 = l1->next;
        } else {
            // L2's node is smaller - take it
            // Say: "L2's value is smaller, so I attach it to our result"
            // WHY else? l2->data < l1->data, so l2's node should come first
            tail->next = l2;

            // Move l2 forward (we've "consumed" this node)
            // Say: "I move l2 forward to its next node"
            // WHY advance? This node is now in our result, get the next candidate
            l2 = l2->next;
        }

        // Move tail forward to the node we just added
        // Say: "I move tail forward to maintain it at the end of our result"
        // WHY move tail? Next iteration needs to append after this node
        tail = tail->next;
    }

    // =========================================================================
    // STEP 4: ATTACH REMAINING NODES (One list is exhausted)
    // =========================================================================
    // Say: "I attach whichever list still has remaining nodes"
    // WHY ternary? One of l1/l2 is NULL; attach the non-NULL one
    // If both NULL, tail->next = NULL (already sorted/empty)
    // WHY can we attach directly? The remaining list is already sorted!
    tail->next = (l1 != NULL) ? l1 : l2;

    // =========================================================================
    // STEP 5: RETURN THE MERGED LIST (skip dummy)
    // =========================================================================
    // Say: "I return dummy's next which is the head of our merged list"
    // WHY dummy.next? Dummy was just a placeholder; real list starts at next
    // WHY "."? dummy is a struct (not pointer), so we use "." not "->"
    return dummy.next;
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
