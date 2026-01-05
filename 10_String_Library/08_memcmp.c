/*
 * ============================================================================
 * PROBLEM: Implement memcmp()
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: HIGH
 *
 * memcmp compares n bytes of two memory regions
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. UNSIGNED CHAR FOR BYTE COMPARISON:
 *    - void* must be cast to unsigned char* for dereferencing
 *    - unsigned ensures bytes 0-255 compare correctly
 *    - signed char would make 0x80-0xFF appear negative
 *
 * 2. KEY DIFFERENCE FROM strcmp:
 *    - strcmp stops at '\0' (null terminator)
 *    - memcmp compares exactly n bytes, doesn't stop at '\0'
 *    - Use memcmp for binary data, structs, arrays with embedded nulls
 *
 * 3. RETURN VALUE INTERPRETATION:
 *    - < 0: ptr1's first differing byte is smaller
 *    - = 0: All n bytes are identical
 *    - > 0: ptr1's first differing byte is larger
 *
 * ============================================================================
 * VISUALIZATION: memcmp("ABC\0D", "ABC\0E", 5)
 * ============================================================================
 *
 *   ptr1: ['A']['B']['C']['\0']['D']
 *   ptr2: ['A']['B']['C']['\0']['E']
 *
 *   Compare byte 0: 'A' == 'A' ✓ continue
 *   Compare byte 1: 'B' == 'B' ✓ continue
 *   Compare byte 2: 'C' == 'C' ✓ continue
 *   Compare byte 3: '\0' == '\0' ✓ continue  ← strcmp would STOP here!
 *   Compare byte 4: 'D' != 'E' ✗ STOP
 *
 *   Return: 'D' - 'E' = 68 - 69 = -1 (negative)
 *
 * ============================================================================
 * RETURN VALUES:
 * ============================================================================
 *
 *   < 0 : First differing byte is smaller in ptr1
 *   = 0 : Memory blocks are identical
 *   > 0 : First differing byte is larger in ptr1
 *
 * ============================================================================
 * DIFFERENCE FROM strcmp:
 * ============================================================================
 *
 *   strcmp:
 *   - Stops at null terminator
 *   - For strings only
 *
 *   memcmp:
 *   - Compares exactly n bytes
 *   - Works on any memory (binary data, structs, etc.)
 *   - Does NOT stop at null bytes
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why use memcmp instead of strcmp?"
 * A1: strcmp stops at null bytes, memcmp doesn't. For binary data (images,
 *     network packets, encrypted data), null bytes are valid data - strcmp
 *     would give wrong results. Also use memcmp to compare structs or arrays.
 *
 * -------------------------------------------------------------------------
 * Q2: "Can you use memcmp to compare structs?"
 * A2: Yes, but beware of padding! Compilers add padding bytes for alignment,
 *     and padding contents are undefined. Two "equal" structs may have
 *     different padding bytes. Either memset struct to 0 before use, or
 *     compare field-by-field for safety.
 *
 * -------------------------------------------------------------------------
 * Q3: "Is memcmp safe for cryptographic comparisons?"
 * A3: NO! Standard memcmp is vulnerable to timing attacks - it returns early
 *     on first difference. Attacker can measure time to guess secrets.
 *     Use constant-time comparison for crypto: always compare ALL bytes,
 *     accumulate differences with XOR, then check if result is zero.
 *
 * -------------------------------------------------------------------------
 * Q4: "How do optimized memcmp implementations work?"
 * A4: Compare word-at-a-time (8 bytes) when aligned. XOR words to find
 *     differences quickly. Only do byte-by-byte for unaligned portions or
 *     when difference found. Can use SIMD for even faster bulk comparison.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

/*
 * ============================================================================
 * MEMCMP FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * int my_memcmp(const void* ptr1, const void* ptr2, size_t n):
 *   - Returns "int" = difference of first differing bytes
 *   - "const void* ptr1" = first memory region (read-only)
 *   - "const void* ptr2" = second memory region (read-only)
 *   - "size_t n" = number of bytes to compare
 *
 * const unsigned char* p1 = (const unsigned char*)ptr1:
 *   - Cast void* to unsigned char* for byte access
 *   - WHY const? We're only reading, not modifying
 *   - WHY unsigned? Bytes 0-255 compare correctly
 *
 * while (n > 0):
 *   - Compare exactly n bytes (doesn't stop at '\0')
 *   - KEY DIFFERENCE from strcmp!
 *
 * if (*p1 != *p2) return *p1 - *p2:
 *   - Found difference, return immediately
 *   - Positive if p1 > p2, negative if p1 < p2
 *
 * return 0:
 *   - All n bytes matched
 *
 * ============================================================================
 */
// memcmp - Compare n bytes of two memory regions
// Say: "I'll implement memcmp by comparing exactly n bytes, regardless of null terminators"
int my_memcmp(const void* ptr1, const void* ptr2, size_t n) {
    // Handle NULL pointers to avoid crashes
    // Say: "First, I check for NULL pointers"
    // WHY: Dereferencing NULL causes segmentation fault
    if (ptr1 == NULL || ptr2 == NULL) {
        // Both NULL = equal
        if (ptr1 == ptr2) return 0;
        // One NULL = NULL is "less than" non-NULL
        return ptr1 == NULL ? -1 : 1;
    }

    // Cast void pointers to unsigned char for byte-level comparison
    // Say: "I cast both pointers to unsigned char to compare byte by byte"
    // WHY: void* cannot be dereferenced; unsigned char ensures 0-255 range
    const unsigned char* p1 = (const unsigned char*)ptr1;
    const unsigned char* p2 = (const unsigned char*)ptr2;

    // Compare exactly n bytes (does NOT stop at '\0' like strcmp!)
    // Say: "I compare each byte until I find a difference or reach n bytes"
    // WHY: Key difference from strcmp - compares binary data correctly
    while (n > 0) {
        // Check if current bytes differ
        // Say: "If the bytes differ, I return their difference"
        // WHY: Positive means p1 > p2, negative means p1 < p2
        if (*p1 != *p2) {
            return *p1 - *p2;   // Return difference of first non-matching bytes
        }

        // Move to next byte in both regions
        // Say: "I advance both pointers to the next byte"
        p1++;       // Advance first pointer
        p2++;       // Advance second pointer
        // Decrement remaining count
        // Say: "I decrement the count of remaining bytes to compare"
        n--;
    }

    // All n bytes matched
    // Say: "If all n bytes matched, I return zero"
    // WHY: Zero indicates the memory regions are identical
    return 0;
}

int main() {
    printf("=== memcmp Implementation ===\n\n");

    // Basic comparison
    printf("1. Basic memcmp:\n");
    printf("   memcmp(\"ABC\", \"ABC\", 3) = %d (equal)\n",
           my_memcmp("ABC", "ABC", 3));
    printf("   memcmp(\"ABC\", \"ABD\", 3) = %d (negative)\n",
           my_memcmp("ABC", "ABD", 3));
    printf("   memcmp(\"ABD\", \"ABC\", 3) = %d (positive)\n\n",
           my_memcmp("ABD", "ABC", 3));

    // Partial comparison
    printf("2. Partial comparison:\n");
    printf("   memcmp(\"Hello\", \"Help\", 3) = %d (first 3 match)\n",
           my_memcmp("Hello", "Help", 3));
    printf("   memcmp(\"Hello\", \"Help\", 4) = %d (4th differs)\n\n",
           my_memcmp("Hello", "Help", 4));

    // Binary data (including null bytes)
    printf("3. Binary data with null bytes:\n");
    char bin1[] = {0x01, 0x00, 0x02};  // Contains null byte
    char bin2[] = {0x01, 0x00, 0x02};
    char bin3[] = {0x01, 0x00, 0x03};
    printf("   {0x01, 0x00, 0x02} vs {0x01, 0x00, 0x02}: %d\n",
           my_memcmp(bin1, bin2, 3));
    printf("   {0x01, 0x00, 0x02} vs {0x01, 0x00, 0x03}: %d\n\n",
           my_memcmp(bin1, bin3, 3));

    // Compare structs
    printf("4. Compare structs:\n");
    struct Point { int x; int y; } p1 = {10, 20}, p2 = {10, 20}, p3 = {10, 30};
    printf("   p1={10,20} vs p2={10,20}: %d (equal)\n",
           my_memcmp(&p1, &p2, sizeof(struct Point)));
    printf("   p1={10,20} vs p3={10,30}: %d (not equal)\n\n",
           my_memcmp(&p1, &p3, sizeof(struct Point)));

    printf("=== Key Points ===\n");
    printf("- Compares exactly n bytes (doesn't stop at null)\n");
    printf("- Works on any memory, not just strings\n");
    printf("- Use for binary data, structs, arrays\n");
    printf("- Returns difference of first non-matching byte\n");

    return 0;
}
