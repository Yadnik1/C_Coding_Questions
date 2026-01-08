/*
 * ============================================================================
 * PROBLEM: Implement strcmp() - String Compare Function
 * ============================================================================
 *
 * WHAT IS strcmp()?
 * strcmp(str1, str2) compares two strings lexicographically (dictionary order)
 * and returns:
 *   - Negative value: if str1 < str2 (str1 comes before str2)
 *   - Zero:           if str1 == str2 (strings are identical)
 *   - Positive value: if str1 > str2 (str1 comes after str2)
 *
 * FUNCTION SIGNATURE:
 * int strcmp(const char *str1, const char *str2);
 *
 * EXAMPLES:
 * - strcmp("abc", "abc") -> 0   (equal)
 * - strcmp("abc", "abd") -> -1  (negative, 'c' < 'd')
 * - strcmp("abd", "abc") -> 1   (positive, 'd' > 'c')
 * - strcmp("ab", "abc")  -> -1  (negative, shorter is "less")
 * - strcmp("ABC", "abc") -> -32 (negative, 'A'=65 < 'a'=97)
 *
 * WHAT IS LEXICOGRAPHIC ORDER?
 * Like dictionary order: compare character by character from left to right.
 * First difference determines the result.
 * "apple" < "banana" because 'a' < 'b' at first position.
 * "app" < "apple" because shorter string is "less" when prefix matches.
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Fundamental string operation
 * - Tests understanding of ASCII values and character comparison
 * - Important for sorting, searching strings
 * - Shows understanding of return value conventions
 *
 * ASCII REMINDER:
 * - 'A' = 65, 'B' = 66, ..., 'Z' = 90
 * - 'a' = 97, 'b' = 98, ..., 'z' = 122
 * - '0' = 48, '1' = 49, ..., '9' = 57
 * - Uppercase letters have LOWER ASCII values than lowercase!
 *
 * VISUAL EXAMPLE:
 * strcmp("cat", "car"):
 *   'c' == 'c' -> continue
 *   'a' == 'a' -> continue
 *   't' vs 'r' -> 't'(116) - 'r'(114) = 2 (positive) -> "cat" > "car"
 *
 * ============================================================================
 */

// Time: O(n), Space: O(1)

#include <stdio.h>

int my_strcmp(const char *str1, const char *str2) {
    // Say: "I'll compare character by character until a difference or end"
    // Say: "Return value follows strcmp convention:"
    // Say: "  < 0 if str1 < str2"
    // Say: "  = 0 if str1 == str2"
    // Say: "  > 0 if str1 > str2"

    // Say: "Handle null pointers"
    if (str1 == NULL && str2 == NULL) return 0;
    if (str1 == NULL) return -1;
    if (str2 == NULL) return 1;

    // Say: "Compare while both strings have characters"
    while (*str1 != '\0' && *str2 != '\0') {
        // Say: "If characters differ, return the difference"
        if (*str1 != *str2) {
            // Say: "Positive if str1 char > str2 char, negative otherwise"
            return (unsigned char)*str1 - (unsigned char)*str2;
            // Say: "Cast to unsigned char to handle extended ASCII correctly"
        }

        // Say: "Characters match, move to next"
        str1++;
        str2++;
    }

    // Say: "One or both strings ended"
    // Say: "If both ended (both are '\0'), they're equal, return 0"
    // Say: "If only one ended, the longer string is 'greater'"
    return (unsigned char)*str1 - (unsigned char)*str2;
}

// Alternative: Return -1, 0, 1 only (some implementations do this)
int my_strcmp_normalized(const char *str1, const char *str2) {
    // Say: "Same logic but normalize return to -1, 0, or 1"

    if (str1 == NULL && str2 == NULL) return 0;
    if (str1 == NULL) return -1;
    if (str2 == NULL) return 1;

    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 < *str2) return -1;
        if (*str1 > *str2) return 1;
        str1++;
        str2++;
    }

    // Say: "At this point, at least one string has ended"
    if (*str1 == '\0' && *str2 == '\0') return 0;   // Say: "Both ended - equal"
    if (*str1 == '\0') return -1;                    // Say: "str1 ended first - str1 < str2"
    return 1;                                         // Say: "str2 ended first - str1 > str2"
}

// Compact version
int my_strcmp_compact(const char *str1, const char *str2) {
    // Say: "Compact version - loops while chars are equal and not null"
    while (*str1 && *str1 == *str2) {
        str1++;
        str2++;
    }
    // Say: "Return difference of first differing chars (or null terminators)"
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

int main() {
    // Test equal strings
    printf("Compare 'hello' vs 'hello': %d\n", my_strcmp("hello", "hello"));  // Expected: 0

    // Test str1 < str2
    printf("Compare 'abc' vs 'abd': %d\n", my_strcmp("abc", "abd"));  // Expected: negative

    // Test str1 > str2
    printf("Compare 'abd' vs 'abc': %d\n", my_strcmp("abd", "abc"));  // Expected: positive

    // Test different lengths
    printf("Compare 'ab' vs 'abc': %d\n", my_strcmp("ab", "abc"));    // Expected: negative
    printf("Compare 'abc' vs 'ab': %d\n", my_strcmp("abc", "ab"));    // Expected: positive

    // Test empty string
    printf("Compare '' vs 'a': %d\n", my_strcmp("", "a"));            // Expected: negative
    printf("Compare 'a' vs '': %d\n", my_strcmp("a", ""));            // Expected: positive
    printf("Compare '' vs '': %d\n", my_strcmp("", ""));              // Expected: 0

    // Test case sensitivity
    printf("Compare 'ABC' vs 'abc': %d\n", my_strcmp("ABC", "abc"));  // Expected: negative
    // Say: "'A' is 65, 'a' is 97 in ASCII, so 'ABC' < 'abc'"

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I'll compare strings character by character, returning their relationship.

 APPROACH:
 - Iterate through both strings simultaneously
 - If characters differ, return difference (str1[i] - str2[i])
 - If one string ends before the other, the shorter one is 'less'
 - If both end at same position, they're equal

 RETURN VALUE CONVENTION:
 - Negative: str1 comes before str2 (str1 < str2)
 - Zero: strings are identical
 - Positive: str1 comes after str2 (str1 > str2)
 - The actual magnitude is the ASCII difference

 WHY UNSIGNED CHAR CAST:
 - Plain char might be signed (-128 to 127) on some systems
 - Extended ASCII (128-255) could give wrong comparison results
 - Casting to unsigned char ensures correct ordering

 KEY OBSERVATIONS:
 - 'a' (97) > 'A' (65) - lowercase comes AFTER uppercase
 - 'abc' > 'ab' - longer string is 'greater' if prefix matches
 - Comparison is lexicographic (dictionary order)

 EDGE CASES:
 - Empty strings: '' vs '' returns 0
 - One empty: '' < any non-empty string
 - Null pointers: handle specially

 VARIATIONS:
 - strcasecmp: case-insensitive comparison
 - strncmp: compare only first n characters
 - memcmp: compare raw bytes (doesn't stop at null)

 COMPLEXITY:
 - Time: O(n) where n is length of shorter string
 - Space: O(1) - just pointer variables"
*/
