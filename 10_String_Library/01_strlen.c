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

// Array indexing version - simpler to explain
// Say: "I'll implement strlen by counting characters until the null terminator"
size_t my_strlen(const char* str) {
    // Check for NULL pointer to avoid dereferencing invalid memory
    // Say: "First, I check if the pointer is NULL and return 0 if it is"
    if (str == NULL) return 0;

    // Initialize length counter to zero
    // Say: "I start with a counter at zero to track the string length"
    size_t len = 0;

    // Traverse the string until we hit the null terminator
    // Say: "Now I loop through the string, incrementing the counter for each character"
    while (str[len] != '\0') {
        len++;      // Found a character, increment count
    }

    // Return the total count (excludes null terminator)
    // Say: "Finally, I return the length which doesn't include the null terminator"
    return len;
}

// Pointer version (more common in interviews)
// Say: "I can also implement this using pointer arithmetic instead of array indexing"
size_t my_strlen_ptr(const char* str) {
    // Check for NULL pointer
    // Say: "Again, I check for NULL first"
    if (str == NULL) return 0;

    // Save the starting address so we can calculate the difference later
    // Say: "I save the original pointer so I can compute the length using pointer arithmetic"
    const char* p = str;

    // Walk the pointer forward until we find null terminator
    // Say: "I advance the pointer through each character until I hit the null"
    while (*p != '\0') {
        p++;        // Move to next character
    }

    // Calculate length by subtracting start address from end address
    // Say: "The length is the difference between the final and initial pointer positions"
    return p - str;  // Pointer arithmetic gives us the length
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
