/*
 * ============================================================================
 *                    SLIDING WINDOW TECHNIQUE - COMPLETE THEORY
 * ============================================================================
 *
 * Sliding Window is a technique for processing arrays/strings by maintaining
 * a "window" that slides through the data, avoiding redundant calculations.
 *
 * KEY INSIGHT: Instead of recalculating everything for each position,
 *              we ADD the new element and REMOVE the old element.
 *
 * ============================================================================
 * TWO TYPES OF SLIDING WINDOW:
 * ============================================================================
 *
 * 1. FIXED SIZE WINDOW
 *    - Window size is constant (given as K)
 *    - Examples: Max sum of K elements, average of K elements
 *
 * 2. VARIABLE SIZE WINDOW
 *    - Window expands/shrinks based on condition
 *    - Examples: Smallest subarray with sum >= target, longest substring
 *
 * ============================================================================
 * FIXED SIZE WINDOW - VISUALIZATION
 * ============================================================================
 *
 * Problem: Find max sum of 3 consecutive elements
 * Array: [2, 1, 5, 1, 3, 2], K = 3
 *
 *   Step 1: Build first window
 *   [2, 1, 5, 1, 3, 2]
 *    ^-----^
 *    window = [2,1,5], sum = 8
 *
 *   Step 2: Slide - Remove 2, Add 1
 *   [2, 1, 5, 1, 3, 2]
 *       ^-----^
 *    sum = 8 - 2 + 1 = 7
 *
 *   Step 3: Slide - Remove 1, Add 3
 *   [2, 1, 5, 1, 3, 2]
 *          ^-----^
 *    sum = 7 - 1 + 3 = 9  ← MAX
 *
 *   Step 4: Slide - Remove 5, Add 2
 *   [2, 1, 5, 1, 3, 2]
 *             ^-----^
 *    sum = 9 - 5 + 2 = 6
 *
 *   Answer: 9
 *
 * ============================================================================
 * FIXED SIZE WINDOW - TEMPLATE
 * ============================================================================
 *
 *   int fixedWindowTemplate(int arr[], int n, int k) {
 *       // Step 1: Build first window
 *       int windowSum = 0;
 *       for (int i = 0; i < k; i++) {
 *           windowSum += arr[i];
 *       }
 *       int maxSum = windowSum;
 *
 *       // Step 2: Slide the window
 *       for (int i = k; i < n; i++) {
 *           windowSum += arr[i];      // Add new element (right)
 *           windowSum -= arr[i - k];  // Remove old element (left)
 *           maxSum = max(maxSum, windowSum);
 *       }
 *
 *       return maxSum;
 *   }
 *
 * ============================================================================
 * VARIABLE SIZE WINDOW - VISUALIZATION
 * ============================================================================
 *
 * Problem: Smallest subarray with sum >= 7
 * Array: [2, 1, 5, 2, 3, 2], target = 7
 *
 *   left = 0, right = 0, sum = 0
 *
 *   Expand right until sum >= target:
 *   [2, 1, 5, 2, 3, 2]
 *    ^
 *    sum = 2 < 7, expand
 *
 *   [2, 1, 5, 2, 3, 2]
 *    ^--^
 *    sum = 3 < 7, expand
 *
 *   [2, 1, 5, 2, 3, 2]
 *    ^-----^
 *    sum = 8 >= 7 ✓ length = 3
 *
 *   Shrink left while sum >= target:
 *   [2, 1, 5, 2, 3, 2]
 *       ^--^
 *    sum = 6 < 7, stop shrinking
 *
 *   Expand right:
 *   [2, 1, 5, 2, 3, 2]
 *       ^-----^
 *    sum = 8 >= 7 ✓ length = 3
 *
 *   Shrink left:
 *   [2, 1, 5, 2, 3, 2]
 *          ^--^
 *    sum = 7 >= 7 ✓ length = 2  ← MIN
 *
 *   Shrink left:
 *   [2, 1, 5, 2, 3, 2]
 *             ^
 *    sum = 2 < 7, stop
 *
 *   Continue... Answer: 2
 *
 * ============================================================================
 * VARIABLE SIZE WINDOW - TEMPLATE
 * ============================================================================
 *
 *   int variableWindowTemplate(int arr[], int n, int target) {
 *       int left = 0;
 *       int sum = 0;
 *       int minLen = INT_MAX;
 *
 *       for (int right = 0; right < n; right++) {
 *           // EXPAND: Add element at right
 *           sum += arr[right];
 *
 *           // SHRINK: While condition is satisfied
 *           while (sum >= target) {
 *               minLen = min(minLen, right - left + 1);
 *               sum -= arr[left];  // Remove element at left
 *               left++;
 *           }
 *       }
 *
 *       return minLen;
 *   }
 *
 * ============================================================================
 * COMMON SLIDING WINDOW PROBLEMS:
 * ============================================================================
 *
 * FIXED SIZE:
 *   1. Maximum sum of K consecutive elements
 *   2. Average of K consecutive elements
 *   3. Maximum of each subarray of size K
 *   4. First negative in every window of size K
 *
 * VARIABLE SIZE:
 *   1. Smallest subarray with sum >= S
 *   2. Longest substring with K distinct characters
 *   3. Longest substring without repeating characters
 *   4. Max consecutive ones with K flips allowed
 *   5. Minimum window substring
 *
 * ============================================================================
 * HOW TO IDENTIFY SLIDING WINDOW PROBLEMS:
 * ============================================================================
 *
 * Look for these keywords:
 *   ✓ "Subarray" or "Substring"
 *   ✓ "Consecutive elements"
 *   ✓ "Window of size K"
 *   ✓ "Longest/Shortest" subarray with some condition
 *   ✓ "Maximum/Minimum" sum of subarray
 *
 * Ask yourself:
 *   1. Am I looking at contiguous elements?
 *   2. Can I avoid recalculating by reusing previous computation?
 *   3. Does adding/removing elements update my answer efficiently?
 *
 * ============================================================================
 * COMPLEXITY:
 * ============================================================================
 *
 * Time:  O(n) - Each element is added once and removed once
 * Space: O(1) - Only pointers and running sum/count
 *
 * Compare to brute force O(n*k) or O(n²) - huge improvement!
 *
 * ============================================================================
 * COMMON MISTAKES:
 * ============================================================================
 *
 * 1. Off-by-one errors in window boundaries
 * 2. Forgetting to handle empty array or k > n
 * 3. Not updating max/min at the right time
 * 4. Shrinking too much or too little in variable window
 *
 * ============================================================================
 */

#include <stdio.h>
#include <limits.h>

// ============================================================================
// FIXED SIZE WINDOW: Maximum sum of K elements
// ============================================================================
int maxSumFixedWindow(int arr[], int n, int k) {
    if (n < k) return -1;

    // Build first window
    int windowSum = 0;
    for (int i = 0; i < k; i++) {
        windowSum += arr[i];
    }
    int maxSum = windowSum;

    // Slide the window
    for (int i = k; i < n; i++) {
        windowSum += arr[i] - arr[i - k];  // Add new, remove old
        if (windowSum > maxSum) {
            maxSum = windowSum;
        }
    }

    return maxSum;
}

// ============================================================================
// VARIABLE SIZE WINDOW: Smallest subarray with sum >= target
// ============================================================================
int minSubarrayLen(int target, int arr[], int n) {
    int left = 0;
    int sum = 0;
    int minLen = INT_MAX;

    for (int right = 0; right < n; right++) {
        sum += arr[right];  // Expand

        while (sum >= target) {
            int len = right - left + 1;
            if (len < minLen) {
                minLen = len;
            }
            sum -= arr[left];  // Shrink
            left++;
        }
    }

    return (minLen == INT_MAX) ? 0 : minLen;
}

// ============================================================================
// VARIABLE SIZE WINDOW: Longest substring without repeating characters
// ============================================================================
int longestUniqueSubstring(char* s) {
    int n = 0;
    while (s[n]) n++;  // strlen

    if (n == 0) return 0;

    int seen[128] = {0};  // ASCII characters
    int left = 0;
    int maxLen = 0;

    for (int right = 0; right < n; right++) {
        char c = s[right];

        // Shrink while duplicate exists
        while (seen[(int)c]) {
            seen[(int)s[left]] = 0;
            left++;
        }

        seen[(int)c] = 1;

        int len = right - left + 1;
        if (len > maxLen) {
            maxLen = len;
        }
    }

    return maxLen;
}

// ============================================================================
// FIXED SIZE: Count occurrences of anagram in string
// ============================================================================
int countAnagrams(char* text, char* pattern) {
    int textLen = 0, patLen = 0;
    while (text[textLen]) textLen++;
    while (pattern[patLen]) patLen++;

    if (textLen < patLen) return 0;

    int patCount[26] = {0};
    int winCount[26] = {0};

    // Count pattern characters
    for (int i = 0; i < patLen; i++) {
        patCount[pattern[i] - 'a']++;
    }

    int count = 0;

    for (int i = 0; i < textLen; i++) {
        // Add character to window
        winCount[text[i] - 'a']++;

        // Remove character leaving window
        if (i >= patLen) {
            winCount[text[i - patLen] - 'a']--;
        }

        // Check if window matches pattern
        if (i >= patLen - 1) {
            int match = 1;
            for (int j = 0; j < 26; j++) {
                if (winCount[j] != patCount[j]) {
                    match = 0;
                    break;
                }
            }
            if (match) count++;
        }
    }

    return count;
}

int main() {
    printf("=== Sliding Window Technique ===\n\n");

    // Fixed size window
    printf("1. FIXED SIZE WINDOW:\n");
    printf("   Max sum of K=3 consecutive elements\n");
    int arr1[] = {2, 1, 5, 1, 3, 2};
    printf("   Array: [2, 1, 5, 1, 3, 2]\n");
    printf("   Max sum: %d\n\n", maxSumFixedWindow(arr1, 6, 3));

    // Variable size window - min length
    printf("2. VARIABLE SIZE WINDOW (shrinking):\n");
    printf("   Smallest subarray with sum >= 7\n");
    int arr2[] = {2, 1, 5, 2, 3, 2};
    printf("   Array: [2, 1, 5, 2, 3, 2]\n");
    printf("   Min length: %d\n\n", minSubarrayLen(7, arr2, 6));

    // Variable size window - max length
    printf("3. VARIABLE SIZE WINDOW (expanding):\n");
    printf("   Longest substring without repeating chars\n");
    printf("   \"abcabcbb\" -> %d\n", longestUniqueSubstring("abcabcbb"));
    printf("   \"bbbbb\"    -> %d\n", longestUniqueSubstring("bbbbb"));
    printf("   \"pwwkew\"   -> %d\n\n", longestUniqueSubstring("pwwkew"));

    // Count anagrams
    printf("4. FIXED SIZE (pattern matching):\n");
    printf("   Count anagrams of \"ab\" in \"abab\"\n");
    printf("   Count: %d\n\n", countAnagrams("abab", "ab"));

    // Summary
    printf("=== Quick Reference ===\n\n");
    printf("FIXED SIZE TEMPLATE:\n");
    printf("  1. Build first window (sum first K elements)\n");
    printf("  2. Slide: add arr[i], remove arr[i-k]\n\n");

    printf("VARIABLE SIZE TEMPLATE:\n");
    printf("  1. Expand: add arr[right]\n");
    printf("  2. Shrink: while condition met, remove arr[left]\n\n");

    printf("IDENTIFY BY:\n");
    printf("  - \"Subarray\" or \"Substring\"\n");
    printf("  - \"Consecutive\" or \"Contiguous\"\n");
    printf("  - \"Longest/Shortest with condition\"\n");

    return 0;
}
