/*
 * ============================================================================
 * PROBLEM: Check if Two Strings are Anagrams
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * Anagrams are words formed by rearranging letters of another word.
 *
 * Example:
 * Input:  "listen", "silent" → true
 * Input:  "hello", "world"   → false
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. CHARACTER FREQUENCY:
 *    - Count occurrences of each character
 *    - Anagrams have same frequency counts
 *
 * 2. ASCII VALUE:
 *    - 'a' = 97, 'z' = 122
 *    - Use char - 'a' to get index 0-25
 *
 * 3. TWO APPROACHES:
 *    - Sorting: O(n log n) time, O(1) space
 *    - Counting: O(n) time, O(1) space (26 chars)
 *
 * ============================================================================
 * METHOD 1: SORTING
 * ============================================================================
 *
 * "listen" → sort → "eilnst"
 * "silent" → sort → "eilnst"
 *
 * Both sorted strings are equal → ANAGRAM!
 *
 * ============================================================================
 * METHOD 2: CHARACTER COUNT (OPTIMAL)
 * ============================================================================
 *
 * "listen" and "silent"
 *
 * Create count array of 26 characters:
 *
 * For "listen": increment count[char - 'a']
 *   count['l'-'a'] = count[11]++  → count[11] = 1
 *   count['i'-'a'] = count[8]++   → count[8] = 1
 *   count['s'-'a'] = count[18]++  → count[18] = 1
 *   count['t'-'a'] = count[19]++  → count[19] = 1
 *   count['e'-'a'] = count[4]++   → count[4] = 1
 *   count['n'-'a'] = count[13]++  → count[13] = 1
 *
 * For "silent": decrement count[char - 'a']
 *   count['s'-'a']--  → count[18] = 0
 *   count['i'-'a']--  → count[8] = 0
 *   count['l'-'a']--  → count[11] = 0
 *   count['e'-'a']--  → count[4] = 0
 *   count['n'-'a']--  → count[13] = 0
 *   count['t'-'a']--  → count[19] = 0
 *
 * All counts are 0 → ANAGRAM!
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 * String 1: "listen"
 * String 2: "silent"
 *
 * Character frequency table:
 *
 *   Char | 'e' | 'i' | 'l' | 'n' | 's' | 't' |
 *   -----|-----|-----|-----|-----|-----|-----|
 *   Idx  |  4  |  8  | 11  | 13  | 18  | 19  |
 *   -----|-----|-----|-----|-----|-----|-----|
 *   str1 |  +1 |  +1 |  +1 |  +1 |  +1 |  +1 |
 *   str2 |  -1 |  -1 |  -1 |  -1 |  -1 |  -1 |
 *   -----|-----|-----|-----|-----|-----|-----|
 *   Net  |  0  |  0  |  0  |  0  |  0  |  0  |
 *
 * All zeros → Anagram!
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   if len(str1) != len(str2):
 *       return false
 *
 *   count[26] = {0}
 *
 *   for i = 0 to len:
 *       count[str1[i] - 'a']++
 *       count[str2[i] - 'a']--
 *
 *   for i = 0 to 25:
 *       if count[i] != 0:
 *           return false
 *
 *   return true
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - One pass through both strings: O(n)
 * - One pass through count array: O(26) = O(1)
 * - Total: O(n)
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Fixed size array of 26 characters
 * - Doesn't grow with input size
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Different lengths: Not anagrams
 * 2. Empty strings: Anagrams of each other
 * 3. Same string: Anagram of itself
 * 4. Case sensitive: 'A' != 'a' (handle if needed)
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Using character count (optimal)
bool are_anagrams(char str1[], char str2[]) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    // Different lengths can't be anagrams
    if (len1 != len2) {
        return false;
    }

    // Count array for 26 lowercase letters
    int count[26] = {0};

    // Increment for str1, decrement for str2
    for (int i = 0; i < len1; i++) {
        count[str1[i] - 'a']++;
        count[str2[i] - 'a']--;
    }

    // Check if all counts are zero
    for (int i = 0; i < 26; i++) {
        if (count[i] != 0) {
            return false;
        }
    }

    return true;
}

// Case-insensitive version
bool are_anagrams_ignore_case(char str1[], char str2[]) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if (len1 != len2) {
        return false;
    }

    int count[26] = {0};

    for (int i = 0; i < len1; i++) {
        count[tolower(str1[i]) - 'a']++;
        count[tolower(str2[i]) - 'a']--;
    }

    for (int i = 0; i < 26; i++) {
        if (count[i] != 0) {
            return false;
        }
    }

    return true;
}

int main() {
    printf("=== Anagram Check ===\n\n");

    printf("\"listen\" & \"silent\": %s\n",
           are_anagrams("listen", "silent") ? "ANAGRAM" : "NOT anagram");

    printf("\"hello\" & \"world\":   %s\n",
           are_anagrams("hello", "world") ? "ANAGRAM" : "NOT anagram");

    printf("\"anagram\" & \"nagaram\": %s\n",
           are_anagrams("anagram", "nagaram") ? "ANAGRAM" : "NOT anagram");

    printf("\"rat\" & \"car\":       %s\n",
           are_anagrams("rat", "car") ? "ANAGRAM" : "NOT anagram");

    printf("\n=== Case-Insensitive ===\n");
    printf("\"Listen\" & \"Silent\": %s\n",
           are_anagrams_ignore_case("Listen", "Silent") ? "ANAGRAM" : "NOT anagram");

    return 0;
}
