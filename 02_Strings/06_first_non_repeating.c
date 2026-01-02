/*
 * ============================================================================
 * PROBLEM: Find First Non-Repeating Character
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * Find the first character that appears only once in the string.
 *
 * Example:
 * Input:  "leetcode"
 * Output: 'l' (appears once, and is first such character)
 *
 * Input:  "loveleetcode"
 * Output: 'v'
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. TWO-PASS APPROACH:
 *    - First pass: Count frequency of each character
 *    - Second pass: Find first character with count = 1
 *
 * 2. ORDER MATTERS:
 *    - Must return FIRST non-repeating
 *    - Second pass must follow string order
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: "leetcode"
 * ============================================================================
 *
 * String: "leetcode"
 *
 * PASS 1: Count frequencies
 *
 *   l: 1
 *   e: 3
 *   t: 1
 *   c: 1
 *   o: 1
 *   d: 1
 *
 *   count['l'-'a'] = count[11] = 1
 *   count['e'-'a'] = count[4]  = 3
 *   count['t'-'a'] = count[19] = 1
 *   count['c'-'a'] = count[2]  = 1
 *   count['o'-'a'] = count[14] = 1
 *   count['d'-'a'] = count[3]  = 1
 *
 * PASS 2: Find first with count = 1
 *
 *   Index 0: 'l' → count[11] = 1 ✓ FOUND!
 *
 * Result: 'l'
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: "loveleetcode"
 * ============================================================================
 *
 * String: "loveleetcode"
 *
 * PASS 1: Count frequencies
 *
 *   l: 2 (positions 0, 10)
 *   o: 2 (positions 1, 8)
 *   v: 1 (position 2)
 *   e: 4 (positions 3, 5, 6, 11)
 *   t: 1 (position 7)
 *   c: 1 (position 9)
 *   d: 1 (position 10)
 *
 * PASS 2: Find first with count = 1
 *
 *   Index 0: 'l' → count = 2 ✗
 *   Index 1: 'o' → count = 2 ✗
 *   Index 2: 'v' → count = 1 ✓ FOUND!
 *
 * Result: 'v'
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   count[26] = {0}
 *
 *   // Pass 1: Count all characters
 *   for each char c in str:
 *       count[c - 'a']++
 *
 *   // Pass 2: Find first with count = 1
 *   for each char c in str:
 *       if count[c - 'a'] == 1:
 *           return c
 *
 *   return '\0'  // No non-repeating char
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - First pass: O(n)
 * - Second pass: O(n) worst case
 * - Total: O(2n) = O(n)
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Fixed size count array (26)
 * - Constant space
 *
 * ============================================================================
 * VARIATIONS:
 * ============================================================================
 *
 * 1. Return index instead of character
 * 2. Find first non-repeating in stream (different approach)
 * 3. Find last non-repeating
 * 4. Find kth non-repeating
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

// Return the first non-repeating character
char first_non_repeating(char str[]) {
    int count[26] = {0};

    // Pass 1: Count frequencies
    for (int i = 0; str[i] != '\0'; i++) {
        count[str[i] - 'a']++;
    }

    // Pass 2: Find first with count = 1
    for (int i = 0; str[i] != '\0'; i++) {
        if (count[str[i] - 'a'] == 1) {
            return str[i];
        }
    }

    return '\0';  // No non-repeating character
}

// Return the index of first non-repeating character
int first_non_repeating_index(char str[]) {
    int count[26] = {0};

    // Pass 1: Count frequencies
    for (int i = 0; str[i] != '\0'; i++) {
        count[str[i] - 'a']++;
    }

    // Pass 2: Find first with count = 1
    for (int i = 0; str[i] != '\0'; i++) {
        if (count[str[i] - 'a'] == 1) {
            return i;
        }
    }

    return -1;  // No non-repeating character
}

int main() {
    char str1[] = "leetcode";
    char result1 = first_non_repeating(str1);
    printf("\"%s\" → First non-repeating: ", str1);
    if (result1 != '\0') {
        printf("'%c'\n", result1);
    } else {
        printf("None\n");
    }

    char str2[] = "loveleetcode";
    char result2 = first_non_repeating(str2);
    printf("\"%s\" → First non-repeating: '%c'\n", str2, result2);

    char str3[] = "aabb";
    char result3 = first_non_repeating(str3);
    printf("\"%s\" → First non-repeating: ", str3);
    if (result3 != '\0') {
        printf("'%c'\n", result3);
    } else {
        printf("None (all repeat)\n");
    }

    char str4[] = "abcabc";
    printf("\"%s\" → Index: %d\n", str4, first_non_repeating_index(str4));

    char str5[] = "z";
    printf("\"%s\" → First non-repeating: '%c'\n", str5, first_non_repeating(str5));

    return 0;
}
