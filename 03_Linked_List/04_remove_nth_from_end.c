/*
 * ============================================================================
 * PROBLEM: Remove Nth Node from End of List
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: High
 *
 * Remove the nth node from the end of a linked list.
 *
 * Example:
 * Input:  1 -> 2 -> 3 -> 4 -> 5, n = 2
 * Output: 1 -> 2 -> 3 -> 5 (removed 4)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. TWO-POINTER GAP TECHNIQUE:
 *    - Maintain n-node gap between two pointers
 *    - When first pointer reaches end, second is at target
 *
 * 2. DUMMY HEAD:
 *    - Use dummy node before head
 *    - Handles edge case of removing first node
 *
 * 3. ONE-PASS SOLUTION:
 *    - Don't need to count length first
 *    - Use gap technique
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * List: 1 -> 2 -> 3 -> 4 -> 5 -> NULL, n = 2
 *
 * Add dummy: D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *
 * Step 1: Advance 'first' by n+1 steps (n=2, so 3 steps)
 *
 *   D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *   ^              ^
 *  second        first
 *
 *   Gap = 3 nodes
 *
 * Step 2: Move both until first reaches NULL
 *
 *   D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *        ^              ^
 *       second        first
 *
 *   D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *             ^              ^
 *            second        first
 *
 *   D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *                  ^              ^
 *                second         first=NULL
 *
 * Step 3: Remove second->next (which is node 4)
 *
 *   second->next = second->next->next
 *   3->next = 5
 *
 *   D -> 1 -> 2 -> 3 ------> 5 -> NULL
 *                            ^
 *                        (4 removed)
 *
 * Result: 1 -> 2 -> 3 -> 5 -> NULL
 *
 * ============================================================================
 * WHY USE DUMMY NODE:
 * ============================================================================
 *
 * Without dummy, removing first node is tricky:
 *
 *   List: 1 -> 2, n = 2 (remove node 1)
 *
 *   With dummy:
 *   D -> 1 -> 2 -> NULL
 *   ^    ^
 *  sec  first (after n+1 steps)
 *
 *   Move until first = NULL:
 *   D -> 1 -> 2 -> NULL
 *   ^              ^
 *  sec           first
 *
 *   sec->next = sec->next->next
 *   D -> 2 -> NULL
 *
 *   Return dummy->next (which is 2)
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   dummy = new Node(0)
 *   dummy->next = head
 *
 *   first = dummy
 *   second = dummy
 *
 *   // Advance first by n+1 steps
 *   for i = 0 to n:
 *       first = first->next
 *
 *   // Move both until first is NULL
 *   while (first != NULL):
 *       first = first->next
 *       second = second->next
 *
 *   // Remove node after second
 *   second->next = second->next->next
 *
 *   return dummy->next
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Single pass through list
 * - L nodes, where L is list length
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only two pointers and dummy node
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why use a dummy node?"
 * A1: The dummy node handles the edge case of removing the FIRST node:
 *     - Without dummy: if n equals list length, we need special logic
 *     - With dummy: second pointer stops at dummy, second->next is head
 *     - Code is cleaner and handles all cases uniformly
 *
 *     ALWAYS mention: "Dummy node simplifies edge cases"
 *
 * -------------------------------------------------------------------------
 * Q2: "Why advance first by n+1 steps instead of n?"
 * A2: We need second to stop ONE NODE BEFORE the target:
 *     - If we advance by n, second lands ON the node to delete
 *     - But we can't delete a node we're pointing to (no prev reference!)
 *     - n+1 steps means second stops at the node BEFORE target
 *     - Then second->next = second->next->next removes the target
 *
 * -------------------------------------------------------------------------
 * Q3: "Can you do this in one pass without dummy node?"
 * A3: Yes, but it's messier:
 *     - Check if removing head (first reaches NULL after n steps)
 *     - Return head->next in that case
 *     - Otherwise use same gap technique
 *
 *     The dummy node makes code cleaner - prefer it in interviews!
 *
 * -------------------------------------------------------------------------
 * Q4: "What if n is larger than list length?"
 * A4: The problem usually guarantees valid n. But to handle it:
 *     - Check if first becomes NULL during the initial n+1 advancement
 *     - If so, n is invalid - return head unchanged or error
 *
 *     Code: if (first == NULL) return head; // n too large
 *
 * -------------------------------------------------------------------------
 * Q5: "How would you remove nth from BEGINNING instead?"
 * A5: Much simpler - just traverse n-1 nodes:
 *     - If n == 1, return head->next (remove head)
 *     - Otherwise, traverse to node n-1, skip node n
 *
 *     curr = head;
 *     for (i = 1; i < n-1; i++) curr = curr->next;
 *     curr->next = curr->next->next;
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
 * MEMORY LAYOUT:
 *   +------+------+
 *   | data | next |---> (next node or NULL)
 *   +------+------+
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
 * Node* createNode(int data):
 *   - Returns a POINTER to a new Node (allocated on heap)
 *   - Takes the data value to store as parameter
 *
 * malloc(sizeof(Node)):
 *   - Requests memory from HEAP (persists after function returns)
 *   - sizeof(Node) = exact bytes needed (typically 12-16 bytes)
 *   - Returns NULL if allocation fails
 *
 * newNode->data = data:
 *   - "->" accesses struct member through pointer
 *   - Equivalent to: (*newNode).data = data
 *   - Stores the parameter value in the node
 *
 * newNode->next = NULL:
 *   - Initialize pointer to NULL (not connected to any node)
 *   - CRITICAL: Always initialize pointers to avoid undefined behavior
 *
 * return newNode:
 *   - Returns pointer to the heap-allocated node
 *   - Caller must eventually free() to prevent memory leaks
 *
 * ============================================================================
 */
Node* createNode(int data) {
    // Allocate memory on HEAP for one Node structure
    // Say: "I allocate memory for a new node using malloc"
    Node* newNode = (Node*)malloc(sizeof(Node));

    // Store the data value in the node
    // Say: "I store the data value in the node's data field"
    newNode->data = data;

    // Initialize next pointer to NULL (not connected yet)
    // Say: "I set next to NULL since this node isn't linked to anything yet"
    newNode->next = NULL;

    // Return pointer to the new node
    // Say: "I return the pointer to the newly created node"
    return newNode;
}

/*
 * ============================================================================
 * REMOVE NTH FROM END FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* removeNthFromEnd(Node* head, int n):
 *   - Returns "Node*" = pointer to the head of modified list
 *   - "Node* head" = pointer to the first node of the list
 *   - "int n" = position from end to remove (1-indexed)
 *   - Purpose: Remove the nth node from the END of the list in ONE PASS
 *
 * ALGORITHM: Two-Pointer Gap Technique
 * ---------------------------------
 * Key insight: If we maintain a GAP of n+1 nodes between two pointers,
 * when the first pointer reaches NULL, the second pointer will be at
 * the node JUST BEFORE the one we want to delete.
 *
 * WHY n+1 GAP (NOT n)?
 * ---------------------------------
 * We need to stop at the node BEFORE the target:
 *   - With n gap: second lands ON the target (can't delete - no prev reference)
 *   - With n+1 gap: second lands BEFORE target (can do: second->next = second->next->next)
 *
 * WHY USE DUMMY NODE?
 * ---------------------------------
 * The dummy node handles edge cases elegantly:
 *   - Removing the FIRST node: without dummy, we'd need special handling
 *   - With dummy: second can stop at dummy, then second->next = head
 *
 * Example: Remove 2nd from end of list [1->2->3->4->5]
 *
 *   D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *
 *   Step 1: Advance first by n+1 = 3 steps
 *   D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *   ^              ^
 *  sec           first
 *
 *   Step 2: Move both until first = NULL
 *   D -> 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *                  ^              ^
 *                 sec           first
 *
 *   Step 3: Remove second->next (which is 4)
 *   D -> 1 -> 2 -> 3 --------> 5 -> NULL
 *
 * EDGE CASE - REMOVING FIRST NODE:
 * ---------------------------------
 * List: [1->2], n=2 (remove node 1 - the head!)
 *
 *   D -> 1 -> 2 -> NULL
 *
 *   After advancing first by 3 steps: first = NULL
 *   But wait - we use a for loop that goes n+1 times...
 *
 *   Actually, let me trace this:
 *   i=0: first = D->next = 1
 *   i=1: first = 1->next = 2
 *   i=2: first = 2->next = NULL
 *
 *   Now first = NULL, loop never executes
 *   second is still at dummy
 *   second->next = 1 (the node to delete)
 *   After: D -> 2 -> NULL
 *   Return dummy->next = 2 (new head!)
 *
 * TIME COMPLEXITY: O(L) where L = list length
 * ---------------------------------
 * - One pass through the list
 * - First pointer traverses entire list once
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 * - Only two pointers + one dummy node
 * - No extra data structures
 *
 * ============================================================================
 */
// Remove nth node from end using two-pointer gap technique
// Say: "I'll use two pointers with a gap to find the node to remove in one pass"
Node* removeNthFromEnd(Node* head, int n) {
    // =========================================================================
    // STEP 1: CREATE DUMMY NODE - Handles edge case of removing first node
    // =========================================================================
    // Say: "I create a dummy node before head to handle edge cases"
    // WHY dummy? If we need to remove the first node, we need something before it
    // Without dummy, removing head requires special case handling
    Node* dummy = createNode(0);

    // Link dummy to head
    // Say: "I link the dummy node to the head of the list"
    // WHY link? Dummy must be part of the list for the gap technique to work
    dummy->next = head;

    // =========================================================================
    // STEP 2: INITIALIZE TWO POINTERS - Both start at dummy
    // =========================================================================
    // Say: "I initialize two pointers, both starting at the dummy node"
    // WHY at dummy? We'll create a gap, then move both together
    Node* first = dummy;   // Will advance ahead to create gap
    Node* second = dummy;  // Will end up BEFORE the node to delete

    // =========================================================================
    // STEP 3: CREATE GAP - Advance first by n+1 steps
    // =========================================================================
    // Say: "I advance the first pointer by n+1 steps to create the gap"
    // WHY n+1? So second ends up ONE NODE BEFORE the target
    // If we only advanced n steps, second would land ON the target (useless!)
    for (int i = 0; i <= n; i++) {
        // Move first pointer forward
        // Say: "Move first forward one step"
        // WHY i <= n? This gives us n+1 iterations (0,1,2,...,n)
        first = first->next;
    }
    // After this loop:
    // - first is n+1 nodes ahead of second
    // - second is still at dummy

    // =========================================================================
    // STEP 4: TRAVERSE TOGETHER - Maintain gap until first reaches NULL
    // =========================================================================
    // Say: "I move both pointers together until first reaches the end"
    // WHY together? Gap stays constant, so when first = NULL, second is in position
    while (first != NULL) {
        // Move first pointer forward
        // Say: "Move first forward one step"
        // WHY check first != NULL? When first reaches end, second is at target-1
        first = first->next;

        // Move second pointer forward
        // Say: "Move second forward one step, maintaining the gap"
        // WHY move both? Gap stays constant at n+1 nodes
        second = second->next;
    }
    // After this loop:
    // - first = NULL (past the end)
    // - second = node BEFORE the one we want to delete

    // =========================================================================
    // STEP 5: DELETE THE TARGET NODE
    // =========================================================================
    // Say: "Now second points to the node just before our target"

    // Save node to delete for cleanup (prevent memory leak!)
    // Say: "I save a pointer to the node I'm about to remove"
    // WHY save? We need to free() it, but after unlinking we lose access
    Node* toDelete = second->next;

    // Remove the node by skipping it
    // Say: "I unlink the target by making second point past it"
    // WHY this works? second->next->next is the node AFTER target
    // This effectively "skips over" the target node
    second->next = second->next->next;

    // Free the removed node (prevent memory leak!)
    // Say: "I free the removed node to prevent memory leaks"
    // WHY free? Every malloc needs a free; the node is no longer in the list
    free(toDelete);

    // =========================================================================
    // STEP 6: CLEANUP AND RETURN
    // =========================================================================
    // Get new head from dummy
    // Say: "The new head is dummy's next pointer"
    // WHY dummy->next? If we deleted the original head, dummy->next is the new head
    // If we didn't delete head, dummy->next is still the original head
    Node* newHead = dummy->next;

    // Free dummy node (we created it, we must free it!)
    // Say: "I free the dummy node I created"
    // WHY free dummy? It was malloc'd in createNode, must be freed
    free(dummy);

    // Return new head
    // Say: "I return the new head of the modified list"
    // WHY return newHead? The original head might have been deleted
    return newHead;
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
 *   - Returns a pointer to the HEAD of newly created list
 *   - "int arr[]" receives the array (decays to pointer)
 *   - "int n" is the number of elements in the array
 *
 * WHY TWO POINTERS (head and curr):
 *   - "head" stays at first node (to return it)
 *   - "curr" moves forward as we build
 *   - If we only used head and moved it, we'd lose the list!
 *
 * ============================================================================
 */
// Create list from array - convert array to linked list
// Say: "I'll create a linked list from the given array"
Node* createList(int arr[], int n) {
    // Edge case: empty array returns empty list (NULL)
    // Say: "First, I check if the array is empty"
    if (n == 0) return NULL;

    // Create the first node (head) with arr[0]
    // Say: "I create the head node with the first element"
    Node* head = createNode(arr[0]);

    // curr pointer will traverse as we build (head stays at start)
    // Say: "I use a current pointer starting at head to build the list"
    Node* curr = head;

    // Loop through remaining elements (starting from index 1)
    // Say: "I loop through the rest of the array, adding nodes"
    for (int i = 1; i < n; i++) {
        // Create new node and link it to current node's next
        // Say: "I create a new node and link it to the current node"
        curr->next = createNode(arr[i]);

        // Move curr forward to the newly added node
        // Say: "I move current to the new node, ready for the next element"
        curr = curr->next;
    }

    // Return head (first node) - gives access to entire list
    // Say: "I return head, which points to the beginning of the list"
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
    printf("=== Remove Nth Node from End ===\n\n");

    // Test 1: Remove 2nd from end
    int arr1[] = {1, 2, 3, 4, 5};
    Node* list1 = createList(arr1, 5);

    printf("Original: ");
    printList(list1);
    printf("Remove 2nd from end\n");

    list1 = removeNthFromEnd(list1, 2);
    printf("Result:   ");
    printList(list1);

    // Test 2: Remove last element
    int arr2[] = {1, 2, 3};
    Node* list2 = createList(arr2, 3);

    printf("\nOriginal: ");
    printList(list2);
    printf("Remove 1st from end (last)\n");

    list2 = removeNthFromEnd(list2, 1);
    printf("Result:   ");
    printList(list2);

    // Test 3: Remove first element
    int arr3[] = {1, 2};
    Node* list3 = createList(arr3, 2);

    printf("\nOriginal: ");
    printList(list3);
    printf("Remove 2nd from end (first)\n");

    list3 = removeNthFromEnd(list3, 2);
    printf("Result:   ");
    printList(list3);

    // Test 4: Single element
    int arr4[] = {1};
    Node* list4 = createList(arr4, 1);

    printf("\nOriginal: ");
    printList(list4);
    printf("Remove 1st from end\n");

    list4 = removeNthFromEnd(list4, 1);
    printf("Result:   ");
    if (list4 == NULL) printf("NULL (empty list)\n");

    /*
     * ============================================================================
     * MEMORY CLEANUP - IMPORTANT FOR INTERVIEWS!
     * ============================================================================
     * Always free dynamically allocated memory when done.
     * Say: "Finally, I free all the lists to prevent memory leaks"
     * ============================================================================
     */
    freeList(list1);
    freeList(list2);
    freeList(list3);
    // list4 is already NULL after removing the only element

    printf("\n=== Memory freed successfully ===\n");

    return 0;
}
