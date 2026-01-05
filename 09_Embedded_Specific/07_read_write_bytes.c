/*
 * ============================================================================
 * PROBLEM: Read/Write Multi-byte Values from Byte Arrays
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: HIGH (Embedded)
 *
 * Read/write 16/32-bit values from byte arrays with explicit endianness.
 *
 * ============================================================================
 * USE CASE:
 * ============================================================================
 *
 * - Parsing network packets
 * - Reading binary file formats
 * - Communicating with sensors over I2C/SPI
 * - Protocol buffers/serialization
 *
 * ============================================================================
 * WHY NOT JUST CAST?
 * ============================================================================
 *
 * Casting can fail due to:
 * 1. Alignment requirements
 * 2. Unknown endianness of source data
 * 3. Strict aliasing rules
 *
 * Explicit byte-by-byte is ALWAYS safe and portable.
 *
 * ============================================================================
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why not use pointer casting?"
 * A1: Three problems: (1) Alignment - some CPUs crash on unaligned access.
 *     (2) Endianness - don't know source data format. (3) Aliasing - compiler
 *     may optimize incorrectly. Byte-by-byte is always safe and portable.
 *
 * -------------------------------------------------------------------------
 * Q2: "Is memcpy faster than byte-by-byte?"
 * A2: For small sizes (2-4 bytes), compiler usually generates same code.
 *     memcpy is clearer for larger structures. Both are O(1) for fixed sizes.
 *     Byte-by-byte explicitly handles endianness; memcpy doesn't.
 *
 * -------------------------------------------------------------------------
 * Q3: "How do you handle unaligned data efficiently?"
 * A3: Some architectures support unaligned access (x86), others don't (ARM).
 *     On ARM, use __packed attribute or byte-by-byte. On x86, casting may work
 *     but byte-by-byte is more portable. Let compiler optimize.
 *
 * -------------------------------------------------------------------------
 * Q4: "What about reading signed values?"
 * A4: Read as unsigned, then cast: (int16_t)readBE16(bytes).
 *     Sign extension happens automatically. Or read into union with
 *     signed/unsigned members sharing storage.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// ============================================================================
// READ FUNCTIONS
// ============================================================================

uint16_t readBE16(const uint8_t* bytes) {
    return ((uint16_t)bytes[0] << 8) | bytes[1];
}

uint16_t readLE16(const uint8_t* bytes) {
    return ((uint16_t)bytes[1] << 8) | bytes[0];
}

uint32_t readBE32(const uint8_t* bytes) {
    return ((uint32_t)bytes[0] << 24) |
           ((uint32_t)bytes[1] << 16) |
           ((uint32_t)bytes[2] << 8)  |
           (uint32_t)bytes[3];
}

uint32_t readLE32(const uint8_t* bytes) {
    return ((uint32_t)bytes[3] << 24) |
           ((uint32_t)bytes[2] << 16) |
           ((uint32_t)bytes[1] << 8)  |
           (uint32_t)bytes[0];
}

// ============================================================================
// WRITE FUNCTIONS
// ============================================================================

void writeBE16(uint8_t* bytes, uint16_t value) {
    bytes[0] = (value >> 8) & 0xFF;
    bytes[1] = value & 0xFF;
}

void writeLE16(uint8_t* bytes, uint16_t value) {
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
}

void writeBE32(uint8_t* bytes, uint32_t value) {
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
}

void writeLE32(uint8_t* bytes, uint32_t value) {
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;
}

void printBytes(const char* label, uint8_t* bytes, int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) {
        printf("0x%02X", bytes[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    printf("=== Read/Write Multi-byte Values ===\n\n");

    // Reading from byte array
    printf("1. Reading from byte arrays:\n");
    uint8_t bytes[] = {0x12, 0x34, 0x56, 0x78};
    printBytes("Bytes", bytes, 4);
    printf("  Read as BE16: 0x%04X\n", readBE16(bytes));
    printf("  Read as LE16: 0x%04X\n", readLE16(bytes));
    printf("  Read as BE32: 0x%08X\n", readBE32(bytes));
    printf("  Read as LE32: 0x%08X\n\n", readLE32(bytes));

    // Writing to byte array
    printf("2. Writing to byte arrays:\n");
    uint8_t outBE[4], outLE[4];
    uint32_t value = 0x12345678;

    printf("  Value: 0x%08X\n", value);
    writeBE32(outBE, value);
    writeLE32(outLE, value);
    printBytes("  Written as BE", outBE, 4);
    printBytes("  Written as LE", outLE, 4);
    printf("\n");

    // Practical example: Parsing a packet header
    printf("3. Example: Parse packet header\n");
    // Simulated network packet (big-endian)
    uint8_t packet[] = {
        0x00, 0x50,             // src port: 80
        0x1F, 0x90,             // dst port: 8080
        0x00, 0x00, 0x00, 0x64  // sequence: 100
    };

    uint16_t srcPort = readBE16(packet);
    uint16_t dstPort = readBE16(packet + 2);
    uint32_t seq = readBE32(packet + 4);

    printf("  Src Port: %d\n", srcPort);
    printf("  Dst Port: %d\n", dstPort);
    printf("  Sequence: %u\n", seq);

    return 0;
}
