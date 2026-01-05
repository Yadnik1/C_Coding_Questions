/*
 * ============================================================================
 * PROBLEM: Longest Subarray with Given Sum
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: HIGH
 *
 * Two versions:
 * 1. Positive integers only - Sliding window O(n)
 * 2. Mixed integers (positive + negative) - Prefix sum with hash O(n)
 *
 * ============================================================================
 * VERSION 1: Positive Integers Only (Sliding Window)
 * ============================================================================
 *
 *   arr = [1, 2, 3, 1, 1, 1, 1], target = 6
 *
 *   Window expands while sum < target
 *   Window shrinks while sum > target
 *   When sum == target, record length
 *
 * ============================================================================
 * VERSION 2: Mixed Integers (Prefix Sum + Hash)
 * ============================================================================
 *
 *   Key insight: If prefix[j] - prefix[i] = target,
 *                then subarray [i+1...j] has sum = target
 *
 *   Store prefix sums in hash, look for (currentSum - target)
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1) positive, O(n) mixed
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why doesn't sliding window work for negative numbers?"
 * A1: Sliding window assumes adding elements increases sum, removing decreases.
 *     With negatives, adding can decrease and removing can increase!
 *     We can't know which direction to shrink. Prefix sum + hash is needed.
 *
 * -------------------------------------------------------------------------
 * Q2: "Why store only the first occurrence in prefix sum approach?"
 * A2: We want the LONGEST subarray. If prefix sum X appears at indices i and j
 *     where i < j, using i gives a longer subarray ending at any later index.
 *     First occurrence = longest possible result.
 *
 * -------------------------------------------------------------------------
 * Q3: "What if we want the SHORTEST subarray with given sum?"
 * A3: For positive integers: still sliding window, but update when sum >= target
 *     and shrink as much as possible. For mixed: store last occurrence instead
 *     of first in the hash table.
 *
 * -------------------------------------------------------------------------
 * Q4: "How do we handle prefix sum overflow?"
 * A4: In interviews, usually assume it fits in int/long. In production:
 *     use 64-bit integers, or compute prefix sum modulo a large prime
 *     (with collision handling), or use arbitrary precision arithmetic.
 *
 * ============================================================================
 */

#include <stdio.h>

// Version 1: Positive integers only - Sliding Window
int longestSubarraySumPositive(int arr[], int n, int target) {
    // Initialize left pointer
    // Say: "I'll start with the left pointer at index 0"
    int left = 0;

    // Initialize running sum
    // Say: "I'll initialize the running sum to 0"
    int sum = 0;

    // Initialize maximum length
    // Say: "I'll initialize the maximum length to 0"
    int maxLen = 0;

    // Expand window using right pointer
    // Say: "Now I'll expand the window using the right pointer"
    for (int right = 0; right < n; right++) {
        // Add element at right to sum
        // Say: "I add the element at the right pointer to the sum"
        sum += arr[right];

        // Shrink window while sum exceeds target
        // Say: "If the sum is greater than the target, I need to shrink the window from the left"
        while (sum > target && left <= right) {
            // Remove element at left from sum
            // Say: "I remove the element at the left pointer from the sum"
            sum -= arr[left];

            // Move left pointer forward
            // Say: "And I increment the left pointer"
            left++;
        }

        // Check if we found the target sum
        // Say: "Now I check if the current sum equals the target"
        if (sum == target) {
            // Calculate current window length
            // Say: "Yes, so I calculate the window length"
            int len = right - left + 1;

            // Update maximum if current is longer
            // Say: "I check if this is the longest subarray so far"
            if (len > maxLen) {
                // Update maximum length
                // Say: "Yes, so I update the maximum length"
                maxLen = len;
            }
        }
    }

    // Return maximum length found
    // Say: "Finally, I return the maximum length"
    return maxLen;
}

// Version 2: Mixed integers - Prefix Sum with Hash
// Using simple hash for demonstration
#define HASH_SIZE 100003
#define OFFSET 50000  // To handle negative sums

// Hash entry structure
// Say: "I'll define a hash entry structure to store sum, index, and usage flag"
typedef struct {
    int sum;      // The prefix sum value
    int index;    // The index where this sum occurred
    int used;     // Flag to indicate if this entry is occupied
} HashEntry;

int longestSubarraySumMixed(int arr[], int n, int target) {
    // Initialize hash table
    // Say: "I'll create a hash table to store prefix sums and their indices"
    HashEntry hash[HASH_SIZE] = {0};

    // Initialize maximum length
    // Say: "I'll initialize the maximum length to 0"
    int maxLen = 0;

    // Initialize prefix sum
    // Say: "I'll initialize the prefix sum to 0"
    int prefixSum = 0;

    // Store initial state: prefix sum 0 at index -1
    // Say: "I'll store the initial prefix sum of 0 at index -1 to handle subarrays starting from index 0"
    int zeroKey = (0 + OFFSET) % HASH_SIZE;
    hash[zeroKey].sum = 0;
    hash[zeroKey].index = -1;
    hash[zeroKey].used = 1;

    // Iterate through array
    // Say: "Now I'll iterate through the array, maintaining the prefix sum"
    for (int i = 0; i < n; i++) {
        // Add current element to prefix sum
        // Say: "I add the current element to the prefix sum"
        prefixSum += arr[i];

        // Calculate what sum we're looking for in hash
        // Say: "I calculate what prefix sum I need to find: current prefix sum minus target"
        int lookFor = prefixSum - target;

        // Calculate hash key for the sum we're looking for
        // Say: "I compute the hash key for this sum"
        int key = ((lookFor % HASH_SIZE) + HASH_SIZE + OFFSET) % HASH_SIZE;

        // Check if we've seen this prefix sum before
        // Say: "I check if this prefix sum exists in the hash table"
        if (hash[key].used && hash[key].sum == lookFor) {
            // Calculate subarray length
            // Say: "Yes, so I calculate the subarray length from that index to current index"
            int len = i - hash[key].index;

            // Update maximum if current is longer
            // Say: "I check if this is the longest subarray so far"
            if (len > maxLen) {
                // Update maximum length
                // Say: "Yes, so I update the maximum length"
                maxLen = len;
            }
        }

        // Store current prefix sum (only first occurrence for longest subarray)
        // Say: "Now I store the current prefix sum in the hash table if it's not already there"
        int curKey = ((prefixSum % HASH_SIZE) + HASH_SIZE + OFFSET) % HASH_SIZE;

        // Only store if this slot is empty
        // Say: "I only store it if this prefix sum hasn't been seen before"
        if (!hash[curKey].used) {
            // Store sum value
            // Say: "I store the sum value"
            hash[curKey].sum = prefixSum;

            // Store index
            // Say: "I store the current index"
            hash[curKey].index = i;

            // Mark as used
            // Say: "And I mark this entry as used"
            hash[curKey].used = 1;
        }
    }

    // Return maximum length found
    // Say: "Finally, I return the maximum length"
    return maxLen;
}

// Simple O(n²) version for verification (works with any integers)
int longestSubarraySumBrute(int arr[], int n, int target) {
    // Initialize maximum length
    // Say: "For the brute force approach, I'll check all possible subarrays"
    int maxLen = 0;

    // Iterate through all starting positions
    // Say: "I iterate through all possible starting positions"
    for (int i = 0; i < n; i++) {
        // Initialize sum for subarray starting at i
        // Say: "For each starting position, I initialize the sum to 0"
        int sum = 0;

        // Iterate through all ending positions
        // Say: "I iterate through all possible ending positions"
        for (int j = i; j < n; j++) {
            // Add element to sum
            // Say: "I add the element at j to the running sum"
            sum += arr[j];

            // Check if sum equals target
            // Say: "I check if the sum equals the target"
            if (sum == target) {
                // Calculate subarray length
                // Say: "Yes, so I calculate the subarray length"
                int len = j - i + 1;

                // Update maximum if current is longer
                // Say: "I check if this is the longest subarray"
                if (len > maxLen) {
                    // Update maximum length
                    // Say: "Yes, so I update the maximum length"
                    maxLen = len;
                }
            }
        }
    }

    // Return maximum length found
    // Say: "Finally, I return the maximum length"
    return maxLen;
}

int main() {
    printf("=== Longest Subarray with Given Sum ===\n\n");

    // Positive integers
    printf("1. Positive integers (Sliding Window):\n");
    int arr1[] = {1, 2, 3, 1, 1, 1, 1};
    int n1 = 7, target1 = 6;
    printf("   Array: [1, 2, 3, 1, 1, 1, 1], Target: %d\n", target1);
    printf("   Longest length: %d\n", longestSubarraySumPositive(arr1, n1, target1));
    printf("   (Subarray: [1, 1, 1, 1, 1, 1] or [3, 1, 1, 1])\n\n");

    int arr2[] = {1, 2, 3, 4, 5};
    int n2 = 5, target2 = 9;
    printf("   Array: [1, 2, 3, 4, 5], Target: %d\n", target2);
    printf("   Longest length: %d\n", longestSubarraySumPositive(arr2, n2, target2));
    printf("   (Subarray: [2, 3, 4] or [4, 5])\n\n");

    // Mixed integers
    printf("2. Mixed integers (Prefix Sum + Hash):\n");
    int arr3[] = {1, -1, 5, -2, 3};
    int n3 = 5, target3 = 3;
    printf("   Array: [1, -1, 5, -2, 3], Target: %d\n", target3);
    printf("   Longest length: %d\n", longestSubarraySumBrute(arr3, n3, target3));
    printf("   (Subarray: [1, -1, 5, -2] = 3)\n\n");

    int arr4[] = {-2, -1, 2, 1};
    int n4 = 4, target4 = 1;
    printf("   Array: [-2, -1, 2, 1], Target: %d\n", target4);
    printf("   Longest length: %d\n", longestSubarraySumBrute(arr4, n4, target4));
    printf("   (Subarray: [-1, 2] = 1)\n\n");

    printf("=== Summary ===\n");
    printf("Positive only: Sliding window O(n) time, O(1) space\n");
    printf("Mixed:         Prefix sum + hash O(n) time, O(n) space\n");

    return 0;
}
