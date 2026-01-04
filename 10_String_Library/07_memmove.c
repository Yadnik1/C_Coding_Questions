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

// memmove - Copy n bytes, handling overlap correctly
// Say: "I'll implement memmove which handles overlapping memory by choosing copy direction"
void* my_memmove(void* dest, const void* src, size_t n) {
    // Handle edge cases: NULL pointers or zero bytes
    // Say: "First, I check for NULL pointers or zero bytes to copy"
    if (dest == NULL || src == NULL || n == 0) return dest;

    // Cast to byte pointers for byte-level access
    // Say: "I cast both pointers to unsigned char for byte operations"
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    // Check if dest is before src in memory
    // Say: "If dest is before src, I can safely copy forward"
    if (d < s) {
        // No overlap risk OR dest before src
        // Copy forward (beginning to end) is safe
        // Say: "I copy from the beginning to the end"
        while (n > 0) {
            *d++ = *s++;    // Copy byte and advance
            n--;            // Decrement count
        }
    }
    // Check if dest is after src in memory
    // Say: "If dest is after src, there might be overlap, so I copy backward"
    else if (d > s) {
        // Potential overlap with dest after src
        // Copy backward (end to beginning) to avoid overwriting source
        // Say: "I start from the end and copy backward to avoid overwriting source data"

        // Move pointers to end of regions
        d += n;     // Point past the end
        s += n;     // Point past the end

        // Copy from end to beginning
        while (n > 0) {
            *--d = *--s;    // Pre-decrement then copy
            n--;            // Decrement count
        }
    }
    // If d == s, nothing to do (source and dest are the same)
    // Say: "If pointers are equal, no copy is needed"

    // Return the original destination pointer
    // Say: "Return the original dest pointer"
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
