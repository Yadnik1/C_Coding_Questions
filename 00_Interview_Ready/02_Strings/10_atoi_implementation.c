/*
 * ============================================================================
 * PROBLEM: Implement atoi() - ASCII to Integer Conversion
 * ============================================================================
 *
 * WHAT IS atoi()?
 * atoi() converts a string representation of a number to an actual integer.
 * "atoi" stands for "ASCII to Integer".
 *
 * FUNCTION SIGNATURE:
 * int atoi(const char *str);
 *
 * EXAMPLES:
 * - atoi("42")        -> 42
 * - atoi("   -42")    -> -42   (handles leading whitespace and minus sign)
 * - atoi("+123")      -> 123   (handles plus sign)
 * - atoi("4193 text") -> 4193  (stops at first non-digit)
 * - atoi("words 123") -> 0     (no valid digits at start)
 * - atoi("")          -> 0     (empty string)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - EXTREMELY common interview question (LeetCode #8)
 * - Tests attention to detail (many edge cases!)
 * - Tests understanding of ASCII arithmetic ('5' - '0' = 5)
 * - Tests overflow handling (what if number > INT_MAX?)
 * - Embedded systems: parsing serial/UART input, config files
 *
 * KEY CONCEPT - CHARACTER TO DIGIT CONVERSION:
 * In ASCII: '0' = 48, '1' = 49, ..., '9' = 57
 * So: '5' - '0' = 53 - 48 = 5
 * This converts a character digit to its numeric value!
 *
 * ALGORITHM:
 * 1. Skip leading whitespace (' ', '\t', '\n')
 * 2. Handle optional sign ('+' or '-')
 * 3. Convert digits one by one: result = result * 10 + digit
 * 4. Stop at first non-digit character
 * 5. Handle overflow (clamp to INT_MAX or INT_MIN)
 *
 * BUILDING THE NUMBER (Example: "123"):
 *   result = 0
 *   See '1': result = 0 * 10 + 1 = 1
 *   See '2': result = 1 * 10 + 2 = 12
 *   See '3': result = 12 * 10 + 3 = 123
 *
 * OVERFLOW HANDLING:
 * - INT_MAX = 2147483647, INT_MIN = -2147483648
 * - If result would exceed these, clamp to the limit
 * - Note: INT_MIN has larger magnitude than INT_MAX by 1!
 *
 * ============================================================================
 */

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

    // ========================================================================
    // STEP 3: PROCESS DIGITS - THE HEART OF THE ALGORITHM
    // ========================================================================
    //
    // Say: "Now comes the main conversion loop - this is the core logic"
    //
    // WHAT THIS WHILE LOOP DOES:
    // - Checks if current character is a digit ('0' through '9')
    // - Converts that digit character to its numeric value
    // - Builds the result number digit by digit
    // - Checks for overflow after each digit
    // - Stops at the first non-digit character
    //
    // ========================================================================

    while (str[i] >= '0' && str[i] <= '9') {
        // ====================================================================
        // LOOP CONDITION EXPLAINED: str[i] >= '0' && str[i] <= '9'
        // ====================================================================
        //
        // Say: "I check if the current character is a digit using ASCII range"
        //
        // WHY THIS WORKS - ASCII TABLE FOR DIGITS:
        //   Character:  '0'  '1'  '2'  '3'  '4'  '5'  '6'  '7'  '8'  '9'
        //   ASCII:       48   49   50   51   52   53   54   55   56   57
        //
        // So checking str[i] >= '0' && str[i] <= '9' means:
        //   str[i] >= 48 && str[i] <= 57
        //
        // This catches ONLY digit characters, nothing else!
        //
        // EXAMPLES:
        //   'a' = 97  -> 97 >= 48? YES, but 97 <= 57? NO  -> NOT a digit
        //   '5' = 53  -> 53 >= 48? YES, and 53 <= 57? YES -> IS a digit
        //   ' ' = 32  -> 32 >= 48? NO                     -> NOT a digit
        //   '@' = 64  -> 64 >= 48? YES, but 64 <= 57? NO  -> NOT a digit
        //
        // WHY NOT USE isdigit()?
        // - In embedded systems, you might not have <ctype.h>
        // - This manual check has zero dependencies
        // - Interviewers want to see you understand ASCII
        //
        // ====================================================================


        // ====================================================================
        // CHARACTER TO INTEGER CONVERSION: int digit = str[i] - '0';
        // ====================================================================
        //
        // Say: "I convert the character digit to its numeric value using ASCII math"
        //
        int digit = str[i] - '0';
        //
        // THIS IS THE MAGIC LINE! Here's exactly what happens:
        //
        // EXAMPLE 1: str[i] = '5'
        //   digit = '5' - '0'
        //   digit = 53 - 48      (ASCII values)
        //   digit = 5            (the actual number five!)
        //
        // EXAMPLE 2: str[i] = '0'
        //   digit = '0' - '0'
        //   digit = 48 - 48
        //   digit = 0
        //
        // EXAMPLE 3: str[i] = '9'
        //   digit = '9' - '0'
        //   digit = 57 - 48
        //   digit = 9
        //
        // WHY THIS WORKS:
        // The digits '0' through '9' are CONSECUTIVE in ASCII!
        //   '0' = 48, '1' = 49, '2' = 50, ... '9' = 57
        //
        // So subtracting '0' (which is 48) gives us:
        //   '0' - '0' = 48 - 48 = 0
        //   '1' - '0' = 49 - 48 = 1
        //   '2' - '0' = 50 - 48 = 2
        //   ... and so on!
        //
        // VISUAL:
        //   ASCII:     48  49  50  51  52  53  54  55  56  57
        //   Character: '0' '1' '2' '3' '4' '5' '6' '7' '8' '9'
        //   After -48:  0   1   2   3   4   5   6   7   8   9
        //
        // Say: "This is a fundamental technique used everywhere in C"
        //
        // ====================================================================


        // ====================================================================
        // BUILDING THE NUMBER: result = result * 10 + digit;
        // ====================================================================
        //
        // Say: "I build the number by shifting existing digits left and adding new one"
        //
        result = result * 10 + digit;
        //
        // THIS IS HOW WE BUILD A MULTI-DIGIT NUMBER!
        //
        // STEP-BY-STEP EXAMPLE: Converting "4273" to integer
        //
        // Initially: result = 0
        //
        // Iteration 1: See '4'
        //   digit = '4' - '0' = 4
        //   result = 0 * 10 + 4 = 0 + 4 = 4
        //   result is now: 4
        //
        // Iteration 2: See '2'
        //   digit = '2' - '0' = 2
        //   result = 4 * 10 + 2 = 40 + 2 = 42
        //   result is now: 42
        //
        // Iteration 3: See '7'
        //   digit = '7' - '0' = 7
        //   result = 42 * 10 + 7 = 420 + 7 = 427
        //   result is now: 427
        //
        // Iteration 4: See '3'
        //   digit = '3' - '0' = 3
        //   result = 427 * 10 + 3 = 4270 + 3 = 4273
        //   result is now: 4273
        //
        // Loop ends (next char is not a digit or end of string)
        // Final result: 4273
        //
        // WHY MULTIPLY BY 10?
        // - When we see a new digit, all previous digits move one place LEFT
        // - In decimal, moving left means multiplying by 10
        // - Example: 42 becomes 420 (4 moves from tens to hundreds, 2 moves to tens)
        //
        // VISUAL - Place Values:
        //
        //   Building "4273":
        //
        //   After '4':    [4]           = 4
        //                  ^ones
        //
        //   After '2':    [4][2]        = 42
        //                  ^   ^
        //                 tens ones
        //
        //   After '7':    [4][2][7]     = 427
        //                  ^   ^   ^
        //                 100s 10s 1s
        //
        //   After '3':    [4][2][7][3]  = 4273
        //                  ^   ^   ^  ^
        //                1000 100 10  1
        //
        // THE FORMULA PATTERN:
        //   result = result * 10 + digit
        //
        //   This is equivalent to:
        //   - Shift all current digits one place left (multiply by 10)
        //   - Put new digit in the ones place (add digit)
        //
        // Say: "Multiplying by 10 shifts digits left, then we add the new digit"
        //
        // ====================================================================


        // ====================================================================
        // STEP 4: OVERFLOW DETECTION - CRITICAL FOR ROBUSTNESS!
        // ====================================================================
        //
        // Say: "I check for overflow AFTER each digit to catch it early"
        //
        // WHY WE NEED OVERFLOW CHECKING:
        // - 32-bit signed int can only hold: -2,147,483,648 to 2,147,483,647
        // - If someone passes "99999999999", we'd overflow without checking
        // - Overflow causes UNDEFINED BEHAVIOR in C (could crash, give wrong answer, etc.)
        //
        // WHY USE 'long' FOR result?
        // - 'long' is typically 64-bit, can hold much larger values
        // - This lets us detect when the value exceeds int range
        // - We can check BEFORE the overflow actually happens
        //
        // INT_MAX = 2147483647  (2^31 - 1)
        // INT_MIN = -2147483648 (-2^31)
        //
        // Note: INT_MIN has ONE MORE negative value than INT_MAX has positive!
        // This asymmetry is because of how two's complement works.
        //
        // ====================================================================

        if (result * sign > INT_MAX) {
            // Say: "Positive overflow detected - return maximum allowed value"
            //
            // WHEN THIS TRIGGERS:
            // - sign is +1 (positive number)
            // - result has grown beyond 2147483647
            //
            // EXAMPLE: Converting "9999999999"
            //   After several iterations, result becomes 9999999999
            //   9999999999 * 1 > 2147483647? YES!
            //   Return INT_MAX (2147483647) instead of garbage
            //
            return INT_MAX;
        }

        if (result * sign < INT_MIN) {
            // Say: "Negative overflow detected - return minimum allowed value"
            //
            // WHEN THIS TRIGGERS:
            // - sign is -1 (negative number)
            // - result * -1 goes below -2147483648
            //
            // EXAMPLE: Converting "-9999999999"
            //   sign = -1
            //   After several iterations, result becomes 9999999999
            //   9999999999 * -1 = -9999999999
            //   -9999999999 < -2147483648? YES!
            //   Return INT_MIN (-2147483648) instead of garbage
            //
            return INT_MIN;
        }

        // ====================================================================
        // WHY CHECK OVERFLOW HERE (after result update)?
        // ====================================================================
        //
        // We check AFTER updating result because:
        // 1. We need to see if the NEW result overflows
        // 2. 'long' can hold the potentially overflowed value safely
        // 3. We catch it before the next iteration makes it worse
        //
        // Alternative: Check BEFORE updating (more complex math):
        //   if (result > (INT_MAX - digit) / 10) { overflow! }
        //
        // Our approach is simpler because 'long' gives us headroom.
        //
        // ====================================================================


        // ====================================================================
        // MOVE TO NEXT CHARACTER: i++;
        // ====================================================================
        //
        i++;
        //
        // Say: "Move to the next character in the string"
        //
        // After processing current digit, we advance to look at the next char.
        // The while loop condition will then check if it's also a digit.
        //
        // EXAMPLE: For string "123abc"
        //   i=0: str[0]='1' is digit, process it, i becomes 1
        //   i=1: str[1]='2' is digit, process it, i becomes 2
        //   i=2: str[2]='3' is digit, process it, i becomes 3
        //   i=3: str[3]='a' -> 'a' >= '0' && 'a' <= '9'? NO! Loop exits.
        //
        // We successfully extracted 123 and stopped at 'a'.
        //
        // ====================================================================
    }

    // ========================================================================
    // AFTER THE LOOP: WHY DID WE EXIT?
    // ========================================================================
    //
    // Say: "The loop stops at the first non-digit character"
    //
    // The loop exits when str[i] is NOT a digit. This could be:
    //   1. End of string: str[i] = '\0' (null terminator)
    //   2. Non-digit character: letter, space, punctuation, etc.
    //
    // EXAMPLES:
    //   "123"      -> Loop processes 1,2,3, exits at '\0', result = 123
    //   "123abc"   -> Loop processes 1,2,3, exits at 'a', result = 123
    //   "   -42 x" -> Loop processes 4,2, exits at ' ', result = 42 (with sign = -1)
    //   ""         -> Loop never runs (first char is '\0'), result = 0
    //
    // ========================================================================


    // ========================================================================
    // FINAL STEP: APPLY SIGN AND RETURN
    // ========================================================================
    //
    // Say: "Finally, I apply the sign we saved earlier and return the result"
    //
    return (int)(result * sign);
    //
    // WHAT THIS LINE DOES:
    //
    // 1. result * sign
    //    - If sign = 1 (positive):  result * 1 = result (unchanged)
    //    - If sign = -1 (negative): result * -1 = -result (negated)
    //
    // 2. (int) cast
    //    - result is 'long', we need to return 'int'
    //    - Safe to cast because we already checked for overflow above
    //    - If we didn't overflow, the value fits in int
    //
    // EXAMPLES:
    //   "42":   result=42, sign=1  -> 42 * 1 = 42
    //   "-42":  result=42, sign=-1 -> 42 * -1 = -42
    //   "+123": result=123, sign=1 -> 123 * 1 = 123
    //   "-0":   result=0, sign=-1  -> 0 * -1 = 0 (negative zero is just zero)
    //
    // WHY APPLY SIGN AT THE END (not during loop)?
    // - Simpler logic: always build positive number, negate at end if needed
    // - Avoids dealing with negative arithmetic during the build process
    // - Cleaner overflow checking (always compare positive result)
    //
    // ========================================================================
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
