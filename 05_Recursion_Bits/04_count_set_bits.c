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
    // Say: "I'll initialize a counter to track set bits"
    // Initialize count to 0
    int count = 0;

    // Say: "I'll loop while n is non-zero"
    // Loop while n has bits remaining
    while (n) {
        // Say: "Add the least significant bit to count using n & 1"
        // Check LSB and add to count
        count += n & 1;  // Add LSB

        // Say: "Then shift n right by 1 to check the next bit"
        // Right shift by 1 position
        n >>= 1;         // Right shift
    }

    // Say: "Return the total count of set bits"
    // Return final count
    return count;
}

// Method 2: Brian Kernighan's (optimal)
int countSetBits_Kernighan(int n) {
    // Say: "I'll use Kernighan's algorithm to count set bits efficiently"
    // Initialize count to 0
    int count = 0;

    // Say: "I'll loop while n is non-zero"
    // Loop while n has set bits
    while (n) {
        // Say: "Clear the rightmost set bit using n & (n-1)"
        // Clear rightmost set bit
        n = n & (n - 1);  // Clear rightmost set bit

        // Say: "Increment count for each bit cleared"
        // Increment counter
        count++;
    }

    // Say: "The count equals the number of set bits"
    // Return total count
    return count;
}

// Method 3: Lookup table (fastest for large data)
int countSetBits_Lookup(int n) {
    // Say: "I'll use a lookup table approach for maximum speed"
    // Declare static lookup table (persists across calls)
    static int table[256];
    // Flag to track if table is initialized
    static int initialized = 0;

    // Say: "First, check if the lookup table needs initialization"
    // Check if table needs to be built
    if (!initialized) {
        // Say: "I'll pre-compute set bit counts for all 8-bit values"
        // Loop through all 256 possible byte values
        for (int i = 0; i < 256; i++) {
            // Compute set bits recursively: LSB + set bits in i/2
            table[i] = (i & 1) + table[i / 2];
        }
        // Mark table as initialized
        initialized = 1;
    }

    // Say: "Now I'll split the 32-bit number into 4 bytes and sum their counts"
    // Extract byte 0 (bits 0-7) and look up count
    // Extract byte 1 (bits 8-15) and look up count
    // Extract byte 2 (bits 16-23) and look up count
    // Extract byte 3 (bits 24-31) and look up count
    // Sum all four byte counts
    return table[n & 0xFF] +
           table[(n >> 8) & 0xFF] +
           table[(n >> 16) & 0xFF] +
           table[(n >> 24) & 0xFF];
}

void printBinary(int n) {
    // Print 8-bit binary representation
    for (int i = 7; i >= 0; i--) {
        // Extract and print bit at position i
        printf("%d", (n >> i) & 1);
    }
}

int main() {
    printf("=== Count Set Bits ===\n\n");

    // Array of test values
    int tests[] = {0, 1, 7, 8, 13, 255};
    // Number of test values
    int n = 6;

    printf("Number | Binary   | Set Bits\n");
    printf("-------|----------|----------\n");

    // Loop through test values
    for (int i = 0; i < n; i++) {
        // Print number
        printf("%6d | ", tests[i]);
        // Print binary representation
        printBinary(tests[i]);
        // Print count of set bits
        printf(" | %d\n", countSetBits_Kernighan(tests[i]));
    }

    printf("\nKernighan's step-by-step for 13:\n");
    // Test value
    int num = 13;
    // Counter
    int count = 0;

    // Loop while num has set bits
    while (num) {
        // Print current value of num
        printf("  n = %d (", num);
        printBinary(num);
        printf(")\n");

        // Clear rightmost set bit
        num = num & (num - 1);
        // Increment count
        count++;
    }

    // Print final result
    printf("  Total set bits: %d\n", count);

    return 0;
}
