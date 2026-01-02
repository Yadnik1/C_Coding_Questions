/*
 * ============================================================================
 * PROBLEM: Extract Byte or Bitfield from Register
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH (Embedded)
 *
 * Extract specific bits from a register value.
 * This is one of the MOST COMMON embedded interview questions.
 *
 * Example:
 * Input:  value = 0xABCD1234, start = 8, width = 8
 * Output: 0x12 (bits 8-15)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. REGISTER LAYOUT:
 *    Registers often have multiple fields packed into one value.
 *
 *    Example: 32-bit status register
 *    Bits 31-24: Error code (8 bits)
 *    Bits 23-16: Device ID (8 bits)
 *    Bits 15-8:  Status flags (8 bits)
 *    Bits 7-0:   Count value (8 bits)
 *
 * 2. WHY EXTRACT BITFIELDS?
 *    - Hardware registers pack multiple values
 *    - Protocol headers have bit-level fields
 *    - Memory-mapped I/O requires precise bit access
 *
 * 3. TWO OPERATIONS:
 *    - EXTRACT: Read bits from a register
 *    - SET: Write bits to a register (without disturbing other bits)
 *
 * ============================================================================
 * EXTRACT FORMULA:
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
 * UNDERSTANDING THE MASK: (1 << width) - 1
 * ============================================================================
 *
 * This creates a mask with exactly 'width' consecutive 1s:
 *
 *   width = 1:  (1 << 1) - 1 = 2 - 1 = 1     = 0b0001
 *   width = 2:  (1 << 2) - 1 = 4 - 1 = 3     = 0b0011
 *   width = 3:  (1 << 3) - 1 = 8 - 1 = 7     = 0b0111
 *   width = 4:  (1 << 4) - 1 = 16 - 1 = 15   = 0b1111
 *   width = 8:  (1 << 8) - 1 = 256 - 1 = 255 = 0b11111111 = 0xFF
 *
 * Why does this work?
 *   1 << width gives us a 1 followed by 'width' zeros
 *   Subtracting 1 flips all those zeros to ones
 *
 *   Example: width = 4
 *   1 << 4 = 0b10000
 *   0b10000 - 1 = 0b01111  ← exactly 4 ones!
 *
 * ============================================================================
 * STEP-BY-STEP VISUALIZATION: EXTRACT BITS
 * ============================================================================
 *
 * Value: 0xABCD1234
 *
 * In binary (with positions labeled):
 *
 *   Position: 31      24 23      16 15       8 7        0
 *             |        | |        | |        | |        |
 *   Binary:   1010_1011 1100_1101 0001_0010 0011_0100
 *   Hex:         A    B    C    D    1    2    3    4
 *   Byte:        Byte 3   Byte 2   Byte 1   Byte 0
 *
 * -------------------------------------------------------------------------
 * EXAMPLE 1: Extract byte 2 (bits 16-23)
 * -------------------------------------------------------------------------
 *
 *   extractBits(0xABCD1234, 16, 8)
 *
 *   Step 1: Shift right by 16 (bring bits 16-23 to position 0-7)
 *
 *   Before: 1010_1011_1100_1101_0001_0010_0011_0100
 *   >> 16:  0000_0000_0000_0000_1010_1011_1100_1101
 *                               ^^^^^^^^_^^^^^^^^
 *                               These were bits 16-31
 *
 *   Step 2: Create mask for 8 bits
 *
 *   (1 << 8) - 1 = 256 - 1 = 255 = 0xFF = 0b11111111
 *
 *   Step 3: AND with mask
 *
 *     0000_0000_0000_0000_1010_1011_1100_1101
 *   & 0000_0000_0000_0000_0000_0000_1111_1111
 *     ----------------------------------------
 *     0000_0000_0000_0000_0000_0000_1100_1101
 *
 *   Result: 0xCD ← This was byte 2!
 *
 * -------------------------------------------------------------------------
 * EXAMPLE 2: Extract bits 4-7 (4 bits starting at position 4)
 * -------------------------------------------------------------------------
 *
 *   extractBits(0xABCD1234, 4, 4)
 *
 *   Value: 0011_0100 (looking at low byte: 0x34)
 *                ^^^^
 *          bits 4-7 = 0011 = 0x3
 *
 *   Step 1: Shift right by 4
 *
 *   Before: ...0011_0100
 *   >> 4:   ...0000_0011  (shifted out: 0100)
 *
 *   Step 2: Mask with 4 bits
 *
 *   (1 << 4) - 1 = 15 = 0xF = 0b1111
 *
 *   Step 3: AND
 *
 *   ...0000_0011 & 0xF = 0x3
 *
 *   Result: 0x3
 *
 * ============================================================================
 * STEP-BY-STEP VISUALIZATION: SET BITFIELD
 * ============================================================================
 *
 * To SET a bitfield, we need to:
 *   1. CLEAR the existing bits at that position
 *   2. SHIFT the new value to the correct position
 *   3. OR it into the register
 *
 * Formula:
 *   value = (value & ~mask) | ((newValue << start) & mask)
 *
 * -------------------------------------------------------------------------
 * EXAMPLE: Set bits 8-15 to 0xFF in 0xABCD1234
 * -------------------------------------------------------------------------
 *
 *   setBitfield(0xABCD1234, 8, 8, 0xFF)
 *
 *   Original: 0xABCD1234 = 1010_1011_1100_1101_0001_0010_0011_0100
 *                                              ^^^^^^^^
 *                                              bits 8-15 = 0x12
 *
 *   Step 1: Create mask at position
 *
 *   mask = ((1 << 8) - 1) << 8
 *        = 0xFF << 8
 *        = 0x0000FF00
 *        = 0000_0000_0000_0000_1111_1111_0000_0000
 *
 *   Step 2: Clear those bits with ~mask
 *
 *   ~mask = 0xFFFF00FF
 *         = 1111_1111_1111_1111_0000_0000_1111_1111
 *
 *   value & ~mask:
 *     1010_1011_1100_1101_0001_0010_0011_0100
 *   & 1111_1111_1111_1111_0000_0000_1111_1111
 *     ----------------------------------------
 *     1010_1011_1100_1101_0000_0000_0011_0100
 *                         ^^^^^^^^
 *                         Cleared!
 *
 *   = 0xABCD0034
 *
 *   Step 3: Shift new value into position
 *
 *   0xFF << 8 = 0x0000FF00
 *
 *   Step 4: OR into cleared register
 *
 *     1010_1011_1100_1101_0000_0000_0011_0100  (cleared)
 *   | 0000_0000_0000_0000_1111_1111_0000_0000  (new value shifted)
 *     ----------------------------------------
 *     1010_1011_1100_1101_1111_1111_0011_0100
 *
 *   Result: 0xABCDFF34
 *           Bits 8-15 are now 0xFF!
 *
 * ============================================================================
 * EXTRACTING INDIVIDUAL BYTES
 * ============================================================================
 *
 * For a 32-bit value, bytes are numbered 0-3 from LSB to MSB:
 *
 *   Value: 0xABCD1234
 *
 *   Byte 0 (bits 0-7):   (value >> 0) & 0xFF  = 0x34
 *   Byte 1 (bits 8-15):  (value >> 8) & 0xFF  = 0x12
 *   Byte 2 (bits 16-23): (value >> 16) & 0xFF = 0xCD
 *   Byte 3 (bits 24-31): (value >> 24) & 0xFF = 0xAB
 *
 * General formula: (value >> (byteNum * 8)) & 0xFF
 *
 * ============================================================================
 * REAL-WORLD EMBEDDED EXAMPLE: UART STATUS REGISTER
 * ============================================================================
 *
 * Suppose UART_STATUS at address 0x40011000 has this layout:
 *
 *   Bit 7:   TX Complete
 *   Bit 6:   TX Empty
 *   Bit 5:   RX Full
 *   Bit 4:   RX Not Empty
 *   Bit 3-2: Parity Error Count (2 bits)
 *   Bit 1:   Framing Error
 *   Bit 0:   Overrun Error
 *
 * To check if TX is complete:
 *   if ((*UART_STATUS >> 7) & 1) { ... }
 *
 * To get parity error count:
 *   int parity_errors = (*UART_STATUS >> 2) & 0x3;  // 2-bit field
 *
 * To clear overrun error (write 1 to clear):
 *   *UART_STATUS |= (1 << 0);
 *
 * ============================================================================
 * COMMON MACROS FOR BITFIELD ACCESS
 * ============================================================================
 *
 * #define GET_FIELD(reg, start, width) \
 *     (((reg) >> (start)) & ((1U << (width)) - 1))
 *
 * #define SET_FIELD(reg, start, width, val) \
 *     (((reg) & ~(((1U << (width)) - 1) << (start))) | \
 *      (((val) & ((1U << (width)) - 1)) << (start)))
 *
 * Usage:
 *   uint8_t parity_count = GET_FIELD(uart_status, 2, 2);
 *   uart_config = SET_FIELD(uart_config, 4, 3, 0x5);
 *
 * ============================================================================
 * TIME COMPLEXITY: O(1)
 * ============================================================================
 * - Shift and AND are single CPU instructions
 * - Constant time operations
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - No extra memory needed
 * - Just arithmetic operations
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Extract full width (width = 32): mask overflow, use special handling
 * 2. Start + width > 32: Undefined behavior, validate inputs
 * 3. Width = 0: Returns 0 (mask = 0)
 * 4. Start = 0: No shift needed, just mask
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why do we shift right BEFORE masking, not after?"
 * A1: Shifting first brings the desired bits to position 0, making the
 *     mask simpler. Otherwise we'd need a mask at the original position
 *     and then still have to shift to get the actual value.
 *
 *     Compare:
 *     Method 1: (value >> start) & mask      ← Simple mask
 *     Method 2: (value & (mask << start)) >> start  ← Extra operations!
 *
 * -------------------------------------------------------------------------
 * Q2: "Why use (1 << width) - 1 for the mask? Why not hardcode 0xFF?"
 * A2: (1 << width) - 1 creates a mask of ANY width dynamically.
 *     Hardcoding 0xFF only works for 8-bit extractions.
 *
 *     For a 3-bit field, you need 0b111 = 7 = (1 << 3) - 1
 *     For a 5-bit field, you need 0b11111 = 31 = (1 << 5) - 1
 *
 * -------------------------------------------------------------------------
 * Q3: "What happens if width is 32? What's (1 << 32) - 1?"
 * A3: UNDEFINED BEHAVIOR! Shifting by the bit width is UB in C.
 *     For 32-bit extraction, use ~0U (all 1s) instead:
 *
 *     if (width == 32) mask = ~0U;
 *     else mask = (1U << width) - 1;
 *
 * -------------------------------------------------------------------------
 * Q4: "How would you set ONLY bits 4-7 to 0xA without affecting other bits?"
 * A4: Three steps:
 *     1. Create mask at position: mask = 0xF << 4 = 0xF0
 *     2. Clear those bits: value &= ~0xF0 (keeps other bits, clears 4-7)
 *     3. Set new value: value |= (0xA << 4)
 *
 *     Combined: value = (value & ~0xF0) | (0xA << 4)
 *
 * -------------------------------------------------------------------------
 * Q5: "In embedded, why use volatile when accessing hardware registers?"
 * A5: volatile tells the compiler:
 *     - The value can change at any time (hardware can modify it)
 *     - Don't optimize away reads/writes
 *     - Don't reorder accesses
 *
 *     Without volatile, compiler might:
 *     - Cache the register value in a CPU register
 *     - Remove "redundant" reads
 *     - Reorder operations (breaking timing requirements)
 *
 *     Example:
 *     volatile uint32_t *STATUS_REG = (volatile uint32_t *)0x40001000;
 *
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
