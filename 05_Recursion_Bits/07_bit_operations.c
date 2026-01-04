/*
 * ============================================================================
 * PROBLEM: Basic Bit Operations
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH (Embedded)
 *
 * Essential bit operations for embedded systems programming.
 * These are MUST-KNOW operations for any embedded/firmware interview.
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. BIT POSITIONS:
 *    - Bit 0 is the RIGHTMOST (LSB - Least Significant Bit)
 *    - Bit 7 is the LEFTMOST in a byte (MSB - Most Significant Bit)
 *    - Bits are numbered from RIGHT to LEFT starting at 0
 *
 *    Example (8-bit number):
 *    Value:    0b01011010
 *    Position:   76543210
 *                ^^^^^^^^
 *                ||||||||
 *                |||||||+-- Bit 0 (LSB) = 0
 *                ||||||+--- Bit 1 = 1
 *                |||||+---- Bit 2 = 0
 *                ||||+----- Bit 3 = 1
 *                |||+------ Bit 4 = 1
 *                ||+------- Bit 5 = 0
 *                |+-------- Bit 6 = 1
 *                +--------- Bit 7 (MSB) = 0
 *
 * 2. THE MASK: (1 << n)
 *    - Creates a number with ONLY bit n set to 1
 *    - All other bits are 0
 *
 *    1 << 0 = 0b00000001
 *    1 << 1 = 0b00000010
 *    1 << 2 = 0b00000100
 *    1 << 3 = 0b00001000
 *    1 << 4 = 0b00010000
 *    1 << 5 = 0b00100000
 *    1 << 6 = 0b01000000
 *    1 << 7 = 0b10000000
 *
 * 3. BITWISE OPERATORS:
 *    &  (AND):  1 & 1 = 1, all others = 0  (keeps bits where BOTH are 1)
 *    |  (OR):   0 | 0 = 0, all others = 1  (sets bit if EITHER is 1)
 *    ^  (XOR):  same = 0, different = 1    (toggles bits)
 *    ~  (NOT):  flips all bits             (0→1, 1→0)
 *
 * ============================================================================
 * BIT MANIPULATION CHEAT SHEET:
 * ============================================================================
 *
 *   SET bit n:      num |= (1 << n)
 *   CLEAR bit n:    num &= ~(1 << n)
 *   TOGGLE bit n:   num ^= (1 << n)
 *   CHECK bit n:    (num >> n) & 1  OR  num & (1 << n)
 *
 *   Even/Odd:       num & 1 == 0 (even), num & 1 == 1 (odd)
 *
 * ============================================================================
 * OPERATION 1: SET BIT (Turn ON)
 * ============================================================================
 *
 * Formula: num |= (1 << n)
 *
 * To set bit n, we OR with a mask that has only bit n as 1:
 *
 *   num = 0b00001010 (10)
 *   Set bit 2:
 *
 *   Step 1: Create mask
 *   1 << 2 = 0b00000100
 *
 *   Step 2: OR the mask
 *   0b00001010  (num)
 * | 0b00000100  (mask: 1 << 2)
 *   ----------
 *   0b00001110  (result: 14)
 *          ^
 *          Bit 2 is now SET
 *
 * WHY OR?
 *   x | 0 = x  (original bit unchanged)
 *   x | 1 = 1  (bit becomes 1 regardless of original)
 *
 * ============================================================================
 * OPERATION 2: CLEAR BIT (Turn OFF)
 * ============================================================================
 *
 * Formula: num &= ~(1 << n)
 *
 * To clear bit n, we AND with a mask that has all 1s EXCEPT bit n:
 *
 *   num = 0b00001010 (10)
 *   Clear bit 1:
 *
 *   Step 1: Create mask
 *   1 << 1    = 0b00000010
 *   ~(1 << 1) = 0b11111101  (flip all bits)
 *
 *   Step 2: AND the inverted mask
 *   0b00001010  (num)
 * & 0b11111101  (mask: ~(1 << 1))
 *   ----------
 *   0b00001000  (result: 8)
 *         ^
 *         Bit 1 is now CLEAR
 *
 * WHY AND with inverted mask?
 *   x & 1 = x  (original bit unchanged)
 *   x & 0 = 0  (bit becomes 0 regardless of original)
 *
 * ============================================================================
 * OPERATION 3: TOGGLE BIT (Flip)
 * ============================================================================
 *
 * Formula: num ^= (1 << n)
 *
 * To toggle bit n, we XOR with a mask that has only bit n as 1:
 *
 *   num = 0b00001010 (10)
 *   Toggle bit 3:
 *
 *   Step 1: Create mask
 *   1 << 3 = 0b00001000
 *
 *   Step 2: XOR the mask
 *   0b00001010  (num)
 * ^ 0b00001000  (mask: 1 << 3)
 *   ----------
 *   0b00000010  (result: 2)
 *       ^
 *       Bit 3 flipped from 1 to 0
 *
 * WHY XOR?
 *   x ^ 0 = x  (original bit unchanged)
 *   x ^ 1 = ~x (bit is flipped: 0→1, 1→0)
 *
 * Toggle bit 3 again:
 *   0b00000010  (num)
 * ^ 0b00001000  (mask)
 *   ----------
 *   0b00001010  (back to original!)
 *
 * ============================================================================
 * OPERATION 4: CHECK BIT (Read/Test)
 * ============================================================================
 *
 * Method 1: (num >> n) & 1
 *
 *   num = 0b00001010 (10)
 *   Check bit 1:
 *
 *   Step 1: Shift right to bring bit n to position 0
 *   num >> 1 = 0b00000101
 *
 *   Step 2: AND with 1 to isolate the bit
 *   0b00000101 & 0b00000001 = 0b00000001 = 1
 *
 *   Result: Bit 1 is SET (returns 1)
 *
 *   Check bit 2:
 *   num >> 2 = 0b00000010
 *   0b00000010 & 0b00000001 = 0b00000000 = 0
 *
 *   Result: Bit 2 is CLEAR (returns 0)
 *
 * Method 2: num & (1 << n)
 *
 *   Check bit 1:
 *   0b00001010 & (1 << 1)
 *   0b00001010 & 0b00000010 = 0b00000010 = 2 (non-zero = true)
 *
 *   Check bit 2:
 *   0b00001010 & (1 << 2)
 *   0b00001010 & 0b00000100 = 0b00000000 = 0 (zero = false)
 *
 * ============================================================================
 * OPERATION 5: CHECK EVEN/ODD
 * ============================================================================
 *
 * Formula: num & 1
 *
 * The LSB (bit 0) determines even/odd:
 *   - If bit 0 is 0 → number is EVEN
 *   - If bit 0 is 1 → number is ODD
 *
 * Why? In binary:
 *   Even numbers: 2=10, 4=100, 6=110, 8=1000, 10=1010
 *   Odd numbers:  1=1,  3=11,  5=101, 7=111,  9=1001
 *                       ^           ^         ^
 *                       All odd numbers end in 1
 *
 * Example:
 *   10 & 1 = 0b1010 & 0b0001 = 0 → EVEN
 *   7 & 1  = 0b0111 & 0b0001 = 1 → ODD
 *
 * ============================================================================
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Set bit at position n
uint32_t setBit(uint32_t num, int n) {
    // Say: "To set bit n, I'll OR num with a mask that has only bit n set"
    // Say: "I'll create the mask using 1U shifted left by n positions"
    // Create mask (1U << n) and OR with num
    return num | (1U << n);
}

// Clear bit at position n
uint32_t clearBit(uint32_t num, int n) {
    // Say: "To clear bit n, I'll AND num with a mask that has all 1s except bit n"
    // Say: "First I'll create a mask with bit n set using 1U << n"
    // Say: "Then I'll invert it with NOT operator to get all 1s except bit n"
    // Create mask ~(1U << n) and AND with num
    return num & ~(1U << n);
}

// Toggle bit at position n
uint32_t toggleBit(uint32_t num, int n) {
    // Say: "To toggle bit n, I'll XOR num with a mask that has only bit n set"
    // Say: "XOR flips the bit: 0 becomes 1, and 1 becomes 0"
    // Create mask (1U << n) and XOR with num
    return num ^ (1U << n);
}

// Check if bit at position n is set
bool checkBit(uint32_t num, int n) {
    // Say: "To check bit n, I'll shift num right by n positions"
    // Say: "Then AND with 1 to isolate the least significant bit"
    // Shift right by n, then AND with 1
    return (num >> n) & 1;
}

// Alternative check
bool checkBitAlt(uint32_t num, int n) {
    // Say: "Alternatively, I can AND num with a mask that has only bit n set"
    // Say: "If the result is non-zero, the bit is set"
    // Create mask and AND, check if non-zero
    return (num & (1U << n)) != 0;
}

// Check if number is even
bool isEven(int num) {
    // Say: "To check if a number is even, I'll AND it with 1"
    // Say: "If bit 0 is 0, the number is even"
    // Check if LSB is 0
    return (num & 1) == 0;
}

// Check if number is odd
bool isOdd(int num) {
    // Say: "To check if a number is odd, I'll AND it with 1"
    // Say: "If bit 0 is 1, the number is odd"
    // Check if LSB is 1
    return (num & 1) == 1;
}

// Print binary representation
void printBinary(uint32_t num, int bits) {
    // Print binary prefix
    printf("0b");

    // Say: "I'll loop from the highest bit down to bit 0"
    // Loop from most significant to least significant bit
    for (int i = bits - 1; i >= 0; i--) {
        // Say: "Extract bit i by shifting right and ANDing with 1"
        // Extract and print bit at position i
        printf("%d", (num >> i) & 1);

        // Add underscore separator every 4 bits for readability
        if (i > 0 && i % 4 == 0) printf("_");
    }
}

int main() {
    printf("=== Basic Bit Operations ===\n\n");

    // Initialize test number
    uint32_t num = 0b00001010;  // 10 in decimal
    printf("Original number: ");
    printBinary(num, 8);
    printf(" (%u)\n\n", num);

    // SET bit demonstration
    printf("1. SET bit 2:\n");
    printf("   Before: "); printBinary(num, 8); printf("\n");
    printf("   After:  "); printBinary(setBit(num, 2), 8);
    printf(" (%u)\n\n", setBit(num, 2));

    // CLEAR bit demonstration
    printf("2. CLEAR bit 1:\n");
    printf("   Before: "); printBinary(num, 8); printf("\n");
    printf("   After:  "); printBinary(clearBit(num, 1), 8);
    printf(" (%u)\n\n", clearBit(num, 1));

    // TOGGLE bit demonstration
    printf("3. TOGGLE bit 3:\n");
    printf("   Before: "); printBinary(num, 8); printf("\n");
    printf("   After:  "); printBinary(toggleBit(num, 3), 8);
    printf(" (%u)\n\n", toggleBit(num, 3));

    // CHECK bit demonstration
    printf("4. CHECK bits:\n");
    // Loop through first 4 bits
    for (int i = 0; i < 4; i++) {
        // Check and print status of bit i
        printf("   Bit %d: %s\n", i, checkBit(num, i) ? "SET" : "CLEAR");
    }
    printf("\n");

    // Even/Odd demonstration
    printf("5. Even/Odd check:\n");
    // Test numbers 1 through 6
    for (int i = 1; i <= 6; i++) {
        // Check and print even/odd status
        printf("   %d is %s\n", i, isEven(i) ? "EVEN" : "ODD");
    }

    printf("\n=== Cheat Sheet ===\n");
    printf("SET:    num |= (1 << n)\n");
    printf("CLEAR:  num &= ~(1 << n)\n");
    printf("TOGGLE: num ^= (1 << n)\n");
    printf("CHECK:  (num >> n) & 1\n");
    printf("EVEN:   (num & 1) == 0\n");

    return 0;
}
