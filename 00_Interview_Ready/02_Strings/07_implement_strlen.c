/*
 * ============================================================================
 * PROBLEM: Implement strlen() - String Length Function
 * ============================================================================
 *
 * WHAT IS strlen()?
 * strlen() is a standard C library function that returns the LENGTH of a
 * string (number of characters) NOT including the null terminator '\0'.
 *
 * HOW C STRINGS WORK:
 * In C, strings are arrays of characters ending with a null terminator '\0'.
 * The string "hello" is stored as: ['h', 'e', 'l', 'l', 'o', '\0']
 * strlen("hello") = 5 (counts 5 characters, ignores the '\0')
 *
 * EXAMPLES:
 * - strlen("hello")     -> 5
 * - strlen("")          -> 0  (empty string, first char is '\0')
 * - strlen("a")         -> 1
 * - strlen("embedded")  -> 8
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Most fundamental string operation
 * - Tests understanding of null-terminated strings (C-specific concept!)
 * - Shows knowledge of pointer arithmetic
 * - Foundation for implementing other string functions (strcpy, strcat, etc.)
 * - Critical for embedded: buffer overflow prevention
 *
 * WHAT IS THE NULL TERMINATOR '\0'?
 * - Special character with ASCII value 0
 * - Marks the end of every C string
 * - Without it, we wouldn't know where the string ends!
 * - '\0' is different from '0' (which is ASCII 48)
 *
 * MEMORY LAYOUT:
 *   "hello" in memory:
 *   +---+---+---+---+---+----+
 *   | h | e | l | l | o | \0 |
 *   +---+---+---+---+---+----+
 *     0   1   2   3   4   5    <- indices
 *                         ^
 *                    NULL terminator (strlen stops here, doesn't count it)
 *
 * TWO APPROACHES:
 * 1. Index-based: Use a counter as array index
 * 2. Pointer-based: Use pointer arithmetic (end - start)
 *
 * ============================================================================
 */

// Time: O(n), Space: O(1)

#include <stdio.h>

int my_strlen(const char *str) {
    // Say: "I'll iterate through string until I hit the null terminator"
    // Say: "Using const char* since we're not modifying the string"

    // Say: "Handle null pointer gracefully"
    if (str == NULL) {
        return 0;
    }

    int length = 0;  // Say: "Counter for characters"

    // Say: "Loop until we reach null terminator '\0'"
    // Say: "Null terminator has ASCII value 0, so we check str[length] != 0"
    while (str[length] != '\0') {
        length++;  // Say: "Count this character and move to next"
    }

    return length;  // Say: "Return count, not including null terminator"
}

// Alternative: Using pointer arithmetic
int my_strlen_pointer(const char *str) {
    // Say: "Same logic but using pointer instead of index"

    if (str == NULL) {
        return 0;
    }

    const char *ptr = str;  // Say: "Start pointer at beginning"

    // Say: "Move pointer until it points to null terminator"
    while (*ptr != '\0') {
        ptr++;  // Say: "Advance pointer to next character"
    }

    // Say: "Pointer difference gives length"
    return ptr - str;  // Say: "End position minus start position = length"
}

// Most compact version (common in production code)
int my_strlen_compact(const char *str) {
    // Say: "Compact version - *ptr evaluates to 0 (false) at null terminator"
    const char *ptr = str;
    while (*ptr) ptr++;  // Say: "*ptr is truthy for any non-null char"
    return ptr - str;
}

int main() {
    const char *test1 = "hello";
    printf("String: '%s'\n", test1);
    printf("my_strlen: %d\n", my_strlen(test1));          // Expected: 5
    printf("my_strlen_pointer: %d\n", my_strlen_pointer(test1));  // Expected: 5

    const char *test2 = "";
    printf("\nString: '%s' (empty)\n", test2);
    printf("my_strlen: %d\n", my_strlen(test2));          // Expected: 0

    const char *test3 = "embedded systems";
    printf("\nString: '%s'\n", test3);
    printf("my_strlen: %d\n", my_strlen(test3));          // Expected: 16

    const char *test4 = "a";
    printf("\nString: '%s'\n", test4);
    printf("my_strlen: %d\n", my_strlen(test4));          // Expected: 1

    // Test null pointer
    printf("\nNull pointer: my_strlen(NULL) = %d\n", my_strlen(NULL));  // Expected: 0

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I'll count characters by iterating until I reach the null terminator.

 APPROACH:
 - Start with length counter at 0
 - Loop through string character by character
 - Stop when we hit '\0' (null terminator, ASCII value 0)
 - Return the counter value

 KEY POINTS ABOUT C STRINGS:
 - C strings are null-terminated character arrays
 - The null terminator '\0' marks the end of the string
 - Length does NOT include the null terminator
 - String 'hello' is actually stored as: {'h','e','l','l','o','\0'}

 TWO IMPLEMENTATION STYLES:
 1. Index-based: Use counter as array index, increment until null
 2. Pointer-based: Move pointer until it points to null, return difference
    - Pointer arithmetic: (end - start) gives number of elements between

 EDGE CASES:
 - Empty string '': first char is '\0', loop doesn't execute, returns 0
 - Single char 'a': loops once, returns 1
 - Null pointer: should return 0 or handle specially

 WHY const char*:
 - We're only reading the string, not modifying
 - const documents intent and catches accidental modifications
 - Allows passing string literals directly

 COMPLEXITY:
 - Time: O(n) - must visit every character once
 - Space: O(1) - only a counter variable (or pointer)

 INTERVIEW TIP:
 Show both implementations to demonstrate understanding of
 both array indexing and pointer arithmetic in C."
*/
