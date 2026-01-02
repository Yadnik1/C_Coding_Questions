/*
 * ============================================================================
 * PROBLEM: Merge Bytes and Reverse Byte Order
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH (Embedded)
 *
 * Common byte manipulation operations:
 * 1. Merge two bytes into 16-bit
 * 2. Merge four bytes into 32-bit
 * 3. Reverse byte order in 32-bit (endianness conversion)
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 * Merge two bytes into 16-bit:
 *   high = 0xAB, low = 0xCD
 *   result = (0xAB << 8) | 0xCD = 0xABCD
 *
 * Reverse bytes in 32-bit:
 *   Before: 0x12345678
 *           [12][34][56][78]
 *   After:  0x78563412
 *           [78][56][34][12]
 *
 * ============================================================================
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// Merge two bytes into 16-bit (big-endian: high byte first)
uint16_t mergeBytes16(uint8_t high, uint8_t low) {
    return ((uint16_t)high << 8) | low;
}

// Merge two bytes into 16-bit (little-endian: low byte first)
uint16_t mergeBytes16LE(uint8_t low, uint8_t high) {
    return ((uint16_t)high << 8) | low;
}

// Merge four bytes into 32-bit (big-endian)
uint32_t mergeBytes32(uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0) {
    return ((uint32_t)b3 << 24) |
           ((uint32_t)b2 << 16) |
           ((uint32_t)b1 << 8)  |
           (uint32_t)b0;
}

// Merge byte array into 32-bit (big-endian)
uint32_t bytesToUint32BE(const uint8_t* bytes) {
    return ((uint32_t)bytes[0] << 24) |
           ((uint32_t)bytes[1] << 16) |
           ((uint32_t)bytes[2] << 8)  |
           (uint32_t)bytes[3];
}

// Merge byte array into 32-bit (little-endian)
uint32_t bytesToUint32LE(const uint8_t* bytes) {
    return ((uint32_t)bytes[3] << 24) |
           ((uint32_t)bytes[2] << 16) |
           ((uint32_t)bytes[1] << 8)  |
           (uint32_t)bytes[0];
}

// Reverse byte order in 32-bit (swap endianness)
uint32_t reverseBytes32(uint32_t value) {
    return ((value >> 24) & 0x000000FF) |
           ((value >> 8)  & 0x0000FF00) |
           ((value << 8)  & 0x00FF0000) |
           ((value << 24) & 0xFF000000);
}

// Reverse byte order in 16-bit
uint16_t reverseBytes16(uint16_t value) {
    return (value >> 8) | (value << 8);
}

// Split 32-bit into bytes
void uint32ToBytes(uint32_t value, uint8_t* bytes) {
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
}

void printBytes(uint8_t* bytes, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("0x%02X", bytes[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]");
}

int main() {
    printf("=== Merge Bytes and Reverse Byte Order ===\n\n");

    // Merge two bytes
    printf("1. Merge two bytes into 16-bit:\n");
    uint8_t high = 0xAB, low = 0xCD;
    printf("   high=0x%02X, low=0x%02X\n", high, low);
    printf("   Result: 0x%04X\n\n", mergeBytes16(high, low));

    // Merge four bytes
    printf("2. Merge four bytes into 32-bit:\n");
    uint8_t b3 = 0x12, b2 = 0x34, b1 = 0x56, b0 = 0x78;
    printf("   Bytes: 0x%02X, 0x%02X, 0x%02X, 0x%02X\n", b3, b2, b1, b0);
    printf("   Result: 0x%08X\n\n", mergeBytes32(b3, b2, b1, b0));

    // From byte array
    printf("3. Byte array to 32-bit:\n");
    uint8_t arr[] = {0x12, 0x34, 0x56, 0x78};
    printf("   Array: "); printBytes(arr, 4); printf("\n");
    printf("   As BE: 0x%08X\n", bytesToUint32BE(arr));
    printf("   As LE: 0x%08X\n\n", bytesToUint32LE(arr));

    // Reverse bytes
    printf("4. Reverse byte order (endianness swap):\n");
    uint32_t val = 0x12345678;
    printf("   Before: 0x%08X\n", val);
    printf("   After:  0x%08X\n\n", reverseBytes32(val));

    // Split 32-bit to bytes
    printf("5. Split 32-bit into bytes:\n");
    uint8_t result[4];
    uint32ToBytes(0xABCD1234, result);
    printf("   0xABCD1234 → "); printBytes(result, 4); printf("\n\n");

    printf("=== Formulas ===\n");
    printf("Merge 16: (high << 8) | low\n");
    printf("Merge 32: (b3 << 24) | (b2 << 16) | (b1 << 8) | b0\n");
    printf("Reverse:  Swap byte positions using shifts and masks\n");

    return 0;
}
