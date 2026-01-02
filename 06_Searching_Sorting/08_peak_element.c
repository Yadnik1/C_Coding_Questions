/*
 * ============================================================================
 * PROBLEM: Find Peak Element
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: High
 *
 * A peak element is greater than its neighbors.
 *
 * Example:
 * Input:  [1, 2, 3, 1]
 * Output: 2 (index of 3)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. BINARY SEARCH WORKS:
 *    - If arr[mid] < arr[mid+1], peak is on right
 *    - If arr[mid] < arr[mid-1], peak is on left
 *    - Otherwise, mid is peak
 *
 * 2. GUARANTEED TO EXIST:
 *    - Array boundaries are -infinity
 *    - At least one peak always exists
 *
 * ============================================================================
 * VISUAL: [1, 2, 1, 3, 5, 6, 4]
 * ============================================================================
 *
 *         *
 *       * *
 *     *   *
 *   *     *
 *   1 2 1 3 5 6 4
 *             ^
 *           peak at index 5 (value 6)
 *
 * Binary search:
 *   mid=3, arr[3]=3 < arr[4]=5 → search right
 *   mid=5, arr[5]=6 > arr[4] and arr[6] → PEAK!
 *
 * ============================================================================
 * TIME: O(log n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>

int findPeakElement(int arr[], int n) {
    int low = 0, high = n - 1;

    while (low < high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] < arr[mid + 1]) {
            // Peak is on right
            low = mid + 1;
        } else {
            // Peak is on left (including mid)
            high = mid;
        }
    }

    return low;  // or high, they're equal
}

// Linear search for comparison
int findPeakLinear(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        int leftOk = (i == 0) || (arr[i] > arr[i - 1]);
        int rightOk = (i == n - 1) || (arr[i] > arr[i + 1]);

        if (leftOk && rightOk) {
            return i;
        }
    }
    return -1;
}

int main() {
    printf("=== Find Peak Element ===\n\n");

    int arr1[] = {1, 2, 3, 1};
    int n1 = 4;
    printf("Array: [1, 2, 3, 1]\n");
    int peak1 = findPeakElement(arr1, n1);
    printf("Peak at index %d (value %d)\n\n", peak1, arr1[peak1]);

    int arr2[] = {1, 2, 1, 3, 5, 6, 4};
    int n2 = 7;
    printf("Array: [1, 2, 1, 3, 5, 6, 4]\n");
    int peak2 = findPeakElement(arr2, n2);
    printf("Peak at index %d (value %d)\n\n", peak2, arr2[peak2]);

    int arr3[] = {1};
    printf("Array: [1]\n");
    printf("Peak at index %d\n", findPeakElement(arr3, 1));

    return 0;
}
