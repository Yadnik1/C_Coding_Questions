/*
 * ============================================================================
 * PROBLEM: Check System Endianness (Big vs Little Endian)
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Determine whether the system stores multi-byte values in:
 * - LITTLE ENDIAN: Least Significant Byte (LSB) at lowest memory address
 * - BIG ENDIAN: Most Significant Byte (MSB) at lowest memory address
 * This is CRITICAL for embedded systems that communicate with networks
 * or other devices!
 *
 * EXAMPLES (with memory layout):
 *   Value: 0x12345678 (32-bit integer)
 *
 *   LITTLE ENDIAN (x86, ARM, most modern processors):
 *   Address:  0x100  0x101  0x102  0x103
 *   Content:  0x78   0x56   0x34   0x12
 *             ^LSB                 ^MSB
 *   "Little end (LSB) comes first"
 *
 *   BIG ENDIAN (Network byte order, some PowerPC, Motorola 68k):
 *   Address:  0x100  0x101  0x102  0x103
 *   Content:  0x12   0x34   0x56   0x78
 *             ^MSB                 ^LSB
 *   "Big end (MSB) comes first"
 *
 * HOW TO CHECK:
 *   uint16_t val = 0x0102;    // High byte=0x01, Low byte=0x02
 *   uint8_t *ptr = (uint8_t*)&val;
 *
 *   if (*ptr == 0x02) -> Little Endian (low byte at low address)
 *   if (*ptr == 0x01) -> Big Endian (high byte at low address)
 *
 * WHY IS THIS ASKED IN EMBEDDED INTERVIEWS?
 * - Network protocols use Big Endian (network byte order)
 * - x86/ARM processors are Little Endian
 * - Must convert when sending/receiving network data
 * - Binary file formats specify endianness
 * - Cross-platform data exchange requires awareness
 * - Sensor data over UART/SPI may be either endian
 * - THIS QUESTION APPEARS IN ALMOST EVERY EMBEDDED INTERVIEW!
 *
 * KEY CONCEPT - BYTE ORDERING:
 * Endianness only affects multi-byte data types (int16, int32, etc.)
 * Single bytes and bit order within bytes are NOT affected.
 *
 * VISUAL:
 *   Storing 0x0102 (16-bit):
 *
 *   LITTLE ENDIAN:
 *   +--------+--------+
 *   | 0x02   | 0x01   |
 *   +--------+--------+
 *   addr     addr+1
 *   ^LSB     ^MSB
 *
 *   BIG ENDIAN:
 *   +--------+--------+
 *   | 0x01   | 0x02   |
 *   +--------+--------+
 *   addr     addr+1
 *   ^MSB     ^LSB
 *
 *   Detection using pointer:
 *   uint16_t val = 0x0102;
 *   uint8_t *p = (uint8_t*)&val;
 *   *p == 0x02 ? Little : Big
 *
 * CONVERSION FUNCTIONS (standard library):
 *   htons() - Host to Network Short (16-bit)
 *   htonl() - Host to Network Long (32-bit)
 *   ntohs() - Network to Host Short
 *   ntohl() - Network to Host Long
 *
 * TIME COMPLEXITY: O(1)
 * SPACE COMPLEXITY: O(1)
 *
 * ============================================================================
 */

// Check system endianness - ESSENTIAL embedded interview question
// Time: O(1), Space: O(1)

#include <stdio.h>
#include <stdint.h>

// Say: "I check how multi-byte integers are stored in memory"
int check_endianness() {
    // Say: "I create a 16-bit value with known byte pattern"
    uint16_t value = 0x0102;  // Say: "High byte is 01, low byte is 02"

    // Say: "Cast to char pointer to examine first byte in memory"
    uint8_t *ptr = (uint8_t*)&value;

    // Say: "Check what's stored at the lowest memory address"
    if (*ptr == 0x02) {
        // Say: "Low byte (02) is at low address = Little Endian"
        return 0;  // Little endian
    } else {
        // Say: "High byte (01) is at low address = Big Endian"
        return 1;  // Big endian
    }
}

// Alternative using union (same concept, different syntax)
int check_endianness_union() {
    // Say: "Union lets me access same memory as different types"
    union {
        uint16_t word;
        uint8_t bytes[2];
    } test;

    test.word = 0x0102;  // Say: "Set as 16-bit value"

    // Say: "Check the first byte (bytes[0] = lowest address)"
    return (test.bytes[0] == 0x01) ? 1 : 0;  // 1 = Big, 0 = Little
}

void print_memory_layout(void *ptr, int size) {
    printf("Memory layout (low to high address): ");
    uint8_t *byte_ptr = (uint8_t*)ptr;
    for (int i = 0; i < size; i++) {
        printf("0x%02X ", byte_ptr[i]);
    }
    printf("\n");
}

int main() {
    // Check endianness
    if (check_endianness() == 0) {
        printf("System is LITTLE ENDIAN\n");
    } else {
        printf("System is BIG ENDIAN\n");
    }

    // Demonstrate with 32-bit value
    uint32_t val32 = 0x01020304;
    printf("\nValue: 0x%08X\n", val32);
    print_memory_layout(&val32, sizeof(val32));

    // x86/ARM (little endian): 0x04 0x03 0x02 0x01
    // Network byte order (big endian): 0x01 0x02 0x03 0x04

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Endianness determines how multi-byte data is stored in memory.

 LITTLE ENDIAN (x86, ARM):
 - Least Significant Byte at lowest address
 - 0x12345678 stored as: 78 56 34 12
 - 'Little end first'

 BIG ENDIAN (Network byte order, some PowerPC):
 - Most Significant Byte at lowest address
 - 0x12345678 stored as: 12 34 56 78
 - 'Big end first'

 HOW I CHECK:
 1. Create a multi-byte value with known pattern (0x0102)
 2. Cast to byte pointer to examine memory byte-by-byte
 3. Check first byte (lowest address)
 4. If it's 0x02 (low byte) -> Little Endian
 5. If it's 0x01 (high byte) -> Big Endian

 WHY THIS MATTERS IN EMBEDDED:
 - Network protocols use big endian (network byte order)
 - x86/ARM processors are little endian
 - Must convert when sending/receiving network data
 - Use htons(), htonl(), ntohs(), ntohl() for conversion
 - File formats may specify endianness

 EMBEDDED EXAMPLE:
 - Receiving 4-byte sensor reading over network (big endian)
 - ARM processor is little endian
 - Must swap bytes: use ntohl() or manual swap

 COMMON MISTAKE:
 - Assuming all systems are the same endianness
 - Not converting network data properly"
*/
