/*
 * ============================================================================
 * PROBLEM: Longest Palindromic Substring
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: HIGH
 *
 * Find the longest substring that is a palindrome.
 *
 * ============================================================================
 * APPROACH: Expand Around Center
 * ============================================================================
 *
 * For each position, expand outward while characters match.
 * Handle both odd-length (single center) and even-length (double center).
 *
 * ============================================================================
 * VISUALIZATION: "babad"
 * ============================================================================
 *
 *   Center at index 0 ('b'):
 *   - Odd:  "b" → length 1
 *   - Even: "ba" → not palindrome
 *
 *   Center at index 1 ('a'):
 *   - Odd:  "a" → expand → "bab" ✓ → length 3
 *   - Even: "ab" → not palindrome
 *
 *   Center at index 2 ('b'):
 *   - Odd:  "b" → expand → "aba" ✓ → length 3
 *   - Even: "ba" → not palindrome
 *
 *   Center at index 3 ('a'):
 *   - Odd:  "a" → length 1
 *   - Even: "ad" → not palindrome
 *
 *   Longest: "bab" or "aba" (length 3)
 *
 * ============================================================================
 * TIME: O(n²) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

// Expand around center and return length of palindrome
int expandAroundCenter(const char* s, int left, int right) {
    int n = strlen(s);

    while (left >= 0 && right < n && s[left] == s[right]) {
        left--;
        right++;
    }

    // Return length (right - left - 1 because we've gone one step too far)
    return right - left - 1;
}

void longestPalindrome(const char* s, char* result) {
    int n = strlen(s);

    if (n == 0) {
        result[0] = '\0';
        return;
    }

    int start = 0;
    int maxLen = 1;

    for (int i = 0; i < n; i++) {
        // Odd length palindrome (single center)
        int len1 = expandAroundCenter(s, i, i);

        // Even length palindrome (double center)
        int len2 = expandAroundCenter(s, i, i + 1);

        int len = (len1 > len2) ? len1 : len2;

        if (len > maxLen) {
            maxLen = len;
            start = i - (len - 1) / 2;
        }
    }

    // Copy result
    strncpy(result, s + start, maxLen);
    result[maxLen] = '\0';
}

// Helper: Check if string is palindrome
int isPalindrome(const char* s, int start, int end) {
    while (start < end) {
        if (s[start] != s[end]) return 0;
        start++;
        end--;
    }
    return 1;
}

// Brute force O(n³) for comparison
void longestPalindromeBrute(const char* s, char* result) {
    int n = strlen(s);
    int maxLen = 0;
    int start = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (isPalindrome(s, i, j)) {
                int len = j - i + 1;
                if (len > maxLen) {
                    maxLen = len;
                    start = i;
                }
            }
        }
    }

    strncpy(result, s + start, maxLen);
    result[maxLen] = '\0';
}

int main() {
    printf("=== Longest Palindromic Substring ===\n\n");

    char result[100];

    const char* test1 = "babad";
    longestPalindrome(test1, result);
    printf("Input:  \"%s\"\n", test1);
    printf("Output: \"%s\"\n\n", result);

    const char* test2 = "cbbd";
    longestPalindrome(test2, result);
    printf("Input:  \"%s\"\n", test2);
    printf("Output: \"%s\"\n\n", result);

    const char* test3 = "forgeeksskeegfor";
    longestPalindrome(test3, result);
    printf("Input:  \"%s\"\n", test3);
    printf("Output: \"%s\"\n\n", result);

    const char* test4 = "a";
    longestPalindrome(test4, result);
    printf("Input:  \"%s\"\n", test4);
    printf("Output: \"%s\"\n\n", result);

    printf("=== Algorithm ===\n");
    printf("1. For each index, expand around center\n");
    printf("2. Check both odd (i,i) and even (i,i+1) centers\n");
    printf("3. Track longest palindrome found\n");
    printf("\nTime: O(n²), Space: O(1)\n");

    return 0;
}
