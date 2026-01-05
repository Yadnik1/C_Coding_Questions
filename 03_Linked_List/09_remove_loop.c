/*
 * ============================================================================
 * PROBLEM: Remove Loop/Cycle from Linked List
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: HIGH
 *
 * Detect loop using Floyd's algorithm, then remove it.
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 * Step 1: Detect loop using slow/fast pointers
 * Step 2: Find loop start (reset slow to head, move both by 1)
 * Step 3: Find node before loop start (the one pointing into loop)
 * Step 4: Set that node's next to NULL
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   1 → 2 → 3 → 4 → 5
 *             ↑     ↓
 *             └─────┘
 *
 *   Loop start: node 3
 *   Node before loop start in the cycle: node 5
 *   Fix: set node 5's next = NULL
 *
 *   After: 1 → 2 → 3 → 4 → 5 → NULL
 *
 * ============================================================================
 * WHY FLOYD'S WORKS FOR FINDING LOOP START:
 * ============================================================================
 *
 *   Let:
 *   - Distance from head to loop start = x
 *   - Distance from loop start to meeting point = y
 *   - Loop length = L
 *
 *   At meeting point:
 *   - slow traveled: x + y
 *   - fast traveled: x + y + nL (n complete loops)
 *
 *   Since fast = 2 * slow:
 *   2(x + y) = x + y + nL
 *   x + y = nL
 *   x = nL - y = (n-1)L + (L - y)
 *
 *   L - y = distance from meeting point to loop start
 *   So moving x from head = moving (L-y) from meeting point
 *   Both reach loop start!
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why reset slow to head in phase 2?"
 * A1: Mathematical proof shows distances are equal:
 *
 *     Let x = distance head to loop start
 *     Let y = distance loop start to meeting point
 *     Let L = loop length
 *
 *     At meeting: slow = x+y, fast = x+y+nL
 *     Since fast = 2*slow: x+y+nL = 2(x+y) → x = nL-y
 *
 *     This means: x = (n-1)L + (L-y)
 *     Distance from head = distance from meeting point to loop start!
 *
 * -------------------------------------------------------------------------
 * Q2: "Why do we check slow->next != fast->next, not slow != fast?"
 * A2: We need to STOP one node BEFORE the loop start!
 *
 *     If we used slow != fast, we'd stop AT the loop start.
 *     But we need to set fast->next = NULL, so we need the node
 *     BEFORE the loop start (the one whose next points into the loop).
 *
 * -------------------------------------------------------------------------
 * Q3: "What's the special case when loop starts at head?"
 * A3: When slow == head initially, we can't use the normal algorithm:
 *
 *     Example: 1 → 2 → 3 → 1 (back to head)
 *
 *     In this case:
 *     - Find the node whose next is head (last node in loop)
 *     - Set that node's next = NULL
 *
 * -------------------------------------------------------------------------
 * Q4: "Can you use hash set to remove loop?"
 * A4: Yes, but O(n) space:
 *
 *     1. Traverse list, store each node in hash set
 *     2. Before storing, check if next is already in set
 *     3. If yes, next is loop start; set current->next = NULL
 *
 *     Floyd's is preferred for embedded (O(1) space).
 *
 * -------------------------------------------------------------------------
 * Q5: "How do you handle the edge case of a single-node loop?"
 * A5: Single node pointing to itself: 1 → 1 (back to itself)
 *
 *     Detection works: slow and fast both stay at node 1
 *     Removal: Find node whose next is head, set to NULL
 *     Result: 1 → NULL
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
 * LINE: typedef struct Node {
 * ---------------------------------
 * - "typedef" = We're creating a type alias (shorthand name)
 * - "struct Node" = Defining a structure with tag name "Node"
 * - "{" opens the structure body
 * - WHY "struct Node"? We need the tag to reference it inside itself
 *
 * LINE: int data;
 * ---------------------------------
 * - This is the PAYLOAD - the actual value stored in this node
 * - "int" means we store integers (could be char, float, or any type)
 * - Each node holds ONE piece of data
 * - This is what makes the list useful - it stores our values!
 *
 * LINE: struct Node* next;
 * ---------------------------------
 * - This is the LINK - pointer to the next node in the chain
 * - "struct Node*" = pointer to another Node structure
 * - WHY "struct Node*" and not "Node*"?
 *   Because at this point in the code, the typedef "Node" isn't
 *   complete yet! The compiler doesn't know "Node" exists yet.
 * - This pointer is what makes it "LINKED" - nodes point to each other
 * - For the LAST node: next = NULL (marks end of list)
 *
 * LINE: } Node;
 * ---------------------------------
 * - "}" closes the structure definition
 * - "Node" after the brace completes the typedef
 * - NOW we can use "Node" as a type alias for "struct Node"
 * - Example: Node* ptr; instead of struct Node* ptr;
 *
 * MEMORY LAYOUT (64-bit system):
 * ---------------------------------
 *   +------------------+------------------+
 *   |      data        |      next        |
 *   |    (4 bytes)     |   (8 bytes)      |
 *   +------------------+------------------+
 *   Total: 12-16 bytes (may have padding for alignment)
 *
 * WHY POINTERS AND NOT ACTUAL NODES?
 * ---------------------------------
 * - If "next" was "struct Node next" (not a pointer), each node would
 *   contain another complete node, which contains another, infinitely!
 * - Pointers have FIXED size (8 bytes on 64-bit) regardless of what
 *   they point to
 * - This allows dynamic memory allocation and flexible list sizes
 *
 * ============================================================================
 */
typedef struct Node {
    // The data field stores the actual integer value (the payload)
    // Say: "Each node has a data field that stores the integer value"
    // WHY int? We're storing integers. Could be any type you need.
    int data;

    // The next pointer stores the memory address of the next node
    // Say: "The next pointer points to the next node, or NULL if last"
    // WHY pointer? So nodes can be anywhere in memory, connected by addresses
    // WHY struct Node*? typedef "Node" isn't complete at this point
    struct Node* next;
} Node;  // Now "Node" is an alias for "struct Node"

/*
 * ============================================================================
 * CREATE NODE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* createNode(int data) {
 *     Node* node = (Node*)malloc(sizeof(Node));
 *     node->data = data;
 *     node->next = NULL;
 *     return node;
 * }
 *
 * LINE: Node* createNode(int data)
 * ---------------------------------
 * - "Node*" = Return type is a pointer to a Node
 * - "createNode" = Function name (describes what it does)
 * - "int data" = Parameter: the value to store in the new node
 * - This is a FACTORY FUNCTION - it creates and initializes new nodes
 *
 * LINE: Node* node = (Node*)malloc(sizeof(Node));
 * ---------------------------------
 * - "Node* node" = Declare a pointer variable to hold the new node's address
 * - "malloc(sizeof(Node))" = Allocate memory on the HEAP
 *   - sizeof(Node) = Calculates exact bytes needed for one Node (12-16 bytes)
 *   - malloc = Memory ALLOCation function from stdlib.h
 *   - Returns void* (generic pointer) to the allocated memory
 *   - Returns NULL if allocation fails (out of memory)
 * - "(Node*)" = Cast the void* to Node* (required in C++ for type safety)
 *
 * WHY HEAP (malloc) and not STACK?
 * - STACK: Local variables live here, destroyed when function returns
 * - HEAP: malloc'd memory lives here, persists until we call free()
 * - Linked list nodes MUST outlive the function that creates them!
 * - If we did "Node node;" (stack), it would vanish when createNode returns
 *
 * LINE: node->data = data;
 * ---------------------------------
 * - "node->data" = Access the 'data' field of the node that 'node' points to
 * - "->" = Arrow operator, shorthand for (*node).data
 *   - Dereference the pointer (*node) and access the member (.data)
 * - "= data" = Copy the parameter value into the node's data field
 * - This stores the actual value we want in our new node
 *
 * LINE: node->next = NULL;
 * ---------------------------------
 * - "node->next" = Access the 'next' pointer field
 * - "= NULL" = Set it to NULL (meaning "points to nothing")
 * - WHY initialize to NULL?
 *   - Uninitialized pointers contain GARBAGE (random memory address)
 *   - Dereferencing garbage = CRASH (segmentation fault)
 *   - NULL is a safe "nothing" value we can check for
 * - The new node is an "orphan" - not connected to any list yet
 * - Caller will set 'next' appropriately when adding to a list
 *
 * LINE: return node;
 * ---------------------------------
 * - Return the pointer to our newly created node
 * - Caller receives the ADDRESS of the node on the heap
 * - Caller is now responsible for:
 *   1. Using this node (adding to a list)
 *   2. Eventually calling free(node) to prevent memory leaks
 *
 * MEMORY VISUALIZATION:
 * ---------------------------------
 *   STACK (createNode)       HEAP
 *   +------------+           +------+------+
 *   | data = 5   |           |  5   | NULL |
 *   +------------+           +------+------+
 *   | node = ----+---------->| 0x1000      |
 *   +------------+           +------+------+
 *
 *   After return:
 *   - Stack frame gone
 *   - Heap node remains at 0x1000
 *   - Caller has pointer to 0x1000
 *
 * ============================================================================
 */
Node* createNode(int data) {
    // Allocate memory on HEAP for one Node structure
    // Say: "First, I allocate memory for a new node using malloc"
    // WHY malloc? Stack memory dies when function returns; heap persists
    // WHY sizeof(Node)? Calculates exact bytes needed for one Node
    // WHY (Node*)? Cast void* returned by malloc to Node* for type safety
    Node* node = (Node*)malloc(sizeof(Node));

    // OPTIONAL: Check if malloc succeeded (production code should do this!)
    // if (node == NULL) {
    //     printf("Memory allocation failed!\n");
    //     return NULL;
    // }

    // Store the data value in the node's data field
    // Say: "I store the data value in the node's data field"
    // WHY ->? Arrow operator accesses struct member through pointer
    // Equivalent to: (*node).data = data;
    node->data = data;

    // Initialize next pointer to NULL (not connected to any other node yet)
    // Say: "I set next to NULL since this node isn't linked to anything yet"
    // WHY NULL? Uninitialized pointers are DANGEROUS (random garbage)
    // NULL is a safe "nothing" value; we can check if (ptr == NULL)
    node->next = NULL;

    // Return pointer to the new node (caller now owns this memory!)
    // Say: "I return the pointer to the newly created node"
    // WHY return pointer? Caller needs the address to use the node
    // CALLER'S RESPONSIBILITY: Eventually free() this memory!
    return node;
}

/*
 * ============================================================================
 * REMOVE LOOP FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * ALGORITHM OVERVIEW:
 * ---------------------------------
 * Phase 1: DETECT if a loop exists (Floyd's Cycle Detection)
 *   - Use slow (1 step) and fast (2 steps) pointers
 *   - If they meet, loop exists
 *   - If fast reaches NULL, no loop
 *
 * Phase 2: FIND the loop start
 *   - Reset slow to head
 *   - Move BOTH pointers by 1 step
 *   - They meet at loop start (mathematical proof in header)
 *
 * Phase 3: REMOVE the loop
 *   - Find the node whose next points to loop start
 *   - Set that node's next to NULL
 *
 * WHY slow->next != fast->next instead of slow != fast?
 * ---------------------------------
 * We need to find the node BEFORE the loop start so we can set its next to NULL.
 * If we used slow != fast, we'd stop AT the loop start, but we need the node
 * that POINTS to the loop start. So we check if their NEXT pointers are equal.
 *
 * ============================================================================
 */
// Detect and remove loop from the linked list
// Say: "I'll detect the loop using Floyd's algorithm, then remove it"
void removeLoop(Node* head) {
    // Edge case: empty list or single node without a loop
    // Say: "First, I check if the list is empty or has only one node"
    // WHY? Empty list (NULL) or single node with next=NULL can't have a loop
    // Single node pointing to itself IS a loop - handled by the algorithm
    if (head == NULL || head->next == NULL) return;

    // Initialize slow and fast pointers at the head
    // Say: "I initialize slow and fast pointers, both starting at head"
    // WHY two pointers? Floyd's algorithm uses speed difference to detect loops
    Node* slow = head;  // Moves 1 step at a time (tortoise)
    Node* fast = head;  // Moves 2 steps at a time (hare)

    // =========================================================================
    // PHASE 1: DETECT LOOP using Floyd's Cycle Detection
    // =========================================================================
    // Say: "Phase 1: I'll move slow by 1 and fast by 2 to detect a loop"
    // WHY does this work? If there's a loop, fast will eventually lap slow
    // Think of two runners on a circular track - faster one will catch up
    while (fast != NULL && fast->next != NULL) {
        // Move slow pointer one step forward
        // Say: "Move slow one step forward"
        // WHY just one? Slow is the "tortoise" - moves cautiously
        slow = slow->next;

        // Move fast pointer two steps forward
        // Say: "Move fast two steps forward"
        // WHY two? Fast is the "hare" - moves twice as fast
        // WHY fast->next->next? We need to check fast->next isn't NULL first
        fast = fast->next->next;

        // Check if slow and fast have met (same memory address)
        // Say: "If slow and fast point to the same node, we've detected a loop"
        // WHY same address? In a loop, fast will eventually catch up to slow
        // WHY break? We found a loop, no need to continue detection
        if (slow == fast) {
            break;  // Loop detected! Exit the while loop
        }
    }

    // Check if we exited because there's NO loop (fast reached the end)
    // Say: "If fast reached NULL, there's no loop - nothing to remove"
    // WHY these conditions? If fast or fast->next is NULL, list ended = no loop
    if (fast == NULL || fast->next == NULL) {
        return;  // No loop exists, nothing to do
    }

    // =========================================================================
    // SPECIAL CASE: Loop starts at head (e.g., 1 → 2 → 3 → 1)
    // =========================================================================
    // Say: "I check the special case where the loop starts at the head"
    // WHY special? The general algorithm fails when loop starts at head
    // In this case, after detection, slow might already equal head
    if (slow == head) {
        // Find the last node in the loop (the one pointing back to head)
        // Say: "I find the node whose next points back to the head"
        // WHY? We need to break the link that creates the loop
        while (fast->next != slow) {
            fast = fast->next;  // Keep moving until we find the node before head
        }

        // Break the loop by setting the last node's next to NULL
        // Say: "I break the loop by setting that node's next to NULL"
        // WHY NULL? This makes it the proper last node of the list
        fast->next = NULL;
        return;
    }

    // =========================================================================
    // PHASE 2: FIND LOOP START (general case)
    // =========================================================================
    // Reset slow to head
    // Say: "Phase 2: I reset slow to head to find where the loop starts"
    // WHY reset? Mathematical proof: distance from head to loop start equals
    //            distance from meeting point to loop start (see header comments)
    slow = head;

    // Move both pointers at SAME speed (1 step) until their NEXT pointers match
    // Say: "I move both pointers by 1 until they're about to enter the loop"
    // WHY slow->next != fast->next? We need the node BEFORE the loop start
    //    so we can set its next to NULL. Checking ->next stops us one node early.
    // WHY same speed now? They'll meet at the loop start (math proof in header)
    while (slow->next != fast->next) {
        // Move slow forward one step
        // Say: "Move slow forward one step"
        slow = slow->next;

        // Move fast forward one step (same speed as slow now!)
        // Say: "Move fast forward one step too"
        // WHY same speed? At this phase, we're finding WHERE they meet, not IF
        fast = fast->next;
    }

    // =========================================================================
    // PHASE 3: REMOVE THE LOOP
    // =========================================================================
    // Now fast->next points to the loop start
    // Break the loop by setting fast->next to NULL
    // Say: "Phase 3: I break the loop by setting the last node's next to NULL"
    // WHY fast->next = NULL? fast is now the last node before the loop start
    //    Setting its next to NULL properly terminates the list
    fast->next = NULL;
}

/*
 * ============================================================================
 * HAS LOOP FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * This is a simplified version that only DETECTS if a loop exists.
 * It doesn't find where the loop starts or remove it.
 *
 * ALGORITHM: Floyd's Cycle Detection (Tortoise and Hare)
 * - slow moves 1 step, fast moves 2 steps
 * - If they ever meet (same address), there's a loop
 * - If fast reaches NULL, no loop
 *
 * RETURN VALUES:
 * - 1 (true): Loop exists
 * - 0 (false): No loop
 *
 * ============================================================================
 */
// Detect if a loop exists in the linked list
// Say: "I'll use Floyd's algorithm to detect if a loop exists"
int hasLoop(Node* head) {
    // Initialize both pointers at head
    // Say: "I initialize slow and fast pointers at the head"
    // WHY both at head? We start both runners at the same position
    Node* slow = head;  // Tortoise: moves 1 step at a time
    Node* fast = head;  // Hare: moves 2 steps at a time

    // Traverse the list with two different speeds
    // Say: "I traverse moving slow by 1 and fast by 2"
    // WHY fast != NULL && fast->next != NULL?
    //   - fast != NULL: fast hasn't fallen off the list
    //   - fast->next != NULL: there's a node for fast to jump to
    //   - Both conditions prevent segmentation fault
    while (fast != NULL && fast->next != NULL) {
        // Move slow one step forward
        // Say: "Move slow one step"
        // WHY just one? Slow is the "tortoise" - moves cautiously
        slow = slow->next;

        // Move fast two steps forward
        // Say: "Move fast two steps"
        // WHY two? Fast is the "hare" - moves twice as fast
        // We already checked fast->next != NULL, so this is safe
        fast = fast->next->next;

        // Check if slow and fast point to the same node (meeting point)
        // Say: "If they meet at the same node, a loop exists"
        // WHY same address means loop?
        //   - In a linear list, fast reaches NULL before catching slow
        //   - In a loop, fast keeps going and eventually laps slow
        //   - Like a faster runner on a circular track catching a slower one
        if (slow == fast) {
            return 1;  // 1 = true, loop exists
        }
    }

    // Fast reached the end (NULL), so no loop exists
    // Say: "If fast reaches NULL, the list ends - no loop"
    // WHY does reaching NULL mean no loop?
    //   - A loop means you can never reach NULL
    //   - If you reach NULL, the list has a proper end
    return 0;  // 0 = false, no loop
}

/*
 * ============================================================================
 * PRINT LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * WARNING: This version has a maxNodes limit to prevent infinite loops!
 * If the list has a loop, a normal printList would run forever.
 *
 * ============================================================================
 */
// Print the linked list (with safety limit for loops)
// Say: "I'll print the list with a safety limit to avoid infinite loops"
void printList(Node* head, int maxNodes) {
    // Create a traversal pointer starting at head
    // Say: "I use a current pointer to traverse without losing the head"
    // WHY not use head directly? We'd lose the reference to the start
    Node* current = head;

    // Count how many nodes we've printed
    // Say: "I count nodes to enforce the maxNodes safety limit"
    // WHY count? If there's a loop, we'd print forever without a limit
    int count = 0;

    printf("List: ");

    // Traverse and print until NULL or maxNodes reached
    // Say: "I loop through each node until NULL or hitting the limit"
    // WHY two conditions?
    //   - current != NULL: Normal list end
    //   - count < maxNodes: Safety limit for loops
    while (current != NULL && count < maxNodes) {
        // Print the current node's data value
        // Say: "Print the data value of the current node"
        printf("%d", current->data);

        // Move to the next node
        // Say: "Move current to the next node"
        current = current->next;

        // Increment the count of printed nodes
        count++;

        // Print arrow separator if more nodes follow
        // Say: "Print an arrow if there's another node to print"
        if (current != NULL && count < maxNodes) {
            printf(" → ");
        }
    }

    // Check if we stopped due to hitting the limit (possible loop)
    // Say: "If we hit the limit but list continues, show truncation"
    if (count >= maxNodes && current != NULL) {
        printf(" → ... (truncated)");
    } else {
        // Normal end - show NULL
        // Say: "Show NULL to indicate proper list end"
        printf(" → NULL");
    }
    printf("\n");
}

/*
 * ============================================================================
 * FREE LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void freeList(Node* head):
 *   - Properly deallocates all memory used by the linked list
 *   - MUST be called when done with a list to prevent memory leaks
 *
 * WHY IS THIS IMPORTANT?
 * ---------------------------------
 * - Every malloc() MUST have a corresponding free()
 * - Without this, memory is "leaked" - allocated but never returned
 * - In long-running programs, leaks accumulate and crash the program
 * - Embedded systems with limited RAM are especially vulnerable
 *
 * WHY CAN'T WE JUST free(head)?
 * ---------------------------------
 * - free(head) only frees the first node!
 * - All other nodes remain allocated but inaccessible (leaked)
 * - We must traverse the list and free each node individually
 *
 * WHY DO WE NEED A TEMP POINTER?
 * ---------------------------------
 * - After free(current), current's memory is invalid
 * - We can't do current = current->next after freeing
 * - So we save 'next' in temp BEFORE freeing current
 *
 * VISUALIZATION:
 * ---------------------------------
 *   Before freeList:
 *   head → [1|*]→[2|*]→[3|NULL]   (all nodes on heap)
 *
 *   After freeList:
 *   head → ???   (memory returned to system, pointers are dangling)
 *
 * INTERVIEW TIP:
 * ---------------------------------
 * Say: "In production code, I would always free the list when done
 *       to prevent memory leaks. For embedded systems, this is critical."
 *
 * ============================================================================
 */
// Free all nodes in the linked list
// Say: "I'll free each node to prevent memory leaks"
void freeList(Node* head) {
    // Create a pointer to traverse the list
    // Say: "I use current to traverse and temp to save the next node"
    Node* current = head;
    Node* temp;

    // Traverse and free each node
    // Say: "I loop through each node, saving next before freeing"
    // WHY check current != NULL? When current is NULL, we've freed everything
    while (current != NULL) {
        // Save the next node BEFORE freeing current
        // Say: "Save the next pointer before I free this node"
        // WHY save first? After free(), current's memory is invalid
        // We can't access current->next after free(current)!
        temp = current->next;

        // Free the current node's memory
        // Say: "Free the current node, returning memory to the heap"
        // WHY free? Every malloc must have a free, or memory leaks
        free(current);

        // Move to the next node (using our saved pointer)
        // Say: "Move to the next node using the saved pointer"
        // WHY use temp? current is now invalid (freed memory)
        current = temp;
    }

    // After this function:
    // - All nodes have been freed
    // - The caller's head pointer is now a DANGLING POINTER
    // - Caller should set head = NULL after calling freeList
    // Say: "All nodes freed. Caller should set head = NULL now."
}

int main() {
    printf("=== Remove Loop from Linked List ===\n\n");

    // Create list: 1 → 2 → 3 → 4 → 5
    Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);

    // Create loop: 5 → 3
    head->next->next->next->next->next = head->next->next;

    printf("Before (with loop):\n");
    printf("1 → 2 → 3 → 4 → 5\n");
    printf("        ↑       ↓\n");
    printf("        └───────┘\n\n");

    printf("Has loop: %s\n\n", hasLoop(head) ? "YES" : "NO");

    // Remove loop
    removeLoop(head);

    printf("After removing loop:\n");
    printList(head, 10);
    printf("Has loop: %s\n\n", hasLoop(head) ? "YES" : "NO");

    // FREE MEMORY - Critical for preventing memory leaks!
    // Say: "I free all nodes to prevent memory leaks"
    // WHY? Every malloc() must have a corresponding free()
    // In production code, always free memory when done with a data structure
    freeList(head);
    head = NULL;  // Avoid dangling pointer

    printf("=== Algorithm ===\n");
    printf("1. Detect loop with Floyd's (slow/fast)\n");
    printf("2. Reset slow to head\n");
    printf("3. Move both by 1 until slow->next == fast->next\n");
    printf("4. Set fast->next = NULL\n");

    printf("\n=== Memory Management ===\n");
    printf("- Every malloc() needs a free()\n");
    printf("- Save next pointer BEFORE freeing current node\n");
    printf("- Set head = NULL after freeing to avoid dangling pointer\n");

    return 0;
}
