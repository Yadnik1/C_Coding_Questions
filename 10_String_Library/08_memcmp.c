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
 */

#include <stdio.h>
#include <stddef.h>

// memcmp - Compare n bytes of two memory regions
// Say: "I'll implement memcmp by comparing exactly n bytes, regardless of null terminators"
int my_memcmp(const void* ptr1, const void* ptr2, size_t n) {
    // Handle NULL pointers to avoid crashes
    // Say: "First, I check for NULL pointers"
    if (ptr1 == NULL || ptr2 == NULL) {
        if (ptr1 == ptr2) return 0;         // Both NULL, consider equal
        return ptr1 == NULL ? -1 : 1;       // One NULL
    }

    // Cast void pointers to unsigned char for byte-level comparison
    // Say: "I cast both pointers to unsigned char to compare byte by byte"
    const unsigned char* p1 = (const unsigned char*)ptr1;
    const unsigned char* p2 = (const unsigned char*)ptr2;

    // Compare exactly n bytes
    // Say: "I compare each byte until I find a difference or reach n bytes"
    while (n > 0) {
        // Check if current bytes differ
        // Say: "If the bytes differ, I return their difference"
        if (*p1 != *p2) {
            return *p1 - *p2;   // Return difference of first non-matching bytes
        }

        // Move to next byte in both regions
        p1++;       // Advance first pointer
        p2++;       // Advance second pointer
        n--;        // Decrement remaining count
    }

    // All n bytes matched
    // Say: "If all n bytes matched, I return zero"
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
