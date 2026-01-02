/*
 * ============================================================================
 * PROBLEM: Count Set Bits (Population Count / Hamming Weight)
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * Count number of 1s in binary representation.
 *
 * Example:
 * 13 = 1101 → 3 set bits
 * 7 = 0111 → 3 set bits
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. BRIAN KERNIGHAN'S TRICK:
 *    - n & (n-1) clears rightmost set bit
 *    - Count iterations until n becomes 0
 *
 * 2. RIGHT SHIFT METHOD:
 *    - Check LSB, right shift, repeat
 *
 * ============================================================================
 * BRIAN KERNIGHAN'S ALGORITHM:
 * ============================================================================
 *
 * n = 13 = 1101
 *
 * Iteration 1:
 *   n   = 1101
 *   n-1 = 1100
 *   n & (n-1) = 1100 → count = 1
 *
 * Iteration 2:
 *   n   = 1100
 *   n-1 = 1011
 *   n & (n-1) = 1000 → count = 2
 *
 * Iteration 3:
 *   n   = 1000
 *   n-1 = 0111
 *   n & (n-1) = 0000 → count = 3
 *
 * n = 0, STOP
 * Result: 3 set bits
 *
 * ============================================================================
 * TIME COMPLEXITY:
 * ============================================================================
 *
 * Right shift: O(log n) - always checks all bits
 * Kernighan's: O(k) where k = number of set bits
 *
 * ============================================================================
 */

#include <stdio.h>

// Method 1: Right shift and check LSB
int countSetBits_Shift(int n) {
    int count = 0;
    while (n) {
        count += n & 1;  // Add LSB
        n >>= 1;         // Right shift
    }
    return count;
}

// Method 2: Brian Kernighan's (optimal)
int countSetBits_Kernighan(int n) {
    int count = 0;
    while (n) {
        n = n & (n - 1);  // Clear rightmost set bit
        count++;
    }
    return count;
}

// Method 3: Lookup table (fastest for large data)
int countSetBits_Lookup(int n) {
    static int table[256];
    static int initialized = 0;

    if (!initialized) {
        for (int i = 0; i < 256; i++) {
            table[i] = (i & 1) + table[i / 2];
        }
        initialized = 1;
    }

    return table[n & 0xFF] +
           table[(n >> 8) & 0xFF] +
           table[(n >> 16) & 0xFF] +
           table[(n >> 24) & 0xFF];
}

void printBinary(int n) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
}

int main() {
    printf("=== Count Set Bits ===\n\n");

    int tests[] = {0, 1, 7, 8, 13, 255};
    int n = 6;

    printf("Number | Binary   | Set Bits\n");
    printf("-------|----------|----------\n");

    for (int i = 0; i < n; i++) {
        printf("%6d | ", tests[i]);
        printBinary(tests[i]);
        printf(" | %d\n", countSetBits_Kernighan(tests[i]));
    }

    printf("\nKernighan's step-by-step for 13:\n");
    int num = 13;
    int count = 0;
    while (num) {
        printf("  n = %d (", num);
        printBinary(num);
        printf(")\n");
        num = num & (num - 1);
        count++;
    }
    printf("  Total set bits: %d\n", count);

    return 0;
}
