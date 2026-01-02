/*
 * ============================================================================
 * PROBLEM: Implement strlen()
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: VERY HIGH
 *
 * strlen returns the length of a string (not counting null terminator)
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   str:  ['H']['e']['l']['l']['o']['\0']
 *   idx:    0    1    2    3    4    5
 *                                    ^
 *                               null terminator
 *
 *   Returns: 5 (doesn't count '\0')
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

// Array indexing version
size_t my_strlen(const char* str) {
    if (str == NULL) return 0;

    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Pointer version (more common in interviews)
size_t my_strlen_ptr(const char* str) {
    if (str == NULL) return 0;

    const char* p = str;
    while (*p != '\0') {
        p++;
    }
    return p - str;  // Pointer arithmetic
}

int main() {
    printf("=== strlen Implementation ===\n\n");

    printf("strlen(\"Hello\")     = %zu\n", my_strlen("Hello"));
    printf("strlen(\"\")          = %zu\n", my_strlen(""));
    printf("strlen(\"A\")         = %zu\n", my_strlen("A"));
    printf("strlen(\"Hello World\") = %zu\n", my_strlen("Hello World"));

    printf("\n--- Pointer version ---\n");
    printf("strlen_ptr(\"Hello\") = %zu\n", my_strlen_ptr("Hello"));

    return 0;
}
