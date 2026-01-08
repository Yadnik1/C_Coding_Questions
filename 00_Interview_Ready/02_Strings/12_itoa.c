/*
 * ============================================================================
 * PROBLEM: Implement itoa() - Integer to ASCII String Conversion
 * ============================================================================
 *
 * WHAT IS itoa()?
 * itoa() is the OPPOSITE of atoi(). It converts an integer to its string
 * representation. "itoa" stands for "Integer TO ASCII".
 *
 * Note: itoa() is NOT a standard C function! It's a common extension.
 * Standard alternative: sprintf(buf, "%d", num);
 *
 * FUNCTION SIGNATURE:
 * char* itoa(int value, char* str, int base);
 *
 * EXAMPLES:
 * - itoa(123, buf, 10)  -> "123"
 * - itoa(-456, buf, 10) -> "-456"
 * - itoa(255, buf, 16)  -> "ff"      (hexadecimal)
 * - itoa(10, buf, 2)    -> "1010"    (binary)
 * - itoa(64, buf, 8)    -> "100"     (octal)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Companion to atoi (shows complete understanding)
 * - Tests modulo and division operations
 * - Tests understanding of digit-to-character conversion
 * - Embedded: debug output without printf, LCD displays, protocol formatting
 *
 * KEY CONCEPT - WHY WE BUILD BACKWARDS:
 * To extract digits, we use modulo (%) which gives the LAST digit first!
 *   123 % 10 = 3 (last digit)
 *   12 % 10 = 2  (second-to-last)
 *   1 % 10 = 1   (first digit)
 *
 * So we get "321" and must REVERSE to get "123"!
 *
 * ALGORITHM:
 * 1. Handle special case: num == 0 (just return "0")
 * 2. Handle negative numbers (add '-' sign for base 10)
 * 3. Extract digits using: digit = num % base
 * 4. Convert digit to char: '0' + digit (for 0-9) or 'a' + digit - 10 (for a-z)
 * 5. Remove extracted digit: num = num / base
 * 6. Reverse the string
 *
 * VISUAL for itoa(123, buf, 10):
 *   123 % 10 = 3 -> str = "3"
 *   12 % 10 = 2  -> str = "32"
 *   1 % 10 = 1   -> str = "321"
 *   Reverse      -> str = "123"
 *
 * BUFFER SIZE REQUIREMENTS:
 * - Base 2: 32 bits + sign + null = 34 bytes
 * - Base 10: max 10 digits + sign + null = 12 bytes
 * - Base 16: max 8 chars + null = 9 bytes
 *
 * ============================================================================
 */

// Time: O(log n), Space: O(1) - digits in number

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Say: "Convert integer to string, return pointer to buffer"
char *my_itoa(int num, char *str, int base) {
    // Say: "Handle invalid base"
    if (base < 2 || base > 36) {
        str[0] = '\0';
        return str;
    }

    int i = 0;
    bool is_negative = false;

    // Say: "Handle zero specially"
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Say: "Handle negative numbers (only for base 10)"
    if (num < 0 && base == 10) {
        is_negative = true;
        num = -num;  // Say: "Work with positive value"
    }

    // Say: "Process digits from least significant to most"
    while (num != 0) {
        int digit = num % base;  // Say: "Get last digit"

        // Say: "Convert digit to character"
        if (digit < 10) {
            str[i++] = '0' + digit;  // Say: "0-9 -> '0'-'9'"
        } else {
            str[i++] = 'a' + (digit - 10);  // Say: "10-35 -> 'a'-'z'"
        }

        num = num / base;  // Say: "Remove last digit"
    }

    // Say: "Add negative sign if needed"
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';  // Say: "Null terminate"

    // Say: "Reverse string - digits are in reverse order"
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    return str;
}

// Say: "Simpler version for base 10 only"
char *int_to_string(int num, char *str) {
    int i = 0;
    bool is_negative = false;

    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    if (num < 0) {
        is_negative = true;
        num = -num;
    }

    // Say: "Extract digits right to left"
    while (num > 0) {
        str[i++] = '0' + (num % 10);  // Say: "Get digit, convert to char"
        num /= 10;                     // Say: "Remove rightmost digit"
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Say: "Reverse the string"
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = temp;
    }

    return str;
}

// Say: "Unsigned version - no negative handling"
char *uint_to_string(unsigned int num, char *str) {
    int i = 0;

    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    while (num > 0) {
        str[i++] = '0' + (num % 10);
        num /= 10;
    }

    str[i] = '\0';

    // Reverse
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = temp;
    }

    return str;
}

int main() {
    char buffer[33];  // Say: "32 bits + null terminator"

    printf("=== itoa Demo ===\n\n");

    // Base 10
    printf("Base 10:\n");
    printf("  12345  -> '%s'\n", my_itoa(12345, buffer, 10));
    printf("  -9876  -> '%s'\n", my_itoa(-9876, buffer, 10));
    printf("  0      -> '%s'\n", my_itoa(0, buffer, 10));

    // Base 16 (hex)
    printf("\nBase 16 (hex):\n");
    printf("  255    -> '%s'\n", my_itoa(255, buffer, 16));
    printf("  4096   -> '%s'\n", my_itoa(4096, buffer, 16));

    // Base 2 (binary)
    printf("\nBase 2 (binary):\n");
    printf("  10     -> '%s'\n", my_itoa(10, buffer, 2));
    printf("  255    -> '%s'\n", my_itoa(255, buffer, 2));

    // Base 8 (octal)
    printf("\nBase 8 (octal):\n");
    printf("  64     -> '%s'\n", my_itoa(64, buffer, 8));

    // Simple int_to_string
    printf("\nSimple int_to_string:\n");
    printf("  42     -> '%s'\n", int_to_string(42, buffer));
    printf("  -123   -> '%s'\n", int_to_string(-123, buffer));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"itoa converts an integer to its string representation in a given base.

 ALGORITHM:
 1. Handle special case: num == 0
 2. Handle negative numbers (base 10 only)
 3. Extract digits using modulo (num % base)
 4. Convert digit to character ('0'+digit or 'a'+digit-10)
 5. Divide by base to remove processed digit
 6. Reverse the string (digits extracted in reverse order)

 WHY REVERSE:
 - We extract least significant digit first (123 % 10 = 3)
 - So we build string as '321', then reverse to '123'

 DIGIT TO CHARACTER:
 - Digits 0-9: '0' + digit (ASCII math)
 - Digits 10-35: 'a' + (digit - 10) for hex/higher bases

 EDGE CASES:
 - Zero: return '0'
 - Negative: prepend '-' (only for base 10)
 - INT_MIN: special handling needed (-2^31 can't be negated)

 BUFFER SIZE:
 - Base 2: needs 32 chars for 32-bit int + sign + null = 34
 - Base 10: max 10 digits + sign + null = 12
 - Base 16: max 8 chars + null = 9

 EMBEDDED USES:
 - Debug output (UART printf alternatives)
 - Display numbers on LCD/7-segment
 - Log file generation
 - Protocol message formatting

 COMMON MISTAKES:
 - Forgetting to reverse the string
 - Buffer overflow (not allocating enough space)
 - Not handling zero case
 - Not null terminating
 - INT_MIN overflow when negating"
*/
