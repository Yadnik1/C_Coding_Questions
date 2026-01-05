/*
 * ============================================================================
 * PROBLEM: Find Intersection Point of Two Linked Lists
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: High
 *
 * Two lists merge at some point. Find the intersection node.
 *
 * Example:
 * L1: 1 -> 2 ↘
 *            5 -> 6 -> NULL
 * L2: 3 -> 4 ↗
 *
 * Output: Node 5
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. LENGTH DIFFERENCE APPROACH:
 *    - Find lengths of both lists
 *    - Advance longer list by difference
 *    - Move both together until they meet
 *
 * 2. TWO-POINTER MAGIC:
 *    - Traverse both simultaneously
 *    - When one ends, redirect to other's head
 *    - They'll meet at intersection!
 *
 * ============================================================================
 * TWO-POINTER VISUALIZATION:
 * ============================================================================
 *
 * L1: a1 -> a2 -> c1 -> c2 -> NULL
 * L2: b1 -> b2 -> b3 -> c1 -> c2 -> NULL
 *
 * Pointer A starts at L1, Pointer B starts at L2
 *
 * A: a1 -> a2 -> c1 -> c2 -> NULL -> b1 -> b2 -> b3 -> c1
 * B: b1 -> b2 -> b3 -> c1 -> c2 -> NULL -> a1 -> a2 -> c1
 *                                                       ^
 *                                                   They meet!
 *
 * Total distance: len(L1) + len(L2) for both
 * They travel same distance, meet at intersection!
 *
 * ============================================================================
 * TIME: O(n + m) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why does the two-pointer technique work?"
 * A1: Both pointers travel the SAME total distance:
 *
 *     Pointer A: len(L1) + len(L2 up to intersection)
 *     Pointer B: len(L2) + len(L1 up to intersection)
 *
 *     Since both traverse len(L1) + len(L2) total, they meet at intersection!
 *     If no intersection, both become NULL at the same time.
 *
 * -------------------------------------------------------------------------
 * Q2: "What's the length difference approach?"
 * A2: Alternative method that's more intuitive:
 *
 *     1. Find length of both lists
 *     2. Advance longer list by (lenA - lenB)
 *     3. Move both simultaneously - they meet at intersection
 *
 *     Two-pointer is more elegant (no counting), but both are O(n+m).
 *
 * -------------------------------------------------------------------------
 * Q3: "Can you use a hash set instead?"
 * A3: Yes, but uses O(n) extra space:
 *
 *     1. Store all nodes from L1 in hash set
 *     2. Traverse L2, check if node exists in set
 *     3. First match is intersection
 *
 *     Two-pointer is better for embedded (O(1) space).
 *
 * -------------------------------------------------------------------------
 * Q4: "What if lists have different lengths with no intersection?"
 * A4: Both pointers become NULL at the same time!
 *
 *     pA traverses: L1 → NULL → L2 → NULL
 *     pB traverses: L2 → NULL → L1 → NULL
 *
 *     They reach NULL together, so we return NULL correctly.
 *
 * -------------------------------------------------------------------------
 * Q5: "How do you verify two lists actually share nodes (not just values)?"
 * A5: We compare POINTERS, not data values!
 *
 *     pA == pB checks if they point to the SAME memory address.
 *     Two nodes with data=5 are NOT the same unless they're the same object.
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
 * malloc(sizeof(Node)): Allocates memory on HEAP for one Node
 * node->data = data:    Stores the parameter value using -> operator
 * node->next = NULL:    Initializes pointer (CRITICAL to avoid bugs)
 * return node:          Returns pointer to heap-allocated node
 *
 * ============================================================================
 */
Node* createNode(int data) {
    // Allocate memory on HEAP for one Node structure
    // Say: "I allocate memory for a new node using malloc"
    Node* node = (Node*)malloc(sizeof(Node));

    // Store the data value in the node
    // Say: "I store the data value in the node's data field"
    node->data = data;

    // Initialize next pointer to NULL (not connected yet)
    // Say: "I set next to NULL since this node isn't linked to anything yet"
    node->next = NULL;

    // Return pointer to the new node
    // Say: "I return the pointer to the newly created node"
    return node;
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
 * GET INTERSECTION FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* getIntersection(Node* headA, Node* headB):
 *   - Returns "Node*" = pointer to intersection node, or NULL if none
 *   - "Node* headA" = head of first list
 *   - "Node* headB" = head of second list
 *   - Purpose: Find the node where two lists merge (share the same memory)
 *
 * ALGORITHM: Two-Pointer Switch Technique
 * ---------------------------------
 * Key insight: If both pointers travel the SAME total distance,
 * they will meet at the intersection point!
 *
 * When pA reaches the end of L1, redirect it to headB
 * When pB reaches the end of L2, redirect it to headA
 *
 * Total distance for pA: len(L1) + len(L2)
 * Total distance for pB: len(L2) + len(L1)
 *
 * Same distance means they meet at the same point!
 *
 * WHY THIS WORKS - MATHEMATICAL PROOF:
 * ---------------------------------
 * Let:
 *   - a = unique nodes in L1 (before intersection)
 *   - b = unique nodes in L2 (before intersection)
 *   - c = common nodes (after intersection)
 *
 * L1 length = a + c
 * L2 length = b + c
 *
 * Path of pA: a + c + b (L1 then part of L2 until intersection)
 * Path of pB: b + c + a (L2 then part of L1 until intersection)
 *
 * Both paths have length a + b + c → they meet at intersection!
 *
 * IF NO INTERSECTION:
 * ---------------------------------
 * pA travels: a + 0 (L1) + b + 0 (L2) = a + b
 * pB travels: b + 0 (L2) + a + 0 (L1) = a + b
 *
 * Both become NULL at the same time → return NULL correctly!
 *
 * VISUALIZATION:
 * ---------------------------------
 * L1: 1 -> 2 -> 5 -> 6 -> NULL    (a=2 unique, c=2 common)
 * L2: 3 -> 4 -> 5 -> 6 -> NULL    (b=2 unique, c=2 common)
 *
 * pA: 1 -> 2 -> 5 -> 6 -> NULL -> 3 -> 4 -> [5]
 * pB: 3 -> 4 -> 5 -> 6 -> NULL -> 1 -> 2 -> [5]
 *                                           ^ MEET!
 *
 * Both traveled: 2 + 2 + 2 = 6 nodes to reach intersection
 *
 * WHY COMPARE POINTERS (NOT VALUES)?
 * ---------------------------------
 * Intersection means SAME NODE IN MEMORY, not same value.
 * pA == pB checks if they point to the SAME address.
 * Two nodes with data=5 at different addresses are NOT the same.
 *
 * TIME COMPLEXITY: O(n + m)
 * ---------------------------------
 * - n = length of L1, m = length of L2
 * - Each pointer traverses at most n + m nodes
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 * - Only two pointers regardless of list sizes
 * - No hash set, no extra data structures!
 *
 * ============================================================================
 */
// Find intersection point using two-pointer switch technique
// Say: "I'll use two pointers that switch lists when reaching the end"
Node* getIntersection(Node* headA, Node* headB) {
    // =========================================================================
    // EDGE CASE: If either list is empty, no intersection possible
    // =========================================================================
    // Say: "First I check if either list is empty"
    // WHY? Empty list can't intersect with anything
    if (headA == NULL || headB == NULL) return NULL;

    // =========================================================================
    // INITIALIZE TWO POINTERS - One for each list
    // =========================================================================
    // Say: "I initialize two pointers, one for each list"
    // WHY two pointers? They'll traverse both lists and meet at intersection
    Node* pA = headA;  // Pointer starting at list A
    Node* pB = headB;  // Pointer starting at list B

    // =========================================================================
    // MAIN LOOP: Traverse until pointers meet (or both NULL)
    // =========================================================================
    // Say: "I move both pointers until they point to the same node"
    // WHY pA != pB? We stop when they meet (either at intersection or both NULL)
    while (pA != pB) {
        // SWITCH LOGIC FOR pA: When reaching end, jump to other list's head
        // Say: "If pA reaches the end of L1, I redirect it to start of L2"
        // WHY switch? This ensures both pointers travel same total distance
        // If pA is NULL, start at headB; otherwise move forward in current list
        pA = (pA == NULL) ? headB : pA->next;

        // SWITCH LOGIC FOR pB: When reaching end, jump to other list's head
        // Say: "If pB reaches the end of L2, I redirect it to start of L1"
        // WHY switch? Same reason - equalizes total distance traveled
        // If pB is NULL, start at headA; otherwise move forward in current list
        pB = (pB == NULL) ? headA : pB->next;
    }

    // =========================================================================
    // RETURN: pA and pB now point to same node (intersection or NULL)
    // =========================================================================
    // Say: "When they meet, that's the intersection point, or NULL if no intersection"
    // WHY return pA? pA == pB at this point, so either works
    // Could be intersection node or NULL (if lists don't intersect)
    return pA;
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
 *
 * SPECIAL CASE FOR INTERSECTING LISTS:
 *   - Two lists share common nodes after intersection
 *   - We MUST NOT free the same node twice (double-free crash!)
 *   - Solution: Only free the unique prefix of each list
 *   - Or: Track which nodes are already freed
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

/*
 * ============================================================================
 * FREE LIST UNTIL FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void freeListUntil(Node* head, Node* stop):
 *   - "void" = Function doesn't return anything
 *   - "Node* head" = Start of the list to free
 *   - "Node* stop" = Node to stop at (don't free this or beyond)
 *   - Purpose: Free only part of a list, up to but NOT including stop
 *
 * WHY DO WE NEED THIS SPECIAL FUNCTION?
 * ---------------------------------
 * When two lists intersect, they SHARE nodes after the intersection point.
 *
 *   L1: 1 -> 2 ↘
 *              5 -> 6 -> NULL  (shared!)
 *   L2: 3 -> 4 ↗
 *
 * If we call freeList(L1) and then freeList(L2), we'd free nodes 5 and 6 TWICE!
 * Double-free causes undefined behavior (crashes, memory corruption).
 *
 * SOLUTION: Free each unique prefix separately, then free shared part ONCE.
 *
 * VISUALIZATION:
 * ---------------------------------
 * freeListUntil(L1, common):  Frees 1 -> 2 (stops at 5)
 * freeListUntil(L2, common):  Frees 3 -> 4 (stops at 5)
 * freeList(common):           Frees 5 -> 6 (only once!)
 *
 * TIME COMPLEXITY: O(k) where k = nodes before stop
 * ---------------------------------
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 *
 * ============================================================================
 */
// Free only the unique prefix of a list (before intersection point)
// Say: "For intersecting lists, I only free the nodes before the intersection"
void freeListUntil(Node* head, Node* stop) {
    // Temporary pointer to save next node before freeing
    // Say: "I need a temp pointer to save next before freeing"
    Node* temp;

    // Free nodes until we reach the stop point (or end of list)
    // Say: "I free nodes until I reach the stop node"
    // WHY head != stop? We don't want to free the shared portion
    // WHY head != NULL? Safety in case stop is not in the list
    while (head != NULL && head != stop) {
        // Save next pointer before freeing
        // Say: "I save the next pointer"
        temp = head->next;

        // Free current node
        // Say: "I free the current node"
        free(head);

        // Move to next node
        // Say: "I move to the next node"
        head = temp;
    }
    // After loop: head == stop (or NULL if stop not found)
    // The shared portion (from stop onwards) is NOT freed here
}

int main() {
    printf("=== Intersection of Two Lists ===\n\n");

    // Create intersection node
    Node* common = createNode(5);
    common->next = createNode(6);

    // Create L1: 1 -> 2 -> 5 -> 6
    Node* l1 = createNode(1);
    l1->next = createNode(2);
    l1->next->next = common;

    // Create L2: 3 -> 4 -> 5 -> 6
    Node* l2 = createNode(3);
    l2->next = createNode(4);
    l2->next->next = common;

    printf("L1: 1 -> 2 -> 5 -> 6 -> NULL\n");
    printf("L2: 3 -> 4 -> 5 -> 6 -> NULL\n");
    printf("          ↘   ↗\n");

    Node* intersection = getIntersection(l1, l2);
    if (intersection) {
        printf("Intersection at node: %d\n", intersection->data);
    } else {
        printf("No intersection\n");
    }

    /*
     * ============================================================================
     * MEMORY CLEANUP - IMPORTANT FOR INTERVIEWS!
     * ============================================================================
     * CAREFUL: L1 and L2 share nodes after intersection (5 -> 6)
     * We must free unique parts separately, then the common part once.
     * Say: "I free the unique prefix of each list, then the common part once"
     * ============================================================================
     */
    freeListUntil(l1, common);   // Free 1 -> 2 (unique to L1)
    freeListUntil(l2, common);   // Free 3 -> 4 (unique to L2)
    freeList(common);            // Free 5 -> 6 (shared part, only once!)

    printf("\n=== Memory freed successfully ===\n");

    return 0;
}
