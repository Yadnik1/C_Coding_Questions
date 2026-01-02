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
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Count lowercase letters only
void count_frequency_lowercase(char str[]) {
    int count[26] = {0};

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            count[str[i] - 'a']++;
        }
    }

    printf("Character frequencies in \"%s\":\n", str);
    for (int i = 0; i < 26; i++) {
        if (count[i] > 0) {
            printf("  '%c' = %d\n", 'a' + i, count[i]);
        }
    }
}

// Count all ASCII characters
void count_frequency_all(char str[]) {
    int count[128] = {0};

    for (int i = 0; str[i] != '\0'; i++) {
        count[(int)str[i]]++;
    }

    printf("All character frequencies in \"%s\":\n", str);
    for (int i = 0; i < 128; i++) {
        if (count[i] > 0) {
            if (i == ' ') {
                printf("  'SPACE' = %d\n", count[i]);
            } else if (i >= 33 && i <= 126) {
                printf("  '%c' = %d\n", i, count[i]);
            }
        }
    }
}

// Find most frequent character
char most_frequent(char str[]) {
    int count[26] = {0};
    int max_count = 0;
    char max_char = '\0';

    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower(str[i]);
        if (c >= 'a' && c <= 'z') {
            count[c - 'a']++;
            if (count[c - 'a'] > max_count) {
                max_count = count[c - 'a'];
                max_char = c;
            }
        }
    }

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
