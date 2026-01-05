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
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. PARSING STEPS (in order):
 *    a. Skip leading whitespace
 *    b. Handle optional +/- sign
 *    c. Convert digits: result = result * 10 + digit
 *    d. Stop at first non-digit character
 *
 * 2. DIGIT TO INTEGER:
 *    - '0' to '9' have ASCII values 48 to 57
 *    - digit = char - '0' (e.g., '5' - '0' = 5)
 *
 * 3. OVERFLOW DETECTION:
 *    - INT_MAX = 2147483647, INT_MIN = -2147483648
 *    - Check BEFORE multiply: result > INT_MAX / 10
 *    - Check last digit: result == INT_MAX/10 && digit > 7
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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "How do you detect overflow before it happens?"
 * A1: Before result = result * 10 + digit, check: result > INT_MAX / 10 OR
 *     (result == INT_MAX / 10 AND digit > 7). This catches overflow before
 *     the operation. For negative, check digit > 8 for INT_MIN's last digit.
 *
 * -------------------------------------------------------------------------
 * Q2: "What's the difference between atoi, strtol, and sscanf?"
 * A2: atoi: Simple, no error detection (returns 0 on failure, same as "0").
 *     strtol: Better - sets errno on overflow, returns end pointer for error
 *     detection, supports any base. sscanf: Most flexible but heavier.
 *     Use strtol in production code for proper error handling.
 *
 * -------------------------------------------------------------------------
 * Q3: "Why is the digit check (str[i] >= '0' && str[i] <= '9') instead of isdigit?"
 * A3: Both work, but direct comparison is: 1) More portable (isdigit behavior
 *     varies with locale), 2) Faster (no function call), 3) Clearer intent.
 *     isdigit is fine but may accept locale-specific digit characters.
 *
 * -------------------------------------------------------------------------
 * Q4: "How would you modify atoi to handle different bases?"
 * A4: Accept base parameter (2-36). Use strtol's logic: for base > 10,
 *     accept letters a-z/A-Z for digits 10-35. Validate digit < base.
 *     Handle 0x prefix for hex, 0 prefix for octal if supporting auto-detect.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <limits.h>

/*
 * ============================================================================
 * ATOI FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * int my_atoi(const char* str):
 *   - Returns "int" = the parsed integer value
 *   - "const char* str" = string to convert
 *
 * while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') i++:
 *   - Skip leading whitespace (spaces, tabs, newlines)
 *   - Standard atoi behavior
 *
 * if (str[i] == '-') sign = -1; i++:
 *   - Handle optional sign character
 *   - '+' is also valid but sign stays positive
 *
 * int digit = str[i] - '0':
 *   - Convert ASCII character to numeric value
 *   - '5' - '0' = 53 - 48 = 5
 *
 * Overflow check (result > INT_MAX / 10):
 *   - If result > 214748364, multiplying by 10 overflows
 *   - If result == 214748364 and digit > 7, overflows
 *   - Return INT_MAX or INT_MIN based on sign
 *
 * result = result * 10 + digit:
 *   - Shift existing digits left, add new digit
 *   - "123" → 0*10+1=1 → 1*10+2=12 → 12*10+3=123
 *
 * ============================================================================
 */
// Full implementation with overflow handling
// Say: "I'll implement atoi to convert a string to an integer, handling edge cases"
int my_atoi(const char* str) {
    // Check for NULL pointer
    // Say: "First, I check if the string is NULL"
    // WHY: Dereferencing NULL causes crash
    if (str == NULL) return 0;

    // Initialize variables for parsing
    // Say: "I initialize an index, sign flag, and result accumulator"
    int i = 0;          // Current position in string
    int sign = 1;       // 1 for positive, -1 for negative
    int result = 0;     // Accumulated result (builds up digit by digit)

    // Step 1: Skip leading whitespace
    // Say: "I skip any leading whitespace characters like spaces, tabs, or newlines"
    // WHY: Standard atoi behavior - "  123" should parse as 123
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
        // Move past each whitespace character
        // Say: "I move past each whitespace character"
        i++;
    }

    // Step 2: Handle optional sign
    // Say: "I check for an optional plus or minus sign"
    // WHY: Both "-123" and "+123" are valid inputs
    if (str[i] == '-') {
        sign = -1;  // Negative number
        i++;        // Move past the sign
    } else if (str[i] == '+') {
        // sign stays 1 (positive)
        i++;        // Move past the sign
    }

    // Step 3: Convert digits to integer
    // Say: "Now I process each digit, converting it to an integer"
    // WHY: str[i] >= '0' && str[i] <= '9' checks if it's a digit character
    while (str[i] >= '0' && str[i] <= '9') {
        // Extract the numeric value of the current digit
        // Say: "I extract the digit by subtracting the ASCII value of zero"
        // WHY: '5' - '0' = 53 - 48 = 5 (converts char to int)
        int digit = str[i] - '0';

        // Check for overflow BEFORE it happens
        // Say: "Before adding the digit, I check if it would cause overflow"
        // WHY: INT_MAX = 2147483647, so INT_MAX / 10 = 214748364
        // If result > 214748364, then result * 10 > INT_MAX
        // If result == 214748364 and digit > 7, then result * 10 + digit > INT_MAX
        if (result > INT_MAX / 10 ||
            (result == INT_MAX / 10 && digit > 7)) {
            // Would overflow - return appropriate limit
            // Say: "If it would overflow, I return INT_MAX or INT_MIN based on the sign"
            // WHY: Clamp to valid range instead of undefined behavior
            return (sign == 1) ? INT_MAX : INT_MIN;
        }

        // Safe to add digit: multiply result by 10 and add new digit
        // Say: "I multiply the current result by 10 and add the new digit"
        // WHY: This shifts existing digits left and appends new one
        // Example: "123" → 0*10+1=1 → 1*10+2=12 → 12*10+3=123
        result = result * 10 + digit;

        // Move to next character
        // Say: "I move to the next character"
        i++;
    }

    // Apply sign and return
    // Say: "Finally, I apply the sign and return the result"
    // WHY: sign * result converts to negative if needed
    return sign * result;
}

/*
 * ============================================================================
 * ATOI SIMPLE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * This is a simplified version without overflow checking.
 * Good for interview quick solutions, but not production-ready.
 *
 * ============================================================================
 */
// Simple version (no overflow check)
// Say: "Here's a simpler version without overflow checking for basic cases"
int my_atoi_simple(const char* str) {
    // Check for NULL
    // Say: "Check for NULL pointer"
    // WHY: Dereferencing NULL causes crash
    if (str == NULL) return 0;

    // Initialize parsing variables
    // Say: "I initialize result, sign, and index"
    int result = 0;     // Accumulates the integer value
    int sign = 1;       // 1 for positive, -1 for negative
    int i = 0;          // Current position in string

    // Skip whitespace
    // Say: "Skip leading spaces"
    // WHY: Standard atoi skips leading whitespace
    while (str[i] == ' ') i++;

    // Handle sign
    // Say: "Check for a sign character"
    // WHY: Ternary operator is compact way to set sign
    if (str[i] == '-' || str[i] == '+') {
        sign = (str[i] == '-') ? -1 : 1;    // Set sign based on character
        i++;                                 // Move past sign
    }

    // Convert digits
    // Say: "Convert each digit by multiplying result by 10 and adding the digit"
    // WHY: This loop builds the number digit by digit
    while (str[i] >= '0' && str[i] <= '9') {
        // Accumulate digit: shift left and add new digit
        // Say: "I shift existing digits and add the new one"
        result = result * 10 + (str[i] - '0');
        // Move to next character
        i++;
    }

    // Apply sign and return
    // Say: "Apply the sign and return the final result"
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
