/*
 * ============================================================================
 * PROBLEM: Detect System Endianness
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: HIGH (Embedded)
 *
 * Determine if system is big-endian or little-endian.
 *
 * ============================================================================
 * WHAT IS ENDIANNESS?
 * ============================================================================
 *
 * Value: 0x12345678
 *
 * BIG-ENDIAN (Network Byte Order):
 *   Address: 0x00  0x01  0x02  0x03
 *   Value:   0x12  0x34  0x56  0x78
 *            ^^^^
 *            MSB at lowest address
 *
 * LITTLE-ENDIAN:
 *   Address: 0x00  0x01  0x02  0x03
 *   Value:   0x78  0x56  0x34  0x12
 *            ^^^^
 *            LSB at lowest address
 *
 * ============================================================================
 * SYSTEMS:
 * ============================================================================
 *
 * Little-Endian: x86, x86-64, ARM (default), most modern processors
 * Big-Endian: Network protocols, Motorola 68k, PowerPC, SPARC
 *
 * ============================================================================
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Method 1: Union trick
bool isLittleEndian_union(void) {
    union {
        uint16_t value;
        uint8_t bytes[2];
    } test;

    test.value = 0x0001;

    // If first byte is 0x01, LSB is at lowest address = little-endian
    return test.bytes[0] == 0x01;
}

// Method 2: Pointer casting
bool isLittleEndian_pointer(void) {
    uint32_t value = 0x00000001;
    uint8_t* bytePtr = (uint8_t*)&value;

    return *bytePtr == 0x01;
}

// Method 3: Char pointer (most portable)
bool isLittleEndian_char(void) {
    int value = 1;
    char* p = (char*)&value;

    return *p == 1;
}

void printBytes(const char* label, void* ptr, size_t size) {
    uint8_t* bytes = (uint8_t*)ptr;
    printf("%s: ", label);
    for (size_t i = 0; i < size; i++) {
        printf("0x%02X ", bytes[i]);
    }
    printf("\n");
}

int main() {
    printf("=== Detect System Endianness ===\n\n");

    printf("This system is: %s\n\n",
           isLittleEndian_union() ? "LITTLE-ENDIAN" : "BIG-ENDIAN");

    // Show memory layout
    uint32_t value = 0x12345678;
    printf("Value: 0x%08X\n", value);
    printBytes("Memory layout", &value, 4);
    printf("\n");

    if (isLittleEndian_union()) {
        printf("Interpretation:\n");
        printf("  Address 0: 0x78 (LSB)\n");
        printf("  Address 1: 0x56\n");
        printf("  Address 2: 0x34\n");
        printf("  Address 3: 0x12 (MSB)\n");
    }

    printf("\n=== Three Methods ===\n");
    printf("1. Union:   %s\n", isLittleEndian_union() ? "Little" : "Big");
    printf("2. Pointer: %s\n", isLittleEndian_pointer() ? "Little" : "Big");
    printf("3. Char:    %s\n", isLittleEndian_char() ? "Little" : "Big");

    return 0;
}
