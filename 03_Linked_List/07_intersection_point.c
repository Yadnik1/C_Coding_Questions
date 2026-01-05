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

    return 0;
}
