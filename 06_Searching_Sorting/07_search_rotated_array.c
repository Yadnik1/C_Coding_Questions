/*
 * ============================================================================
 * PROBLEM: Search in Rotated Sorted Array
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: High
 *
 * Example:
 * Input:  [4, 5, 6, 7, 0, 1, 2], target = 0
 * Output: 4
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. KEY INSIGHT:
 *    - One half is always sorted
 *    - Check which half, decide where to search
 *
 * 2. STEPS:
 *    - Find mid
 *    - Check if left half [low..mid] is sorted
 *    - If target in sorted half, search there
 *    - Else search other half
 *
 * ============================================================================
 * VISUAL: Find 0 in [4, 5, 6, 7, 0, 1, 2]
 * ============================================================================
 *
 * Round 1: low=0, high=6, mid=3
 *   [4, 5, 6, 7, 0, 1, 2]
 *    ^        ^        ^
 *   low      mid      high
 *
 *   Left half [4,5,6,7] sorted (arr[low] <= arr[mid])
 *   Is 0 in [4,7]? No (0 < 4)
 *   Search right: low = 4
 *
 * Round 2: low=4, high=6, mid=5
 *   [4, 5, 6, 7, 0, 1, 2]
 *               ^  ^  ^
 *              low mid high
 *
 *   Left half [0,1] sorted
 *   Is 0 in [0,1]? Yes!
 *   Search left: high = 4
 *
 * Round 3: low=4, high=4, mid=4
 *   arr[4] = 0 == target → FOUND at index 4!
 *
 * ============================================================================
 * TIME: O(log n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>

int searchRotated(int arr[], int n, int target) {
    int low = 0, high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == target) {
            return mid;
        }

        // Check if left half is sorted
        if (arr[low] <= arr[mid]) {
            // Target in left sorted half?
            if (target >= arr[low] && target < arr[mid]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        // Right half is sorted
        else {
            // Target in right sorted half?
            if (target > arr[mid] && target <= arr[high]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }

    return -1;
}

int main() {
    printf("=== Search in Rotated Sorted Array ===\n\n");

    int arr[] = {4, 5, 6, 7, 0, 1, 2};
    int n = 7;

    printf("Array: [4, 5, 6, 7, 0, 1, 2]\n\n");

    int targets[] = {0, 3, 7, 2};
    int numTargets = 4;

    for (int i = 0; i < numTargets; i++) {
        int result = searchRotated(arr, n, targets[i]);
        printf("Search %d: ", targets[i]);
        if (result != -1) {
            printf("Found at index %d\n", result);
        } else {
            printf("Not found\n");
        }
    }

    return 0;
}
