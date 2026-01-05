/*
 * ============================================================================
 * PROBLEM: Implement itoa()
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: VERY HIGH
 *
 * itoa = Integer TO ASCII (int → string)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. DIGIT EXTRACTION:
 *    - digit = value % base (gets rightmost digit)
 *    - value /= base (removes rightmost digit)
 *    - Digits come out in REVERSE order!
 *
 * 2. DIGIT TO CHARACTER:
 *    - For 0-9: '0' + digit (e.g., '0' + 5 = '5')
 *    - For 10-35: 'a' + (digit - 10) (for hex, etc.)
 *
 * 3. SPECIAL CASES:
 *    - Zero: handled separately (% 10 of 0 is 0, loop won't execute)
 *    - INT_MIN: can't negate because |INT_MIN| > INT_MAX
 *    - Negative: only matters for base 10
 *
 * 4. REVERSAL:
 *    - After extraction, string is backwards
 *    - Swap characters from both ends toward middle
 *
 * ============================================================================
 * VISUALIZATION: itoa(-456)
 * ============================================================================
 *
 *   Input: -456
 *
 *   Step 1: Note negative, work with 456
 *
 *   Step 2: Extract digits (reverse order):
 *           456 % 10 = 6, 456 / 10 = 45
 *           45 % 10 = 5, 45 / 10 = 4
 *           4 % 10 = 4, 4 / 10 = 0 → STOP
 *
 *   Step 3: Digits collected: "654"
 *
 *   Step 4: Reverse: "456"
 *
 *   Step 5: Add sign: "-456"
 *
 * ============================================================================
 * TIME: O(log n) | SPACE: O(1) - modifies provided buffer
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why is itoa not standard C?"
 * A1: itoa is not in C standard library! It's a non-standard extension from
 *     older compilers. Use sprintf(buf, "%d", value) for portable code, or
 *     snprintf for safety. Some systems provide itoa, but don't rely on it.
 *
 * -------------------------------------------------------------------------
 * Q2: "How do you handle INT_MIN in itoa?"
 * A2: INT_MIN (-2147483648) can't be negated because |INT_MIN| > INT_MAX.
 *     Solution: Handle last digit separately (8), then process -214748364.
 *     Or use unsigned arithmetic: work with (unsigned)value throughout.
 *
 * -------------------------------------------------------------------------
 * Q3: "Why do digits come out in reverse order?"
 * A3: We extract digits using % 10 which gives the rightmost digit first.
 *     123 % 10 = 3, 12 % 10 = 2, 1 % 10 = 1. So we get "321" and must
 *     reverse to "123". Alternative: calculate total digits first, fill
 *     from end backwards (no reversal needed).
 *
 * -------------------------------------------------------------------------
 * Q4: "How would you implement itoa without reversing?"
 * A4: First count digits: log10(n)+1 for positive, +1 for sign. Then fill
 *     buffer from the end: buf[len-1] = '0' + n%10; n/=10; repeat backward.
 *     This avoids the reverse step but requires two passes over the number.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

/*
 * ============================================================================
 * ITOA FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * char* my_itoa(int value, char* str, int base):
 *   - Returns "char*" = pointer to the string (same as str parameter)
 *   - "int value" = integer to convert
 *   - "char* str" = buffer to store result
 *   - "int base" = number base (2=binary, 10=decimal, 16=hex)
 *
 * if (value == 0) *ptr++ = '0'; *ptr = '\0'; return str:
 *   - Zero is special case (loop won't execute for 0)
 *   - Just write "0\0" and return
 *
 * int digit = value % base:
 *   - Gets rightmost digit (e.g., 456 % 10 = 6)
 *   - value /= base removes that digit (456/10 = 45)
 *
 * if (digit < 10) *ptr++ = '0' + digit:
 *   - Convert digit 0-9 to character '0'-'9'
 *   - '0' + 5 = '5'
 *
 * else *ptr++ = 'a' + (digit - 10):
 *   - For bases > 10, digits 10-35 become 'a'-'z'
 *   - 'a' + (15-10) = 'a' + 5 = 'f'
 *
 * Reversal loop: swap characters from both ends
 *   - String was built backwards, need to reverse
 *   - "654-" becomes "-456"
 *
 * ============================================================================
 */
// itoa with base support (2, 10, 16, etc.)
// Say: "I'll implement itoa to convert an integer to a string in any base"
char* my_itoa(int value, char* str, int base) {
    // Validate inputs
    // Say: "First, I validate that the string pointer is valid and the base is between 2 and 36"
    // WHY: Base must be 2-36 (0-9 plus a-z gives 36 digit symbols)
    if (str == NULL || base < 2 || base > 36) {
        return NULL;
    }

    // Set up pointers and flags
    // Say: "I set up a pointer to build the string and a flag for negative numbers"
    char* ptr = str;        // Current position in output string
    char* start = str;      // Remember start for reversal
    bool negative = false;  // Track if number is negative

    // Handle zero as a special case
    // Say: "If the value is zero, I just write zero and return"
    // WHY: Loop uses value != 0 as condition, so 0 would produce empty string
    if (value == 0) {
        *ptr++ = '0';   // Write '0'
        *ptr = '\0';    // Null terminate
        return str;
    }

    // Handle negative numbers (only for base 10)
    // Say: "For base 10, I handle negative numbers by noting the sign and working with positive"
    // WHY: Negative numbers only make sense in base 10 representation
    if (value < 0 && base == 10) {
        negative = true;    // Remember it's negative

        // Special case for INT_MIN which can't be negated
        // Say: "INT_MIN is special because we can't negate it, so I handle the last digit separately"
        // WHY: INT_MIN = -2147483648, but INT_MAX = 2147483647, so -INT_MIN overflows!
        if (value == INT_MIN) {
            *ptr++ = '8';           // Last digit of -2147483648
            value = -214748364;     // Remaining digits (now safe to negate)
        } else {
            value = -value;         // Make positive for digit extraction
        }
    } else if (value < 0) {
        // For other bases, treat as unsigned
        // Say: "For bases other than 10, I treat negative numbers as unsigned"
        // WHY: Hex/binary representation of negative numbers shows bit pattern
        value = (unsigned int)value;
    }

    // Extract digits in reverse order
    // Say: "Now I extract digits by repeatedly taking modulo and dividing"
    // WHY: % gives rightmost digit, / removes it - but builds string backwards
    while (value != 0) {
        // Get the rightmost digit
        // Say: "I get the rightmost digit using modulo"
        // WHY: 456 % 10 = 6, 45 % 10 = 5, 4 % 10 = 4
        int digit = value % base;

        // Convert digit to character (0-9 or a-z)
        // Say: "I convert the digit to a character - digits 0-9 use ASCII, 10+ use letters"
        // WHY: '0' + 5 = '5', 'a' + 5 = 'f' (for hex)
        if (digit < 10) {
            *ptr++ = '0' + digit;       // For 0-9
        } else {
            *ptr++ = 'a' + (digit - 10); // For a-z (bases > 10)
        }

        // Remove the rightmost digit
        // Say: "I divide by the base to remove the digit I just processed"
        // WHY: 456 / 10 = 45, removes the 6 we just extracted
        value /= base;
    }

    // Add negative sign if needed
    // Say: "If the number was negative, I add a minus sign"
    // WHY: Sign goes at the start, but we're building backwards, so add it last
    if (negative) {
        *ptr++ = '-';   // Add minus sign
    }

    // Null terminate the string
    // Say: "I add the null terminator to end the string"
    *ptr = '\0';

    // Reverse the string since we built it backwards
    // Say: "Since I extracted digits in reverse order, I need to reverse the string"
    // WHY: We built "654-" but need "-456"
    ptr--;  // Point to last character (before null)

    // Swap characters from both ends moving toward middle
    // Say: "I swap characters from the ends, moving toward the middle"
    // WHY: Classic in-place string reversal algorithm
    while (start < ptr) {
        char temp = *start;     // Save left character
        *start = *ptr;          // Copy right to left
        *ptr = temp;            // Copy saved to right
        start++;                // Move left pointer right
        ptr--;                  // Move right pointer left
    }

    // Return the original pointer
    // Say: "Finally, I return the pointer to the beginning of the string"
    // WHY: Caller needs pointer to the result
    return str;
}

/*
 * ============================================================================
 * ITOA SIMPLE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * This is a simplified base-10 only version using a temporary buffer.
 * Alternative approach: build in temp buffer, copy reversed to output.
 *
 * ============================================================================
 */
// Simple itoa (base 10 only)
// Say: "Here's a simpler version for base 10 only"
char* my_itoa_simple(int value, char* str) {
    // Check for NULL
    // Say: "Check for NULL pointer"
    // WHY: Dereferencing NULL causes crash
    if (str == NULL) return NULL;

    // Use a temporary buffer to collect digits
    // Say: "I use a temporary buffer because digits come out in reverse order"
    // WHY: Alternative to in-place reversal - collect then copy in correct order
    char buffer[20];    // Enough for 64-bit int with sign
    int i = 0;          // Current position in buffer
    bool negative = false;

    // Handle zero
    // Say: "Handle zero as a special case"
    // WHY: Loop condition (value > 0) won't execute for 0
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    // Handle negative
    // Say: "If negative, note it and make the value positive"
    // WHY: We need positive value for digit extraction, add sign later
    if (value < 0) {
        negative = true;
        value = -value;     // Make positive
    }

    // Extract digits (reverse order) into buffer
    // Say: "Extract each digit into the buffer using modulo and division"
    // WHY: Builds digits backwards in buffer
    while (value > 0) {
        // Get rightmost digit and convert to character
        // Say: "I extract the rightmost digit and convert to character"
        buffer[i++] = '0' + (value % 10);
        // Remove the digit we just extracted
        // Say: "I remove the digit by dividing by 10"
        value /= 10;
    }

    // Build result string
    // Say: "Now I build the result string in the correct order"
    int j = 0;      // Position in output string

    // Add minus sign if negative
    // Say: "I add the minus sign first if the number was negative"
    if (negative) {
        str[j++] = '-';     // Add sign first
    }

    // Copy digits in reverse (correct order)
    // Say: "I copy the digits from the buffer in reverse order"
    // WHY: Buffer has "654", we copy as "456"
    while (i > 0) {
        str[j++] = buffer[--i];     // Pre-decrement then copy
    }

    // Null terminate
    // Say: "I add the null terminator"
    str[j] = '\0';

    // Return the result
    // Say: "Return pointer to the result string"
    return str;
}

int main() {
    printf("=== itoa Implementation ===\n\n");

    char buffer[50];

    printf("1. Basic conversion (base 10):\n");
    printf("   itoa(123)  → \"%s\"\n", my_itoa(123, buffer, 10));
    printf("   itoa(-456) → \"%s\"\n", my_itoa(-456, buffer, 10));
    printf("   itoa(0)    → \"%s\"\n\n", my_itoa(0, buffer, 10));

    printf("2. Different bases:\n");
    printf("   itoa(255, base 10) → \"%s\"\n", my_itoa(255, buffer, 10));
    printf("   itoa(255, base 16) → \"%s\" (hex)\n", my_itoa(255, buffer, 16));
    printf("   itoa(255, base 2)  → \"%s\" (binary)\n", my_itoa(255, buffer, 2));
    printf("   itoa(255, base 8)  → \"%s\" (octal)\n\n", my_itoa(255, buffer, 8));

    printf("3. Larger numbers:\n");
    printf("   itoa(12345)     → \"%s\"\n", my_itoa(12345, buffer, 10));
    printf("   itoa(-98765)    → \"%s\"\n", my_itoa(-98765, buffer, 10));
    printf("   itoa(INT_MAX)   → \"%s\"\n", my_itoa(INT_MAX, buffer, 10));
    printf("   itoa(INT_MIN)   → \"%s\"\n\n", my_itoa(INT_MIN, buffer, 10));

    printf("4. Hex values:\n");
    printf("   itoa(4096, base 16)  → \"%s\"\n", my_itoa(4096, buffer, 16));
    printf("   itoa(65535, base 16) → \"%s\"\n\n", my_itoa(65535, buffer, 16));

    printf("=== Key Points ===\n");
    printf("- Extract digits using %% 10 (reverse order)\n");
    printf("- Divide by base to get next digit\n");
    printf("- Reverse the string at the end\n");
    printf("- Handle 0 and negative specially\n");
    printf("- For hex: digits 10-15 → 'a'-'f'\n");

    return 0;
}
