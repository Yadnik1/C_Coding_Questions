/*
 * ============================================================================
 * PROBLEM: Extract and Insert Bit Fields (Masking and Shifting)
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Extract a group of consecutive bits from a value, or insert a value into
 * specific bit positions within a register. This is ESSENTIAL for working
 * with hardware registers where multiple fields are packed into one word.
 *
 * EXAMPLES (with binary representation):
 *   Register: 0x00001234 = 0000_0000_0000_0000_0001_0010_0011_0100
 *                                              ^^^^_^^^^_^^^^_^^^^
 *
 *   Layout: [31:16]=Reserved, [15:8]=Count, [7:4]=Error, [3:0]=Status
 *
 *   EXTRACT bits [3:0] (Status):
 *   - Shift right by 0, mask with 0xF (4 bits)
 *   - Result: 0x4 = 0100 (decimal 4)
 *
 *   EXTRACT bits [7:4] (Error):
 *   - Shift right by 4, mask with 0xF (4 bits)
 *   - Result: 0x3 = 0011 (decimal 3)
 *
 *   EXTRACT bits [15:8] (Count):
 *   - Shift right by 8, mask with 0xFF (8 bits)
 *   - Result: 0x12 = 0001_0010 (decimal 18)
 *
 *   INSERT value 0xF into bits [7:4] (Error):
 *   - Clear bits [7:4]: reg & ~(0xF << 4) = 0x00001204
 *   - Insert new value: 0x00001204 | (0xF << 4) = 0x000012F4
 *
 * WHY IS THIS ASKED IN EMBEDDED INTERVIEWS?
 * - Hardware registers pack multiple fields into one word
 * - GPIO configuration (mode, speed, pull-up all in one register)
 * - Interrupt flags and enable bits
 * - Protocol headers (pack multiple fields efficiently)
 * - Memory-mapped I/O manipulation
 * - THIS IS DAILY WORK FOR EMBEDDED ENGINEERS!
 *
 * KEY CONCEPT - EXTRACT AND INSERT:
 *   EXTRACT: (value >> pos) & ((1 << width) - 1)
 *            1. Shift field to position 0
 *            2. Mask with 'width' ones
 *
 *   INSERT:  (target & ~(mask << pos)) | ((field & mask) << pos)
 *            1. Clear the target bits (create hole)
 *            2. Mask and shift new value
 *            3. OR to insert
 *
 * VISUAL:
 *   EXTRACT bits [7:4] from 0x1234:
 *
 *   Original:      0001_0010_0011_0100  (0x1234)
 *                       ^^^^            bits [7:4]
 *
 *   Step 1: Shift right by 4
 *   Shifted:       0000_0001_0010_0011  (0x123)
 *                                 ^^^^  now at position [3:0]
 *
 *   Step 2: Mask with 0xF (4 bits)
 *   Mask:          0000_0000_0000_1111  (0xF)
 *   Result:        0000_0000_0000_0011  (0x3)
 *
 *   -----------------------------------------------
 *
 *   INSERT 0xF into bits [7:4]:
 *
 *   Original:      0001_0010_0011_0100  (0x1234)
 *
 *   Step 1: Create inverted mask at position
 *   Mask << 4:     0000_0000_1111_0000  (0xF0)
 *   Inverted:      1111_1111_0000_1111  (~0xF0)
 *
 *   Step 2: Clear bits [7:4]
 *   AND result:    0001_0010_0000_0100  (0x1204)
 *                            ^^^^       hole created
 *
 *   Step 3: Shift new value to position
 *   0xF << 4:      0000_0000_1111_0000  (0xF0)
 *
 *   Step 4: OR to insert
 *   Final:         0001_0010_1111_0100  (0x12F4)
 *                            ^^^^       new value inserted!
 *
 * TIME COMPLEXITY: O(1)
 * SPACE COMPLEXITY: O(1)
 *
 * ============================================================================
 */

// Extract/Insert bit fields - ESSENTIAL for embedded register manipulation
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

// Say: "Extract a field of 'width' bits starting at 'pos'"
uint32_t extract_bits(uint32_t value, int pos, int width) {
    // Say: "First, create a mask with 'width' ones"
    // Say: "(1 << width) - 1 gives us width ones"
    // Example: width=3 -> (1 << 3) - 1 = 8 - 1 = 7 = 0b111
    uint32_t mask = (1U << width) - 1;

    // Say: "Shift value right to bring target bits to position 0"
    // Say: "Then AND with mask to isolate only those bits"
    return (value >> pos) & mask;
}

// Say: "Insert 'field' value into 'target' at position 'pos' with given 'width'"
uint32_t insert_bits(uint32_t target, uint32_t field, int pos, int width) {
    // Say: "Create mask for the field width"
    uint32_t mask = (1U << width) - 1;

    // Say: "Step 1: Clear the target bits - create hole"
    // Say: "Shift mask to position, invert it, AND with target"
    target &= ~(mask << pos);  // Say: "Clears bits at position"

    // Say: "Step 2: Insert new value"
    // Say: "Mask field to width, shift to position, OR into target"
    target |= (field & mask) << pos;  // Say: "Insert field value"

    return target;
}

// Say: "Modify specific bits without affecting others"
uint32_t modify_bits(uint32_t reg, uint32_t value, int pos, int width) {
    // Say: "Same as insert - clear then set"
    uint32_t mask = ((1U << width) - 1) << pos;
    return (reg & ~mask) | ((value << pos) & mask);
}

int main() {
    // Example: Working with a hardware register
    // Say: "Simulating a status register with bit fields"
    // Bits [3:0]   = Status (4 bits)
    // Bits [7:4]   = Error code (4 bits)
    // Bits [15:8]  = Count (8 bits)
    // Bits [31:16] = Reserved

    uint32_t reg = 0x00001234;  // Example register value

    printf("Register value: 0x%08X\n", reg);
    print_binary(reg);
    printf("\n\n");

    // Extract status (bits 0-3)
    uint32_t status = extract_bits(reg, 0, 4);
    printf("Status (bits 3:0):     %u (0x%X)\n", status, status);

    // Extract error code (bits 4-7)
    uint32_t error = extract_bits(reg, 4, 4);
    printf("Error (bits 7:4):      %u (0x%X)\n", error, error);

    // Extract count (bits 8-15)
    uint32_t count = extract_bits(reg, 8, 8);
    printf("Count (bits 15:8):     %u (0x%X)\n", count, count);

    // Insert new error code
    printf("\nInserting error code 0xF at bits [7:4]:\n");
    uint32_t new_reg = insert_bits(reg, 0xF, 4, 4);
    printf("New register: 0x%08X\n", new_reg);
    print_binary(new_reg);
    printf("\n");

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Bit field extraction and insertion are fundamental for embedded register access.

 EXTRACT BITS (read a field):
 1. Create mask: (1 << width) - 1 gives 'width' ones
 2. Shift value right to bring field to position 0
 3. AND with mask to isolate the bits

 Formula: (value >> pos) & ((1 << width) - 1)

 Example - Extract bits [7:4] from 0x1234:
 - value = 0x1234 = 0001_0010_0011_0100
 - Shift right by 4: 0000_0001_0010_0011 = 0x123
 - Mask with 0xF (4 bits): 0x3
 - Result: 3

 INSERT BITS (write a field):
 1. Create mask for the field position
 2. Clear target bits: target &= ~(mask << pos)
 3. Insert new value: target |= (value << pos)

 WHY THIS PATTERN:
 - Hardware registers pack multiple fields into one word
 - Example: GPIO config might have mode, speed, pull-up in one register
 - Must modify specific bits without disturbing others

 EMBEDDED EXAMPLE - Configure GPIO:
 - Register has: [1:0]=Mode, [3:2]=Speed, [5:4]=Pull
 - To set mode=2 without changing others:
   - reg = insert_bits(reg, 2, 0, 2);

 MACRO VERSIONS (common in embedded):
 #define EXTRACT(val, pos, width) (((val) >> (pos)) & ((1 << (width)) - 1))
 #define INSERT(target, val, pos, width) \\
     (((target) & ~(((1 << (width)) - 1) << (pos))) | (((val) << (pos))))

 IMPORTANT: Always use unsigned types for bit manipulation!"
*/
