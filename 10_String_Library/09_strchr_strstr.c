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
 */

#include <stdio.h>
#include <stddef.h>

// strchr - Find character in string
char* my_strchr(const char* str, int c) {
    if (str == NULL) return NULL;

    while (*str != '\0') {
        if (*str == (char)c) {
            return (char*)str;
        }
        str++;
    }

    // Check if searching for null terminator
    if ((char)c == '\0') {
        return (char*)str;
    }

    return NULL;  // Not found
}

// strrchr - Find LAST occurrence of character
char* my_strrchr(const char* str, int c) {
    if (str == NULL) return NULL;

    const char* last = NULL;

    while (*str != '\0') {
        if (*str == (char)c) {
            last = str;
        }
        str++;
    }

    // Check for null terminator
    if ((char)c == '\0') {
        return (char*)str;
    }

    return (char*)last;
}

// strstr - Find substring in string
char* my_strstr(const char* haystack, const char* needle) {
    if (haystack == NULL || needle == NULL) return NULL;

    // Empty needle matches at start
    if (*needle == '\0') {
        return (char*)haystack;
    }

    while (*haystack != '\0') {
        const char* h = haystack;
        const char* n = needle;

        // Try to match needle at current position
        while (*h == *n && *n != '\0') {
            h++;
            n++;
        }

        // If we reached end of needle, it's a match
        if (*n == '\0') {
            return (char*)haystack;
        }

        haystack++;
    }

    return NULL;  // Not found
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
