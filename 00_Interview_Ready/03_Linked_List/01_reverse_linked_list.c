/*
 * ============================================================================
 * PROBLEM: Reverse a Singly Linked List
 * ============================================================================
 *
 * WHAT IS A LINKED LIST?
 * A linked list is a linear data structure where elements (nodes) are stored
 * in non-contiguous memory locations. Each node contains:
 *   1. DATA: The actual value stored
 *   2. NEXT: A pointer to the next node in the sequence
 *
 * NODE STRUCTURE:
 *   +-------+-------+
 *   | data  | next  | ---> points to next node (or NULL)
 *   +-------+-------+
 *
 * In C, we define it as:
 *   typedef struct Node {
 *       int data;           // Value stored in node
 *       struct Node* next;  // Pointer to next node
 *   } Node;
 *
 * LINKED LIST vs ARRAY:
 *   - Array: Contiguous memory, O(1) access, O(n) insert/delete
 *   - Linked List: Scattered memory, O(n) access, O(1) insert/delete (if at position)
 *
 * WHAT IS THIS PROBLEM?
 * Given the head of a singly linked list, reverse the list so that the last
 * node becomes the first, second-to-last becomes second, and so on.
 * Return the new head (originally the last node).
 *
 * EXAMPLES:
 *   Input:  1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *   Output: 5 -> 4 -> 3 -> 2 -> 1 -> NULL
 *
 *   Input:  1 -> 2 -> NULL
 *   Output: 2 -> 1 -> NULL
 *
 *   Input:  NULL (empty list)
 *   Output: NULL
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 *   - Tests understanding of pointer manipulation
 *   - Foundation for many linked list problems (palindrome check, etc.)
 *   - Shows ability to handle edge cases (empty list, single node)
 *   - Classic problem that every engineer should know
 *   - Often asked as warm-up before harder linked list questions
 *
 * KEY CONCEPT: Three Pointer Technique
 *   Use three pointers to reverse links one by one:
 *   - prev: Points to the already-reversed portion (starts at NULL)
 *   - curr: Current node being processed (starts at head)
 *   - next: Temporarily saves the next node before we break the link
 *
 *   For each node: Save next, reverse link, move pointers forward
 *
 * VISUAL:
 *
 *   Initial State:
 *   NULL    1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *    ^      ^
 *   prev   curr
 *
 *   After Step 1: (reverse 1's link)
 *   NULL <- 1    2 -> 3 -> 4 -> 5 -> NULL
 *           ^    ^
 *          prev curr
 *
 *   After Step 2: (reverse 2's link)
 *   NULL <- 1 <- 2    3 -> 4 -> 5 -> NULL
 *                ^    ^
 *               prev curr
 *
 *   ... continue until curr is NULL ...
 *
 *   Final State:
 *   NULL <- 1 <- 2 <- 3 <- 4 <- 5
 *                               ^
 *                              prev (NEW HEAD!)
 *
 * ============================================================================
 */

/*
 * REVERSE LINKED LIST - Iterative Three Pointer Approach
 *
 * Time Complexity: O(n) - Single pass through the list
 * Space Complexity: O(1) - Only three pointers used
 *
 * Pattern: Three Pointer Technique (prev, curr, next)
 */

#include <stdio.h>
#include <stdlib.h>

/* ==================== HELPER FUNCTIONS ==================== */

/*
 * ============================================================================
 * NODE STRUCTURE - Line by Line Explanation
 * ============================================================================
 *
 * typedef struct Node {
 *     ^^^^^^
 *     typedef = "type definition" - creates an alias so we can write
 *               "Node" instead of "struct Node" everywhere
 *
 *     struct Node {
 *     ^^^^^^
 *     struct = keyword to define a composite data type that groups
 *              multiple variables together under one name
 *
 *     int data;
 *         ^^^^
 *         data = the actual VALUE stored in this node (the payload)
 *                In this case, an integer. Could be any type.
 *
 *     struct Node* next;
 *     ^^^^^^^^^^^^
 *     struct Node* = pointer TO another Node
 *                    This is what LINKS nodes together!
 *                    Points to the next node in the list (or NULL if last)
 *
 *     Why "struct Node*" and not just "Node*"?
 *     - Inside the struct definition, "Node" alias doesn't exist yet
 *     - The typedef completes AFTER the struct definition
 *     - So we must use "struct Node*" for the self-reference
 *
 * } Node;
 *   ^^^^
 *   Node = the alias name created by typedef
 *          Now we can use "Node" instead of "struct Node"
 *
 * MEMORY LAYOUT:
 * +--------+--------+
 * |  data  |  next  |
 * | (4 B)  | (8 B)  |  <- on 64-bit system, pointer is 8 bytes
 * +--------+--------+
 *
 * ============================================================================
 */
typedef struct Node {
    int data;               // The value stored in this node
    struct Node* next;      // Pointer to the next node (NULL if last)
} Node;

/*
 * ============================================================================
 * CREATE NODE FUNCTION - Line by Line Explanation
 * ============================================================================
 *
 * Node* create_node(int data) {
 * ^^^^^
 * Node* = return type is a POINTER to a Node
 *         We return an address, not the struct itself
 *
 *     Node* node = (Node*)malloc(sizeof(Node));
 *           ^^^^   ^^^^^^ ^^^^^^ ^^^^^^
 *           |      |      |      |
 *           |      |      |      sizeof(Node) = how many bytes a Node takes
 *           |      |      |                     (typically 12-16 bytes)
 *           |      |      |
 *           |      |      malloc() = allocates that many bytes on the HEAP
 *           |      |                 returns void* (generic pointer)
 *           |      |                 memory persists until free() is called
 *           |      |
 *           |      (Node*) = cast the void* to Node*
 *           |                tells compiler "treat this address as Node"
 *           |
 *           node = local variable storing the address malloc returned
 *
 *     Why malloc (heap) instead of local variable (stack)?
 *     - Stack: Node node; <- Dies when function returns! Can't return it.
 *     - Heap:  malloc()   <- Lives until free(). Can return pointer safely.
 *
 *     node->data = data;
 *     ^^^^
 *     node->data = equivalent to (*node).data
 *                  -> is the "arrow operator" for accessing members through pointer
 *                  node is a POINTER, so we use -> (not .)
 *
 *     node->next = NULL;
 *                  ^^^^
 *                  NULL = this node doesn't point to anything yet
 *                         It's a "dead end" until we link it
 *
 *     return node;
 *            ^^^^
 *            Returns the ADDRESS of the newly created node
 *            The actual data lives on heap, we just pass the address
 *
 * MEMORY VISUALIZATION:
 *
 *   Stack (function scope)     Heap (persists)
 *   +--------+                 +--------+--------+
 *   | node   | ------------->  |  data  |  NULL  |
 *   | 0x1234 |                 +--------+--------+
 *   +--------+                 Address: 0x1234
 *
 *   When function returns, 'node' variable dies (stack cleaned up)
 *   But the heap memory at 0x1234 survives!
 *   We return 0x1234 so caller can access the Node.
 *
 * ============================================================================
 */
Node* create_node(int data) {
    Node* node = (Node*)malloc(sizeof(Node));   // Allocate memory on heap
    node->data = data;                          // Set the value
    node->next = NULL;                          // Initialize next to NULL (no link yet)
    return node;                                // Return address of new node
}

/*
 * ============================================================================
 * PRINT LIST FUNCTION - Line by Line Explanation
 * ============================================================================
 *
 * void print_list(Node* head) {
 * ^^^^            ^^^^^
 * void = returns nothing, just prints
 * Node* head = we receive a POINTER to the first node
 *              "head" is the conventional name for the start of a list
 *
 *     printf("[");
 *     Start the visual representation with opening bracket
 *
 *     while (head) {
 *            ^^^^
 *     while (head) is shorthand for while (head != NULL)
 *     In C, NULL is 0, and 0 is "false" in boolean context
 *     Any non-NULL pointer is "true"
 *     Loop continues until we hit the end of the list (NULL)
 *
 *         printf("%d", head->data);
 *                      ^^^^^^^^^^
 *         head->data = access the 'data' field through the pointer
 *         Print the value stored in current node
 *
 *         if (head->next) printf(" -> ");
 *             ^^^^^^^^^^
 *         If there's a next node, print arrow separator
 *         Only print arrow if NOT the last node (cleaner output)
 *
 *         head = head->next;
 *         ^^^^^^^^^^^^^^^^^
 *         MOVE to the next node!
 *         head now points to whatever the current node's 'next' pointed to
 *         This is how we TRAVERSE a linked list
 *
 *         IMPORTANT: We're modifying the LOCAL COPY of head
 *         The original pointer in main() is unchanged
 *         (Parameters are passed by value in C)
 *
 *     printf("]\n");
 *     Close the bracket and newline
 *
 * TRAVERSAL VISUALIZATION:
 *
 *   Initial: head -> [1] -> [2] -> [3] -> NULL
 *                     ^
 *                   head points here
 *
 *   After head = head->next:
 *            head -> [1] -> [2] -> [3] -> NULL
 *                            ^
 *                          head points here now
 *
 *   After another head = head->next:
 *            head -> [1] -> [2] -> [3] -> NULL
 *                                   ^
 *                                 head points here
 *
 *   After another: head = NULL -> loop exits
 *
 * ============================================================================
 */
void print_list(Node* head) {
    printf("[");                            // Start output
    while (head) {                          // While not at end (head != NULL)
        printf("%d", head->data);           // Print current node's value
        if (head->next) printf(" -> ");     // Print arrow if more nodes follow
        head = head->next;                  // Move to next node
    }
    printf("]\n");                          // End output with newline
}

/* ==================== SOLUTION ==================== */

/*
 * ============================================================================
 * DRY-RUN DIAGRAM: Reverse Linked List 1 -> 2 -> 3 -> 4 -> 5
 * ============================================================================
 *
 * INITIAL STATE:
 * ==============
 *
 *   prev    curr                                    next
 *    |       |                                       |
 *    v       v                                       v
 *   NULL    [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 1: Process node 1
 * ----------------------------------------------------------------------------
 *
 *   Step 1: next = curr->next  (save next before we break the link)
 *
 *   prev    curr    next
 *    |       |       |
 *    v       v       v
 *   NULL    [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *
 *   Step 2: curr->next = prev  (reverse the link - 1 now points to NULL)
 *
 *   prev    curr    next
 *    |       |       |
 *    v       v       v
 *   NULL <--[1]     [2] --> [3] --> [4] --> [5] --> NULL
 *
 *   Step 3: prev = curr  (move prev to current position)
 *
 *           prev    next
 *            |       |
 *            v       v
 *   NULL <--[1]     [2] --> [3] --> [4] --> [5] --> NULL
 *
 *   Step 4: curr = next  (move curr to next position)
 *
 *           prev    curr
 *            |       |
 *            v       v
 *   NULL <--[1]     [2] --> [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 2: Process node 2
 * ----------------------------------------------------------------------------
 *
 *   Step 1: next = curr->next
 *
 *           prev    curr    next
 *            |       |       |
 *            v       v       v
 *   NULL <--[1]     [2] --> [3] --> [4] --> [5] --> NULL
 *
 *   Step 2: curr->next = prev  (2 now points to 1)
 *
 *           prev    curr    next
 *            |       |       |
 *            v       v       v
 *   NULL <--[1] <--[2]     [3] --> [4] --> [5] --> NULL
 *
 *   Step 3: prev = curr
 *   Step 4: curr = next
 *
 *                  prev    curr
 *                   |       |
 *                   v       v
 *   NULL <--[1] <--[2]     [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 3: Process node 3
 * ----------------------------------------------------------------------------
 *
 *   After all steps:
 *
 *                          prev    curr
 *                           |       |
 *                           v       v
 *   NULL <--[1] <--[2] <--[3]     [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 4: Process node 4
 * ----------------------------------------------------------------------------
 *
 *   After all steps:
 *
 *                                  prev    curr
 *                                   |       |
 *                                   v       v
 *   NULL <--[1] <--[2] <--[3] <--[4]     [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 5: Process node 5
 * ----------------------------------------------------------------------------
 *
 *   After all steps:
 *
 *                                          prev    curr
 *                                           |       |
 *                                           v       v
 *   NULL <--[1] <--[2] <--[3] <--[4] <--[5]       NULL
 *
 * ----------------------------------------------------------------------------
 * LOOP ENDS: curr == NULL
 * ----------------------------------------------------------------------------
 *
 *   FINAL STATE: prev points to new head (node 5)
 *
 *                                          prev
 *                                           |
 *                                           v
 *   NULL <--[1] <--[2] <--[3] <--[4] <--[5]
 *                                        ^
 *                                     NEW HEAD!
 *
 *   Return prev (which is 5)
 *
 *   Result: 5 -> 4 -> 3 -> 2 -> 1 -> NULL
 *
 * ============================================================================
 */

/*
 * ============================================================================
 * REVERSE LIST FUNCTION - Line by Line Explanation
 * ============================================================================
 */
Node* reverse_list(Node* head) {
    /*
     * Say: "I use three pointers: prev, curr, and next"
     *
     * WHY THREE POINTERS?
     * - prev: Keeps track of the already-reversed portion
     * - curr: The node we're currently processing
     * - next: Saves the next node BEFORE we break the link
     *         (If we reverse curr->next first, we lose access to the rest!)
     */

    Node* prev = NULL;
    /*    ^^^^^^^^^^^
     *    prev = NULL because the first node (after reversal) should point to NULL
     *           It will become the LAST node, and last nodes point to NULL
     *
     *    Visualization:
     *    prev
     *     |
     *     v
     *    NULL    [1] -> [2] -> [3] -> NULL
     */

    Node* curr = head;
    /*    ^^^^^^^^^^^
     *    curr = head means we start processing from the first node
     *
     *    Visualization:
     *    prev    curr
     *     |       |
     *     v       v
     *    NULL    [1] -> [2] -> [3] -> NULL
     */

    Node* next = NULL;
    /*    ^^^^^^^^^^^
     *    next = NULL for now, we'll set it inside the loop
     *    It's declared here to avoid declaring inside loop (style preference)
     */

    while (curr != NULL) {
        /*    ^^^^^^^^^^^^
         *    Continue until we've processed ALL nodes
         *    When curr becomes NULL, we've reached past the end
         */

        next = curr->next;
        /*    ^^^^^^^^^^^^^
         *    STEP 1: SAVE the next node before we break the link!
         *
         *    WHY? Because in the next step, we change curr->next to point
         *    backwards. If we don't save it, we lose access to the rest of the list!
         *
         *    Example at first iteration:
         *    prev    curr    next
         *     |       |       |
         *     v       v       v
         *    NULL    [1] --> [2] -> [3] -> NULL
         *
         *    next now holds address of [2]
         */

        curr->next = prev;
        /*    ^^^^^^^^^^^^^
         *    STEP 2: REVERSE the link!
         *
         *    Before: curr (node 1) points to node 2
         *    After:  curr (node 1) points to prev (NULL)
         *
         *    This is the actual reversal - we're changing where the arrow points!
         *
         *    prev    curr    next
         *     |       |       |
         *     v       v       v
         *    NULL <--[1]     [2] -> [3] -> NULL
         *         ^^^^
         *         Link reversed! Node 1 now points backwards
         */

        prev = curr;
        /*    ^^^^^^^^^
         *    STEP 3: Move prev forward to current position
         *
         *    prev is now at node 1 (the node we just processed)
         *    This becomes the "previous" node for the next iteration
         *
         *           prev    next
         *            |       |
         *            v       v
         *    NULL <--[1]     [2] -> [3] -> NULL
         */

        curr = next;
        /*    ^^^^^^^^^
         *    STEP 4: Move curr forward to the next unprocessed node
         *
         *    curr is now at node 2 (the node we saved earlier)
         *    Ready to process the next node in the next iteration
         *
         *           prev    curr
         *            |       |
         *            v       v
         *    NULL <--[1]     [2] -> [3] -> NULL
         *
         *    Loop continues with node 2...
         */
    }
    /*
     * When loop ends, curr = NULL (we've gone past the last node)
     * prev points to what WAS the last node (now the FIRST node of reversed list)
     *
     *                                      prev    curr
     *                                       |       |
     *                                       v       v
     *    NULL <-- [1] <-- [2] <-- [3]              NULL
     *                             ^^^
     *                            NEW HEAD!
     */

    return prev;
    /*     ^^^^
     *     Return prev because it points to the new head
     *     (The old last node, which is now the first node)
     *
     *     Original: 1 -> 2 -> 3 -> NULL
     *     Reversed: 3 -> 2 -> 1 -> NULL
     *               ^
     *              prev (returned as new head)
     */
}

/* ==================== TEST ==================== */

int main() {
    // Create list: 1 -> 2 -> 3 -> 4 -> 5
    Node* head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);
    head->next->next->next->next = create_node(5);

    printf("Original: ");
    print_list(head);

    head = reverse_list(head);

    printf("Reversed: ");
    print_list(head);

    // Test empty list
    Node* empty = NULL;
    empty = reverse_list(empty);
    printf("Empty reversed: ");
    print_list(empty);

    // Test single node
    Node* single = create_node(42);
    single = reverse_list(single);
    printf("Single reversed: ");
    print_list(single);

    return 0;
}

/*
 * ==================== INTERVIEW EXPLANATION ====================
 *
 * PROBLEM: Reverse a singly linked list in-place.
 *
 * APPROACH - Three Pointer Technique:
 *
 * Initial State:
 *   NULL    1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *    ^      ^
 *   prev   curr
 *
 * Step by Step:
 *   1. Save: next = curr->next (save 2)
 *   2. Reverse: curr->next = prev (1 points to NULL)
 *   3. Move: prev = curr (prev at 1)
 *   4. Move: curr = next (curr at 2)
 *
 * After all iterations:
 *   NULL <- 1 <- 2 <- 3 <- 4 <- 5
 *                               ^
 *                              prev (new head)
 *
 * WHY THREE POINTERS:
 * - prev: Tracks the already-reversed portion
 * - curr: Current node being processed
 * - next: Saves reference before we break the link
 *
 * EDGE CASES TO MENTION:
 * 1. Empty list (head = NULL) -> Return NULL
 * 2. Single node -> Returns same node (no change)
 * 3. Two nodes -> Simple swap
 *
 * COMMON MISTAKES:
 * - Forgetting to save next before reversing link (lose rest of list)
 * - Returning curr instead of prev (curr is NULL at end)
 * - Not handling empty list
 *
 * FOLLOW-UP: Recursive solution?
 * - Base case: head is NULL or single node
 * - Recurse to end, then reverse links on way back
 * - Same O(n) time but O(n) space for call stack
 *
 * INTERVIEW TIP: Draw the pointers and show movement visually!
 */
