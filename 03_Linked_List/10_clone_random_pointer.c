/*
 * ============================================================================
 * PROBLEM: Clone Linked List with Random Pointers
 * ============================================================================
 *
 * DIFFICULTY: Hard | TIME: 20 mins | FREQUENCY: MEDIUM
 *
 * Clone a linked list where each node has a next pointer AND a random pointer
 * that can point to any node in the list or NULL.
 *
 * ============================================================================
 * APPROACH: Interleaving Method (O(n) time, O(1) space)
 * ============================================================================
 *
 * Step 1: Insert cloned nodes between original nodes
 *         A → A' → B → B' → C → C'
 *
 * Step 2: Set random pointers for cloned nodes
 *         clone->random = original->random->next
 *
 * Step 3: Separate the two lists
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   Original:
 *   A → B → C → NULL
 *   ↓   ↓   ↓
 *   C   A   B   (random pointers)
 *
 *   Step 1 (Interleave):
 *   A → A' → B → B' → C → C' → NULL
 *
 *   Step 2 (Set random):
 *   A'.random = A.random.next = C.next = C'
 *   B'.random = B.random.next = A.next = A'
 *   C'.random = C.random.next = B.next = B'
 *
 *   Step 3 (Separate):
 *   Original: A → B → C → NULL
 *   Clone:    A' → B' → C' → NULL (with correct random pointers)
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1) extra (excluding output)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* random;
} Node;

Node* createNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    node->random = NULL;
    return node;
}

Node* cloneList(Node* head) {
    // Edge case: empty list
    // Say: "First check if the list is empty"
    if (head == NULL) return NULL;

    // Declare pointer for traversal
    Node* current;

    // Step 1: Insert cloned nodes between original nodes
    // Say: "Step 1: I'll create clones and interleave them with originals"
    // A → B → C becomes A → A' → B → B' → C → C'
    current = head;

    // Create and insert clones
    // Say: "For each node, create a clone and insert it right after"
    while (current != NULL) {
        // Create clone of current node
        // Say: "Create a clone with the same data"
        Node* clone = createNode(current->data);

        // Insert clone after current
        // Say: "Insert the clone between current and current's next"
        clone->next = current->next;

        // Link current to clone
        // Say: "Link current node to its clone"
        current->next = clone;

        // Move to next original node (skip the clone)
        // Say: "Move to the next original node"
        current = clone->next;
    }

    // Step 2: Set random pointers for cloned nodes
    // Say: "Step 2: Set up the random pointers for all cloned nodes"
    current = head;

    // Traverse and set random pointers
    // Say: "For each original node, set its clone's random pointer"
    while (current != NULL) {
        // If original has random pointer
        // Say: "If the original has a random pointer"
        if (current->random != NULL) {
            // Clone's random = original's random's clone
            // Say: "Set clone's random to the clone of original's random"
            current->next->random = current->random->next;
        }

        // Move to next original node
        // Say: "Move to the next original node"
        current = current->next->next;
    }

    // Step 3: Separate the two lists
    // Say: "Step 3: Separate the original and cloned lists"

    // Save clone head
    // Say: "Save the head of the cloned list"
    Node* cloneHead = head->next;

    // Reset current to head
    current = head;

    // Declare clone current
    Node* cloneCurrent;

    // Separate the lists
    // Say: "Restore the original list and build the cloned list"
    while (current != NULL) {
        // Get clone of current
        // Say: "Get the clone of the current node"
        cloneCurrent = current->next;

        // Restore original's next
        // Say: "Restore original's next pointer to skip the clone"
        current->next = cloneCurrent->next;

        // Set clone's next if not at end
        // Say: "Set clone's next to the next clone if it exists"
        if (cloneCurrent->next != NULL) {
            cloneCurrent->next = cloneCurrent->next->next;
        }

        // Move to next original
        // Say: "Move to next original node"
        current = current->next;
    }

    // Return cloned list
    // Say: "Return the head of the cloned list"
    return cloneHead;
}

// Print list with random pointers
void printList(Node* head) {
    Node* current = head;
    printf("List:\n");
    while (current != NULL) {
        printf("  Node %d: next=%s, random=%s\n",
               current->data,
               current->next ? (char[10]){0} : "NULL",
               current->random ? (char[10]){0} : "NULL");

        if (current->next) {
            printf("          next→%d", current->next->data);
        } else {
            printf("          next→NULL");
        }

        if (current->random) {
            printf(", random→%d\n", current->random->data);
        } else {
            printf(", random→NULL\n");
        }

        current = current->next;
    }
}

void printListSimple(Node* head, const char* label) {
    printf("%s: ", label);
    Node* current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->random) {
            printf("(r→%d)", current->random->data);
        } else {
            printf("(r→N)");
        }
        current = current->next;
        if (current) printf(" → ");
    }
    printf(" → NULL\n");
}

int main() {
    printf("=== Clone Linked List with Random Pointers ===\n\n");

    // Create list: 1 → 2 → 3 → 4 → 5
    Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);

    // Set random pointers
    head->random = head->next->next;           // 1 → 3
    head->next->random = head;                 // 2 → 1
    head->next->next->random = head->next->next->next->next;  // 3 → 5
    head->next->next->next->random = head->next->next;        // 4 → 3
    head->next->next->next->next->random = head->next;        // 5 → 2

    printf("Original:\n");
    printListSimple(head, "Original");
    printf("\n");

    // Clone
    Node* clone = cloneList(head);

    printf("After cloning:\n");
    printListSimple(head, "Original");
    printListSimple(clone, "Clone   ");
    printf("\n");

    // Verify they are independent
    printf("Verification (addresses different):\n");
    printf("  Original head: %p\n", (void*)head);
    printf("  Clone head:    %p\n", (void*)clone);

    printf("\n=== Algorithm ===\n");
    printf("1. Interleave: A → A' → B → B' → C → C'\n");
    printf("2. Set random: clone.random = orig.random.next\n");
    printf("3. Separate:   Restore both lists\n");
    printf("\nTime: O(n), Space: O(1) extra\n");

    return 0;
}
