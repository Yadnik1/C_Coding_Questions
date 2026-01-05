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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why is strcpy considered dangerous?"
 * A1: No bounds checking! If dest buffer is smaller than src, it overflows
 *     into adjacent memory - classic buffer overflow vulnerability.
 *     This is the cause of countless security exploits. Use strncpy, strlcpy,
 *     or snprintf instead in production code.
 *
 * -------------------------------------------------------------------------
 * Q2: "What's the difference between strncpy and strlcpy?"
 * A2: strncpy: Pads with nulls if src < n, but does NOT null-terminate if
 *     src >= n. You must manually add '\0'. Also wastes cycles padding.
 *     strlcpy: Always null-terminates, returns total length needed (for
 *     truncation detection). Not standard C, but available on BSD/macOS.
 *
 * -------------------------------------------------------------------------
 * Q3: "Why does strcpy return char*?"
 * A3: Enables chaining: printf("%s", strcpy(dest, src)). Also allows checking
 *     if operation succeeded (returns NULL on error in safe implementations).
 *     The return is always the original dest pointer, not current position.
 *
 * -------------------------------------------------------------------------
 * Q4: "What happens if src and dest overlap?"
 * A4: Undefined behavior! If copying "hello" within same buffer where regions
 *     overlap, you get corrupted data. Use memmove for overlapping regions.
 *     strcpy is only safe when src and dest are completely separate buffers.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

// strcpy - Copy entire string
// Say: "I'll implement strcpy by copying each character from source to destination"
char* my_strcpy(char* dest, const char* src) {
    // Validate input pointers to avoid crashes
    // Say: "First, I check that both pointers are valid"
    if (dest == NULL || src == NULL) return NULL;

    // Save the original destination pointer to return later
    // Say: "I save the original dest pointer because I need to return it at the end"
    char* original = dest;

    // Copy each character until we hit the null terminator
    // Say: "Now I copy each character one by one from source to destination"
    while (*src != '\0') {
        *dest = *src;   // Copy the current character
        dest++;         // Move destination pointer forward
        src++;          // Move source pointer forward
    }

    // Don't forget to copy the null terminator!
    // Say: "Critical step - I add the null terminator to properly end the string"
    *dest = '\0';

    // Return the original destination pointer
    // Say: "Finally, I return the original pointer to the start of dest"
    return original;
}

// Compact one-liner version
// Say: "Here's a more compact version that combines the copy and advance in one line"
char* my_strcpy_compact(char* dest, const char* src) {
    // Validate input pointers
    // Say: "Same validation as before"
    if (dest == NULL || src == NULL) return NULL;

    // Save return pointer
    // Say: "Save the original dest pointer"
    char* ret = dest;

    // Copy and advance in a single expression - stops when it copies '\0'
    // Say: "This copies each character and advances both pointers, stopping after copying the null"
    while ((*dest++ = *src++) != '\0');

    // Return original pointer
    return ret;
}

// strncpy - Copy at most n characters
// Say: "Now I'll implement strncpy which is safer because it limits the number of characters copied"
char* my_strncpy(char* dest, const char* src, size_t n) {
    // Validate input pointers
    // Say: "First, check for NULL pointers"
    if (dest == NULL || src == NULL) return NULL;

    // Save original pointer to return later
    // Say: "Save the original dest pointer to return"
    char* ret = dest;
    size_t i;

    // Copy up to n characters or until we hit null in source
    // Say: "I copy characters up to n, or until I hit the source's null terminator"
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];   // Copy one character
    }

    // Important: Pad remaining space with null bytes if source was shorter than n
    // Say: "If the source was shorter than n, I pad the rest with null bytes"
    for (; i < n; i++) {
        dest[i] = '\0';     // Fill with null bytes
    }

    // Return original destination pointer
    // Say: "Return the original pointer"
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
