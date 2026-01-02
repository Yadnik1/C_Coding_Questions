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

void* my_memset(void* ptr, int value, size_t n) {
    if (ptr == NULL) return NULL;

    unsigned char* p = (unsigned char*)ptr;
    unsigned char v = (unsigned char)value;

    while (n > 0) {
        *p++ = v;
        n--;
    }

    return ptr;
}

// Optimized version (word-at-a-time for large blocks)
void* my_memset_fast(void* ptr, int value, size_t n) {
    if (ptr == NULL) return NULL;

    unsigned char* p = (unsigned char*)ptr;
    unsigned char v = (unsigned char)value;

    // Fill byte-by-byte for small sizes or alignment
    while (n > 0 && ((uintptr_t)p & 7)) {  // Align to 8 bytes
        *p++ = v;
        n--;
    }

    // Fill 8 bytes at a time
    if (n >= 8) {
        uint64_t pattern = v;
        pattern |= pattern << 8;
        pattern |= pattern << 16;
        pattern |= pattern << 32;

        uint64_t* p64 = (uint64_t*)p;
        while (n >= 8) {
            *p64++ = pattern;
            n -= 8;
        }
        p = (unsigned char*)p64;
    }

    // Fill remaining bytes
    while (n > 0) {
        *p++ = v;
        n--;
    }

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
