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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why normalize n with modulo?"
 * A1: Rotating by 32 (or any multiple) returns the original value.
 *     n % 32 handles large rotation counts efficiently.
 *     Without it, shifting by >= word size is undefined behavior in C!
 *
 * -------------------------------------------------------------------------
 * Q2: "Is there a hardware rotate instruction?"
 * A2: Yes! x86 has ROL (rotate left) and ROR (rotate right).
 *     ARM has ROR. GCC may optimize shift-OR pattern to use hardware rotate.
 *     Use __builtin intrinsics for guaranteed hardware instruction.
 *
 * -------------------------------------------------------------------------
 * Q3: "Where is bit rotation used in embedded?"
 * A3: - CRC calculations (polynomial division)
 *     - Cryptographic algorithms (SHA, MD5, AES)
 *     - Hash functions
 *     - Circular buffer indexing (power-of-2 size)
 *
 * -------------------------------------------------------------------------
 * Q4: "What's the difference between logical and arithmetic shift?"
 * A4: Logical shift: fills with zeros (used for unsigned).
 *     Arithmetic shift right: fills with sign bit (used for signed).
 *     Rotate: no fill needed, bits wrap around. Always use unsigned for rotate!
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// Rotate left (32-bit)
uint32_t rotateLeft32(uint32_t num, int n) {
    // Say: "First, I'll normalize n using modulo to handle values >= 32"
    // Handle n >= 32 by taking modulo
    n = n % 32;

    // Say: "To rotate left, I'll shift num left by n positions"
    // Say: "Then OR it with num shifted right by 32 minus n to wrap the bits"
    // Rotate: shift left n bits, OR with wrapped bits from right
    return (num << n) | (num >> (32 - n));
}

// Rotate right (32-bit)
uint32_t rotateRight32(uint32_t num, int n) {
    // Say: "First, I'll normalize n using modulo"
    // Handle n >= 32
    n = n % 32;

    // Say: "To rotate right, I'll shift num right by n positions"
    // Say: "Then OR it with num shifted left by 32 minus n to wrap the bits"
    // Rotate: shift right n bits, OR with wrapped bits from left
    return (num >> n) | (num << (32 - n));
}

// Rotate left (8-bit)
uint8_t rotateLeft8(uint8_t num, int n) {
    // Say: "For 8-bit rotation, the concept is the same but with 8 bits"
    // Handle n >= 8
    n = n % 8;

    // Say: "Shift left by n and OR with right shift by 8 minus n"
    // Rotate left with 8-bit width
    return (num << n) | (num >> (8 - n));
}

// Rotate right (8-bit)
uint8_t rotateRight8(uint8_t num, int n) {
    // Say: "Same concept: shift right by n and wrap around from left"
    // Handle n >= 8
    n = n % 8;

    // Rotate right with 8-bit width
    return (num >> n) | (num << (8 - n));
}

// Generic rotate for any bit width
uint32_t rotateLeft(uint32_t num, int n, int bits) {
    // Say: "For generic rotation, I'll normalize n based on bit width"
    // Normalize n
    n = n % bits;

    // Say: "I'll create a mask to keep only the lower 'bits' bits"
    // Create mask for specified bit width
    uint32_t mask = (1U << bits) - 1;

    // Say: "Then perform rotation and mask the result"
    // Rotate and mask to bit width
    return ((num << n) | (num >> (bits - n))) & mask;
}

uint32_t rotateRight(uint32_t num, int n, int bits) {
    // Normalize n
    n = n % bits;

    // Create mask for specified bit width
    uint32_t mask = (1U << bits) - 1;

    // Rotate right and mask to bit width
    return ((num >> n) | (num << (bits - n))) & mask;
}

void printBinary8(uint8_t num) {
    // Print binary prefix
    printf("0b");

    // Loop through 8 bits
    for (int i = 7; i >= 0; i--) {
        // Extract and print bit at position i
        printf("%d", (num >> i) & 1);
    }
}

void printBinary32(uint32_t num) {
    // Print binary prefix
    printf("0b");

    // Loop through 32 bits
    for (int i = 31; i >= 0; i--) {
        // Extract and print bit at position i
        printf("%d", (num >> i) & 1);

        // Add separator every 8 bits
        if (i > 0 && i % 8 == 0) printf("_");
    }
}

int main() {
    printf("=== Rotate Bits ===\n\n");

    // 8-bit examples
    printf("8-bit Rotation:\n\n");
    // Test value
    uint8_t num8 = 0b11010010;

    printf("Original: "); printBinary8(num8); printf(" (%u)\n\n", num8);

    printf("Rotate LEFT:\n");
    // Demonstrate left rotations
    for (int i = 1; i <= 3; i++) {
        // Rotate left by i positions
        uint8_t rotated = rotateLeft8(num8, i);
        printf("  By %d: ", i); printBinary8(rotated);
        printf(" (%u)\n", rotated);
    }

    printf("\nRotate RIGHT:\n");
    // Demonstrate right rotations
    for (int i = 1; i <= 3; i++) {
        // Rotate right by i positions
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
