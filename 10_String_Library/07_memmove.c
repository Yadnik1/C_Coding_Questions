/*
 * ============================================================================
 * PROBLEM: Implement memmove()
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: HIGH
 *
 * memmove copies n bytes, correctly handling overlapping memory regions
 *
 * ============================================================================
 * WHY memmove IS NEEDED:
 * ============================================================================
 *
 * The problem with memcpy on overlapping memory:
 *
 *   buf: [A][B][C][D][E][F]
 *   Goal: memmove(buf+2, buf, 4)  → Copy ABCD to position 2
 *   Expected: [A][B][A][B][C][D]
 *
 *   With memcpy (WRONG - copies forward):
 *   Step 1: buf[2] = buf[0] → [A][B][A][D][E][F]
 *   Step 2: buf[3] = buf[1] → [A][B][A][B][E][F]
 *   Step 3: buf[4] = buf[2] → [A][B][A][B][A][F]  ← WRONG! Should be C
 *   Step 4: buf[5] = buf[3] → [A][B][A][B][A][B]  ← WRONG! Should be D
 *
 *   The source data gets overwritten before being copied!
 *
 * ============================================================================
 * SOLUTION: Copy backwards when dest > src
 * ============================================================================
 *
 *   If dest > src and overlapping: copy from end to beginning
 *   If dest < src or no overlap: copy from beginning to end
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

void* my_memmove(void* dest, const void* src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return dest;

    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    if (d < s) {
        // No overlap risk OR dest before src
        // Copy forward (beginning to end)
        while (n > 0) {
            *d++ = *s++;
            n--;
        }
    } else if (d > s) {
        // Potential overlap with dest after src
        // Copy backward (end to beginning)
        d += n;
        s += n;
        while (n > 0) {
            *--d = *--s;
            n--;
        }
    }
    // If d == s, nothing to do

    return dest;
}

// Helper to print array
void printArray(const char* label, char* arr, int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) {
        printf("%c", arr[i]);
        if (i < n - 1) printf("][");
    }
    printf("]\n");
}

int main() {
    printf("=== memmove Implementation ===\n\n");

    // Non-overlapping (same as memcpy)
    printf("1. Non-overlapping copy:\n");
    char src[] = "ABCDE";
    char dest[6];
    my_memmove(dest, src, 6);
    printf("   src:  \"%s\"\n", src);
    printf("   dest: \"%s\"\n\n", dest);

    // Overlapping: dest > src (copy backward)
    printf("2. Overlapping: dest > src (need backward copy):\n");
    char buf1[] = "ABCDEF";
    printf("   Before: "); printArray("buf", buf1, 6);
    printf("   Goal:   memmove(buf+2, buf, 4) → copy ABCD to position 2\n");
    my_memmove(buf1 + 2, buf1, 4);
    printf("   After:  "); printArray("buf", buf1, 6);
    printf("   Expected: [A][B][A][B][C][D] ✓\n\n");

    // Overlapping: dest < src (copy forward)
    printf("3. Overlapping: dest < src (forward copy is safe):\n");
    char buf2[] = "ABCDEF";
    printf("   Before: "); printArray("buf", buf2, 6);
    printf("   Goal:   memmove(buf, buf+2, 4) → copy CDEF to position 0\n");
    my_memmove(buf2, buf2 + 2, 4);
    printf("   After:  "); printArray("buf", buf2, 6);
    printf("   (Last 2 positions unchanged)\n\n");

    printf("=== Key Points ===\n");
    printf("- Handles overlapping memory correctly\n");
    printf("- If dest > src: copy backward\n");
    printf("- If dest < src: copy forward\n");
    printf("- Slightly slower than memcpy due to check\n");
    printf("- When in doubt, use memmove!\n");

    return 0;
}
