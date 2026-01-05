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
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "What's the difference between reversing string and array?"
 * A1: Same algorithm! But strings have null terminator '\0' which we
 *     must preserve. Use strlen() to find the actual content length.
 *
 * -------------------------------------------------------------------------
 * Q2: "How would you reverse words in a sentence?"
 * A2: Two-step approach:
 *     1. Reverse entire string
 *     2. Reverse each word individually
 *
 *     "hello world" → "dlrow olleh" → "world hello"
 *
 * -------------------------------------------------------------------------
 * Q3: "Can you reverse without extra space (temp variable)?"
 * A3: Yes, using XOR swap, but it's error-prone and not recommended.
 *     Temp variable is cleaner and works in all cases.
 *
 * -------------------------------------------------------------------------
 * Q4: "What about Unicode/UTF-8 strings?"
 * A4: More complex! Multi-byte characters can't be swapped byte-by-byte.
 *     Need to identify character boundaries first. C stdlib doesn't
 *     handle this well - use wide characters (wchar_t).
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

void reverse_string(char str[]) {
    // Initialize left pointer at the start
    // Say: "I'll use two pointers to reverse the string in place"
    int left = 0;

    // Initialize right pointer at the end
    // Say: "Left starts at index 0, right starts at the last character"
    int right = strlen(str) - 1;

    // Keep swapping until pointers meet in the middle
    // Say: "I'll swap characters from both ends and move the pointers inward"
    while (left < right) {
        // Save the left character in a temporary variable
        // Say: "First, I save the left character in temp to avoid losing it"
        char temp = str[left];

        // Copy right character to left position
        // Say: "Then I copy the right character to the left position"
        str[left] = str[right];

        // Copy saved character to right position
        // Say: "Finally, I copy the saved character to the right position"
        str[right] = temp;

        // Move left pointer one step to the right
        // Say: "Now I move both pointers toward the center"
        left++;

        // Move right pointer one step to the left
        right--;
    }
    // Say: "Once the pointers cross, the string is fully reversed"
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
