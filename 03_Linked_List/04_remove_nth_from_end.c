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

Node* removeNthFromEnd(Node* head, int n) {
    // Create dummy node to handle edge cases
    // Say: "I'll create a dummy node before head to handle removing the first node"
    Node* dummy = createNode(0);

    // Link dummy to head
    // Say: "Link the dummy node to the head of the list"
    dummy->next = head;

    // Initialize first pointer at dummy
    // Say: "I'll use two pointers with a gap of n+1 nodes between them"
    Node* first = dummy;

    // Initialize second pointer at dummy
    // Say: "Both pointers start at the dummy node"
    Node* second = dummy;

    // Advance first by n+1 steps to create gap
    // Say: "First, move the first pointer n+1 steps ahead"
    for (int i = 0; i <= n; i++) {
        first = first->next;
    }

    // Move both pointers until first reaches NULL
    // Say: "Now move both pointers together until first reaches the end"
    while (first != NULL) {
        // Move first pointer forward
        // Say: "Move first forward one step"
        first = first->next;

        // Move second pointer forward
        // Say: "Move second forward one step"
        second = second->next;
    }

    // Now second is just before the node to delete
    // Say: "Now second is pointing to the node just before the target"

    // Save node to delete for cleanup
    // Say: "Save the node we're about to remove so we can free it"
    Node* toDelete = second->next;

    // Remove the node by skipping it
    // Say: "Remove the node by pointing second's next to the node after target"
    second->next = second->next->next;

    // Free the removed node
    // Say: "Free the memory of the removed node"
    free(toDelete);

    // Get new head from dummy
    // Say: "The new head is dummy's next"
    Node* newHead = dummy->next;

    // Free dummy node
    // Say: "Free the dummy node we created"
    free(dummy);

    // Return new head
    // Say: "Return the new head of the list"
    return newHead;
}

/*
 * ============================================================================
 * PRINT LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void printList(Node* head):
 *   - "void" means function doesn't return anything (just prints)
 *   - "Node* head" receives a COPY of the pointer (not the original)
 *   - We can safely modify "head" inside without affecting caller's pointer
 *
 * while (head != NULL):
 *   - Loop continues as long as head points to a valid node
 *   - When head becomes NULL, we've passed the last node
 *
 * head = head->next:
 *   - Move head to point to the next node (traversal step)
 *   - Original head in caller is UNCHANGED (we modified the copy)
 *
 * ============================================================================
 */
// Print list - traverse and print each node's data
// Say: "I'll traverse the list and print each node's value"
void printList(Node* head) {
    // Loop until we reach the end (NULL)
    // Say: "I loop while head is not NULL"
    while (head != NULL) {
        // Print current node's data
        // Say: "I print the current node's data"
        printf("%d", head->data);

        // Print arrow if there's a next node
        // Say: "If there's a next node, I print an arrow for readability"
        if (head->next) printf(" -> ");

        // Move to next node (modifies LOCAL copy of head)
        // Say: "I advance head to the next node"
        head = head->next;
    }

    // Print NULL at the end to show list termination
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

    return 0;
}
