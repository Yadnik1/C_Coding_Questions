/*
 * ============================================================================
 * PROBLEM: Detect Cycle in a Linked List (Floyd's Algorithm)
 * ============================================================================
 *
 * WHAT IS A LINKED LIST? (Quick Recap)
 * A linked list is a chain of nodes where each node has:
 *   - data: The value stored
 *   - next: Pointer to the next node (NULL if last node)
 *
 * NODE STRUCTURE:
 *   +-------+-------+
 *   | data  | next  | ---> next node
 *   +-------+-------+
 *
 * WHAT IS A CYCLE?
 * A cycle occurs when a node's "next" pointer points back to a previous
 * node in the list, creating an infinite loop. The list never ends!
 *
 * WHAT IS THIS PROBLEM?
 * Given the head of a linked list, determine if the list contains a cycle.
 * A cycle exists if following the next pointers eventually leads back to
 * a node you've already visited.
 *
 * EXAMPLES:
 *   Input:  1 -> 2 -> 3 -> 4 -> 5
 *                ^              |
 *                |______________|  (5 points back to 3)
 *   Output: true (cycle exists)
 *
 *   Input:  1 -> 2 -> 3 -> NULL
 *   Output: false (no cycle, list ends)
 *
 *   Input:  1 -> (points to itself)
 *   Output: true (self-loop is a cycle)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 *   - Tests understanding of the famous Floyd's algorithm
 *   - O(1) space solution is non-trivial and clever
 *   - Foundation for finding cycle start, cycle length
 *   - Real-world application: detecting infinite loops in systems
 *   - Shows mathematical thinking (why do they meet?)
 *
 * KEY CONCEPT: Floyd's Tortoise and Hare Algorithm
 *   Use two pointers moving at different speeds:
 *   - Slow (tortoise): Moves 1 step at a time
 *   - Fast (hare): Moves 2 steps at a time
 *
 *   If there's a cycle, fast will eventually "lap" slow and they'll meet.
 *   If no cycle, fast reaches NULL (end of list).
 *
 *   WHY THEY MUST MEET:
 *   - In the cycle, fast gains 1 node per iteration (2-1=1)
 *   - The gap between them decreases by 1 each step
 *   - They MUST eventually meet within the cycle
 *
 * VISUAL:
 *
 *   List with cycle:
 *   1 -> 2 -> 3 -> 4 -> 5
 *             ^         |
 *             |_________|
 *
 *   Step 0: slow=1, fast=1
 *   Step 1: slow=2, fast=3
 *   Step 2: slow=3, fast=5
 *   Step 3: slow=4, fast=4  <-- THEY MEET! Cycle detected.
 *
 *   List without cycle:
 *   1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *
 *   Step 0: slow=1, fast=1
 *   Step 1: slow=2, fast=3
 *   Step 2: slow=3, fast=5
 *   Step 3: fast->next is NULL, STOP. No cycle.
 *
 * BONUS: Finding where cycle starts (Phase 2)
 *   After detection, reset slow to head.
 *   Move both pointers 1 step at a time.
 *   They meet at the cycle's starting node!
 *   (Mathematical proof: involves cycle length equations)
 *
 * ============================================================================
 */

/*
 * DETECT CYCLE IN LINKED LIST - Floyd's Cycle Detection
 *
 * Time Complexity: O(n) - At most 2n steps
 * Space Complexity: O(1) - Only two pointers used
 *
 * Pattern: Floyd's Tortoise and Hare Algorithm
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ==================== HELPER FUNCTIONS ==================== */

/*
 * ============================================================================
 * NODE STRUCTURE - Line by Line Explanation
 * ============================================================================
 * Say: "A Node has two parts - data stores the value, next is a pointer
 *       to the next node in the list"
 *
 * typedef struct Node {
 *     int data;           <- The payload/value stored in this node
 *     struct Node* next;  <- Pointer to the next node (creates the "link")
 * } Node;                 <- 'Node' is now an alias for 'struct Node'
 *
 * Memory layout:
 * +--------+--------+
 * |  data  |  next  |----> points to another Node (or NULL)
 * +--------+--------+
 * ============================================================================
 */
typedef struct Node {
    int data;               // Say: "The value this node holds"
    struct Node* next;      // Say: "Address of the next node in the chain"
} Node;

/*
 * ============================================================================
 * CREATE NODE FUNCTION - Line by Line Explanation
 * ============================================================================
 * Say: "This function allocates memory for a new node and initializes it"
 *
 * Node* create_node(int data) {
 * ^^^^^                       <- Returns a POINTER to the new node
 *
 *     Node* node = (Node*)malloc(sizeof(Node));
 *                  ^^^^^^ ^^^^^^ ^^^^^^^^^^^^
 *                  |      |      |
 *                  |      |      sizeof(Node) = bytes needed for one Node
 *                  |      malloc = allocate memory on heap (survives function return)
 *                  (Node*) = cast void* to Node* (tell compiler it's a Node)
 *
 *     node->data = data;   <- Set the value (-> because node is a pointer)
 *     node->next = NULL;   <- No next node yet (NULL = end of chain)
 *     return node;         <- Return the address of the new node
 *
 * Say: "We use malloc because stack variables die when function returns,
 *       but heap memory persists until we free() it"
 * ============================================================================
 */
Node* create_node(int data) {
    Node* node = (Node*)malloc(sizeof(Node));   // Allocate on heap
    node->data = data;                          // Store the value
    node->next = NULL;                          // Initialize link to NULL
    return node;                                // Return address of new node
}

/*
 * ============================================================================
 * PRINT LIST FUNCTION - Line by Line Explanation
 * ============================================================================
 * Say: "This traverses the list from head to end, printing each value"
 *
 * void print_list(Node* head) {
 * ^^^^                       <- Returns nothing, just prints
 *      Node* head            <- Receives pointer to first node
 *
 *     while (head) {         <- while (head != NULL) - continue until end
 *         printf("%d", head->data);  <- Print current node's value
 *         if (head->next) printf(" -> ");  <- Arrow if more nodes follow
 *         head = head->next; <- Move to next node (traverse the link)
 *     }
 *
 * Say: "We keep following the 'next' pointers until we hit NULL"
 *
 * Traversal visualization:
 *   head -> [1] -> [2] -> [3] -> NULL
 *            ^
 *          start here, print 1, move to [2], print 2, etc.
 * ============================================================================
 */
void print_list(Node* head) {
    printf("[");                            // Start output
    while (head) {                          // Until we reach NULL
        printf("%d", head->data);           // Print current value
        if (head->next) printf(" -> ");     // Arrow if not last node
        head = head->next;                  // Move to next node
    }
    printf("]\n");                          // End output
}

/* ==================== SOLUTION ==================== */

/*
 * ============================================================================
 * DRY-RUN DIAGRAM: Detect Cycle - Floyd's Tortoise and Hare
 * ============================================================================
 *
 * EXAMPLE 1: List WITH Cycle
 * ==========================
 *
 *   List: 1 -> 2 -> 3 -> 4 -> 5
 *                   ^         |
 *                   |_________|  (5->next points to 3)
 *
 * INITIAL STATE:
 *
 *   slow, fast
 *       |
 *       v
 *      [1] --> [2] --> [3] --> [4] --> [5]
 *                       ^               |
 *                       |_______________|
 *
 * ----------------------------------------------------------------------------
 * ITERATION 1:
 * ----------------------------------------------------------------------------
 *   slow = slow->next (move to 2)
 *   fast = fast->next->next (move to 3)
 *
 *              slow          fast
 *               |             |
 *               v             v
 *      [1] --> [2] --> [3] --> [4] --> [5]
 *                       ^               |
 *                       |_______________|
 *
 *   slow != fast (2 != 3) -> continue
 *
 * ----------------------------------------------------------------------------
 * ITERATION 2:
 * ----------------------------------------------------------------------------
 *   slow = slow->next (move to 3)
 *   fast = fast->next->next (move to 5)
 *
 *                      slow            fast
 *                       |               |
 *                       v               v
 *      [1] --> [2] --> [3] --> [4] --> [5]
 *                       ^               |
 *                       |_______________|
 *
 *   slow != fast (3 != 5) -> continue
 *
 * ----------------------------------------------------------------------------
 * ITERATION 3:
 * ----------------------------------------------------------------------------
 *   slow = slow->next (move to 4)
 *   fast = fast->next->next (5->next is 3, 3->next is 4, so fast moves to 4)
 *
 *                              slow, fast
 *                                  |
 *                                  v
 *      [1] --> [2] --> [3] --> [4] --> [5]
 *                       ^               |
 *                       |_______________|
 *
 *   slow == fast (4 == 4) -> CYCLE DETECTED! Return true
 *
 * ============================================================================
 * EXAMPLE 2: List WITHOUT Cycle
 * ============================================================================
 *
 *   List: 1 -> 2 -> 3 -> 4 -> 5 -> NULL
 *
 * INITIAL STATE:
 *
 *   slow, fast
 *       |
 *       v
 *      [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 1:
 * ----------------------------------------------------------------------------
 *   slow = slow->next (move to 2)
 *   fast = fast->next->next (move to 3)
 *
 *              slow          fast
 *               |             |
 *               v             v
 *      [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 2:
 * ----------------------------------------------------------------------------
 *   slow = slow->next (move to 3)
 *   fast = fast->next->next (move to 5)
 *
 *                      slow            fast
 *                       |               |
 *                       v               v
 *      [1] --> [2] --> [3] --> [4] --> [5] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 3:
 * ----------------------------------------------------------------------------
 *   Check: fast != NULL (true) && fast->next != NULL (false - 5->next is NULL)
 *   LOOP EXITS
 *
 *   Return false (no cycle)
 *
 * ============================================================================
 * WHY THEY MUST MEET IN A CYCLE:
 * ============================================================================
 *
 *   - In the cycle, fast gains 1 node per iteration (2 - 1 = 1)
 *   - The gap between them decreases by 1 each step
 *   - If cycle length is C, they MUST meet within C iterations
 *
 *   Example: slow at position 0, fast at position 2 in cycle of length 5
 *   Gap = 2
 *   After 1 iteration: gap = 1
 *   After 2 iterations: gap = 0 (they meet!)
 *
 * ============================================================================
 */

/*
 * ============================================================================
 * HAS_CYCLE FUNCTION - Line by Line Explanation
 * ============================================================================
 */
bool has_cycle(Node* head) {
    /*
     * Say: "I use Floyd's algorithm with slow and fast pointers"
     *
     * FLOYD'S ALGORITHM CONCEPT:
     * - Two pointers start at head
     * - Slow moves 1 step per iteration
     * - Fast moves 2 steps per iteration
     * - If there's a cycle, fast will eventually "lap" slow and they'll meet
     * - If no cycle, fast reaches NULL (end of list)
     */

    if (head == NULL || head->next == NULL) {
        /*
         * head == NULL: Empty list, no nodes, no cycle possible
         * head->next == NULL: Only one node, and it points to NULL, no cycle
         *
         * Say: "Edge case - empty or single node without self-loop has no cycle"
         */
        return false;
    }

    Node* slow = head;
    /*    ^^^^^^^^^^^^
     *    slow = "tortoise" pointer, starts at head
     *    Will move 1 step at a time
     *
     *    Visualization:
     *    slow
     *     |
     *     v
     *    [1] -> [2] -> [3] -> ...
     */

    Node* fast = head;
    /*    ^^^^^^^^^^^^
     *    fast = "hare" pointer, also starts at head
     *    Will move 2 steps at a time
     *
     *    Visualization:
     *    slow, fast
     *        |
     *        v
     *       [1] -> [2] -> [3] -> ...
     */

    while (fast != NULL && fast->next != NULL) {
        /*    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
         *    WHY TWO CONDITIONS?
         *
         *    fast != NULL:
         *    - Check fast hasn't gone past the end
         *    - If fast is NULL, we've reached end, no cycle
         *
         *    fast->next != NULL:
         *    - Check we can safely move fast TWO steps
         *    - We're about to do fast->next->next
         *    - If fast->next is NULL, we can't do another ->next
         *
         *    Say: "We check both because fast moves 2 steps - need both steps valid"
         */

        slow = slow->next;
        /*    ^^^^^^^^^^^^^
         *    MOVE SLOW BY 1 STEP
         *
         *    slow now points to what was slow->next
         *    Like taking 1 step forward
         *
         *    Before: slow at [1]
         *    After:  slow at [2]
         */

        fast = fast->next->next;
        /*    ^^^^^^^^^^^^^^^^^^
         *    MOVE FAST BY 2 STEPS
         *
         *    fast->next = first step
         *    fast->next->next = second step
         *    fast now points 2 nodes ahead
         *
         *    Before: fast at [1]
         *    After:  fast at [3]
         *
         *    WHY FAST MOVES 2x:
         *    - If there's a cycle, fast gains 1 node per iteration
         *    - Eventually fast "catches up" to slow from behind
         *    - They MUST meet if there's a cycle
         */

        if (slow == fast) {
            /*    ^^^^^^^^^^^
             *    THEY MEET = CYCLE EXISTS!
             *
             *    We're comparing ADDRESSES, not values
             *    slow == fast means they point to the SAME NODE in memory
             *
             *    Why they meet in a cycle:
             *    - In the cycle, fast gains 1 node per iteration (2-1=1)
             *    - Gap between them decreases by 1 each step
             *    - They MUST meet within cycle_length iterations
             *
             *    Say: "If slow and fast point to the same node, there's a cycle"
             */
            return true;
        }
    }
    /*
     * Loop ended because fast reached NULL or fast->next is NULL
     * This means fast ran off the end of the list
     * If there was a cycle, fast would never reach NULL!
     *
     * Say: "Fast reached the end, so no cycle exists"
     */

    return false;
    /*     ^^^^^
     *     No cycle found - the list has an end (terminates at NULL)
     */
}

// BONUS: Find the start of the cycle
Node* find_cycle_start(Node* head) {
    // Say: "First detect if cycle exists using Floyd's"
    if (head == NULL || head->next == NULL) {
        return NULL;
    }

    Node* slow = head;
    Node* fast = head;

    // Phase 1: Detect cycle
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            // Say: "Cycle found! Now find the start"
            // Phase 2: Find cycle start
            // Say: "Reset slow to head, move both by 1, they meet at cycle start"
            slow = head;
            while (slow != fast) {
                slow = slow->next;
                fast = fast->next;
            }
            return slow;
        }
    }

    return NULL;
}

/* ==================== TEST ==================== */

int main() {
    // Create list with cycle: 1 -> 2 -> 3 -> 4 -> 5 -> 3 (cycle)
    Node* head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);
    head->next->next->next->next = create_node(5);
    head->next->next->next->next->next = head->next->next;  // Create cycle to 3

    printf("List with cycle (1->2->3->4->5->3...)\n");
    printf("Has cycle: %s\n", has_cycle(head) ? "YES" : "NO");

    Node* cycle_start = find_cycle_start(head);
    if (cycle_start) {
        printf("Cycle starts at node with value: %d\n", cycle_start->data);
    }

    // Create list without cycle
    Node* no_cycle = create_node(1);
    no_cycle->next = create_node(2);
    no_cycle->next->next = create_node(3);

    printf("\nList without cycle: ");
    print_list(no_cycle);
    printf("Has cycle: %s\n", has_cycle(no_cycle) ? "YES" : "NO");

    // Test empty list
    printf("\nEmpty list has cycle: %s\n", has_cycle(NULL) ? "YES" : "NO");

    // Test single node pointing to itself
    Node* self_loop = create_node(1);
    self_loop->next = self_loop;
    printf("Self-loop has cycle: %s\n", has_cycle(self_loop) ? "YES" : "NO");

    return 0;
}

/*
 * ==================== INTERVIEW EXPLANATION ====================
 *
 * PROBLEM: Detect if a linked list has a cycle.
 *
 * FLOYD'S TORTOISE AND HARE ALGORITHM:
 *
 * Visual (with cycle):
 *   1 -> 2 -> 3 -> 4 -> 5
 *             ^         |
 *             |_________|
 *
 * WHY IT WORKS:
 * - If there's a cycle, fast will eventually "lap" slow
 * - Fast gains 1 node per iteration (2-1 = 1)
 * - They MUST meet within the cycle
 *
 * MATHEMATICAL PROOF (for finding cycle start):
 * - Let distance from head to cycle start = a
 * - Let distance from cycle start to meeting point = b
 * - Let cycle length = c
 *
 * When they meet:
 * - Slow traveled: a + b
 * - Fast traveled: a + b + nc (n complete cycles)
 * - Fast = 2 * Slow: a + b + nc = 2(a + b)
 * - Therefore: nc = a + b, so a = nc - b
 *
 * This means: Distance from head to cycle start (a) =
 *             Distance from meeting point to cycle start (c - b)
 *
 * WHY SLOW/FAST POINTER (not hash set):
 * 1. O(1) space vs O(n) for hash set
 * 2. No extra data structures needed
 * 3. Classic interview expectation
 *
 * EDGE CASES TO MENTION:
 * 1. Empty list -> No cycle
 * 2. Single node, no self-loop -> No cycle
 * 3. Single node with self-loop -> Has cycle
 * 4. Cycle at head vs cycle in middle
 *
 * COMMON MISTAKES:
 * - Checking fast != NULL but not fast->next != NULL
 * - Not handling empty or single-node lists
 * - Confusing cycle detection with finding cycle start
 *
 * FOLLOW-UP QUESTIONS:
 * Q: Why check fast AND fast->next?
 * A: fast moves 2 steps; need to ensure both steps are valid
 *
 * Q: What if we want cycle length?
 * A: After detection, keep one pointer fixed, count until they meet again
 */
