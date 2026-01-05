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
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. BUFFER OVERFLOW:
 *    - If dest is smaller than src, strcpy writes past dest's memory
 *    - This overwrites other data = SECURITY VULNERABILITY
 *    - Classic cause of exploits and crashes
 *
 * 2. NULL TERMINATOR:
 *    - Must ALWAYS copy the '\0' at the end
 *    - Without it, the destination is not a valid string
 *
 * 3. TWO POINTERS:
 *    - Need separate pointers for src and dest
 *    - Both advance together as we copy
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   src:  ['H']['e']['l']['l']['o']['\0']
 *   dest: ['?']['?']['?']['?']['?']['?']['?']['?']
 *
 *   Step 1: Copy 'H'     dest: ['H']['?']['?']['?']['?']['?']['?']['?']
 *   Step 2: Copy 'e'     dest: ['H']['e']['?']['?']['?']['?']['?']['?']
 *   Step 3: Copy 'l'     dest: ['H']['e']['l']['?']['?']['?']['?']['?']
 *   Step 4: Copy 'l'     dest: ['H']['e']['l']['l']['?']['?']['?']['?']
 *   Step 5: Copy 'o'     dest: ['H']['e']['l']['l']['o']['?']['?']['?']
 *   Step 6: Copy '\0'    dest: ['H']['e']['l']['l']['o']['\0']['?']['?']
 *
 *   Result: "Hello" (properly null-terminated)
 *
 * ============================================================================
 * WHY RETURN THE DESTINATION POINTER:
 * ============================================================================
 *
 *   Enables chaining: printf("%s", strcpy(dest, src));
 *   Also allows: char* result = strcpy(dest, src);
 *
 *   ALWAYS return the ORIGINAL dest, not the advanced pointer!
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   save original dest
 *   while (*src != '\0'):
 *       *dest = *src
 *       dest++
 *       src++
 *   *dest = '\0'  // Copy null terminator!
 *   return original dest
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Must copy every character including '\0'
 * - n = length of source string
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only pointer variables, no extra arrays
 *
 * ============================================================================
 * WARNING: strcpy has no bounds checking!
 *          dest must be large enough for src + null terminator
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

/*
 * ============================================================================
 * STRCPY FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * char* my_strcpy(char* dest, const char* src):
 *   - Returns "char*" = pointer to destination (for chaining)
 *   - "char* dest" = destination buffer (we WILL modify this)
 *   - "const char* src" = source string (we won't modify this)
 *
 * if (dest == NULL || src == NULL) return NULL:
 *   - Validate BOTH pointers before using them
 *   - Dereferencing NULL = crash
 *
 * char* original = dest:
 *   - Save the starting address of dest
 *   - WHY: We advance dest during copying, but need to return original
 *
 * while (*src != '\0'):
 *   - *src dereferences source pointer = gets current character
 *   - Loop until we hit the null terminator
 *
 * *dest = *src:
 *   - Copy character from src to dest
 *   - *src = read from source, *dest = write to destination
 *
 * dest++; src++:
 *   - Move both pointers to next position
 *   - Both advance together
 *
 * *dest = '\0':
 *   - CRITICAL: Copy the null terminator
 *   - Without this, dest is not a valid string!
 *
 * return original:
 *   - Return the saved starting address
 *   - NOT the current position of dest
 *
 * ============================================================================
 */
// strcpy - Copy entire string
// Say: "I'll implement strcpy by copying each character from source to destination"
char* my_strcpy(char* dest, const char* src) {
    // Validate input pointers to avoid crashes
    // Say: "First, I check that both pointers are valid"
    // WHY: Dereferencing NULL causes segmentation fault
    if (dest == NULL || src == NULL) return NULL;

    // Save the original destination pointer to return later
    // Say: "I save the original dest pointer because I need to return it at the end"
    // WHY: We advance dest during copying, but function should return original
    char* original = dest;

    // Copy each character until we hit the null terminator
    // Say: "Now I copy each character one by one from source to destination"
    // WHY: Loop stops when we reach '\0' in source
    while (*src != '\0') {
        // Copy the current character from source to destination
        // Say: "I copy the current character"
        // WHY: *src reads the char, *dest writes it
        *dest = *src;

        // Move destination pointer forward
        // Say: "I move both pointers to the next position"
        dest++;

        // Move source pointer forward
        src++;
    }

    // Don't forget to copy the null terminator!
    // Say: "Critical step - I add the null terminator to properly end the string"
    // WHY: Without this, dest is not a valid C string!
    *dest = '\0';

    // Return the original destination pointer
    // Say: "Finally, I return the original pointer to the start of dest"
    // WHY: This allows chaining like printf("%s", strcpy(dest, src))
    return original;
}

/*
 * ============================================================================
 * COMPACT STRCPY - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * while ((*dest++ = *src++) != '\0');
 *
 * This single line does everything! Let's break it down:
 *
 * 1. *src       = Get character from source
 * 2. *dest =    = Assign that character to destination
 * 3. *src++     = After reading, advance src pointer
 * 4. *dest++    = After writing, advance dest pointer
 * 5. != '\0'    = Continue until we've copied '\0'
 *
 * The '\0' IS copied because we check AFTER the assignment.
 *
 * EXECUTION ORDER:
 *   1. Read *src (get 'H')
 *   2. Write to *dest (store 'H')
 *   3. Compare result with '\0' ('H' != '\0', continue)
 *   4. Increment both src++ and dest++
 *   5. Repeat until we copy and compare '\0'
 *
 * ============================================================================
 */
// Compact one-liner version
// Say: "Here's a more compact version that combines the copy and advance in one line"
char* my_strcpy_compact(char* dest, const char* src) {
    // Validate input pointers
    // Say: "Same validation as before"
    if (dest == NULL || src == NULL) return NULL;

    // Save return pointer
    // Say: "Save the original dest pointer"
    char* ret = dest;

    // Copy and advance in a single expression - stops AFTER copying '\0'
    // Say: "This copies each character and advances both pointers, stopping after copying the null"
    // WHY: The assignment happens first, then the comparison
    // So '\0' is copied, THEN we check if it was '\0' and exit
    while ((*dest++ = *src++) != '\0');

    // Return original pointer
    return ret;
}

/*
 * ============================================================================
 * STRNCPY FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * char* my_strncpy(char* dest, const char* src, size_t n):
 *   - "size_t n" = maximum number of characters to copy
 *   - SAFER than strcpy because it limits the copy
 *
 * for (i = 0; i < n && src[i] != '\0'; i++):
 *   - Copy while: we haven't copied n chars AND src isn't exhausted
 *   - Stop early if source ends before n characters
 *
 * for (; i < n; i++) dest[i] = '\0':
 *   - Pad remaining space with null bytes
 *   - WHY: strncpy standard behavior (wasteful, but specified)
 *
 * WARNING: If src >= n characters, dest is NOT null-terminated!
 *          You must manually add '\0': dest[n-1] = '\0';
 *
 * ============================================================================
 */
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
    // WHY: Two conditions - don't exceed n, and stop if source ends
    for (i = 0; i < n && src[i] != '\0'; i++) {
        // Copy one character
        // Say: "I copy character at position i"
        dest[i] = src[i];
    }

    // Important: Pad remaining space with null bytes if source was shorter than n
    // Say: "If the source was shorter than n, I pad the rest with null bytes"
    // WHY: This is strncpy's standard behavior (fills remaining with '\0')
    for (; i < n; i++) {
        // Fill with null bytes
        // Say: "I fill remaining positions with null"
        dest[i] = '\0';
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
    printf("   strcpy(dest, \"Hello\") -> \"%s\"\n\n", dest);

    // strncpy - src shorter than n
    printf("2. strncpy (src shorter than n):\n");
    char dest2[10] = "XXXXXXXXX";
    my_strncpy(dest2, "Hi", 5);
    printf("   strncpy(dest, \"Hi\", 5) -> \"%s\"\n", dest2);
    printf("   (padded with nulls)\n\n");

    // strncpy - src longer than n
    printf("3. strncpy (src longer than n):\n");
    char dest3[10];
    my_strncpy(dest3, "Hello World", 5);
    dest3[5] = '\0';  // Must manually null-terminate!
    printf("   strncpy(dest, \"Hello World\", 5) -> \"%s\"\n", dest3);
    printf("   WARNING: May not be null-terminated!\n\n");

    printf("=== Key Points ===\n");
    printf("- strcpy: No bounds check, can overflow\n");
    printf("- strncpy: Safer, but may not null-terminate\n");
    printf("- Always ensure dest is large enough\n");
    printf("- Return original dest pointer for chaining\n");
    printf("- Don't forget to copy the null terminator!\n");

    return 0;
}
