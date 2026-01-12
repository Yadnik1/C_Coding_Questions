/*
 * ============================================================================
 * PROBLEM: Implement strstr() - Find Substring in String
 * ============================================================================
 *
 * WHAT IS strstr()?
 * Find the first occurrence of a substring (needle) in a string (haystack).
 * Returns pointer to the first match, or NULL if not found.
 *
 * EXAMPLES:
 *   strstr("hello world", "world")  -> pointer to "world" (position 6)
 *   strstr("hello world", "xyz")    -> NULL (not found)
 *   strstr("hello", "ll")           -> pointer to "llo" (position 2)
 *
 * TIME COMPLEXITY: O(n * m)
 * SPACE COMPLEXITY: O(1)
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

char* my_strstr(const char* haystack, const char* needle) {
    if (*needle == '\0') return (char*)haystack;  // Empty needle returns haystack

    for (int h = 0; haystack[h] != '\0'; h++) {   // Try each starting position
        int n;
        for (n = 0; needle[n] != '\0'; n++) {     // Try to match needle
            if (haystack[h + n] == '\0') return NULL;  // Haystack ended early
            if (haystack[h + n] != needle[n]) break;   // Mismatch
        }
        if (needle[n] == '\0') return (char*)&haystack[h];  // Full match found
    }
    return NULL;  // Not found
}

int main() {
    const char* str = "hello world";

    printf("strstr(\"%s\", \"world\") = %s\n", str, my_strstr(str, "world"));
    printf("strstr(\"%s\", \"xyz\") = %s\n", str, my_strstr(str, "xyz") ? my_strstr(str, "xyz") : "NULL");
    printf("strstr(\"%s\", \"ll\") = %s\n", str, my_strstr(str, "ll"));

    return 0;
}

/*
 * ============================================================================
 * DRY RUN: my_strstr("hello", "ll")
 * ============================================================================
 *
 * haystack = "hello"
 * needle   = "ll"
 *
 * h=0: haystack[0]='h'
 *   n=0: 'h' vs 'l' -> MISMATCH, break
 *   needle[0]='l' != '\0' -> no match
 *
 * h=1: haystack[1]='e'
 *   n=0: 'e' vs 'l' -> MISMATCH, break
 *   needle[0]='l' != '\0' -> no match
 *
 * h=2: haystack[2]='l'
 *   n=0: 'l' vs 'l' -> MATCH, n++
 *   n=1: haystack[3]='l' vs needle[1]='l' -> MATCH, n++
 *   n=2: needle[2]='\0' -> break
 *   needle[2]='\0' -> FOUND! return &haystack[2]
 *
 * RESULT: Returns pointer to "llo" (position 2)
 *
 * ============================================================================
 * INTERVIEW TIPS:
 * - Handle empty needle (return haystack)
 * - Check haystack doesn't end early
 * - Return pointer to match start, not boolean
 * ============================================================================
 */
