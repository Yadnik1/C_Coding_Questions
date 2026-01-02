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
int my_strcmp(const char* str1, const char* str2) {
    if (str1 == NULL || str2 == NULL) {
        if (str1 == str2) return 0;
        return str1 == NULL ? -1 : 1;
    }

    while (*str1 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }

    return (unsigned char)*str1 - (unsigned char)*str2;
}

// strncmp - Compare at most n characters
int my_strncmp(const char* str1, const char* str2, size_t n) {
    if (str1 == NULL || str2 == NULL || n == 0) {
        return 0;
    }

    while (n > 0 && *str1 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
        n--;
    }

    if (n == 0) {
        return 0;  // First n chars matched
    }

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
