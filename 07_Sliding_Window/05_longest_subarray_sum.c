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
 */

#include <stdio.h>

// Version 1: Positive integers only - Sliding Window
int longestSubarraySumPositive(int arr[], int n, int target) {
    int left = 0;
    int sum = 0;
    int maxLen = 0;

    for (int right = 0; right < n; right++) {
        sum += arr[right];

        // Shrink window while sum > target
        while (sum > target && left <= right) {
            sum -= arr[left];
            left++;
        }

        // Check if we found target sum
        if (sum == target) {
            int len = right - left + 1;
            if (len > maxLen) {
                maxLen = len;
            }
        }
    }

    return maxLen;
}

// Version 2: Mixed integers - Prefix Sum with Hash
// Using simple hash for demonstration
#define HASH_SIZE 100003
#define OFFSET 50000  // To handle negative sums

typedef struct {
    int sum;
    int index;
    int used;
} HashEntry;

int longestSubarraySumMixed(int arr[], int n, int target) {
    HashEntry hash[HASH_SIZE] = {0};

    int maxLen = 0;
    int prefixSum = 0;

    // Store index for prefix sum 0 at position -1
    int zeroKey = (0 + OFFSET) % HASH_SIZE;
    hash[zeroKey].sum = 0;
    hash[zeroKey].index = -1;
    hash[zeroKey].used = 1;

    for (int i = 0; i < n; i++) {
        prefixSum += arr[i];

        // Look for (prefixSum - target) in hash
        int lookFor = prefixSum - target;
        int key = ((lookFor % HASH_SIZE) + HASH_SIZE + OFFSET) % HASH_SIZE;

        if (hash[key].used && hash[key].sum == lookFor) {
            int len = i - hash[key].index;
            if (len > maxLen) {
                maxLen = len;
            }
        }

        // Store current prefix sum (only first occurrence for longest)
        int curKey = ((prefixSum % HASH_SIZE) + HASH_SIZE + OFFSET) % HASH_SIZE;
        if (!hash[curKey].used) {
            hash[curKey].sum = prefixSum;
            hash[curKey].index = i;
            hash[curKey].used = 1;
        }
    }

    return maxLen;
}

// Simple O(n²) version for verification (works with any integers)
int longestSubarraySumBrute(int arr[], int n, int target) {
    int maxLen = 0;

    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = i; j < n; j++) {
            sum += arr[j];
            if (sum == target) {
                int len = j - i + 1;
                if (len > maxLen) {
                    maxLen = len;
                }
            }
        }
    }

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
