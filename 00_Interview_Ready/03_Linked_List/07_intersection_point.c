/*
 * ============================================================================
 * PROBLEM: Find Intersection Point of Two Linked Lists
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given the heads of two singly linked lists, find the node where they
 * intersect (merge into a single list). If they don't intersect, return NULL.
 *
 * IMPORTANT: Intersection means the same PHYSICAL NODE (same memory address),
 * not just nodes with the same value!
 *
 * EXAMPLES:
 *   Input:
 *         1 -> 2 \
 *                 -> 6 -> 7 -> 8 -> NULL
 *   3 -> 4 -> 5 /
 *
 *   Output: Node with value 6 (the intersection point)
 *
 *   Input:  List A: 1 -> 2 -> 3
 *           List B: 4 -> 5 -> 6
 *   Output: NULL (no intersection, completely separate lists)
 *
 *   Input:  List A: 1 -> 2 -> 3 -> 4
 *           List B: same as List A (identical)
 *   Output: Node with value 1 (intersection at head)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 *   - Tests creative thinking for O(1) space solution
 *   - Two elegant approaches to discuss
 *   - Shows understanding of pointer comparison vs value comparison
 *   - Real-world: detecting shared resources, memory references
 *   - Good follow-up: "What if there might be cycles?"
 *
 * KEY CONCEPT: Two Approaches
 *
 * APPROACH 1: Length Alignment
 *   - Calculate lengths of both lists
 *   - Advance the longer list's pointer by the difference
 *   - Move both pointers together until they meet
 *
 * APPROACH 2: Two-Pointer Cycle (Elegant!)
 *   - Pointer A traverses: List A, then List B
 *   - Pointer B traverses: List B, then List A
 *   - Both travel same total distance and meet at intersection!
 *
 * VISUAL (Approach 1 - Length Alignment):
 *
 *         A: 1 -> 2 ----\
 *                        -> 6 -> 7 -> 8
 *   B: 3 -> 4 -> 5 -----/
 *
 *   Length A = 5 (1,2,6,7,8)
 *   Length B = 6 (3,4,5,6,7,8)
 *   Difference = 1
 *
 *   Advance B by 1: Start at 4
 *   Now both have equal distance to intersection:
 *   A: 1 -> 2 -> 6 -> 7 -> 8  (start at 1)
 *   B: 4 -> 5 -> 6 -> 7 -> 8  (start at 4)
 *
 *   Move together: 1,4 -> 2,5 -> 6,6 (MATCH!)
 *
 * VISUAL (Approach 2 - Two Pointer Cycle):
 *
 *   ptrA path: A1 -> A2 -> 6 -> 7 -> 8 -> B3 -> B4 -> B5 -> 6 (MEET!)
 *   ptrB path: B3 -> B4 -> B5 -> 6 -> 7 -> 8 -> A1 -> A2 -> 6 (MEET!)
 *
 *   Both travel: lenA + lenB steps
 *   They sync up at intersection after switching lists!
 *
 *   If no intersection: Both reach NULL at the same time
 *
 * ============================================================================
 */

/*
 * INTERSECTION POINT OF TWO LINKED LISTS - Length Alignment
 *
 * Time Complexity: O(n + m) - Calculate lengths + traverse
 * Space Complexity: O(1) - Only pointers
 *
 * Pattern: Calculate Lengths, Align Starts, Find Intersection
 */

#include <stdio.h>
#include <stdlib.h>

/* ==================== HELPER FUNCTIONS ==================== */

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

void print_list(Node* head) {
    printf("[");
    while (head) {
        printf("%d", head->data);
        if (head->next) printf(" -> ");
        head = head->next;
    }
    printf("]\n");
}

/* ==================== SOLUTION ==================== */

// Helper: Get length of list
int get_length(Node* head) {
    int len = 0;
    while (head) {
        len++;
        head = head->next;
    }
    return len;
}

/*
 * ============================================================================
 * DRY-RUN DIAGRAM: Find Intersection Point of Two Linked Lists
 * ============================================================================
 *
 * EXAMPLE:
 *   List A: [1] --> [2] --\
 *                          \
 *                           --> [6] --> [7] --> [8] --> NULL
 *                          /
 *   List B: [3] --> [4] --> [5] --/
 *
 *   lenA = 5 (nodes: 1, 2, 6, 7, 8)
 *   lenB = 6 (nodes: 3, 4, 5, 6, 7, 8)
 *   Difference = 1 (B is longer)
 *
 * ============================================================================
 * PHASE 1: Calculate lengths
 * ============================================================================
 *
 *   lenA = 5, lenB = 6
 *   Difference = 6 - 5 = 1
 *
 * ============================================================================
 * PHASE 2: Align starting points (advance longer list by difference)
 * ============================================================================
 *
 *   List B is longer by 1, so advance headB by 1:
 *
 *   headA: [1] --> [2] --> [6] --> [7] --> [8] --> NULL
 *           ^
 *         headA
 *
 *   headB: [3] --> [4] --> [5] --> [6] --> [7] --> [8] --> NULL
 *                   ^
 *                 headB (advanced by 1)
 *
 *   Now both have 5 nodes remaining to traverse!
 *
 * ============================================================================
 * PHASE 3: Move both pointers together until they meet
 * ============================================================================
 *
 * ----------------------------------------------------------------------------
 * ITERATION 1:
 * ----------------------------------------------------------------------------
 *   headA points to [1], headB points to [4]
 *   headA != headB (different nodes)
 *   headA = headA->next (move to 2)
 *   headB = headB->next (move to 5)
 *
 * ----------------------------------------------------------------------------
 * ITERATION 2:
 * ----------------------------------------------------------------------------
 *   headA points to [2], headB points to [5]
 *   headA != headB
 *   headA = headA->next (move to 6)
 *   headB = headB->next (move to 6)
 *
 * ----------------------------------------------------------------------------
 * ITERATION 3:
 * ----------------------------------------------------------------------------
 *   headA points to [6], headB points to [6]
 *   headA == headB (SAME NODE! Same memory address!)
 *
 *   LOOP EXITS
 *
 *   Return headA = [6] (the intersection point!)
 *
 * ============================================================================
 * WHY IT WORKS:
 * ============================================================================
 *
 *   By aligning the starting points:
 *   - Both pointers have the same distance to the intersection
 *   - When we move both together, they reach intersection simultaneously
 *
 *   headA path: [1] --> [2] --> [6] (3 steps)
 *   headB path: [4] --> [5] --> [6] (3 steps)
 *                              ^^^
 *                         They meet here!
 *
 * ============================================================================
 * NO INTERSECTION CASE:
 * ============================================================================
 *
 *   List A: [1] --> [2] --> NULL
 *   List B: [3] --> [4] --> NULL
 *
 *   After alignment (same length, no advance needed):
 *
 *   Iteration 1: headA=[1], headB=[3] -> different
 *   Iteration 2: headA=[2], headB=[4] -> different
 *   Iteration 3: headA=NULL, headB=NULL -> SAME (both NULL)
 *
 *   Return NULL (no intersection)
 *
 * ============================================================================
 */

Node* find_intersection(Node* headA, Node* headB) {
    // Say: "First, calculate lengths of both lists"
    int lenA = get_length(headA);
    int lenB = get_length(headB);

    // Say: "Align the starting points by advancing the longer list"
    while (lenA > lenB) {
        headA = headA->next;
        lenA--;
    }
    while (lenB > lenA) {
        headB = headB->next;
        lenB--;
    }

    // Say: "Now traverse both together until they meet"
    while (headA != headB) {
        headA = headA->next;
        headB = headB->next;
    }

    // Say: "headA is intersection point (or NULL if no intersection)"
    return headA;
}

// Alternative: Two-pointer cycle method (elegant, same complexity)
Node* find_intersection_elegant(Node* headA, Node* headB) {
    // Say: "Two pointers, switch lists when reaching end"
    if (headA == NULL || headB == NULL) {
        return NULL;
    }

    Node* ptrA = headA;
    Node* ptrB = headB;

    // Say: "Each pointer traverses both lists - they meet at intersection"
    while (ptrA != ptrB) {
        // When reaching end, switch to other list's head
        ptrA = (ptrA == NULL) ? headB : ptrA->next;
        ptrB = (ptrB == NULL) ? headA : ptrB->next;
    }

    return ptrA;
}

/* ==================== TEST ==================== */

int main() {
    // Create intersecting lists:
    // List A: 1 -> 2 \
    //                  -> 6 -> 7 -> 8
    // List B: 3 -> 4 -> 5 /

    // Common part
    Node* common = create_node(6);
    common->next = create_node(7);
    common->next->next = create_node(8);

    // List A: 1 -> 2 -> common
    Node* headA = create_node(1);
    headA->next = create_node(2);
    headA->next->next = common;

    // List B: 3 -> 4 -> 5 -> common
    Node* headB = create_node(3);
    headB->next = create_node(4);
    headB->next->next = create_node(5);
    headB->next->next->next = common;

    printf("List A: 1 -> 2 -> 6 -> 7 -> 8\n");
    printf("List B: 3 -> 4 -> 5 -> 6 -> 7 -> 8\n");
    printf("(Intersection at node 6)\n\n");

    Node* intersection = find_intersection(headA, headB);
    if (intersection) {
        printf("Method 1 (Length): Intersection at node %d\n", intersection->data);
    } else {
        printf("Method 1 (Length): No intersection\n");
    }

    intersection = find_intersection_elegant(headA, headB);
    if (intersection) {
        printf("Method 2 (Elegant): Intersection at node %d\n", intersection->data);
    } else {
        printf("Method 2 (Elegant): No intersection\n");
    }

    // Test with no intersection
    Node* listC = create_node(1);
    listC->next = create_node(2);

    Node* listD = create_node(3);
    listD->next = create_node(4);

    printf("\nList C: ");
    print_list(listC);
    printf("List D: ");
    print_list(listD);

    intersection = find_intersection(listC, listD);
    printf("Intersection: %s\n", intersection ? "Found" : "None");

    // Test with same head (entire list is intersection)
    printf("\nLists with same head:\n");
    intersection = find_intersection(headA, headA);
    if (intersection) {
        printf("Intersection at node %d (head itself)\n", intersection->data);
    }

    return 0;
}

/*
 * ==================== INTERVIEW EXPLANATION ====================
 *
 * PROBLEM: Find where two linked lists intersect.
 *
 * APPROACH 1 - Length Alignment:
 *
 * Visual:
 *   A: 1 -> 2 ----\
 *                  -> 6 -> 7 -> 8
 *   B: 3 -> 4 -> 5 /
 *
 *   Length A = 5, Length B = 6
 *   Difference = 1
 *
 *   Advance B by 1: start at 4
 *   Now both have 4 nodes to traverse to reach intersection
 *
 * Steps:
 * 1. Calculate length of both lists
 * 2. Advance longer list's pointer by difference
 * 3. Move both pointers together until they meet
 *
 * APPROACH 2 - Two Pointer Cycle (Elegant):
 *
 * Key insight: If ptrA traverses A then B, and ptrB traverses B then A,
 * they travel the same total distance and meet at intersection.
 *
 * Why it works:
 * - ptrA travels: lenA + lenB (including common part)
 * - ptrB travels: lenB + lenA (including common part)
 * - They align at intersection point
 *
 * If no intersection:
 * - Both reach NULL simultaneously (after traversing both lists)
 *
 * WHY THESE WORK:
 * - Both ensure pointers have equal remaining distance to intersection
 * - Method 1: Explicit length calculation
 * - Method 2: Implicit via list switching
 *
 * EDGE CASES:
 * 1. No intersection -> Return NULL
 * 2. One or both lists empty -> Return NULL
 * 3. Lists of same length -> Direct comparison works
 * 4. Intersection at head -> Return head
 * 5. Intersection at last node only
 *
 * HASH SET ALTERNATIVE (O(n) space):
 * - Store all nodes of list A in hash set
 * - Traverse list B, check if node in set
 * - First match is intersection
 *
 * COMMON MISTAKES:
 * - Comparing values instead of node addresses
 * - Forgetting to handle NULL lists
 * - Infinite loop if lists don't intersect (method 2 needs careful handling)
 *
 * INTERVIEW TIP:
 * - Clarify: "We're comparing node addresses, not values, right?"
 * - Intersection means same physical node, not same value
 *
 * FOLLOW-UP: What if lists might have cycles?
 * - Need to detect and handle cycles first
 * - Much more complex problem
 */
