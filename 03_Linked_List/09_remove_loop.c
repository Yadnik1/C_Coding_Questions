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

// Detect and remove loop
void removeLoop(Node* head) {
    // Edge case: empty or single node
    // Say: "First check if the list is empty or has only one node"
    if (head == NULL || head->next == NULL) return;

    // Initialize pointers for detection
    // Say: "Step 1: Detect if a loop exists using Floyd's algorithm"
    Node* slow = head;
    Node* fast = head;

    // Phase 1: Detect loop
    // Say: "Move slow by 1 and fast by 2 to detect the loop"
    while (fast != NULL && fast->next != NULL) {
        // Move slow one step
        // Say: "Move slow one step forward"
        slow = slow->next;

        // Move fast two steps
        // Say: "Move fast two steps forward"
        fast = fast->next->next;

        // Check if they met
        // Say: "If they meet, we've detected a loop"
        if (slow == fast) {
            break;  // Loop detected
        }
    }

    // No loop found
    // Say: "If fast reached the end, there's no loop to remove"
    if (fast == NULL || fast->next == NULL) {
        return;
    }

    // Special case: loop starts at head
    // Say: "Check special case where the loop starts at the head"
    if (slow == head) {
        // Find the last node in the loop
        // Say: "Find the last node in the loop that points back to head"
        while (fast->next != slow) {
            fast = fast->next;
        }

        // Break the loop
        // Say: "Break the loop by setting that node's next to NULL"
        fast->next = NULL;
        return;
    }

    // General case: reset slow to head
    // Say: "Step 2: Find where the loop starts by resetting slow to head"
    slow = head;

    // Move both at same speed until they're about to meet
    // Say: "Move both pointers one step at a time until they're about to meet"
    while (slow->next != fast->next) {
        // Move slow one step
        // Say: "Move slow forward one step"
        slow = slow->next;

        // Move fast one step
        // Say: "Move fast forward one step"
        fast = fast->next;
    }

    // Remove loop - fast->next is loop start
    // Say: "Step 3: Break the loop by setting the last node's next to NULL"
    fast->next = NULL;
}

// Detect loop (returns 1 if loop exists)
int hasLoop(Node* head) {
    // Initialize pointers
    // Say: "Use slow and fast pointers to detect a loop"
    Node* slow = head;
    Node* fast = head;

    // Traverse with two speeds
    // Say: "Move slow by 1 and fast by 2"
    while (fast != NULL && fast->next != NULL) {
        // Move slow one step
        slow = slow->next;

        // Move fast two steps
        fast = fast->next->next;

        // Check for meeting point
        // Say: "If they meet, a loop exists"
        if (slow == fast) {
            return 1;
        }
    }

    // No loop
    // Say: "If fast reaches end, no loop exists"
    return 0;
}

// Print list (careful with loops!)
void printList(Node* head, int maxNodes) {
    Node* current = head;
    int count = 0;

    printf("List: ");
    while (current != NULL && count < maxNodes) {
        printf("%d", current->data);
        current = current->next;
        count++;
        if (current != NULL && count < maxNodes) {
            printf(" → ");
        }
    }
    if (count >= maxNodes && current != NULL) {
        printf(" → ... (truncated)");
    } else {
        printf(" → NULL");
    }
    printf("\n");
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

    // Free memory
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    printf("=== Algorithm ===\n");
    printf("1. Detect loop with Floyd's (slow/fast)\n");
    printf("2. Reset slow to head\n");
    printf("3. Move both by 1 until slow->next == fast->next\n");
    printf("4. Set fast->next = NULL\n");

    return 0;
}
