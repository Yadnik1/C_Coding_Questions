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
    // Get string length for boundary checking
    // Say: "This helper expands around a center to find palindrome length"
    int n = strlen(s);

    // Expand while characters match
    // Say: "I expand outward while characters on both sides match"
    while (left >= 0 && right < n && s[left] == s[right]) {
        // Say: "Characters match, so I expand further outward"
        left--;
        right++;
    }

    // Return length (right - left - 1 because we've gone one step too far)
    // Say: "When characters don't match, I calculate the palindrome length"
    return right - left - 1;
}

void longestPalindrome(const char* s, char* result) {
    // Get string length
    // Say: "I'll find the longest palindromic substring using expand-around-center"
    int n = strlen(s);

    // Handle empty string
    // Say: "If the string is empty, I return an empty result"
    if (n == 0) {
        result[0] = '\0';
        return;
    }

    // Track start position and max length
    // Say: "I'll track the start position and maximum length found"
    int start = 0;

    // Initialize max length to 1
    // Say: "Every single character is a palindrome of length 1"
    int maxLen = 1;

    // Try each position as a center
    // Say: "I'll try each position as a potential palindrome center"
    for (int i = 0; i < n; i++) {
        // Odd length palindrome (single center)
        // Say: "First, I check for odd-length palindromes with this position as center"
        int len1 = expandAroundCenter(s, i, i);

        // Even length palindrome (double center)
        // Say: "Then I check for even-length palindromes with this and next position as centers"
        int len2 = expandAroundCenter(s, i, i + 1);

        // Take the longer of the two
        // Say: "I take whichever is longer: odd or even length palindrome"
        int len = (len1 > len2) ? len1 : len2;

        // Update if this is the longest so far
        // Say: "If this palindrome is longer than the current maximum, I update it"
        if (len > maxLen) {
            // Say: "Update the maximum length"
            maxLen = len;

            // Calculate start position
            // Say: "I calculate the start position based on center and length"
            start = i - (len - 1) / 2;
        }
    }

    // Copy result
    // Say: "Finally, I extract the palindrome substring from the original string"
    strncpy(result, s + start, maxLen);

    // Null terminate
    // Say: "And add a null terminator to complete the result"
    result[maxLen] = '\0';
}

// Helper: Check if string is palindrome
int isPalindrome(const char* s, int start, int end) {
    // Check palindrome property
    // Say: "This helper checks if a substring is a palindrome"
    while (start < end) {
        // Check if characters match
        // Say: "I compare characters from both ends"
        if (s[start] != s[end]) return 0;

        // Move inward
        // Say: "Move both pointers toward the center"
        start++;
        end--;
    }

    // Is palindrome
    // Say: "All characters matched, so it's a palindrome"
    return 1;
}

// Brute force O(n³) for comparison
void longestPalindromeBrute(const char* s, char* result) {
    // Get string length
    // Say: "This brute force approach checks all possible substrings"
    int n = strlen(s);

    // Track maximum length
    // Say: "I track the maximum palindrome length found"
    int maxLen = 0;

    // Track start of longest palindrome
    // Say: "And the start position of the longest palindrome"
    int start = 0;

    // Try all starting positions
    // Say: "I try every possible starting position"
    for (int i = 0; i < n; i++) {
        // Try all ending positions from this start
        // Say: "For each start, I try every possible ending position"
        for (int j = i; j < n; j++) {
            // Check if this substring is palindrome
            // Say: "I check if this substring is a palindrome"
            if (isPalindrome(s, i, j)) {
                // Calculate length
                // Say: "If it's a palindrome, I calculate its length"
                int len = j - i + 1;

                // Update if longer
                // Say: "If it's longer than the current max, I update it"
                if (len > maxLen) {
                    maxLen = len;
                    start = i;
                }
            }
        }
    }

    // Copy result
    // Say: "Finally, I extract the longest palindrome found"
    strncpy(result, s + start, maxLen);

    // Null terminate
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
