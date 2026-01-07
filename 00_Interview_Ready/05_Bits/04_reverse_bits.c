// Reverse bits of a 32-bit unsigned integer
// Time: O(32) = O(1), Space: O(1)

#include <stdio.h>
#include <stdint.h>

void print_binary(uint32_t n) {
    printf("0b");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0 && i != 0) printf("_");
    }
    printf("\n");
}

uint32_t reverse_bits(uint32_t n) {
    // Say: "Build result bit by bit from right to left"
    uint32_t result = 0;

    // Say: "Process all 32 bits"
    for (int i = 0; i < 32; i++) {
        // Say: "Make room for new bit in result"
        result <<= 1;

        // Say: "Extract rightmost bit of n and add to result"
        result |= (n & 1);

        // Say: "Move to next bit of n"
        n >>= 1;
    }

    return result;
}

int main() {
    uint32_t num = 0b00000000000000000000000000001011;  // 11

    printf("Original: ");
    print_binary(num);

    uint32_t reversed = reverse_bits(num);

    printf("Reversed: ");
    print_binary(reversed);

    printf("Decimal:  %u -> %u\n", 11, reversed);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I reverse bits by extracting from right and building from left.

 Algorithm:
 1. Initialize result = 0
 2. For each of 32 bits:
    - Shift result left (make room)
    - Get rightmost bit of n: (n & 1)
    - Add it to result: result |= bit
    - Shift n right (move to next bit)

 Example: n = 1011 (4-bit for simplicity)
 - i=0: result=0000, n&1=1, result=0001, n=0101
 - i=1: result=0010, n&1=1, result=0011, n=0010
 - i=2: result=0110, n&1=0, result=0110, n=0001
 - i=3: result=1100, n&1=1, result=1101, n=0000
 - Result: 1101 ✓

 Time: O(32) = O(1) - fixed iterations
 Space: O(1) - single result variable

 WHY THIS MATTERS IN EMBEDDED:
 - Bit reversal needed for LSB-first vs MSB-first protocols
 - Some hardware expects reversed bit order
 - CRC calculations sometimes need bit reversal"
*/
