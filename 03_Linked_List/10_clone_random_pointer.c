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
    if (head == NULL) return NULL;

    Node* current;

    // Step 1: Insert cloned nodes between original nodes
    // A → B → C becomes A → A' → B → B' → C → C'
    current = head;
    while (current != NULL) {
        Node* clone = createNode(current->data);
        clone->next = current->next;
        current->next = clone;
        current = clone->next;
    }

    // Step 2: Set random pointers for cloned nodes
    current = head;
    while (current != NULL) {
        if (current->random != NULL) {
            current->next->random = current->random->next;
        }
        current = current->next->next;
    }

    // Step 3: Separate the two lists
    Node* cloneHead = head->next;
    current = head;
    Node* cloneCurrent;

    while (current != NULL) {
        cloneCurrent = current->next;
        current->next = cloneCurrent->next;

        if (cloneCurrent->next != NULL) {
            cloneCurrent->next = cloneCurrent->next->next;
        }

        current = current->next;
    }

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
