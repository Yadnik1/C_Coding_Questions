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
    // Edge case: check if array has enough elements
    // Say: "First, I'll handle the edge case where array size is less than k"
    if (n < k) return -1;

    // Build first window by summing first k elements
    // Say: "I'll initialize the window sum by adding the first k elements"
    int windowSum = 0;

    // Loop through first k elements
    // Say: "Let me iterate through the first k elements to build the initial window"
    for (int i = 0; i < k; i++) {
        // Add current element to window sum
        // Say: "I add element at index i to the window sum"
        windowSum += arr[i];
    }

    // Initialize max sum with first window
    // Say: "I'll set the initial max sum to the first window's sum"
    int maxSum = windowSum;

    // Slide the window from position k to end
    // Say: "Now I slide the window across the rest of the array"
    for (int i = k; i < n; i++) {
        // Slide window: add new element at right, remove old at left
        // Say: "I add the new element at the right and remove the old element at the left"
        windowSum += arr[i] - arr[i - k];  // Add new, remove old

        // Check if current window sum is greater than max
        // Say: "I check if this window sum is greater than my current maximum"
        if (windowSum > maxSum) {
            // Update max sum
            // Say: "Yes, so I update the maximum"
            maxSum = windowSum;
        }
    }

    // Return the maximum sum found
    // Say: "Finally, I return the maximum sum"
    return maxSum;
}

// ============================================================================
// VARIABLE SIZE WINDOW: Smallest subarray with sum >= target
// ============================================================================
int minSubarrayLen(int target, int arr[], int n) {
    // Initialize left pointer of window
    // Say: "I'll start with left pointer at index 0"
    int left = 0;

    // Initialize running sum
    // Say: "I initialize the running sum to 0"
    int sum = 0;

    // Initialize minimum length to maximum possible value
    // Say: "I set the minimum length to INT_MAX as a sentinel value"
    int minLen = INT_MAX;

    // Iterate with right pointer across the array
    // Say: "I'll use the right pointer to expand the window"
    for (int right = 0; right < n; right++) {
        // Expand window: add element at right
        // Say: "I expand the window by adding the element at the right pointer"
        sum += arr[right];

        // Shrink window while condition is satisfied
        // Say: "Now I shrink the window from the left while sum is still greater than or equal to target"
        while (sum >= target) {
            // Calculate current window length
            // Say: "I calculate the current window length"
            int len = right - left + 1;

            // Update minimum length if current is smaller
            // Say: "I check if this is the smallest window so far"
            if (len < minLen) {
                // Update minimum
                // Say: "Yes, so I update the minimum length"
                minLen = len;
            }

            // Shrink window: remove element at left
            // Say: "I shrink the window by removing the element at the left pointer"
            sum -= arr[left];

            // Move left pointer forward
            // Say: "And I increment the left pointer"
            left++;
        }
    }

    // Return result: 0 if no valid subarray found, else minimum length
    // Say: "Finally, I return 0 if no valid subarray was found, otherwise the minimum length"
    return (minLen == INT_MAX) ? 0 : minLen;
}

// ============================================================================
// VARIABLE SIZE WINDOW: Longest substring without repeating characters
// ============================================================================
int longestUniqueSubstring(char* s) {
    // Calculate string length manually
    // Say: "First, I calculate the length of the string"
    int n = 0;
    while (s[n]) n++;  // strlen

    // Edge case: empty string
    // Say: "I handle the edge case of an empty string"
    if (n == 0) return 0;

    // Array to track which characters we've seen in current window
    // Say: "I create an array to track which ASCII characters are in the current window"
    int seen[128] = {0};  // ASCII characters

    // Initialize left pointer
    // Say: "I initialize the left pointer to 0"
    int left = 0;

    // Initialize maximum length
    // Say: "I initialize the maximum length to 0"
    int maxLen = 0;

    // Iterate with right pointer
    // Say: "I'll expand the window using the right pointer"
    for (int right = 0; right < n; right++) {
        // Get character at right pointer
        // Say: "I get the character at the right pointer"
        char c = s[right];

        // Shrink while we have a duplicate
        // Say: "If this character is already in the window, I shrink from the left"
        while (seen[(int)c]) {
            // Mark character at left as not seen
            // Say: "I mark the character at left as no longer in the window"
            seen[(int)s[left]] = 0;

            // Move left pointer forward
            // Say: "And increment the left pointer"
            left++;
        }

        // Mark current character as seen
        // Say: "I mark the current character as seen"
        seen[(int)c] = 1;

        // Calculate current window length
        // Say: "I calculate the current window length"
        int len = right - left + 1;

        // Update maximum if current is longer
        // Say: "I check if this is the longest substring so far"
        if (len > maxLen) {
            // Update maximum
            // Say: "Yes, so I update the maximum length"
            maxLen = len;
        }
    }

    // Return the maximum length found
    // Say: "Finally, I return the maximum length"
    return maxLen;
}

// ============================================================================
// FIXED SIZE: Count occurrences of anagram in string
// ============================================================================
int countAnagrams(char* text, char* pattern) {
    // Calculate text length
    // Say: "First, I calculate the length of both the text and pattern"
    int textLen = 0, patLen = 0;
    while (text[textLen]) textLen++;
    while (pattern[patLen]) patLen++;

    // Edge case: text shorter than pattern
    // Say: "I check if the text is shorter than the pattern"
    if (textLen < patLen) return 0;

    // Frequency array for pattern characters
    // Say: "I create a frequency array to count characters in the pattern"
    int patCount[26] = {0};

    // Frequency array for current window
    // Say: "And another frequency array for the current window"
    int winCount[26] = {0};

    // Count frequency of each character in pattern
    // Say: "I count the frequency of each character in the pattern"
    for (int i = 0; i < patLen; i++) {
        // Increment count for this character
        // Say: "I increment the count for this character"
        patCount[pattern[i] - 'a']++;
    }

    // Initialize anagram count
    // Say: "I initialize the count of anagrams found to 0"
    int count = 0;

    // Slide window across text
    // Say: "Now I slide a fixed-size window across the text"
    for (int i = 0; i < textLen; i++) {
        // Add current character to window
        // Say: "I add the current character to the window"
        winCount[text[i] - 'a']++;

        // Remove character that's leaving the window
        // Say: "If the window is larger than pattern length, I remove the leftmost character"
        if (i >= patLen) {
            // Decrement count for character leaving window
            // Say: "I decrement the count for the character leaving the window"
            winCount[text[i - patLen] - 'a']--;
        }

        // Check if we have a full window
        // Say: "Once I have a full window, I check if it's an anagram of the pattern"
        if (i >= patLen - 1) {
            // Assume it's a match initially
            // Say: "I assume it's a match and verify by comparing character frequencies"
            int match = 1;

            // Compare frequency of each letter
            // Say: "I compare the frequency of each letter in both arrays"
            for (int j = 0; j < 26; j++) {
                // If frequencies don't match, not an anagram
                // Say: "If any frequency doesn't match, it's not an anagram"
                if (winCount[j] != patCount[j]) {
                    match = 0;
                    break;
                }
            }

            // If all frequencies matched, increment count
            // Say: "If all frequencies match, I increment the count"
            if (match) count++;
        }
    }

    // Return total anagram count
    // Say: "Finally, I return the total count of anagrams found"
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
