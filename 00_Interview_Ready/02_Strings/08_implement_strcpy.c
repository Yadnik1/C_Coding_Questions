// Implement strcpy - copy string character by character
// Time: O(n), Space: O(1) - no extra space besides destination

#include <stdio.h>

char* my_strcpy(char *dest, const char *src) {
    // Say: "I'll copy each character from source to destination"
    // Say: "Including the null terminator at the end"

    // Say: "Save original destination pointer to return"
    // Say: "This matches standard strcpy behavior - returns dest pointer"
    char *original = dest;

    // Say: "Handle null pointers to avoid crashes"
    if (dest == NULL || src == NULL) {
        return dest;
    }

    // Say: "Copy character by character until null terminator"
    while (*src != '\0') {
        *dest = *src;  // Say: "Copy current character"
        dest++;        // Say: "Move destination pointer forward"
        src++;         // Say: "Move source pointer forward"
    }

    // Say: "Don't forget to copy the null terminator!"
    // Say: "This is crucial - without it, copied string has no end"
    *dest = '\0';

    return original;  // Say: "Return pointer to start of destination"
}

// Compact version using post-increment in assignment
char* my_strcpy_compact(char *dest, const char *src) {
    // Say: "More compact but same logic"
    char *original = dest;

    if (dest == NULL || src == NULL) {
        return dest;
    }

    // Say: "*dest++ = *src++ copies char, then advances both pointers"
    // Say: "Loop continues while copied char is not null"
    while ((*dest++ = *src++) != '\0');
    // Say: "When '\0' is copied, expression evaluates to 0 (false), loop ends"
    // Say: "Null terminator gets copied as part of the loop condition!"

    return original;
}

// Safe version with size limit (like strncpy but ensures null termination)
char* my_strcpy_safe(char *dest, const char *src, size_t size) {
    // Say: "Safer version - prevents buffer overflow"
    // Say: "Always null-terminates unlike strncpy"

    if (dest == NULL || src == NULL || size == 0) {
        return dest;
    }

    char *original = dest;
    size_t i;

    // Say: "Copy up to size-1 characters, leaving room for null terminator"
    for (i = 0; i < size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    // Say: "Always null terminate"
    dest[i] = '\0';

    return original;
}

int main() {
    char dest1[20];
    char *src1 = "Hello, World!";

    my_strcpy(dest1, src1);
    printf("Source: '%s'\n", src1);
    printf("Copied: '%s'\n", dest1);  // Expected: "Hello, World!"

    char dest2[20];
    my_strcpy_compact(dest2, "embedded");
    printf("\nCompact copy: '%s'\n", dest2);  // Expected: "embedded"

    // Test safe copy
    char small_buf[5];
    my_strcpy_safe(small_buf, "Hello, World!", sizeof(small_buf));
    printf("\nSafe copy to small buffer: '%s'\n", small_buf);  // Expected: "Hell"

    // Test empty string
    char dest3[10];
    my_strcpy(dest3, "");
    printf("\nCopy empty string: '%s' (length should be 0)\n", dest3);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I'll copy the string character by character, including the null terminator.

 APPROACH:
 - Save the original destination pointer (to return it later)
 - Loop through source string, copying each character to destination
 - Stop when we reach null terminator in source
 - Copy the null terminator to destination
 - Return original destination pointer

 WHY RETURN DESTINATION:
 - Matches standard strcpy behavior
 - Allows chaining: printf('%s', strcpy(buf, 'hello'))
 - Returns the same pointer passed in

 TWO STYLES:
 1. Explicit: Separate copy, then null terminate
 2. Compact: while((*dest++ = *src++)) copies and advances in one step
    - When '\0' is copied, expression is 0 (false), loop ends
    - Null terminator is already copied by the time loop ends

 IMPORTANT NOTES:
 - Caller must ensure destination has enough space
 - No bounds checking in standard strcpy - can cause buffer overflow
 - strncpy is safer but has quirks (may not null-terminate)
 - My safe version always null-terminates

 EDGE CASES:
 - Empty source: copies just null terminator
 - Null pointers: return early to prevent crash
 - Source = destination: works but undefined behavior in standard

 COMMON BUGS TO AVOID:
 - Forgetting to copy null terminator
 - Not saving original dest pointer before incrementing
 - Buffer overflow if dest too small

 COMPLEXITY:
 - Time: O(n) where n is length of source string
 - Space: O(1) - no extra space used"
*/
