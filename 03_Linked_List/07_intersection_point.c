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

Node* getIntersection(Node* headA, Node* headB) {
    // Edge case: if either list is empty
    // Say: "First check if either list is empty - no intersection possible"
    if (headA == NULL || headB == NULL) return NULL;

    // Initialize pointer for list A
    // Say: "I'll use two pointers, one for each list"
    Node* pA = headA;

    // Initialize pointer for list B
    // Say: "Both pointers start at their respective heads"
    Node* pB = headB;

    // Traverse both lists simultaneously
    // Say: "Move both pointers until they meet or both become NULL"
    while (pA != pB) {
        // Move pA forward, or switch to headB if at end
        // Say: "If pA reaches the end, redirect it to headB, otherwise move forward"
        pA = (pA == NULL) ? headB : pA->next;

        // Move pB forward, or switch to headA if at end
        // Say: "If pB reaches the end, redirect it to headA, otherwise move forward"
        pB = (pB == NULL) ? headA : pB->next;
    }

    // Either intersection or both NULL
    // Say: "When they meet, that's either the intersection or NULL if no intersection"
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

// Free only the unique prefix of a list (before intersection point)
// Say: "For intersecting lists, I only free the nodes before the intersection"
void freeListUntil(Node* head, Node* stop) {
    Node* temp;
    while (head != NULL && head != stop) {
        temp = head->next;
        free(head);
        head = temp;
    }
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
