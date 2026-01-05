/*
 * ============================================================================
 * PROBLEM: Implement memset()
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: VERY HIGH
 *
 * memset fills memory with a byte value
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. VOID POINTER:
 *    - void* can point to ANY type of data
 *    - Must cast to unsigned char* for byte operations
 *    - Why unsigned char? Values 0-255, no sign issues
 *
 * 2. BYTE-LEVEL OPERATION:
 *    - memset sets each BYTE, not each int/float/etc
 *    - value is cast to unsigned char (only lowest 8 bits used)
 *
 * 3. COMMON USE CASES:
 *    - Zero out memory: memset(buf, 0, sizeof(buf))
 *    - Initialize char arrays: memset(str, 'A', 10)
 *    - Clear structs: memset(&myStruct, 0, sizeof(myStruct))
 *
 * ============================================================================
 * VISUALIZATION: memset(buf, 'X', 5)
 * ============================================================================
 *
 *   Before: [?][?][?][?][?]
 *
 *   Step 1: buf[0] = 'X'  →  ['X'][?][?][?][?]
 *   Step 2: buf[1] = 'X'  →  ['X']['X'][?][?][?]
 *   Step 3: buf[2] = 'X'  →  ['X']['X']['X'][?][?]
 *   Step 4: buf[3] = 'X'  →  ['X']['X']['X']['X'][?]
 *   Step 5: buf[4] = 'X'  →  ['X']['X']['X']['X']['X']
 *
 *   After:  ['X']['X']['X']['X']['X']
 *
 * ============================================================================
 * CRITICAL TRAP: memset on int arrays
 * ============================================================================
 *
 *   int arr[4];
 *   memset(arr, 1, sizeof(arr));
 *
 *   EXPECTED: [1, 1, 1, 1]
 *
 *   ACTUAL:
 *   Each byte is set to 0x01
 *   Each int (4 bytes) = 0x01010101 = 16843009
 *   Result: [16843009, 16843009, 16843009, 16843009]
 *
 *   SAFE VALUES for int arrays:
 *   - 0:  all bytes 0x00 → int = 0 ✓
 *   - -1: all bytes 0xFF → int = -1 (two's complement) ✓
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   cast ptr to unsigned char*
 *   cast value to unsigned char
 *   while (n > 0):
 *       *p++ = value
 *       n--
 *   return original ptr
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Must write each byte once
 * - n = number of bytes to set
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
 * Q1: "Why can't you use memset to initialize an int array to 1?"
 * A1: memset fills each BYTE, not each int! Setting bytes to 0x01 creates
 *     0x01010101 = 16843009 for each int. Only 0 (all bytes 0x00) and -1
 *     (all bytes 0xFF) work correctly for int arrays.
 *
 * -------------------------------------------------------------------------
 * Q2: "How do optimized memset implementations work?"
 * A2: They use word-aligned writes (8 bytes at a time on 64-bit) for the
 *     middle portion. First handle unaligned prefix bytes, then write 64-bit
 *     words with the byte pattern repeated 8 times, then handle suffix.
 *     Can also use SIMD (SSE/AVX) for even larger chunks.
 *
 * -------------------------------------------------------------------------
 * Q3: "What's volatile and when would you use it with memset?"
 * A3: Use memset_s or explicit_bzero for security-sensitive clearing (passwords).
 *     Regular memset can be optimized away by compilers if buffer isn't used
 *     afterward. volatile prevents this: *(volatile char*)p = 0;
 *
 * -------------------------------------------------------------------------
 * Q4: "What's the difference between memset and calloc?"
 * A4: calloc allocates AND zeros memory atomically. memset only fills existing
 *     memory. calloc may use OS-provided pre-zeroed pages (faster for large
 *     allocations). Use calloc for new zeroed allocations, memset for existing.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

/*
 * ============================================================================
 * MEMSET FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void* my_memset(void* ptr, int value, size_t n):
 *   - Returns "void*" = pointer to start of memory (for chaining)
 *   - "void* ptr" = pointer to memory to fill (generic pointer)
 *   - "int value" = value to set (cast to unsigned char)
 *   - "size_t n" = number of bytes to set
 *
 * unsigned char* p = (unsigned char*)ptr:
 *   - Cast void* to unsigned char* for byte access
 *   - WHY void*? Works with ANY data type
 *   - WHY unsigned char? Byte values 0-255, no sign issues
 *
 * unsigned char v = (unsigned char)value:
 *   - Only use lowest 8 bits of value
 *   - memset(buf, 0x12345678, n) uses only 0x78
 *
 * while (n > 0) *p++ = v; n--;
 *   - Write value to current byte
 *   - Advance pointer
 *   - Decrement count
 *
 * return ptr:
 *   - Return original pointer (not advanced pointer!)
 *   - Enables chaining
 *
 * ============================================================================
 */
// memset - Fill memory with a byte value
// Say: "I'll implement memset by filling each byte of memory with the specified value"
void* my_memset(void* ptr, int value, size_t n) {
    // Check for NULL pointer
    // Say: "First, I check if the pointer is NULL"
    // WHY: Dereferencing NULL causes crash
    if (ptr == NULL) return NULL;

    // Cast to unsigned char pointer to work with individual bytes
    // Say: "I cast the void pointer to unsigned char pointer so I can write byte by byte"
    // WHY: void* cannot be dereferenced directly
    // WHY unsigned char? Byte values 0-255 with no sign issues
    unsigned char* p = (unsigned char*)ptr;

    // Convert value to unsigned char (only lowest byte is used)
    // Say: "I cast the value to unsigned char because memset works on bytes"
    // WHY: If value is 0x12345678, only 0x78 is used
    unsigned char v = (unsigned char)value;

    // Fill n bytes with the value
    // Say: "Now I write the byte value to each of the n bytes in memory"
    // WHY: Simple loop - write and advance
    while (n > 0) {
        // Write value and advance pointer
        // Say: "I write the value to the current byte and move to the next"
        *p++ = v;
        // Decrement remaining count
        // Say: "I decrement the count of remaining bytes"
        n--;
    }

    // Return the original pointer
    // Say: "Finally, I return the original pointer"
    // WHY: Enables chaining like printf after memset
    return ptr;
}

/*
 * ============================================================================
 * OPTIMIZED MEMSET - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Key insight: Writing 8 bytes at once is faster than 1 byte at a time
 *
 * Steps:
 * 1. Fill unaligned prefix bytes (0-7 bytes)
 * 2. Fill aligned middle using 64-bit writes
 * 3. Fill unaligned suffix bytes (0-7 bytes)
 *
 * Pattern creation:
 *   v = 0x41 ('A')
 *   pattern = 0x41
 *   pattern |= pattern << 8   → 0x4141
 *   pattern |= pattern << 16  → 0x41414141
 *   pattern |= pattern << 32  → 0x4141414141414141
 *
 * ============================================================================
 */
// Optimized version (word-at-a-time for large blocks)
// Say: "Here's an optimized version that fills 8 bytes at a time for better performance"
void* my_memset_fast(void* ptr, int value, size_t n) {
    // Check for NULL pointer
    // Say: "Check for NULL first"
    if (ptr == NULL) return NULL;

    // Set up byte pointer and value
    unsigned char* p = (unsigned char*)ptr;
    unsigned char v = (unsigned char)value;

    // Fill byte-by-byte until we reach 8-byte alignment
    // Say: "First, I fill bytes one at a time until the pointer is 8-byte aligned"
    // WHY: Unaligned 64-bit writes can be slow or crash on some architectures
    while (n > 0 && ((uintptr_t)p & 7)) {  // Check if lowest 3 bits are non-zero
        // Write one byte
        // Say: "I fill one byte at a time until aligned"
        *p++ = v;
        n--;
    }

    // Fill 8 bytes at a time for better performance
    // Say: "Now that we're aligned, I can fill 8 bytes at a time"
    if (n >= 8) {
        // Create a 64-bit pattern by repeating the byte value
        // Say: "I create a 64-bit value with the byte repeated 8 times"
        // WHY: One 64-bit write is faster than eight 8-bit writes
        uint64_t pattern = v;
        pattern |= pattern << 8;    // Repeat to 2 bytes: 0x4141
        pattern |= pattern << 16;   // Repeat to 4 bytes: 0x41414141
        pattern |= pattern << 32;   // Repeat to 8 bytes: 0x4141414141414141

        // Cast to 64-bit pointer for word-sized writes
        uint64_t* p64 = (uint64_t*)p;

        // Write 8 bytes at a time
        // Say: "I write 64 bits at a time while we have at least 8 bytes left"
        while (n >= 8) {
            // Write 8 bytes
            // Say: "I write 8 bytes in one operation"
            *p64++ = pattern;
            n -= 8;
        }

        // Update byte pointer to where we left off
        p = (unsigned char*)p64;
    }

    // Fill remaining bytes (less than 8)
    // Say: "Finally, I fill any remaining bytes one at a time"
    while (n > 0) {
        // Write remaining bytes
        // Say: "I fill the remaining bytes"
        *p++ = v;
        n--;
    }

    // Return the original pointer
    return ptr;
}

int main() {
    printf("=== memset Implementation ===\n\n");

    // Basic usage
    printf("1. Basic memset:\n");
    char buf1[6];
    my_memset(buf1, 'A', 5);
    buf1[5] = '\0';
    printf("   memset(buf, 'A', 5) -> \"%s\"\n\n", buf1);

    // Zero-fill
    printf("2. Zero-fill:\n");
    int arr[4] = {1, 2, 3, 4};
    printf("   Before: [%d, %d, %d, %d]\n", arr[0], arr[1], arr[2], arr[3]);
    my_memset(arr, 0, sizeof(arr));
    printf("   After memset(arr, 0, sizeof(arr)): [%d, %d, %d, %d]\n\n",
           arr[0], arr[1], arr[2], arr[3]);

    // Common trap demonstration
    printf("3. Common TRAP with integers:\n");
    int arr2[4];
    my_memset(arr2, 1, sizeof(arr2));
    printf("   memset(arr, 1, sizeof(arr))\n");
    printf("   Expected: [1, 1, 1, 1]\n");
    printf("   Actual:   [%d, %d, %d, %d]\n", arr2[0], arr2[1], arr2[2], arr2[3]);
    printf("   (Each byte is 0x01, so each int is 0x01010101 = 16843009)\n\n");

    // -1 works correctly
    printf("4. Using -1 (0xFF):\n");
    int arr3[4];
    my_memset(arr3, -1, sizeof(arr3));
    printf("   memset(arr, -1, sizeof(arr)) -> [%d, %d, %d, %d]\n\n",
           arr3[0], arr3[1], arr3[2], arr3[3]);

    printf("=== Key Points ===\n");
    printf("- Sets each BYTE to value (cast to unsigned char)\n");
    printf("- For int arrays, only 0 and -1 work as expected\n");
    printf("- Common use: memset(buf, 0, sizeof(buf))\n");
    printf("- Use void* for generic memory operations\n");
    printf("- Optimized versions write multiple bytes at once\n");

    return 0;
}
