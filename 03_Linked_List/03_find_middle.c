/*
 * ============================================================================
 * PROBLEM: Find Middle Element of Linked List
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * Find the middle node of a singly linked list.
 * For even length, return the second middle node.
 *
 * Example:
 * Input:  1 -> 2 -> 3 -> 4 -> 5
 * Output: 3
 *
 * Input:  1 -> 2 -> 3 -> 4 -> 5 -> 6
 * Output: 4 (second middle)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. SLOW-FAST POINTER TECHNIQUE:
 *    - Slow moves 1 step
 *    - Fast moves 2 steps
 *    - When fast reaches end, slow is at middle
 *
 * 2. WHY IT WORKS:
 *    - Fast covers 2x distance of slow
 *    - When fast at end, slow at half = middle
 *
 * ============================================================================
 * VISUAL WALKTHROUGH (Odd Length):
 * ============================================================================
 *
 * List: 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *
 * Initial:
 *   slow = 1, fast = 1
 *   S,F
 *   1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *
 * Step 1:
 *   slow = 2 (move 1)
 *   fast = 3 (move 2)
 *
 *   1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *        S    F
 *
 * Step 2:
 *   slow = 3 (move 1)
 *   fast = 5 (move 2)
 *
 *   1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *             S         F
 *
 * Step 3:
 *   fast->next = NULL, STOP
 *
 * slow = 3 = MIDDLE!
 *
 * ============================================================================
 * VISUAL WALKTHROUGH (Even Length):
 * ============================================================================
 *
 * List: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL
 *
 * Initial:
 *   slow = 1, fast = 1
 *
 * Step 1:
 *   slow = 2, fast = 3
 *
 * Step 2:
 *   slow = 3, fast = 5
 *
 * Step 3:
 *   slow = 4, fast = NULL (after moving 2)
 *
 * fast = NULL, STOP
 * slow = 4 = SECOND MIDDLE!
 *
 *   1 -> 2 -> 3 -> 4 -> 5 -> 6
 *             ^    ^
 *           first second
 *           middle middle
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   slow = head
 *   fast = head
 *
 *   while (fast != NULL AND fast->next != NULL):
 *       slow = slow->next
 *       fast = fast->next->next
 *
 *   return slow  // Middle node
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Fast pointer traverses entire list once
 * - Actually n/2 iterations
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only two pointers
 *
 * ============================================================================
 * VARIATIONS:
 * ============================================================================
 *
 * 1. Return first middle for even length:
 *    - Start fast one step ahead: fast = head->next
 *
 * 2. Return middle value (not node):
 *    - Return slow->data
 *
 * 3. Delete middle node:
 *    - Need to track prev pointer too
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why use slow/fast pointers instead of counting length first?"
 * A1: Both work, but slow/fast is better:
 *
 *     Count length approach:
 *     - Pass 1: Count nodes (n iterations)
 *     - Pass 2: Go to n/2 (n/2 iterations)
 *     - Total: 1.5n iterations
 *
 *     Slow/fast approach:
 *     - Single pass: n/2 iterations
 *     - Total: 0.5n iterations (3x faster!)
 *
 *     ALWAYS mention: "Slow/fast requires only one pass through the list"
 *
 * -------------------------------------------------------------------------
 * Q2: "What if I need the FIRST middle for even-length lists?"
 * A2: Start fast one position ahead:
 *
 *     Node* fast = head->next;  // Instead of head
 *
 *     This makes slow stop at the first middle because fast reaches
 *     NULL one step earlier.
 *
 * -------------------------------------------------------------------------
 * Q3: "How would you DELETE the middle node?"
 * A3: You need to track the previous node:
 *
 *     Node* slow = head;
 *     Node* fast = head;
 *     Node* prev = NULL;
 *
 *     while (fast && fast->next) {
 *         prev = slow;
 *         slow = slow->next;
 *         fast = fast->next->next;
 *     }
 *
 *     prev->next = slow->next;  // Skip middle
 *     free(slow);               // Free middle node
 *
 * -------------------------------------------------------------------------
 * Q4: "Can this technique find the 1/3 or 1/4 point?"
 * A4: Yes! Adjust the speed ratio:
 *
 *     For 1/3 point: fast moves 3 steps, slow moves 1
 *     For 1/4 point: fast moves 4 steps, slow moves 1
 *
 *     General formula: For 1/k point, fast moves k steps per iteration
 *
 * -------------------------------------------------------------------------
 * Q5: "What's the loop condition 'fast != NULL && fast->next != NULL'?"
 * A5: This handles both odd and even length lists:
 *
 *     - fast != NULL: Handles even length (fast becomes NULL)
 *     - fast->next != NULL: Handles odd length (fast->next becomes NULL)
 *
 *     If we only check fast != NULL, we'd crash on fast->next->next
 *     when fast->next is NULL!
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

// Find middle (returns second middle for even length)
Node* findMiddle(Node* head) {
    // Edge case: empty list
    // Say: "First check if the list is empty"
    if (head == NULL) return NULL;

    // Initialize slow pointer at head
    // Say: "I'll use the tortoise and hare technique with two pointers"
    Node* slow = head;

    // Initialize fast pointer at head
    // Say: "Both pointers start at the head"
    Node* fast = head;

    // Traverse until fast reaches end
    // Say: "Move slow by 1 and fast by 2 until fast reaches the end"
    while (fast != NULL && fast->next != NULL) {
        // Move slow one step
        // Say: "Slow moves one step forward"
        slow = slow->next;

        // Move fast two steps
        // Say: "Fast moves two steps forward"
        fast = fast->next->next;
    }

    // When fast reaches end, slow is at middle
    // Say: "When fast reaches the end, slow is at the middle"
    return slow;
}

// Find first middle for even length
Node* findFirstMiddle(Node* head) {
    // Edge case: empty list
    // Say: "Handle the edge case of an empty list"
    if (head == NULL) return NULL;

    // Initialize slow at head
    // Say: "For the first middle, I'll start slow at head"
    Node* slow = head;

    // Initialize fast one step ahead
    // Say: "And start fast one step ahead at head->next"
    Node* fast = head->next;

    // Traverse with adjusted starting position
    // Say: "This adjustment makes slow land on the first middle for even-length lists"
    while (fast != NULL && fast->next != NULL) {
        // Move slow one step
        slow = slow->next;

        // Move fast two steps
        fast = fast->next->next;
    }

    // Return first middle
    // Say: "Return slow which points to the first middle"
    return slow;
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
 * WHY WE CAN USE head DIRECTLY (instead of creating temp pointer):
 *   - Parameters are COPIES in C (pass by value)
 *   - Original pointer in main() is NOT modified
 *   - Using head directly saves creating an extra variable
 *
 * while (head != NULL):
 *   - Loop continues as long as head points to a valid node
 *   - When head becomes NULL, we've passed the last node
 *   - NULL acts as the "end of list" marker
 *
 * printf("%d", head->data):
 *   - head->data accesses the data field of current node
 *   - %d prints the integer value
 *   - Equivalent to: (*head).data
 *
 * if (head->next) printf(" -> "):
 *   - head->next is a pointer (address or NULL)
 *   - Non-NULL pointer evaluates to TRUE
 *   - NULL pointer evaluates to FALSE
 *   - Only print arrow if there's a next node (cleaner output)
 *
 * head = head->next:
 *   - Move head to point to the next node
 *   - This is the "traversal step" - advances through the list
 *   - Original head in caller is UNCHANGED (we modified the copy)
 *
 * TRAVERSAL VISUALIZATION:
 *   List: 1 -> 2 -> 3 -> NULL
 *
 *   Iteration 1: head points to node(1), print "1"
 *                head->next exists, print " -> "
 *                head = head->next (now points to node(2))
 *
 *   Iteration 2: head points to node(2), print "2"
 *                head->next exists, print " -> "
 *                head = head->next (now points to node(3))
 *
 *   Iteration 3: head points to node(3), print "3"
 *                head->next is NULL, don't print arrow
 *                head = head->next (now head = NULL)
 *
 *   Loop ends: head == NULL
 *   Print " -> NULL\n" for clarity
 *
 *   Output: "1 -> 2 -> 3 -> NULL"
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

        // Move to next node (this modifies our LOCAL copy of head)
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
 * if (n == 0) return NULL:
 *   - Edge case: empty array means empty list
 *   - Return NULL immediately (no nodes to create)
 *   - ALWAYS handle edge cases first!
 *
 * Node* head = createNode(arr[0]):
 *   - Create the FIRST node with first array element
 *   - This becomes the HEAD of our list
 *   - We save this pointer to return at the end
 *
 * Node* curr = head:
 *   - "curr" is our traversal pointer (current position)
 *   - Starts at head, will move forward as we add nodes
 *   - We need this because we can't move head (would lose the list!)
 *
 * WHY WE NEED TWO POINTERS (head and curr):
 *   - "head" stays at the first node (so we can return it)
 *   - "curr" moves forward as we build the list
 *   - If we only used head and moved it, we'd lose track of the start!
 *
 * for (int i = 1; i < n; i++):
 *   - Start at index 1 (we already used arr[0] for head)
 *   - Loop through remaining elements
 *   - Each iteration adds one node to the list
 *
 * curr->next = createNode(arr[i]):
 *   - Create a new node with arr[i]'s value
 *   - Link it to current node's "next" pointer
 *   - This CONNECTS the new node to the list
 *
 * curr = curr->next:
 *   - Move curr to point to the newly added node
 *   - Now curr is at the END of the list
 *   - Ready to add next node in the next iteration
 *
 * return head:
 *   - Return pointer to FIRST node (head never moved!)
 *   - Caller now has access to the entire list through head
 *
 * STEP-BY-STEP VISUALIZATION for arr = {1, 2, 3}:
 *
 *   After head = createNode(arr[0]):
 *     head -> [1|NULL]
 *     curr -> [1|NULL]  (same as head)
 *
 *   Loop i=1: curr->next = createNode(arr[1])
 *     head -> [1| *]----> [2|NULL]
 *     curr -> [1| *]  (still at node 1)
 *
 *   Loop i=1: curr = curr->next
 *     head -> [1| *]----> [2|NULL]
 *                         curr (now at node 2)
 *
 *   Loop i=2: curr->next = createNode(arr[2])
 *     head -> [1| *]----> [2| *]----> [3|NULL]
 *                         curr
 *
 *   Loop i=2: curr = curr->next
 *     head -> [1| *]----> [2| *]----> [3|NULL]
 *                                     curr (now at node 3)
 *
 *   Return head: Caller gets pointer to [1| *]
 *                Can traverse: 1 -> 2 -> 3 -> NULL
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
 * Node* temp:
 *   - We need a temporary pointer to save the next node
 *   - WHY? After free(head), we can't access head->next anymore!
 *   - The memory is gone, accessing it is UNDEFINED BEHAVIOR
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
    printf("=== Find Middle of Linked List ===\n\n");

    // Odd length
    int arr1[] = {1, 2, 3, 4, 5};
    Node* list1 = createList(arr1, 5);

    printf("List (odd): ");
    printList(list1);
    printf("Middle: %d\n\n", findMiddle(list1)->data);

    // Even length
    int arr2[] = {1, 2, 3, 4, 5, 6};
    Node* list2 = createList(arr2, 6);

    printf("List (even): ");
    printList(list2);
    printf("Second middle: %d\n", findMiddle(list2)->data);
    printf("First middle:  %d\n\n", findFirstMiddle(list2)->data);

    // Single element
    int arr3[] = {42};
    Node* list3 = createList(arr3, 1);

    printf("List (single): ");
    printList(list3);
    printf("Middle: %d\n\n", findMiddle(list3)->data);

    // Two elements
    int arr4[] = {1, 2};
    Node* list4 = createList(arr4, 2);

    printf("List (two): ");
    printList(list4);
    printf("Second middle: %d\n", findMiddle(list4)->data);
    printf("First middle:  %d\n", findFirstMiddle(list4)->data);

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
    freeList(list4);

    printf("\n=== Memory freed successfully ===\n");

    return 0;
}
