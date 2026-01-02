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
 */

#include <stdio.h>

int maxSumSubarray(int arr[], int n, int k) {
    if (n < k) return -1;

    // Build first window
    int windowSum = 0;
    for (int i = 0; i < k; i++) {
        windowSum += arr[i];
    }

    int maxSum = windowSum;

    // Slide the window
    for (int i = k; i < n; i++) {
        windowSum += arr[i] - arr[i - k];  // Add new element, remove old
        if (windowSum > maxSum) {
            maxSum = windowSum;
        }
    }

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
