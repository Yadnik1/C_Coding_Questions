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
    return n > 0 && (n & (n - 1)) == 0;
}

// Alternative: Count set bits (should be 1)
bool isPowerOf2_CountBits(int n) {
    if (n <= 0) return false;

    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count == 1;
}

// Alternative: Division method
bool isPowerOf2_Division(int n) {
    if (n <= 0) return false;

    while (n > 1) {
        if (n % 2 != 0) return false;
        n /= 2;
    }
    return true;
}

int main() {
    printf("=== Check Power of 2 ===\n\n");

    printf("Testing 1 to 20:\n");
    for (int i = 1; i <= 20; i++) {
        if (isPowerOf2(i)) {
            printf("%d is a power of 2\n", i);
        }
    }

    printf("\nBinary representation:\n");
    int powers[] = {1, 2, 4, 8, 16};
    for (int i = 0; i < 5; i++) {
        int n = powers[i];
        printf("%2d = ", n);
        for (int j = 4; j >= 0; j--) {
            printf("%d", (n >> j) & 1);
        }
        printf(" → n & (n-1) = %d\n", n & (n - 1));
    }

    return 0;
}
