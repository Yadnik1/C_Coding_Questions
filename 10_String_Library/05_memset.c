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
 * VISUALIZATION:
 * ============================================================================
 *
 *   Before: [?][?][?][?][?]
 *
 *   memset(buf, 0, 5):
 *   After:  [0][0][0][0][0]
 *
 *   memset(buf, 'A', 5):
 *   After:  [A][A][A][A][A]
 *
 * ============================================================================
 * COMMON TRAP:
 * ============================================================================
 *
 *   memset sets each BYTE, not each int!
 *
 *   int arr[4];
 *   memset(arr, 1, sizeof(arr));
 *
 *   You might expect: [1, 1, 1, 1]
 *   Actually get:     [0x01010101, 0x01010101, ...]
 *                     = [16843009, 16843009, ...]
 *
 *   Safe values for int arrays:
 *   - 0   → 0x00000000 (works!)
 *   - -1  → 0xFFFFFFFF (works!)
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

// memset - Fill memory with a byte value
// Say: "I'll implement memset by filling each byte of memory with the specified value"
void* my_memset(void* ptr, int value, size_t n) {
    // Check for NULL pointer
    // Say: "First, I check if the pointer is NULL"
    if (ptr == NULL) return NULL;

    // Cast to unsigned char pointer to work with individual bytes
    // Say: "I cast the void pointer to unsigned char pointer so I can write byte by byte"
    unsigned char* p = (unsigned char*)ptr;

    // Convert value to unsigned char (only lowest byte is used)
    // Say: "I cast the value to unsigned char because memset works on bytes"
    unsigned char v = (unsigned char)value;

    // Fill n bytes with the value
    // Say: "Now I write the byte value to each of the n bytes in memory"
    while (n > 0) {
        *p++ = v;   // Write value and advance pointer
        n--;        // Decrement remaining count
    }

    // Return the original pointer
    // Say: "Finally, I return the original pointer"
    return ptr;
}

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
    while (n > 0 && ((uintptr_t)p & 7)) {  // Check if lowest 3 bits are non-zero
        *p++ = v;   // Write one byte
        n--;        // Decrement count
    }

    // Fill 8 bytes at a time for better performance
    // Say: "Now that we're aligned, I can fill 8 bytes at a time"
    if (n >= 8) {
        // Create a 64-bit pattern by repeating the byte value
        // Say: "I create a 64-bit value with the byte repeated 8 times"
        uint64_t pattern = v;
        pattern |= pattern << 8;    // Repeat to 2 bytes
        pattern |= pattern << 16;   // Repeat to 4 bytes
        pattern |= pattern << 32;   // Repeat to 8 bytes

        // Cast to 64-bit pointer for word-sized writes
        uint64_t* p64 = (uint64_t*)p;

        // Write 8 bytes at a time
        // Say: "I write 64 bits at a time while we have at least 8 bytes left"
        while (n >= 8) {
            *p64++ = pattern;   // Write 8 bytes
            n -= 8;             // Decrement by 8
        }

        // Update byte pointer to where we left off
        p = (unsigned char*)p64;
    }

    // Fill remaining bytes (less than 8)
    // Say: "Finally, I fill any remaining bytes one at a time"
    while (n > 0) {
        *p++ = v;   // Write remaining bytes
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
    printf("   memset(buf, 'A', 5) → \"%s\"\n\n", buf1);

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
    printf("   (Each byte is 0x01, so each int is 0x01010101)\n\n");

    // -1 works correctly
    printf("4. Using -1 (0xFF):\n");
    int arr3[4];
    my_memset(arr3, -1, sizeof(arr3));
    printf("   memset(arr, -1, sizeof(arr)) → [%d, %d, %d, %d]\n\n",
           arr3[0], arr3[1], arr3[2], arr3[3]);

    printf("=== Key Points ===\n");
    printf("- Sets each BYTE to value (cast to unsigned char)\n");
    printf("- For int arrays, only 0 and -1 work as expected\n");
    printf("- Common use: memset(buf, 0, sizeof(buf))\n");

    return 0;
}
