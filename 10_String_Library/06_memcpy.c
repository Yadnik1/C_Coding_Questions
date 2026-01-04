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
 * VISUALIZATION:
 * ============================================================================
 *
 *   src:  [A][B][C][D][E]
 *   dest: [?][?][?][?][?]
 *
 *   memcpy(dest, src, 5):
 *   dest: [A][B][C][D][E]
 *
 * ============================================================================
 * WARNING: Undefined behavior if src and dest overlap!
 *          Use memmove() for overlapping regions.
 * ============================================================================
 *
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

// memcpy - Copy n bytes from src to dest
// Say: "I'll implement memcpy by copying bytes one at a time from source to destination"
void* my_memcpy(void* dest, const void* src, size_t n) {
    // Validate input pointers
    // Say: "First, I check that both pointers are valid"
    if (dest == NULL || src == NULL) return NULL;

    // Cast void pointers to unsigned char pointers for byte-level access
    // Say: "I cast both pointers to unsigned char so I can copy byte by byte"
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    // Copy n bytes from source to destination
    // Say: "Now I copy each byte from source to destination"
    while (n > 0) {
        *d++ = *s++;    // Copy byte and advance both pointers
        n--;            // Decrement remaining count
    }

    // Return the original destination pointer
    // Say: "Return the original dest pointer"
    return dest;
}

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
    while (n > 0 && ((uintptr_t)d & 7)) {   // Check alignment
        *d++ = *s++;    // Copy one byte
        n--;            // Decrement count
    }

    // If src is also aligned, we can copy 8 bytes at a time
    // Say: "If source is also aligned, I can copy 8 bytes at a time for better performance"
    if (((uintptr_t)s & 7) == 0) {  // src also aligned
        // Cast to 64-bit pointers
        uint64_t* d64 = (uint64_t*)d;
        const uint64_t* s64 = (const uint64_t*)s;

        // Copy 8 bytes at a time
        // Say: "Copy 64 bits at a time while we have at least 8 bytes left"
        while (n >= 8) {
            *d64++ = *s64++;    // Copy 8 bytes
            n -= 8;             // Decrement by 8
        }

        // Update byte pointers to where we left off
        d = (unsigned char*)d64;
        s = (const unsigned char*)s64;
    }

    // Copy remaining bytes (less than 8)
    // Say: "Finally, I copy any remaining bytes one at a time"
    while (n > 0) {
        *d++ = *s++;    // Copy remaining bytes
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
    // This is UNDEFINED BEHAVIOR:
    // my_memcpy(overlap + 2, overlap, 4);
    printf("   memcpy with overlap → UNDEFINED BEHAVIOR!\n");
    printf("   Use memmove() instead for overlapping regions.\n\n");

    printf("=== Key Points ===\n");
    printf("- Copies n bytes from src to dest\n");
    printf("- Does NOT check for overlap (undefined behavior)\n");
    printf("- Returns dest pointer\n");
    printf("- Use memmove() for overlapping memory\n");

    return 0;
}
