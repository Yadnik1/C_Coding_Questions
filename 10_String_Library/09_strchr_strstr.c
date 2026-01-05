/*
 * ============================================================================
 * PROBLEM: Implement strchr() and strstr()
 * ============================================================================
 *
 * DIFFICULTY: Easy-Medium | TIME: 10 mins | FREQUENCY: HIGH
 *
 * strchr - Find first occurrence of character in string
 * strstr - Find first occurrence of substring in string
 *
 * ============================================================================
 * VISUALIZATION: strchr("Hello", 'l')
 * ============================================================================
 *
 *   str: ['H']['e']['l']['l']['o']['\0']
 *                  ^
 *                  First 'l' found at index 2
 *
 *   Returns: pointer to str[2]
 *
 * ============================================================================
 * VISUALIZATION: strstr("Hello World", "World")
 * ============================================================================
 *
 *   haystack: "Hello World"
 *   needle:   "World"
 *
 *   Check at index 0: "Hello" != "World"
 *   Check at index 1: "ello " != "World"
 *   ...
 *   Check at index 6: "World" == "World" ✓
 *
 *   Returns: pointer to haystack[6]
 *
 * ============================================================================
 * TIME: strchr O(n), strstr O(n*m) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Can strstr be optimized beyond O(n*m)?"
 * A1: Yes! KMP (Knuth-Morris-Pratt) achieves O(n+m) by preprocessing needle
 *     to skip redundant comparisons. Rabin-Karp uses rolling hash for O(n+m)
 *     average case. Boyer-Moore skips ahead based on mismatch character.
 *     For short needles, naive O(n*m) is often faster due to simplicity.
 *
 * -------------------------------------------------------------------------
 * Q2: "Why does strchr take int c instead of char c?"
 * A2: Historical reasons - early C promoted char to int in function calls.
 *     Also allows EOF (-1) to be passed for certain use cases. The value
 *     is internally cast to char. Modern code could use char, but int is
 *     standard for compatibility.
 *
 * -------------------------------------------------------------------------
 * Q3: "How does strstr handle empty needle?"
 * A3: Empty needle returns pointer to start of haystack (standard behavior).
 *     This is logical: every string "contains" the empty string at position 0.
 *     Important to check this edge case explicitly in implementation.
 *
 * -------------------------------------------------------------------------
 * Q4: "What's the difference between strchr and memchr?"
 * A4: strchr stops at null terminator, memchr searches exactly n bytes.
 *     Use memchr for binary data that may contain null bytes, or when you
 *     know the exact length. memchr is also faster for fixed-size buffers.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

// strchr - Find character in string
// Say: "I'll implement strchr by scanning through the string until I find the character"
char* my_strchr(const char* str, int c) {
    // Check for NULL pointer
    // Say: "First, I check if the string pointer is NULL"
    if (str == NULL) return NULL;

    // Scan through string looking for the character
    // Say: "I scan through each character looking for a match"
    while (*str != '\0') {
        // Check if current character matches what we're looking for
        // Say: "If I find the character, I return a pointer to its location"
        if (*str == (char)c) {
            return (char*)str;  // Found it, return pointer to this position
        }
        str++;  // Move to next character
    }

    // Special case: searching for null terminator itself
    // Say: "I also need to check if we're searching for the null terminator"
    if ((char)c == '\0') {
        return (char*)str;  // Return pointer to the null terminator
    }

    // Character not found
    // Say: "If not found, I return NULL"
    return NULL;
}

// strrchr - Find LAST occurrence of character
// Say: "I'll implement strrchr which finds the last occurrence instead of the first"
char* my_strrchr(const char* str, int c) {
    // Check for NULL pointer
    // Say: "Check for NULL pointer"
    if (str == NULL) return NULL;

    // Track the last position where we found the character
    // Say: "I keep track of the last position where I found the character"
    const char* last = NULL;

    // Scan entire string, updating last position each time we find it
    // Say: "I scan the entire string, updating my pointer each time I find a match"
    while (*str != '\0') {
        // Update last position if we find the character
        if (*str == (char)c) {
            last = str;     // Update last found position
        }
        str++;  // Keep scanning
    }

    // Check for null terminator
    // Say: "Check if we're looking for the null terminator"
    if ((char)c == '\0') {
        return (char*)str;  // Return pointer to null terminator
    }

    // Return the last position found (or NULL if never found)
    // Say: "Return the last position where we found it, or NULL if not found"
    return (char*)last;
}

// strstr - Find substring in string
// Say: "I'll implement strstr to find a substring within a string"
char* my_strstr(const char* haystack, const char* needle) {
    // Check for NULL pointers
    // Say: "First, I check that both pointers are valid"
    if (haystack == NULL || needle == NULL) return NULL;

    // Empty needle matches at the start (standard behavior)
    // Say: "An empty needle is defined to match at the beginning"
    if (*needle == '\0') {
        return (char*)haystack;
    }

    // Try to find needle starting at each position in haystack
    // Say: "I try matching the needle at each position in the haystack"
    while (*haystack != '\0') {
        // Set up pointers to compare from this position
        const char* h = haystack;   // Current position in haystack
        const char* n = needle;     // Start of needle

        // Try to match needle at current position
        // Say: "At each position, I try to match the entire needle"
        while (*h == *n && *n != '\0') {
            h++;    // Advance in haystack
            n++;    // Advance in needle
        }

        // If we reached end of needle, we found a complete match
        // Say: "If I matched the entire needle, I return the starting position"
        if (*n == '\0') {
            return (char*)haystack;  // Match found at this position
        }

        // No match at this position, try next position in haystack
        haystack++;
    }

    // Needle not found in haystack
    // Say: "If no match is found, I return NULL"
    return NULL;
}

int main() {
    printf("=== strchr / strstr Implementation ===\n\n");

    // strchr
    printf("1. strchr - Find character:\n");
    const char* str = "Hello World";

    char* p = my_strchr(str, 'o');
    printf("   strchr(\"%s\", 'o') → \"%s\"\n", str, p ? p : "NULL");

    p = my_strchr(str, 'l');
    printf("   strchr(\"%s\", 'l') → \"%s\"\n", str, p ? p : "NULL");

    p = my_strchr(str, 'z');
    printf("   strchr(\"%s\", 'z') → %s\n\n", str, p ? p : "NULL");

    // strrchr
    printf("2. strrchr - Find LAST occurrence:\n");
    p = my_strrchr(str, 'o');
    printf("   strrchr(\"%s\", 'o') → \"%s\"\n", str, p ? p : "NULL");

    p = my_strrchr(str, 'l');
    printf("   strrchr(\"%s\", 'l') → \"%s\"\n\n", str, p ? p : "NULL");

    // strstr
    printf("3. strstr - Find substring:\n");
    p = my_strstr("Hello World", "World");
    printf("   strstr(\"Hello World\", \"World\") → \"%s\"\n", p ? p : "NULL");

    p = my_strstr("Hello World", "lo");
    printf("   strstr(\"Hello World\", \"lo\")    → \"%s\"\n", p ? p : "NULL");

    p = my_strstr("Hello World", "xyz");
    printf("   strstr(\"Hello World\", \"xyz\")   → %s\n", p ? p : "NULL");

    p = my_strstr("Hello World", "");
    printf("   strstr(\"Hello World\", \"\")      → \"%s\"\n\n", p ? p : "NULL");

    printf("=== Key Points ===\n");
    printf("strchr:\n");
    printf("  - Returns pointer to first match\n");
    printf("  - Returns NULL if not found\n");
    printf("  - Can search for '\\0'\n\n");
    printf("strstr:\n");
    printf("  - Returns pointer to start of substring\n");
    printf("  - Empty needle returns haystack\n");
    printf("  - O(n*m) worst case\n");

    return 0;
}
