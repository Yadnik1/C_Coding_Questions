// Find first non-repeating character in string
// Time: O(n), Space: O(1) - using fixed size frequency array

#include <stdio.h>
#include <string.h>

char first_non_repeating(char *str) {
    // Say: "I'll use a frequency array to count occurrences of each character"
    // Say: "Then scan string again to find first char with count 1"

    if (str == NULL || str[0] == '\0') {
        return '\0';  // Say: "Handle empty string"
    }

    // Say: "Frequency array for all 256 ASCII characters"
    // Say: "This is O(1) space since size is fixed"
    int freq[256] = {0};

    // Say: "First pass: count frequency of each character"
    for (int i = 0; str[i] != '\0'; i++) {
        freq[(unsigned char)str[i]]++;  // Say: "Increment count for this char"
    }

    // Say: "Second pass: find first character with frequency 1"
    // Say: "We must scan in order to find the FIRST non-repeating"
    for (int i = 0; str[i] != '\0'; i++) {
        if (freq[(unsigned char)str[i]] == 1) {
            return str[i];  // Say: "Found first non-repeating character"
        }
    }

    // Say: "All characters repeat"
    return '\0';
}

// Return index instead of character
int first_non_repeating_index(char *str) {
    // Say: "Same approach but return index for flexibility"

    if (str == NULL || str[0] == '\0') {
        return -1;
    }

    int freq[256] = {0};

    // Say: "Count all character frequencies"
    for (int i = 0; str[i] != '\0'; i++) {
        freq[(unsigned char)str[i]]++;
    }

    // Say: "Find index of first char with frequency 1"
    for (int i = 0; str[i] != '\0'; i++) {
        if (freq[(unsigned char)str[i]] == 1) {
            return i;  // Say: "Return index, not character"
        }
    }

    return -1;  // Say: "Return -1 if no non-repeating char found"
}

int main() {
    char *str1 = "leetcode";
    char result1 = first_non_repeating(str1);
    printf("String: '%s'\n", str1);
    printf("First non-repeating char: '%c'\n\n", result1);  // Expected: 'l'

    char *str2 = "loveleetcode";
    char result2 = first_non_repeating(str2);
    printf("String: '%s'\n", str2);
    printf("First non-repeating char: '%c'\n\n", result2);  // Expected: 'v'

    char *str3 = "aabb";
    char result3 = first_non_repeating(str3);
    printf("String: '%s'\n", str3);
    if (result3 == '\0') {
        printf("No non-repeating character found\n\n");
    }

    char *str4 = "aabccbdeff";
    int index = first_non_repeating_index(str4);
    printf("String: '%s'\n", str4);
    printf("Index of first non-repeating: %d (char: '%c')\n", index, str4[index]);  // Expected: 6 ('d')

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I'll use a two-pass frequency counting approach.

 APPROACH:
 - First pass: count frequency of every character in the string
 - Second pass: find the first character with frequency exactly 1
 - Return that character (or '\0' if none found)

 WHY TWO PASSES:
 - We need to know total count before deciding if char is unique
 - Can't determine 'first non-repeating' in single pass
 - First pass: O(n), Second pass: O(n), Total: O(2n) = O(n)

 WHY FREQUENCY ARRAY:
 - O(1) lookup time for character count
 - Size 256 covers all ASCII characters
 - Fixed size means O(1) space complexity
 - For lowercase only: array of 26 would work

 EDGE CASES:
 - Empty string: return '\0' or -1
 - All repeating: return '\0' or -1
 - First char is unique: return it
 - Single character string: that char is non-repeating

 INTERVIEW VARIATIONS:
 - Return index instead of character
 - First non-repeating in stream of characters (harder - need linked list + map)
 - Find all non-repeating characters
 - First repeating character (return when freq becomes 2)

 COMPLEXITY:
 - Time: O(n) - two passes through string
 - Space: O(1) - fixed 256-element array, doesn't scale with input"
*/
