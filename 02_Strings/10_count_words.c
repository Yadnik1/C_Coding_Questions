/*
 * ============================================================================
 * PROBLEM: Count Words in a String
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: High
 *
 * Count the number of words in a string (space-separated).
 *
 * Example:
 * Input:  "Hello World"
 * Output: 2
 *
 * Input:  "  Hello   World  "
 * Output: 2 (handle multiple spaces)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. WORD DEFINITION:
 *    - Contiguous sequence of non-space characters
 *    - Separated by one or more spaces
 *
 * 2. COUNTING STRATEGY:
 *    - Count transitions from space to non-space
 *    - Or count space-separated tokens
 *
 * 3. HANDLE EDGE CASES:
 *    - Leading/trailing spaces
 *    - Multiple consecutive spaces
 *    - Empty string
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: "  Hello   World  "
 * ============================================================================
 *
 * String: "  Hello   World  "
 *          0123456789...
 *
 * Track: in_word = false
 *
 *   Pos 0: ' ' → in_word stays false
 *   Pos 1: ' ' → in_word stays false
 *   Pos 2: 'H' → Entering word! count++, in_word = true
 *   Pos 3: 'e' → in_word stays true
 *   Pos 4: 'l' → in_word stays true
 *   Pos 5: 'l' → in_word stays true
 *   Pos 6: 'o' → in_word stays true
 *   Pos 7: ' ' → Exiting word, in_word = false
 *   Pos 8: ' ' → in_word stays false
 *   Pos 9: ' ' → in_word stays false
 *   Pos 10: 'W' → Entering word! count++, in_word = true
 *   Pos 11: 'o' → in_word stays true
 *   ...
 *   Pos 15: ' ' → Exiting word, in_word = false
 *   Pos 16: ' ' → in_word stays false
 *
 * Result: count = 2
 *
 * ============================================================================
 * STATE MACHINE VISUALIZATION:
 * ============================================================================
 *
 *                    +---------+
 *        +--space--->| OUTSIDE |<--space--+
 *        |           |  WORD   |          |
 *        |           +---------+          |
 *        |               |                |
 *        |           non-space            |
 *        |           (count++)            |
 *        |               v                |
 *        |           +---------+          |
 *        +-non-space-| INSIDE  |--space---+
 *                    |  WORD   |
 *                    +---------+
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   count = 0
 *   in_word = false
 *
 *   for each char c in string:
 *       if c is space:
 *           in_word = false
 *       else:
 *           if not in_word:
 *               count++  // Starting new word
 *           in_word = true
 *
 *   return count
 *
 * ============================================================================
 * ALTERNATIVE: Check previous character
 * ============================================================================
 *
 *   count = 0
 *
 *   for i = 0 to len-1:
 *       if str[i] is not space:
 *           if i == 0 OR str[i-1] is space:
 *               count++  // Word start
 *
 *   return count
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Single pass through string
 * - Constant time per character
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only boolean flag and counter
 * - Constant space
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Empty string "": 0 words
 * 2. Only spaces "   ": 0 words
 * 3. Single word "hello": 1 word
 * 4. Leading spaces "  hello": 1 word
 * 5. Trailing spaces "hello  ": 1 word
 * 6. Multiple spaces between words: Handle correctly
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Method 1: State machine approach
int count_words_v1(char str[]) {
    int count = 0;
    bool in_word = false;

    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace(str[i])) {
            in_word = false;
        } else {
            if (!in_word) {
                count++;  // Starting a new word
            }
            in_word = true;
        }
    }

    return count;
}

// Method 2: Check previous character
int count_words_v2(char str[]) {
    int count = 0;
    int len = strlen(str);

    for (int i = 0; i < len; i++) {
        // Non-space that starts a word
        if (!isspace(str[i])) {
            if (i == 0 || isspace(str[i - 1])) {
                count++;
            }
        }
    }

    return count;
}

// Method 3: Using strtok (modifies string!)
int count_words_strtok(char str[]) {
    char copy[1000];
    strcpy(copy, str);

    int count = 0;
    char *token = strtok(copy, " \t\n");

    while (token != NULL) {
        count++;
        token = strtok(NULL, " \t\n");
    }

    return count;
}

int main() {
    printf("=== Count Words ===\n\n");

    char* test1 = "Hello World";
    printf("\"%s\" → %d words\n", test1, count_words_v1(test1));

    char* test2 = "  Hello   World  ";
    printf("\"%s\" → %d words\n", test2, count_words_v1(test2));

    char* test3 = "";
    printf("\"\" → %d words\n", count_words_v1(test3));

    char* test4 = "   ";
    printf("\"   \" → %d words\n", count_words_v1(test4));

    char* test5 = "OneWord";
    printf("\"%s\" → %d words\n", test5, count_words_v1(test5));

    char* test6 = "This is a longer sentence with many words";
    printf("\"%s\" → %d words\n", test6, count_words_v1(test6));

    printf("\n=== Testing all methods ===\n");
    char* sample = "  multiple   spaces   here  ";
    printf("String: \"%s\"\n", sample);
    printf("Method 1 (state): %d\n", count_words_v1(sample));
    printf("Method 2 (prev):  %d\n", count_words_v2(sample));
    printf("Method 3 (strtok): %d\n", count_words_strtok(sample));

    return 0;
}
