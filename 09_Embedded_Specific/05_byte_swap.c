/*
 * ============================================================================
 * PROBLEM: Byte Swapping (16, 32, 64-bit)
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: VERY HIGH (Embedded)
 *
 * Swap byte order for endianness conversion.
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 * 16-bit:
 *   0x1234 → 0x3412
 *   [12][34] → [34][12]
 *
 * 32-bit:
 *   0x12345678 → 0x78563412
 *   [12][34][56][78] → [78][56][34][12]
 *
 * ============================================================================
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Are there built-in functions for byte swapping?"
 * A1: Yes! GCC has __builtin_bswap16/32/64. x86 has BSWAP instruction.
 *     In production, use built-ins - they compile to single instruction.
 *     Know manual method for interviews and platforms without built-ins.
 *
 * -------------------------------------------------------------------------
 * Q2: "Why use masks in 32-bit swap?"
 * A2: To isolate each byte before OR-ing. Without masks, bits from high bytes
 *     could bleed into low positions after shift. Masks ensure clean isolation.
 *     Some compilers optimize this, but explicit masks are safer.
 *
 * -------------------------------------------------------------------------
 * Q3: "What about signed integers?"
 * A3: Byte swap doesn't care about signedness - it's pure bit manipulation.
 *     Cast to unsigned, swap, cast back if needed. Result is correct because
 *     we're not interpreting values, just rearranging bytes.
 *
 * -------------------------------------------------------------------------
 * Q4: "How do you swap array of values efficiently?"
 * A4: Loop and swap each element. For large arrays, SIMD can swap multiple
 *     values in parallel. Some processors have vector byte swap instructions.
 *     For network buffers, consider leaving in network order until needed.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// 16-bit swap
uint16_t swap16(uint16_t value) {
    return (value >> 8) | (value << 8);
}

// 32-bit swap
uint32_t swap32(uint32_t value) {
    return ((value >> 24) & 0x000000FF) |
           ((value >> 8)  & 0x0000FF00) |
           ((value << 8)  & 0x00FF0000) |
           ((value << 24) & 0xFF000000);
}

// 32-bit swap (alternative using 16-bit swaps)
uint32_t swap32_v2(uint32_t value) {
    value = ((value >> 16) | (value << 16));  // Swap 16-bit halves
    value = ((value & 0xFF00FF00) >> 8) | ((value & 0x00FF00FF) << 8);
    return value;
}

// 64-bit swap
uint64_t swap64(uint64_t value) {
    return ((value >> 56) & 0x00000000000000FFULL) |
           ((value >> 40) & 0x000000000000FF00ULL) |
           ((value >> 24) & 0x0000000000FF0000ULL) |
           ((value >> 8)  & 0x00000000FF000000ULL) |
           ((value << 8)  & 0x000000FF00000000ULL) |
           ((value << 24) & 0x0000FF0000000000ULL) |
           ((value << 40) & 0x00FF000000000000ULL) |
           ((value << 56) & 0xFF00000000000000ULL);
}

int main() {
    printf("=== Byte Swapping ===\n\n");

    // 16-bit
    uint16_t val16 = 0x1234;
    printf("16-bit swap:\n");
    printf("  0x%04X → 0x%04X\n\n", val16, swap16(val16));

    // 32-bit
    uint32_t val32 = 0x12345678;
    printf("32-bit swap:\n");
    printf("  0x%08X → 0x%08X\n\n", val32, swap32(val32));

    // 64-bit
    uint64_t val64 = 0x123456789ABCDEF0ULL;
    printf("64-bit swap:\n");
    printf("  0x%016llX → 0x%016llX\n\n",
           (unsigned long long)val64, (unsigned long long)swap64(val64));

    printf("=== Breakdown (32-bit) ===\n");
    printf("Original: 0x12345678\n");
    printf("  Byte 0: 0x12 → position 3\n");
    printf("  Byte 1: 0x34 → position 2\n");
    printf("  Byte 2: 0x56 → position 1\n");
    printf("  Byte 3: 0x78 → position 0\n");
    printf("Result:   0x78563412\n");

    return 0;
}
