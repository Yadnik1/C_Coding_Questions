/*
 * ============================================================================
 * PROBLEM: Detect Loop in Linked List
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: VERY HIGH
 *
 * Detect if a linked list has a cycle (loop).
 *
 * Example:
 * 1 -> 2 -> 3 -> 4 -> 5
 *           ^         |
 *           +---------+
 * Output: Loop detected at node 3
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. FLOYD'S CYCLE DETECTION (Tortoise & Hare):
 *    - Slow pointer moves 1 step
 *    - Fast pointer moves 2 steps
 *    - If loop exists, they WILL meet
 *
 * 2. WHY IT WORKS:
 *    - In a loop, fast gains 1 step per iteration
 *    - Eventually catches up to slow
 *    - Like runners on circular track
 *
 * 3. NO EXTRA SPACE:
 *    - O(1) space vs O(n) for hash set
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * List with loop:
 *
 *   1 -> 2 -> 3 -> 4 -> 5
 *             ^         |
 *             +---------+
 *
 * Initial:
 *   slow = 1, fast = 1
 *
 * Step 1:
 *   slow = 2 (move 1)
 *   fast = 3 (move 2)
 *
 *   1 -> 2 -> 3 -> 4 -> 5
 *        S    F         |
 *             ^---------|
 *
 * Step 2:
 *   slow = 3 (move 1)
 *   fast = 5 (move 2)
 *
 *   1 -> 2 -> 3 -> 4 -> 5
 *             S         F
 *             ^---------|
 *
 * Step 3:
 *   slow = 4 (move 1)
 *   fast = 4 (move 2, 5->3->4)
 *
 *   slow == fast! LOOP DETECTED!
 *
 * ============================================================================
 * WHY THEY MUST MEET:
 * ============================================================================
 *
 * Think of circular track:
 *
 *       +---+---+---+
 *       |           |
 *       3 <- 4 <- 5 |
 *       |           |
 *       +-----------+
 *
 * Fast gains 1 position per step:
 *   - If slow at position 0
 *   - Fast at position 2
 *   - Gap = 2
 *
 * Next iteration:
 *   - Slow at 1, Fast at 4
 *   - Gap = 3
 *
 * Eventually gap = loop_length, meaning same position!
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   slow = head
 *   fast = head
 *
 *   while (fast != NULL AND fast->next != NULL):
 *       slow = slow->next          // Move 1 step
 *       fast = fast->next->next    // Move 2 steps
 *
 *       if (slow == fast):
 *           return true  // Loop found!
 *
 *   return false  // No loop (reached end)
 *
 * ============================================================================
 * FINDING LOOP START (Bonus):
 * ============================================================================
 *
 * After detecting loop:
 * 1. Reset slow to head
 * 2. Move both slow and fast by 1 step
 * 3. They meet at loop start!
 *
 * Math proof: Distance from head to loop start =
 *             Distance from meeting point to loop start
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Fast pointer covers list at most twice
 * - O(n) iterations
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only two pointers
 * - No extra data structure
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why does fast move 2 steps? Would 3 steps work?"
 * A1: Yes, any speed > 1 works! But 2 is optimal.
 *
 *     Why 2 is best:
 *     - Minimizes iterations (fast catches slow quickly)
 *     - Simple implementation (just fast->next->next)
 *     - Guaranteed to meet within one loop traversal
 *
 *     With speed 3: Fast might "jump over" slow if loop is small,
 *     requiring more iterations to catch up.
 *
 * -------------------------------------------------------------------------
 * Q2: "How do you FIND where the loop starts, not just detect it?"
 * A2: After detection (slow == fast), reset slow to head.
 *     Move BOTH one step at a time. They meet at loop start!
 *
 *     Mathematical proof:
 *     Let:
 *       - x = distance from head to loop start
 *       - y = distance from loop start to meeting point
 *       - c = loop length
 *
 *     When they meet:
 *       slow traveled: x + y
 *       fast traveled: x + y + n*c (some complete loops)
 *
 *     Since fast = 2*slow:
 *       x + y + n*c = 2(x + y)
 *       n*c = x + y
 *       x = n*c - y
 *
 *     This means: distance from head = distance from meeting point
 *     (going around the loop). They meet at loop start!
 *
 * -------------------------------------------------------------------------
 * Q3: "Can you detect loop using hash set? Trade-offs?"
 * A3: Yes! Store visited nodes, check if current node already seen.
 *
 *     Hash Set approach:
 *     - Time: O(n) - one pass
 *     - Space: O(n) - store all nodes
 *
 *     Floyd's approach:
 *     - Time: O(n) - same
 *     - Space: O(1) - constant!
 *
 *     Floyd's is better for memory-constrained embedded systems.
 *
 * -------------------------------------------------------------------------
 * Q4: "What if the loop starts at the head (entire list is a loop)?"
 * A4: Floyd's algorithm still works!
 *
 *     Example: 1 -> 2 -> 3 -> 1 (back to head)
 *
 *     The pointers will still meet inside the loop.
 *     When resetting slow to head, slow == fast already,
 *     so loop start is head.
 *
 * -------------------------------------------------------------------------
 * Q5: "How would you REMOVE the loop once detected?"
 * A5: Find the node whose next points to loop start, set it to NULL.
 *
 *     1. Find loop start (using Phase 2 above)
 *     2. Traverse loop to find the node pointing back to start
 *     3. Set that node's next = NULL
 *
 *     Code:
 *     Node* curr = loopStart;
 *     while (curr->next != loopStart) {
 *         curr = curr->next;
 *     }
 *     curr->next = NULL;  // Break the loop
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
 * HAS LOOP FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * bool hasLoop(Node* head):
 *   - Returns "bool" = true if loop exists, false otherwise
 *   - "Node* head" = pointer to the first node of the list
 *   - Purpose: Detect if the linked list contains a cycle
 *
 * ALGORITHM: Floyd's Cycle Detection (Tortoise and Hare)
 * ---------------------------------
 * Two pointers move at different speeds:
 *   - Slow (tortoise): moves 1 step per iteration
 *   - Fast (hare): moves 2 steps per iteration
 *
 * If there's a loop, fast will eventually catch up to slow (they'll meet)
 * If there's no loop, fast will reach NULL (end of list)
 *
 * WHY THIS WORKS - INTUITION:
 * ---------------------------------
 * Think of two runners on a circular track:
 *   - Slow runner runs at speed 1
 *   - Fast runner runs at speed 2
 *   - Fast gains 1 position per unit time
 *   - Fast WILL eventually lap slow!
 *
 * In a linked list with a loop:
 *   - Once both pointers enter the loop
 *   - Fast gains 1 position per iteration
 *   - Eventually fast catches up to slow
 *
 * WHY FAST MOVES 2 STEPS (not 3 or more)?
 * ---------------------------------
 * - Speed of 2 guarantees meeting within one loop traversal
 * - Higher speeds might "skip over" slow (take more iterations)
 * - 2 steps is optimal for both correctness and efficiency
 *
 * TIME COMPLEXITY: O(n)
 * ---------------------------------
 * - If no loop: fast reaches end in n/2 iterations
 * - If loop: fast catches slow within loop_length iterations
 * - Both cases are O(n)
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 * - Only two pointer variables
 * - No extra data structures needed
 * - Much better than hash set approach (O(n) space)
 *
 * ============================================================================
 */
// Detect if loop exists using Floyd's Cycle Detection
// Say: "I'll use Floyd's algorithm with two pointers moving at different speeds"
bool hasLoop(Node* head) {
    // =========================================================================
    // EDGE CASE: Empty list cannot have a loop
    // =========================================================================
    // Say: "First, I handle the edge case of an empty list"
    // WHY check NULL? Can't traverse an empty list, no loop possible
    if (head == NULL) return false;

    // =========================================================================
    // INITIALIZE: Both pointers start at the head
    // =========================================================================
    // Say: "I initialize slow and fast pointers, both starting at head"
    // WHY both at head? We start both "runners" at the same starting line
    Node* slow = head;  // Tortoise: moves 1 step at a time
    Node* fast = head;  // Hare: moves 2 steps at a time

    // =========================================================================
    // TRAVERSE: Move pointers until they meet or fast reaches end
    // =========================================================================
    // Say: "I traverse moving slow by 1 and fast by 2 until they meet or reach end"
    // WHY fast != NULL? Ensures fast hasn't fallen off the end
    // WHY fast->next != NULL? Ensures we can safely do fast->next->next
    while (fast != NULL && fast->next != NULL) {
        // Move slow pointer one step forward (tortoise speed)
        // Say: "I move slow one step forward"
        // WHY just one? Slow is the "tortoise" - moves cautiously
        slow = slow->next;

        // Move fast pointer two steps forward (hare speed)
        // Say: "I move fast two steps forward"
        // WHY two? Fast is the "hare" - moves twice as fast
        // This is safe because we checked fast->next != NULL above
        fast = fast->next->next;

        // =====================================================================
        // CHECK: Did the pointers meet? (same memory address)
        // =====================================================================
        // Say: "If slow and fast point to the same node, we found a loop"
        // WHY same address means loop?
        //   - In a linear list, fast reaches NULL before catching slow
        //   - In a loop, fast keeps going and eventually laps slow
        //   - Like a faster runner on a circular track catching a slower one
        if (slow == fast) {
            return true;  // Loop detected! Both pointers at same node
        }
    }

    // =========================================================================
    // NO LOOP: Fast reached the end (NULL) without meeting slow
    // =========================================================================
    // Say: "Fast reached NULL, which means the list has an end - no loop"
    // WHY does reaching NULL mean no loop?
    //   - A loop means you can never reach NULL
    //   - If you reach NULL, the list has a proper termination
    return false;
}

/*
 * ============================================================================
 * DETECT LOOP START FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* detectLoopStart(Node* head):
 *   - Returns "Node*" = pointer to the node where loop starts, or NULL if no loop
 *   - "Node* head" = pointer to the first node of the list
 *   - Purpose: Find WHERE the loop begins (not just IF it exists)
 *
 * ALGORITHM: Two-Phase Approach
 * ---------------------------------
 * Phase 1: Detect if loop exists (Floyd's algorithm)
 *   - Use slow/fast pointers to find meeting point inside loop
 *
 * Phase 2: Find loop start
 *   - Reset slow to head
 *   - Move BOTH pointers by 1 step until they meet
 *   - They will meet at the loop start!
 *
 * WHY PHASE 2 WORKS - MATHEMATICAL PROOF:
 * ---------------------------------
 * Let:
 *   - x = distance from head to loop start
 *   - y = distance from loop start to meeting point
 *   - L = total loop length
 *
 * When slow and fast meet (Phase 1):
 *   - slow traveled: x + y (entered loop, went y steps)
 *   - fast traveled: x + y + nL (n complete loops, n >= 1)
 *
 * Since fast moves twice as fast:
 *   2(x + y) = x + y + nL
 *   x + y = nL
 *   x = nL - y = (n-1)L + (L - y)
 *
 * Key insight: x = (n-1)L + (L - y)
 *   - (L - y) is the distance from meeting point to loop start!
 *   - (n-1)L is some complete laps around the loop
 *
 * So: distance from head to loop start (x) equals
 *     distance from meeting point to loop start (L - y) + some full loops
 *
 * When both pointers move at same speed:
 *   - One starts at head, travels x steps to reach loop start
 *   - One starts at meeting point, travels (L-y) + (n-1)L = x steps
 *   - They MEET at loop start!
 *
 * VISUALIZATION:
 * ---------------------------------
 *
 *   Head --x steps--> [Loop Start] --y steps--> [Meeting Point]
 *                          |                          |
 *                          +---- (L-y) steps ---------+
 *
 *   After Phase 2:
 *   - Pointer from head travels x steps
 *   - Pointer from meeting travels (L-y) + complete loops = x steps
 *   - Both arrive at Loop Start simultaneously!
 *
 * ============================================================================
 */
// Detect loop and return the node where loop starts
// Say: "I'll find where the loop starts using a two-phase approach"
Node* detectLoopStart(Node* head) {
    // =========================================================================
    // EDGE CASE: Empty list cannot have a loop
    // =========================================================================
    // Say: "First, handle the edge case of an empty list"
    // WHY check NULL? No nodes means no loop possible
    if (head == NULL) return NULL;

    // =========================================================================
    // PHASE 1 SETUP: Initialize pointers for loop detection
    // =========================================================================
    // Say: "Phase 1: I'll first detect if a loop exists"
    // WHY two pointers? Floyd's algorithm requires slow and fast
    Node* slow = head;  // Tortoise: moves 1 step
    Node* fast = head;  // Hare: moves 2 steps

    // Flag to track if we found a loop (needed to know if Phase 2 is required)
    // Say: "I use a flag to remember if we detected a loop"
    // WHY flag? We need to know whether to proceed to Phase 2
    bool loopExists = false;

    // =========================================================================
    // PHASE 1: Detect if loop exists using Floyd's algorithm
    // =========================================================================
    // Say: "In Phase 1, I detect if there's a loop"
    // WHY same as hasLoop()? We need to find the meeting point for Phase 2
    while (fast != NULL && fast->next != NULL) {
        // Move slow one step, fast two steps
        // Say: "I move slow by 1 and fast by 2"
        slow = slow->next;
        fast = fast->next->next;

        // Check if they met (same memory address)
        // Say: "When they meet, I know a loop exists"
        // WHY break? We found what we need for Phase 2
        if (slow == fast) {
            loopExists = true;
            break;  // Exit loop - we have the meeting point!
        }
    }

    // =========================================================================
    // NO LOOP CHECK: If no loop, return NULL
    // =========================================================================
    // Say: "If no loop exists, I return NULL"
    // WHY check loopExists? If false, fast reached NULL, no Phase 2 needed
    if (!loopExists) return NULL;

    // =========================================================================
    // PHASE 2: Find the loop start
    // =========================================================================
    // Say: "Phase 2: Now I find exactly where the loop starts"

    // Reset slow to head, keep fast at meeting point
    // Say: "I reset slow to head, keeping fast at the meeting point"
    // WHY reset slow? Math proves distance head→loop_start = meeting→loop_start
    slow = head;

    // Move BOTH pointers at SAME speed (1 step each)
    // Say: "I move both pointers one step at a time until they meet"
    // WHY same speed now? They travel equal distances to meet at loop start
    // WHY slow != fast? They will meet at loop start (mathematical guarantee)
    while (slow != fast) {
        // Both move one step forward
        // Say: "Both pointers move forward one step"
        slow = slow->next;
        fast = fast->next;  // NOTE: Fast moves 1 step now, not 2!
    }

    // =========================================================================
    // RETURN: Both pointers now at loop start
    // =========================================================================
    // Say: "When they meet, that's the start of the loop"
    // WHY return slow? Both slow and fast point to loop start now
    return slow;  // (or return fast - they're equal)
}

/*
 * ============================================================================
 * COUNT LOOP LENGTH FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * int countLoopLength(Node* head):
 *   - Returns "int" = number of nodes in the loop, or 0 if no loop
 *   - "Node* head" = pointer to the first node of the list
 *   - Purpose: Count how many nodes are in the cycle
 *
 * ALGORITHM:
 * ---------------------------------
 * 1. First, find where the loop starts (using detectLoopStart)
 * 2. If no loop exists, return 0
 * 3. Starting from loop start, traverse the loop counting nodes
 * 4. Stop when we return to loop start (one full circle)
 *
 * WHY START FROM LOOP START?
 * ---------------------------------
 * - We could also count from the meeting point
 * - Loop start is cleaner conceptually
 * - We already have detectLoopStart() function
 *
 * ALTERNATIVE: Count during detection
 * ---------------------------------
 * You could also count while finding the meeting point:
 *   - After slow == fast, keep one pointer fixed
 *   - Move the other until it returns to same position
 *   - Count the steps
 *
 * This avoids calling detectLoopStart() but is less modular.
 *
 * VISUALIZATION:
 * ---------------------------------
 *
 *   Loop: 3 -> 4 -> 5 -> 6 -> 3 (back to 3)
 *
 *   loopStart = node 3
 *   curr starts at node 4 (loopStart->next)
 *
 *   Iteration 1: curr = 4, count = 2
 *   Iteration 2: curr = 5, count = 3
 *   Iteration 3: curr = 6, count = 4
 *   Iteration 4: curr = 3 = loopStart, STOP
 *
 *   Return 4 (loop has 4 nodes)
 *
 * TIME COMPLEXITY: O(n)
 * ---------------------------------
 * - detectLoopStart is O(n)
 * - Counting the loop is O(loop_length) <= O(n)
 * - Total: O(n)
 *
 * ============================================================================
 */
// Count the number of nodes in the loop
// Say: "I'll count nodes in the loop by traversing it once"
int countLoopLength(Node* head) {
    // =========================================================================
    // STEP 1: Find where the loop starts
    // =========================================================================
    // Say: "First, I find the loop start node"
    // WHY detectLoopStart? We need a reference point to count from
    // Also handles the "no loop" case for us
    Node* loopStart = detectLoopStart(head);

    // =========================================================================
    // EDGE CASE: No loop exists
    // =========================================================================
    // Say: "If there's no loop, return 0"
    // WHY return 0? No loop means loop length is zero
    if (loopStart == NULL) return 0;

    // =========================================================================
    // INITIALIZE: Start counting from 1 (loopStart itself is in the loop)
    // =========================================================================
    // Say: "I initialize count to 1 for the loop start node itself"
    // WHY 1? The loopStart node is part of the loop, so count it
    int count = 1;

    // =========================================================================
    // TRAVERSE: Start from the node after loopStart
    // =========================================================================
    // Say: "I start traversing from the node after loopStart"
    // WHY loopStart->next? We already counted loopStart, start from next
    Node* curr = loopStart->next;

    // =========================================================================
    // COUNT: Traverse until we return to loopStart
    // =========================================================================
    // Say: "I traverse the loop, counting nodes until I return to start"
    // WHY curr != loopStart? We stop when we complete one full circle
    while (curr != loopStart) {
        // Increment count for each node visited
        // Say: "I increment count for each node in the loop"
        count++;

        // Move to the next node in the loop
        // Say: "I move to the next node"
        // WHY no NULL check? We're in a loop, there's no NULL!
        curr = curr->next;
    }

    // =========================================================================
    // RETURN: Total number of nodes in the loop
    // =========================================================================
    // Say: "I return the total count of nodes in the loop"
    // WHY return count? count now holds the loop length
    return count;
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
 * NOTE FOR LISTS WITH LOOPS:
 *   - This function will loop forever on lists with cycles!
 *   - Use the hasLoop() function first to detect loops
 *   - Or use a version with maxNodes limit (see 09_remove_loop.c)
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
 * IMPORTANT NOTE FOR LISTS WITH LOOPS:
 *   - Cannot use this function on lists with loops!
 *   - Would cause infinite loop or double-free
 *   - Must REMOVE the loop first, then free
 *
 * Node* temp:
 *   - We need a temporary pointer to save the next node
 *   - WHY? After free(head), we can't access head->next anymore!
 *
 * ============================================================================
 */
// Free all nodes in the linked list (only for lists WITHOUT loops!)
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

/*
 * ============================================================================
 * REMOVE LOOP FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Purpose: Break the loop so the list can be properly freed
 * Must be called before freeList() on lists with loops!
 *
 * ============================================================================
 */
// Remove the loop from a linked list
// Say: "To free a list with a loop, I first need to break the loop"
void removeLoop(Node* head) {
    // First, find where the loop starts
    // Say: "First, I find the loop start using the detection algorithm"
    Node* loopStart = detectLoopStart(head);

    // If no loop, nothing to do
    // Say: "If there's no loop, nothing to remove"
    if (loopStart == NULL) return;

    // Find the node that points back to loopStart
    // Say: "I traverse the loop to find the node pointing back to the start"
    Node* curr = loopStart;
    while (curr->next != loopStart) {
        curr = curr->next;
    }

    // Break the loop by setting that node's next to NULL
    // Say: "I break the loop by setting that node's next to NULL"
    curr->next = NULL;
}

int main() {
    printf("=== Detect Loop in Linked List ===\n\n");

    // Create list with loop
    Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);

    // Create loop: 5 -> 3
    head->next->next->next->next->next = head->next->next;

    printf("List: 1 -> 2 -> 3 -> 4 -> 5 -> (back to 3)\n");
    printf("Has loop: %s\n", hasLoop(head) ? "YES" : "NO");

    Node* loopStart = detectLoopStart(head);
    if (loopStart) {
        printf("Loop starts at node: %d\n", loopStart->data);
    }

    printf("Loop length: %d\n", countLoopLength(head));

    // Create list without loop
    Node* head2 = createNode(1);
    head2->next = createNode(2);
    head2->next->next = createNode(3);

    printf("\nList: 1 -> 2 -> 3 -> NULL\n");
    printf("Has loop: %s\n", hasLoop(head2) ? "YES" : "NO");

    /*
     * ============================================================================
     * MEMORY CLEANUP - IMPORTANT FOR INTERVIEWS!
     * ============================================================================
     * For list with loop: Must remove loop FIRST, then free
     * For normal list: Can free directly
     * Say: "Finally, I clean up memory - removing the loop first for the looped list"
     * ============================================================================
     */
    removeLoop(head);       // Break the loop first!
    freeList(head);         // Now safe to free
    freeList(head2);        // No loop, free directly

    printf("\n=== Memory freed successfully ===\n");

    return 0;
}
