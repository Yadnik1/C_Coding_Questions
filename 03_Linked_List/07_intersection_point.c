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

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

Node* getIntersection(Node* headA, Node* headB) {
    if (headA == NULL || headB == NULL) return NULL;

    Node* pA = headA;
    Node* pB = headB;

    while (pA != pB) {
        pA = (pA == NULL) ? headB : pA->next;
        pB = (pB == NULL) ? headA : pB->next;
    }

    return pA;  // Either intersection or NULL
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
