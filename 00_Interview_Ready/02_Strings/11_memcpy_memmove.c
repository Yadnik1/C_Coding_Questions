/*
 * ============================================================================
 * PROBLEM: Implement memcpy() and memmove() - Memory Copy Functions
 * ============================================================================
 *
 * WHAT ARE THESE FUNCTIONS?
 *
 * memcpy(dest, src, n):
 *   Copies 'n' bytes from 'src' to 'dest'.
 *   DOES NOT handle overlapping memory regions!
 *   Use when: you're 100% sure src and dest don't overlap.
 *
 * memmove(dest, src, n):
 *   Copies 'n' bytes from 'src' to 'dest'.
 *   SAFELY handles overlapping memory regions!
 *   Use when: regions might overlap, or you're not sure.
 *
 * FUNCTION SIGNATURES:
 * void *memcpy(void *dest, const void *src, size_t n);
 * void *memmove(void *dest, const void *src, size_t n);
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - ESSENTIAL for embedded systems (buffer handling, DMA, protocols)
 * - Tests understanding of pointer arithmetic and void* casting
 * - Tests understanding of overlapping memory regions
 * - Shows awareness of low-level memory operations
 *
 * WHAT IS MEMORY OVERLAP?
 * When src and dest point to regions that share some bytes.
 *
 * EXAMPLE OF OVERLAP (the tricky case):
 *   Memory: [1][2][3][4][5][0][0]
 *            ^src          ^dest (at offset 2)
 *
 *   If we copy bytes 0-4 to bytes 2-6:
 *     - Forward copy would overwrite src[2] BEFORE we read it!
 *     - We need to copy BACKWARDS to avoid this.
 *
 * VISUAL - WHY FORWARD COPY FAILS WITH OVERLAP:
 *   Initial:     [1][2][3][4][5][0][0]
 *   Copy 1->pos2: [1][2][1][4][5][0][0]  (OK so far)
 *   Copy 2->pos3: [1][2][1][2][5][0][0]  (OK so far)
 *   Copy 1->pos4: [1][2][1][2][1][0][0]  (WRONG! We wanted 3 here!)
 *   The source data was overwritten before we could copy it!
 *
 * VISUAL - BACKWARD COPY WORKS:
 *   Start from end, copy byte 5 first, then 4, 3, 2, 1
 *   Result: [1][2][1][2][3][4][5] (CORRECT!)
 *
 * WHEN TO USE BACKWARD VS FORWARD:
 * - If dest > src AND overlap: copy BACKWARD
 * - Otherwise: copy FORWARD (safe)
 *
 * ============================================================================
 */

// Time: O(n), Space: O(1)

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

// Say: "memcpy copies n bytes from src to dest - NO overlap handling"
void *my_memcpy(void *dest, const void *src, size_t n) {
    // Say: "Cast to byte pointers for byte-by-byte copy"
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    // Say: "Simple forward copy - assumes no overlap"
    while (n--) {
        *d++ = *s++;  // Say: "Copy byte and advance both pointers"
    }

    return dest;  // Say: "Return dest for chaining"
}

// Say: "memmove handles overlapping regions safely"
void *my_memmove(void *dest, const void *src, size_t n) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    // Say: "Check if regions overlap and dest > src"
    if (d > s && d < s + n) {
        // Say: "Overlap case: copy backwards to avoid overwriting source"
        d += n;  // Say: "Start from end of dest"
        s += n;  // Say: "Start from end of src"
        while (n--) {
            *--d = *--s;  // Say: "Copy backwards"
        }
    } else {
        // Say: "No overlap or dest < src: forward copy is safe"
        while (n--) {
            *d++ = *s++;
        }
    }

    return dest;
}

// Say: "Optimized memcpy using word-sized copies"
void *my_memcpy_optimized(void *dest, const void *src, size_t n) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    // Say: "Copy bytes until dest is word-aligned"
    while (n && ((uintptr_t)d & (sizeof(size_t) - 1))) {
        *d++ = *s++;
        n--;
    }

    // Say: "Copy word-sized chunks for speed"
    size_t *dw = (size_t *)d;
    const size_t *sw = (const size_t *)s;
    while (n >= sizeof(size_t)) {
        *dw++ = *sw++;
        n -= sizeof(size_t);
    }

    // Say: "Copy remaining bytes"
    d = (uint8_t *)dw;
    s = (const uint8_t *)sw;
    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

void print_buffer(const char *label, uint8_t *buf, size_t len) {
    printf("%s: [", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02X", buf[i]);
        if (i < len - 1) printf(" ");
    }
    printf("]\n");
}

int main() {
    printf("=== memcpy Demo ===\n");
    uint8_t src[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t dest[5] = {0};

    print_buffer("Source", src, 5);
    print_buffer("Dest before", dest, 5);

    my_memcpy(dest, src, 5);
    print_buffer("Dest after memcpy", dest, 5);

    printf("\n=== memmove with Overlap ===\n");
    uint8_t overlap[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00};

    print_buffer("Before", overlap, 7);
    printf("Moving overlap[0:5] to overlap[2:7]\n");

    my_memmove(&overlap[2], &overlap[0], 5);
    print_buffer("After memmove", overlap, 7);

    printf("\n=== Why memmove matters ===\n");
    uint8_t test1[] = {1, 2, 3, 4, 5, 0, 0};
    uint8_t test2[] = {1, 2, 3, 4, 5, 0, 0};

    printf("Copying [0:5] to [2:7] with overlap:\n");
    // Simulating what memcpy would do (wrong for overlap)
    printf("memcpy (unsafe): would give wrong result with overlap\n");

    my_memmove(&test2[2], &test2[0], 5);
    print_buffer("memmove (safe)", test2, 7);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"memcpy and memmove copy n bytes between memory regions.

 memcpy:
 - Simple byte-by-byte copy
 - DOES NOT handle overlapping regions
 - Undefined behavior if src and dest overlap
 - Faster (no overlap check)

 memmove:
 - Handles overlapping regions safely
 - If dest > src and overlap: copy backwards
 - If dest < src or no overlap: copy forwards
 - Slightly slower due to overlap check

 WHY OVERLAP MATTERS:
 Copying [1,2,3,4,5] from index 0 to index 2:

 Forward copy (memcpy, WRONG):
 [1,2,3,4,5,0,0] -> copy 1 to pos 2 -> [1,2,1,4,5,0,0]
                 -> copy 2 to pos 3 -> [1,2,1,2,5,0,0]
                 -> copy 1 to pos 4 -> [1,2,1,2,1,0,0]  WRONG!

 Backward copy (memmove, CORRECT):
 Start from end, copy 5,4,3,2,1 -> [1,2,1,2,3,4,5]  CORRECT!

 OPTIMIZATIONS:
 - Word-aligned copies (4 or 8 bytes at a time)
 - Use DMA for large transfers in embedded
 - Compiler intrinsics (__builtin_memcpy)

 EMBEDDED USES:
 - Copying data between buffers
 - Shifting data in circular buffers
 - Protocol packet assembly
 - DMA buffer handling

 COMMON MISTAKES:
 - Using memcpy when regions might overlap
 - Not checking for NULL pointers
 - Wrong byte count (forgetting sizeof)
 - Assuming alignment"
*/
