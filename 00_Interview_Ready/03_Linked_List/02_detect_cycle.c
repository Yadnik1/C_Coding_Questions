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

bool has_cycle(Node* head) {
    // Say: "I use Floyd's algorithm with slow and fast pointers"
    if (head == NULL || head->next == NULL) {
        return false;
    }

    Node* slow = head;
    Node* fast = head;

    // Say: "Slow moves 1 step, fast moves 2 steps"
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;          // Move slow by 1
        fast = fast->next->next;    // Move fast by 2

        // Say: "If they meet, there's a cycle"
        if (slow == fast) {
            return true;
        }
    }

    // Say: "If fast reaches NULL, no cycle exists"
    return false;
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
