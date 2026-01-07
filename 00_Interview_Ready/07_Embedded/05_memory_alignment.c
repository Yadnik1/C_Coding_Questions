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
