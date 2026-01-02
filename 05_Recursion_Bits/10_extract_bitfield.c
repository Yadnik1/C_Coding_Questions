/*
 * ============================================================================
 * PROBLEM: Extract Byte or Bitfield from Register
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH (Embedded)
 *
 * Extract specific bits from a register value.
 *
 * ============================================================================
 * FORMULA:
 * ============================================================================
 *
 *   Extract bits from position 'start' to 'start + width - 1':
 *
 *   result = (value >> start) & ((1 << width) - 1)
 *
 *   Breakdown:
 *   1. (value >> start)     - Shift desired bits to position 0
 *   2. (1 << width) - 1     - Create mask with 'width' ones
 *   3. & mask               - Keep only the desired bits
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   Value: 0xABCD1234 = 0b1010_1011_1100_1101_0001_0010_0011_0100
 *
 *   Extract byte 2 (bits 16-23):
 *   Step 1: >> 16 = 0b0000_0000_0000_0000_1010_1011_1100_1101
 *   Step 2: & 0xFF = 0b0000_0000_0000_0000_0000_0000_1100_1101
 *   Result: 0xCD
 *
 *   Extract bits 4-7 (4 bits starting at position 4):
 *   Step 1: >> 4 = 0b0000_1010_1011_1100_1101_0001_0010_0011
 *   Step 2: & 0xF = 0b0000_0000_0000_0000_0000_0000_0000_0011
 *   Result: 0x3
 *
 * ============================================================================
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// Extract 'width' bits starting at position 'start'
uint32_t extractBits(uint32_t value, int start, int width) {
    uint32_t mask = (1U << width) - 1;  // Create mask with 'width' ones
    return (value >> start) & mask;
}

// Extract specific byte (0 = lowest, 3 = highest)
uint8_t extractByte(uint32_t value, int byteNum) {
    return (value >> (byteNum * 8)) & 0xFF;
}

// Set 'width' bits starting at position 'start' to 'newValue'
uint32_t setBitfield(uint32_t value, int start, int width, uint32_t newValue) {
    uint32_t mask = ((1U << width) - 1) << start;  // Mask at correct position
    value &= ~mask;                                 // Clear bits
    value |= (newValue << start) & mask;           // Set new value
    return value;
}

// Common register field macros (embedded style)
#define GET_FIELD(reg, start, width)  (((reg) >> (start)) & ((1U << (width)) - 1))
#define SET_FIELD(reg, start, width, val) \
    (((reg) & ~(((1U << (width)) - 1) << (start))) | (((val) & ((1U << (width)) - 1)) << (start)))

void printBinary32(uint32_t num) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
        if (i > 0 && i % 4 == 0) printf("_");
    }
}

int main() {
    printf("=== Extract Bitfield from Register ===\n\n");

    uint32_t reg = 0xABCD1234;
    printf("Register value: 0x%08X\n", reg);
    printf("Binary: "); printBinary32(reg); printf("\n\n");

    // Extract bytes
    printf("1. Extract individual bytes:\n");
    for (int i = 0; i < 4; i++) {
        printf("   Byte %d (bits %2d-%2d): 0x%02X\n",
               i, i*8, i*8+7, extractByte(reg, i));
    }
    printf("\n");

    // Extract arbitrary bitfields
    printf("2. Extract bitfields:\n");
    printf("   Bits 0-3:   0x%X\n", extractBits(reg, 0, 4));
    printf("   Bits 4-7:   0x%X\n", extractBits(reg, 4, 4));
    printf("   Bits 8-15:  0x%02X\n", extractBits(reg, 8, 8));
    printf("   Bits 20-27: 0x%02X\n\n", extractBits(reg, 20, 8));

    // Modify bitfield
    printf("3. Modify bitfield:\n");
    printf("   Original:  0x%08X\n", reg);
    uint32_t modified = setBitfield(reg, 8, 8, 0xFF);  // Set byte 1 to 0xFF
    printf("   Set byte 1 to 0xFF: 0x%08X\n\n", modified);

    // Using macros
    printf("4. Using macros:\n");
    printf("   GET_FIELD(0x%08X, 4, 4) = 0x%X\n", reg, GET_FIELD(reg, 4, 4));
    printf("   SET_FIELD(0x%08X, 4, 4, 0xA) = 0x%08X\n",
           reg, SET_FIELD(reg, 4, 4, 0xA));

    printf("\n=== Formula ===\n");
    printf("Extract: (value >> start) & ((1 << width) - 1)\n");
    printf("Set:     (value & ~mask) | ((newVal << start) & mask)\n");

    return 0;
}
