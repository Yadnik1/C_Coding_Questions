/*
 * ============================================================================
 * PROBLEM: Check if Linked List is Palindrome
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: High
 *
 * Example:
 * Input:  1 -> 2 -> 2 -> 1 -> NULL
 * Output: true (palindrome)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. APPROACH:
 *    - Find middle using slow/fast
 *    - Reverse second half
 *    - Compare both halves
 *
 * 2. COMBINES THREE CONCEPTS:
 *    - Find middle
 *    - Reverse list
 *    - Compare lists
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: 1 -> 2 -> 2 -> 1
 * ============================================================================
 *
 * Step 1: Find middle
 *   1 -> 2 -> 2 -> 1 -> NULL
 *        ^    ^
 *       slow fast
 *
 *   Middle = 2 (second node)
 *
 * Step 2: Reverse second half (from middle->next)
 *   First half:  1 -> 2
 *   Second half: 2 -> 1 → reversed → 1 -> 2
 *
 * Step 3: Compare
 *   First half:  1 -> 2
 *   Second half: 1 -> 2
 *   Match! → PALINDROME
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why not just use a stack to compare?"
 * A1: Stack approach works but uses O(n) space:
 *     - Push all elements to stack
 *     - Traverse again, compare with popped elements
 *
 *     Our approach: O(1) space by reversing in-place!
 *     ALWAYS mention: "For embedded, O(1) space is preferred"
 *
 * -------------------------------------------------------------------------
 * Q2: "Do you need to restore the list after checking?"
 * A2: Depends on requirements! To restore:
 *     - After comparison, reverse the second half again
 *     - Reconnect to first half
 *
 *     In interviews, ASK: "Should I restore the original list?"
 *
 * -------------------------------------------------------------------------
 * Q3: "How does this handle odd-length lists?"
 * A3: Works perfectly! For 1 -> 2 -> 3 -> 2 -> 1:
 *     - Middle is 3
 *     - First half: 1 -> 2
 *     - Second half: 2 -> 1 (reversed)
 *     - Middle element (3) is ignored - palindromes ignore center!
 *
 * -------------------------------------------------------------------------
 * Q4: "What's the trick to finding the correct middle?"
 * A4: For palindrome check, we want slow to stop at:
 *     - Odd length: exactly middle (skip it)
 *     - Even length: end of first half
 *
 *     Use: while (fast->next && fast->next->next)
 *     This makes slow stop at the END of first half.
 *
 * -------------------------------------------------------------------------
 * Q5: "Can you do this recursively?"
 * A5: Yes, but it's tricky and uses O(n) stack space:
 *
 *     Node* left;
 *     bool check(Node* right) {
 *         if (right == NULL) return true;
 *         if (!check(right->next)) return false;
 *         if (left->data != right->data) return false;
 *         left = left->next;
 *         return true;
 *     }
 *
 *     Not recommended for embedded (stack overflow risk)!
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

/*
 * ============================================================================
 * REVERSE LIST HELPER FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* reverseList(Node* head):
 *   - Returns "Node*" = pointer to the new head (was tail)
 *   - "Node* head" = pointer to first node of the list to reverse
 *   - Purpose: Reverse the linked list in-place
 *   - Used by isPalindrome() to reverse the second half for comparison
 *
 * ALGORITHM: Three-Pointer Iterative Reversal
 * ---------------------------------
 * We use three pointers:
 *   - prev: Points to the previous node (starts NULL)
 *   - head: Current node being processed
 *   - next: Saves the next node before we break the link
 *
 * THE FOUR-STEP DANCE (for each iteration):
 *   1. SAVE: next = head->next (save before breaking link)
 *   2. REVERSE: head->next = prev (reverse the arrow)
 *   3. ADVANCE prev: prev = head (move prev forward)
 *   4. ADVANCE head: head = next (move to next node)
 *
 * VISUALIZATION:
 * ---------------------------------
 * Original: 1 -> 2 -> 3 -> NULL
 *
 * Start: prev=NULL, head=1
 *
 * Step 1: next=2, 1->NULL, prev=1, head=2
 *   NULL <- 1    2 -> 3 -> NULL
 *          prev  head
 *
 * Step 2: next=3, 2->1, prev=2, head=3
 *   NULL <- 1 <- 2    3 -> NULL
 *               prev  head
 *
 * Step 3: next=NULL, 3->2, prev=3, head=NULL
 *   NULL <- 1 <- 2 <- 3
 *                    prev
 *
 * Return prev (now head of reversed list): 3 -> 2 -> 1 -> NULL
 *
 * TIME COMPLEXITY: O(n)
 * ---------------------------------
 * - Each node visited exactly once
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 * - Only three pointers regardless of list size
 *
 * ============================================================================
 */
// Reverse a linked list in-place (helper for palindrome check)
// Say: "I'll reverse the list using the three-pointer technique"
Node* reverseList(Node* head) {
    // =========================================================================
    // INITIALIZE: Previous pointer starts at NULL (will be new tail)
    // =========================================================================
    // Say: "I initialize prev to NULL - it will become the new tail"
    // WHY NULL? The first node of original list becomes last node of reversed
    // The last node's next should be NULL
    Node* prev = NULL;

    // =========================================================================
    // MAIN LOOP: Process each node (the four-step dance)
    // =========================================================================
    // Say: "I iterate through and reverse each link"
    // WHY "while (head)"? Continue until we've processed all nodes
    while (head) {
        // STEP 1: SAVE - Save next node before we break the forward link
        // Say: "I save the next node before I modify the current pointer"
        // WHY save first? After head->next = prev, we lose access to the rest!
        Node* next = head->next;

        // STEP 2: REVERSE - Point current node backward
        // Say: "I reverse the link by pointing current back to prev"
        // WHY this? This is the actual reversal - arrow now points backward
        head->next = prev;

        // STEP 3: ADVANCE PREV - Move prev to current position
        // Say: "I move prev forward to the current node"
        // WHY? Next iteration, current node will be the "previous" node
        prev = head;

        // STEP 4: ADVANCE HEAD - Move to next node using saved pointer
        // Say: "I move to the next node using the saved pointer"
        // WHY saved pointer? head->next now points backward, can't use it!
        head = next;
    }

    // =========================================================================
    // RETURN: prev is now the new head (was the last node)
    // =========================================================================
    // Say: "I return prev, which is now the head of the reversed list"
    // WHY prev? When head becomes NULL, prev points to the last processed node
    // That last node is the first node of reversed list (new head)
    return prev;
}

/*
 * ============================================================================
 * IS PALINDROME FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * bool isPalindrome(Node* head):
 *   - Returns "bool" = true if list is palindrome, false otherwise
 *   - "Node* head" = pointer to the first node of the list
 *   - Purpose: Check if the list reads the same forwards and backwards
 *
 * ALGORITHM: Three-Step Approach
 * ---------------------------------
 * 1. Find the middle of the list (slow/fast pointers)
 * 2. Reverse the second half
 * 3. Compare first half with reversed second half
 *
 * This combines THREE fundamental linked list operations!
 *
 * WHY THIS APPROACH?
 * ---------------------------------
 * Alternative 1: Use a stack - O(n) space (push all, then compare)
 * Alternative 2: Convert to array - O(n) space
 * Alternative 3: Recursive - O(n) stack space
 *
 * Our approach: O(1) space by reversing in-place!
 * For embedded systems, O(1) space is critical.
 *
 * WHY USE "fast->next && fast->next->next" CONDITION?
 * ---------------------------------
 * This specific condition makes slow stop at the END of the first half:
 *
 * For EVEN length (1->2->3->4):
 *   Initial: slow=1, fast=1
 *   Step 1:  slow=2, fast=3
 *   fast->next=4 (exists), fast->next->next=NULL (fail) → STOP
 *   slow at 2 (end of first half) ✓
 *
 * For ODD length (1->2->3->2->1):
 *   Initial: slow=1, fast=1
 *   Step 1:  slow=2, fast=3
 *   Step 2:  slow=3, fast=1
 *   fast->next=NULL (fail) → STOP
 *   slow at 3 (middle, which we skip) ✓
 *
 * VISUALIZATION - EVEN LENGTH:
 * ---------------------------------
 * List: 1 -> 2 -> 2 -> 1 -> NULL
 *
 * After finding middle (slow at 2):
 *   First half:  1 -> 2
 *   Second half: 2 -> 1
 *
 * After reversing second half:
 *   First half:  1 -> 2
 *   Second half: 1 -> 2
 *
 * Compare: 1==1, 2==2 → PALINDROME!
 *
 * VISUALIZATION - ODD LENGTH:
 * ---------------------------------
 * List: 1 -> 2 -> 3 -> 2 -> 1 -> NULL
 *
 * After finding middle (slow at 3):
 *   First half:  1 -> 2 (stop before middle)
 *   Second half: 2 -> 1 (after middle)
 *   Middle (3) is ignored - palindromes don't care about center!
 *
 * After reversing second half:
 *   First half:  1 -> 2
 *   Second half: 1 -> 2
 *
 * Compare: 1==1, 2==2 → PALINDROME!
 *
 * IMPORTANT NOTE:
 * ---------------------------------
 * This function MODIFIES the list (reverses second half).
 * If you need to preserve the original list, reverse again after comparison.
 *
 * TIME COMPLEXITY: O(n)
 * ---------------------------------
 * - Find middle: O(n/2)
 * - Reverse: O(n/2)
 * - Compare: O(n/2)
 * - Total: O(1.5n) = O(n)
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 * - Only a few pointers regardless of list size
 * - No stack, no array, no extra data structures!
 *
 * ============================================================================
 */
// Check if linked list is a palindrome (reads same forwards and backwards)
// Say: "I'll check if it's a palindrome by comparing first half with reversed second half"
bool isPalindrome(Node* head) {
    // =========================================================================
    // EDGE CASE: Empty or single node is always a palindrome
    // =========================================================================
    // Say: "First I handle edge cases - empty or single node is a palindrome"
    // WHY? Empty list has nothing to compare, single node is trivially palindrome
    if (head == NULL || head->next == NULL) return true;

    // =========================================================================
    // STEP 1: FIND MIDDLE using slow/fast pointers
    // =========================================================================
    // Say: "Step 1: I find the middle using slow and fast pointers"
    Node* slow = head;  // Will end up at middle (or end of first half)
    Node* fast = head;  // Will reach end of list

    // Special loop condition for palindrome check
    // Say: "I move slow by 1 and fast by 2 until fast reaches near the end"
    // WHY this condition? Makes slow stop at the END of first half
    // Different from findMiddle which uses "fast && fast->next"
    while (fast->next && fast->next->next) {
        // Move slow pointer one step
        // Say: "I move slow one step forward"
        // WHY one step? Slow covers half the distance of fast
        slow = slow->next;

        // Move fast pointer two steps
        // Say: "I move fast two steps forward"
        // WHY two steps? When fast reaches end, slow is at middle
        fast = fast->next->next;
    }
    // After loop: slow points to end of first half (or middle for odd length)

    // =========================================================================
    // STEP 2: REVERSE THE SECOND HALF
    // =========================================================================
    // Say: "Step 2: I reverse the second half of the list"
    // WHY slow->next? We start reversing AFTER the middle point
    // This way, slow remains the connection point between halves
    Node* secondHalf = reverseList(slow->next);

    // Save pointer to first half (head never moved)
    // Say: "I keep a pointer to the first half starting from head"
    // WHY separate pointer? We need to traverse both halves simultaneously
    Node* firstHalf = head;

    // =========================================================================
    // STEP 3: COMPARE BOTH HALVES
    // =========================================================================
    // Say: "Step 3: I compare the first half with the reversed second half"
    // WHY secondHalf in condition? Second half is shorter (or equal) for odd lists
    // For even: both halves equal length
    // For odd: first half is 1 longer (includes middle), but we compare to shorter
    while (secondHalf) {
        // Check if current values match
        // Say: "I compare corresponding values from both halves"
        // WHY compare data? Checking if values are symmetric around middle
        if (firstHalf->data != secondHalf->data) {
            // Mismatch found - not a palindrome!
            // Say: "I found a mismatch, so it's not a palindrome"
            // WHY return immediately? One mismatch is enough to know it's not palindrome
            return false;
        }

        // Move to next node in first half
        // Say: "I move to the next node in the first half"
        firstHalf = firstHalf->next;

        // Move to next node in second half
        // Say: "I move to the next node in the second half"
        secondHalf = secondHalf->next;
    }

    // =========================================================================
    // ALL VALUES MATCHED - It's a palindrome!
    // =========================================================================
    // Say: "All values matched, so it's a palindrome"
    // WHY true? We compared all corresponding elements and found no mismatch
    return true;
}

/*
 * ============================================================================
 * PRINT LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void printList(Node* head):
 *   - "void" = Function doesn't return anything (just prints to console)
 *   - "Node* head" = Receives a COPY of the head pointer (pass by value)
 *   - We can safely modify this copy without affecting caller's pointer
 *
 * WHY USE head DIRECTLY (no separate traversal pointer)?
 * ---------------------------------
 * - "head" is a LOCAL COPY of the pointer passed in
 * - Modifying head inside the function does NOT affect caller's pointer
 * - This is more concise - no need for extra "curr" variable
 * - Both approaches work; this is the cleaner style
 *
 * while (head != NULL):
 *   - Loop continues while head points to a valid node
 *   - When head becomes NULL, we've printed all nodes
 *   - NULL marks the end of a properly terminated linked list
 *
 * printf("%d", head->data):
 *   - Access the data field of the node head points to
 *   - %d prints the integer value
 *   - "->" is shorthand for (*head).data
 *
 * if (head->next) printf(" -> "):
 *   - Shorthand for "if (head->next != NULL)"
 *   - In C, non-NULL pointers evaluate to TRUE
 *   - Only print arrow if there's another node
 *   - Cleaner output: no trailing arrow after last node
 *
 * head = head->next:
 *   - Move to the next node in the list
 *   - This is the TRAVERSAL step
 *   - Original caller's head pointer is UNCHANGED
 *
 * printf(" -> NULL\n"):
 *   - Show that the list properly terminates
 *   - Visual indicator of list end
 *   - \n adds newline for clean output
 *
 * ============================================================================
 */
// Print list - traverse and print each node's data
// Say: "I'll traverse the list and print each node's value"
void printList(Node* head) {
    // Loop until we reach the end (NULL)
    // Say: "I loop while head is not NULL"
    // WHY use head directly? It's a local copy, won't affect caller
    while (head != NULL) {
        // Print current node's data
        // Say: "I print the current node's data"
        printf("%d", head->data);

        // Print arrow if there's a next node
        // Say: "If there's a next node, I print an arrow"
        if (head->next) printf(" -> ");

        // Move to next node
        // Say: "I advance head to the next node"
        head = head->next;
    }

    // Print NULL to show end of list
    // Say: "I print NULL to show the end of the list"
    printf(" -> NULL\n");
}

/*
 * ============================================================================
 * CREATE LIST FROM ARRAY - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Key insight: We need TWO pointers:
 *   - "head" stays at first node (to return it)
 *   - "curr" moves forward as we build
 *
 * If we only used head and moved it, we'd lose the list start!
 *
 * ============================================================================
 */
// Create list from array
// Say: "I'll create a linked list from the given array"
Node* createList(int arr[], int n) {
    // Edge case: empty array
    // Say: "First, I check if the array is empty"
    if (n == 0) return NULL;

    // Create head with first element
    // Say: "I create the head node with the first element"
    Node* head = createNode(arr[0]);

    // curr tracks where to add next node
    // Say: "I use current to track the end of the list"
    Node* curr = head;

    // Add remaining elements
    // Say: "I loop through remaining elements"
    for (int i = 1; i < n; i++) {
        // Create and link
        // Say: "I create a new node and link it"
        curr->next = createNode(arr[i]);

        // Move forward
        // Say: "I move current forward"
        curr = curr->next;
    }

    // Return head (never moved!)
    // Say: "I return head to access the list"
    return head;
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
 * NOTE FOR PALINDROME CHECK:
 *   - isPalindrome() modifies the list (reverses second half)
 *   - The original list structure is changed after the check
 *   - We can still free all nodes, but they're in a different order
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
    printf("=== Palindrome Linked List ===\n\n");

    int arr1[] = {1, 2, 2, 1};
    Node* list1 = createList(arr1, 4);
    printf("List: ");
    printList(list1);
    printf("Palindrome: %s\n\n", isPalindrome(list1) ? "YES" : "NO");

    int arr2[] = {1, 2, 3, 2, 1};
    Node* list2 = createList(arr2, 5);
    printf("List: ");
    printList(list2);
    printf("Palindrome: %s\n\n", isPalindrome(list2) ? "YES" : "NO");

    int arr3[] = {1, 2, 3};
    Node* list3 = createList(arr3, 3);
    printf("List: ");
    printList(list3);
    printf("Palindrome: %s\n", isPalindrome(list3) ? "YES" : "NO");

    /*
     * ============================================================================
     * MEMORY CLEANUP - IMPORTANT FOR INTERVIEWS!
     * ============================================================================
     * NOTE: isPalindrome() modifies the list structure (reverses second half).
     * The nodes are still allocated and need to be freed.
     * Say: "Finally, I free all the lists to prevent memory leaks"
     * ============================================================================
     */
    freeList(list1);
    freeList(list2);
    freeList(list3);

    printf("\n=== Memory freed successfully ===\n");

    return 0;
}
