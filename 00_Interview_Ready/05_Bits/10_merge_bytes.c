/*
 * ============================================================================
 * PROBLEM: Merge Two Nibbles into a Byte and Split Back
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Combine two 4-bit nibbles into one 8-bit byte, and split a byte back into
 * its two nibbles. This is the foundation for all byte/word merging in embedded.
 *
 * SIMPLIFIED FOR PRACTICE:
 * Instead of 32-bit values, we work with 8-bit values (easier to visualize!)
 * Same concepts apply to 16-bit, 32-bit, etc.
 *
 * ============================================================================
 * EXAMPLES (8-bit values with binary):
 * ============================================================================
 *
 * MERGE two 4-bit nibbles into 8-bit byte:
 *
 *   high_nibble = 0x5 = 0101 (binary)
 *   low_nibble  = 0xA = 1010 (binary)
 *
 *   Step 1: Shift high nibble left by 4
 *   0x5 << 4 = 0101_0000 = 0x50
 *
 *   Step 2: OR with low nibble
 *   0101_0000 | 0000_1010 = 0101_1010 = 0x5A
 *
 *   VISUAL:
 *   +------+------+
 *   | 0101 | 1010 |  = 0x5A
 *   +------+------+
 *   bits 7-4  3-0
 *    high    low
 *
 * ============================================================================
 *
 * SPLIT 8-bit byte into two 4-bit nibbles:
 *
 *   value = 0x5A = 0101_1010
 *
 *   Get high nibble: (0x5A >> 4) & 0x0F
 *     0101_1010 >> 4 = 0000_0101 = 0x05
 *     0000_0101 & 0x0F = 0x05
 *
 *   Get low nibble: 0x5A & 0x0F
 *     0101_1010 & 0000_1111 = 0000_1010 = 0x0A
 *
 *   VISUAL:
 *   0101_1010
 *   ^^^^ ^^^^
 *   high low
 *   0x5  0xA
 *
 * ============================================================================
 *
 * WHY IS THIS ASKED IN EMBEDDED INTERVIEWS?
 * - Receiving multi-byte data over UART/SPI/I2C
 * - Parsing sensor readings (temperature, ADC, etc.)
 * - Working with packed data structures
 * - Same pattern scales to 16-bit, 32-bit, 64-bit
 *
 * KEY FORMULAS:
 *   MERGE:  (high << 4) | low
 *   SPLIT:  high = (value >> 4) & 0x0F
 *           low  = value & 0x0F
 *
 * TIME COMPLEXITY: O(1)
 * SPACE COMPLEXITY: O(1)
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/* ==================== HELPER: Print 8-bit binary ==================== */

void print_binary_8(uint8_t n) {
    // Say: "Print 8 bits with separator between nibbles"
    printf("0b");
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i == 4) printf("_");  // Separator between nibbles
    }
}

/* ==================== SOLUTION 1: MERGE NIBBLES ==================== */

/*
 * ============================================================================
 * DRY-RUN DIAGRAM: Merge Two Nibbles
 * ============================================================================
 *
 * INPUT: high = 0x5 (0101), low = 0xA (1010)
 *
 * STEP 1: Shift high nibble to upper position
 * -----------------------------------------
 *   high << 4:
 *
 *   Before shift:  0000_0101  (0x05)
 *   After shift:   0101_0000  (0x50)
 *
 *   VISUAL:
 *   0101 moves from bits [3:0] to bits [7:4]
 *
 *        Before:  0000_0101
 *                      ^^^^
 *                      here
 *
 *        After:   0101_0000
 *                 ^^^^
 *                 now here
 *
 * STEP 2: OR with low nibble
 * --------------------------
 *     0101_0000   (high << 4)
 *   | 0000_1010   (low)
 *   -----------
 *     0101_1010   (0x5A)
 *
 *   VISUAL:
 *   +------+------+
 *   | 0101 | 1010 |  = 0x5A (result!)
 *   +------+------+
 *    high    low
 *
 * ============================================================================
 */
uint8_t merge_nibbles(uint8_t high, uint8_t low) {
    /*
     * Say: "Merge two 4-bit nibbles into one 8-bit byte"
     *
     * high goes to bits [7:4] (upper nibble)
     * low  goes to bits [3:0] (lower nibble)
     */

    // Say: "Shift high nibble left by 4 to put it in upper position"
    uint8_t high_shifted = high << 4;
    // high = 0101 (0x5)
    // high << 4 = 0101_0000 (0x50)

    // Say: "OR combines them - each bit comes from one or the other"
    uint8_t result = high_shifted | low;
    // 0101_0000 | 0000_1010 = 0101_1010 (0x5A)

    return result;

    // One-liner version:
    // return (high << 4) | low;
}

/* ==================== SOLUTION 2: SPLIT BYTE ==================== */

/*
 * ============================================================================
 * DRY-RUN DIAGRAM: Split Byte into Nibbles
 * ============================================================================
 *
 * INPUT: value = 0x5A = 0101_1010
 *
 * GOAL: Extract high nibble (0x5) and low nibble (0xA)
 *
 * EXTRACT HIGH NIBBLE: (value >> 4) & 0x0F
 * -----------------------------------------
 *
 *   Step 1: Shift right by 4
 *     0101_1010 >> 4 = 0000_0101
 *
 *     VISUAL:
 *     Before: 0101_1010
 *             ^^^^
 *             high nibble here
 *
 *     After:  0000_0101
 *                  ^^^^
 *                  now in low position
 *
 *   Step 2: Mask with 0x0F (to be safe/clear upper bits)
 *     0000_0101 & 0000_1111 = 0000_0101 = 0x05
 *
 * EXTRACT LOW NIBBLE: value & 0x0F
 * --------------------------------
 *
 *   Mask directly (no shift needed):
 *     0101_1010 & 0000_1111 = 0000_1010 = 0x0A
 *
 *     VISUAL:
 *     0101_1010
 *   & 0000_1111  (mask = 0x0F)
 *   -----------
 *     0000_1010  (0x0A)
 *
 *   The mask 0x0F keeps only the lower 4 bits!
 *
 * RESULT: high = 0x05, low = 0x0A
 *
 * ============================================================================
 */
void split_byte(uint8_t value, uint8_t *high, uint8_t *low) {
    /*
     * Say: "Split one 8-bit byte into two 4-bit nibbles"
     *
     * Extract bits [7:4] for high nibble
     * Extract bits [3:0] for low nibble
     */

    // Say: "Shift right by 4 to bring high nibble to low position, then mask"
    *high = (value >> 4) & 0x0F;
    // value = 0101_1010
    // value >> 4 = 0000_0101
    // & 0x0F = 0000_0101 (0x05)

    // Say: "Mask with 0x0F to extract low nibble directly"
    *low = value & 0x0F;
    // 0101_1010 & 0000_1111 = 0000_1010 (0x0A)
}

/* ==================== TEST ==================== */

int main() {
    printf("=== MERGE NIBBLES ===\n\n");

    uint8_t high_nibble = 0x5;  // 0101 in binary
    uint8_t low_nibble = 0xA;   // 1010 in binary

    printf("High nibble: 0x%X = ", high_nibble);
    print_binary_8(high_nibble);
    printf("\n");

    printf("Low nibble:  0x%X = ", low_nibble);
    print_binary_8(low_nibble);
    printf("\n\n");

    uint8_t merged = merge_nibbles(high_nibble, low_nibble);

    printf("Merged:      0x%02X = ", merged);
    print_binary_8(merged);
    printf("\n");

    printf("\n=== SPLIT BYTE ===\n\n");

    uint8_t value = 0x5A;
    printf("Value to split: 0x%02X = ", value);
    print_binary_8(value);
    printf("\n\n");

    uint8_t extracted_high, extracted_low;
    split_byte(value, &extracted_high, &extracted_low);

    printf("Extracted high: 0x%X = ", extracted_high);
    print_binary_8(extracted_high);
    printf("\n");

    printf("Extracted low:  0x%X = ", extracted_low);
    print_binary_8(extracted_low);
    printf("\n");

    printf("\n=== VERIFY ROUNDTRIP ===\n\n");

    // Split and merge back - should get original value
    uint8_t original = 0xAB;
    printf("Original: 0x%02X = ", original);
    print_binary_8(original);
    printf("\n");

    uint8_t h, l;
    split_byte(original, &h, &l);
    printf("Split -> high=0x%X, low=0x%X\n", h, l);

    uint8_t reconstructed = merge_nibbles(h, l);
    printf("Merged back: 0x%02X = ", reconstructed);
    print_binary_8(reconstructed);
    printf("\n");

    printf("Match: %s\n", (original == reconstructed) ? "YES" : "NO");

    return 0;
}

/*
 * ============================================================================
 * INTERVIEW EXPLANATION
 * ============================================================================
 *
 * MERGE NIBBLES (combine two 4-bit values into one 8-bit value):
 *
 *   Formula: (high << 4) | low
 *
 *   Visual:
 *   high = 0101, low = 1010
 *
 *   high << 4:  0101_0000
 *   low:        0000_1010
 *   OR result:  0101_1010  (0x5A)
 *
 * SPLIT BYTE (extract two 4-bit values from one 8-bit value):
 *
 *   Formula: high = (value >> 4) & 0x0F
 *            low  = value & 0x0F
 *
 *   Visual for 0x5A = 0101_1010:
 *
 *   High: shift right 4, then mask
 *   0101_1010 >> 4 = 0000_0101 = 0x05
 *
 *   Low: just mask (no shift needed)
 *   0101_1010 & 0x0F = 0000_1010 = 0x0A
 *
 * SCALING TO LARGER TYPES:
 *
 *   16-bit from 2 bytes:  (high << 8) | low
 *   32-bit from 4 bytes:  (b3 << 24) | (b2 << 16) | (b1 << 8) | b0
 *
 *   CRITICAL: Cast bytes before shifting large amounts!
 *   WRONG: uint8_t b = 0x12; b << 24;  // overflow!
 *   RIGHT: ((uint32_t)b) << 24;        // correct!
 *
 * COMMON MISTAKES:
 * - Forgetting to mask when extracting (can have garbage bits)
 * - Wrong shift amount (4 for nibbles, 8 for bytes, etc.)
 * - Not casting to larger type before large shifts
 *
 * EMBEDDED EXAMPLE:
 *   // Receive 2-byte sensor reading over UART
 *   uint8_t high = uart_receive();
 *   uint8_t low = uart_receive();
 *   uint16_t temperature = ((uint16_t)high << 8) | low;
 *
 * ============================================================================
 */
