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
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Vertical scanning approach
char* longest_common_prefix(char* strs[], int n) {
    if (n == 0 || strs[0][0] == '\0') {
        return "";
    }

    int prefix_len = strlen(strs[0]);

    // Compare with each string
    for (int i = 1; i < n; i++) {
        int j = 0;

        // Compare character by character
        while (j < prefix_len &&
               strs[i][j] != '\0' &&
               strs[0][j] == strs[i][j]) {
            j++;
        }

        // Update prefix length to minimum match
        prefix_len = j;

        // Early exit if no common prefix
        if (prefix_len == 0) {
            return "";
        }
    }

    // Create result string
    static char result[100];
    strncpy(result, strs[0], prefix_len);
    result[prefix_len] = '\0';

    return result;
}

// Alternative: Find prefix between two strings first
int common_prefix_len(char* s1, char* s2) {
    int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i]) {
        i++;
    }
    return i;
}

char* lcp_horizontal(char* strs[], int n) {
    if (n == 0) return "";

    int prefix_len = strlen(strs[0]);

    for (int i = 1; i < n; i++) {
        int len = common_prefix_len(strs[0], strs[i]);
        if (len < prefix_len) {
            prefix_len = len;
        }
        if (prefix_len == 0) return "";
    }

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
