/*
 * ============================================================================
 * PROBLEM: Count Frequency of Characters
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: High
 *
 * Count how many times each character appears in a string.
 *
 * Example:
 * Input:  "hello"
 * Output: h=1, e=1, l=2, o=1
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. ASCII TABLE:
 *    - Total 128 or 256 characters
 *    - 'a'-'z': 97-122 (26 lowercase)
 *    - 'A'-'Z': 65-90 (26 uppercase)
 *    - '0'-'9': 48-57 (10 digits)
 *
 * 2. COUNTING TECHNIQUE:
 *    - Use array as hash map
 *    - Index = character value
 *    - Value = count
 *
 * 3. CHARACTER AS INDEX:
 *    - str[i] - 'a' gives 0-25 for lowercase
 *    - str[i] directly gives 0-127 for ASCII
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: "hello"
 * ============================================================================
 *
 * String: "hello"
 *
 * Initialize: count[26] = {0, 0, 0, ..., 0}
 *
 * Step 1: 'h' → count['h'-'a'] = count[7]++
 *   count[7] = 1
 *
 * Step 2: 'e' → count['e'-'a'] = count[4]++
 *   count[4] = 1
 *
 * Step 3: 'l' → count['l'-'a'] = count[11]++
 *   count[11] = 1
 *
 * Step 4: 'l' → count['l'-'a'] = count[11]++
 *   count[11] = 2
 *
 * Step 5: 'o' → count['o'-'a'] = count[14]++
 *   count[14] = 1
 *
 * Final count array:
 *   Index: 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15...
 *   Char:  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p...
 *   Count: 0  0  0  0  1  0  0  1  0  0  0  2  0  0  1  0...
 *                     ^        ^           ^        ^
 *                     e=1      h=1         l=2      o=1
 *
 * ============================================================================
 * VISUALIZATION OF ARRAY AS HASH MAP:
 * ============================================================================
 *
 *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 *   | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 1 | 0 | 0 | 0 | 2 | 0 | 0 | 1 |...
 *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 *     a   b   c   d   e   f   g   h   i   j   k   l   m   n   o
 *                     ↑           ↑               ↑           ↑
 *                    e:1         h:1             l:2         o:1
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   count[26] = {0}  // For lowercase only
 *
 *   for each char c in string:
 *       count[c - 'a']++
 *
 *   // Print non-zero counts
 *   for i = 0 to 25:
 *       if count[i] > 0:
 *           print (char)('a' + i), count[i]
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - One pass to count: O(n)
 * - One pass to print: O(26) = O(1)
 * - Total: O(n)
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Fixed size array (26 or 128 or 256)
 * - Doesn't grow with input
 *
 * ============================================================================
 * VARIATIONS:
 * ============================================================================
 *
 * 1. Case-insensitive: Convert to lowercase first
 * 2. All ASCII: Use array of size 128
 * 3. Find most frequent: Track max while counting
 * 4. Sort by frequency: Store pairs and sort
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why use char - 'a' as index?"
 * A1: Maps 'a'-'z' to indices 0-25. This is a perfect hash function
 *     for lowercase letters with O(1) lookup and no collisions.
 *
 * -------------------------------------------------------------------------
 * Q2: "How to find the most frequent character?"
 * A2: Track max count and corresponding character while counting:
 *     if (count[i] > max_count) { max_count = count[i]; max_char = 'a'+i; }
 *
 * -------------------------------------------------------------------------
 * Q3: "What if we need frequency for ALL ASCII characters?"
 * A3: Use array of size 128 (or 256 for extended ASCII).
 *     Index directly with character value: count[str[i]]++
 *
 * -------------------------------------------------------------------------
 * Q4: "Time complexity for different methods?"
 * A4: Count array: O(n) time, O(1) space (fixed 26/128/256)
 *     Hash map: O(n) time, O(k) space (k = unique chars)
 *     Sorting: O(n log n) time, O(n) space
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Count lowercase letters only
void count_frequency_lowercase(char str[]) {
    // Initialize count array for 26 lowercase letters
    // Say: "I'll create an array of size 26 to count each lowercase letter"
    int count[26] = {0};

    // Count each character in the string
    // Say: "I'll iterate through the string and count each lowercase letter"
    for (int i = 0; str[i] != '\0'; i++) {
        // Check if character is lowercase letter
        // Say: "For each character, I check if it's a lowercase letter"
        if (str[i] >= 'a' && str[i] <= 'z') {
            // Increment count for this character
            // Say: "I use the character minus 'a' as the index and increment the count"
            count[str[i] - 'a']++;
        }
    }

    // Print the frequencies
    // Say: "Now I'll print the frequency of each character that appeared"
    printf("Character frequencies in \"%s\":\n", str);

    // Iterate through count array
    // Say: "I iterate through the count array to display non-zero frequencies"
    for (int i = 0; i < 26; i++) {
        // Only print characters that appeared
        // Say: "I only print characters that have a count greater than zero"
        if (count[i] > 0) {
            // Print character and its count
            // Say: "I convert the index back to a character using 'a' plus i"
            printf("  '%c' = %d\n", 'a' + i, count[i]);
        }
    }
}

// Count all ASCII characters
void count_frequency_all(char str[]) {
    // Initialize count array for all ASCII characters
    // Say: "For all ASCII characters, I create an array of size 128"
    int count[128] = {0};

    // Count each character in the string
    // Say: "I'll count every character including spaces and punctuation"
    for (int i = 0; str[i] != '\0'; i++) {
        // Use character value directly as index
        // Say: "I use the ASCII value of each character as the array index"
        count[(int)str[i]]++;
    }

    // Print the frequencies
    // Say: "Now I'll display the frequency of all characters"
    printf("All character frequencies in \"%s\":\n", str);

    // Iterate through count array
    // Say: "I check all 128 ASCII positions for non-zero counts"
    for (int i = 0; i < 128; i++) {
        // Only print characters that appeared
        // Say: "I only print characters that actually appeared in the string"
        if (count[i] > 0) {
            // Handle space character specially
            // Say: "I handle space specially to make it visible in output"
            if (i == ' ') {
                printf("  'SPACE' = %d\n", count[i]);
            } else if (i >= 33 && i <= 126) {
                // Print printable characters
                // Say: "For printable characters, I display them directly"
                printf("  '%c' = %d\n", i, count[i]);
            }
        }
    }
}

// Find most frequent character
char most_frequent(char str[]) {
    // Initialize count array for lowercase letters
    // Say: "To find the most frequent character, I'll count and track the maximum"
    int count[26] = {0};

    // Track maximum count and corresponding character
    // Say: "I'll keep track of the highest count seen so far"
    int max_count = 0;

    // Track character with maximum count
    // Say: "And I'll remember which character has that highest count"
    char max_char = '\0';

    // Count each character and track maximum
    // Say: "As I count characters, I'll update the maximum if I find a higher count"
    for (int i = 0; str[i] != '\0'; i++) {
        // Convert to lowercase
        // Say: "I convert each character to lowercase for case-insensitive counting"
        char c = tolower(str[i]);

        // Check if it's a letter
        // Say: "I only process alphabetic characters"
        if (c >= 'a' && c <= 'z') {
            // Increment count for this character
            // Say: "I increment the count for this character"
            count[c - 'a']++;

            // Check if this is the new maximum
            // Say: "If this character's count exceeds the current max, I update it"
            if (count[c - 'a'] > max_count) {
                // Update maximum count
                // Say: "I update the maximum count"
                max_count = count[c - 'a'];

                // Update character with maximum count
                // Say: "And remember this character as the most frequent"
                max_char = c;
            }
        }
    }

    // Return the most frequent character
    // Say: "Finally, I return the most frequent character"
    return max_char;
}

int main() {
    char str1[] = "hello";
    count_frequency_lowercase(str1);

    printf("\n");
    char str2[] = "programming";
    count_frequency_lowercase(str2);

    printf("\n");
    char str3[] = "Hello World!";
    count_frequency_all(str3);

    printf("\n=== Most Frequent Character ===\n");
    printf("In \"hello\": '%c'\n", most_frequent("hello"));
    printf("In \"programming\": '%c'\n", most_frequent("programming"));
    printf("In \"aabbbbcccc\": '%c'\n", most_frequent("aabbbbcccc"));

    return 0;
}
