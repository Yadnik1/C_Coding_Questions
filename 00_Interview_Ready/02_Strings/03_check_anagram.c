/*
 * ============================================================================
 * PROBLEM: Check if Two Strings are Anagrams
 * ============================================================================
 *
 * WHAT IS AN ANAGRAM?
 * Two strings are anagrams if they contain the SAME characters, just
 * rearranged in a different order. They must have identical character counts.
 *
 * EXAMPLES:
 * - "listen" and "silent"  -> Both have: l(1), i(1), s(1), t(1), e(1), n(1) ✓
 * - "anagram" and "nagaram" -> Same letters, different order ✓
 * - "hello" and "world"    -> Different letters ✗
 * - "rat" and "car"        -> Different letters ✗
 *
 * REAL-WORLD USE CASE:
 * - Word games (Scrabble, crossword helpers)
 * - Cryptography (some ciphers are anagram-based)
 * - Detecting plagiarism (word shuffling)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Tests understanding of character frequency counting
 * - Shows knowledge of ASCII arithmetic ('a' - 'a' = 0)
 * - Alternative to sorting approach (demonstrates optimization thinking)
 * - Common pattern: use fixed-size array as hash map for characters
 *
 * LIBRARY FUNCTIONS USED:
 * - strlen(str): Returns string length
 *
 * KEY INSIGHT - CHARACTER TO INDEX CONVERSION:
 * In ASCII: 'a' = 97, 'b' = 98, ..., 'z' = 122
 * So: 'a' - 'a' = 0, 'b' - 'a' = 1, ..., 'z' - 'a' = 25
 * This maps each lowercase letter to an array index (0-25)
 *
 * APPROACH:
 * 1. Create array of 26 integers (one per letter)
 * 2. For str1: increment count for each character
 * 3. For str2: decrement count for each character
 * 4. If all counts are 0, they're anagrams!
 *
 * ============================================================================
 */

// Time: O(n), Space: O(1) - fixed size array of 26

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool are_anagrams(char *str1, char *str2) {
    // Say: "First, anagrams must have same length"
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if (len1 != len2) {
        return false;  // Say: "Different lengths can't be anagrams"
    }

    // Say: "I'll use a frequency array - index 0 for 'a', 25 for 'z'"
    // Say: "This is O(1) space since array size is fixed at 26"
    int freq[26] = {0};  // Say: "Initialize all counts to 0"

    // Say: "Increment count for str1 chars, decrement for str2"
    // Say: "If anagrams, all counts will be zero at the end"
    for (int i = 0; i < len1; i++) {
        // Say: "str1[i] - 'a' converts character to array index (0-25)"
        freq[str1[i] - 'a']++;  // Say: "Add 1 for each char in first string"
        freq[str2[i] - 'a']--;  // Say: "Subtract 1 for each char in second string"
    }

    // Say: "Check if all frequencies are zero"
    for (int i = 0; i < 26; i++) {
        if (freq[i] != 0) {
            return false;  // Say: "Non-zero means character count mismatch"
        }
    }

    return true;  // Say: "All counts balanced - they're anagrams"
}

// Alternative: Using two separate arrays (clearer but same complexity)
bool are_anagrams_v2(char *str1, char *str2) {
    if (strlen(str1) != strlen(str2)) {
        return false;
    }

    int freq1[26] = {0};  // Say: "Frequency count for first string"
    int freq2[26] = {0};  // Say: "Frequency count for second string"

    // Say: "Count characters in both strings"
    for (int i = 0; str1[i] != '\0'; i++) {
        freq1[str1[i] - 'a']++;
        freq2[str2[i] - 'a']++;
    }

    // Say: "Compare frequency arrays"
    for (int i = 0; i < 26; i++) {
        if (freq1[i] != freq2[i]) {
            return false;
        }
    }

    return true;
}

int main() {
    char *s1 = "listen";
    char *s2 = "silent";
    printf("'%s' and '%s' are anagrams: %s\n", s1, s2,
           are_anagrams(s1, s2) ? "true" : "false");

    char *s3 = "hello";
    char *s4 = "world";
    printf("'%s' and '%s' are anagrams: %s\n", s3, s4,
           are_anagrams(s3, s4) ? "true" : "false");

    char *s5 = "anagram";
    char *s6 = "nagaram";
    printf("'%s' and '%s' are anagrams: %s\n", s5, s6,
           are_anagrams(s5, s6) ? "true" : "false");

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I'll use a character frequency counting approach with an array of size 26.

 APPROACH:
 - First check if lengths are equal (must be for anagrams)
 - Create array of 26 integers (one per lowercase letter)
 - For each character in str1: increment its count
 - For each character in str2: decrement its count
 - If all counts are zero, strings are anagrams

 WHY FREQUENCY ARRAY:
 - Sorting would be O(n log n), this is O(n)
 - Fixed size array (26) counts as O(1) space
 - Single pass through both strings (combined in one loop)

 CHARACTER TO INDEX MAPPING:
 - 'a' - 'a' = 0, 'b' - 'a' = 1, ..., 'z' - 'a' = 25
 - ASCII math: 'a' is 97, 'z' is 122
 - Subtracting 'a' gives us 0-25 range

 EDGE CASES:
 - Different lengths: early return false
 - Empty strings: both empty are anagrams
 - Same string: obviously anagram of itself

 VARIATIONS FOR FOLLOW-UP:
 - Case insensitive: convert to lowercase first (c | 32)
 - Include all ASCII: use array of 256 instead of 26
 - Unicode: use hash map instead of array

 COMPLEXITY:
 - Time: O(n) - one pass through strings + O(26) for final check = O(n)
 - Space: O(1) - fixed array of 26, doesn't grow with input"
*/
