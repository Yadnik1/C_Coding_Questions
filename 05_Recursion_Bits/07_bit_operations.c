/*
 * ============================================================================
 * PROBLEM: Basic Bit Operations
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH (Embedded)
 *
 * Essential bit operations for embedded systems programming.
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
 * VISUALIZATION:
 * ============================================================================
 *
 *   num = 0b00001010 (10 in decimal)
 *
 *   SET bit 2:    0b00001010 | 0b00000100 = 0b00001110
 *   CLEAR bit 1:  0b00001010 & 0b11111101 = 0b00001000
 *   TOGGLE bit 3: 0b00001010 ^ 0b00001000 = 0b00000010
 *   CHECK bit 1:  (0b00001010 >> 1) & 1 = 1
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
    return num | (1U << n);
}

// Clear bit at position n
uint32_t clearBit(uint32_t num, int n) {
    return num & ~(1U << n);
}

// Toggle bit at position n
uint32_t toggleBit(uint32_t num, int n) {
    return num ^ (1U << n);
}

// Check if bit at position n is set
bool checkBit(uint32_t num, int n) {
    return (num >> n) & 1;
}

// Alternative check
bool checkBitAlt(uint32_t num, int n) {
    return (num & (1U << n)) != 0;
}

// Check if number is even
bool isEven(int num) {
    return (num & 1) == 0;
}

// Check if number is odd
bool isOdd(int num) {
    return (num & 1) == 1;
}

// Print binary representation
void printBinary(uint32_t num, int bits) {
    printf("0b");
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i > 0 && i % 4 == 0) printf("_");
    }
}

int main() {
    printf("=== Basic Bit Operations ===\n\n");

    uint32_t num = 0b00001010;  // 10 in decimal
    printf("Original number: ");
    printBinary(num, 8);
    printf(" (%u)\n\n", num);

    // SET bit
    printf("1. SET bit 2:\n");
    printf("   Before: "); printBinary(num, 8); printf("\n");
    printf("   After:  "); printBinary(setBit(num, 2), 8);
    printf(" (%u)\n\n", setBit(num, 2));

    // CLEAR bit
    printf("2. CLEAR bit 1:\n");
    printf("   Before: "); printBinary(num, 8); printf("\n");
    printf("   After:  "); printBinary(clearBit(num, 1), 8);
    printf(" (%u)\n\n", clearBit(num, 1));

    // TOGGLE bit
    printf("3. TOGGLE bit 3:\n");
    printf("   Before: "); printBinary(num, 8); printf("\n");
    printf("   After:  "); printBinary(toggleBit(num, 3), 8);
    printf(" (%u)\n\n", toggleBit(num, 3));

    // CHECK bit
    printf("4. CHECK bits:\n");
    for (int i = 0; i < 4; i++) {
        printf("   Bit %d: %s\n", i, checkBit(num, i) ? "SET" : "CLEAR");
    }
    printf("\n");

    // Even/Odd
    printf("5. Even/Odd check:\n");
    for (int i = 1; i <= 6; i++) {
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
