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
 */

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

// itoa with base support (2, 10, 16, etc.)
// Say: "I'll implement itoa to convert an integer to a string in any base"
char* my_itoa(int value, char* str, int base) {
    // Validate inputs
    // Say: "First, I validate that the string pointer is valid and the base is between 2 and 36"
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
    if (value == 0) {
        *ptr++ = '0';   // Write '0'
        *ptr = '\0';    // Null terminate
        return str;
    }

    // Handle negative numbers (only for base 10)
    // Say: "For base 10, I handle negative numbers by noting the sign and working with positive"
    if (value < 0 && base == 10) {
        negative = true;    // Remember it's negative

        // Special case for INT_MIN which can't be negated
        // Say: "INT_MIN is special because we can't negate it, so I handle the last digit separately"
        if (value == INT_MIN) {
            *ptr++ = '8';           // Last digit of -2147483648
            value = -214748364;     // Remaining digits
        } else {
            value = -value;         // Make positive
        }
    } else if (value < 0) {
        // For other bases, treat as unsigned
        // Say: "For bases other than 10, I treat negative numbers as unsigned"
        value = (unsigned int)value;
    }

    // Extract digits in reverse order
    // Say: "Now I extract digits by repeatedly taking modulo and dividing"
    while (value != 0) {
        // Get the rightmost digit
        // Say: "I get the rightmost digit using modulo"
        int digit = value % base;

        // Convert digit to character (0-9 or a-z)
        // Say: "I convert the digit to a character - digits 0-9 use ASCII, 10+ use letters"
        if (digit < 10) {
            *ptr++ = '0' + digit;       // For 0-9
        } else {
            *ptr++ = 'a' + (digit - 10); // For a-z (bases > 10)
        }

        // Remove the rightmost digit
        // Say: "I divide by the base to remove the digit I just processed"
        value /= base;
    }

    // Add negative sign if needed
    // Say: "If the number was negative, I add a minus sign"
    if (negative) {
        *ptr++ = '-';   // Add minus sign
    }

    // Null terminate the string
    *ptr = '\0';

    // Reverse the string since we built it backwards
    // Say: "Since I extracted digits in reverse order, I need to reverse the string"
    ptr--;  // Point to last character (before null)

    // Swap characters from both ends moving toward middle
    while (start < ptr) {
        char temp = *start;     // Save left character
        *start = *ptr;          // Copy right to left
        *ptr = temp;            // Copy saved to right
        start++;                // Move left pointer right
        ptr--;                  // Move right pointer left
    }

    // Return the original pointer
    // Say: "Finally, I return the pointer to the beginning of the string"
    return str;
}

// Simple itoa (base 10 only)
// Say: "Here's a simpler version for base 10 only"
char* my_itoa_simple(int value, char* str) {
    // Check for NULL
    // Say: "Check for NULL pointer"
    if (str == NULL) return NULL;

    // Use a temporary buffer to collect digits
    // Say: "I use a temporary buffer because digits come out in reverse order"
    char buffer[20];
    int i = 0;
    bool negative = false;

    // Handle zero
    // Say: "Handle zero as a special case"
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    // Handle negative
    // Say: "If negative, note it and make the value positive"
    if (value < 0) {
        negative = true;
        value = -value;     // Make positive
    }

    // Extract digits (reverse order) into buffer
    // Say: "Extract each digit into the buffer using modulo and division"
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);   // Get digit and convert to char
        value /= 10;                        // Remove digit
    }

    // Build result string
    // Say: "Now I build the result string in the correct order"
    int j = 0;

    // Add minus sign if negative
    if (negative) {
        str[j++] = '-';     // Add sign first
    }

    // Copy digits in reverse (correct order)
    // Say: "I copy the digits from the buffer in reverse order"
    while (i > 0) {
        str[j++] = buffer[--i];     // Copy in reverse
    }

    // Null terminate
    str[j] = '\0';

    // Return the result
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
