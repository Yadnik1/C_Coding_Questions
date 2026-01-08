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

/*
 * ============================================================================
 * WHY IS TYPE CASTING NEEDED IN C? (GENERAL EXPLANATION)
 * ============================================================================
 *
 * WHAT IS TYPE CASTING?
 * Type casting is converting a value from one data type to another.
 *   int x = 5;
 *   float f = (float)x;    // Cast int to float: f = 5.0
 *   char *p = (char *)ptr; // Cast pointer type
 *
 * ============================================================================
 * WHY DO WE NEED CASTING IN C?
 * ============================================================================
 *
 * REASON 1: C IS A STATICALLY TYPED LANGUAGE
 * ------------------------------------------
 * - Every variable has a type known at compile time
 * - The compiler needs to know EXACTLY how to:
 *   - Store the value in memory (how many bytes?)
 *   - Perform operations (integer math vs floating point?)
 *   - Move pointers (how many bytes per increment?)
 *
 * Without type information, the compiler is BLIND:
 *   void *p;   // "I'm a pointer to... something?"
 *   p++;       // ERROR! Move how many bytes? 1? 4? 8? Unknown!
 *
 * REASON 2: POINTER ARITHMETIC REQUIRES TYPE SIZE
 * ------------------------------------------------
 * When you do pointer++, C adds sizeof(pointed_type) to the address:
 *
 *   int *ip = 0x1000;
 *   ip++;           // ip is now 0x1004 (added 4 bytes = sizeof(int))
 *
 *   char *cp = 0x1000;
 *   cp++;           // cp is now 0x1001 (added 1 byte = sizeof(char))
 *
 *   void *vp = 0x1000;
 *   vp++;           // ERROR! sizeof(void) is undefined!
 *
 * VISUAL - Why void* can't do arithmetic:
 *
 *   Memory: [00][01][02][03][04][05][06][07][08]...
 *            ^
 *            vp = 0x1000
 *
 *   vp++ should point to... where?
 *   - If it's bytes: 0x1001
 *   - If it's ints:  0x1004
 *   - If it's longs: 0x1008
 *   Compiler doesn't know, so it REFUSES to compile!
 *
 * REASON 3: DEREFERENCING REQUIRES KNOWING THE SIZE
 * --------------------------------------------------
 * When you do *ptr, C reads sizeof(pointed_type) bytes:
 *
 *   Memory: [0x12][0x34][0x56][0x78]
 *            ^
 *            ptr
 *
 *   If ptr is char*:  *ptr reads 1 byte  = 0x12
 *   If ptr is short*: *ptr reads 2 bytes = 0x3412 (little endian)
 *   If ptr is int*:   *ptr reads 4 bytes = 0x78563412
 *
 *   void *vp;
 *   *vp;      // ERROR! How many bytes should I read???
 *
 * ============================================================================
 * WHEN IS CASTING REQUIRED?
 * ============================================================================
 *
 * 1. VOID POINTER TO TYPED POINTER:
 *    void *ptr = malloc(100);
 *    int *ip = (int *)ptr;      // Must cast to use as int*
 *    char *cp = (char *)ptr;    // Or cast to char* for bytes
 *
 * 2. BETWEEN POINTER TYPES:
 *    int arr[10];
 *    char *bytes = (char *)arr; // View int array as bytes
 *    // Useful for: serialization, network protocols, memory inspection
 *
 * 3. FUNCTION POINTERS:
 *    void (*callback)(void) = (void (*)(void))some_function;
 *
 * 4. INTEGER TO POINTER (embedded systems):
 *    volatile uint32_t *reg = (volatile uint32_t *)0x40021000;
 *    // Access hardware register at known memory address
 *
 * 5. POINTER TO INTEGER (for alignment checks):
 *    uintptr_t addr = (uintptr_t)ptr;
 *    if (addr % 4 == 0) { // Check 4-byte alignment }
 *
 * 6. SIGNED/UNSIGNED CONVERSIONS:
 *    char c = -1;              // Could be 0xFF as signed char
 *    unsigned char uc = (unsigned char)c;  // Now definitely 255
 *
 * ============================================================================
 * IMPLICIT VS EXPLICIT CASTING
 * ============================================================================
 *
 * IMPLICIT (automatic, compiler does it):
 *   int i = 5;
 *   double d = i;    // Compiler implicitly converts int to double
 *   char c = 'A';
 *   int x = c;       // Compiler implicitly converts char to int
 *
 * EXPLICIT (you write the cast):
 *   double d = 3.14;
 *   int i = (int)d;  // You explicitly say "truncate to int"
 *   void *vp = malloc(10);
 *   int *ip = (int *)vp;  // You explicitly cast void* to int*
 *
 * WHEN EXPLICIT IS REQUIRED:
 * - void* to any other pointer type (REQUIRED in C++)
 * - Pointer type to different pointer type
 * - When you want to suppress compiler warnings
 * - When implicit conversion would lose data
 *
 * ============================================================================
 * DANGERS OF INCORRECT CASTING
 * ============================================================================
 *
 * 1. ALIGNMENT VIOLATIONS:
 *    char buffer[10];
 *    int *ip = (int *)&buffer[1];  // DANGER! Not 4-byte aligned!
 *    *ip = 42;  // May crash on ARM/embedded (bus error)
 *
 * 2. TYPE PUNNING ISSUES:
 *    float f = 3.14;
 *    int *ip = (int *)&f;
 *    printf("%d\n", *ip);  // Prints garbage (float bits as int)
 *
 * 3. STRICT ALIASING VIOLATIONS:
 *    // Compiler may optimize incorrectly if you cast and access
 *    // the same memory through different pointer types
 *
 * 4. SIGNED/UNSIGNED SURPRISES:
 *    signed char c = -1;     // Binary: 11111111
 *    int i = c;              // Sign-extended: 0xFFFFFFFF = -1
 *    unsigned char uc = -1;  // Binary: 11111111
 *    int j = uc;             // Zero-extended: 0x000000FF = 255
 *
 * ============================================================================
 * WHY uint8_t IS PREFERRED FOR RAW MEMORY OPERATIONS
 * ============================================================================
 *
 * For memcpy, memmove, and similar byte-by-byte operations:
 *
 * 1. EXACTLY 1 BYTE: uint8_t is guaranteed to be 8 bits (1 byte)
 *    - char is "at least 8 bits" but could theoretically be larger
 *
 * 2. ALWAYS UNSIGNED: uint8_t range is 0-255
 *    - char can be signed (-128 to 127) or unsigned depending on platform
 *    - ARM: char is unsigned by default
 *    - x86: char is signed by default
 *
 * 3. NO SIGN EXTENSION ISSUES:
 *    signed char c = 0xFF;    // Value is -1 (signed)
 *    int i = c;               // Sign-extends to 0xFFFFFFFF (-1)
 *
 *    uint8_t u = 0xFF;        // Value is 255 (unsigned)
 *    int j = u;               // Zero-extends to 0x000000FF (255)
 *
 * 4. CLEAR INTENT: "I'm working with raw bytes"
 *    - uint8_t screams "this is byte-level memory manipulation"
 *    - char might be confused with "character" operations
 *
 * ============================================================================
 * SUMMARY - THE CASTING RULES
 * ============================================================================
 *
 * 1. void* MUST be cast before pointer arithmetic or dereferencing
 * 2. Use uint8_t* (not char*) for raw byte manipulation
 * 3. Be careful with alignment when casting pointer types
 * 4. Watch for signed/unsigned issues, especially with char
 * 5. Use (const type*) for read-only source data
 * 6. Explicit casts document your intent and suppress warnings
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

// ============================================================================
// memcpy - Simple byte-by-byte copy (NO overlap handling)
// ============================================================================
//
// Say: "memcpy copies n bytes from src to dest - NO overlap handling"
//
void *my_memcpy(void *dest, const void *src, size_t n) {

    // ========================================================================
    // WHY DO WE CAST void* TO uint8_t* ???
    // ========================================================================
    //
    // Say: "I cast to uint8_t* because pointer arithmetic requires a known type"
    //
    // THE PROBLEM WITH void*:
    // - void* means "pointer to unknown type"
    // - C does NOT allow pointer arithmetic on void*
    // - You CANNOT do: dest++; or src++;
    // - Why? The compiler doesn't know how many bytes to move!
    //
    // EXAMPLE - Why void* arithmetic fails:
    //   void *p = some_address;
    //   p++;  // ERROR! How many bytes should p move forward?
    //         // 1 byte? 4 bytes? 8 bytes? Compiler doesn't know!
    //
    // HOW POINTER ARITHMETIC WORKS:
    //   int *ip = some_address;    // int is 4 bytes
    //   ip++;                       // moves forward 4 bytes (sizeof(int))
    //
    //   char *cp = some_address;   // char is 1 byte
    //   cp++;                       // moves forward 1 byte (sizeof(char))
    //
    //   uint8_t *bp = some_address; // uint8_t is 1 byte
    //   bp++;                        // moves forward 1 byte (sizeof(uint8_t))
    //
    // ========================================================================

    // ========================================================================
    // WHY uint8_t SPECIFICALLY (not char*)?
    // ========================================================================
    //
    // Say: "uint8_t is exactly 1 byte, unsigned, and preferred for raw memory"
    //
    // memcpy copies BYTE-BY-BYTE, not ints, structs, or words.
    // So we need a pointer type where incrementing moves exactly 1 byte.
    //
    // uint8_t is PERFECT because:
    //   1. It's EXACTLY 1 byte (guaranteed by <stdint.h>)
    //   2. It's UNSIGNED (0 to 255, no negative values)
    //   3. Incrementing it moves forward exactly 1 byte
    //
    // WHY NOT USE char*?
    // You COULD use:
    //   char *d = (char *)dest;
    //   const char *s = (const char *)src;
    //
    // BUT there are issues:
    //   - 'char' may be SIGNED on some platforms (-128 to 127)
    //   - Signed chars can cause sign-extension issues when casting
    //   - uint8_t is EXPLICITLY unsigned (0 to 255)
    //   - In low-level / embedded code, uint8_t is the standard
    //
    // SIGN EXTENSION PROBLEM:
    //   char c = 0xFF;        // Could be -1 if char is signed!
    //   int i = c;            // Sign-extends to 0xFFFFFFFF (-1)
    //
    //   uint8_t u = 0xFF;     // Always 255 (unsigned)
    //   int i = u;            // Zero-extends to 0x000000FF (255)
    //
    // For raw memory operations, we want UNSIGNED bytes!
    //
    // ========================================================================

    uint8_t *d = (uint8_t *)dest;
    //
    // WHAT THIS LINE DOES:
    // 1. Takes 'dest' which is void* (unknown type pointer)
    // 2. Casts it to uint8_t* (pointer to unsigned bytes)
    // 3. Now 'd' can be incremented and dereferenced
    //
    // After this: d++ moves forward 1 byte, *d reads 1 byte
    //

    // ========================================================================
    // WHY IS src DECLARED AS 'const'?
    // ========================================================================
    //
    // Say: "const means we promise NOT to modify the source data"
    //
    const uint8_t *s = (const uint8_t *)src;
    //
    // WHY const?
    // 1. memcpy must NOT modify the source - const enforces this
    // 2. Matches the standard function signature: const void *src
    // 3. Compiler will ERROR if you accidentally write to *s
    // 4. Documents intent: "this is read-only"
    //
    // EXAMPLE - const prevents bugs:
    //   *s = 0x00;  // COMPILER ERROR! s points to const data
    //   *d = *s;    // OK - reading from s, writing to d
    //
    // ========================================================================

    // ========================================================================
    // THE COPY LOOP: while (n--) { *d++ = *s++; }
    // ========================================================================
    //
    // Say: "Simple forward copy - assumes no overlap"
    //
    while (n--) {
        // ==================================================================
        // BREAKDOWN OF: *d++ = *s++;
        // ==================================================================
        //
        // This single line does FOUR things! Let's break it down:
        //
        // Step 1: *s     -> Read the byte that s points to
        // Step 2: *d = x -> Write that byte to where d points
        // Step 3: s++    -> Move s forward 1 byte (post-increment)
        // Step 4: d++    -> Move d forward 1 byte (post-increment)
        //
        // EXECUTION ORDER (due to post-increment):
        //   1. Read byte at current s
        //   2. Write to current d
        //   3. THEN increment both pointers
        //
        // VISUAL for copying 3 bytes:
        //
        //   src:  [A][B][C]
        //          ^
        //          s
        //   dest: [?][?][?]
        //          ^
        //          d
        //
        //   Iteration 1: *d = *s (copy A), then d++, s++
        //   src:  [A][B][C]
        //             ^
        //             s
        //   dest: [A][?][?]
        //             ^
        //             d
        //
        //   Iteration 2: *d = *s (copy B), then d++, s++
        //   src:  [A][B][C]
        //                ^
        //                s
        //   dest: [A][B][?]
        //                ^
        //                d
        //
        //   Iteration 3: *d = *s (copy C), then d++, s++
        //   dest: [A][B][C]  <- DONE!
        //
        // ==================================================================
        *d++ = *s++;
    }
    // Loop exits when n becomes 0 (all bytes copied)

    // ========================================================================
    // WHY RETURN dest?
    // ========================================================================
    //
    // Say: "Return dest for function chaining"
    //
    return dest;
    //
    // Matches standard memcpy behavior.
    // Allows chaining: printf("%s", memcpy(buf, src, n));
    // Note: We return the ORIGINAL dest, not the incremented d!
    //
    // ========================================================================
}

// ============================================================================
// memmove - Safe copy that handles overlapping memory regions
// ============================================================================
//
// Say: "memmove handles overlapping regions safely by choosing copy direction"
//
void *my_memmove(void *dest, const void *src, size_t n) {

    // Same casting as memcpy - see detailed explanation above
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    // ========================================================================
    // THE KEY DECISION: WHEN DO WE NEED TO COPY BACKWARDS?
    // ========================================================================
    //
    // Say: "I check if regions overlap AND dest is after src in memory"
    //
    // CONDITION: if (d > s && d < s + n)
    //
    // Let's break down this condition:
    //
    // d > s:
    //   - dest pointer is at a HIGHER memory address than src
    //   - This means dest is "to the right" of src in memory
    //
    // d < s + n:
    //   - dest starts BEFORE the end of src region
    //   - s + n is the first byte AFTER the source region
    //   - If d < s + n, there IS overlap
    //
    // BOTH must be true for dangerous overlap:
    //   - dest is after src (d > s)
    //   - AND dest starts within src's range (d < s + n)
    //
    // ========================================================================
    // VISUAL - THE DANGEROUS OVERLAP CASE (d > s && d < s + n):
    // ========================================================================
    //
    //   Memory addresses: 0    1    2    3    4    5    6
    //                    [src ][src ][src ][src ][src ]
    //                     ^                             ^
    //                     s                           s+n (end of src)
    //
    //                              [dest][dest][dest][dest][dest]
    //                               ^
    //                               d
    //
    //   Here: d=2, s=0, n=5, s+n=5
    //   Is d > s? 2 > 0? YES
    //   Is d < s+n? 2 < 5? YES
    //   BOTH true -> OVERLAP! Need backward copy.
    //
    // WHY FORWARD COPY FAILS HERE:
    //   Copying src[0] to dest[0] (which is src[2]) overwrites src[2]
    //   BEFORE we've copied src[2] to dest[2]!
    //
    // ========================================================================
    // VISUAL - SAFE CASES (no backward copy needed):
    // ========================================================================
    //
    // CASE 1: d < s (dest is before src) - Forward copy is safe
    //   [dest][dest][dest]
    //                     [src ][src ][src ]
    //   We never overwrite src before reading it.
    //
    // CASE 2: d >= s + n (no overlap at all) - Forward copy is safe
    //   [src ][src ][src ]
    //                     [dest][dest][dest]
    //   Regions don't touch.
    //
    // ========================================================================

    if (d > s && d < s + n) {
        // ====================================================================
        // OVERLAP DETECTED! COPY BACKWARDS
        // ====================================================================
        //
        // Say: "Overlap case: copy backwards to avoid overwriting source"
        //
        // STRATEGY:
        // - Start from the END of both regions
        // - Copy backwards (right to left)
        // - This way, we copy data BEFORE we overwrite it
        //
        // ====================================================================

        d += n;
        s += n;
        //
        // WHAT THESE LINES DO:
        // - d += n: Move d to point ONE PAST the end of dest region
        // - s += n: Move s to point ONE PAST the end of src region
        //
        // WHY "one past"? Because we use PRE-decrement (*--d, *--s)
        // The pre-decrement will move the pointer BEFORE dereferencing.
        //
        // VISUAL after d += n and s += n:
        //
        //   Memory: [0][1][2][3][4][5][6]
        //   src region: indices 0-4
        //   dest region: indices 2-6
        //
        //   After s += 5: s points to index 5 (one past src end)
        //   After d += 5: d points to index 7 (one past dest end)
        //
        // ====================================================================

        while (n--) {
            // ================================================================
            // BREAKDOWN OF: *--d = *--s;
            // ================================================================
            //
            // This is PRE-decrement (different from post-increment!)
            //
            // Step 1: --s    -> Decrement s FIRST (move back 1 byte)
            // Step 2: *s     -> Read the byte at new s position
            // Step 3: --d    -> Decrement d FIRST (move back 1 byte)
            // Step 4: *d = x -> Write the byte to new d position
            //
            // VISUAL for copying backwards:
            //
            //   Initially (after d+=n, s+=n):
            //   src:  [A][B][C][D][E]    <- we want to copy these 5 bytes
            //                        ^
            //                        s (one past end)
            //   dest: [?][?][?][?][?]
            //                        ^
            //                        d (one past end)
            //
            //   Iteration 1: --s, --d, then copy
            //   src:  [A][B][C][D][E]
            //                     ^
            //                     s
            //   dest: [?][?][?][?][E]   <- E copied first!
            //                     ^
            //                     d
            //
            //   Iteration 2: --s, --d, then copy
            //   src:  [A][B][C][D][E]
            //                  ^
            //                  s
            //   dest: [?][?][?][D][E]   <- D copied
            //                  ^
            //                  d
            //
            //   ... continues until all bytes copied right-to-left
            //
            //   Final: dest: [A][B][C][D][E]
            //
            // WHY THIS WORKS WITH OVERLAP:
            // We copy the LAST bytes first, which are FURTHEST from src start.
            // By the time we copy the overlapping bytes, we've already
            // saved the data that would have been overwritten!
            //
            // ================================================================
            *--d = *--s;
        }

    } else {
        // ====================================================================
        // NO DANGEROUS OVERLAP - FORWARD COPY IS SAFE
        // ====================================================================
        //
        // Say: "No overlap or dest < src: forward copy is safe"
        //
        // We get here when:
        // 1. d <= s: dest is before or at src (copying left doesn't overwrite)
        // 2. d >= s + n: dest starts after src ends (no overlap at all)
        //
        // In these cases, simple forward copy works fine!
        //
        // ====================================================================

        while (n--) {
            *d++ = *s++;  // Same as memcpy - forward copy
        }
    }

    // ========================================================================
    // RETURN ORIGINAL dest POINTER
    // ========================================================================
    //
    // Note: d has been modified (incremented/decremented during copy)
    // We return the ORIGINAL dest that was passed in.
    // This matches the standard memmove behavior.
    //
    // ========================================================================

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
