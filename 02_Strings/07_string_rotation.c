/*
 * ============================================================================
 * PROBLEM: Check if String is Rotation of Another
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: High
 *
 * Check if one string is a rotation of another string.
 *
 * Example:
 * Input:  "waterbottle", "erbottlewat"
 * Output: true
 *
 * Input:  "hello", "lohel"
 * Output: false
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. ROTATION PROPERTY:
 *    - If B is rotation of A, then B is substring of A+A
 *    - Example: "erbottlewat" is in "waterbottlewaterbottle"
 *
 * 2. CONCATENATION TRICK:
 *    - Concatenate first string with itself
 *    - Check if second string is substring
 *
 * 3. PREREQUISITE:
 *    - Both strings must have same length
 *
 * ============================================================================
 * VISUAL UNDERSTANDING:
 * ============================================================================
 *
 * Original:  "waterbottle"
 * Rotated:   "erbottlewat"
 *
 * How rotation works:
 *
 *   "waterbottle"
 *      ↓↓↓
 *   Move "wat" from start to end
 *      ↓↓↓
 *   "erbottle" + "wat" = "erbottlewat"
 *
 * ============================================================================
 * THE MAGIC TRICK: CONCATENATION
 * ============================================================================
 *
 * Concatenate A with itself:
 *
 *   A + A = "waterbottle" + "waterbottle"
 *         = "waterbottlewaterbottle"
 *
 * Now look for B in A+A:
 *
 *   "waterbottlewaterbottle"
 *          ^^^^^^^^^^^
 *         "erbottlewat"
 *
 * Found! So "erbottlewat" IS a rotation of "waterbottle"
 *
 * ============================================================================
 * WHY THIS WORKS:
 * ============================================================================
 *
 * Any rotation splits the string into two parts:
 *
 *   Original: [FIRST_PART][SECOND_PART]
 *   Rotated:  [SECOND_PART][FIRST_PART]
 *
 * When we concatenate original with itself:
 *
 *   [FIRST][SECOND] + [FIRST][SECOND]
 *   = [FIRST][SECOND][FIRST][SECOND]
 *              ↑_____________↑
 *              [SECOND][FIRST]
 *              = ROTATED STRING!
 *
 * Every possible rotation appears in A+A!
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   if len(A) != len(B):
 *       return false
 *
 *   concatenated = A + A
 *
 *   if B is substring of concatenated:
 *       return true
 *   else:
 *       return false
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Concatenation: O(n)
 * - Substring search (strstr): O(n) average
 * - Total: O(n)
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(n)
 * ============================================================================
 * - Need space for concatenated string
 * - Size = 2n + 1
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Different lengths: Not rotation
 * 2. Empty strings: Both empty = rotation
 * 3. Same string: Always a rotation (0 rotation)
 * 4. Single character: Same char = rotation
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool is_rotation(char str1[], char str2[]) {
    // Get length of first string
    // Say: "I'll first check if both strings have the same length"
    int len1 = strlen(str1);

    // Get length of second string
    int len2 = strlen(str2);

    // Must be same length
    // Say: "If lengths are different, they can't be rotations"
    if (len1 != len2) {
        return false;
    }

    // Empty strings are rotations of each other
    // Say: "Empty strings are considered rotations of each other"
    if (len1 == 0) {
        return true;
    }

    // Concatenate str1 with itself
    // Say: "Now I'll use the concatenation trick: I'll create str1 plus str1"
    char *concat = (char *)malloc(2 * len1 + 1);

    // Copy first instance of str1
    // Say: "First, I copy str1 to the concatenated string"
    strcpy(concat, str1);

    // Append second instance of str1
    // Say: "Then I append str1 again to create the doubled string"
    strcat(concat, str1);

    // Check if str2 is substring of concat
    // Say: "If str2 is a rotation, it will be a substring of this doubled string"
    bool result = (strstr(concat, str2) != NULL);

    // Free allocated memory
    // Say: "Finally, I free the memory and return the result"
    free(concat);
    return result;
}

// Alternative: Manual check without extra space
bool is_rotation_manual(char str1[], char str2[]) {
    // Get length and check equality
    // Say: "This alternative method tries all possible rotation points"
    int len = strlen(str1);

    // Check if lengths match
    if (len != strlen(str2)) return false;

    // Handle empty strings
    if (len == 0) return true;

    // Try each rotation point
    // Say: "I'll try rotating at each possible position"
    for (int i = 0; i < len; i++) {
        // Assume this rotation works
        // Say: "For each rotation point, I check if all characters match"
        bool match = true;

        // Check all positions for this rotation
        // Say: "I compare characters using modulo arithmetic for wrapping"
        for (int j = 0; j < len; j++) {
            // Check if characters match at this position
            // Say: "I check if str2 at position j matches str1 at rotated position"
            if (str2[j] != str1[(i + j) % len]) {
                // Mismatch found
                // Say: "If there's a mismatch, this rotation point doesn't work"
                match = false;
                break;
            }
        }

        // If all matched, found rotation
        // Say: "If all characters matched, I found a valid rotation"
        if (match) return true;
    }

    // No rotation found
    // Say: "If no rotation point works, they're not rotations"
    return false;
}

int main() {
    printf("=== String Rotation Check ===\n\n");

    printf("\"waterbottle\" & \"erbottlewat\": %s\n",
           is_rotation("waterbottle", "erbottlewat") ? "YES (rotation)" : "NO");

    printf("\"hello\" & \"lohel\": %s\n",
           is_rotation("hello", "lohel") ? "YES (rotation)" : "NO");

    printf("\"hello\" & \"llohe\": %s\n",
           is_rotation("hello", "llohe") ? "YES (rotation)" : "NO");

    printf("\"abcd\" & \"abcd\": %s\n",
           is_rotation("abcd", "abcd") ? "YES (rotation)" : "NO");

    printf("\"abc\" & \"ab\": %s\n",
           is_rotation("abc", "ab") ? "YES (rotation)" : "NO");

    printf("\n=== Showing All Rotations of \"hello\" ===\n");
    char str[] = "hello";
    int len = strlen(str);
    printf("Original: %s\n", str);
    printf("Rotations:\n");
    for (int i = 1; i < len; i++) {
        printf("  Rotate %d: ", i);
        for (int j = 0; j < len; j++) {
            printf("%c", str[(i + j) % len]);
        }
        printf("\n");
    }

    return 0;
}
