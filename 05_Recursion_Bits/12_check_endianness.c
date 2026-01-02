/*
 * ============================================================================
 * PROBLEM: Check Endianness of the System
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: VERY HIGH (Embedded)
 *
 * Determine if the system is big-endian or little-endian.
 *
 * ============================================================================
 * WHAT IS ENDIANNESS?
 * ============================================================================
 *
 * How multi-byte values are stored in memory.
 *
 * LITTLE-ENDIAN: LSB (Least Significant Byte) at lowest address
 *   Value 0x12345678 stored as: [78][56][34][12]
 *   Systems: x86, x86-64, ARM (default)
 *
 * BIG-ENDIAN: MSB (Most Significant Byte) at lowest address
 *   Value 0x12345678 stored as: [12][34][56][78]
 *   Systems: Network protocols, PowerPC, SPARC
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   Value: 0x01020304
 *
 *   Little-Endian Memory Layout:
 *   Address:  0x1000  0x1001  0x1002  0x1003
 *   Value:    [04]    [03]    [02]    [01]
 *              ^
 *              LSB at lowest address
 *
 *   Big-Endian Memory Layout:
 *   Address:  0x1000  0x1001  0x1002  0x1003
 *   Value:    [01]    [02]    [03]    [04]
 *              ^
 *              MSB at lowest address
 *
 * ============================================================================
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Method 1: Using union
bool isLittleEndian_Union(void) {
    union {
        uint16_t value;
        uint8_t bytes[2];
    } test;

    test.value = 0x0001;

    // If bytes[0] is 1, LSB is at lowest address = little-endian
    return test.bytes[0] == 0x01;
}

// Method 2: Using pointer cast
bool isLittleEndian_Pointer(void) {
    uint32_t value = 0x00000001;
    uint8_t* bytePtr = (uint8_t*)&value;

    return *bytePtr == 0x01;
}

// Method 3: Using char pointer (most portable)
bool isLittleEndian_Char(void) {
    int value = 1;
    return *(char*)&value == 1;
}

// Show memory layout
void showMemoryLayout(uint32_t value) {
    uint8_t* bytes = (uint8_t*)&value;

    printf("   Value: 0x%08X\n", value);
    printf("   Memory layout (address order):\n");
    printf("   ");
    for (int i = 0; i < 4; i++) {
        printf("[0x%02X] ", bytes[i]);
    }
    printf("\n");
    printf("   addr:   +0     +1     +2     +3\n");
}

int main() {
    printf("=== Check System Endianness ===\n\n");

    // Check endianness
    printf("1. Endianness Detection:\n");
    printf("   Method 1 (Union):   %s\n",
           isLittleEndian_Union() ? "Little-Endian" : "Big-Endian");
    printf("   Method 2 (Pointer): %s\n",
           isLittleEndian_Pointer() ? "Little-Endian" : "Big-Endian");
    printf("   Method 3 (Char):    %s\n\n",
           isLittleEndian_Char() ? "Little-Endian" : "Big-Endian");

    // Show memory layout
    printf("2. Memory Layout:\n");
    showMemoryLayout(0x01020304);
    printf("\n");

    if (isLittleEndian_Union()) {
        printf("   Interpretation: Little-Endian\n");
        printf("   - LSB (0x04) at lowest address\n");
        printf("   - MSB (0x01) at highest address\n");
    } else {
        printf("   Interpretation: Big-Endian\n");
        printf("   - MSB (0x01) at lowest address\n");
        printf("   - LSB (0x04) at highest address\n");
    }

    printf("\n=== Endianness Summary ===\n\n");
    printf("Little-Endian (LE):\n");
    printf("  - LSB first (at lower address)\n");
    printf("  - x86, x86-64, ARM (default)\n");
    printf("  - 0x1234 stored as [34][12]\n\n");

    printf("Big-Endian (BE):\n");
    printf("  - MSB first (at lower address)\n");
    printf("  - Network byte order, PowerPC\n");
    printf("  - 0x1234 stored as [12][34]\n\n");

    printf("Detection trick:\n");
    printf("  Store 0x0001 in 16-bit int\n");
    printf("  Check first byte: 0x01 = LE, 0x00 = BE\n");

    return 0;
}
