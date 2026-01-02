/*
 * ============================================================================
 * PROBLEM: Implement strstr() - Find Substring
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: Very High
 *
 * Find the first occurrence of needle in haystack.
 * Return pointer to first match, or NULL if not found.
 *
 * Example:
 * Input:  haystack = "hello", needle = "ll"
 * Output: pointer to "llo" (index 2)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. BRUTE FORCE APPROACH:
 *    - Try matching needle at each position
 *    - If mismatch, move to next position
 *
 * 2. OPTIMIZATION:
 *    - Only search until haystack_len - needle_len
 *    - Early exit on mismatch
 *
 * 3. EDGE CASES:
 *    - Empty needle: Return haystack
 *    - Needle longer than haystack: Return NULL
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: haystack="hello", needle="ll"
 * ============================================================================
 *
 * Position 0: Try to match "ll" starting at 'h'
 *
 *   h e l l o
 *   ^
 *   l l       → 'h' != 'l' ✗ Move to next
 *
 * Position 1: Try to match "ll" starting at 'e'
 *
 *   h e l l o
 *     ^
 *     l l     → 'e' != 'l' ✗ Move to next
 *
 * Position 2: Try to match "ll" starting at first 'l'
 *
 *   h e l l o
 *       ^
 *       l l   → 'l' == 'l' ✓
 *         ^
 *         l   → 'l' == 'l' ✓ FULL MATCH!
 *
 * Return pointer to index 2
 *
 * ============================================================================
 * SLIDING WINDOW VISUALIZATION:
 * ============================================================================
 *
 * haystack: "hello world"
 * needle:   "wor"
 *
 *   h e l l o   w o r l d
 *   [w o r]                   No match at 0
 *     [w o r]                 No match at 1
 *       [w o r]               No match at 2
 *         [w o r]             No match at 3
 *           [w o r]           No match at 4
 *             [w o r]         MATCH at 6!
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   if needle is empty:
 *       return haystack
 *
 *   for i = 0 to haystack_len - needle_len:
 *       j = 0
 *       while j < needle_len AND haystack[i+j] == needle[j]:
 *           j++
 *
 *       if j == needle_len:
 *           return &haystack[i]  // Found!
 *
 *   return NULL  // Not found
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n * m)
 * ============================================================================
 * - n = length of haystack
 * - m = length of needle
 * - Worst case: compare m chars at each of n positions
 *
 * Note: KMP algorithm can do O(n + m) but is more complex
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only loop variables
 * - No extra arrays
 *
 * ============================================================================
 * BETTER ALGORITHMS (Know these exist):
 * ============================================================================
 *
 *   Algorithm      | Time          | When to use
 *   ---------------|---------------|------------------
 *   Brute Force    | O(n*m)        | Short strings
 *   KMP            | O(n+m)        | Long patterns
 *   Rabin-Karp     | O(n+m) avg    | Multiple patterns
 *   Boyer-Moore    | O(n/m) best   | Long texts
 *
 * For interviews, brute force is usually acceptable!
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Empty needle: Return haystack (convention)
 * 2. Empty haystack: Return NULL (unless needle empty)
 * 3. Needle longer than haystack: Return NULL
 * 4. Needle equals haystack: Return haystack
 * 5. Needle at end of haystack: Still find it
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

// Implement strstr from scratch
char* my_strstr(char* haystack, char* needle) {
    // Empty needle returns haystack
    if (*needle == '\0') {
        return haystack;
    }

    int h_len = strlen(haystack);
    int n_len = strlen(needle);

    // Needle longer than haystack
    if (n_len > h_len) {
        return NULL;
    }

    // Try each starting position
    for (int i = 0; i <= h_len - n_len; i++) {
        int j = 0;

        // Try to match needle starting at position i
        while (j < n_len && haystack[i + j] == needle[j]) {
            j++;
        }

        // If we matched all of needle
        if (j == n_len) {
            return &haystack[i];
        }
    }

    return NULL;  // Not found
}

// Return index instead of pointer
int strstr_index(char* haystack, char* needle) {
    char* result = my_strstr(haystack, needle);
    if (result == NULL) {
        return -1;
    }
    return result - haystack;
}

int main() {
    printf("=== Implement strstr ===\n\n");

    char* result;

    result = my_strstr("hello", "ll");
    printf("strstr(\"hello\", \"ll\"): ");
    if (result) {
        printf("Found at index %ld → \"%s\"\n", result - "hello", result);
    } else {
        printf("Not found\n");
    }

    result = my_strstr("hello", "lo");
    printf("strstr(\"hello\", \"lo\"): ");
    if (result) {
        printf("Found at index %ld → \"%s\"\n", result - "hello", result);
    } else {
        printf("Not found\n");
    }

    result = my_strstr("hello", "world");
    printf("strstr(\"hello\", \"world\"): ");
    if (result) {
        printf("Found\n");
    } else {
        printf("Not found\n");
    }

    result = my_strstr("hello", "");
    printf("strstr(\"hello\", \"\"): ");
    if (result) {
        printf("Returns haystack → \"%s\"\n", result);
    }

    printf("\n=== Using Index Function ===\n");
    printf("Index of \"ll\" in \"hello\": %d\n", strstr_index("hello", "ll"));
    printf("Index of \"wor\" in \"hello world\": %d\n",
           strstr_index("hello world", "wor"));
    printf("Index of \"xyz\" in \"hello\": %d\n", strstr_index("hello", "xyz"));

    return 0;
}
