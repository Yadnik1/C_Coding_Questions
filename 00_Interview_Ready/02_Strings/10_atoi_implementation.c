// Implement atoi - convert string to integer
// Time: O(n), Space: O(1)

#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

int my_atoi(const char *str) {
    // Say: "I'll handle whitespace, sign, digits, and overflow carefully"
    // Say: "This is a common interview question that tests attention to detail"

    if (str == NULL) {
        return 0;  // Say: "Handle null pointer"
    }

    int i = 0;
    int sign = 1;      // Say: "Default to positive"
    long result = 0;   // Say: "Use long to detect overflow before casting to int"

    // Say: "Step 1: Skip leading whitespace"
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
        i++;  // Say: "Move past spaces, tabs, newlines"
    }

    // Say: "Step 2: Handle optional sign character"
    if (str[i] == '-') {
        sign = -1;     // Say: "Remember negative sign"
        i++;
    } else if (str[i] == '+') {
        i++;           // Say: "Plus sign is optional, just skip it"
    }

    // Say: "Step 3: Process digits"
    while (str[i] >= '0' && str[i] <= '9') {
        // Say: "Convert digit char to integer: '5' - '0' = 5"
        int digit = str[i] - '0';

        // Say: "Build result: multiply existing by 10 and add new digit"
        result = result * 10 + digit;

        // Say: "Step 4: Check for overflow BEFORE it happens"
        if (result * sign > INT_MAX) {
            return INT_MAX;  // Say: "Clamp to INT_MAX on positive overflow"
        }
        if (result * sign < INT_MIN) {
            return INT_MIN;  // Say: "Clamp to INT_MIN on negative overflow"
        }

        i++;  // Say: "Move to next character"
    }
    // Say: "Stop at first non-digit character"

    return (int)(result * sign);  // Say: "Apply sign and return"
}

// Simpler version without overflow handling (for basic interviews)
int my_atoi_simple(const char *str) {
    // Say: "Simplified version - assumes valid input, no overflow"

    int i = 0;
    int sign = 1;
    int result = 0;

    // Skip whitespace
    while (str[i] == ' ') i++;

    // Handle sign
    if (str[i] == '-' || str[i] == '+') {
        sign = (str[i] == '-') ? -1 : 1;
        i++;
    }

    // Process digits
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return result * sign;
}

int main() {
    // Basic tests
    printf("'42' -> %d\n", my_atoi("42"));           // Expected: 42
    printf("'   -42' -> %d\n", my_atoi("   -42"));   // Expected: -42
    printf("'+123' -> %d\n", my_atoi("+123"));       // Expected: 123
    printf("'4193 with words' -> %d\n", my_atoi("4193 with words"));  // Expected: 4193

    // Edge cases
    printf("'words and 987' -> %d\n", my_atoi("words and 987"));  // Expected: 0
    printf("'' -> %d\n", my_atoi(""));               // Expected: 0
    printf("'  ' -> %d\n", my_atoi("  "));           // Expected: 0
    printf("'0' -> %d\n", my_atoi("0"));             // Expected: 0
    printf("'-0' -> %d\n", my_atoi("-0"));           // Expected: 0

    // Overflow tests
    printf("'2147483647' -> %d\n", my_atoi("2147483647"));   // Expected: 2147483647 (INT_MAX)
    printf("'2147483648' -> %d\n", my_atoi("2147483648"));   // Expected: 2147483647 (clamped)
    printf("'-2147483648' -> %d\n", my_atoi("-2147483648")); // Expected: -2147483648 (INT_MIN)
    printf("'-2147483649' -> %d\n", my_atoi("-2147483649")); // Expected: -2147483648 (clamped)

    printf("\nINT_MAX = %d, INT_MIN = %d\n", INT_MAX, INT_MIN);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I'll convert the string to integer, handling edge cases carefully.

 ALGORITHM:
 1. Skip leading whitespace (spaces, tabs, newlines)
 2. Handle optional sign (+ or -)
 3. Process consecutive digits, building the number
 4. Stop at first non-digit character
 5. Handle integer overflow by clamping to INT_MAX/INT_MIN

 CHARACTER TO DIGIT CONVERSION:
 - '0' has ASCII value 48, '9' has 57
 - str[i] - '0' gives us the numeric value (0-9)
 - Example: '5' - '0' = 53 - 48 = 5

 BUILDING THE NUMBER:
 - For '123': result = 0
 - See '1': result = 0 * 10 + 1 = 1
 - See '2': result = 1 * 10 + 2 = 12
 - See '3': result = 12 * 10 + 3 = 123

 OVERFLOW HANDLING:
 - Use 'long' to detect overflow before it wraps around
 - Check after each digit addition
 - Clamp to INT_MAX (2147483647) or INT_MIN (-2147483648)

 EDGE CASES:
 - Empty string: return 0
 - Only whitespace: return 0
 - Leading non-digit: return 0 (after skipping whitespace)
 - Negative zero '-0': return 0
 - Mixed content '123abc': return 123

 COMMON MISTAKES:
 - Forgetting to handle negative sign
 - Not checking for overflow
 - Not stopping at non-digit characters
 - Off-by-one errors with INT_MIN (it's one larger in magnitude than INT_MAX)

 COMPLEXITY:
 - Time: O(n) - single pass through string
 - Space: O(1) - only a few variables"
*/
