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
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "How do you handle spaces and punctuation?"
 * A1: Skip non-alphanumeric characters using isalnum():
 *     while (!isalnum(str[left])) left++;
 *     while (!isalnum(str[right])) right--;
 *
 * -------------------------------------------------------------------------
 * Q2: "What about case sensitivity?"
 * A2: Use tolower() or toupper() when comparing:
 *     if (tolower(str[left]) != tolower(str[right]))
 *
 * -------------------------------------------------------------------------
 * Q3: "Can you check palindrome using recursion?"
 * A3: Yes, but uses O(n) stack space:
 *     bool isPal(str, l, r) {
 *         if (l >= r) return true;
 *         if (str[l] != str[r]) return false;
 *         return isPal(str, l+1, r-1);
 *     }
 *
 * -------------------------------------------------------------------------
 * Q4: "What about checking if string can become palindrome?"
 * A4: Count character frequencies. For palindrome:
 *     - At most ONE character can have odd frequency
 *     - That character goes in the middle
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Basic palindrome check
bool is_palindrome(char str[]) {
    // Initialize left pointer at start
    // Say: "I'll use two pointers to compare characters from both ends"
    int left = 0;

    // Initialize right pointer at end
    // Say: "Left starts at 0, right starts at the last character"
    int right = strlen(str) - 1;

    // Compare characters until pointers meet
    // Say: "I'll keep comparing until the pointers cross"
    while (left < right) {
        // Check if characters at both ends match
        // Say: "If characters don't match, it's not a palindrome"
        if (str[left] != str[right]) {
            // Say: "Found a mismatch, so I return false immediately"
            return false;
        }

        // Move left pointer forward
        // Say: "Characters match, so I move both pointers toward the center"
        left++;

        // Move right pointer backward
        right--;
    }

    // All characters matched
    // Say: "All characters matched, so it's a palindrome"
    return true;
}

// Case-insensitive palindrome check
bool is_palindrome_ignore_case(char str[]) {
    // Initialize left pointer at start
    // Say: "For case-insensitive checking, I'll use the same two-pointer approach"
    int left = 0;

    // Initialize right pointer at end
    // Say: "Left at 0, right at the last character"
    int right = strlen(str) - 1;

    // Compare characters until pointers meet
    // Say: "I'll compare characters ignoring case differences"
    while (left < right) {
        // Check if lowercase versions match
        // Say: "I convert both characters to lowercase before comparing"
        if (tolower(str[left]) != tolower(str[right])) {
            // Say: "Found a mismatch even after lowercasing, return false"
            return false;
        }

        // Move left pointer forward
        // Say: "Characters match, move both pointers inward"
        left++;

        // Move right pointer backward
        right--;
    }

    // All characters matched
    // Say: "All characters matched ignoring case, it's a palindrome"
    return true;
}

// Ignore non-alphanumeric characters
bool is_palindrome_alphanumeric(char str[]) {
    // Initialize left pointer at start
    // Say: "For alphanumeric-only checking, I start with two pointers"
    int left = 0;

    // Initialize right pointer at end
    // Say: "Left at 0, right at the end"
    int right = strlen(str) - 1;

    // Process until pointers meet
    // Say: "I'll skip non-alphanumeric characters and compare only letters and digits"
    while (left < right) {
        // Skip non-alphanumeric characters from left
        // Say: "First, I skip any spaces or punctuation from the left"
        while (left < right && !isalnum(str[left])) {
            // Say: "Moving left pointer past non-alphanumeric characters"
            left++;
        }

        // Skip non-alphanumeric characters from right
        // Say: "Then, I skip spaces or punctuation from the right"
        while (left < right && !isalnum(str[right])) {
            // Say: "Moving right pointer past non-alphanumeric characters"
            right--;
        }

        // Compare the alphanumeric characters (case-insensitive)
        // Say: "Now I compare the actual alphanumeric characters, ignoring case"
        if (tolower(str[left]) != tolower(str[right])) {
            // Say: "Characters don't match, not a palindrome"
            return false;
        }

        // Move both pointers inward
        // Say: "Characters match, move both pointers toward center"
        left++;
        right--;
    }

    // All alphanumeric characters matched
    // Say: "All alphanumeric characters matched, it's a palindrome"
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
