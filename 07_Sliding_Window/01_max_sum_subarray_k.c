/*
 * ============================================================================
 * PROBLEM: Maximum Sum Subarray of Size K
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: HIGH
 *
 * Pattern: FIXED SIZE SLIDING WINDOW
 *
 * Find the maximum sum of any contiguous subarray of size k.
 *
 * ============================================================================
 * VISUALIZATION: arr = [2, 1, 5, 1, 3, 2], k = 3
 * ============================================================================
 *
 *   Window 1: [2, 1, 5] sum = 8
 *              ^     ^
 *
 *   Slide: Remove 2, Add 1
 *   Window 2: [1, 5, 1] sum = 8 - 2 + 1 = 7
 *                 ^     ^
 *
 *   Slide: Remove 1, Add 3
 *   Window 3: [5, 1, 3] sum = 7 - 1 + 3 = 9 ← MAX
 *                    ^     ^
 *
 *   Slide: Remove 5, Add 2
 *   Window 4: [1, 3, 2] sum = 9 - 5 + 2 = 6
 *                       ^     ^
 *
 *   Maximum sum = 9
 *
 * ============================================================================
 * TEMPLATE:
 * ============================================================================
 *
 *   // 1. Build first window
 *   for (i = 0; i < k; i++) {
 *       windowSum += arr[i];
 *   }
 *   maxSum = windowSum;
 *
 *   // 2. Slide window
 *   for (i = k; i < n; i++) {
 *       windowSum += arr[i] - arr[i - k];  // Add new, remove old
 *       if (windowSum > maxSum) maxSum = windowSum;
 *   }
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why is sliding window O(n) instead of O(n*k)?"
 * A1: Each element is added once and removed once. We don't recalculate the
 *     entire sum each time - we just subtract the leaving element and add
 *     the entering element. Total operations = 2n = O(n).
 *
 * -------------------------------------------------------------------------
 * Q2: "When does sliding window apply?"
 * A2: Fixed-size: When window size k is given (max sum, average, etc.).
 *     Variable-size: When looking for a condition (longest subarray with
 *     sum <= target, smallest subarray with sum >= target).
 *
 * -------------------------------------------------------------------------
 * Q3: "What if we need the actual subarray, not just the sum?"
 * A3: Track the starting index when you update maxSum. Keep startIndex and
 *     update it to (i - k + 1) when finding a new maximum. Return both
 *     the sum and the starting position.
 *
 * -------------------------------------------------------------------------
 * Q4: "What about minimum sum subarray of size k?"
 * A4: Same algorithm! Just change the comparison from > to < and initialize
 *     minSum with the first window sum. Works identically.
 *
 * ============================================================================
 */

#include <stdio.h>

int maxSumSubarray(int arr[], int n, int k) {
    // Edge case: check if we have enough elements
    // Say: "First, I'll check if the array has at least k elements"
    if (n < k) return -1;

    // Build first window of size k
    // Say: "I'll initialize a variable to track the window sum"
    int windowSum = 0;

    // Loop to sum first k elements
    // Say: "Now I'll build the first window by summing the first k elements"
    for (int i = 0; i < k; i++) {
        // Add element to window sum
        // Say: "I add the element at index i to the window sum"
        windowSum += arr[i];
    }

    // Initialize max sum with first window's sum
    // Say: "I'll initialize the maximum sum with the first window's sum"
    int maxSum = windowSum;

    // Slide the window from position k to end
    // Say: "Now I'll slide the window across the rest of the array"
    for (int i = k; i < n; i++) {
        // Add new element (at right), remove old element (at left)
        // Say: "I slide the window by adding the new element at index i and removing the element at index i minus k"
        windowSum += arr[i] - arr[i - k];  // Add new element, remove old

        // Check if current window sum is greater
        // Say: "I check if the current window sum is greater than the maximum"
        if (windowSum > maxSum) {
            // Update maximum sum
            // Say: "Yes, so I update the maximum sum"
            maxSum = windowSum;
        }
    }

    // Return the maximum sum found
    // Say: "Finally, I return the maximum sum"
    return maxSum;
}

int main() {
    printf("=== Maximum Sum Subarray of Size K ===\n\n");

    int arr1[] = {2, 1, 5, 1, 3, 2};
    int n1 = 6, k1 = 3;
    printf("Array: [2, 1, 5, 1, 3, 2], k = 3\n");
    printf("Max sum: %d\n\n", maxSumSubarray(arr1, n1, k1));

    int arr2[] = {2, 3, 4, 1, 5};
    int n2 = 5, k2 = 2;
    printf("Array: [2, 3, 4, 1, 5], k = 2\n");
    printf("Max sum: %d\n\n", maxSumSubarray(arr2, n2, k2));

    int arr3[] = {1, 2, 3, 4, 5};
    int n3 = 5, k3 = 1;
    printf("Array: [1, 2, 3, 4, 5], k = 1\n");
    printf("Max sum: %d\n", maxSumSubarray(arr3, n3, k3));

    return 0;
}
