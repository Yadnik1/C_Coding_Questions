/*
 * ADD TWO NUMBERS (Linked List) - Digit by Digit with Carry
 *
 * Time Complexity: O(max(n, m)) - Traverse both lists once
 * Space Complexity: O(max(n, m)) - New list for result
 *
 * Pattern: Simultaneous Traversal with Carry
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

// Numbers are stored in REVERSE order (least significant digit first)
// Example: 342 is stored as 2 -> 4 -> 3
Node* add_two_numbers(Node* l1, Node* l2) {
    // Say: "I use a dummy head and process digit by digit with carry"
    Node dummy;
    dummy.next = NULL;
    Node* tail = &dummy;
    int carry = 0;

    // Say: "Continue while there are digits or carry remaining"
    while (l1 != NULL || l2 != NULL || carry != 0) {
        // Say: "Get current digits (0 if list ended)"
        int val1 = (l1 != NULL) ? l1->data : 0;
        int val2 = (l2 != NULL) ? l2->data : 0;

        // Say: "Add digits and carry"
        int sum = val1 + val2 + carry;
        carry = sum / 10;           // New carry
        int digit = sum % 10;       // Current digit

        // Say: "Create new node with digit"
        tail->next = create_node(digit);
        tail = tail->next;

        // Say: "Advance pointers if not NULL"
        if (l1 != NULL) l1 = l1->next;
        if (l2 != NULL) l2 = l2->next;
    }

    return dummy.next;
}

// Helper: Create list from number (reverse order)
Node* number_to_list(int num) {
    if (num == 0) {
        return create_node(0);
    }

    Node* head = NULL;
    Node* tail = NULL;

    while (num > 0) {
        Node* node = create_node(num % 10);
        if (head == NULL) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        num /= 10;
    }

    return head;
}

// Helper: Convert list to number (for verification)
int list_to_number(Node* head) {
    int result = 0;
    int multiplier = 1;

    while (head) {
        result += head->data * multiplier;
        multiplier *= 10;
        head = head->next;
    }

    return result;
}

/* ==================== TEST ==================== */

int main() {
    // Test case 1: 342 + 465 = 807
    // 342 stored as: 2 -> 4 -> 3
    // 465 stored as: 5 -> 6 -> 4
    Node* l1 = create_node(2);
    l1->next = create_node(4);
    l1->next->next = create_node(3);

    Node* l2 = create_node(5);
    l2->next = create_node(6);
    l2->next->next = create_node(4);

    printf("Number 1: ");
    print_list(l1);
    printf("(Represents: %d)\n", list_to_number(l1));

    printf("Number 2: ");
    print_list(l2);
    printf("(Represents: %d)\n", list_to_number(l2));

    Node* sum = add_two_numbers(l1, l2);
    printf("Sum:      ");
    print_list(sum);
    printf("(Represents: %d)\n\n", list_to_number(sum));

    // Test case 2: 99 + 1 = 100 (carry propagation)
    Node* l3 = create_node(9);
    l3->next = create_node(9);

    Node* l4 = create_node(1);

    printf("99 + 1:\n");
    printf("Number 1: ");
    print_list(l3);
    printf("Number 2: ");
    print_list(l4);

    sum = add_two_numbers(l3, l4);
    printf("Sum:      ");
    print_list(sum);
    printf("(Represents: %d)\n\n", list_to_number(sum));

    // Test case 3: Different lengths
    // 1234 + 56 = 1290
    Node* l5 = number_to_list(1234);
    Node* l6 = number_to_list(56);

    printf("1234 + 56:\n");
    printf("Number 1: ");
    print_list(l5);
    printf("Number 2: ");
    print_list(l6);

    sum = add_two_numbers(l5, l6);
    printf("Sum:      ");
    print_list(sum);
    printf("(Represents: %d)\n\n", list_to_number(sum));

    // Test case 4: Zero
    Node* l7 = create_node(0);
    Node* l8 = create_node(5);

    printf("0 + 5:\n");
    sum = add_two_numbers(l7, l8);
    printf("Sum:      ");
    print_list(sum);
    printf("(Represents: %d)\n", list_to_number(sum));

    return 0;
}

/*
 * ==================== INTERVIEW EXPLANATION ====================
 *
 * PROBLEM: Add two numbers represented as linked lists.
 *          Digits are stored in REVERSE order (least significant first).
 *
 * WHY REVERSE ORDER:
 * - Addition starts from least significant digit
 * - Reverse order aligns with natural addition process
 * - Head of list = ones place (no need to find tail first)
 *
 * APPROACH - Digit by Digit Addition:
 *
 * Visual for 342 + 465:
 *   l1: 2 -> 4 -> 3
 *   l2: 5 -> 6 -> 4
 *
 *   Step 1: 2 + 5 = 7, carry = 0, result: 7
 *   Step 2: 4 + 6 = 10, carry = 1, result: 7 -> 0
 *   Step 3: 3 + 4 + 1(carry) = 8, carry = 0, result: 7 -> 0 -> 8
 *
 *   Final: 7 -> 0 -> 8 (represents 807)
 *
 * KEY POINTS:
 * 1. Use dummy head for clean list building
 * 2. Handle different length lists (treat missing as 0)
 * 3. Don't forget final carry! (e.g., 999 + 1 = 1000)
 *
 * LOOP CONDITION:
 *   while (l1 || l2 || carry)
 * - l1 has remaining digits
 * - l2 has remaining digits
 * - carry needs to be processed
 *
 * EDGE CASES:
 * 1. Different lengths (1234 + 56)
 * 2. Carry at the end (99 + 1 = 100)
 * 3. One number is zero
 * 4. Both numbers are zero
 * 5. Large numbers that overflow int (need long or BigInt)
 *
 * COMMON MISTAKES:
 * - Forgetting final carry (99 + 1 should be 100, not 00)
 * - Not handling different lengths
 * - Trying to convert to int first (overflow for large numbers)
 *
 * VARIANT: Numbers in FORWARD order
 * - More complex: need to align digits first
 * - Options:
 *   1. Reverse both lists, add, reverse result
 *   2. Use recursion (add from end)
 *   3. Use stack to access digits in reverse
 *
 * FOLLOW-UP QUESTIONS:
 *
 * Q: What if numbers are stored in forward order?
 * A: Reverse lists first, or use recursion/stack to access tail first
 *
 * Q: Handle very large numbers?
 * A: Current approach works! No integer overflow since we process digit by digit
 *
 * Q: In-place modification?
 * A: Possible but messy - reuse longer list, handle remaining carry
 *
 * INTERVIEW TIP:
 * - Clarify: "Digits in reverse or forward order?"
 * - Mention: "This approach handles arbitrary length numbers without overflow"
 */
