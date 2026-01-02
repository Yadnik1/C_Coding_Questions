/*
 * ============================================================================
 * PROBLEM: Rotate Bits Left and Right
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: HIGH (Embedded)
 *
 * Rotate bits left or right by n positions.
 * Bits that fall off one end wrap around to the other end.
 *
 * ============================================================================
 * DIFFERENCE: Rotate vs Shift
 * ============================================================================
 *
 *   Shift left (<<):   Bits that fall off are lost, zeros fill in
 *   Rotate left:       Bits that fall off wrap around to right
 *
 *   Example (8-bit):
 *   0b11010010 << 2 = 0b01001000 (lost 11, added 00)
 *   rotateLeft(0b11010010, 2) = 0b01001011 (11 wrapped to right)
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   Rotate Left by 2:
 *   Before: [1][1][0][1][0][0][1][0]
 *   After:  [0][1][0][0][1][0][1][1]
 *           └──────────────────┘ ↑↑
 *           Shifted left     Wrapped around
 *
 * ============================================================================
 * FORMULA:
 * ============================================================================
 *
 *   Rotate Left:  (num << n) | (num >> (bits - n))
 *   Rotate Right: (num >> n) | (num << (bits - n))
 *
 * ============================================================================
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// Rotate left (32-bit)
uint32_t rotateLeft32(uint32_t num, int n) {
    n = n % 32;  // Handle n >= 32
    return (num << n) | (num >> (32 - n));
}

// Rotate right (32-bit)
uint32_t rotateRight32(uint32_t num, int n) {
    n = n % 32;
    return (num >> n) | (num << (32 - n));
}

// Rotate left (8-bit)
uint8_t rotateLeft8(uint8_t num, int n) {
    n = n % 8;
    return (num << n) | (num >> (8 - n));
}

// Rotate right (8-bit)
uint8_t rotateRight8(uint8_t num, int n) {
    n = n % 8;
    return (num >> n) | (num << (8 - n));
}

// Generic rotate for any bit width
uint32_t rotateLeft(uint32_t num, int n, int bits) {
    n = n % bits;
    uint32_t mask = (1U << bits) - 1;  // Mask for 'bits' number of bits
    return ((num << n) | (num >> (bits - n))) & mask;
}

uint32_t rotateRight(uint32_t num, int n, int bits) {
    n = n % bits;
    uint32_t mask = (1U << bits) - 1;
    return ((num >> n) | (num << (bits - n))) & mask;
}

void printBinary8(uint8_t num) {
    printf("0b");
    for (int i = 7; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
}

void printBinary32(uint32_t num) {
    printf("0b");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i > 0 && i % 8 == 0) printf("_");
    }
}

int main() {
    printf("=== Rotate Bits ===\n\n");

    // 8-bit examples
    printf("8-bit Rotation:\n\n");
    uint8_t num8 = 0b11010010;

    printf("Original: "); printBinary8(num8); printf(" (%u)\n\n", num8);

    printf("Rotate LEFT:\n");
    for (int i = 1; i <= 3; i++) {
        uint8_t rotated = rotateLeft8(num8, i);
        printf("  By %d: ", i); printBinary8(rotated);
        printf(" (%u)\n", rotated);
    }

    printf("\nRotate RIGHT:\n");
    for (int i = 1; i <= 3; i++) {
        uint8_t rotated = rotateRight8(num8, i);
        printf("  By %d: ", i); printBinary8(rotated);
        printf(" (%u)\n", rotated);
    }

    // Compare with shift
    printf("\n=== Rotate vs Shift ===\n\n");
    printf("Original:       "); printBinary8(num8); printf("\n");
    printf("Shift left 2:   "); printBinary8(num8 << 2); printf(" (loses bits)\n");
    printf("Rotate left 2:  "); printBinary8(rotateLeft8(num8, 2)); printf(" (wraps bits)\n");

    printf("\n=== Formulas ===\n");
    printf("Rotate Left:  (num << n) | (num >> (bits - n))\n");
    printf("Rotate Right: (num >> n) | (num << (bits - n))\n");

    return 0;
}
