// Rotate bits left/right - Different from shift (bits wrap around)
// Time: O(1), Space: O(1)

#include <stdio.h>
#include <stdint.h>

void print_binary(uint32_t n) {
    printf("0b");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0 && i != 0) printf("_");
    }
}

// Say: "Rotate left - bits that fall off left side come back on right"
uint32_t rotate_left(uint32_t value, int shift) {
    // Say: "Normalize shift to avoid undefined behavior"
    shift = shift % 32;  // Say: "Rotating by 32 is same as rotating by 0"

    if (shift == 0) return value;

    // Say: "Left part: shift value left by 'shift' positions"
    // Say: "Right part: take bits that fell off (shift right by 32-shift)"
    // Say: "OR them together to complete the rotation"
    return (value << shift) | (value >> (32 - shift));
}

// Say: "Rotate right - bits that fall off right side come back on left"
uint32_t rotate_right(uint32_t value, int shift) {
    // Say: "Normalize shift"
    shift = shift % 32;

    if (shift == 0) return value;

    // Say: "Right part: shift value right by 'shift' positions"
    // Say: "Left part: take bits that fell off (shift left by 32-shift)"
    return (value >> shift) | (value << (32 - shift));
}

// 8-bit versions (common in embedded)
uint8_t rotate_left_8(uint8_t value, int shift) {
    shift = shift % 8;
    if (shift == 0) return value;
    return (value << shift) | (value >> (8 - shift));
}

uint8_t rotate_right_8(uint8_t value, int shift) {
    shift = shift % 8;
    if (shift == 0) return value;
    return (value >> shift) | (value << (8 - shift));
}

int main() {
    uint32_t val = 0x80000001;  // MSB and LSB set

    printf("Original value: 0x%08X\n", val);
    print_binary(val);
    printf("\n\n");

    // Rotate left by 1
    uint32_t rotl = rotate_left(val, 1);
    printf("Rotate left by 1:  0x%08X\n", rotl);
    print_binary(rotl);
    printf("\n\n");

    // Rotate right by 1
    uint32_t rotr = rotate_right(val, 1);
    printf("Rotate right by 1: 0x%08X\n", rotr);
    print_binary(rotr);
    printf("\n\n");

    // Demonstrate with 8-bit value
    uint8_t byte = 0b10110001;
    printf("8-bit value: 0x%02X (", byte);
    for (int i = 7; i >= 0; i--) printf("%d", (byte >> i) & 1);
    printf(")\n");

    uint8_t rotl8 = rotate_left_8(byte, 2);
    printf("Rotate left 2:  0x%02X (", rotl8);
    for (int i = 7; i >= 0; i--) printf("%d", (rotl8 >> i) & 1);
    printf(")\n");

    uint8_t rotr8 = rotate_right_8(byte, 2);
    printf("Rotate right 2: 0x%02X (", rotr8);
    for (int i = 7; i >= 0; i--) printf("%d", (rotr8 >> i) & 1);
    printf(")\n");

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Rotation is circular shift - bits that fall off one end wrap to the other.

 SHIFT vs ROTATE:
 - Shift: bits fall off and are lost, zeros fill in
 - Rotate: bits wrap around, no information lost

 Example (8-bit, rotate left by 2):
 Before: 1011_0001
 After:  1100_0110
 The '10' from left wrapped to right side

 ROTATE LEFT FORMULA:
 (value << shift) | (value >> (bits - shift))

 Why this works:
 - (value << shift): Move bits left, zeros on right
 - (value >> (bits-shift)): Get bits that would fall off
 - OR combines them

 ROTATE RIGHT FORMULA:
 (value >> shift) | (value << (bits - shift))

 WHY MODULO THE SHIFT:
 - Rotating by 32 is same as rotating by 0
 - Prevents undefined behavior for large shifts
 - shift = shift % 32

 EMBEDDED USES:
 1. Cryptography algorithms (many use rotations)
 2. Circular buffers index wrapping
 3. CRC calculations
 4. Hash functions
 5. Some communication protocols

 HARDWARE INSTRUCTIONS:
 - Many processors have ROL/ROR instructions
 - Compiler may optimize this pattern to single instruction
 - ARM: ROR, RRX instructions
 - x86: ROL, ROR instructions

 COMMON MISTAKE:
 - Using shift instead of rotate (losing bits)
 - Not handling shift = 0 case
 - Using signed integers (undefined for negative shift)"
*/
