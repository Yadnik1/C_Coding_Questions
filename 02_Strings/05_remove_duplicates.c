/*
 * ============================================================================
 * PROBLEM: Remove Duplicate Characters from String
 * ============================================================================
 *
 * DIFFICULTY: Easy-Medium | TIME: 10 mins | FREQUENCY: High
 *
 * Remove all duplicate characters, keeping only first occurrence.
 *
 * Example:
 * Input:  "programming"
 * Output: "progamin"
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. TRACKING SEEN CHARACTERS:
 *    - Use boolean array of 26 (or 128 for ASCII)
 *    - Mark character as seen after first occurrence
 *
 * 2. TWO-POINTER TECHNIQUE:
 *    - Read pointer: scans all characters
 *    - Write pointer: position to write unique chars
 *
 * 3. IN-PLACE MODIFICATION:
 *    - Overwrite duplicates, shift remaining
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: "programming"
 * ============================================================================
 *
 * String: "programming"
 *          0123456789A
 *
 * seen[26] = {false, false, ...}
 * write = 0, read = 0
 *
 * Step 1: 'p' not seen
 *   str[write=0] = 'p'
 *   seen['p'-'a'] = true
 *   write = 1
 *
 * Step 2: 'r' not seen
 *   str[write=1] = 'r'
 *   seen['r'-'a'] = true
 *   write = 2
 *
 * Step 3: 'o' not seen
 *   str[write=2] = 'o'
 *   write = 3
 *
 * Step 4: 'g' not seen
 *   str[write=3] = 'g'
 *   write = 4
 *
 * Step 5: 'r' SEEN! Skip
 *   write stays 4
 *
 * Step 6: 'a' not seen
 *   str[write=4] = 'a'
 *   write = 5
 *
 * Step 7: 'm' not seen
 *   str[write=5] = 'm'
 *   write = 6
 *
 * Step 8: 'm' SEEN! Skip
 *
 * Step 9: 'i' not seen
 *   str[write=6] = 'i'
 *   write = 7
 *
 * Step 10: 'n' not seen
 *   str[write=7] = 'n'
 *   write = 8
 *
 * Step 11: 'g' SEEN! Skip
 *
 * Add null terminator: str[8] = '\0'
 *
 * Result: "progamin"
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 * Original: p r o g r a m m i n g
 *           ^ ^
 *          w=0 r=0
 *
 * After processing each character:
 *
 *   r=0: 'p' new → write, w=1
 *   p r o g r a m m i n g
 *     ^ ^
 *
 *   r=1: 'r' new → write, w=2
 *   p r o g r a m m i n g
 *       ^ ^
 *
 *   ...
 *
 *   r=4: 'r' seen → skip
 *   p r o g r a m m i n g
 *           ^   ^
 *          w=4  r=4
 *
 *   Final: p r o g a m i n \0
 *                          ^
 *                         w=8
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   seen[26] = {false}
 *   write = 0
 *
 *   for read = 0 to len-1:
 *       if not seen[str[read] - 'a']:
 *           seen[str[read] - 'a'] = true
 *           str[write++] = str[read]
 *
 *   str[write] = '\0'
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Single pass through string
 * - O(1) lookup in seen array
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Fixed size boolean array (26 or 128)
 * - In-place modification
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Empty string: Return empty
 * 2. All same characters: Return single char
 * 3. All unique: Return same string
 * 4. Single character: Return same
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void remove_duplicates(char str[]) {
    // Create boolean array to track seen characters
    // Say: "I'll use a boolean array to track which characters I've already seen"
    bool seen[26] = {false};

    // Initialize write pointer at start
    // Say: "I'll use a write pointer to track where to place unique characters"
    int write = 0;

    // Iterate through string with read pointer
    // Say: "I'll scan through the string with a read pointer"
    for (int read = 0; str[read] != '\0'; read++) {
        // Calculate array index for this character
        // Say: "I convert the character to an index by subtracting 'a'"
        int index = str[read] - 'a';

        // Check if character has been seen before
        // Say: "I check if this character has been seen before"
        if (!seen[index]) {
            // Mark character as seen
            // Say: "It's new, so I mark it as seen"
            seen[index] = true;

            // Write character to write position
            // Say: "And I write it to the write position"
            str[write++] = str[read];
        }
        // Say: "If character was already seen, I just skip it"
    }

    // Add null terminator at write position
    // Say: "Finally, I add a null terminator to end the string"
    str[write] = '\0';
}

// Version that handles all ASCII
void remove_duplicates_ascii(char str[]) {
    // Create boolean array for all ASCII characters
    // Say: "For all ASCII characters, I use an array of size 128"
    bool seen[128] = {false};

    // Initialize write pointer
    // Say: "I start with write pointer at position 0"
    int write = 0;

    // Iterate through string
    // Say: "I'll scan through each character in the string"
    for (int read = 0; str[read] != '\0'; read++) {
        // Get ASCII value as index
        // Say: "I use the ASCII value directly as the index"
        int index = (int)str[read];

        // Check if character is new
        // Say: "I check if this ASCII character is new"
        if (!seen[index]) {
            // Mark as seen
            // Say: "It's new, so I mark it as seen"
            seen[index] = true;

            // Copy to write position
            // Say: "And write it to the output position"
            str[write++] = str[read];
        }
        // Say: "Duplicate characters are simply skipped"
    }

    // Terminate string
    // Say: "I add the null terminator to complete the string"
    str[write] = '\0';
}

int main() {
    char str1[] = "programming";
    printf("Original: \"%s\"\n", str1);
    remove_duplicates(str1);
    printf("After:    \"%s\"\n", str1);

    char str2[] = "hello";
    printf("\nOriginal: \"%s\"\n", str2);
    remove_duplicates(str2);
    printf("After:    \"%s\"\n", str2);

    char str3[] = "aaaaaa";
    printf("\nOriginal: \"%s\"\n", str3);
    remove_duplicates(str3);
    printf("After:    \"%s\"\n", str3);

    char str4[] = "abcdef";
    printf("\nOriginal: \"%s\"\n", str4);
    remove_duplicates(str4);
    printf("After:    \"%s\"\n", str4);

    // With mixed case and spaces
    char str5[] = "Hello World";
    printf("\nOriginal (ASCII): \"%s\"\n", str5);
    remove_duplicates_ascii(str5);
    printf("After:            \"%s\"\n", str5);

    return 0;
}
