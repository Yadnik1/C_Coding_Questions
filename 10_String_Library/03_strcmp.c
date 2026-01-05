/*
 * ============================================================================
 * PROBLEM: Implement strcmp() and strncmp()
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: VERY HIGH
 *
 * strcmp  - Compare two strings
 * strncmp - Compare at most n characters
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. RETURN VALUE MEANINGS:
 *    - < 0 : str1 comes BEFORE str2 lexicographically (str1 < str2)
 *    - = 0 : strings are EQUAL
 *    - > 0 : str1 comes AFTER str2 lexicographically (str1 > str2)
 *
 * 2. ASCII VALUES:
 *    - 'a' = 97, 'z' = 122
 *    - 'A' = 65, 'Z' = 90
 *    - '0' = 48, '9' = 57
 *    - Characters are compared by their ASCII values
 *
 * 3. UNSIGNED CHAR CAST:
 *    - char can be signed (-128 to 127) on some systems
 *    - Extended ASCII (128-255) would compare incorrectly as negative
 *    - Cast to unsigned char to ensure correct comparison
 *
 * ============================================================================
 * VISUALIZATION: strcmp("apple", "apply")
 * ============================================================================
 *
 *   str1: ['a']['p']['p']['l']['e']['\0']
 *   str2: ['a']['p']['p']['l']['y']['\0']
 *
 *   Compare index 0: 'a' == 'a' ✓ continue
 *   Compare index 1: 'p' == 'p' ✓ continue
 *   Compare index 2: 'p' == 'p' ✓ continue
 *   Compare index 3: 'l' == 'l' ✓ continue
 *   Compare index 4: 'e' != 'y' ✗ STOP
 *
 *   Return: 'e' - 'y' = 101 - 121 = -20 (negative, str1 < str2)
 *
 * ============================================================================
 * STEP-BY-STEP ALGORITHM:
 * ============================================================================
 *
 *   while (*str1 != '\0' AND *str1 == *str2):
 *       str1++
 *       str2++
 *   return *str1 - *str2
 *
 *   Loop exits when:
 *   1. We hit end of str1 (found '\0')
 *   2. Characters differ (*str1 != *str2)
 *
 *   Then return the difference of current characters
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - n = length of shorter string (or first differing position)
 * - Best case O(1): first characters differ
 * - Worst case O(n): strings are equal
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only pointer variables, no extra storage
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why cast to unsigned char?"
 * A1: char can be signed (-128 to 127) on some platforms. Extended ASCII
 *     characters (128-255) would be negative, causing wrong comparison.
 *     unsigned char ensures all values 0-255 compare correctly.
 *
 * -------------------------------------------------------------------------
 * Q2: "What does strcmp actually return?"
 * A2: The difference of first differing chars, not just -1/0/1! Standard only
 *     guarantees <0, 0, or >0. Don't write code that depends on specific
 *     values like -1 or 1. Always use < 0, == 0, > 0 comparisons.
 *
 * -------------------------------------------------------------------------
 * Q3: "How do you do case-insensitive comparison?"
 * A3: Use strcasecmp (POSIX) or _stricmp (Windows). Or implement:
 *     Convert both chars to lowercase using tolower() before comparing.
 *     while (*s1 && tolower(*s1) == tolower(*s2)) { s1++; s2++; }
 *
 * -------------------------------------------------------------------------
 * Q4: "What about locale-aware string comparison?"
 * A4: strcmp does byte comparison, not locale-aware. For proper locale
 *     handling (accents, umlauts), use strcoll() which considers locale's
 *     collation rules. Critical for internationalized applications.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

/*
 * ============================================================================
 * STRCMP FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * int my_strcmp(const char* str1, const char* str2):
 *   - Returns "int" = difference between first differing characters
 *   - "const char*" = pointers to strings we won't modify
 *
 * while (*str1 != '\0' && *str1 == *str2):
 *   - Continue while: str1 hasn't ended AND characters match
 *   - *str1 != '\0' : we haven't hit end of str1
 *   - *str1 == *str2 : current characters are equal
 *
 * str1++; str2++:
 *   - Move both pointers to next character
 *   - Check next pair of characters
 *
 * return (unsigned char)*str1 - (unsigned char)*str2:
 *   - Dereference to get current characters
 *   - Cast to unsigned char for correct comparison
 *   - Return the DIFFERENCE (not just -1/0/1)
 *
 * WHY THIS WORKS:
 *   - If strings are equal: both reach '\0' simultaneously
 *     Return: '\0' - '\0' = 0
 *   - If str1 shorter: str1 hits '\0' first
 *     Return: '\0' - char = negative
 *   - If str2 shorter: str2 hits '\0' when we check difference
 *     Return: char - '\0' = positive
 *   - If chars differ: return their difference
 *
 * ============================================================================
 */
// strcmp - Compare entire strings
// Say: "I'll implement strcmp by comparing characters until I find a difference or reach the end"
int my_strcmp(const char* str1, const char* str2) {
    // Handle NULL pointers to avoid crashes
    // Say: "First, I handle the NULL cases"
    // WHY: Dereferencing NULL causes segmentation fault
    if (str1 == NULL || str2 == NULL) {
        // Both NULL = equal
        if (str1 == str2) return 0;
        // One NULL = NULL is "less than" non-NULL
        return str1 == NULL ? -1 : 1;
    }

    // Keep comparing while characters match AND we haven't hit the end
    // Say: "I advance through both strings as long as the characters match and we haven't hit a null"
    // WHY: Loop until we find a difference or reach end of str1
    while (*str1 != '\0' && *str1 == *str2) {
        // Move to next character in str1
        // Say: "I move to the next character in both strings"
        str1++;
        // Move to next character in str2
        str2++;
    }

    // Return the difference of the first non-matching characters
    // Say: "I return the difference between the first differing characters, cast to unsigned char"
    // WHY: Cast to unsigned char to handle extended ASCII (values 128-255) correctly
    // Note: Without cast, char values > 127 could be negative on some systems
    return (unsigned char)*str1 - (unsigned char)*str2;
}

/*
 * ============================================================================
 * STRNCMP FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * int my_strncmp(const char* str1, const char* str2, size_t n):
 *   - "size_t n" = maximum number of characters to compare
 *   - Compare at most n characters
 *
 * while (n > 0 && *str1 != '\0' && *str1 == *str2):
 *   - Additional condition: n > 0 (haven't compared n chars yet)
 *   - Same logic as strcmp otherwise
 *
 * if (n == 0) return 0:
 *   - If we compared n characters and all matched
 *   - Return 0 (equal within the limit)
 *
 * USE CASE: Check string prefixes
 *   strncmp("hello", "help", 3) = 0 (first 3 chars match)
 *   strncmp("hello", "help", 4) < 0 ('l' < 'p')
 *
 * ============================================================================
 */
// strncmp - Compare at most n characters
// Say: "Now I'll implement strncmp which only compares up to n characters"
int my_strncmp(const char* str1, const char* str2, size_t n) {
    // Handle edge cases: NULL pointers or n=0
    // Say: "Check for NULL or if n is zero, in which case we consider them equal"
    // WHY: Comparing 0 characters means they're "equal" within that limit
    if (str1 == NULL || str2 == NULL || n == 0) {
        return 0;
    }

    // Compare while we have characters left to check AND they match
    // Say: "I compare characters while n is positive, and the characters match"
    // WHY: Three conditions - n not exhausted, not at end of str1, chars match
    while (n > 0 && *str1 != '\0' && *str1 == *str2) {
        // Move to next character
        // Say: "I move to the next character"
        str1++;
        str2++;
        // Decrement remaining count
        // Say: "I decrement the count of remaining characters to check"
        n--;
    }

    // If we exhausted our n characters, they matched within the limit
    // Say: "If n reached zero, the first n characters matched"
    // WHY: We compared all n characters successfully
    if (n == 0) {
        return 0;  // First n chars matched
    }

    // Return the difference of the characters at this position
    // Say: "Otherwise, return the difference of the current characters"
    // WHY: Either we found a difference or hit end of str1
    return (unsigned char)*str1 - (unsigned char)*str2;
}

int main() {
    printf("=== strcmp / strncmp Implementation ===\n\n");

    // strcmp tests
    printf("1. strcmp:\n");
    printf("   strcmp(\"abc\", \"abc\") = %d (equal)\n", my_strcmp("abc", "abc"));
    printf("   strcmp(\"abc\", \"abd\") = %d (negative: c < d)\n", my_strcmp("abc", "abd"));
    printf("   strcmp(\"abd\", \"abc\") = %d (positive: d > c)\n", my_strcmp("abd", "abc"));
    printf("   strcmp(\"ab\", \"abc\")  = %d (negative: shorter)\n", my_strcmp("ab", "abc"));
    printf("   strcmp(\"\", \"\")       = %d (empty strings equal)\n\n", my_strcmp("", ""));

    // strncmp tests
    printf("2. strncmp:\n");
    printf("   strncmp(\"hello\", \"help\", 3) = %d (first 3 match)\n",
           my_strncmp("hello", "help", 3));
    printf("   strncmp(\"hello\", \"help\", 4) = %d (4th differs: l < p)\n",
           my_strncmp("hello", "help", 4));
    printf("   strncmp(\"abc\", \"abd\", 2)    = %d (first 2 match)\n\n",
           my_strncmp("abc", "abd", 2));

    printf("=== Key Points ===\n");
    printf("- Returns difference of first non-matching chars\n");
    printf("- Use unsigned char to handle extended ASCII\n");
    printf("- 0 means strings are equal\n");
    printf("- Don't rely on -1/0/1 - use < 0, == 0, > 0\n");
    printf("- strncmp useful for prefix matching\n");

    return 0;
}
