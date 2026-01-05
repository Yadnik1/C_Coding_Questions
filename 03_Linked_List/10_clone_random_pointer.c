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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why not just use a hash map?"
 * A1: Hash map works but uses O(n) extra space:
 *
 *     map[original] = clone
 *
 *     For each node:
 *       clone->random = map[original->random]
 *
 *     Interleaving method uses O(1) extra space - better for embedded!
 *
 * -------------------------------------------------------------------------
 * Q2: "Why interleave instead of creating a separate list?"
 * A2: Interleaving gives us a O(1) way to find each clone:
 *
 *     original->next is ALWAYS the clone
 *     original->random->next is ALWAYS the clone of random
 *
 *     Without interleaving, we'd need a hash map to track mappings.
 *
 * -------------------------------------------------------------------------
 * Q3: "What's the key insight for setting random pointers?"
 * A3: clone->random = original->random->next
 *
 *     Because after interleaving:
 *     - Every original node is followed by its clone
 *     - So original->random (another original) is followed by ITS clone
 *     - That clone is what we want for clone->random!
 *
 * -------------------------------------------------------------------------
 * Q4: "What if random pointer is NULL?"
 * A4: We must check before dereferencing:
 *
 *     if (current->random != NULL) {
 *         current->next->random = current->random->next;
 *     }
 *     // else clone->random stays NULL (from initialization)
 *
 * -------------------------------------------------------------------------
 * Q5: "Is this problem common in embedded systems?"
 * A5: The TECHNIQUE is valuable! It demonstrates:
 *
 *     - In-place algorithms (O(1) space)
 *     - Pointer manipulation skills
 *     - Understanding of memory layout
 *
 *     Similar patterns used in: memory-mapped structures, DMA descriptor
 *     chains, interrupt vectors with callbacks.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * ============================================================================
 * NODE STRUCTURE - LINE BY LINE EXPLANATION (SPECIAL: WITH RANDOM POINTER)
 * ============================================================================
 *
 * typedef struct Node {
 *     int data;            // PAYLOAD: The actual value stored
 *     struct Node* next;   // LINK: Pointer to the next node
 *     struct Node* random; // RANDOM: Pointer to any arbitrary node (or NULL)
 * } Node;
 *
 * This is a SPECIAL node structure with an additional "random" pointer:
 * - "int data" stores the value
 * - "struct Node* next" points to the next node (standard linked list)
 * - "struct Node* random" can point to ANY node in the list (or NULL)
 *   This is what makes cloning this list tricky!
 *
 * MEMORY LAYOUT:
 *   +------+------+--------+
 *   | data | next | random |---> (any node or NULL)
 *   +------+------+--------+
 *
 * ============================================================================
 */
typedef struct Node {
    int data;            // The data field stores the actual value (payload)
    struct Node* next;   // Pointer to the next node in the list (or NULL)
    struct Node* random; // Pointer to any arbitrary node (or NULL)
} Node;

/*
 * ============================================================================
 * CREATE NODE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * malloc(sizeof(Node)): Allocates memory on HEAP for one Node
 * node->data = data:    Stores the parameter value using -> operator
 * node->next = NULL:    Initializes next pointer (CRITICAL to avoid bugs)
 * node->random = NULL:  Initializes random pointer (special for this problem)
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

    // Initialize random pointer to NULL (special for this problem)
    // Say: "I also set random to NULL - it will be set later if needed"
    node->random = NULL;

    // Return pointer to the new node
    // Say: "I return the pointer to the newly created node"
    return node;
}

/*
 * ============================================================================
 * CLONE LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* cloneList(Node* head):
 *   - Returns "Node*" = pointer to head of the cloned list
 *   - "Node* head" = pointer to the original list with random pointers
 *   - Purpose: Create a DEEP COPY of the list, including random pointers
 *
 * THE CHALLENGE:
 * ---------------------------------
 * Why is this problem HARD?
 * - Simple cloning: just copy data and set next pointers
 * - But random pointers point to nodes that DON'T EXIST YET in the clone!
 * - How do we know which clone node corresponds to which original?
 *
 * TWO SOLUTIONS:
 * ---------------------------------
 * 1. Hash Map Approach (O(n) space):
 *    - map[original] = clone
 *    - clone->random = map[original->random]
 *    Easy but uses extra space!
 *
 * 2. Interleaving Approach (O(1) space) - WHAT WE USE:
 *    - Insert clones right after originals
 *    - Use position to find corresponding clone
 *    - Separate lists at the end
 *
 * ALGORITHM: THREE-STEP INTERLEAVING
 * ---------------------------------
 *
 * STEP 1: INTERLEAVE - Insert clone after each original
 *   Original: A → B → C → NULL
 *   After:    A → A' → B → B' → C → C' → NULL
 *
 *   Now every original's clone is just original->next!
 *
 * STEP 2: SET RANDOM POINTERS
 *   Key insight: clone->random = original->random->next
 *
 *   Why? Because original->random is an original node,
 *   and that original's clone is at original->random->next!
 *
 *   If original->random = B, then clone->random = B' = B->next
 *
 * STEP 3: SEPARATE - Restore both lists
 *   Interleaved: A → A' → B → B' → C → C' → NULL
 *   Original:    A → B → C → NULL (restored)
 *   Clone:       A' → B' → C' → NULL (extracted)
 *
 * VISUALIZATION:
 * ---------------------------------
 * Original: 1 → 2 → 3 → NULL
 * Random:   1→3, 2→1, 3→2
 *
 * After Step 1 (Interleave):
 *   1 → 1' → 2 → 2' → 3 → 3' → NULL
 *
 * After Step 2 (Set random):
 *   1'.random = 1.random.next = 3.next = 3'  ✓
 *   2'.random = 2.random.next = 1.next = 1'  ✓
 *   3'.random = 3.random.next = 2.next = 2'  ✓
 *
 * After Step 3 (Separate):
 *   Original: 1 → 2 → 3 → NULL (restored!)
 *   Clone:    1' → 2' → 3' → NULL (with correct random pointers!)
 *
 * WHY IS THIS O(1) SPACE?
 * ---------------------------------
 * We don't use a hash map or any auxiliary data structure.
 * We only use the list's own structure (interleaving) to track mappings.
 * The only "extra" space is the clone nodes themselves (required output).
 *
 * TIME COMPLEXITY: O(n)
 * ---------------------------------
 * - Step 1: O(n) - traverse and insert clones
 * - Step 2: O(n) - traverse and set random pointers
 * - Step 3: O(n) - traverse and separate
 * - Total: O(3n) = O(n)
 *
 * SPACE COMPLEXITY: O(1) extra
 * ---------------------------------
 * - Only a few pointers regardless of list size
 * - Clone nodes are required output, not "extra" space
 *
 * ============================================================================
 */
// Clone a list with random pointers using O(1) space interleaving method
// Say: "I'll clone this list using the three-step interleaving approach"
Node* cloneList(Node* head) {
    // =========================================================================
    // EDGE CASE: Empty list has no clone
    // =========================================================================
    // Say: "First I check if the list is empty"
    // WHY check? Can't clone an empty list
    if (head == NULL) return NULL;

    // Declare pointer for traversal (reused across all steps)
    Node* current;

    // =========================================================================
    // STEP 1: INTERLEAVE - Insert clone after each original node
    // =========================================================================
    // Say: "Step 1: I create clones and insert them right after each original"
    // This transforms: A → B → C → NULL
    //            into: A → A' → B → B' → C → C' → NULL
    current = head;

    // Traverse and create clones
    // Say: "For each node, I create a clone and insert it immediately after"
    while (current != NULL) {
        // Create clone with same data value
        // Say: "I create a clone with the same data value"
        // WHY createNode? It allocates memory and initializes next/random to NULL
        Node* clone = createNode(current->data);

        // Insert clone between current and current's next
        // Say: "I insert the clone between current and its next"
        // WHY this order? Clone needs to point to what current pointed to
        clone->next = current->next;

        // Link current to its clone
        // Say: "I link current to its clone"
        // WHY? Now current->next IS the clone (key for Step 2!)
        current->next = clone;

        // Move to next ORIGINAL node (skip the clone we just inserted)
        // Say: "I move to the next original node, skipping the clone"
        // WHY clone->next? That's where the next original is now
        current = clone->next;
    }
    // After this loop: A → A' → B → B' → C → C' → NULL

    // =========================================================================
    // STEP 2: SET RANDOM POINTERS for cloned nodes
    // =========================================================================
    // Say: "Step 2: I set up the random pointers for all cloned nodes"
    // Key insight: clone->random = original->random->next
    current = head;

    // Traverse and set random pointers
    // Say: "For each original, I set its clone's random pointer"
    while (current != NULL) {
        // Only set random if original has a random pointer
        // Say: "If the original has a random pointer, I set the clone's random"
        // WHY check? If random is NULL, clone's random stays NULL (from createNode)
        if (current->random != NULL) {
            // THE KEY LINE: clone->random = original->random->next
            // Say: "Clone's random equals the clone of original's random target"
            // WHY this works?
            //   - current->next is current's clone
            //   - current->random is some original node X
            //   - current->random->next is X's clone (X')
            //   - So clone's random should point to X' ✓
            current->next->random = current->random->next;
        }

        // Move to next ORIGINAL node (every other node)
        // Say: "I move to the next original node"
        // WHY current->next->next? Skip the clone to get to next original
        current = current->next->next;
    }

    // =========================================================================
    // STEP 3: SEPARATE - Extract clone list and restore original list
    // =========================================================================
    // Say: "Step 3: I separate the interleaved list into two separate lists"

    // Save clone head (needed for return)
    // Say: "I save the head of the cloned list"
    // WHY head->next? First clone is right after first original
    Node* cloneHead = head->next;

    // Reset current to head for separation
    current = head;

    // Pointer for clone traversal
    Node* cloneCurrent;

    // Separate the lists
    // Say: "I restore the original list and extract the cloned list"
    while (current != NULL) {
        // Get clone of current (it's right after current)
        // Say: "I get the clone of the current node"
        cloneCurrent = current->next;

        // Restore original's next (skip the clone)
        // Say: "I restore original's next pointer to skip over the clone"
        // WHY cloneCurrent->next? That's the next original (or NULL)
        current->next = cloneCurrent->next;

        // Set clone's next to the next clone (if it exists)
        // Say: "I set clone's next to point to the next clone"
        // WHY check? If cloneCurrent->next is NULL, there's no next clone
        if (cloneCurrent->next != NULL) {
            // WHY cloneCurrent->next->next? Next original's clone is after it
            cloneCurrent->next = cloneCurrent->next->next;
        }

        // Move to next original (we just restored this link)
        // Say: "I move to the next original node"
        current = current->next;
    }
    // After this loop:
    //   Original: A → B → C → NULL (restored!)
    //   Clone:    A' → B' → C' → NULL (with correct random pointers!)

    // =========================================================================
    // RETURN: Head of the cloned list
    // =========================================================================
    // Say: "I return the head of the cloned list"
    // WHY cloneHead? We saved it before separating; it's the clone's head
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
 *   - Memory leaks can crash long-running programs
 *   - In interviews, mentioning memory cleanup shows professionalism
 *
 * NOTE FOR THIS PROBLEM:
 *   - We have BOTH original list and cloned list
 *   - Each must be freed separately (they don't share nodes)
 *   - The clone is a DEEP copy - separate memory allocations
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

    /*
     * ============================================================================
     * MEMORY CLEANUP - IMPORTANT FOR INTERVIEWS!
     * ============================================================================
     * We have TWO separate lists - original and clone.
     * Each must be freed independently since clone is a deep copy.
     * Say: "Finally, I free both the original and cloned lists"
     * ============================================================================
     */
    freeList(head);
    freeList(clone);

    printf("\n=== Memory freed successfully ===\n");

    return 0;
}
