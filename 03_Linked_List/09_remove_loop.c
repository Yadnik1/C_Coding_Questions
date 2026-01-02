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
    if (head == NULL || head->next == NULL) return;

    Node* slow = head;
    Node* fast = head;

    // Step 1: Detect loop
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            break;  // Loop detected
        }
    }

    // No loop
    if (fast == NULL || fast->next == NULL) {
        return;
    }

    // Step 2: Find loop start
    // Special case: loop starts at head
    if (slow == head) {
        // Find the last node in the loop
        while (fast->next != slow) {
            fast = fast->next;
        }
        fast->next = NULL;
        return;
    }

    // General case: reset slow to head
    slow = head;
    while (slow->next != fast->next) {
        slow = slow->next;
        fast = fast->next;
    }

    // Step 3: Remove loop (fast->next is loop start)
    fast->next = NULL;
}

// Detect loop (returns 1 if loop exists)
int hasLoop(Node* head) {
    Node* slow = head;
    Node* fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return 1;
        }
    }

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
