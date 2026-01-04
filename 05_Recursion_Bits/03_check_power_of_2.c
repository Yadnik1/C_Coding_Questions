/*
 * ============================================================================
 * PROBLEM: Check if Number is Power of 2
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 2 mins | FREQUENCY: Very High
 *
 * Powers of 2: 1, 2, 4, 8, 16, 32, 64, ...
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. BINARY PROPERTY OF POWERS OF 2:
 *    - Only ONE bit is set
 *    - 1 = 0001, 2 = 0010, 4 = 0100, 8 = 1000
 *
 * 2. TRICK: n & (n-1)
 *    - Clears the rightmost set bit
 *    - For power of 2, result is 0!
 *
 * ============================================================================
 * VISUAL PROOF:
 * ============================================================================
 *
 * For n = 8 (power of 2):
 *   n     = 8 = 1000
 *   n-1   = 7 = 0111
 *   n & (n-1) = 0000 ← All zeros!
 *
 * For n = 6 (NOT power of 2):
 *   n     = 6 = 0110
 *   n-1   = 5 = 0101
 *   n & (n-1) = 0100 ← Not zero!
 *
 * For n = 16 (power of 2):
 *   n     = 16 = 10000
 *   n-1   = 15 = 01111
 *   n & (n-1)  = 00000 ← All zeros!
 *
 * ============================================================================
 * WHY n & (n-1) WORKS:
 * ============================================================================
 *
 * n-1 flips all bits after rightmost 1 (including it)
 *
 *   n   = ...1000...0
 *   n-1 = ...0111...1
 *
 * AND clears that rightmost 1:
 *   n & (n-1) = ...0000...0
 *
 * If only ONE bit was set (power of 2), result is 0!
 *
 * ============================================================================
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Optimal: Using bit manipulation
bool isPowerOf2(int n) {
    // Say: "A power of 2 must be positive and have exactly one bit set"
    // Say: "So I'll check if n is greater than 0 AND n & (n-1) equals zero"
    // Check: n must be positive AND n & (n-1) must equal 0
    return n > 0 && (n & (n - 1)) == 0;
}

// Alternative: Count set bits (should be 1)
bool isPowerOf2_CountBits(int n) {
    // Say: "First, check if n is positive"
    // Validate n is positive
    if (n <= 0) return false;

    // Say: "I'll count the number of set bits"
    // Initialize counter to 0
    int count = 0;

    // Say: "I'll loop through all bits of n"
    // Loop while n is non-zero
    while (n) {
        // Say: "Add 1 to count if the least significant bit is set"
        // Check LSB and add to count
        count += n & 1;

        // Say: "Then shift n right by 1 to check the next bit"
        // Shift right by 1 position
        n >>= 1;
    }

    // Say: "A power of 2 has exactly 1 set bit"
    // Return true if count equals 1
    return count == 1;
}

// Alternative: Division method
bool isPowerOf2_Division(int n) {
    // Say: "First, check if n is positive"
    // Validate n is positive
    if (n <= 0) return false;

    // Say: "I'll keep dividing by 2 until n becomes 1"
    // Loop while n is greater than 1
    while (n > 1) {
        // Say: "If n is odd at any point, it's not a power of 2"
        // Check if n is odd (n % 2 != 0)
        if (n % 2 != 0) return false;

        // Say: "Otherwise, divide n by 2 and continue"
        // Divide n by 2
        n /= 2;
    }

    // Say: "If we reach 1, then n was a power of 2"
    // Return true
    return true;
}

int main() {
    printf("=== Check Power of 2 ===\n\n");

    printf("Testing 1 to 20:\n");
    // Loop from 1 to 20
    for (int i = 1; i <= 20; i++) {
        // Check if i is a power of 2
        if (isPowerOf2(i)) {
            // Print if it's a power of 2
            printf("%d is a power of 2\n", i);
        }
    }

    printf("\nBinary representation:\n");
    // Array of known powers of 2
    int powers[] = {1, 2, 4, 8, 16};

    // Loop through the array
    for (int i = 0; i < 5; i++) {
        // Get current power of 2
        int n = powers[i];

        // Print the number
        printf("%2d = ", n);

        // Print binary representation (5 bits)
        for (int j = 4; j >= 0; j--) {
            // Extract bit at position j
            printf("%d", (n >> j) & 1);
        }

        // Print the result of n & (n-1)
        printf(" → n & (n-1) = %d\n", n & (n - 1));
    }

    return 0;
}
