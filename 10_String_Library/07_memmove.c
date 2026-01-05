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
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. THE OVERLAP PROBLEM:
 *    - memcpy assumes src and dest DON'T overlap
 *    - memmove handles overlap correctly by choosing copy direction
 *    - If dest > src: must copy backward (end to start)
 *    - If dest < src: can copy forward (start to end)
 *
 * 2. COPY DIRECTION:
 *    - Forward copy: d[0]=s[0], d[1]=s[1], d[2]=s[2]...
 *    - Backward copy: d[n-1]=s[n-1], d[n-2]=s[n-2]...
 *    - Direction choice prevents overwriting source before reading
 *
 * 3. POINTER ARITHMETIC:
 *    - d += n moves pointer to position n (past end)
 *    - *--d decrements pointer THEN dereferences
 *    - *d++ dereferences THEN increments pointer
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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "When should you use memmove vs memcpy?"
 * A1: Use memmove when source and destination MIGHT overlap (even if unsure).
 *     Use memcpy only when you're CERTAIN there's no overlap. When in doubt,
 *     use memmove - it's safer. Performance difference is minimal.
 *
 * -------------------------------------------------------------------------
 * Q2: "Why doesn't memmove just use a temporary buffer?"
 * A2: That would require O(n) extra space and another copy operation.
 *     By choosing copy direction intelligently (forward if dest < src,
 *     backward if dest > src), we handle overlap in O(n) time, O(1) space.
 *
 * -------------------------------------------------------------------------
 * Q3: "How do you detect if memory regions overlap?"
 * A3: Regions [a, a+n) and [b, b+m) overlap if: a < b+m AND b < a+n.
 *     But memmove doesn't need to detect overlap - it just copies in the
 *     safe direction based on pointer comparison. If d < s, forward is safe.
 *
 * -------------------------------------------------------------------------
 * Q4: "What real-world operations need memmove?"
 * A4: Array element deletion (shift elements left), insertion (shift right),
 *     in-place string manipulation, ring buffer operations, gap buffer in
 *     text editors. Any time you shift data within the same array.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

/*
 * ============================================================================
 * MEMMOVE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void* my_memmove(void* dest, const void* src, size_t n):
 *   - Returns "void*" = pointer to dest (for chaining)
 *   - "void* dest" = destination memory (we modify it)
 *   - "const void* src" = source memory (we read from it)
 *   - "size_t n" = number of bytes to copy
 *
 * unsigned char* d = (unsigned char*)dest:
 *   - Cast void* to unsigned char* for byte access
 *   - WHY void*? Works with ANY data type
 *   - WHY unsigned char? Byte values 0-255, no sign issues
 *
 * if (d < s) - Forward copy:
 *   - dest is before src in memory
 *   - Safe to copy start→end, won't overwrite src
 *
 * else if (d > s) - Backward copy:
 *   - dest overlaps with src (dest after src)
 *   - Must copy end→start to avoid corruption
 *   - d += n; s += n; moves pointers past end
 *   - *--d = *--s; pre-decrement then copy
 *
 * return dest:
 *   - Return original dest pointer (not advanced!)
 *
 * ============================================================================
 */
// memmove - Copy n bytes, handling overlap correctly
// Say: "I'll implement memmove which handles overlapping memory by choosing copy direction"
void* my_memmove(void* dest, const void* src, size_t n) {
    // Handle edge cases: NULL pointers or zero bytes
    // Say: "First, I check for NULL pointers or zero bytes to copy"
    // WHY: Dereferencing NULL causes crash, zero bytes means nothing to do
    if (dest == NULL || src == NULL || n == 0) return dest;

    // Cast to byte pointers for byte-level access
    // Say: "I cast both pointers to unsigned char for byte operations"
    // WHY: void* cannot be dereferenced, need byte-sized type
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    // Check if dest is before src in memory
    // Say: "If dest is before src, I can safely copy forward"
    // WHY: Forward copy won't overwrite unread source bytes
    if (d < s) {
        // No overlap risk OR dest before src
        // Copy forward (beginning to end) is safe
        // Say: "I copy from the beginning to the end"
        while (n > 0) {
            // Copy byte and advance both pointers
            // Say: "I copy one byte and advance both pointers"
            *d++ = *s++;
            // Decrement remaining count
            // Say: "I decrement the count of remaining bytes"
            n--;
        }
    }
    // Check if dest is after src in memory
    // Say: "If dest is after src, there might be overlap, so I copy backward"
    // WHY: Backward copy prevents overwriting source before reading
    else if (d > s) {
        // Potential overlap with dest after src
        // Copy backward (end to beginning) to avoid overwriting source
        // Say: "I start from the end and copy backward to avoid overwriting source data"

        // Move pointers to end of regions (past last byte)
        // Say: "I move both pointers to past the end of their regions"
        // WHY: Pre-decrement (*--p) needs to start past the end
        d += n;
        s += n;

        // Copy from end to beginning
        // Say: "I copy from end to beginning using pre-decrement"
        while (n > 0) {
            // Pre-decrement then copy: moves pointer first, then copies
            // Say: "I decrement the pointer first, then copy the byte"
            // WHY: This copies d[n-1]=s[n-1], d[n-2]=s[n-2], etc.
            *--d = *--s;
            // Decrement remaining count
            n--;
        }
    }
    // If d == s, nothing to do (source and dest are the same)
    // Say: "If pointers are equal, no copy is needed"
    // WHY: Copying memory to itself is a no-op

    // Return the original destination pointer
    // Say: "Return the original dest pointer"
    // WHY: Enables chaining, returns where data was copied to
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
