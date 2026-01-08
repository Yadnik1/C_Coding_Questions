/*
 * ============================================================================
 * PROBLEM: Check if Number is Power of 2
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Determine if a given positive integer is a power of 2 (1, 2, 4, 8, 16...).
 * A power of 2 has exactly ONE bit set in its binary representation.
 *
 * EXAMPLES (with binary representation):
 * - Input: 1 (binary: 0001)   -> Output: YES (2^0)
 * - Input: 2 (binary: 0010)   -> Output: YES (2^1)
 * - Input: 4 (binary: 0100)   -> Output: YES (2^2)
 * - Input: 8 (binary: 1000)   -> Output: YES (2^3)
 * - Input: 3 (binary: 0011)   -> Output: NO (two 1s)
 * - Input: 6 (binary: 0110)   -> Output: NO (two 1s)
 * - Input: 0 (binary: 0000)   -> Output: NO (zero is not a power of 2)
 *
 * WHY IS THIS ASKED IN EMBEDDED INTERVIEWS?
 * - Memory alignment (must be power of 2)
 * - Buffer sizes (often powers of 2 for efficient indexing)
 * - DMA transfer sizes must be powers of 2
 * - Hash table sizes (for modulo optimization: n & (size-1))
 * - Checking valid memory region sizes
 *
 * KEY CONCEPT - THE n & (n-1) TRICK:
 * Power of 2 has exactly ONE bit set.
 * n & (n-1) clears the rightmost set bit.
 * If clearing that ONE bit gives 0, it was a power of 2!
 *
 * VISUAL:
 *   Power of 2 (n = 8):
 *   n:        1 0 0 0  (8)    <- only one 1
 *   n-1:      0 1 1 1  (7)    <- rightmost 1 flipped, all after become 1
 *   n&(n-1):  0 0 0 0  (0)    <- result is 0! Power of 2!
 *
 *   NOT power of 2 (n = 6):
 *   n:        0 1 1 0  (6)    <- two 1s
 *   n-1:      0 1 0 1  (5)
 *   n&(n-1):  0 1 0 0  (4)    <- result is NOT 0! Not power of 2!
 *
 *   Edge case (n = 0):
 *   0 is NOT a power of 2, but 0 & (-1) = 0
 *   That is why we also check n != 0
 *
 * FORMULA: (n != 0) && ((n & (n-1)) == 0)
 *
 * TIME COMPLEXITY: O(1) - single bitwise operation
 * SPACE COMPLEXITY: O(1)
 *
 * ============================================================================
 */

// Check if number is power of 2
// Time: O(1), Space: O(1)

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void print_binary(uint32_t n) {
    printf("0b");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0 && i != 0) printf("_");
    }
}

bool is_power_of_2(uint32_t n) {
    // Say: "Power of 2 has exactly ONE bit set"
    // Say: "Example: 8 = 1000, 16 = 10000"

    // Say: "n & (n-1) clears the rightmost set bit"
    // Say: "If n is power of 2, clearing that one bit gives 0"
    // Say: "Also check n != 0 because 0 is not a power of 2"

    return (n != 0) && ((n & (n - 1)) == 0);
}

int main() {
    uint32_t test_values[] = {0, 1, 2, 3, 4, 8, 15, 16, 32, 100};
    int n = sizeof(test_values) / sizeof(test_values[0]);

    printf("Checking power of 2:\n");
    for (int i = 0; i < n; i++) {
        printf("%3u: ", test_values[i]);
        print_binary(test_values[i]);
        printf(" -> %s\n", is_power_of_2(test_values[i]) ? "YES" : "NO");
    }

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Power of 2 has exactly one bit set (like 1, 10, 100, 1000 in binary).

 I use: (n != 0) && ((n & (n-1)) == 0)

 Why n & (n-1)?
 - (n-1) flips all bits after rightmost 1, including that 1
 - If n has only one 1 bit, n & (n-1) becomes 0
 - If n has multiple 1s, result is non-zero

 Example: n = 8 (1000)
 - n-1 = 7 (0111)
 - n & (n-1) = 1000 & 0111 = 0000 ✓ Power of 2

 Example: n = 6 (0110)
 - n-1 = 5 (0101)
 - n & (n-1) = 0110 & 0101 = 0100 ✗ Not power of 2

 Why check n != 0?
 - 0 & (-1) = 0, but 0 is not a power of 2

 Time: O(1), Space: O(1) - single bitwise operation"
*/
