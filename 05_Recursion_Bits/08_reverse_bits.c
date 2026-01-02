/*
 * ============================================================================
 * PROBLEM: Reverse Bits of an Integer
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: HIGH (Embedded)
 *
 * Reverse the bit order of a 32-bit unsigned integer.
 *
 * Example:
 * Input:  11 (0b00001011)
 * Output: 3489660928 (0b11010000_00000000_00000000_00000000)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. BIT EXTRACTION:
 *    (num >> i) & 1  → Gets bit at position i
 *
 * 2. BIT PLACEMENT:
 *    bit << (31 - i) → Puts bit at reversed position
 *
 * 3. BUILD RESULT:
 *    result |= (bit << position) → Accumulate bits in result
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   Input:  0b00000000_00000000_00000000_00001011 (11)
 *   Output: 0b11010000_00000000_00000000_00000000 (3489660928)
 *
 *   What happens to each bit:
 *   Bit 0 (value=1) → moves to Bit 31
 *   Bit 1 (value=1) → moves to Bit 30
 *   Bit 2 (value=0) → moves to Bit 29
 *   Bit 3 (value=1) → moves to Bit 28
 *   ... (all other bits are 0)
 *
 * ============================================================================
 * METHOD 1: SIMPLE LOOP (Easy to understand)
 * ============================================================================
 *
 * Step-by-step for 8-bit number 0b00001011:
 *
 *   i=0: bit = (00001011 >> 0) & 1 = 1
 *        result |= 1 << 7 = 10000000
 *
 *   i=1: bit = (00001011 >> 1) & 1 = 1
 *        result |= 1 << 6 = 11000000
 *
 *   i=2: bit = (00001011 >> 2) & 1 = 0
 *        result |= 0 << 5 = 11000000 (unchanged)
 *
 *   i=3: bit = (00001011 >> 3) & 1 = 1
 *        result |= 1 << 4 = 11010000
 *
 *   i=4 to 7: all bits are 0, result stays 11010000
 *
 *   Final: 0b11010000 = 208
 *
 * ============================================================================
 * METHOD 2: DIVIDE AND CONQUER (Faster - O(log n) operations)
 * ============================================================================
 *
 * Swap progressively larger groups:
 *
 *   Original:    abcdefgh ijklmnop qrstuvwx yz123456
 *
 *   Step 1: Swap adjacent bits (odd/even)
 *   0xAAAAAAAA = 10101010... (selects odd bits)
 *   0x55555555 = 01010101... (selects even bits)
 *
 *   Result:      badcfehg jilknmpo rstqvuxw zy214365
 *
 *   Step 2: Swap pairs
 *   0xCCCCCCCC = 11001100... (selects pairs)
 *   0x33333333 = 00110011... (selects other pairs)
 *
 *   Result:      dcbahgfe lkjiponm tuvwrqxs 4321zy65
 *
 *   Step 3: Swap nibbles (4 bits)
 *   0xF0F0F0F0 and 0x0F0F0F0F
 *
 *   Result:      hgfedcba ponmlkji xwvutsrq 6543z21y
 *
 *   Step 4: Swap bytes
 *   0xFF00FF00 and 0x00FF00FF
 *
 *   Result:      ponmlkji hgfedcba 6543z21y xwvutsrq
 *
 *   Step 5: Swap 16-bit halves
 *
 *   Result:      6543z21y xwvutsrq ponmlkji hgfedcba
 *                ← Completely reversed! →
 *
 * ============================================================================
 * WHY THE MAGIC NUMBERS?
 * ============================================================================
 *
 *   0xAAAAAAAA = 10101010_10101010_10101010_10101010
 *                Selects bits at EVEN positions (0,2,4,6...)
 *
 *   0x55555555 = 01010101_01010101_01010101_01010101
 *                Selects bits at ODD positions (1,3,5,7...)
 *
 *   0xCCCCCCCC = 11001100_11001100_11001100_11001100
 *                Selects pairs at positions 2-3, 6-7, 10-11...
 *
 *   0x33333333 = 00110011_00110011_00110011_00110011
 *                Selects pairs at positions 0-1, 4-5, 8-9...
 *
 * ============================================================================
 * APPROACHES COMPARISON:
 * ============================================================================
 *
 *   Method          | Operations | Use Case
 *   ----------------|------------|---------------------------
 *   Simple loop     | 32 iters   | Clear, easy to understand
 *   Divide & conquer| 5 steps    | Faster, interview favorite
 *   Lookup table    | 4 lookups  | Fastest (if memory ok)
 *
 * ============================================================================
 * TIME COMPLEXITY: O(1)
 * ============================================================================
 * - Fixed 32 bits, constant iterations
 * - Simple loop: 32 iterations
 * - D&C: 5 operations
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only local variables
 * - No extra arrays
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "What's the time complexity of the divide-and-conquer method?"
 * A1: O(log n) where n is number of bits. For 32 bits, it's 5 operations.
 *     Each step halves the group size: 1→2→4→8→16→32
 *     That's log2(32) = 5 steps.
 *
 * -------------------------------------------------------------------------
 * Q2: "How would you optimize for repeated calls (same function called many times)?"
 * A2: Use a lookup table!
 *
 *     Pre-compute reversed values for all 8-bit numbers (256 entries).
 *     For 32-bit: split into 4 bytes, look up each, combine in reverse order.
 *
 *     static uint8_t reversedByte[256] = { ... pre-computed ... };
 *
 *     uint32_t reverseFast(uint32_t n) {
 *         return (reversedByte[n & 0xFF] << 24) |
 *                (reversedByte[(n >> 8) & 0xFF] << 16) |
 *                (reversedByte[(n >> 16) & 0xFF] << 8) |
 *                (reversedByte[(n >> 24) & 0xFF]);
 *     }
 *
 * -------------------------------------------------------------------------
 * Q3: "Why is bit reversal useful in embedded systems?"
 * A3: Several use cases:
 *     - FFT algorithms (butterfly operations)
 *     - CRC calculations
 *     - Serial communication (MSB vs LSB first)
 *     - Graphics (mirroring images)
 *
 * -------------------------------------------------------------------------
 * Q4: "What if we only want to reverse lower n bits, not all 32?"
 * A4: Reverse all 32, then shift right by (32 - n).
 *
 *     uint32_t reverseNBits(uint32_t num, int n) {
 *         return reverseBits(num) >> (32 - n);
 *     }
 *
 * -------------------------------------------------------------------------
 * Q5: "How would you do this without using the shift operator?"
 * A5: Use multiplication/division by powers of 2:
 *
 *     x >> 1  is same as  x / 2
 *     x << 1  is same as  x * 2
 *
 *     But shift operators are preferred (faster, cleaner).
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// Method 1: Simple loop
uint32_t reverseBits(uint32_t num) {
    uint32_t result = 0;

    for (int i = 0; i < 32; i++) {
        // Extract bit at position i
        uint32_t bit = (num >> i) & 1;
        // Place it at position (31 - i)
        result |= (bit << (31 - i));
    }

    return result;
}

// Method 2: Divide and Conquer (faster)
uint32_t reverseBitsFast(uint32_t num) {
    // Swap adjacent bits
    num = ((num & 0xAAAAAAAA) >> 1) | ((num & 0x55555555) << 1);
    // Swap pairs
    num = ((num & 0xCCCCCCCC) >> 2) | ((num & 0x33333333) << 2);
    // Swap nibbles
    num = ((num & 0xF0F0F0F0) >> 4) | ((num & 0x0F0F0F0F) << 4);
    // Swap bytes
    num = ((num & 0xFF00FF00) >> 8) | ((num & 0x00FF00FF) << 8);
    // Swap 16-bit halves
    num = (num >> 16) | (num << 16);

    return num;
}

// Reverse bits of 8-bit number (simpler example)
uint8_t reverseBits8(uint8_t num) {
    uint8_t result = 0;

    for (int i = 0; i < 8; i++) {
        result |= ((num >> i) & 1) << (7 - i);
    }

    return result;
}

void printBinary32(uint32_t num) {
    printf("0b");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i > 0 && i % 8 == 0) printf("_");
    }
}

void printBinary8(uint8_t num) {
    printf("0b");
    for (int i = 7; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i == 4) printf("_");
    }
}

int main() {
    printf("=== Reverse Bits ===\n\n");

    // 8-bit example
    printf("1. 8-bit reversal:\n");
    uint8_t num8 = 0b00001011;  // 11
    printf("   Before: "); printBinary8(num8); printf(" (%u)\n", num8);
    uint8_t rev8 = reverseBits8(num8);
    printf("   After:  "); printBinary8(rev8); printf(" (%u)\n\n", rev8);

    // 32-bit example
    printf("2. 32-bit reversal:\n");
    uint32_t num32 = 0b00000000000000000000000000001011;  // 11
    printf("   Before: "); printBinary32(num32); printf("\n");
    printf("           = %u\n", num32);
    uint32_t rev32 = reverseBits(num32);
    printf("   After:  "); printBinary32(rev32); printf("\n");
    printf("           = %u\n\n", rev32);

    // Another example
    printf("3. Another example:\n");
    uint32_t num = 43261596;
    printf("   Before: "); printBinary32(num); printf("\n");
    printf("           = %u\n", num);
    uint32_t rev = reverseBitsFast(num);
    printf("   After:  "); printBinary32(rev); printf("\n");
    printf("           = %u\n\n", rev);

    printf("=== Algorithm ===\n");
    printf("Simple: For each bit i, place at position (31-i)\n");
    printf("Fast:   Divide & conquer - swap halves recursively\n");

    return 0;
}
