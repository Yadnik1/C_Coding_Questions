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
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. TWO-PHASE APPROACH:
 *    - Phase 1: Find the END of dest (the '\0')
 *    - Phase 2: Copy src characters starting there
 *
 * 2. NULL TERMINATOR:
 *    - Must add '\0' after copying
 *    - strncat ALWAYS adds '\0' (unlike strncpy!)
 *
 * 3. BUFFER SIZE WARNING:
 *    - dest must have enough space for: strlen(dest) + strlen(src) + 1
 *    - The +1 is for the null terminator
 *
 * ============================================================================
 * VISUALIZATION: strcat("Hi", "!!")
 * ============================================================================
 *
 *   dest: ['H']['i']['\0'][?][?][?][?][?]
 *   src:  ['!']['!']['\0']
 *
 *   Phase 1: Find end of dest
 *   dest: ['H']['i']['\0'][?][?][?][?][?]
 *                     ^
 *                  dest ptr stops here
 *
 *   Phase 2: Copy src starting at '\0' position
 *   Step 1: dest: ['H']['i']['!'][?][?][?][?][?]
 *   Step 2: dest: ['H']['i']['!']['!'][?][?][?][?]
 *   Step 3: dest: ['H']['i']['!']['!']['\0'][?][?][?]
 *
 *   Result: "Hi!!"
 *
 * ============================================================================
 * STEP-BY-STEP ALGORITHM:
 * ============================================================================
 *
 *   // Phase 1: Find end of dest
 *   while (*dest != '\0'):
 *       dest++
 *
 *   // Phase 2: Copy src (like strcpy)
 *   while (*src != '\0'):
 *       *dest++ = *src++
 *   *dest = '\0'
 *
 *   return original dest
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n + m)
 * ============================================================================
 * - n = length of dest (to find end)
 * - m = length of src (to copy)
 * - Must traverse BOTH strings
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only pointer variables
 *
 * ============================================================================
 * WARNING: "SCHLEMIEL THE PAINTER'S ALGORITHM"
 * ============================================================================
 *
 * Repeated strcat in a loop is O(n²):
 *
 *   char result[1000] = "";
 *   for (int i = 0; i < 100; i++) {
 *       strcat(result, "x");  // Each call scans from beginning!
 *   }
 *
 *   Call 1: scan 0 chars, copy 1  → 1 operation
 *   Call 2: scan 1 char, copy 1   → 2 operations
 *   Call 3: scan 2 chars, copy 1  → 3 operations
 *   ...
 *   Total: 1 + 2 + 3 + ... + 100 = O(n²)
 *
 * BETTER: Track end position, or use snprintf
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why is repeated strcat in a loop inefficient?"
 * A1: Each strcat must find the end of dest first - O(n) each time!
 *     Concatenating k strings of length n: O(n) + O(2n) + O(3n) = O(kn²).
 *     Better: track end position, or use snprintf, or pre-calculate total
 *     length and copy directly. This is "Schlemiel the painter's algorithm".
 *
 * -------------------------------------------------------------------------
 * Q2: "How does strncat differ from strncpy in null termination?"
 * A2: strncat ALWAYS adds null terminator after appending (so needs n+1 space).
 *     strncpy does NOT add null if src >= n characters. This inconsistency
 *     is a common source of bugs. strncat is safer in this regard.
 *
 * -------------------------------------------------------------------------
 * Q3: "What's the safest way to concatenate strings in C?"
 * A3: Use snprintf: snprintf(dest, size, "%s%s", str1, str2). It always
 *     null-terminates and returns chars that would be written (for truncation
 *     check). Or use strlcat on BSD systems. Never use strcat in production.
 *
 * -------------------------------------------------------------------------
 * Q4: "Can strcat cause security vulnerabilities?"
 * A4: Absolutely! Same as strcpy - buffer overflow if dest too small.
 *     Attacker-controlled src can overflow stack/heap, enabling code execution.
 *     Always validate lengths before concatenation. Use bounded functions.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stddef.h>

/*
 * ============================================================================
 * STRCAT FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * char* my_strcat(char* dest, const char* src):
 *   - Returns "char*" = pointer to dest (for chaining)
 *   - "char* dest" = destination (we modify it)
 *   - "const char* src" = source to append (we don't modify)
 *
 * char* ret = dest:
 *   - Save original dest address
 *   - WHY: We'll advance dest, but need to return original
 *
 * while (*dest != '\0') dest++:
 *   - Phase 1: Find end of dest
 *   - Stop when we find '\0'
 *   - Now dest points to where we'll start copying
 *
 * while (*src != '\0') *dest++ = *src++:
 *   - Phase 2: Copy src (just like strcpy)
 *   - Copy character and advance both pointers
 *
 * *dest = '\0':
 *   - Add null terminator at the end
 *   - CRITICAL: Without this, result isn't a valid string
 *
 * return ret:
 *   - Return original dest pointer
 *   - Allows chaining: printf("%s", strcat(dest, src))
 *
 * ============================================================================
 */
// strcat - Concatenate src to end of dest
// Say: "I'll implement strcat by first finding the end of dest, then copying src there"
char* my_strcat(char* dest, const char* src) {
    // Validate input pointers
    // Say: "First, I check that both pointers are valid"
    // WHY: Dereferencing NULL causes crash
    if (dest == NULL || src == NULL) return dest;

    // Save original pointer to return later
    // Say: "I save the original dest pointer to return it"
    // WHY: We'll advance dest, but function should return original
    char* ret = dest;

    // Phase 1: Find the end of dest (the null terminator)
    // Say: "Now I need to find where dest ends by looking for the null terminator"
    // WHY: We need to start appending from this position
    while (*dest != '\0') {
        // Move forward until we find '\0'
        // Say: "I move forward until I find the null terminator"
        dest++;
    }

    // Now dest points to the null terminator of the original string
    // Phase 2: Copy src characters to the end of dest
    // Say: "Now I copy each character from src to the end of dest"
    // WHY: This is essentially strcpy starting at dest's end
    while (*src != '\0') {
        // Copy char and advance both pointers
        // Say: "I copy a character from src to dest and advance both pointers"
        *dest++ = *src++;
    }

    // Add the null terminator to complete the concatenated string
    // Say: "Finally, I add the null terminator to end the combined string"
    // WHY: Without this, the result isn't a valid C string!
    *dest = '\0';

    // Return the original destination pointer
    // Say: "Return the original pointer to the beginning of dest"
    // WHY: Enables chaining like printf("%s", strcat(a, b))
    return ret;
}

/*
 * ============================================================================
 * STRNCAT FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * char* my_strncat(char* dest, const char* src, size_t n):
 *   - "size_t n" = maximum characters to append from src
 *   - SAFER than strcat (limits how much is appended)
 *
 * while (n > 0 && *src != '\0'):
 *   - Copy up to n characters from src
 *   - Stop early if src ends before n characters
 *
 * *dest = '\0':
 *   - ALWAYS add null terminator (unlike strncpy!)
 *   - This is a KEY DIFFERENCE from strncpy
 *
 * NOTE: dest needs space for: strlen(dest) + min(n, strlen(src)) + 1
 *
 * ============================================================================
 */
// strncat - Concatenate at most n chars from src
// Say: "I'll implement strncat which limits how many characters are appended"
char* my_strncat(char* dest, const char* src, size_t n) {
    // Validate input pointers
    // Say: "Check for NULL pointers"
    if (dest == NULL || src == NULL) return dest;

    // Save original pointer to return later
    // Say: "Save the original dest pointer"
    char* ret = dest;

    // Phase 1: Find the end of dest
    // Say: "First, find where dest currently ends"
    // WHY: Same as strcat - need to find where to start appending
    while (*dest != '\0') {
        // Move to the null terminator
        // Say: "Move forward until I find the null terminator"
        dest++;
    }

    // Phase 2: Copy at most n characters from src
    // Say: "Now I copy up to n characters from src, or until src ends"
    // WHY: Two conditions - don't exceed n, and stop if src ends
    while (n > 0 && *src != '\0') {
        // Copy character and advance
        // Say: "I copy a character and advance both pointers"
        *dest++ = *src++;
        // Decrement counter
        // Say: "I decrement the count of remaining characters to copy"
        n--;
    }

    // Always null-terminate (unlike strncpy!)
    // Say: "Important: strncat always adds a null terminator, unlike strncpy"
    // WHY: This makes strncat safer - result is always a valid string
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
    printf("   strcat(dest, \" World\") -> \"%s\"\n\n", dest1);

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
    printf("   strncat(\"Hello\", \" World!!!\", 6) -> \"%s\"\n\n", dest3);

    // strncat with short src
    printf("4. strncat (src shorter than n):\n");
    char dest4[20] = "Hi";
    my_strncat(dest4, "!", 10);  // src is only 1 char
    printf("   strncat(\"Hi\", \"!\", 10) -> \"%s\"\n\n", dest4);

    printf("=== Key Points ===\n");
    printf("- Phase 1: Find end of dest\n");
    printf("- Phase 2: Copy src starting there\n");
    printf("- strcat: No bounds check (dangerous)\n");
    printf("- strncat: Limits chars BUT always null-terminates\n");
    printf("- Repeated strcat is O(n^2) - track end position instead!\n");

    return 0;
}
