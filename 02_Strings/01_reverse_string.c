/*
 * ============================================================================
 * PROBLEM: Reverse a String
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * Reverse a string in-place using two pointers.
 *
 * Example:
 * Input:  "hello"
 * Output: "olleh"
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. C STRINGS:
 *    - Null-terminated character arrays
 *    - strlen() gives length (excludes '\0')
 *    - Index from 0 to length-1
 *
 * 2. TWO-POINTER TECHNIQUE:
 *    - left pointer at start (index 0)
 *    - right pointer at end (index length-1)
 *    - Swap and move inward
 *
 * 3. IN-PLACE MODIFICATION:
 *    - No extra memory needed
 *    - Directly modify original string
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: "hello"
 * ============================================================================
 *
 * Initial:
 *   Index:  0   1   2   3   4
 *   String: h   e   l   l   o
 *           ^               ^
 *         left=0         right=4
 *
 * Step 1: Swap str[0] and str[4]
 *   temp = 'h'
 *   str[0] = 'o'
 *   str[4] = 'h'
 *
 *   String: o   e   l   l   h
 *               ^       ^
 *             left=1  right=3
 *
 * Step 2: Swap str[1] and str[3]
 *   temp = 'e'
 *   str[1] = 'l'
 *   str[3] = 'e'
 *
 *   String: o   l   l   e   h
 *                   ^
 *               left=2, right=2
 *
 * Step 3: left >= right, STOP
 *
 * Result: "olleh"
 *
 * ============================================================================
 * SWAP LOGIC VISUALIZATION:
 * ============================================================================
 *
 * Before swap:
 *   +-----+   +-----+
 *   |  A  |   |  B  |
 *   +-----+   +-----+
 *    left      right
 *
 * Step 1: Save left in temp
 *   +-----+   +-----+   +-----+
 *   |  A  |   |  B  |   |  A  |
 *   +-----+   +-----+   +-----+
 *    left      right     temp
 *
 * Step 2: Copy right to left
 *   +-----+   +-----+   +-----+
 *   |  B  |   |  B  |   |  A  |
 *   +-----+   +-----+   +-----+
 *    left      right     temp
 *
 * Step 3: Copy temp to right
 *   +-----+   +-----+   +-----+
 *   |  B  |   |  A  |   |  A  |
 *   +-----+   +-----+   +-----+
 *    left      right     temp
 *
 * SWAPPED!
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   left = 0
 *   right = strlen(str) - 1
 *
 *   while (left < right):
 *       swap(str[left], str[right])
 *       left++
 *       right--
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - n/2 swaps for string of length n
 * - O(n/2) = O(n)
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only temp variable for swap
 * - In-place modification
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Empty string "": Nothing to reverse
 * 2. Single character "a": Already reversed
 * 3. Palindrome "racecar": Result same as input
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

void reverse_string(char str[]) {
    int left = 0;
    int right = strlen(str) - 1;

    while (left < right) {
        // Swap characters
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;

        // Move pointers
        left++;
        right--;
    }
}

int main() {
    char str1[] = "hello";
    printf("Original: \"%s\"\n", str1);
    reverse_string(str1);
    printf("Reversed: \"%s\"\n", str1);

    char str2[] = "world";
    printf("\nOriginal: \"%s\"\n", str2);
    reverse_string(str2);
    printf("Reversed: \"%s\"\n", str2);

    char str3[] = "a";
    printf("\nOriginal: \"%s\"\n", str3);
    reverse_string(str3);
    printf("Reversed: \"%s\"\n", str3);

    char str4[] = "racecar";
    printf("\nOriginal: \"%s\"\n", str4);
    reverse_string(str4);
    printf("Reversed: \"%s\"\n", str4);

    return 0;
}
