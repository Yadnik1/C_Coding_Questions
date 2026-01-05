/*
 * ============================================================================
 * PROBLEM: Reverse a Linked List
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH (MUST KNOW!)
 *
 * Reverse a singly linked list in-place.
 *
 * Example:
 * Input:  1 -> 2 -> 3 -> 4 -> 5 -> NULL
 * Output: 5 -> 4 -> 3 -> 2 -> 1 -> NULL
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. LINKED LIST BASICS:
 *    - Each node has: data + pointer to next
 *    - Head points to first node
 *    - Last node points to NULL
 *
 * 2. THREE POINTER TECHNIQUE:
 *    - prev: tracks previous node (starts NULL)
 *    - curr: current node being processed
 *    - next: temporarily stores next node
 *
 * 3. REVERSE DIRECTION:
 *    - Change curr->next to point to prev
 *    - Move all pointers one step forward
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * Original: 1 -> 2 -> 3 -> NULL
 *
 * Initial state:
 *   prev = NULL
 *   curr = 1
 *   next = ?
 *
 *   NULL   1 -> 2 -> 3 -> NULL
 *    ^     ^
 *   prev  curr
 *
 * Step 1: Save next, reverse link
 *   next = curr->next (save 2)
 *   curr->next = prev (1 points to NULL)
 *   prev = curr (prev moves to 1)
 *   curr = next (curr moves to 2)
 *
 *   NULL <- 1    2 -> 3 -> NULL
 *           ^    ^
 *          prev curr
 *
 * Step 2: Repeat
 *   next = 3
 *   curr->next = prev (2 points to 1)
 *   prev = 2
 *   curr = 3
 *
 *   NULL <- 1 <- 2    3 -> NULL
 *                ^    ^
 *               prev curr
 *
 * Step 3: Repeat
 *   next = NULL
 *   curr->next = prev (3 points to 2)
 *   prev = 3
 *   curr = NULL
 *
 *   NULL <- 1 <- 2 <- 3
 *                     ^
 *                    prev (new head!)
 *
 * curr is NULL, STOP
 * Return prev as new head
 *
 * Result: 3 -> 2 -> 1 -> NULL
 *
 * ============================================================================
 * POINTER MOVEMENT VISUALIZATION:
 * ============================================================================
 *
 *   +------+    +------+    +------+
 *   |  1   | -> |  2   | -> |  3   | -> NULL
 *   +------+    +------+    +------+
 *
 *   Before each iteration:
 *   1. Save: next = curr->next
 *   2. Reverse: curr->next = prev
 *   3. Move prev: prev = curr
 *   4. Move curr: curr = next
 *
 *   +------+    +------+    +------+
 *   |  1   | <- |  2   | <- |  3   |
 *   +------+    +------+    +------+
 *   NULL <-          new head ->
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   prev = NULL
 *   curr = head
 *
 *   while (curr != NULL):
 *       next = curr->next     // Save
 *       curr->next = prev     // Reverse
 *       prev = curr           // Move prev
 *       curr = next           // Move curr
 *
 *   return prev  // New head
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Visit each node exactly once
 * - Constant operations per node
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only three pointers
 * - In-place reversal
 *
 * ============================================================================
 * RECURSIVE VERSION:
 * ============================================================================
 *
 *   Node* reverseRecursive(Node* head) {
 *       if (head == NULL || head->next == NULL)
 *           return head;
 *
 *       Node* newHead = reverseRecursive(head->next);
 *       head->next->next = head;
 *       head->next = NULL;
 *
 *       return newHead;
 *   }
 *
 *   Space: O(n) due to recursion stack
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why do you need three pointers (prev, curr, next)?"
 * A1: Each pointer has a specific job:
 *     - prev: Tracks the node that curr should point to after reversal
 *     - curr: The node we're currently processing
 *     - next: Saves the next node BEFORE we break the link
 *
 *     Without 'next': After curr->next = prev, we lose access to rest of list!
 *     Without 'prev': We don't know what to point curr to!
 *
 * -------------------------------------------------------------------------
 * Q2: "Can you reverse without extra pointers?"
 * A2: No, you need at least 3 pointers for iterative. Here's why:
 *     - You must save the next node before modifying curr->next
 *     - You need to track where curr should point (prev)
 *     - You need to know current position (curr)
 *
 *     Recursive uses call stack instead (O(n) space).
 *
 * -------------------------------------------------------------------------
 * Q3: "Iterative vs Recursive - which is better?"
 * A3: For interviews and embedded systems, ITERATIVE is preferred:
 *
 *     Iterative:
 *     - O(1) space (no stack frames)
 *     - No risk of stack overflow for long lists
 *     - Easier to debug
 *
 *     Recursive:
 *     - O(n) space (call stack)
 *     - Can overflow stack for long lists
 *     - More elegant code, harder to understand
 *
 *     ALWAYS mention: "In embedded, I'd use iterative to avoid stack overflow"
 *
 * -------------------------------------------------------------------------
 * Q4: "What happens if you reverse an empty list or single node?"
 * A4: Both are handled correctly:
 *     - Empty list (head = NULL): While loop never executes, return NULL
 *     - Single node: While loop runs once, node points to NULL, return node
 *
 *     No special handling needed - the algorithm handles edge cases naturally!
 *
 * -------------------------------------------------------------------------
 * Q5: "How would you reverse only a portion of the list (nodes m to n)?"
 * A5: This is a follow-up question! Approach:
 *     1. Traverse to node m-1 (connection point)
 *     2. Reverse nodes from m to n using same technique
 *     3. Reconnect: (m-1)->next = n, m->next = (n+1)
 *
 *     Key insight: Save the node before m and the node after n
 *
 * -------------------------------------------------------------------------
 * Q6: "How do you reverse in groups of k?"
 * A6: Another common follow-up:
 *     1. Reverse first k nodes
 *     2. Recursively reverse rest of list in groups of k
 *     3. Connect the reversed group to the result of recursion
 *
 *     Time: O(n), Space: O(n/k) for recursion
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
 *     int data;
 *     struct Node* next;
 * } Node;
 *
 * LINE 1: typedef struct Node {
 * ---------------------------------
 * - "typedef" = We're creating a new type name (alias)
 * - "struct Node" = We're defining a structure AND giving it a tag name "Node"
 * - The "{" opens the structure definition
 * - We use "struct Node" as the tag so we can reference it inside itself (for next pointer)
 *
 * LINE 2: int data;
 * ---------------------------------
 * - This is the PAYLOAD - the actual value stored in the node
 * - Can be any type: int, char, float, or even another struct
 * - Each node carries one piece of data
 *
 * LINE 3: struct Node* next;
 * ---------------------------------
 * - This is the LINK - pointer to the next node in the list
 * - "struct Node*" = pointer to another Node structure
 * - We MUST say "struct Node*" here (not just "Node*") because
 *   the typedef alias "Node" isn't complete yet at this point!
 * - This is what makes it a "linked" list - nodes point to each other
 * - Last node has next = NULL (end of list marker)
 *
 * LINE 4: } Node;
 * ---------------------------------
 * - "}" closes the structure definition
 * - "Node" after the "}" completes the typedef
 * - Now we can use "Node" as a type instead of "struct Node"
 * - Example: Node myNode; instead of struct Node myNode;
 *
 * MEMORY LAYOUT (assuming 64-bit system):
 * ---------------------------------
 *   +------------------+------------------+
 *   |      data        |      next        |
 *   |    (4 bytes)     |    (8 bytes)     |
 *   +------------------+------------------+
 *   Total: 12-16 bytes (with padding)
 *
 * VISUAL REPRESENTATION:
 * ---------------------------------
 *   Node A          Node B          Node C
 *   +------+---+    +------+---+    +------+------+
 *   | data |  -+--->| data |  -+--->| data | NULL |
 *   +------+---+    +------+---+    +------+------+
 *      10              20              30
 *
 * ============================================================================
 */

// Node structure definition
typedef struct Node {
    // The data field stores the actual value (payload) of this node
    // Say: "Each node has a data field to store the integer value"
    int data;

    // The next pointer stores the address of the next node in the list
    // Say: "And a next pointer that points to the next node, or NULL if this is the last node"
    // NOTE: We use "struct Node*" because the typedef "Node" isn't complete yet
    struct Node* next;
} Node;  // "Node" is now an alias for "struct Node"

/*
 * ============================================================================
 * CREATE NODE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* createNode(int data) {
 *     Node* newNode = (Node*)malloc(sizeof(Node));
 *     newNode->data = data;
 *     newNode->next = NULL;
 *     return newNode;
 * }
 *
 * LINE 1: Node* createNode(int data) {
 * ---------------------------------
 * - "Node*" = This function RETURNS a pointer to a Node
 * - "createNode" = Function name (descriptive!)
 * - "int data" = The value we want to store in the new node
 * - This is a FACTORY FUNCTION - it creates and returns new nodes
 *
 * LINE 2: Node* newNode = (Node*)malloc(sizeof(Node));
 * ---------------------------------
 * - "Node* newNode" = Declare a pointer that will hold address of new node
 * - "malloc(sizeof(Node))" = Request memory from heap for one Node
 *   - sizeof(Node) = calculates exact bytes needed for one Node structure
 *   - malloc returns void* (generic pointer) to the allocated memory
 *   - Returns NULL if allocation fails (out of memory)
 * - "(Node*)" = Cast the void* to Node* (required in C++)
 * - The node is allocated on the HEAP (persists after function returns)
 *   - Unlike local variables which are on STACK (destroyed after function)
 *
 * WHY HEAP ALLOCATION?
 * - Stack memory is freed when function returns
 * - Heap memory persists until we explicitly free() it
 * - Linked list nodes must outlive the function that creates them
 *
 * LINE 3: newNode->data = data;
 * ---------------------------------
 * - "newNode->data" = Access the 'data' field of the node pointed to by newNode
 * - "->" is the arrow operator: shorthand for (*newNode).data
 * - "= data" = Copy the parameter value into the node's data field
 * - This stores the actual value in our new node
 *
 * LINE 4: newNode->next = NULL;
 * ---------------------------------
 * - "newNode->next" = Access the 'next' pointer field
 * - "= NULL" = Set it to NULL (no next node yet)
 * - IMPORTANT: Always initialize pointers! Uninitialized pointers = bugs
 * - New nodes are "orphans" - not connected to any list yet
 * - The caller will set 'next' to link this node into a list
 *
 * LINE 5: return newNode;
 * ---------------------------------
 * - Return the pointer to our newly created node
 * - Caller receives the address of the node on the heap
 * - Caller is responsible for:
 *   1. Linking this node into a list
 *   2. Eventually calling free() to prevent memory leaks
 *
 * MEMORY VISUALIZATION:
 * ---------------------------------
 *
 *   BEFORE malloc:
 *   Stack                    Heap
 *   +--------+              (empty)
 *   | data=5 |
 *   +--------+
 *   |newNode |---> ???
 *   +--------+
 *
 *   AFTER malloc:
 *   Stack                    Heap
 *   +--------+              +------+------+
 *   | data=5 |              | ???  | ???  |
 *   +--------+              +------+------+
 *   |newNode |------------->| 0x100       |
 *   +--------+              +------+------+
 *
 *   AFTER initialization:
 *   Stack                    Heap
 *   +--------+              +------+------+
 *   | data=5 |              |  5   | NULL |
 *   +--------+              +------+------+
 *   |newNode |------------->| 0x100       |
 *   +--------+              +------+------+
 *
 * ============================================================================
 */

// Create a new node with given data
Node* createNode(int data) {
    // Allocate memory on the HEAP for one Node structure
    // malloc returns pointer to allocated memory, or NULL if failed
    // sizeof(Node) calculates the exact number of bytes needed
    // (Node*) casts the void* returned by malloc to Node*
    // Say: "First, I allocate memory for a new node using malloc"
    Node* newNode = (Node*)malloc(sizeof(Node));

    // OPTIONAL: Check if malloc succeeded (good practice!)
    // if (newNode == NULL) return NULL;

    // Store the data value in the node's data field
    // The -> operator accesses struct members through a pointer
    // This is equivalent to: (*newNode).data = data;
    // Say: "Then I store the data value in the node's data field"
    newNode->data = data;

    // Initialize next pointer to NULL (not connected to any node yet)
    // CRITICAL: Always initialize pointers to avoid undefined behavior
    // Say: "I set the next pointer to NULL since this node isn't linked yet"
    newNode->next = NULL;

    // Return pointer to the newly created node
    // Caller will link this node into a list
    // Say: "Finally, I return the pointer to the new node"
    return newNode;
}

/*
 * ============================================================================
 * REVERSE ITERATIVE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* reverseIterative(Node* head):
 *   - Returns "Node*" = pointer to the new head (was the last node)
 *   - "Node* head" = pointer to the first node of the list
 *   - Purpose: Reverse the linked list in-place using iteration
 *
 * WHY THREE POINTERS (prev, curr, next)?
 * ---------------------------------
 * - prev: Tracks the node that curr should point to after reversal
 *         Starts at NULL because the new tail (old head) should point to NULL
 * - curr: The node we're currently processing
 *         Moves through the list from head to end
 * - next: Temporarily saves curr->next BEFORE we modify it
 *         Without this, we'd lose the rest of the list!
 *
 * THE FOUR-STEP DANCE (in each iteration):
 * ---------------------------------
 *   1. SAVE:    next = curr->next     (save before we break the link)
 *   2. REVERSE: curr->next = prev     (point backward instead of forward)
 *   3. ADVANCE: prev = curr           (move prev to current position)
 *   4. ADVANCE: curr = next           (move curr to saved next position)
 *
 * VISUALIZATION OF ONE ITERATION:
 * ---------------------------------
 *   Before:  NULL <- [1] <- [2]    [3] -> [4] -> NULL
 *                     ^     ^       ^
 *                   prev   curr    next
 *
 *   Step 1: next = curr->next (next now points to [4])
 *   Step 2: curr->next = prev ([3] now points to [2])
 *   Step 3: prev = curr (prev now points to [3])
 *   Step 4: curr = next (curr now points to [4])
 *
 *   After:   NULL <- [1] <- [2] <- [3]    [4] -> NULL
 *                                   ^      ^
 *                                  prev   curr
 *
 * WHY IS ITERATIVE PREFERRED?
 * ---------------------------------
 * - O(1) space - only 3 pointers, no matter how long the list
 * - No risk of stack overflow for very long lists
 * - Easier to debug and understand step by step
 * - Preferred for embedded systems with limited stack space
 *
 * ============================================================================
 */
// Iterative reversal (PREFERRED for production/embedded)
// Say: "I'll reverse the list iteratively using three pointers"
Node* reverseIterative(Node* head) {
    // Initialize previous pointer to NULL
    // Say: "I initialize prev to NULL because the new tail points to NULL"
    // WHY NULL? The first node (head) will become the last node, pointing to NULL
    Node* prev = NULL;

    // Current pointer starts at head - this is what we're processing
    // Say: "Current starts at the head - this is the node I'm currently reversing"
    // WHY head? We start reversing from the first node
    Node* curr = head;

    // Next pointer for temporary storage - will be set inside loop
    // Say: "Next will temporarily store the node ahead before I break the link"
    // WHY declare here? We need it to persist across loop iterations
    Node* next = NULL;

    // Traverse until we reach the end (curr becomes NULL)
    // Say: "I'll iterate through the list until curr becomes NULL"
    // WHY curr != NULL? When curr is NULL, we've processed all nodes
    while (curr != NULL) {
        // =====================================================================
        // STEP 1: SAVE - Save the next node before we break the forward link
        // =====================================================================
        // Say: "First, I save curr's next pointer before I modify it"
        // WHY save first? After curr->next = prev, we lose access to the rest!
        // This is the CRITICAL step - if you forget this, you lose the list!
        next = curr->next;

        // =====================================================================
        // STEP 2: REVERSE - Point current node backward (to prev)
        // =====================================================================
        // Say: "Now I reverse the link by pointing curr's next to prev"
        // WHY prev? prev is the node that should come AFTER curr in reversed list
        // This is the actual reversal - changing the arrow direction!
        curr->next = prev;

        // =====================================================================
        // STEP 3: ADVANCE PREV - Move prev to current position
        // =====================================================================
        // Say: "Move prev forward to the current node"
        // WHY? For the next iteration, curr needs to point to this node
        // prev always trails one step behind curr
        prev = curr;

        // =====================================================================
        // STEP 4: ADVANCE CURR - Move curr to the next node (which we saved)
        // =====================================================================
        // Say: "Move curr forward to the node I saved earlier"
        // WHY use next? We can't use curr->next anymore (it points to prev!)
        // This advances our processing to the next node
        curr = next;
    }

    // When loop ends: curr is NULL, prev points to the last node (new head)
    // Say: "When I exit the loop, prev is pointing to the new head of the reversed list"
    // WHY return prev? curr is NULL, but prev points to what was the last node
    // That last node is now the first node of the reversed list!
    return prev;
}

/*
 * ============================================================================
 * REVERSE RECURSIVE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* reverseRecursive(Node* head):
 *   - Returns "Node*" = pointer to the new head (was the last node)
 *   - "Node* head" = pointer to the current node being processed
 *   - Purpose: Reverse the linked list using recursion
 *
 * HOW RECURSION WORKS HERE:
 * ---------------------------------
 * 1. Recursively go to the END of the list first
 * 2. On the way BACK (unwinding), reverse each link
 * 3. The last node becomes the new head and is passed all the way up
 *
 * VISUALIZATION: Reversing 1 -> 2 -> 3 -> NULL
 * ---------------------------------
 *
 *   Going DOWN (recursive calls):
 *   reverseRecursive(1) -> reverseRecursive(2) -> reverseRecursive(3)
 *                                                  ↓
 *                                             return 3 (base case!)
 *
 *   Coming BACK UP (unwinding):
 *   At node 2: head->next->next = head means 3->next = 2
 *              head->next = NULL means 2->next = NULL (temporarily)
 *              Result: 3 -> 2 -> NULL
 *
 *   At node 1: head->next->next = head means 2->next = 1
 *              head->next = NULL means 1->next = NULL
 *              Result: 3 -> 2 -> 1 -> NULL
 *
 * THE MAGIC LINE: head->next->next = head
 * ---------------------------------
 *   Before: head -> [A] -> [B] -> ...
 *                    ^      ^
 *                   head   head->next
 *
 *   head->next->next = head means:
 *   "Make the node AFTER me point BACK to me"
 *
 *   After: head -> [A] <-> [B] -> ... (temporarily bidirectional!)
 *
 *   Then head->next = NULL breaks the forward link:
 *   After: ... <- [A] <- [B] -> ...
 *
 * WHY RECURSIVE IS NOT PREFERRED:
 * ---------------------------------
 * - O(n) space due to call stack (each recursive call uses stack frame)
 * - Risk of stack overflow for very long lists
 * - Harder to debug (need to trace through call stack)
 * - But: More elegant and concise code
 *
 * CALL STACK VISUALIZATION (for 1->2->3):
 * ---------------------------------
 *   STACK GROWING DOWN:
 *   | reverseRecursive(1)  | waiting for result...
 *   | reverseRecursive(2)  | waiting for result...
 *   | reverseRecursive(3)  | BASE CASE! Returns 3
 *   +----------------------+
 *
 *   STACK UNWINDING:
 *   | reverseRecursive(1)  | gets 3, reverses 1<-2, returns 3
 *   | reverseRecursive(2)  | gets 3, reverses 2<-3, returns 3
 *   | reverseRecursive(3)  | returned 3
 *   +----------------------+
 *
 * ============================================================================
 */
// Recursive reversal (elegant but uses O(n) stack space)
// Say: "I'll reverse the list recursively by going to the end first, then reversing on the way back"
Node* reverseRecursive(Node* head) {
    // =========================================================================
    // BASE CASE: Empty list or single node - already reversed!
    // =========================================================================
    // Say: "Base case: if empty or single node, just return it - it's already reversed"
    // WHY head == NULL? Empty list, nothing to reverse
    // WHY head->next == NULL? Single node is already "reversed"
    // This is also where recursion stops and starts unwinding
    if (head == NULL || head->next == NULL) {
        return head;  // This becomes the new head of the reversed list!
    }

    // =========================================================================
    // RECURSIVE CASE: First, reverse everything AFTER this node
    // =========================================================================
    // Say: "Recursively reverse everything after the current node"
    // WHY head->next? We're saying "reverse the rest of the list starting from next node"
    // This call will eventually return the LAST node (which becomes the new head)
    // The newHead is the same for all unwinding calls - it's always the original last node
    Node* newHead = reverseRecursive(head->next);

    // =========================================================================
    // REVERSE THE LINK: Make the next node point back to current
    // =========================================================================
    // Say: "Make the node after me point back to me"
    // WHY head->next->next? Let's break it down:
    //   - head->next = the node after current (let's call it B)
    //   - head->next->next = B's next pointer
    //   - = head = make B's next point to current (A)
    // Before: A -> B -> ...
    // After:  A -> B -> A (circular temporarily!) but B was already reversed
    // Actually: ... <- B <- A (B already points backward from recursion)
    head->next->next = head;

    // =========================================================================
    // BREAK OLD LINK: Current node should point to NULL (for now)
    // =========================================================================
    // Say: "Current node is now at the end of its portion, so point to NULL"
    // WHY NULL? We need to break the old forward link to avoid cycles
    // The NEXT unwinding call will fix this by making the previous node point here
    // For the original head: this stays NULL, making it the new tail!
    head->next = NULL;

    // =========================================================================
    // RETURN: Pass the new head all the way up the call stack
    // =========================================================================
    // Say: "Return the new head which came from the deepest recursion"
    // WHY return newHead? newHead is the original LAST node
    // Every recursive call returns this same pointer
    // It gets passed all the way up to become the new head
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
 *   - Returns pointer to the head of the newly created list
 *   - "int arr[]" = Array of values to convert to linked list
 *   - "int n" = Number of elements in the array
 *
 * if (n == 0) return NULL:
 *   - Edge case: empty array means empty list
 *   - Return NULL to represent an empty linked list
 *
 * Node* head = createNode(arr[0]):
 *   - Create the first node with the first array element
 *   - This becomes the head of our list
 *
 * Node* curr = head:
 *   - Create a pointer to track the current (last) node
 *   - We'll use this to append new nodes
 *
 * for (int i = 1; i < n; i++):
 *   - Start from index 1 (we already used index 0 for head)
 *   - Loop through remaining array elements
 *
 * curr->next = createNode(arr[i]):
 *   - Create a new node with the current array element
 *   - Link it to the current node's next pointer
 *   - This appends the new node to the end of the list
 *
 * curr = curr->next:
 *   - Move curr to point to the newly added node
 *   - Now curr is at the end of the list again
 *
 * return head:
 *   - Return the pointer to the first node
 *   - Caller can access entire list through this head pointer
 *
 * VISUALIZATION of createList([1, 2, 3], 3):
 * ---------------------------------
 *
 *   After head = createNode(arr[0]):
 *   head -> [1|NULL]
 *   curr -> [1|NULL]
 *
 *   After i=1 (arr[1]=2):
 *   head -> [1| ]-->[2|NULL]
 *   curr ---------> [2|NULL]
 *
 *   After i=2 (arr[2]=3):
 *   head -> [1| ]-->[2| ]-->[3|NULL]
 *   curr -----------------> [3|NULL]
 *
 *   Return head (pointing to node with 1)
 *
 * ============================================================================
 */
// Create a linked list from an array of values
Node* createList(int arr[], int n) {
    // Edge case: empty array means empty list
    // Say: "If the array is empty, return NULL for an empty list"
    if (n == 0) return NULL;

    // Create the head node with the first array element
    // Say: "Create the first node with arr[0] as the head"
    Node* head = createNode(arr[0]);

    // Create a pointer to track the current (last) node
    // Say: "Use curr to keep track of where to append new nodes"
    Node* curr = head;

    // Loop through remaining array elements (starting from index 1)
    // Say: "For each remaining element, create a node and link it"
    for (int i = 1; i < n; i++) {
        // Create new node and link it to current node's next
        // Say: "Create a new node and attach it to curr's next pointer"
        curr->next = createNode(arr[i]);

        // Move curr to the newly created node (new end of list)
        // Say: "Move curr forward to the newly added node"
        curr = curr->next;
    }

    // Return head pointer - provides access to entire list
    // Say: "Return the head pointer to access the complete list"
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
 *   - Memory leaks can crash long-running programs (servers, embedded systems)
 *   - In interviews, mentioning memory cleanup shows professionalism
 *
 * Node* temp:
 *   - We need a temporary pointer to save the next node
 *   - WHY? After free(head), we can't access head->next anymore!
 *   - The memory is gone, accessing it is UNDEFINED BEHAVIOR
 *
 * while (head != NULL):
 *   - Loop through all nodes until we reach the end
 *   - Each iteration frees one node
 *
 * temp = head->next:
 *   - SAVE the next node BEFORE freeing current node
 *   - This is CRITICAL - order matters!
 *   - If we free first, head->next becomes invalid
 *
 * free(head):
 *   - Return this node's memory to the heap
 *   - After this, head points to FREED memory (dangling pointer)
 *   - We must NOT use head after this point
 *
 * head = temp:
 *   - Move to the next node (which we saved earlier)
 *   - Continue until all nodes are freed
 *
 * MEMORY VISUALIZATION:
 * ---------------------------------
 *
 *   Before freeList:
 *   Heap: [Node1] -> [Node2] -> [Node3] -> NULL
 *
 *   After freeList:
 *   Heap: (empty - all memory returned)
 *
 * COMMON MISTAKE:
 * ---------------------------------
 *   // WRONG! Don't do this:
 *   while (head != NULL) {
 *       free(head);           // head is now invalid!
 *       head = head->next;    // CRASH! Accessing freed memory
 *   }
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
        // WHY: After free(), head->next is invalid memory
        temp = head->next;

        // Free the current node
        // Say: "I free the current node, returning its memory to the heap"
        free(head);

        // Move to the next node using saved pointer
        // Say: "I move to the next node using the saved pointer"
        head = temp;
    }
    // Say: "All nodes have been freed, no memory leaks"
}

int main() {
    printf("=== Reverse Linked List ===\n\n");

    // Test iterative
    int arr1[] = {1, 2, 3, 4, 5};
    Node* list1 = createList(arr1, 5);

    printf("Original: ");
    printList(list1);

    list1 = reverseIterative(list1);
    printf("Reversed (iterative): ");
    printList(list1);

    // Test recursive
    int arr2[] = {10, 20, 30};
    Node* list2 = createList(arr2, 3);

    printf("\nOriginal: ");
    printList(list2);

    list2 = reverseRecursive(list2);
    printf("Reversed (recursive): ");
    printList(list2);

    // Single element
    int arr3[] = {42};
    Node* list3 = createList(arr3, 1);

    printf("\nSingle element: ");
    printList(list3);
    list3 = reverseIterative(list3);
    printf("Reversed: ");
    printList(list3);

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

    printf("\n=== Memory freed successfully ===\n");

    return 0;
}
