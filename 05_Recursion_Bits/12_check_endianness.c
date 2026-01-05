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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Is the union method well-defined in C?"
 * A1: Yes in C (type punning via union is allowed). In C++, it's technically
 *     undefined but works on all major compilers. The char pointer method
 *     is well-defined in both C and C++ and most portable.
 *
 * -------------------------------------------------------------------------
 * Q2: "Why does network byte order matter?"
 * A2: TCP/IP uses big-endian (network byte order). If you're on a little-endian
 *     system, you must convert: use htons/htonl (host to network) for sending,
 *     ntohs/ntohl (network to host) for receiving. Critical for portability!
 *
 * -------------------------------------------------------------------------
 * Q3: "What is bi-endian and when is it used?"
 * A3: Some processors (ARM, MIPS) can switch endianness at runtime via
 *     configuration register. Useful when interfacing with fixed-endian
 *     peripherals or for compatibility modes.
 *
 * -------------------------------------------------------------------------
 * Q4: "How does endianness affect struct packing?"
 * A4: Fields within bytes are stored in native byte order. When sharing
 *     structs across systems with different endianness, each multi-byte
 *     field must be converted. Single bytes don't need conversion!
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Method 1: Using union
bool isLittleEndian_Union(void) {
    // Say: "I'll use a union to overlay a 16-bit value with a byte array"
    // Declare union with 16-bit value and 2-byte array
    union {
        uint16_t value;
        uint8_t bytes[2];
    } test;

    // Say: "I'll set the value to 0x0001"
    // Set value to 0x0001
    test.value = 0x0001;

    // Say: "If bytes[0] is 1, then LSB is at lowest address, meaning little-endian"
    // Check if first byte is 0x01 (little-endian) or 0x00 (big-endian)
    return test.bytes[0] == 0x01;
}

// Method 2: Using pointer cast
bool isLittleEndian_Pointer(void) {
    // Say: "I'll create a 32-bit value and cast a pointer to it as a byte pointer"
    // Declare 32-bit value with LSB = 1
    uint32_t value = 0x00000001;

    // Say: "I'll cast the address of value to a byte pointer"
    // Cast to byte pointer to access individual bytes
    uint8_t* bytePtr = (uint8_t*)&value;

    // Say: "If the first byte is 1, the system is little-endian"
    // Check if first byte in memory is 0x01
    return *bytePtr == 0x01;
}

// Method 3: Using char pointer (most portable)
bool isLittleEndian_Char(void) {
    // Say: "This is the most portable method using a char pointer"
    // Declare int value
    int value = 1;

    // Say: "I'll cast the address to a char pointer and check the first byte"
    // Cast to char pointer and check first byte
    return *(char*)&value == 1;
}

// Show memory layout
void showMemoryLayout(uint32_t value) {
    // Say: "Let me show how the value is actually laid out in memory"
    // Cast value address to byte pointer
    uint8_t* bytes = (uint8_t*)&value;

    printf("   Value: 0x%08X\n", value);
    printf("   Memory layout (address order):\n");
    printf("   ");

    // Loop through 4 bytes in memory order
    for (int i = 0; i < 4; i++) {
        // Print byte at address offset i
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
    // Show layout of test value
    showMemoryLayout(0x01020304);
    printf("\n");

    // Explain the layout
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
