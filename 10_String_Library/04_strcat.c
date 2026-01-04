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
// Say: "I'll implement strcat by first finding the end of dest, then copying src there"
char* my_strcat(char* dest, const char* src) {
    // Validate input pointers
    // Say: "First, I check that both pointers are valid"
    if (dest == NULL || src == NULL) return dest;

    // Save original pointer to return later
    // Say: "I save the original dest pointer to return it"
    char* ret = dest;

    // Find the end of dest (the null terminator)
    // Say: "Now I need to find where dest ends by looking for the null terminator"
    while (*dest != '\0') {
        dest++;     // Move forward until we find '\0'
    }

    // Now dest points to the null terminator of the original string
    // Copy src characters to the end of dest
    // Say: "Now I copy each character from src to the end of dest"
    while (*src != '\0') {
        *dest++ = *src++;   // Copy char and advance both pointers
    }

    // Add the null terminator to complete the concatenated string
    // Say: "Finally, I add the null terminator to end the combined string"
    *dest = '\0';

    // Return the original destination pointer
    // Say: "Return the original pointer to the beginning of dest"
    return ret;
}

// strncat - Concatenate at most n chars from src
// Say: "I'll implement strncat which limits how many characters are appended"
char* my_strncat(char* dest, const char* src, size_t n) {
    // Validate input pointers
    // Say: "Check for NULL pointers"
    if (dest == NULL || src == NULL) return dest;

    // Save original pointer to return later
    // Say: "Save the original dest pointer"
    char* ret = dest;

    // Find the end of dest
    // Say: "First, find where dest currently ends"
    while (*dest != '\0') {
        dest++;     // Move to the null terminator
    }

    // Copy at most n characters from src
    // Say: "Now I copy up to n characters from src, or until src ends"
    while (n > 0 && *src != '\0') {
        *dest++ = *src++;   // Copy character and advance
        n--;                // Decrement counter
    }

    // Always null-terminate (unlike strncpy!)
    // Say: "Important: strncat always adds a null terminator, unlike strncpy"
    *dest = '\0';

    // Return the original pointer
    // Say: "Return the original pointer"
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
