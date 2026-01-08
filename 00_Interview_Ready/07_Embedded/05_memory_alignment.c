/*
 * ============================================================================
 * PROBLEM: Memory Alignment and Struct Packing
 * ============================================================================
 *
 * WHAT IS THIS?
 * Memory alignment refers to how data is arranged in memory based on address
 * boundaries. CPUs access memory most efficiently when data is "aligned" -
 * meaning its address is a multiple of its size. The compiler adds "padding"
 * bytes between struct members to ensure proper alignment, which increases
 * struct size. "Packing" removes this padding when exact memory layout is needed.
 *
 * WHY IS THIS CRITICAL FOR EMBEDDED SYSTEMS?
 * - Protocol Structures: Network/serial packets must match exact byte layout
 * - Hardware Registers: Register maps require precise memory mapping
 * - Memory Constraints: Padding wastes precious RAM on small MCUs
 * - Performance: Misaligned access can be 2-10x slower or cause faults
 * - DMA Transfers: DMA often requires aligned buffers
 * - Inter-Processor Communication: Shared structs must have same layout
 *
 * EXAMPLES:
 * Unoptimized struct (16 bytes wasted space):
 *   struct { uint8_t a; uint32_t b; uint8_t c; uint16_t d; }; // 16 bytes!
 *
 * Optimized struct (8 bytes, no waste):
 *   struct { uint32_t b; uint16_t d; uint8_t a; uint8_t c; }; // 8 bytes!
 *
 * Protocol packet (must be packed):
 *   #pragma pack(1)
 *   struct { uint8_t cmd; uint32_t addr; uint16_t len; }; // 7 bytes exact
 *
 * KEY CONCEPT:
 * Alignment Rule: A variable of size N must be at address divisible by N.
 * - uint8_t: any address (1-byte aligned)
 * - uint16_t: even address (2-byte aligned)
 * - uint32_t: address divisible by 4 (4-byte aligned)
 *
 * VISUAL:
 *
 *   STRUCT WITH PADDING (Default):
 *
 *   struct Example {
 *       uint8_t  a;    // 1 byte
 *       uint32_t b;    // 4 bytes
 *       uint8_t  c;    // 1 byte
 *       uint16_t d;    // 2 bytes
 *   };
 *
 *   Memory Layout:
 *   Offset:  0     1  2  3     4  5  6  7     8     9 10    11 12 13 14 15
 *          +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 *          | a | P | P | P | b | b | b | b | c | P | d | d | P | P | P | P |
 *          +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 *            ^   ^-------^   ^-----------^   ^   ^   ^---^   ^-----------^
 *            |   padding     uint32_t        |  pad  uint16  struct padding
 *          uint8_t           (4-aligned)   uint8_t  (2-aligned)
 *
 *   Total: 16 bytes (8 bytes padding = 50% waste!)
 *
 *
 *   OPTIMIZED MEMBER ORDER:
 *
 *   struct Optimized {
 *       uint32_t b;    // 4 bytes (largest first)
 *       uint16_t d;    // 2 bytes
 *       uint8_t  a;    // 1 byte
 *       uint8_t  c;    // 1 byte
 *   };
 *
 *   Offset:  0  1  2  3     4  5     6     7
 *          +---+---+---+---+---+---+---+---+
 *          | b | b | b | b | d | d | a | c |
 *          +---+---+---+---+---+---+---+---+
 *            ^-----------^   ^---^   ^   ^
 *            uint32_t        uint16  u8  u8
 *            (4-aligned)     (2-aligned)
 *
 *   Total: 8 bytes (0 bytes padding = 0% waste!)
 *
 *
 *   PACKED STRUCT (for protocols):
 *
 *   #pragma pack(push, 1)
 *   struct Packet {
 *       uint8_t  cmd;      // 1 byte
 *       uint32_t address;  // 4 bytes
 *       uint16_t length;   // 2 bytes
 *   };
 *   #pragma pack(pop)
 *
 *   Memory Layout (matches wire format exactly):
 *   Offset:  0     1  2  3  4     5  6
 *          +---+---+---+---+---+---+---+
 *          |cmd|  address  | length  |
 *          +---+---+---+---+---+---+---+
 *
 *   Total: 7 bytes (exact protocol size)
 *
 *
 *   ALIGNMENT REQUIREMENTS BY TYPE:
 *
 *   Type         Size    Alignment   Valid Addresses
 *   --------     ----    ---------   ---------------
 *   uint8_t      1       1           0, 1, 2, 3, 4, 5...
 *   uint16_t     2       2           0, 2, 4, 6, 8...
 *   uint32_t     4       4           0, 4, 8, 12, 16...
 *   uint64_t     8       8           0, 8, 16, 24...
 *   pointer      4/8     4/8         depends on arch
 *
 * ============================================================================
 */

// Memory Alignment & Struct Packing - ESSENTIAL for embedded
// Understanding padding, alignment, and packed structures

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>  // For offsetof

// ============================================================
// EXAMPLE 1: Default Alignment (with padding)
// ============================================================

// Say: "Compiler adds padding for alignment"
struct DefaultAlign {
    uint8_t  a;   // 1 byte + 3 padding
    uint32_t b;   // 4 bytes (must be 4-byte aligned)
    uint8_t  c;   // 1 byte + 3 padding
    uint16_t d;   // 2 bytes + 2 padding
};  // Total: 16 bytes (not 8!)

// ============================================================
// EXAMPLE 2: Optimized Ordering (minimize padding)
// ============================================================

// Say: "Reorder members to minimize padding"
struct OptimizedAlign {
    uint32_t b;   // 4 bytes (naturally aligned)
    uint16_t d;   // 2 bytes
    uint8_t  a;   // 1 byte
    uint8_t  c;   // 1 byte
};  // Total: 8 bytes (no wasted space!)

// ============================================================
// EXAMPLE 3: Packed Structure (no padding)
// ============================================================

// Say: "Packed forces no padding - for protocol/hardware structures"
#pragma pack(push, 1)  // Say: "Push current alignment, set to 1 byte"
struct PackedStruct {
    uint8_t  header;
    uint32_t data;
    uint16_t checksum;
};  // Total: 7 bytes exactly
#pragma pack(pop)  // Say: "Restore previous alignment"

// Alternative GCC attribute syntax
struct __attribute__((packed)) GCCPacked {
    uint8_t  header;
    uint32_t data;
    uint16_t checksum;
};

// ============================================================
// EXAMPLE 4: Protocol Structure (real-world)
// ============================================================

#pragma pack(push, 1)
// Say: "Matches wire format of protocol exactly"
typedef struct {
    uint8_t  start_byte;     // 0x55
    uint8_t  message_type;
    uint16_t payload_length;
    uint8_t  payload[64];
    uint16_t crc;
} ProtocolPacket;
#pragma pack(pop)

// ============================================================
// HELPER FUNCTIONS
// ============================================================

void print_struct_info(const char *name, size_t size, size_t expected) {
    printf("%-20s Size: %2zu bytes (expected: %zu) %s\n",
           name, size, expected,
           size == expected ? "✓" : "PADDING!");
}

void analyze_default_align(void) {
    struct DefaultAlign s;

    printf("\n=== Default Alignment Analysis ===\n");
    printf("Member   Offset  Size\n");
    printf("a        %zu       1\n", offsetof(struct DefaultAlign, a));
    printf("b        %zu       4\n", offsetof(struct DefaultAlign, b));
    printf("c        %zu       1\n", offsetof(struct DefaultAlign, c));
    printf("d        %zu       2\n", offsetof(struct DefaultAlign, d));
    printf("Total size: %zu\n", sizeof(struct DefaultAlign));
}

int main() {
    printf("=== Memory Alignment Demo ===\n");

    // Compare sizes
    printf("\n--- Structure Sizes ---\n");
    print_struct_info("DefaultAlign", sizeof(struct DefaultAlign), 8);
    print_struct_info("OptimizedAlign", sizeof(struct OptimizedAlign), 8);
    print_struct_info("PackedStruct", sizeof(struct PackedStruct), 7);
    print_struct_info("ProtocolPacket", sizeof(ProtocolPacket), 69);

    // Detailed offset analysis
    analyze_default_align();

    printf("\n=== Optimized Alignment Analysis ===\n");
    printf("Member   Offset  Size\n");
    printf("b        %zu       4\n", offsetof(struct OptimizedAlign, b));
    printf("d        %zu       2\n", offsetof(struct OptimizedAlign, d));
    printf("a        %zu       1\n", offsetof(struct OptimizedAlign, a));
    printf("c        %zu       1\n", offsetof(struct OptimizedAlign, c));
    printf("Total size: %zu\n", sizeof(struct OptimizedAlign));

    // Alignment requirements
    printf("\n=== Type Alignment Requirements ===\n");
    printf("uint8_t:  %zu byte alignment\n", _Alignof(uint8_t));
    printf("uint16_t: %zu byte alignment\n", _Alignof(uint16_t));
    printf("uint32_t: %zu byte alignment\n", _Alignof(uint32_t));
    printf("uint64_t: %zu byte alignment\n", _Alignof(uint64_t));
    printf("pointer:  %zu byte alignment\n", _Alignof(void*));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Memory alignment affects struct size, performance, and hardware compatibility.

 WHY ALIGNMENT MATTERS:
 1. Performance: Misaligned access is slower (extra cycles)
 2. Correctness: Some CPUs fault on misaligned access (ARM Cortex-M0)
 3. Hardware: Peripheral registers require specific alignment
 4. Protocols: Network/serial protocols need exact byte layout

 ALIGNMENT RULES:
 - Variables aligned to their size (uint32_t on 4-byte boundary)
 - Struct aligned to largest member's alignment
 - Compiler adds padding to meet alignment

 EXAMPLE PADDING:
 struct {
     uint8_t a;    // offset 0
     // 3 bytes padding
     uint32_t b;   // offset 4 (must be 4-byte aligned)
     uint8_t c;    // offset 8
     // 1 byte padding
     uint16_t d;   // offset 10 (must be 2-byte aligned)
     // 2 bytes padding (struct size must be multiple of 4)
 };  // Total: 16 bytes

 OPTIMIZATION - REORDER MEMBERS:
 - Put larger types first, smaller types last
 - Group same-sized members together
 - Can reduce struct size significantly

 PACKING - WHEN TO USE:
 1. Protocol structures (must match wire format)
 2. Hardware register definitions
 3. File format structures
 4. Inter-processor communication

 PACKING SYNTAX:
 GCC:     __attribute__((packed))
 MSVC:    #pragma pack(push, 1) ... #pragma pack(pop)
 C11:     _Alignas() for explicit alignment

 PACKING DRAWBACKS:
 - Slower access (multiple memory reads)
 - Potential for faults on strict-alignment CPUs
 - Pointer to packed member is dangerous

 INTERVIEW TIPS:
 1. Know why padding exists (alignment requirements)
 2. Know how to minimize padding (reorder members)
 3. Know when packing is necessary (protocols)
 4. Know packing risks (performance, faults)"
*/
