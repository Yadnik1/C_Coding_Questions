/*
 * ============================================================================
 * PROBLEM: Count Set Bits (Hamming Weight / Population Count)
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Count the number of 1s (set bits) in the binary representation of an
 * integer. This is also called "Hamming Weight" or "Population Count".
 *
 * EXAMPLES (with binary representation):
 * - Input:  5 (binary: 0000_0101)  -> Output: 2 (two 1s)
 * - Input: 13 (binary: 0000_1101)  -> Output: 3 (three 1s)
 * - Input: 255 (binary: 1111_1111) -> Output: 8 (all 1s)
 * - Input: 0 (binary: 0000_0000)   -> Output: 0 (no 1s)
 *
 * WHY IS THIS ASKED IN EMBEDDED INTERVIEWS?
 * - Counting active flags in a status register
 * - Error detection (counting parity bits)
 * - Determining number of enabled peripherals/interrupts
 * - Calculating Hamming distance for error correction codes
 * - Many processors have dedicated POPCNT instruction
 *
 * KEY CONCEPT - BRIAN KERNIGHAN'S ALGORITHM:
 * The expression n & (n-1) clears the RIGHTMOST set bit.
 *
 * Why? When you subtract 1 from n:
 * - All bits to the right of rightmost 1 become 1
 * - The rightmost 1 becomes 0
 * - ANDing with original zeros out that bit
 *
 * VISUAL:
 *   n = 12 (decimal)
 *
 *   Binary:   1 1 0 0  (12)
 *             ^rightmost 1
 *
 *   Step 1: n & (n-1)
 *   n:        1 1 0 0  (12)
 *   n-1:      1 0 1 1  (11)   <- rightmost 1 and bits after it flip
 *   AND:      1 0 0 0  (8)    <- rightmost 1 is cleared!
 *   count = 1
 *
 *   Step 2: n & (n-1)
 *   n:        1 0 0 0  (8)
 *   n-1:      0 1 1 1  (7)
 *   AND:      0 0 0 0  (0)    <- rightmost 1 is cleared!
 *   count = 2
 *
 *   n = 0, STOP. Answer = 2 set bits
 *
 * TIME COMPLEXITY: O(k) where k = number of set bits (not 32!)
 * SPACE COMPLEXITY: O(1)
 *
 * ============================================================================
 */

// Count number of 1s (set bits) in an integer
// Time: O(k) where k = number of set bits, Space: O(1)

#include <stdio.h>
#include <stdint.h>

// Say: "Consistent helper to print binary representation"
void print_binary(uint32_t n) {
    printf("0b");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0 && i != 0) printf("_");  // Say: "Group by nibbles for readability"
    }
    printf("\n");
}

int count_set_bits(uint32_t n) {
    // Say: "Brian Kernighan's algorithm - most efficient"
    // Say: "Key insight: n & (n-1) clears the rightmost set bit"
    int count = 0;

    while (n) {
        n = n & (n - 1);  // Say: "Clear rightmost 1 bit"
        count++;          // Say: "Count each cleared bit"
    }
    // Say: "Loop runs only k times where k = number of set bits"

    return count;
}

int main() {
    uint32_t num = 0b10110101;  // Say: "Binary literal - 5 set bits"

    printf("Number: %u\n", num);
    printf("Binary: ");
    print_binary(num);

    printf("Set bits: %d\n", count_set_bits(num));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use Brian Kernighan's algorithm.

 Key insight: n & (n-1) clears the RIGHTMOST set bit.

 Example: n = 12 (1100 in binary)
 - n & (n-1) = 1100 & 1011 = 1000 (cleared rightmost 1)
 - n & (n-1) = 1000 & 0111 = 0000 (cleared rightmost 1)
 - n = 0, stop. Count = 2 ✓

 Why is n & (n-1) special?
 - (n-1) flips all bits after rightmost 1, including that 1
 - ANDing with n zeros out that rightmost 1

 Time: O(k) where k = number of set bits
 Space: O(1)

 Alternative: Loop through all 32 bits - O(32) = O(1) but slower
 Alternative: Lookup table - O(1) but uses extra memory"
*/
