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
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. STRINGS IN C:
 *    - Array of characters ending with '\0' (null terminator)
 *    - '\0' has ASCII value 0
 *    - "Hello" is actually 6 bytes: H, e, l, l, o, \0
 *
 * 2. POINTER BASICS:
 *    - char* str points to the first character
 *    - *str gives the character at current position
 *    - str++ moves pointer to next character
 *
 * 3. NULL vs '\0':
 *    - NULL is a pointer (address 0) - used for invalid pointers
 *    - '\0' is a character (value 0) - used to end strings
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
 *   strlen("Hello") = 5 (doesn't count '\0')
 *
 * ============================================================================
 * STEP-BY-STEP WALKTHROUGH:
 * ============================================================================
 *
 *   Input: "Hello"
 *
 *   Initial: len = 0, str points to 'H'
 *
 *   Iteration 1: str[0] = 'H' != '\0', len = 1
 *   Iteration 2: str[1] = 'e' != '\0', len = 2
 *   Iteration 3: str[2] = 'l' != '\0', len = 3
 *   Iteration 4: str[3] = 'l' != '\0', len = 4
 *   Iteration 5: str[4] = 'o' != '\0', len = 5
 *   Iteration 6: str[5] = '\0' == '\0', STOP
 *
 *   Return: 5
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   len = 0
 *   while (str[len] != '\0'):
 *       len++
 *   return len
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Must visit every character to find '\0'
 * - No way to know length without scanning
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only one counter variable
 * - No extra arrays or data structures
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "What's the difference between size_t and int?"
 * A1: size_t is unsigned and guaranteed to hold any array size on the platform.
 *     int might be too small for very large strings. Always use size_t for
 *     lengths and array indices in production code.
 *
 * -------------------------------------------------------------------------
 * Q2: "Can strlen be faster than O(n)?"
 * A2: Not without additional data structure. Some libraries use word-at-a-time
 *     tricks (check 4/8 bytes at once for null), but still O(n).
 *     If you store length separately (like std::string), then O(1).
 *
 * -------------------------------------------------------------------------
 * Q3: "What happens with unterminated string?"
 * A3: Undefined behavior! strlen keeps reading until it finds '\0'.
 *     May read past allocated memory, causing crash or security vulnerability.
 *     Always ensure strings are properly null-terminated.
 *
 * -------------------------------------------------------------------------
 * Q4: "Array indexing vs pointer - which is better?"
 * A4: Modern compilers optimize both to identical machine code. Use whichever
 *     is clearer. Pointer version shows understanding of C memory model.
 *     Array indexing is easier to understand for beginners.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

/*
 * ============================================================================
 * STRLEN FUNCTION (Array Version) - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * size_t my_strlen(const char* str):
 *   - "size_t" = return type for sizes (unsigned, can't be negative)
 *   - "const char*" = pointer to characters we won't modify
 *   - "str" = the string whose length we want
 *
 * if (str == NULL) return 0:
 *   - Check if pointer is invalid (NULL)
 *   - Dereferencing NULL = crash! Always check first
 *   - Return 0 for NULL string (or could return error)
 *
 * size_t len = 0:
 *   - Initialize counter to zero
 *   - Will count each character we find
 *
 * while (str[len] != '\0'):
 *   - str[len] accesses character at position len
 *   - Compare to null terminator '\0'
 *   - Stop when we find the end
 *
 * len++:
 *   - Found a valid character, increment counter
 *   - Next iteration checks str[len+1]
 *
 * return len:
 *   - Return the count (excludes '\0')
 *
 * ============================================================================
 */
// Array indexing version - simpler to explain
// Say: "I'll implement strlen by counting characters until the null terminator"
size_t my_strlen(const char* str) {
    // Check for NULL pointer to avoid dereferencing invalid memory
    // Say: "First, I check if the pointer is NULL and return 0 if it is"
    // WHY: Dereferencing NULL causes a segmentation fault (crash)
    if (str == NULL) return 0;

    // Initialize length counter to zero
    // Say: "I start with a counter at zero to track the string length"
    // WHY: We'll increment this for each character we find
    size_t len = 0;

    // Traverse the string until we hit the null terminator
    // Say: "Now I loop through the string, incrementing the counter for each character"
    // WHY: '\0' marks the end of a string in C
    while (str[len] != '\0') {
        // Found a character, increment count
        // Say: "I found a character, so I increment the length"
        len++;
    }

    // Return the total count (excludes null terminator)
    // Say: "Finally, I return the length which doesn't include the null terminator"
    // WHY: Standard strlen doesn't count '\0' as part of length
    return len;
}

/*
 * ============================================================================
 * STRLEN FUNCTION (Pointer Version) - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * const char* p = str:
 *   - Create a second pointer starting at same position
 *   - We'll advance p, keeping str at the start
 *   - WHY: We need both positions to calculate length
 *
 * while (*p != '\0'):
 *   - *p dereferences pointer = gets character at current position
 *   - Check if it's the null terminator
 *
 * p++:
 *   - Move pointer to next memory address
 *   - p now points to next character
 *
 * return p - str:
 *   - POINTER ARITHMETIC: subtracting pointers gives distance in elements
 *   - If str = address 1000 and p = address 1005
 *   - p - str = 5 (the length!)
 *
 * VISUALIZATION:
 *
 *   Memory: [H][e][l][l][o][\0]
 *   Address: 100 101 102 103 104 105
 *
 *   str points to 100 (start)
 *   After loop, p points to 105 (the '\0')
 *   p - str = 105 - 100 = 5
 *
 * ============================================================================
 */
// Pointer version (more common in interviews)
// Say: "I can also implement this using pointer arithmetic instead of array indexing"
size_t my_strlen_ptr(const char* str) {
    // Check for NULL pointer
    // Say: "Again, I check for NULL first"
    if (str == NULL) return 0;

    // Save the starting address so we can calculate the difference later
    // Say: "I save the original pointer so I can compute the length using pointer arithmetic"
    // WHY: We'll advance p, but need str to stay at the start
    const char* p = str;

    // Walk the pointer forward until we find null terminator
    // Say: "I advance the pointer through each character until I hit the null"
    // WHY: *p dereferences the pointer to get the character
    while (*p != '\0') {
        // Move to next character
        // Say: "I move the pointer to the next character"
        p++;
    }

    // Calculate length by subtracting start address from end address
    // Say: "The length is the difference between the final and initial pointer positions"
    // WHY: Pointer arithmetic gives us the number of elements between two pointers
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

    printf("\n=== Key Points ===\n");
    printf("- Count characters until '\\0'\n");
    printf("- Don't count the null terminator itself\n");
    printf("- Check for NULL pointer first\n");
    printf("- Use size_t for return type (unsigned)\n");

    return 0;
}
