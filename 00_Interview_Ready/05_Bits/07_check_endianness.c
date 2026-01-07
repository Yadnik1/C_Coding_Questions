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
