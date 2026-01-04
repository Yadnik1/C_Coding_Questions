/*
 * ============================================================================
 * PROBLEM: Count and Say
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: MEDIUM
 *
 * Generate the nth term of the "count and say" sequence.
 *
 * ============================================================================
 * SEQUENCE:
 * ============================================================================
 *
 *   n=1: "1"           (base case)
 *   n=2: "11"          (one 1)
 *   n=3: "21"          (two 1s)
 *   n=4: "1211"        (one 2, one 1)
 *   n=5: "111221"      (one 1, one 2, two 1s)
 *   n=6: "312211"      (three 1s, two 2s, one 1)
 *
 * ============================================================================
 * VISUALIZATION (n=4 → n=5):
 * ============================================================================
 *
 *   n=4: "1211"
 *
 *   Reading left to right:
 *   - "1"  → one 1    → "11"
 *   - "2"  → one 2    → "12"
 *   - "11" → two 1s   → "21"
 *
 *   n=5: "111221"
 *
 * ============================================================================
 * TIME: O(n * m) where m is length of string | SPACE: O(m)
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

#define MAX_LEN 5000

void countAndSay(int n, char* result) {
    // Base case
    // Say: "For n equals 1, the answer is simply the string '1'"
    strcpy(result, "1");

    // Check if base case
    // Say: "If n is 1, I return immediately"
    if (n == 1) return;

    // Declare working strings
    // Say: "I'll use two strings to alternate between current and next iterations"
    char current[MAX_LEN];
    char next[MAX_LEN];

    // Initialize with base case
    // Say: "I start with current equals '1'"
    strcpy(current, "1");

    // Generate each term from 2 to n
    // Say: "I'll iteratively build each term from 2 up to n"
    for (int i = 2; i <= n; i++) {
        // Get length of current string
        // Say: "For this iteration, I get the length of the current string"
        int len = strlen(current);

        // Track position in next string
        // Say: "I use nextIdx to track where to write in the next string"
        int nextIdx = 0;

        // Process current string
        // Say: "I'll scan through the current string and count consecutive digits"
        int j = 0;

        // Count and say for each group
        // Say: "I process each group of consecutive identical digits"
        while (j < len) {
            // Get the digit to count
            // Say: "I identify the current digit to count"
            char digit = current[j];

            // Count consecutive occurrences
            // Say: "I count how many times this digit repeats consecutively"
            int count = 1;

            // Count consecutive same digits
            // Say: "I keep counting while the next character matches the current digit"
            while (j + count < len && current[j + count] == digit) {
                // Say: "Found another matching digit, increment count"
                count++;
            }

            // Append count and digit to next
            // Say: "Now I append the count followed by the digit to the next string"
            if (count >= 10) {
                // Handle counts of 10 or more (rare)
                // Say: "If count is 10 or more, I need to split it into multiple digits"
                next[nextIdx++] = '0' + (count / 10);
                next[nextIdx++] = '0' + (count % 10);
            } else {
                // Single digit count
                // Say: "I convert the count to a character and append it"
                next[nextIdx++] = '0' + count;
            }

            // Append the digit itself
            // Say: "Then I append the digit that was counted"
            next[nextIdx++] = digit;

            // Move to next group
            // Say: "I move past all the digits I just counted"
            j += count;
        }

        // Null terminate next string
        // Say: "I add a null terminator to complete the next string"
        next[nextIdx] = '\0';

        // Copy next to current for next iteration
        // Say: "I copy the next string to current for the next iteration"
        strcpy(current, next);
    }

    // Copy final result
    // Say: "Finally, I copy the result to the output parameter"
    strcpy(result, current);
}

// Print with explanation
void explainCountAndSay(const char* prev, const char* next) {
    // Print what we're reading
    // Say: "This helper explains the count-and-say transformation"
    printf("   \"%s\" → reading: ", prev);

    // Get length of previous string
    int len = strlen(prev);

    // Process previous string to show explanation
    int j = 0;

    // Show each group
    while (j < len) {
        // Get digit
        char digit = prev[j];

        // Count occurrences
        int count = 1;

        // Count consecutive
        while (j + count < len && prev[j + count] == digit) {
            count++;
        }

        // Print count and digit
        printf("%d×'%c' ", count, digit);

        // Move to next group
        j += count;
    }

    // Show result
    printf("→ \"%s\"\n", next);
}

int main() {
    printf("=== Count and Say ===\n\n");

    char result[MAX_LEN];
    char prev[MAX_LEN];

    printf("Sequence:\n");
    for (int n = 1; n <= 8; n++) {
        if (n > 1) strcpy(prev, result);
        countAndSay(n, result);

        if (n == 1) {
            printf("   n=%d: \"%s\" (base case)\n", n, result);
        } else {
            printf("   n=%d: ", n);
            explainCountAndSay(prev, result);
        }
    }

    printf("\n=== How to Read ===\n");
    printf("Look at previous string, count consecutive digits:\n");
    printf("   \"1211\" has: one 1, one 2, two 1s\n");
    printf("   Write counts: \"11\" + \"12\" + \"21\" = \"111221\"\n");

    return 0;
}
