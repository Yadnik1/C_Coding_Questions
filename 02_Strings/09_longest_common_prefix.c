/*
 * ============================================================================
 * PROBLEM: Longest Common Prefix
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: High
 *
 * Find the longest common prefix string amongst an array of strings.
 *
 * Example:
 * Input:  ["flower", "flow", "flight"]
 * Output: "fl"
 *
 * Input:  ["dog", "racecar", "car"]
 * Output: "" (no common prefix)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. HORIZONTAL SCANNING:
 *    - Start with first string as prefix
 *    - Compare with each subsequent string
 *    - Shorten prefix until it matches
 *
 * 2. VERTICAL SCANNING:
 *    - Compare characters column by column
 *    - Stop when mismatch found
 *
 * 3. KEY INSIGHT:
 *    - Prefix can only get shorter, never longer
 *    - Stop as soon as prefix becomes empty
 *
 * ============================================================================
 * METHOD 1: HORIZONTAL SCANNING
 * ============================================================================
 *
 * Strings: ["flower", "flow", "flight"]
 *
 * Step 1: prefix = "flower"
 *
 * Step 2: Compare with "flow"
 *   "flower" vs "flow"
 *   flower starts with flow? NO
 *   Remove last char: "flowe"
 *   flowe starts with flow? NO
 *   Remove: "flow"
 *   flow starts with flow? YES!
 *   prefix = "flow"
 *
 * Step 3: Compare with "flight"
 *   "flow" vs "flight"
 *   flight starts with flow? NO
 *   Remove: "flo"
 *   flight starts with flo? NO
 *   Remove: "fl"
 *   flight starts with fl? YES!
 *   prefix = "fl"
 *
 * Result: "fl"
 *
 * ============================================================================
 * METHOD 2: VERTICAL SCANNING (SIMPLER)
 * ============================================================================
 *
 * Strings: ["flower", "flow", "flight"]
 *
 * Compare column by column:
 *
 *   Col 0: f f f → All same ✓
 *   Col 1: l l l → All same ✓
 *   Col 2: o o i → MISMATCH! Stop
 *
 * Result: "fl" (first 2 characters)
 *
 *   f l o w e r
 *   f l o w
 *   f l i g h t
 *   ^ ^
 *   Match until here
 *
 * ============================================================================
 * ALGORITHM (Vertical Scanning):
 * ============================================================================
 *
 *   if empty array: return ""
 *
 *   for col = 0 to len(first_string):
 *       char c = first_string[col]
 *
 *       for each string in array:
 *           if col >= len(string) OR string[col] != c:
 *               return first_string[0...col-1]
 *
 *   return first_string  // All strings are identical
 *
 * ============================================================================
 * TIME COMPLEXITY: O(S)
 * ============================================================================
 * - S = sum of all characters in all strings
 * - Worst case: all strings identical
 * - Best case: first characters differ = O(n)
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only store prefix length
 * - Or O(m) if copying result string
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Empty array: Return ""
 * 2. Single string: Return that string
 * 3. Empty string in array: Return ""
 * 4. All identical: Return full string
 * 5. No common prefix: Return ""
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Vertical vs horizontal scanning?"
 * A1: Vertical: Compare char by char across ALL strings (shown here).
 *     Horizontal: Compare pairs of strings, reduce prefix progressively.
 *     Both are O(S) where S = sum of all characters.
 *
 * -------------------------------------------------------------------------
 * Q2: "Can binary search help here?"
 * A2: Yes! Binary search on prefix length (0 to min_length).
 *     For each mid, check if first 'mid' chars match all strings.
 *     Time: O(S * log(minLen)), useful when strings are very similar.
 *
 * -------------------------------------------------------------------------
 * Q3: "What about using Trie?"
 * A3: Build trie from all strings. LCP is the path from root until
 *     first branch (node with >1 children) or end marker.
 *     Good for multiple queries but O(S) preprocessing.
 *
 * -------------------------------------------------------------------------
 * Q4: "Why is early termination important?"
 * A4: If any character doesn't match, we can stop immediately.
 *     Best case: O(minLen) when strings differ early.
 *     Worst case: O(S) when all strings are identical.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Vertical scanning approach
char* longest_common_prefix(char* strs[], int n) {
    // Check for empty array or empty first string
    // Say: "First, I check for edge cases: empty array or empty first string"
    if (n == 0 || strs[0][0] == '\0') {
        return "";
    }

    // Start with length of first string as max prefix
    // Say: "I'll start by assuming the first string could be the common prefix"
    int prefix_len = strlen(strs[0]);

    // Compare with each string
    // Say: "Now I compare each string with the first one to find the common prefix"
    for (int i = 1; i < n; i++) {
        // Initialize character position
        // Say: "For each string, I compare characters one by one"
        int j = 0;

        // Compare character by character
        // Say: "I keep comparing while characters match and I haven't exceeded either string"
        while (j < prefix_len &&
               strs[i][j] != '\0' &&
               strs[0][j] == strs[i][j]) {
            // Say: "Characters match, so I move to the next position"
            j++;
        }

        // Update prefix length to minimum match
        // Say: "I update the prefix length to be the minimum I've found so far"
        prefix_len = j;

        // Early exit if no common prefix
        // Say: "If prefix length becomes zero, there's no common prefix"
        if (prefix_len == 0) {
            return "";
        }
    }

    // Create result string
    // Say: "Finally, I create the result string by copying the prefix"
    static char result[100];

    // Copy prefix from first string
    // Say: "I copy the first prefix_len characters from the first string"
    strncpy(result, strs[0], prefix_len);

    // Null terminate the result
    // Say: "And add a null terminator to complete the string"
    result[prefix_len] = '\0';

    // Return the common prefix
    // Say: "I return the longest common prefix"
    return result;
}

// Alternative: Find prefix between two strings first
int common_prefix_len(char* s1, char* s2) {
    // Initialize index
    // Say: "This helper finds the common prefix length between two strings"
    int i = 0;

    // Compare characters while they match
    // Say: "I compare characters until they differ or a string ends"
    while (s1[i] && s2[i] && s1[i] == s2[i]) {
        // Say: "Characters match, increment index"
        i++;
    }

    // Return length of common prefix
    // Say: "Return the length of matching prefix"
    return i;
}

char* lcp_horizontal(char* strs[], int n) {
    // Check for empty array
    // Say: "This is the horizontal scanning approach"
    if (n == 0) return "";

    // Start with first string length
    // Say: "I start with the full length of the first string as potential prefix"
    int prefix_len = strlen(strs[0]);

    // Compare with each subsequent string
    // Say: "I compare with each string and reduce the prefix length as needed"
    for (int i = 1; i < n; i++) {
        // Find common prefix with this string
        // Say: "Find how much of the prefix matches this string"
        int len = common_prefix_len(strs[0], strs[i]);

        // Update to shorter prefix
        // Say: "Take the minimum prefix length"
        if (len < prefix_len) {
            prefix_len = len;
        }

        // Early exit if no prefix
        // Say: "If no common prefix exists, return empty string"
        if (prefix_len == 0) return "";
    }

    // Build result string
    // Say: "Build the result by copying the common prefix"
    static char result[100];
    strncpy(result, strs[0], prefix_len);
    result[prefix_len] = '\0';
    return result;
}

int main() {
    printf("=== Longest Common Prefix ===\n\n");

    char* strs1[] = {"flower", "flow", "flight"};
    printf("[\"flower\", \"flow\", \"flight\"]\n");
    printf("LCP: \"%s\"\n\n", longest_common_prefix(strs1, 3));

    char* strs2[] = {"dog", "racecar", "car"};
    printf("[\"dog\", \"racecar\", \"car\"]\n");
    printf("LCP: \"%s\"\n\n", longest_common_prefix(strs2, 3));

    char* strs3[] = {"interspecies", "interstellar", "interstate"};
    printf("[\"interspecies\", \"interstellar\", \"interstate\"]\n");
    printf("LCP: \"%s\"\n\n", longest_common_prefix(strs3, 3));

    char* strs4[] = {"a"};
    printf("[\"a\"]\n");
    printf("LCP: \"%s\"\n\n", longest_common_prefix(strs4, 1));

    char* strs5[] = {"", "b"};
    printf("[\"\", \"b\"]\n");
    printf("LCP: \"%s\"\n", longest_common_prefix(strs5, 2));

    return 0;
}
