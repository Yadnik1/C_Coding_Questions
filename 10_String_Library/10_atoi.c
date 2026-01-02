/*
 * ============================================================================
 * PROBLEM: Implement atoi()
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: VERY HIGH
 *
 * atoi = ASCII TO Integer (string → int)
 *
 * ============================================================================
 * WHAT INTERVIEWERS LOOK FOR:
 * ============================================================================
 *
 * 1. NULL pointer handling
 * 2. Leading whitespace
 * 3. Optional +/- sign
 * 4. Overflow/underflow detection
 * 5. Stop at non-digit characters
 *
 * ============================================================================
 * VISUALIZATION: atoi("  -123abc")
 * ============================================================================
 *
 *   "  -123abc"
 *    ^^
 *    Skip whitespace
 *
 *   "  -123abc"
 *      ^
 *      Found '-', negative = true
 *
 *   "  -123abc"
 *       ^^^
 *       Digits: 0*10+1=1, 1*10+2=12, 12*10+3=123
 *
 *   "  -123abc"
 *          ^
 *          Non-digit, STOP
 *
 *   Apply sign: -123
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <limits.h>

// Full implementation with overflow handling
int my_atoi(const char* str) {
    if (str == NULL) return 0;

    int i = 0;
    int sign = 1;
    int result = 0;

    // Step 1: Skip leading whitespace
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
        i++;
    }

    // Step 2: Handle optional sign
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Step 3: Convert digits
    while (str[i] >= '0' && str[i] <= '9') {
        int digit = str[i] - '0';

        // Check for overflow BEFORE it happens
        // INT_MAX = 2147483647
        if (result > INT_MAX / 10 ||
            (result == INT_MAX / 10 && digit > 7)) {
            return (sign == 1) ? INT_MAX : INT_MIN;
        }

        result = result * 10 + digit;
        i++;
    }

    return sign * result;
}

// Simple version (no overflow check)
int my_atoi_simple(const char* str) {
    if (str == NULL) return 0;

    int result = 0;
    int sign = 1;
    int i = 0;

    // Skip whitespace
    while (str[i] == ' ') i++;

    // Handle sign
    if (str[i] == '-' || str[i] == '+') {
        sign = (str[i] == '-') ? -1 : 1;
        i++;
    }

    // Convert digits
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return sign * result;
}

int main() {
    printf("=== atoi Implementation ===\n\n");

    printf("1. Basic conversion:\n");
    printf("   atoi(\"123\")      = %d\n", my_atoi("123"));
    printf("   atoi(\"-456\")     = %d\n", my_atoi("-456"));
    printf("   atoi(\"+789\")     = %d\n\n", my_atoi("+789"));

    printf("2. Whitespace handling:\n");
    printf("   atoi(\"  42\")     = %d\n", my_atoi("  42"));
    printf("   atoi(\"  -42\")    = %d\n\n", my_atoi("  -42"));

    printf("3. Stop at non-digit:\n");
    printf("   atoi(\"42abc\")    = %d\n", my_atoi("42abc"));
    printf("   atoi(\"  -42abc\") = %d\n\n", my_atoi("  -42abc"));

    printf("4. Edge cases:\n");
    printf("   atoi(\"\")         = %d\n", my_atoi(""));
    printf("   atoi(\"abc\")      = %d\n", my_atoi("abc"));
    printf("   atoi(\"   \")      = %d\n\n", my_atoi("   "));

    printf("5. Overflow handling:\n");
    printf("   atoi(\"2147483647\")  = %d (INT_MAX)\n", my_atoi("2147483647"));
    printf("   atoi(\"2147483648\")  = %d (overflow → INT_MAX)\n", my_atoi("2147483648"));
    printf("   atoi(\"-2147483648\") = %d (INT_MIN)\n", my_atoi("-2147483648"));
    printf("   atoi(\"-2147483649\") = %d (underflow → INT_MIN)\n\n", my_atoi("-2147483649"));

    printf("=== Key Points ===\n");
    printf("- Skip whitespace first\n");
    printf("- Handle +/- sign\n");
    printf("- digit = char - '0'\n");
    printf("- result = result * 10 + digit\n");
    printf("- Check overflow BEFORE multiply/add\n");

    return 0;
}
