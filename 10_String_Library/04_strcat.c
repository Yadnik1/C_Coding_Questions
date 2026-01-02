/*
 * ============================================================================
 * PROBLEM: Implement strcat() and strncat()
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: HIGH
 *
 * strcat  - Concatenate strings
 * strncat - Concatenate at most n characters
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   dest: ['H']['i']['\0'][?][?][?][?][?]
 *   src:  ['!']['!']['\0']
 *
 *   After strcat(dest, src):
 *   dest: ['H']['i']['!']['!']['\0'][?][?][?]
 *
 * ============================================================================
 * WARNING: dest must have enough space for both strings!
 * ============================================================================
 *
 * TIME: O(n + m) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

// strcat - Concatenate src to end of dest
char* my_strcat(char* dest, const char* src) {
    if (dest == NULL || src == NULL) return dest;

    char* ret = dest;

    // Find end of dest
    while (*dest != '\0') {
        dest++;
    }

    // Copy src to end of dest
    while (*src != '\0') {
        *dest++ = *src++;
    }
    *dest = '\0';

    return ret;
}

// strncat - Concatenate at most n chars from src
char* my_strncat(char* dest, const char* src, size_t n) {
    if (dest == NULL || src == NULL) return dest;

    char* ret = dest;

    // Find end of dest
    while (*dest != '\0') {
        dest++;
    }

    // Copy at most n chars from src
    while (n > 0 && *src != '\0') {
        *dest++ = *src++;
        n--;
    }
    *dest = '\0';  // Always null-terminates (unlike strncpy!)

    return ret;
}

int main() {
    printf("=== strcat / strncat Implementation ===\n\n");

    // strcat
    printf("1. strcat:\n");
    char dest1[20] = "Hello";
    printf("   Before: \"%s\"\n", dest1);
    my_strcat(dest1, " World");
    printf("   strcat(dest, \" World\") → \"%s\"\n\n", dest1);

    // Multiple concatenations
    printf("2. Multiple strcat:\n");
    char dest2[30] = "";
    my_strcat(dest2, "One");
    my_strcat(dest2, " + ");
    my_strcat(dest2, "Two");
    printf("   Result: \"%s\"\n\n", dest2);

    // strncat
    printf("3. strncat:\n");
    char dest3[20] = "Hello";
    my_strncat(dest3, " World!!!", 6);  // Only append 6 chars
    printf("   strncat(\"Hello\", \" World!!!\", 6) → \"%s\"\n\n", dest3);

    printf("=== Key Points ===\n");
    printf("- Find end of dest first, then copy\n");
    printf("- strcat: No bounds check (dangerous)\n");
    printf("- strncat: Limits chars BUT always null-terminates\n");
    printf("- Ensure dest has enough space!\n");

    return 0;
}
