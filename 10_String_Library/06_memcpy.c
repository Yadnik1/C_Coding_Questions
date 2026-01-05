/*
 * ============================================================================
 * PROBLEM: Implement memcpy()
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: VERY HIGH
 *
 * memcpy copies n bytes from src to dest
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. VOID POINTERS:
 *    - void* can point to ANY type of data
 *    - Must cast to unsigned char* for byte-by-byte copying
 *    - Works with int, float, struct, anything!
 *
 * 2. UNDEFINED BEHAVIOR WITH OVERLAP:
 *    - memcpy assumes src and dest DO NOT overlap
 *    - If they overlap, behavior is undefined (can corrupt data)
 *    - Use memmove() for overlapping regions
 *
 * 3. BYTE COUNT (not element count):
 *    - n = number of BYTES, not number of elements
 *    - For int array: memcpy(dest, src, sizeof(int) * count)
 *    - Or use: memcpy(dest, src, sizeof(arr))
 *
 * ============================================================================
 * VISUALIZATION: memcpy(dest, src, 5)
 * ============================================================================
 *
 *   src:  [A][B][C][D][E]
 *   dest: [?][?][?][?][?]
 *
 *   Step 1: dest[0] = src[0]  →  [A][?][?][?][?]
 *   Step 2: dest[1] = src[1]  →  [A][B][?][?][?]
 *   Step 3: dest[2] = src[2]  →  [A][B][C][?][?]
 *   Step 4: dest[3] = src[3]  →  [A][B][C][D][?]
 *   Step 5: dest[4] = src[4]  →  [A][B][C][D][E]
 *
 *   Result: dest = [A][B][C][D][E]
 *
 * ============================================================================
 * OVERLAP PROBLEM (WHY USE memmove):
 * ============================================================================
 *
 *   buf: [A][B][C][D][E][F]
 *   Goal: memcpy(buf+2, buf, 4)  // Copy ABCD to position 2
 *
 *   Expected result: [A][B][A][B][C][D]
 *
 *   What happens with forward copy:
 *   Step 1: buf[2] = buf[0] = 'A'  →  [A][B][A][D][E][F]
 *   Step 2: buf[3] = buf[1] = 'B'  →  [A][B][A][B][E][F]
 *   Step 3: buf[4] = buf[2] = 'A'  →  [A][B][A][B][A][F]  ← WRONG!
 *   Step 4: buf[5] = buf[3] = 'B'  →  [A][B][A][B][A][B]  ← WRONG!
 *
 *   Source data was overwritten before being copied!
 *   This is why overlapping memcpy is UNDEFINED BEHAVIOR.
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   cast dest and src to unsigned char*
 *   while (n > 0):
 *       *d++ = *s++
 *       n--
 *   return original dest
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Must copy each byte once
 * - n = number of bytes to copy
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only pointer variables
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why does memcpy have undefined behavior with overlapping memory?"
 * A1: For performance! Without checking for overlap, memcpy can copy in any
 *     direction and use optimized word-sized copies. The standard allows this
 *     flexibility. If overlap is possible, you MUST use memmove.
 *
 * -------------------------------------------------------------------------
 * Q2: "How do optimized memcpy implementations achieve high throughput?"
 * A2: Multiple techniques: 1) Word-aligned copies (8 bytes at a time),
 *     2) SIMD instructions (16-64 bytes), 3) Non-temporal stores for large
 *     copies (bypass cache), 4) Rep movsb on modern x86 (hardware optimized).
 *     Modern libc memcpy can reach memory bandwidth limits.
 *
 * -------------------------------------------------------------------------
 * Q3: "Can memcpy copy structs safely?"
 * A3: Yes for POD (plain old data) types without pointers. Copies raw bytes
 *     including padding. For structs with pointers, you get shallow copy -
 *     both copies point to same memory! For deep copy, implement manually.
 *
 * -------------------------------------------------------------------------
 * Q4: "What's restrict keyword and how does it help memcpy?"
 * A4: restrict tells compiler the pointers don't overlap, enabling more
 *     aggressive optimizations. Standard memcpy declaration uses restrict:
 *     void *memcpy(void *restrict dest, const void *restrict src, size_t n).
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

/*
 * ============================================================================
 * MEMCPY FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void* my_memcpy(void* dest, const void* src, size_t n):
 *   - Returns "void*" = pointer to dest (for chaining)
 *   - "void* dest" = destination memory (we modify it)
 *   - "const void* src" = source memory (we don't modify)
 *   - "size_t n" = number of bytes to copy
 *
 * unsigned char* d = (unsigned char*)dest:
 *   - Cast void* to unsigned char* for byte access
 *   - WHY void*? Works with ANY data type
 *   - WHY unsigned char? Byte values 0-255, no sign issues
 *
 * const unsigned char* s = (const unsigned char*)src:
 *   - Same for source, but const (we don't modify source)
 *
 * while (n > 0) *d++ = *s++; n--;
 *   - Copy one byte from source to dest
 *   - Advance both pointers
 *   - Decrement remaining count
 *
 * return dest:
 *   - Return original dest pointer (not advanced!)
 *   - Enables chaining
 *
 * ============================================================================
 */
// memcpy - Copy n bytes from src to dest
// Say: "I'll implement memcpy by copying bytes one at a time from source to destination"
void* my_memcpy(void* dest, const void* src, size_t n) {
    // Validate input pointers
    // Say: "First, I check that both pointers are valid"
    // WHY: Dereferencing NULL causes crash
    if (dest == NULL || src == NULL) return NULL;

    // Cast void pointers to unsigned char pointers for byte-level access
    // Say: "I cast both pointers to unsigned char so I can copy byte by byte"
    // WHY: void* cannot be dereferenced, need byte-sized type
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    // Copy n bytes from source to destination
    // Say: "Now I copy each byte from source to destination"
    // WHY: Simple loop copies one byte at a time
    while (n > 0) {
        // Copy byte and advance both pointers
        // Say: "I copy a byte and advance both pointers"
        *d++ = *s++;
        // Decrement remaining count
        // Say: "I decrement the count of remaining bytes"
        n--;
    }

    // Return the original destination pointer
    // Say: "Return the original dest pointer"
    // WHY: Enables chaining, returns where data was copied to
    return dest;
}

/*
 * ============================================================================
 * OPTIMIZED MEMCPY - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Key insight: Copying 8 bytes at once is faster than 1 byte at a time
 *
 * Steps:
 * 1. Copy unaligned prefix bytes (0-7 bytes)
 * 2. Copy aligned middle using 64-bit loads/stores
 * 3. Copy unaligned suffix bytes (0-7 bytes)
 *
 * Alignment check:
 *   ((uintptr_t)ptr & 7) == 0 means ptr is 8-byte aligned
 *   The & 7 checks if lowest 3 bits are zero
 *
 * ============================================================================
 */
// Optimized version (copy words when aligned)
// Say: "Here's an optimized version that copies 8 bytes at a time when pointers are aligned"
void* my_memcpy_fast(void* dest, const void* src, size_t n) {
    // Validate input pointers
    // Say: "Check for NULL pointers"
    if (dest == NULL || src == NULL) return NULL;

    // Set up byte pointers
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    // Copy bytes until dest is 8-byte aligned
    // Say: "First, I copy bytes one at a time until dest is 8-byte aligned"
    // WHY: Aligned 64-bit access is faster and required on some architectures
    while (n > 0 && ((uintptr_t)d & 7)) {   // Check alignment
        // Copy one byte
        // Say: "I copy one byte at a time until aligned"
        *d++ = *s++;
        n--;
    }

    // If src is also aligned, we can copy 8 bytes at a time
    // Say: "If source is also aligned, I can copy 8 bytes at a time for better performance"
    // WHY: Both need to be aligned for fast 64-bit operations
    if (((uintptr_t)s & 7) == 0) {  // src also aligned
        // Cast to 64-bit pointers
        uint64_t* d64 = (uint64_t*)d;
        const uint64_t* s64 = (const uint64_t*)s;

        // Copy 8 bytes at a time
        // Say: "Copy 64 bits at a time while we have at least 8 bytes left"
        while (n >= 8) {
            // Copy 8 bytes
            // Say: "I copy 8 bytes in one operation"
            *d64++ = *s64++;
            n -= 8;
        }

        // Update byte pointers to where we left off
        d = (unsigned char*)d64;
        s = (const unsigned char*)s64;
    }

    // Copy remaining bytes (less than 8)
    // Say: "Finally, I copy any remaining bytes one at a time"
    while (n > 0) {
        // Copy remaining bytes
        // Say: "I copy the remaining bytes"
        *d++ = *s++;
        n--;
    }

    // Return the original destination pointer
    return dest;
}

int main() {
    printf("=== memcpy Implementation ===\n\n");

    // Basic copy
    printf("1. Basic memcpy:\n");
    char src[] = "Hello";
    char dest[10];
    my_memcpy(dest, src, 6);  // Include null terminator
    printf("   src:  \"%s\"\n", src);
    printf("   dest: \"%s\"\n\n", dest);

    // Copy integers
    printf("2. Copy integer array:\n");
    int srcArr[] = {1, 2, 3, 4, 5};
    int destArr[5];
    my_memcpy(destArr, srcArr, sizeof(srcArr));
    printf("   src:  [%d, %d, %d, %d, %d]\n",
           srcArr[0], srcArr[1], srcArr[2], srcArr[3], srcArr[4]);
    printf("   dest: [%d, %d, %d, %d, %d]\n\n",
           destArr[0], destArr[1], destArr[2], destArr[3], destArr[4]);

    // Copy struct
    printf("3. Copy struct:\n");
    struct {
        int x;
        int y;
    } p1 = {10, 20}, p2;
    my_memcpy(&p2, &p1, sizeof(p1));
    printf("   p1: (%d, %d)\n", p1.x, p1.y);
    printf("   p2: (%d, %d)\n\n", p2.x, p2.y);

    // Overlap warning
    printf("4. DANGER - Overlapping memory:\n");
    char overlap[] = "ABCDEF";
    printf("   Before: \"%s\"\n", overlap);
    printf("   memcpy with overlap -> UNDEFINED BEHAVIOR!\n");
    printf("   Use memmove() instead for overlapping regions.\n\n");

    printf("=== Key Points ===\n");
    printf("- Copies n bytes from src to dest\n");
    printf("- Does NOT check for overlap (undefined behavior)\n");
    printf("- Returns dest pointer\n");
    printf("- Use memmove() for overlapping memory\n");
    printf("- Use sizeof() to get correct byte count\n");

    return 0;
}
