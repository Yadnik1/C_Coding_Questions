/*
 * ============================================================================
 * PROBLEM: Implement strcpy() and strncpy()
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: VERY HIGH
 *
 * strcpy  - Copy string (UNSAFE - no bounds check)
 * strncpy - Copy at most n characters (SAFER)
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   src:  ['H']['e']['l']['l']['o']['\0']
 *   dest: ['?']['?']['?']['?']['?']['?']
 *
 *   After strcpy:
 *   dest: ['H']['e']['l']['l']['o']['\0']
 *
 * ============================================================================
 * WARNING: strcpy has no bounds checking!
 *          dest must be large enough for src + null terminator
 * ============================================================================
 *
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

// strcpy - Copy entire string
char* my_strcpy(char* dest, const char* src) {
    if (dest == NULL || src == NULL) return NULL;

    char* original = dest;

    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';  // Don't forget null terminator!

    return original;
}

// Compact one-liner version
char* my_strcpy_compact(char* dest, const char* src) {
    if (dest == NULL || src == NULL) return NULL;

    char* ret = dest;
    while ((*dest++ = *src++) != '\0');
    return ret;
}

// strncpy - Copy at most n characters
char* my_strncpy(char* dest, const char* src, size_t n) {
    if (dest == NULL || src == NULL) return NULL;

    char* ret = dest;
    size_t i;

    // Copy up to n chars or until '\0'
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    // Pad with '\0' if src was shorter than n
    for (; i < n; i++) {
        dest[i] = '\0';
    }

    return ret;
}

int main() {
    printf("=== strcpy / strncpy Implementation ===\n\n");

    char dest[20];

    // strcpy
    printf("1. strcpy:\n");
    my_strcpy(dest, "Hello");
    printf("   strcpy(dest, \"Hello\") → \"%s\"\n\n", dest);

    // strncpy - src shorter than n
    printf("2. strncpy (src shorter than n):\n");
    char dest2[10] = "XXXXXXXXX";
    my_strncpy(dest2, "Hi", 5);
    printf("   strncpy(dest, \"Hi\", 5) → \"%s\"\n", dest2);
    printf("   (padded with nulls)\n\n");

    // strncpy - src longer than n
    printf("3. strncpy (src longer than n):\n");
    char dest3[10];
    my_strncpy(dest3, "Hello World", 5);
    dest3[5] = '\0';  // Must manually null-terminate!
    printf("   strncpy(dest, \"Hello World\", 5) → \"%s\"\n", dest3);
    printf("   WARNING: May not be null-terminated!\n\n");

    printf("=== Key Points ===\n");
    printf("- strcpy: No bounds check, can overflow\n");
    printf("- strncpy: Safer, but may not null-terminate\n");
    printf("- Always ensure dest is large enough\n");

    return 0;
}
