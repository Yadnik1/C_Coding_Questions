/*
 * ============================================================================
 * PROBLEM: Reverse Bits of an Integer
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: HIGH (Embedded)
 *
 * Reverse the bit order of a 32-bit unsigned integer.
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   Input:  0b00000000_00000000_00000000_00001011 (11)
 *   Output: 0b11010000_00000000_00000000_00000000 (3489660928)
 *
 *   Bits before: bit0=1, bit1=1, bit2=0, bit3=1
 *   Bits after:  bit31=1, bit30=1, bit29=0, bit28=1
 *
 * ============================================================================
 * APPROACHES:
 * ============================================================================
 *
 * 1. Simple loop: Extract each bit, place in reversed position
 * 2. Divide and conquer: Swap halves, quarters, etc.
 *
 * ============================================================================
 * TIME: O(32) = O(1) | SPACE: O(1)
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
