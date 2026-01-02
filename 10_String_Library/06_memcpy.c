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

void* my_memcpy(void* dest, const void* src, size_t n) {
    if (dest == NULL || src == NULL) return NULL;

    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    while (n > 0) {
        *d++ = *s++;
        n--;
    }

    return dest;
}

// Optimized version (copy words when aligned)
void* my_memcpy_fast(void* dest, const void* src, size_t n) {
    if (dest == NULL || src == NULL) return NULL;

    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    // Copy bytes until aligned
    while (n > 0 && ((uintptr_t)d & 7)) {
        *d++ = *s++;
        n--;
    }

    // Copy 8 bytes at a time
    if (((uintptr_t)s & 7) == 0) {  // src also aligned
        uint64_t* d64 = (uint64_t*)d;
        const uint64_t* s64 = (const uint64_t*)s;

        while (n >= 8) {
            *d64++ = *s64++;
            n -= 8;
        }

        d = (unsigned char*)d64;
        s = (const unsigned char*)s64;
    }

    // Copy remaining bytes
    while (n > 0) {
        *d++ = *s++;
        n--;
    }

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
