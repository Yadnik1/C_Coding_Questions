/*
 * ============================================================================
 * PROBLEM: Merge Bytes into Integer and Split Back
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Combine multiple individual bytes into a larger integer (16-bit, 32-bit),
 * and split a larger integer back into its component bytes. Essential for
 * parsing communication protocols and working with multi-byte sensor data.
 *
 * EXAMPLES (with binary/hex representation):
 *   MERGE 4 bytes into 32-bit (Big Endian order):
 *   Bytes: 0x12, 0x34, 0x56, 0x78
 *
 *   0x12 << 24 = 0x12_00_00_00   (byte at bits [31:24])
 *   0x34 << 16 = 0x00_34_00_00   (byte at bits [23:16])
 *   0x56 << 8  = 0x00_00_56_00   (byte at bits [15:8])
 *   0x78 << 0  = 0x00_00_00_78   (byte at bits [7:0])
 *   OR all:    = 0x12_34_56_78
 *
 *   SPLIT 0x12345678 into bytes:
 *   (value >> 24) & 0xFF = 0x12  (MSB)
 *   (value >> 16) & 0xFF = 0x34
 *   (value >> 8)  & 0xFF = 0x56
 *   (value >> 0)  & 0xFF = 0x78  (LSB)
 *
 *   BYTE SWAP (Little <-> Big Endian):
 *   0x12345678 -> 0x78563412
 *   Each byte moves to opposite position.
 *
 * WHY IS THIS ASKED IN EMBEDDED INTERVIEWS?
 * - Receiving multi-byte data over UART/SPI/I2C
 * - Parsing network packets (TCP/IP headers)
 * - Reading sensor values (ADC, temperature, etc.)
 * - Working with binary file formats
 * - Endianness conversion between protocols and CPU
 * - VERY COMMON IN REAL EMBEDDED WORK!
 *
 * KEY CONCEPT - SHIFT AND MASK:
 *   MERGE:  (b3 << 24) | (b2 << 16) | (b1 << 8) | b0
 *           Cast bytes to larger type BEFORE shifting!
 *
 *   SPLIT:  (value >> position) & 0xFF
 *           Shift byte to lowest position, mask with 0xFF
 *
 * VISUAL:
 *   MERGE bytes 0x12, 0x34, 0x56, 0x78:
 *
 *   Byte positions in 32-bit word:
 *   +------+------+------+------+
 *   | 0x12 | 0x34 | 0x56 | 0x78 |
 *   +------+------+------+------+
 *   bits 31-24  23-16  15-8   7-0
 *        MSB                  LSB
 *
 *   Step 1: Shift each to position
 *   0x12 << 24: 0001_0010 0000_0000 0000_0000 0000_0000
 *   0x34 << 16: 0000_0000 0011_0100 0000_0000 0000_0000
 *   0x56 << 8:  0000_0000 0000_0000 0101_0110 0000_0000
 *   0x78 << 0:  0000_0000 0000_0000 0000_0000 0111_1000
 *
 *   Step 2: OR all together
 *   Result:     0001_0010 0011_0100 0101_0110 0111_1000
 *               = 0x12345678
 *
 *   -----------------------------------------------
 *
 *   BYTE SWAP 0x12345678 -> 0x78563412:
 *
 *   Before: | 12 | 34 | 56 | 78 |
 *           pos3  pos2  pos1  pos0
 *
 *   After:  | 78 | 56 | 34 | 12 |
 *           pos3  pos2  pos1  pos0
 *
 *   Formula:
 *   ((val >> 24) & 0xFF) << 0   -> 0x12 to pos0
 *   ((val >> 16) & 0xFF) << 8   -> 0x34 to pos1
 *   ((val >> 8)  & 0xFF) << 16  -> 0x56 to pos2
 *   ((val >> 0)  & 0xFF) << 24  -> 0x78 to pos3
 *
 * CRITICAL: Cast bytes to uint32_t BEFORE shifting!
 *   WRONG: uint8_t b = 0x12; b << 24;  // overflow!
 *   RIGHT: (uint32_t)b << 24;          // correct!
 *
 * TIME COMPLEXITY: O(1)
 * SPACE COMPLEXITY: O(1)
 *
 * ============================================================================
 */

// Merge bytes into larger integer and split back - ESSENTIAL for protocol parsing
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

// Say: "Merge 4 bytes into 32-bit integer (Big Endian order)"
uint32_t merge_bytes_be(uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0) {
    // Say: "b3 is MSB, b0 is LSB (Big Endian: most significant first)"
    // Say: "Shift each byte to its position and OR together"
    return ((uint32_t)b3 << 24) |  // Say: "MSB at bits 31-24"
           ((uint32_t)b2 << 16) |  // Say: "Next byte at bits 23-16"
           ((uint32_t)b1 << 8)  |  // Say: "Next byte at bits 15-8"
           ((uint32_t)b0);         // Say: "LSB at bits 7-0"
}

// Say: "Merge 4 bytes into 32-bit integer (Little Endian order)"
uint32_t merge_bytes_le(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
    // Say: "b0 is at lowest address but contains LSB in little endian"
    return ((uint32_t)b3 << 24) |
           ((uint32_t)b2 << 16) |
           ((uint32_t)b1 << 8)  |
           ((uint32_t)b0);
}

// Say: "Extract individual bytes from 32-bit integer"
void split_bytes(uint32_t value, uint8_t *b3, uint8_t *b2, uint8_t *b1, uint8_t *b0) {
    // Say: "Shift and mask to extract each byte"
    *b3 = (value >> 24) & 0xFF;  // Say: "Get bits 31-24"
    *b2 = (value >> 16) & 0xFF;  // Say: "Get bits 23-16"
    *b1 = (value >> 8) & 0xFF;   // Say: "Get bits 15-8"
    *b0 = value & 0xFF;          // Say: "Get bits 7-0"
}

// Say: "Merge 2 bytes into 16-bit integer"
uint16_t merge_bytes_16(uint8_t high, uint8_t low) {
    return ((uint16_t)high << 8) | low;
}

// Say: "Split 16-bit integer into 2 bytes"
void split_bytes_16(uint16_t value, uint8_t *high, uint8_t *low) {
    *high = (value >> 8) & 0xFF;
    *low = value & 0xFF;
}

// Say: "Byte swap for endianness conversion"
uint32_t byte_swap_32(uint32_t value) {
    // Say: "Reverse byte order: ABCD becomes DCBA"
    return ((value >> 24) & 0x000000FF) |  // Say: "Move byte 3 to byte 0"
           ((value >> 8)  & 0x0000FF00) |  // Say: "Move byte 2 to byte 1"
           ((value << 8)  & 0x00FF0000) |  // Say: "Move byte 1 to byte 2"
           ((value << 24) & 0xFF000000);   // Say: "Move byte 0 to byte 3"
}

uint16_t byte_swap_16(uint16_t value) {
    return ((value >> 8) & 0x00FF) |
           ((value << 8) & 0xFF00);
}

int main() {
    // Simulate receiving 4 bytes from UART/SPI
    uint8_t rx_buffer[] = {0x12, 0x34, 0x56, 0x78};

    printf("Received bytes: 0x%02X 0x%02X 0x%02X 0x%02X\n",
           rx_buffer[0], rx_buffer[1], rx_buffer[2], rx_buffer[3]);

    // Merge assuming big endian protocol (network byte order)
    uint32_t value_be = merge_bytes_be(rx_buffer[0], rx_buffer[1],
                                        rx_buffer[2], rx_buffer[3]);
    printf("\nMerged (Big Endian):    0x%08X\n", value_be);
    print_binary(value_be);
    printf("\n");

    // Merge assuming little endian protocol
    uint32_t value_le = merge_bytes_le(rx_buffer[0], rx_buffer[1],
                                        rx_buffer[2], rx_buffer[3]);
    printf("\nMerged (Little Endian): 0x%08X\n", value_le);
    print_binary(value_le);
    printf("\n");

    // Split back into bytes
    printf("\nSplit 0x12345678 into bytes:\n");
    uint8_t b3, b2, b1, b0;
    split_bytes(0x12345678, &b3, &b2, &b1, &b0);
    printf("Bytes: 0x%02X 0x%02X 0x%02X 0x%02X\n", b3, b2, b1, b0);

    // Byte swap demonstration
    printf("\nByte swap 0x12345678:\n");
    uint32_t swapped = byte_swap_32(0x12345678);
    printf("Swapped: 0x%08X\n", swapped);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Byte merging and splitting is essential for protocol parsing and hardware communication.

 MERGE BYTES (reconstruct multi-byte value):
 - Cast each byte to target size to prevent overflow
 - Shift each byte to its position
 - OR all bytes together

 Formula: (b3 << 24) | (b2 << 16) | (b1 << 8) | b0

 WHY CAST TO uint32_t:
 - uint8_t << 24 would overflow before shifting
 - Cast BEFORE shift: ((uint32_t)byte << 24)
 - Common bug: forgetting the cast

 SPLIT BYTES (extract individual bytes):
 - Shift right to bring byte to lowest position
 - AND with 0xFF to mask off other bytes

 ENDIANNESS MATTERS:
 - Network protocols typically use Big Endian
 - x86/ARM are Little Endian
 - Must know which convention the data uses

 EMBEDDED EXAMPLE - Receive sensor reading over UART:
 // Receive 2 bytes representing 16-bit temperature
 uint8_t high = uart_receive();
 uint8_t low = uart_receive();
 uint16_t temp = merge_bytes_16(high, low);

 BYTE SWAP for endianness conversion:
 - Reverses byte order in a value
 - Used when converting between big/little endian
 - Same as htonl()/ntohl() for 32-bit

 ALTERNATIVE - Using union (hardware access):
 union {
     uint32_t word;
     uint8_t bytes[4];
 } data;
 data.bytes[0] = rx_buffer[0];  // etc.

 WARNING: Union approach is endianness-dependent!

 COMMON MISTAKES:
 - Forgetting to cast bytes before shifting
 - Wrong byte order (big vs little endian)
 - Using signed char instead of uint8_t"
*/
