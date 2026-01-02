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
