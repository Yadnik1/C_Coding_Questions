/*
 * ============================================================================
 * PROBLEM: Swap Two Numbers Without Temporary Variable
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Exchange the values of two variables without using a third temporary
 * variable. Uses the XOR bitwise operator and its special properties.
 *
 * EXAMPLES (with binary representation):
 * - Input:  a = 5 (0101), b = 3 (0011)
 * - Output: a = 3 (0011), b = 5 (0101)
 *
 * Step-by-step with binary:
 *   Initial:  a = 0101 (5), b = 0011 (3)
 *
 *   Step 1: a = a ^ b
 *           a = 0101 ^ 0011 = 0110 (6)     <- a holds XOR of both
 *
 *   Step 2: b = a ^ b
 *           b = 0110 ^ 0011 = 0101 (5)     <- b now has original a!
 *
 *   Step 3: a = a ^ b
 *           a = 0110 ^ 0101 = 0011 (3)     <- a now has original b!
 *
 *   Final:   a = 0011 (3), b = 0101 (5)    SWAPPED!
 *
 * WHY IS THIS ASKED IN EMBEDDED INTERVIEWS?
 * - Tests understanding of XOR properties
 * - Classic bit manipulation problem
 * - Memory-constrained systems (no extra variable)
 * - Demonstrates clever use of bitwise operations
 * - NOTE: Modern compilers optimize temp swap equally well
 *
 * KEY CONCEPT - XOR PROPERTIES:
 * 1. A ^ A = 0      (XOR with itself gives 0)
 * 2. A ^ 0 = A      (XOR with 0 gives original)
 * 3. A ^ B = B ^ A  (Commutative)
 * 4. (A ^ B) ^ C = A ^ (B ^ C)  (Associative)
 *
 * VISUAL:
 *   XOR Truth Table:
 *   A | B | A^B
 *   --|---|----
 *   0 | 0 |  0
 *   0 | 1 |  1
 *   1 | 0 |  1
 *   1 | 1 |  0     <- same bits give 0
 *
 *   Why it works (algebraically):
 *   After step 1: a = A ^ B
 *   After step 2: b = (A ^ B) ^ B = A ^ (B ^ B) = A ^ 0 = A
 *   After step 3: a = (A ^ B) ^ A = (A ^ A) ^ B = 0 ^ B = B
 *
 * CRITICAL CAVEAT:
 * If a and b point to the SAME memory location, both become 0!
 * x ^ x = 0, so we must check (a != b) or (&a != &b) first.
 *
 * TIME COMPLEXITY: O(1)
 * SPACE COMPLEXITY: O(1)
 *
 * ============================================================================
 */

// Swap two numbers without using temporary variable
// Time: O(1), Space: O(1)

#include <stdio.h>

void swap_xor(int *a, int *b) {
    // Say: "XOR swap - uses property that X ^ X = 0"
    // Say: "Works because XOR is its own inverse"

    if (a == b) return;  // Say: "Same address would zero both values"

    *a = *a ^ *b;  // Say: "a now holds XOR of original a and b"
    *b = *a ^ *b;  // Say: "b = (a^b) ^ b = a (original a)"
    *a = *a ^ *b;  // Say: "a = (a^b) ^ a = b (original b)"

    // Say: "After this, values are swapped"
}

int main() {
    int x = 5, y = 10;

    printf("Before: x=%d, y=%d\n", x, y);
    swap_xor(&x, &y);
    printf("After:  x=%d, y=%d\n", x, y);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use XOR swap based on XOR properties:
 - A ^ A = 0 (XOR with itself gives 0)
 - A ^ 0 = A (XOR with 0 gives original)
 - XOR is commutative and associative

 Step by step for a=5, b=3:
 - a = a ^ b = 5 ^ 3 = 6    (a holds encoded value)
 - b = a ^ b = 6 ^ 3 = 5    (b now has original a)
 - a = a ^ b = 6 ^ 5 = 3    (a now has original b)

 IMPORTANT CAVEAT:
 - If a and b point to same memory location, this zeros both!
 - That's why we check if (a == b) first

 WHY THIS MATTERS IN EMBEDDED:
 - Sometimes used in memory-constrained systems
 - But modern compilers optimize temp variable swap well
 - Usually prefer temp variable for clarity

 Time: O(1), Space: O(1)"
*/
