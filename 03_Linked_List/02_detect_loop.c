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

// Detect if loop exists
bool hasLoop(Node* head) {
    // Edge case: empty list has no loop
    // Say: "First, handle the edge case of an empty list"
    if (head == NULL) return false;

    // Initialize slow pointer at head
    // Say: "I'll use Floyd's cycle detection with two pointers"
    Node* slow = head;

    // Initialize fast pointer at head
    // Say: "Both pointers start at the head"
    Node* fast = head;

    // Traverse list with two speeds
    // Say: "I'll move slow by 1 and fast by 2 until they meet or fast reaches end"
    while (fast != NULL && fast->next != NULL) {
        // Move slow pointer one step
        // Say: "Slow moves one step forward"
        slow = slow->next;

        // Move fast pointer two steps
        // Say: "Fast moves two steps forward"
        fast = fast->next->next;

        // Check if pointers met
        // Say: "If they meet, we've found a loop"
        if (slow == fast) {
            return true;  // Loop detected!
        }
    }

    // Fast reached end without meeting slow
    // Say: "Fast reached the end, so no loop exists"
    return false;
}

// Detect loop and return start node
Node* detectLoopStart(Node* head) {
    // Edge case: empty list
    // Say: "Handle empty list edge case"
    if (head == NULL) return NULL;

    // Initialize pointers for phase 1
    // Say: "Phase 1: Detect if a loop exists"
    Node* slow = head;
    Node* fast = head;

    // Track whether loop was found
    // Say: "I'll use a flag to track if we find a loop"
    bool loopExists = false;

    // Phase 1: Detect loop
    // Say: "First, detect if there's a loop using the same algorithm"
    while (fast != NULL && fast->next != NULL) {
        // Move pointers at different speeds
        slow = slow->next;
        fast = fast->next->next;

        // Check for meeting point
        // Say: "When they meet, we know a loop exists"
        if (slow == fast) {
            loopExists = true;
            break;
        }
    }

    // No loop found
    // Say: "If no loop exists, return NULL"
    if (!loopExists) return NULL;

    // Phase 2: Find loop start
    // Say: "Phase 2: Find where the loop starts"
    // Reset slow to head
    // Say: "Reset slow to head while keeping fast at the meeting point"
    slow = head;

    // Move both pointers at same speed
    // Say: "Now move both pointers one step at a time"
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;  // Both move 1 step now
    }

    // They meet at loop start
    // Say: "When they meet again, that's the start of the loop"
    return slow;
}

// Count nodes in loop
int countLoopLength(Node* head) {
    // Find where loop starts
    // Say: "First find the loop start node"
    Node* loopStart = detectLoopStart(head);

    // No loop case
    // Say: "If there's no loop, return 0"
    if (loopStart == NULL) return 0;

    // Start counting from 1
    // Say: "Initialize count to 1 for the loop start node"
    int count = 1;

    // Traverse the loop
    // Say: "Traverse the loop and count nodes until we return to start"
    Node* curr = loopStart->next;

    // Count until we return to start
    while (curr != loopStart) {
        count++;
        curr = curr->next;
    }

    // Return total count
    // Say: "Return the total number of nodes in the loop"
    return count;
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

    return 0;
}
