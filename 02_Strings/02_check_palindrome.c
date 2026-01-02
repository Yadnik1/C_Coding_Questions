/*
 * ============================================================================
 * PROBLEM: Check if String is Palindrome
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * A palindrome reads the same forward and backward.
 *
 * Example:
 * Input:  "racecar" → true
 * Input:  "hello"   → false
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. TWO-POINTER COMPARISON:
 *    - Compare characters from both ends
 *    - Move toward center
 *    - Any mismatch = not palindrome
 *
 * 2. EARLY EXIT:
 *    - Return false immediately on first mismatch
 *    - Efficient for non-palindromes
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: "racecar"
 * ============================================================================
 *
 *   Index:   0   1   2   3   4   5   6
 *   String:  r   a   c   e   c   a   r
 *            ^                       ^
 *          left=0                 right=6
 *
 * Iteration 1:
 *   Compare 'r' == 'r' ✓ MATCH
 *   Move: left=1, right=5
 *
 * Iteration 2:
 *   Compare 'a' == 'a' ✓ MATCH
 *   Move: left=2, right=4
 *
 * Iteration 3:
 *   Compare 'c' == 'c' ✓ MATCH
 *   Move: left=3, right=3
 *
 * Iteration 4:
 *   left >= right, STOP
 *   All matched → PALINDROME!
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: "hello" (NOT a palindrome)
 * ============================================================================
 *
 *   Index:   0   1   2   3   4
 *   String:  h   e   l   l   o
 *            ^               ^
 *          left=0         right=4
 *
 * Iteration 1:
 *   Compare 'h' != 'o' ✗ MISMATCH
 *   Return false immediately
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   left = 0
 *   right = length - 1
 *
 *   while (left < right):
 *       if str[left] != str[right]:
 *           return false
 *       left++
 *       right--
 *
 *   return true
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Best case: O(1) - first mismatch
 * - Worst case: O(n/2) = O(n) - is palindrome
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only two pointer variables
 * - Constant space
 *
 * ============================================================================
 * VARIATIONS:
 * ============================================================================
 *
 * 1. Case-insensitive:
 *    if (tolower(str[left]) != tolower(str[right]))
 *
 * 2. Ignore non-alphanumeric:
 *    Skip spaces and punctuation
 *    "A man, a plan, a canal: Panama" → true
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Basic palindrome check
bool is_palindrome(char str[]) {
    int left = 0;
    int right = strlen(str) - 1;

    while (left < right) {
        if (str[left] != str[right]) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

// Case-insensitive palindrome check
bool is_palindrome_ignore_case(char str[]) {
    int left = 0;
    int right = strlen(str) - 1;

    while (left < right) {
        if (tolower(str[left]) != tolower(str[right])) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

// Ignore non-alphanumeric characters
bool is_palindrome_alphanumeric(char str[]) {
    int left = 0;
    int right = strlen(str) - 1;

    while (left < right) {
        // Skip non-alphanumeric on left
        while (left < right && !isalnum(str[left])) {
            left++;
        }
        // Skip non-alphanumeric on right
        while (left < right && !isalnum(str[right])) {
            right--;
        }

        if (tolower(str[left]) != tolower(str[right])) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

int main() {
    printf("=== Basic Palindrome Check ===\n");
    printf("\"racecar\": %s\n", is_palindrome("racecar") ? "YES" : "NO");
    printf("\"hello\":   %s\n", is_palindrome("hello") ? "YES" : "NO");
    printf("\"madam\":   %s\n", is_palindrome("madam") ? "YES" : "NO");
    printf("\"a\":       %s\n", is_palindrome("a") ? "YES" : "NO");

    printf("\n=== Case-Insensitive ===\n");
    printf("\"RaceCar\": %s\n", is_palindrome_ignore_case("RaceCar") ? "YES" : "NO");
    printf("\"Madam\":   %s\n", is_palindrome_ignore_case("Madam") ? "YES" : "NO");

    printf("\n=== Ignore Non-Alphanumeric ===\n");
    printf("\"A man, a plan, a canal: Panama\": %s\n",
           is_palindrome_alphanumeric("A man, a plan, a canal: Panama") ? "YES" : "NO");
    printf("\"race a car\": %s\n",
           is_palindrome_alphanumeric("race a car") ? "YES" : "NO");

    return 0;
}
