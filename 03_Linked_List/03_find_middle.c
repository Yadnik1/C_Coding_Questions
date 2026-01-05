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

/*
 * ============================================================================
 * FIND MIDDLE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* findMiddle(Node* head):
 *   - Returns "Node*" = pointer to the middle node
 *   - "Node* head" = pointer to the first node of the list
 *   - Purpose: Find the middle node using slow/fast pointer technique
 *   - For even-length lists, returns the SECOND middle node
 *
 * ALGORITHM: Slow/Fast Pointer (Tortoise and Hare)
 * ---------------------------------
 * Two pointers move at different speeds:
 *   - Slow (tortoise): moves 1 step per iteration
 *   - Fast (hare): moves 2 steps per iteration
 *
 * When fast reaches the end, slow is at the middle!
 *
 * WHY THIS WORKS - MATHEMATICAL INTUITION:
 * ---------------------------------
 * Let n = total nodes in the list
 *
 * When fast pointer reaches the end:
 *   - Fast has traveled n steps (entire list)
 *   - Slow has traveled n/2 steps (half the list)
 *   - Slow is at position n/2 = middle!
 *
 * For ODD length (n=5):
 *   - Positions: 1, 2, 3, 4, 5
 *   - Middle is position 3 (index 2)
 *   - Fast ends at position 5, slow at position 3 ✓
 *
 * For EVEN length (n=6):
 *   - Positions: 1, 2, 3, 4, 5, 6
 *   - Two middles: 3 (first) and 4 (second)
 *   - Fast ends past position 6 (NULL), slow at position 4 ✓
 *   - Returns SECOND middle (4)
 *
 * WHY CHECK BOTH fast != NULL AND fast->next != NULL?
 * ---------------------------------
 * - fast != NULL: Handles EVEN length lists
 *   (fast becomes NULL after jumping past the last node)
 * - fast->next != NULL: Handles ODD length lists
 *   (fast lands on last node, can't take another step)
 *
 * Without both checks, we'd crash trying to access fast->next->next
 * when fast or fast->next is NULL!
 *
 * VISUALIZATION - ODD LENGTH (1->2->3->4->5):
 * ---------------------------------
 *   Start:    S,F at 1
 *   Step 1:   S at 2, F at 3
 *   Step 2:   S at 3, F at 5
 *   F->next is NULL, STOP
 *   Return slow (node 3) ✓
 *
 * VISUALIZATION - EVEN LENGTH (1->2->3->4->5->6):
 * ---------------------------------
 *   Start:    S,F at 1
 *   Step 1:   S at 2, F at 3
 *   Step 2:   S at 3, F at 5
 *   Step 3:   S at 4, F at NULL (jumped past 6)
 *   F is NULL, STOP
 *   Return slow (node 4 = second middle) ✓
 *
 * TIME COMPLEXITY: O(n/2) = O(n)
 * ---------------------------------
 * - Only n/2 iterations (fast traverses while slow moves half)
 * - Much faster than counting length first (1.5n operations)
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 * - Only two pointer variables
 * - No extra data structures needed
 *
 * ============================================================================
 */
// Find middle node (returns second middle for even length)
// Say: "I'll use the slow/fast pointer technique to find the middle"
Node* findMiddle(Node* head) {
    // =========================================================================
    // EDGE CASE: Empty list has no middle
    // =========================================================================
    // Say: "First, I check if the list is empty"
    // WHY check NULL? Can't find middle of empty list
    if (head == NULL) return NULL;

    // =========================================================================
    // INITIALIZE: Both pointers start at head
    // =========================================================================
    // Say: "I initialize slow and fast pointers, both starting at head"
    // WHY both at head? Both "runners" start at the same position
    Node* slow = head;  // Tortoise: moves 1 step per iteration
    Node* fast = head;  // Hare: moves 2 steps per iteration

    // =========================================================================
    // TRAVERSE: Move pointers until fast reaches the end
    // =========================================================================
    // Say: "I move slow by 1 and fast by 2 until fast reaches the end"
    // WHY fast != NULL? For even length, fast becomes NULL after last jump
    // WHY fast->next != NULL? For odd length, fast lands on last node
    // Both conditions prevent crash when accessing fast->next->next
    while (fast != NULL && fast->next != NULL) {
        // Move slow pointer one step forward (tortoise speed)
        // Say: "I move slow one step forward"
        // WHY one step? Slow covers half the distance of fast
        slow = slow->next;

        // Move fast pointer two steps forward (hare speed)
        // Say: "I move fast two steps forward"
        // WHY two steps? When fast reaches end, slow is at middle
        // This is safe because we checked fast->next != NULL above
        fast = fast->next->next;
    }

    // =========================================================================
    // RETURN: Slow is now at the middle
    // =========================================================================
    // Say: "When fast reaches the end, slow is at the middle"
    // WHY slow? Fast traveled full distance, slow traveled half = middle
    return slow;
}

/*
 * ============================================================================
 * FIND FIRST MIDDLE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* findFirstMiddle(Node* head):
 *   - Returns "Node*" = pointer to the FIRST middle node
 *   - "Node* head" = pointer to the first node of the list
 *   - Purpose: Find middle for even-length lists where we want the FIRST middle
 *   - For odd-length lists, behaves same as findMiddle (only one middle exists)
 *
 * KEY DIFFERENCE FROM findMiddle():
 * ---------------------------------
 * The ONLY difference is where fast starts:
 *   - findMiddle():      fast = head        → returns SECOND middle
 *   - findFirstMiddle(): fast = head->next  → returns FIRST middle
 *
 * WHY STARTING FAST ONE AHEAD WORKS:
 * ---------------------------------
 * By starting fast one position ahead, we effectively make fast "reach the end"
 * one iteration earlier. This causes slow to stop one node earlier too!
 *
 * MATHEMATICAL INTUITION:
 * ---------------------------------
 * For even length n:
 *   - First middle is at position n/2
 *   - Second middle is at position (n/2) + 1
 *
 * Starting fast at head->next is like having a list that's 1 shorter from
 * fast's perspective, so slow stops 1 position earlier.
 *
 * VISUALIZATION - EVEN LENGTH (1->2->3->4->5->6):
 * ---------------------------------
 *   Start:    S at 1, F at 2 (one ahead!)
 *
 *   Step 1:   S at 2, F at 4
 *             1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL
 *                  S         F
 *
 *   Step 2:   S at 3, F at 6
 *             1 -> 2 -> 3 -> 4 -> 5 -> 6 -> NULL
 *                       S              F
 *
 *   Step 3:   F->next is NULL, STOP
 *             Return slow (node 3 = FIRST middle) ✓
 *
 * COMPARE WITH findMiddle() ON SAME LIST:
 * ---------------------------------
 *   findMiddle():      Returns 4 (second middle)
 *   findFirstMiddle(): Returns 3 (first middle)
 *
 * VISUALIZATION - ODD LENGTH (1->2->3->4->5):
 * ---------------------------------
 *   Start:    S at 1, F at 2
 *
 *   Step 1:   S at 2, F at 4
 *   Step 2:   S at 3, F at NULL (jumped past 5)
 *
 *   F is NULL, STOP
 *   Return slow (node 3) - same as findMiddle()!
 *
 * For odd length, both functions return the same middle (there's only one)
 *
 * WHEN TO USE EACH FUNCTION:
 * ---------------------------------
 * - findMiddle(): When you need the second middle (splitting for merge sort)
 * - findFirstMiddle(): When you need the first middle (some interview variants)
 *
 * TIME COMPLEXITY: O(n/2) = O(n)
 * ---------------------------------
 * - Same as findMiddle() - only n/2 iterations needed
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 * - Only two pointer variables
 *
 * ============================================================================
 */
// Find first middle for even length (same middle for odd length)
// Say: "I'll use slow/fast but start fast one ahead to get the first middle"
Node* findFirstMiddle(Node* head) {
    // =========================================================================
    // EDGE CASE: Empty list has no middle
    // =========================================================================
    // Say: "First, I check if the list is empty"
    // WHY check NULL? Can't find middle of empty list
    if (head == NULL) return NULL;

    // =========================================================================
    // INITIALIZE SLOW: Starts at head (same as findMiddle)
    // =========================================================================
    // Say: "I initialize slow pointer at head"
    // WHY at head? Slow always starts at the beginning
    Node* slow = head;

    // =========================================================================
    // INITIALIZE FAST: Starts ONE AHEAD at head->next (KEY DIFFERENCE!)
    // =========================================================================
    // Say: "I initialize fast pointer at head->next - this is the key difference!"
    // WHY one ahead? This makes slow stop at the FIRST middle instead of second
    // It's like making the list "appear shorter" to fast by 1 node
    Node* fast = head->next;

    // =========================================================================
    // TRAVERSE: Move pointers until fast reaches the end
    // =========================================================================
    // Say: "I traverse with slow moving 1 step and fast moving 2 steps"
    // WHY same loop condition? We still need to prevent accessing NULL
    while (fast != NULL && fast->next != NULL) {
        // Move slow pointer one step forward
        // Say: "I move slow one step forward"
        // WHY one step? Same reason as findMiddle - slow covers half the distance
        slow = slow->next;

        // Move fast pointer two steps forward
        // Say: "I move fast two steps forward"
        // WHY two steps? Fast reaches end faster, but started ahead
        fast = fast->next->next;
    }

    // =========================================================================
    // RETURN: Slow is now at the FIRST middle
    // =========================================================================
    // Say: "When fast reaches the end, slow is at the first middle"
    // WHY first middle? Because fast started one ahead, slow stopped one earlier
    return slow;
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
