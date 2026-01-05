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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why is byte order conversion important?"
 * A1: Network protocols use big-endian (network byte order).
 *     x86/ARM use little-endian. Must convert when sending/receiving data!
 *     htonl/ntohl functions do this (host to network long, network to host long).
 *
 * -------------------------------------------------------------------------
 * Q2: "What's the BSWAP instruction?"
 * A2: x86 instruction that reverses byte order in a register.
 *     GCC has __builtin_bswap32/16/64 that compile to BSWAP.
 *     Much faster than manual shift-OR operations!
 *
 * -------------------------------------------------------------------------
 * Q3: "Why cast to uint32_t before shifting?"
 * A3: uint8_t is promoted to int before shift. If shift result exceeds
 *     int range, behavior is undefined. Explicit cast ensures unsigned
 *     32-bit arithmetic. Always cast to target type before shifting!
 *
 * -------------------------------------------------------------------------
 * Q4: "When would you use memcpy instead of shifts?"
 * A4: memcpy is safer for strict aliasing compliance and works for any size.
 *     Compiler usually optimizes memcpy of small fixed sizes to shifts anyway.
 *     uint32_t val; memcpy(&val, bytes, 4); // May be faster and portable
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// Merge two bytes into 16-bit (big-endian: high byte first)
uint16_t mergeBytes16(uint8_t high, uint8_t low) {
    // Say: "To merge two bytes into 16-bit, I'll shift high byte left by 8"
    // Say: "Then OR it with the low byte to combine them"
    // Shift high byte left 8 positions, OR with low byte
    return ((uint16_t)high << 8) | low;
}

// Merge two bytes into 16-bit (little-endian: low byte first)
uint16_t mergeBytes16LE(uint8_t low, uint8_t high) {
    // Say: "For little-endian, the parameters are swapped but logic is the same"
    // Shift high byte left 8 positions, OR with low byte
    return ((uint16_t)high << 8) | low;
}

// Merge four bytes into 32-bit (big-endian)
uint32_t mergeBytes32(uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0) {
    // Say: "To merge four bytes, I'll shift each byte to its position"
    // Say: "Byte 3 goes to bits 24-31, byte 2 to 16-23, byte 1 to 8-15, byte 0 to 0-7"
    // Say: "Then I'll OR all four shifted values together"
    // Shift b3 to bits 24-31
    // Shift b2 to bits 16-23
    // Shift b1 to bits 8-15
    // b0 stays at bits 0-7
    // OR all together
    return ((uint32_t)b3 << 24) |
           ((uint32_t)b2 << 16) |
           ((uint32_t)b1 << 8)  |
           (uint32_t)b0;
}

// Merge byte array into 32-bit (big-endian)
uint32_t bytesToUint32BE(const uint8_t* bytes) {
    // Say: "For big-endian byte array, byte 0 is most significant"
    // Say: "I'll shift bytes[0] to bits 24-31, bytes[1] to 16-23, etc."
    // Combine bytes in big-endian order
    return ((uint32_t)bytes[0] << 24) |
           ((uint32_t)bytes[1] << 16) |
           ((uint32_t)bytes[2] << 8)  |
           (uint32_t)bytes[3];
}

// Merge byte array into 32-bit (little-endian)
uint32_t bytesToUint32LE(const uint8_t* bytes) {
    // Say: "For little-endian byte array, byte 0 is least significant"
    // Say: "I'll shift bytes[3] to bits 24-31, bytes[2] to 16-23, etc."
    // Combine bytes in little-endian order
    return ((uint32_t)bytes[3] << 24) |
           ((uint32_t)bytes[2] << 16) |
           ((uint32_t)bytes[1] << 8)  |
           (uint32_t)bytes[0];
}

// Reverse byte order in 32-bit (swap endianness)
uint32_t reverseBytes32(uint32_t value) {
    // Say: "To reverse byte order, I'll extract each byte and reposition it"
    // Say: "Byte 0 becomes byte 3, byte 1 becomes byte 2, etc."
    // Extract bits 24-31, shift right to bits 0-7
    // Extract bits 16-23, shift right to bits 8-15
    // Extract bits 8-15, shift left to bits 16-23
    // Extract bits 0-7, shift left to bits 24-31
    return ((value >> 24) & 0x000000FF) |
           ((value >> 8)  & 0x0000FF00) |
           ((value << 8)  & 0x00FF0000) |
           ((value << 24) & 0xFF000000);
}

// Reverse byte order in 16-bit
uint16_t reverseBytes16(uint16_t value) {
    // Say: "For 16-bit, I'll swap the two bytes"
    // Say: "Shift high byte right to low, shift low byte left to high"
    // Shift right by 8 to move high byte to low position
    // Shift left by 8 to move low byte to high position
    return (value >> 8) | (value << 8);
}

// Split 32-bit into bytes
void uint32ToBytes(uint32_t value, uint8_t* bytes) {
    // Say: "To split a 32-bit value, I'll extract each byte using shifts and masks"
    // Extract byte 3 (bits 24-31)
    bytes[0] = (value >> 24) & 0xFF;
    // Extract byte 2 (bits 16-23)
    bytes[1] = (value >> 16) & 0xFF;
    // Extract byte 1 (bits 8-15)
    bytes[2] = (value >> 8) & 0xFF;
    // Extract byte 0 (bits 0-7)
    bytes[3] = value & 0xFF;
}

void printBytes(uint8_t* bytes, int n) {
    // Print opening bracket
    printf("[");

    // Loop through bytes
    for (int i = 0; i < n; i++) {
        // Print byte in hex
        printf("0x%02X", bytes[i]);

        // Add comma if not last
        if (i < n - 1) printf(", ");
    }

    // Print closing bracket
    printf("]");
}

int main() {
    printf("=== Merge Bytes and Reverse Byte Order ===\n\n");

    // Merge two bytes
    printf("1. Merge two bytes into 16-bit:\n");
    // Initialize bytes
    uint8_t high = 0xAB, low = 0xCD;
    printf("   high=0x%02X, low=0x%02X\n", high, low);
    // Merge and print result
    printf("   Result: 0x%04X\n\n", mergeBytes16(high, low));

    // Merge four bytes
    printf("2. Merge four bytes into 32-bit:\n");
    // Initialize bytes
    uint8_t b3 = 0x12, b2 = 0x34, b1 = 0x56, b0 = 0x78;
    printf("   Bytes: 0x%02X, 0x%02X, 0x%02X, 0x%02X\n", b3, b2, b1, b0);
    // Merge and print result
    printf("   Result: 0x%08X\n\n", mergeBytes32(b3, b2, b1, b0));

    // From byte array
    printf("3. Byte array to 32-bit:\n");
    // Initialize byte array
    uint8_t arr[] = {0x12, 0x34, 0x56, 0x78};
    printf("   Array: "); printBytes(arr, 4); printf("\n");
    // Convert to 32-bit in both byte orders
    printf("   As BE: 0x%08X\n", bytesToUint32BE(arr));
    printf("   As LE: 0x%08X\n\n", bytesToUint32LE(arr));

    // Reverse bytes
    printf("4. Reverse byte order (endianness swap):\n");
    // Test value
    uint32_t val = 0x12345678;
    printf("   Before: 0x%08X\n", val);
    // Reverse and print
    printf("   After:  0x%08X\n\n", reverseBytes32(val));

    // Split 32-bit to bytes
    printf("5. Split 32-bit into bytes:\n");
    // Result array
    uint8_t result[4];
    // Split value into bytes
    uint32ToBytes(0xABCD1234, result);
    printf("   0xABCD1234 → "); printBytes(result, 4); printf("\n\n");

    printf("=== Formulas ===\n");
    printf("Merge 16: (high << 8) | low\n");
    printf("Merge 32: (b3 << 24) | (b2 << 16) | (b1 << 8) | b0\n");
    printf("Reverse:  Swap byte positions using shifts and masks\n");

    return 0;
}
