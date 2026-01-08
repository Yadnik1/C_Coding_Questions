/*
 * ============================================================================
 * PROBLEM: Check if a Linked List is a Palindrome
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given the head of a singly linked list, determine if it is a palindrome.
 * A palindrome reads the same forward and backward.
 *
 * Unlike arrays/strings, we can't easily access the end of a linked list
 * or traverse backwards. This makes the O(1) space solution non-trivial!
 *
 * EXAMPLES:
 *   Input:  1 -> 2 -> 3 -> 2 -> 1
 *   Output: true (same forwards and backwards)
 *
 *   Input:  1 -> 2 -> 2 -> 1
 *   Output: true (even length palindrome)
 *
 *   Input:  1 -> 2 -> 3 -> 4
 *   Output: false (not a palindrome)
 *
 *   Input:  1
 *   Output: true (single node is palindrome)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 *   - Combines multiple linked list techniques in one problem
 *   - Tests: find middle, reverse list, compare lists
 *   - Shows understanding of space-time tradeoffs
 *   - Demonstrates ability to modify and restore data structures
 *   - Common follow-up: "Can you do O(1) space?"
 *
 * KEY CONCEPT: Three-Step Process
 *   1. FIND MIDDLE: Use slow/fast pointers
 *   2. REVERSE second half of the list
 *   3. COMPARE first half with reversed second half
 *   (Optional: Restore the list to original state)
 *
 * VISUAL:
 *
 *   Input: 1 -> 2 -> 3 -> 2 -> 1
 *
 *   Step 1: Find middle (slow/fast pointer)
 *   1 -> 2 -> 3 -> 2 -> 1
 *             ^
 *           slow (middle)
 *
 *   Step 2: Reverse second half (after middle)
 *   First half:  1 -> 2 -> 3
 *   Second half: 1 -> 2  (was 2 -> 1, now reversed!)
 *
 *   Step 3: Compare node by node
 *   first:  1 -> 2 -> 3...
 *   second: 1 -> 2 -> NULL
 *
 *   Compare: 1==1, 2==2, second ends -> PALINDROME!
 *
 *   Even length example: 1 -> 2 -> 2 -> 1
 *
 *   Step 1: Find middle
 *   1 -> 2 -> 2 -> 1
 *        ^
 *      slow (first middle)
 *
 *   Step 2: Reverse from slow->next
 *   First:  1 -> 2
 *   Second: 1 -> 2 (was 2 -> 1)
 *
 *   Step 3: Compare: 1==1, 2==2 -> PALINDROME!
 *
 * ALTERNATIVE APPROACHES:
 *   - Stack: Push first half, pop and compare with second (O(n) space)
 *   - Recursion: Compare first with last recursively (O(n) space for stack)
 *   - Copy to array: Two-pointer comparison (O(n) space)
 *
 * ============================================================================
 */

/*
 * PALINDROME LINKED LIST - Reverse Second Half and Compare
 *
 * Time Complexity: O(n) - Three passes: find middle, reverse, compare
 * Space Complexity: O(1) - In-place reversal
 *
 * Pattern: Slow/Fast + Reverse + Compare
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

// Helper: Reverse a linked list
Node* reverse_list(Node* head) {
    Node* prev = NULL;
    Node* curr = head;
    while (curr != NULL) {
        Node* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

/*
 * ============================================================================
 * DRY-RUN DIAGRAM: Check if Linked List is Palindrome
 * ============================================================================
 *
 * EXAMPLE: 1 -> 2 -> 3 -> 2 -> 1 (odd length palindrome)
 *
 * ============================================================================
 * STEP 1: Find Middle using slow/fast pointers
 * ============================================================================
 *
 * INITIAL STATE:
 *
 *   slow, fast
 *       |
 *       v
 *      [1] --> [2] --> [3] --> [2] --> [1] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 1:
 * ----------------------------------------------------------------------------
 *   Check: fast->next != NULL (true) && fast->next->next != NULL (true)
 *   slow = slow->next (move to 2)
 *   fast = fast->next->next (move to 3)
 *
 *              slow          fast
 *               |             |
 *               v             v
 *      [1] --> [2] --> [3] --> [2] --> [1] --> NULL
 *
 * ----------------------------------------------------------------------------
 * ITERATION 2:
 * ----------------------------------------------------------------------------
 *   Check: fast->next != NULL (true) && fast->next->next != NULL (false)
 *   LOOP EXITS
 *
 *   slow is at [2] (first middle for odd length)
 *
 *              slow
 *               |
 *               v
 *      [1] --> [2] --> [3] --> [2] --> [1] --> NULL
 *
 * ============================================================================
 * STEP 2: Reverse second half (after slow)
 * ============================================================================
 *
 *   second_half = reverse_list(slow->next) = reverse_list([3])
 *
 *   Before reverse:  [3] --> [2] --> [1] --> NULL
 *   After reverse:   [1] --> [2] --> [3] --> NULL
 *
 *   Now we have:
 *   First half:  [1] --> [2] --> [3]  (slow->next now broken)
 *   Second half: [1] --> [2] --> [3] --> NULL (reversed!)
 *
 * ============================================================================
 * STEP 3: Compare first and second halves
 * ============================================================================
 *
 *   first_half = head = [1]
 *   second_ptr = second_half = [1] (reversed second half)
 *
 *   First half:  [1] --> [2] --> ...
 *   Second half: [1] --> [2] --> [3] --> NULL
 *
 * ----------------------------------------------------------------------------
 * Comparison 1:
 * ----------------------------------------------------------------------------
 *   first_half->data (1) == second_ptr->data (1) -> MATCH!
 *   first_half = first_half->next (move to 2)
 *   second_ptr = second_ptr->next (move to 2)
 *
 * ----------------------------------------------------------------------------
 * Comparison 2:
 * ----------------------------------------------------------------------------
 *   first_half->data (2) == second_ptr->data (2) -> MATCH!
 *   first_half = first_half->next (move to 3)
 *   second_ptr = second_ptr->next (move to 3)
 *
 * ----------------------------------------------------------------------------
 * Comparison 3:
 * ----------------------------------------------------------------------------
 *   second_ptr->next == NULL -> LOOP EXITS
 *
 *   All comparisons passed! result = true
 *
 * ============================================================================
 * STEP 4: Restore the list (optional but good practice)
 * ============================================================================
 *
 *   slow->next = reverse_list(second_half)
 *   Reverse [1] --> [2] --> [3] back to [3] --> [2] --> [1]
 *
 *   Restored: [1] --> [2] --> [3] --> [2] --> [1] --> NULL
 *
 *   Return true (IS PALINDROME!)
 *
 * ============================================================================
 * EXAMPLE 2: 1 -> 2 -> 3 -> 4 (NOT a palindrome)
 * ============================================================================
 *
 *   After finding middle and reversing:
 *   First half:  [1] --> [2] --> ...
 *   Second half: [4] --> [3] --> NULL
 *
 *   Comparison 1: 1 != 4 -> MISMATCH! result = false
 *
 *   Return false (NOT PALINDROME)
 *
 * ============================================================================
 */

bool is_palindrome(Node* head) {
    // Say: "Empty or single node is a palindrome"
    if (head == NULL || head->next == NULL) {
        return true;
    }

    // Say: "Step 1: Find middle using slow/fast pointers"
    Node* slow = head;
    Node* fast = head;

    while (fast->next != NULL && fast->next->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    // Say: "slow is now at first middle (for even) or exact middle (for odd)"

    // Say: "Step 2: Reverse the second half"
    Node* second_half = reverse_list(slow->next);

    // Say: "Step 3: Compare first and second halves"
    Node* first_half = head;
    Node* second_ptr = second_half;
    bool result = true;

    while (second_ptr != NULL) {
        if (first_half->data != second_ptr->data) {
            result = false;
            break;
        }
        first_half = first_half->next;
        second_ptr = second_ptr->next;
    }

    // Say: "Step 4: Restore the list (good practice, optional)"
    slow->next = reverse_list(second_half);

    return result;
}

/* ==================== TEST ==================== */

int main() {
    // Test case 1: Odd length palindrome
    // 1 -> 2 -> 3 -> 2 -> 1
    Node* list1 = create_node(1);
    list1->next = create_node(2);
    list1->next->next = create_node(3);
    list1->next->next->next = create_node(2);
    list1->next->next->next->next = create_node(1);

    printf("List: ");
    print_list(list1);
    printf("Is palindrome: %s\n\n", is_palindrome(list1) ? "YES" : "NO");

    // Test case 2: Even length palindrome
    // 1 -> 2 -> 2 -> 1
    Node* list2 = create_node(1);
    list2->next = create_node(2);
    list2->next->next = create_node(2);
    list2->next->next->next = create_node(1);

    printf("List: ");
    print_list(list2);
    printf("Is palindrome: %s\n\n", is_palindrome(list2) ? "YES" : "NO");

    // Test case 3: Not a palindrome
    // 1 -> 2 -> 3 -> 4
    Node* list3 = create_node(1);
    list3->next = create_node(2);
    list3->next->next = create_node(3);
    list3->next->next->next = create_node(4);

    printf("List: ");
    print_list(list3);
    printf("Is palindrome: %s\n\n", is_palindrome(list3) ? "YES" : "NO");

    // Test case 4: Single node
    Node* list4 = create_node(1);
    printf("List: ");
    print_list(list4);
    printf("Is palindrome: %s\n\n", is_palindrome(list4) ? "YES" : "NO");

    // Test case 5: Two nodes, same value
    Node* list5 = create_node(1);
    list5->next = create_node(1);
    printf("List: ");
    print_list(list5);
    printf("Is palindrome: %s\n\n", is_palindrome(list5) ? "YES" : "NO");

    // Test case 6: Two nodes, different values
    Node* list6 = create_node(1);
    list6->next = create_node(2);
    printf("List: ");
    print_list(list6);
    printf("Is palindrome: %s\n", is_palindrome(list6) ? "YES" : "NO");

    return 0;
}

/*
 * ==================== INTERVIEW EXPLANATION ====================
 *
 * PROBLEM: Check if a linked list is a palindrome.
 *
 * APPROACH - Three Step Process:
 *
 * Step 1: Find Middle (slow/fast pointer)
 *   1 -> 2 -> 3 -> 2 -> 1
 *             ^
 *           slow (middle)
 *
 * Step 2: Reverse Second Half
 *   First half:  1 -> 2 -> 3
 *   Second half: 1 -> 2 (was 2 -> 1, now reversed)
 *
 * Step 3: Compare
 *   First:  1 -> 2 -> 3...
 *   Second: 1 -> 2 -> NULL
 *   Compare until second half ends
 *
 * WHY COMPARE UNTIL SECOND HALF ENDS:
 * - For odd length: first half has extra middle element
 * - For even length: both halves same length
 * - Second half is always <= first half length
 *
 * WHY RESTORE THE LIST:
 * - Good practice in interviews (don't modify input)
 * - Shows attention to side effects
 * - Some interviewers require it
 *
 * ALTERNATIVE APPROACHES:
 *
 * 1. Stack-based O(n) space:
 *    - Push first half onto stack
 *    - Pop and compare with second half
 *
 * 2. Recursive O(n) space:
 *    - Compare first with last recursively
 *    - Uses call stack
 *
 * 3. Array conversion O(n) space:
 *    - Copy to array
 *    - Use two-pointer on array
 *
 * EDGE CASES:
 * 1. Empty list -> Palindrome (vacuously true)
 * 2. Single node -> Palindrome
 * 3. Two same nodes -> Palindrome
 * 4. Two different nodes -> Not palindrome
 *
 * COMMON MISTAKES:
 * - Wrong middle finding (affects odd/even handling)
 * - Forgetting to handle odd length correctly
 * - Not comparing values (comparing pointers instead)
 * - Off-by-one in comparison loop
 *
 * INTERVIEW TIP:
 * - Mention trade-off: O(1) space requires modifying list
 * - Ask if modifying input is acceptable
 * - Offer to restore list after check
 */
