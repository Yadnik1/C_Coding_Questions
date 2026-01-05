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
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why is divide-and-conquer method faster?"
 * A1: Simple loop does 32 iterations with conditional branches.
 *     D&C method uses only 5 parallel operations (no branching).
 *     Modern CPUs execute parallel bit ops faster than loops.
 *
 * -------------------------------------------------------------------------
 * Q2: "Where is bit reversal used in embedded?"
 * A2: - CRC calculations (some use reversed bit order)
 *     - FFT algorithms (bit-reversal permutation)
 *     - Serial protocols (LSB vs MSB first conversion)
 *     - Cryptographic operations
 *
 * -------------------------------------------------------------------------
 * Q3: "Is there a hardware instruction for this?"
 * A3: ARM has RBIT instruction (Reverse BITS). x86 doesn't have direct
 *     instruction but has BSWAP (byte swap). Use GCC's __builtin_bitreverse32
 *     if available for platform-optimal implementation.
 *
 * -------------------------------------------------------------------------
 * Q4: "How do you reverse only lower N bits?"
 * A4: Extract lower N bits, reverse N-bit value, clear original N bits,
 *     OR in the reversed value. Mask operations!
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// Method 1: Simple loop
uint32_t reverseBits(uint32_t num) {
    // Say: "I'll initialize result to 0 to build the reversed number"
    // Initialize result to 0
    uint32_t result = 0;

    // Say: "I'll loop through all 32 bits"
    // Loop through all 32 bits
    for (int i = 0; i < 32; i++) {
        // Say: "First, I'll extract bit i from num by shifting right and ANDing with 1"
        // Extract bit at position i
        uint32_t bit = (num >> i) & 1;

        // Say: "Then I'll place this bit at the reversed position 31 minus i"
        // Say: "I'll shift the bit left by (31-i) and OR it into result"
        // Place bit at reversed position (31 - i)
        result |= (bit << (31 - i));
    }

    // Say: "After processing all bits, result contains the reversed bit pattern"
    // Return reversed result
    return result;
}

// Method 2: Divide and Conquer (faster)
uint32_t reverseBitsFast(uint32_t num) {
    // Say: "I'll use divide and conquer to reverse bits in 5 steps"

    // Say: "Step 1: Swap adjacent bits using masks"
    // Say: "0xAAAAAAAA selects odd positions, 0x55555555 selects even positions"
    // Swap adjacent bits (positions 0-1, 2-3, 4-5, etc.)
    num = ((num & 0xAAAAAAAA) >> 1) | ((num & 0x55555555) << 1);

    // Say: "Step 2: Swap pairs of bits"
    // Say: "0xCCCCCCCC and 0x33333333 select alternating 2-bit groups"
    // Swap pairs (positions 0-1 with 2-3, 4-5 with 6-7, etc.)
    num = ((num & 0xCCCCCCCC) >> 2) | ((num & 0x33333333) << 2);

    // Say: "Step 3: Swap nibbles (4-bit groups)"
    // Swap nibbles (positions 0-3 with 4-7, etc.)
    num = ((num & 0xF0F0F0F0) >> 4) | ((num & 0x0F0F0F0F) << 4);

    // Say: "Step 4: Swap bytes"
    // Swap bytes (positions 0-7 with 8-15, etc.)
    num = ((num & 0xFF00FF00) >> 8) | ((num & 0x00FF00FF) << 8);

    // Say: "Step 5: Swap 16-bit halves"
    // Swap 16-bit halves (positions 0-15 with 16-31)
    num = (num >> 16) | (num << 16);

    // Say: "After 5 swaps, all bits are completely reversed"
    // Return fully reversed result
    return num;
}

// Reverse bits of 8-bit number (simpler example)
uint8_t reverseBits8(uint8_t num) {
    // Say: "For an 8-bit number, I'll use the same loop approach"
    // Initialize result to 0
    uint8_t result = 0;

    // Say: "I'll loop through all 8 bits"
    // Loop through 8 bits
    for (int i = 0; i < 8; i++) {
        // Say: "Extract bit i and place it at position 7 minus i"
        // Extract bit and place at reversed position
        result |= ((num >> i) & 1) << (7 - i);
    }

    // Return reversed result
    return result;
}

void printBinary32(uint32_t num) {
    // Print binary prefix
    printf("0b");

    // Loop through all 32 bits
    for (int i = 31; i >= 0; i--) {
        // Extract and print bit at position i
        printf("%d", (num >> i) & 1);

        // Add separator every 8 bits for readability
        if (i > 0 && i % 8 == 0) printf("_");
    }
}

void printBinary8(uint8_t num) {
    // Print binary prefix
    printf("0b");

    // Loop through all 8 bits
    for (int i = 7; i >= 0; i--) {
        // Extract and print bit at position i
        printf("%d", (num >> i) & 1);

        // Add separator at middle
        if (i == 4) printf("_");
    }
}

int main() {
    printf("=== Reverse Bits ===\n\n");

    // 8-bit example
    printf("1. 8-bit reversal:\n");
    // Test value
    uint8_t num8 = 0b00001011;  // 11
    printf("   Before: "); printBinary8(num8); printf(" (%u)\n", num8);
    // Reverse bits
    uint8_t rev8 = reverseBits8(num8);
    printf("   After:  "); printBinary8(rev8); printf(" (%u)\n\n", rev8);

    // 32-bit example
    printf("2. 32-bit reversal:\n");
    // Test value
    uint32_t num32 = 0b00000000000000000000000000001011;  // 11
    printf("   Before: "); printBinary32(num32); printf("\n");
    printf("           = %u\n", num32);
    // Reverse bits using simple method
    uint32_t rev32 = reverseBits(num32);
    printf("   After:  "); printBinary32(rev32); printf("\n");
    printf("           = %u\n\n", rev32);

    // Another example
    printf("3. Another example:\n");
    // Test value
    uint32_t num = 43261596;
    printf("   Before: "); printBinary32(num); printf("\n");
    printf("           = %u\n", num);
    // Reverse using fast method
    uint32_t rev = reverseBitsFast(num);
    printf("   After:  "); printBinary32(rev); printf("\n");
    printf("           = %u\n\n", rev);

    printf("=== Algorithm ===\n");
    printf("Simple: For each bit i, place at position (31-i)\n");
    printf("Fast:   Divide & conquer - swap halves recursively\n");

    return 0;
}
