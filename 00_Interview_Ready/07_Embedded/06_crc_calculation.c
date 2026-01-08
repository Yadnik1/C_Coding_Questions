/*
 * ============================================================================
 * PROBLEM: CRC (Cyclic Redundancy Check) Calculation
 * ============================================================================
 *
 * WHAT IS THIS?
 * CRC is an error-detection algorithm that calculates a checksum (digest) of
 * data by treating it as a polynomial and dividing by a generator polynomial.
 * The remainder is the CRC value. If data is corrupted during transmission/
 * storage, the recalculated CRC won't match, detecting the error.
 *
 * WHY IS THIS CRITICAL FOR EMBEDDED SYSTEMS?
 * - Communication Protocols: UART, SPI, CAN, Modbus all use CRC
 * - Flash Memory: Verify firmware integrity after programming
 * - OTA Updates: Ensure downloaded firmware is not corrupted
 * - Sensor Data: Validate readings from remote sensors
 * - SD Card/File Systems: Detect storage corruption
 * - Boot Verification: Check bootloader/application integrity at startup
 *
 * EXAMPLES:
 * UART Packet with CRC:
 *   [Header][Length][Payload....][CRC16]
 *   Receiver calculates CRC of Header+Length+Payload
 *   If matches received CRC16 -> data good
 *   If mismatch -> request retransmission
 *
 * Firmware Verification:
 *   CRC32 of 256KB firmware stored at end of flash
 *   Bootloader recalculates CRC32 at boot
 *   Match = run app, Mismatch = stay in bootloader
 *
 * KEY CONCEPT:
 * Two implementation methods:
 * 1. Bitwise: Process each bit, slow but small code
 * 2. Table-Lookup: Pre-computed 256-entry table, fast but uses 256-1024 bytes
 *
 * Core operation: XOR data with polynomial when MSB is 1, then shift
 *
 * VISUAL:
 *
 *   CRC CALCULATION CONCEPT:
 *
 *   Data:       1 0 1 1 0 1 0 0  (0xB4)
 *   Polynomial: 1 0 0 0 0 0 1 1 1  (0x107, but we use 0x07 as implicit MSB)
 *
 *   Process each bit:
 *   +---------+--------+------------------------------------------+
 *   | Step    | CRC    | Operation                                |
 *   +---------+--------+------------------------------------------+
 *   | Init    | 00     |                                          |
 *   | XOR B4  | B4     | CRC ^= data_byte                         |
 *   | Bit 7=1 | 6B     | MSB=1: (CRC<<1) ^ POLY = (0x168)^07 = 6B |
 *   | Bit 6=0 | D6     | MSB=0: CRC<<1 = D6                       |
 *   | ...     | ...    | Continue for all 8 bits                  |
 *   | Final   | XX     | CRC value after all bits                 |
 *   +---------+--------+------------------------------------------+
 *
 *
 *   BITWISE VS TABLE-DRIVEN:
 *
 *   BITWISE (slow, small):            TABLE-DRIVEN (fast, larger):
 *   +------------------------+        +------------------------+
 *   | for each byte:         |        | for each byte:         |
 *   |   CRC ^= byte          |        |   index = CRC ^ byte   |
 *   |   for 8 bits:          |        |   CRC = table[index]   |
 *   |     if MSB set:        |        +------------------------+
 *   |       CRC = CRC<<1^POLY|             |
 *   |     else:              |             v
 *   |       CRC = CRC<<1     |        +------------------------+
 *   +------------------------+        | Table[256] precomputed |
 *          |                          +------------------------+
 *          v
 *   8 iterations per byte             1 lookup per byte
 *   ~40 cycles per byte               ~5 cycles per byte
 *
 *
 *   COMMON CRC TYPES:
 *
 *   +--------+------+------------+------------------+------------+
 *   | Type   | Bits | Polynomial | Init    | Final  | Use Case   |
 *   +--------+------+------------+---------+--------+------------+
 *   | CRC-8  |  8   | 0x07       | 0x00    | 0x00   | I2C, 1-Wire|
 *   | CRC-16 | 16   | 0x1021     | 0xFFFF  | 0x0000 | Modbus,X.25|
 *   | CRC-32 | 32   | 0xEDB88320 | 0xFFFF  | 0xFFFF | Ethernet   |
 *   +--------+------+------------+---------+--------+------------+
 *
 *
 *   DATA FLOW WITH CRC:
 *
 *   TRANSMITTER:                      RECEIVER:
 *   +-------+                         +-------+
 *   | Data  | -----> Calculate -----> | Data  | --+
 *   +-------+        CRC              +-------+   |
 *       |             |                   |       |
 *       v             v                   v       v
 *   +-------+-----+               +-------+-----+ |
 *   | Data  | CRC | --transmit--> | Data  | CRC | |
 *   +-------+-----+               +-------+-----+ |
 *                                        |        |
 *                                        v        v
 *                                   Calculate   Compare
 *                                      CRC    <----+
 *                                        |
 *                                        v
 *                                   Match? Good!
 *                                   Mismatch? Error!
 *
 * ============================================================================
 */

// CRC Calculation - ESSENTIAL for embedded data integrity
// Time: O(n) for n bytes, Space: O(1) for bitwise, O(256) for table

#include <stdio.h>
#include <stdint.h>
#include <string.h>

// ============================================================
// CRC-8 (Simple, common in embedded)
// ============================================================

// Say: "CRC-8 polynomial: x^8 + x^2 + x^1 + 1 = 0x07"
#define CRC8_POLY 0x07

// Say: "Bitwise CRC-8 - simple but slower"
uint8_t crc8_bitwise(uint8_t *data, size_t len) {
    uint8_t crc = 0x00;  // Say: "Initial value"

    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];  // Say: "XOR byte into CRC"

        // Say: "Process each bit"
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x80) {  // Say: "If MSB is 1"
                crc = (crc << 1) ^ CRC8_POLY;  // Say: "Shift and XOR with poly"
            } else {
                crc <<= 1;  // Say: "Just shift"
            }
        }
    }
    return crc;
}

// Say: "Lookup table for faster CRC-8"
static const uint8_t crc8_table[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
    0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
    0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
    0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
    0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    // ... (remaining 192 entries would go here)
    // Using shortened table for demo
};

uint8_t crc8_table_lookup(uint8_t *data, size_t len) {
    uint8_t crc = 0x00;
    for (size_t i = 0; i < len; i++) {
        crc = crc8_table[crc ^ data[i]];
    }
    return crc;
}

// ============================================================
// CRC-16 (Common: CRC-16-CCITT)
// ============================================================

// Say: "CRC-16-CCITT polynomial: 0x1021"
#define CRC16_POLY 0x1021

uint16_t crc16_ccitt(uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;  // Say: "Standard initial value"

    for (size_t i = 0; i < len; i++) {
        crc ^= ((uint16_t)data[i] << 8);  // Say: "XOR byte into high byte"

        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ CRC16_POLY;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

// ============================================================
// CRC-32 (Most common: Ethernet, ZIP, etc.)
// ============================================================

// Say: "CRC-32 polynomial (reflected): 0xEDB88320"
#define CRC32_POLY 0xEDB88320

// Say: "Generate CRC-32 lookup table"
static uint32_t crc32_table[256];
static int crc32_table_generated = 0;

void generate_crc32_table(void) {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ CRC32_POLY;  // Say: "Reflected algorithm"
            } else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
    crc32_table_generated = 1;
}

uint32_t crc32(uint8_t *data, size_t len) {
    if (!crc32_table_generated) {
        generate_crc32_table();
    }

    uint32_t crc = 0xFFFFFFFF;  // Say: "Initial value"

    for (size_t i = 0; i < len; i++) {
        // Say: "Table lookup with XOR"
        crc = crc32_table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }

    return crc ^ 0xFFFFFFFF;  // Say: "Final XOR"
}

int main() {
    printf("=== CRC Calculation Demo ===\n\n");

    // Test data
    uint8_t test_data[] = "123456789";  // Say: "Standard test vector"
    size_t len = 9;  // Without null terminator

    printf("Test data: \"%s\" (%zu bytes)\n\n", test_data, len);

    // CRC-8
    printf("CRC-8 (poly=0x07):\n");
    printf("  Bitwise:  0x%02X\n", crc8_bitwise(test_data, len));

    // CRC-16
    printf("\nCRC-16-CCITT (poly=0x1021):\n");
    printf("  Result:   0x%04X\n", crc16_ccitt(test_data, len));
    // Expected: 0x29B1 for "123456789"

    // CRC-32
    printf("\nCRC-32 (poly=0xEDB88320):\n");
    printf("  Result:   0x%08X\n", crc32(test_data, len));
    // Expected: 0xCBF43926 for "123456789"

    // Verify data integrity
    printf("\n=== Data Integrity Check ===\n");
    uint8_t packet[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t original_crc = crc8_bitwise(packet, 5);
    printf("Original CRC-8: 0x%02X\n", original_crc);

    // Corrupt one byte
    packet[2] = 0xFF;
    uint8_t corrupted_crc = crc8_bitwise(packet, 5);
    printf("After corruption: 0x%02X\n", corrupted_crc);
    printf("CRCs match: %s\n", (original_crc == corrupted_crc) ? "YES (bad!)" : "NO (corruption detected)");

    return 0;
}

/*
INTERVIEW EXPLANATION:
"CRC (Cyclic Redundancy Check) detects errors in data transmission/storage.

 HOW CRC WORKS:
 - Treat data as a large polynomial
 - Divide by a generator polynomial
 - Remainder is the CRC value
 - Implemented using XOR and shifts

 COMMON CRC TYPES:
 | Type    | Bits | Polynomial | Use Case                |
 |---------|------|------------|-------------------------|
 | CRC-8   | 8    | 0x07       | I2C, SMBus, simple      |
 | CRC-16  | 16   | 0x1021     | Modbus, X.25            |
 | CRC-32  | 32   | 0x04C11DB7 | Ethernet, ZIP, PNG      |

 IMPLEMENTATION OPTIONS:
 1. Bitwise: Process bit by bit
    - Small code size
    - Slow (8 iterations per byte)
    - Good for small data or limited RAM

 2. Table-driven: Lookup table
    - Fast (one lookup per byte)
    - 256 entries per table
    - Good for large data or speed critical

 CRC PARAMETERS:
 - Polynomial: Determines error detection capability
 - Initial value: Starting CRC value
 - Final XOR: XOR applied to result
 - Reflection: Bit order (MSB or LSB first)

 WHY CRC NOT CHECKSUM:
 - CRC detects burst errors (multiple consecutive bits)
 - Checksum (sum of bytes) misses errors that cancel out
 - CRC has mathematical properties for error detection

 EMBEDDED APPLICATIONS:
 1. Communication protocols (UART, SPI)
 2. Flash memory integrity
 3. OTA update verification
 4. Sensor data validation
 5. File system integrity

 COMMON MISTAKES:
 - Wrong polynomial
 - Wrong initial value
 - Forgetting final XOR
 - Wrong reflection (MSB vs LSB)
 - Not including length in CRC"
*/
