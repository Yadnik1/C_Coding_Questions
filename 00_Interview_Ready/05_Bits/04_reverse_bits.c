/*
 * ============================================================================
 * PROBLEM: Reverse Bits of a 32-bit Integer
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Reverse all 32 bits of an unsigned integer. The MSB becomes LSB and
 * vice versa. This is a mirror reflection of the binary representation.
 *
 * EXAMPLES (with binary representation):
 * - Input:  00000000_00000000_00000000_00001011 (11)
 *   Output: 11010000_00000000_00000000_00000000 (3489660928)
 *
 * - Input:  00000000_00000000_00000000_00000001 (1)
 *   Output: 10000000_00000000_00000000_00000000 (2147483648)
 *
 * - Input:  10101010_10101010_10101010_10101010 (0xAAAAAAAA)
 *   Output: 01010101_01010101_01010101_01010101 (0x55555555)
 *
 * Simplified 8-bit example:
 * - Input:  1011_0001 (177)
 *   Output: 1000_1101 (141)
 *           ^ mirrored ^
 *
 * WHY IS THIS ASKED IN EMBEDDED INTERVIEWS?
 * - LSB-first vs MSB-first serial protocols (SPI, UART modes)
 * - CRC calculations may require bit reversal
 * - Some hardware peripherals expect reversed bit order
 * - FFT (Fast Fourier Transform) butterfly addressing
 * - Communication protocol endianness handling
 *
 * KEY CONCEPT - BUILD RESULT BIT BY BIT:
 * Extract bits from right side of input, add to left side of output.
 * 1. Shift result left (make room for new bit)
 * 2. Extract rightmost bit of input: (n & 1)
 * 3. Add bit to result: result |= bit
 * 4. Shift input right (move to next bit)
 *
 * VISUAL (4-bit example for clarity):
 *   Input n = 1011 (11), Build result from scratch
 *
 *   i=0: result=0000     extract n&1=1     result=0001     n>>1=0101
 *   i=1: result=0010     extract n&1=1     result=0011     n>>1=0010
 *   i=2: result=0110     extract n&1=0     result=0110     n>>1=0001
 *   i=3: result=1100     extract n&1=1     result=1101     n>>1=0000
 *        ^^^^shift left       ^extract         ^OR in
 *
 *   Final result: 1101 (which is 1011 reversed!)
 *
 *   Original: 1 0 1 1
 *             | | | |
 *             v v v v
 *   Reversed: 1 1 0 1
 *
 * TIME COMPLEXITY: O(32) = O(1) - fixed number of iterations
 * SPACE COMPLEXITY: O(1)
 *
 * ============================================================================
 */

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
