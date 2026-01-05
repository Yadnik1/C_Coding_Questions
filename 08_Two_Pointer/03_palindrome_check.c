/*
 * ============================================================================
 * PROBLEM: Palindrome Check using Two Pointers
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: VERY HIGH
 *
 * Pattern: OPPOSITE ENDS (Converging Pointers)
 *
 * ============================================================================
 * VISUALIZATION: "racecar"
 * ============================================================================
 *
 *   "racecar"
 *    ^     ^
 *   left  right
 *
 *   Step 1: 'r' == 'r' ✓
 *   Step 2: 'a' == 'a' ✓
 *   Step 3: 'c' == 'c' ✓
 *   Step 4: left >= right, STOP
 *
 *   All matched → IS PALINDROME
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why check left < right, not left <= right?"
 * A1: When left == right, we're at the middle character. A single character
 *     is always a palindrome with itself - no need to compare!
 *     Using < vs <= doesn't affect correctness but < is cleaner.
 *
 * -------------------------------------------------------------------------
 * Q2: "What about 'almost palindrome' - can delete one char?"
 * A2: When mismatch found, try two possibilities: skip left char OR skip right
 *     char. Check if either remaining substring is palindrome.
 *     Called "Valid Palindrome II" - still O(n).
 *
 * -------------------------------------------------------------------------
 * Q3: "Is there a way to check palindrome recursively?"
 * A3: Yes: isPalindrome(s, left, right) = (s[left]==s[right]) &&
 *     isPalindrome(s, left+1, right-1). But uses O(n) stack space vs O(1)
 *     iterative. Iterative is preferred.
 *
 * -------------------------------------------------------------------------
 * Q4: "How do you handle Unicode strings?"
 * A4: isalnum and tolower work for ASCII only. For Unicode: use proper
 *     Unicode-aware libraries, handle multi-byte characters correctly,
 *     and consider locale-specific case folding rules.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Basic palindrome check
bool isPalindrome(const char* str) {
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

// Palindrome check ignoring non-alphanumeric and case
bool isPalindromeAlphaNum(const char* str) {
    int left = 0;
    int right = strlen(str) - 1;

    while (left < right) {
        // Skip non-alphanumeric from left
        while (left < right && !isalnum(str[left])) {
            left++;
        }
        // Skip non-alphanumeric from right
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
    printf("=== Palindrome Check (Two Pointer) ===\n\n");

    printf("1. Basic palindrome:\n");
    printf("   \"racecar\": %s\n", isPalindrome("racecar") ? "YES" : "NO");
    printf("   \"hello\":   %s\n", isPalindrome("hello") ? "YES" : "NO");
    printf("   \"a\":       %s\n", isPalindrome("a") ? "YES" : "NO");
    printf("   \"\":        %s\n\n", isPalindrome("") ? "YES" : "NO");

    printf("2. Alphanumeric only (ignore spaces/punctuation):\n");
    printf("   \"A man, a plan, a canal: Panama\": %s\n",
           isPalindromeAlphaNum("A man, a plan, a canal: Panama") ? "YES" : "NO");
    printf("   \"race a car\": %s\n",
           isPalindromeAlphaNum("race a car") ? "YES" : "NO");
    printf("   \"Was it a car or a cat I saw?\": %s\n",
           isPalindromeAlphaNum("Was it a car or a cat I saw?") ? "YES" : "NO");

    return 0;
}
