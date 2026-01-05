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
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why not use C bit-fields instead?"
 * A1: C bit-fields have implementation-defined behavior:
 *     - Bit order is compiler-dependent
 *     - Padding is unpredictable
 *     - Can't map to hardware registers reliably
 *     Manual masks are portable and explicit!
 *
 * -------------------------------------------------------------------------
 * Q2: "How do you clear a bitfield before setting it?"
 * A2: mask = ((1U << width) - 1) << start;  // Create positioned mask
 *     value &= ~mask;                        // Clear field (AND with NOT mask)
 *     value |= (newVal << start) & mask;     // Set new value
 *     Always clear before setting to avoid OR combining old and new!
 *
 * -------------------------------------------------------------------------
 * Q3: "What are common register field operations?"
 * A3: - Status registers: check specific flag bits
 *     - Control registers: set configuration fields
 *     - UART: baud rate divisor, parity bits
 *     - GPIO: pin mode, pull-up/down configuration
 *
 * -------------------------------------------------------------------------
 * Q4: "How do you handle bit fields that cross byte boundaries?"
 * A4: Same formula works! (value >> start) & ((1U << width) - 1)
 *     The mask and shift handle any alignment. Just be careful with
 *     width > 32 which needs uint64_t.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// Extract 'width' bits starting at position 'start'
uint32_t extractBits(uint32_t value, int start, int width) {
    // Say: "To extract a bitfield, I'll first create a mask with 'width' ones"
    // Say: "The mask is created by shifting 1 left by width and subtracting 1"
    // Create mask with 'width' ones
    uint32_t mask = (1U << width) - 1;

    // Say: "Then I'll shift the value right by 'start' to bring the desired bits to position 0"
    // Say: "Finally, AND with the mask to keep only the desired bits"
    // Shift right by start, then AND with mask
    return (value >> start) & mask;
}

// Extract specific byte (0 = lowest, 3 = highest)
uint8_t extractByte(uint32_t value, int byteNum) {
    // Say: "To extract byte number N, I'll shift right by N times 8"
    // Say: "Then AND with 0xFF to keep only the lowest byte"
    // Shift right by (byteNum * 8) bits, mask with 0xFF
    return (value >> (byteNum * 8)) & 0xFF;
}

// Set 'width' bits starting at position 'start' to 'newValue'
uint32_t setBitfield(uint32_t value, int start, int width, uint32_t newValue) {
    // Say: "To set a bitfield, I'll first create a mask at the correct position"
    // Say: "The mask has 'width' ones starting at position 'start'"
    // Create mask at correct position
    uint32_t mask = ((1U << width) - 1) << start;

    // Say: "Next, I'll clear the existing bits by ANDing with NOT mask"
    // Clear the bits at that position
    value &= ~mask;

    // Say: "Finally, I'll shift the new value left and OR it in"
    // Say: "I'll AND with mask to ensure only the intended bits are set"
    // Set new value (shift left and AND with mask)
    value |= (newValue << start) & mask;

    // Return modified value
    return value;
}

// Common register field macros (embedded style)
#define GET_FIELD(reg, start, width)  (((reg) >> (start)) & ((1U << (width)) - 1))
#define SET_FIELD(reg, start, width, val) \
    (((reg) & ~(((1U << (width)) - 1) << (start))) | (((val) & ((1U << (width)) - 1)) << (start)))

void printBinary32(uint32_t num) {
    // Print binary representation
    for (int i = 31; i >= 0; i--) {
        // Extract and print bit at position i
        printf("%d", (num >> i) & 1);

        // Add separator every 4 bits
        if (i > 0 && i % 4 == 0) printf("_");
    }
}

int main() {
    printf("=== Extract Bitfield from Register ===\n\n");

    // Test register value
    uint32_t reg = 0xABCD1234;
    printf("Register value: 0x%08X\n", reg);
    printf("Binary: "); printBinary32(reg); printf("\n\n");

    // Extract bytes
    printf("1. Extract individual bytes:\n");
    // Loop through all 4 bytes
    for (int i = 0; i < 4; i++) {
        // Extract and print byte i
        printf("   Byte %d (bits %2d-%2d): 0x%02X\n",
               i, i*8, i*8+7, extractByte(reg, i));
    }
    printf("\n");

    // Extract arbitrary bitfields
    printf("2. Extract bitfields:\n");
    // Extract various bitfields
    printf("   Bits 0-3:   0x%X\n", extractBits(reg, 0, 4));
    printf("   Bits 4-7:   0x%X\n", extractBits(reg, 4, 4));
    printf("   Bits 8-15:  0x%02X\n", extractBits(reg, 8, 8));
    printf("   Bits 20-27: 0x%02X\n\n", extractBits(reg, 20, 8));

    // Modify bitfield
    printf("3. Modify bitfield:\n");
    printf("   Original:  0x%08X\n", reg);
    // Set byte 1 (bits 8-15) to 0xFF
    uint32_t modified = setBitfield(reg, 8, 8, 0xFF);
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
