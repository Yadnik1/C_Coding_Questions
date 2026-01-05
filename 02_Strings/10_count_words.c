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
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "State machine vs counting transitions?"
 * A1: State machine: Track if we're "in a word" or not.
 *     Counting transitions: Count space-to-letter transitions.
 *     Both are O(n), state machine is cleaner.
 *
 * -------------------------------------------------------------------------
 * Q2: "What characters are word separators?"
 * A2: Depends on requirements! Common options:
 *     - Just space: isspace(c)
 *     - All whitespace: space, tab, newline
 *     - Punctuation too: !isalnum(c)
 *     Always clarify with interviewer!
 *
 * -------------------------------------------------------------------------
 * Q3: "How to handle special cases like hyphens?"
 * A3: Depends on definition:
 *     - "well-known": 1 word or 2 words?
 *     - "don't": 1 word or 2 words?
 *     Define separator rules based on requirements.
 *
 * -------------------------------------------------------------------------
 * Q4: "Can strtok be used?"
 * A4: Yes, but be careful! strtok modifies the original string
 *     and uses static buffer (not thread-safe).
 *     Manual implementation is often preferred.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Method 1: State machine approach
int count_words_v1(char str[]) {
    // Initialize word counter
    // Say: "I'll count words using a state machine approach with an in_word flag"
    int count = 0;

    // Track whether we're currently inside a word
    // Say: "I start outside any word"
    bool in_word = false;

    // Process each character
    // Say: "I'll scan through each character in the string"
    for (int i = 0; str[i] != '\0'; i++) {
        // Check if current character is a space
        // Say: "If I encounter a space, I mark that I'm outside a word"
        if (isspace(str[i])) {
            // Say: "This is whitespace, so I'm outside a word"
            in_word = false;
        } else {
            // Non-space character
            // Say: "This is a non-space character"
            if (!in_word) {
                // Starting a new word
                // Say: "If I wasn't in a word before, this starts a new word, so I increment count"
                count++;
            }

            // Now we're inside a word
            // Say: "Now I'm inside a word"
            in_word = true;
        }
    }

    // Return total word count
    // Say: "Finally, I return the total word count"
    return count;
}

// Method 2: Check previous character
int count_words_v2(char str[]) {
    // Initialize word counter
    // Say: "This alternative checks if each character starts a new word"
    int count = 0;

    // Get string length
    // Say: "I get the string length for the loop"
    int len = strlen(str);

    // Process each character
    // Say: "I scan through each character"
    for (int i = 0; i < len; i++) {
        // Non-space that starts a word
        // Say: "If this is a non-space character"
        if (!isspace(str[i])) {
            // Check if it's the start of a word
            // Say: "And it's either at the beginning or follows a space, it starts a new word"
            if (i == 0 || isspace(str[i - 1])) {
                // Say: "This is the start of a word, increment count"
                count++;
            }
        }
    }

    // Return total word count
    // Say: "Return the total number of words found"
    return count;
}

// Method 3: Using strtok (modifies string!)
int count_words_strtok(char str[]) {
    // Make a copy since strtok modifies the string
    // Say: "For strtok method, I need to copy the string since strtok modifies it"
    char copy[1000];

    // Copy the string
    // Say: "I create a copy of the original string"
    strcpy(copy, str);

    // Initialize counter
    // Say: "Initialize word counter to zero"
    int count = 0;

    // Get first token
    // Say: "I use strtok to split by spaces, tabs, and newlines"
    char *token = strtok(copy, " \t\n");

    // Count tokens
    // Say: "I count each token returned by strtok"
    while (token != NULL) {
        // Say: "Found a word, increment count"
        count++;

        // Get next token
        // Say: "Get the next token"
        token = strtok(NULL, " \t\n");
    }

    // Return count
    // Say: "Return the total word count"
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
