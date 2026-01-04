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
 * RETURN VALUES:
 * ============================================================================
 *
 *   < 0 : str1 < str2 (first differing char is smaller in str1)
 *   = 0 : str1 == str2
 *   > 0 : str1 > str2 (first differing char is larger in str1)
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   str1: "apple"
 *   str2: "apply"
 *
 *   Compare: a==a, p==p, p==p, l==l, e!=y
 *   'e' (101) < 'y' (121)
 *   Returns: negative value (-20)
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

// strcmp - Compare entire strings
// Say: "I'll implement strcmp by comparing characters until I find a difference or reach the end"
int my_strcmp(const char* str1, const char* str2) {
    // Handle NULL pointers to avoid crashes
    // Say: "First, I handle the NULL cases"
    if (str1 == NULL || str2 == NULL) {
        if (str1 == str2) return 0;         // Both NULL, consider equal
        return str1 == NULL ? -1 : 1;       // One NULL, it's "less than"
    }

    // Keep comparing while characters match AND we haven't hit the end
    // Say: "I advance through both strings as long as the characters match and we haven't hit a null"
    while (*str1 != '\0' && *str1 == *str2) {
        str1++;     // Move to next character in str1
        str2++;     // Move to next character in str2
    }

    // Return the difference of the first non-matching characters
    // Say: "I return the difference between the first differing characters, cast to unsigned char"
    // Note: Cast to unsigned char to handle extended ASCII properly
    return (unsigned char)*str1 - (unsigned char)*str2;
}

// strncmp - Compare at most n characters
// Say: "Now I'll implement strncmp which only compares up to n characters"
int my_strncmp(const char* str1, const char* str2, size_t n) {
    // Handle edge cases: NULL pointers or n=0
    // Say: "Check for NULL or if n is zero, in which case we consider them equal"
    if (str1 == NULL || str2 == NULL || n == 0) {
        return 0;
    }

    // Compare while we have characters left to check AND they match
    // Say: "I compare characters while n is positive, and the characters match"
    while (n > 0 && *str1 != '\0' && *str1 == *str2) {
        str1++;     // Move to next character
        str2++;     // Move to next character
        n--;        // Decrement remaining count
    }

    // If we exhausted our n characters, they matched within the limit
    // Say: "If n reached zero, the first n characters matched"
    if (n == 0) {
        return 0;  // First n chars matched
    }

    // Return the difference of the characters at this position
    // Say: "Otherwise, return the difference of the current characters"
    return (unsigned char)*str1 - (unsigned char)*str2;
}

int main() {
    printf("=== strcmp / strncmp Implementation ===\n\n");

    // strcmp tests
    printf("1. strcmp:\n");
    printf("   strcmp(\"abc\", \"abc\") = %d (equal)\n", my_strcmp("abc", "abc"));
    printf("   strcmp(\"abc\", \"abd\") = %d (negative)\n", my_strcmp("abc", "abd"));
    printf("   strcmp(\"abd\", \"abc\") = %d (positive)\n", my_strcmp("abd", "abc"));
    printf("   strcmp(\"ab\", \"abc\")  = %d (shorter)\n", my_strcmp("ab", "abc"));
    printf("   strcmp(\"\", \"\")       = %d (empty)\n\n", my_strcmp("", ""));

    // strncmp tests
    printf("2. strncmp:\n");
    printf("   strncmp(\"hello\", \"help\", 3) = %d (first 3 match)\n",
           my_strncmp("hello", "help", 3));
    printf("   strncmp(\"hello\", \"help\", 4) = %d (4th differs)\n",
           my_strncmp("hello", "help", 4));
    printf("   strncmp(\"abc\", \"abd\", 2)    = %d (first 2 match)\n\n",
           my_strncmp("abc", "abd", 2));

    printf("=== Key Points ===\n");
    printf("- Returns difference of first non-matching chars\n");
    printf("- Use unsigned char to handle extended ASCII\n");
    printf("- 0 means strings are equal\n");

    return 0;
}
