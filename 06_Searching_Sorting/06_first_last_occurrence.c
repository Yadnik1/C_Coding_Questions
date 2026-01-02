/*
 * ============================================================================
 * PROBLEM: Find First and Last Occurrence
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: High
 *
 * Find first and last position of target in sorted array.
 *
 * Example:
 * Input:  [5, 7, 7, 8, 8, 10], target = 8
 * Output: [3, 4]
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. MODIFIED BINARY SEARCH:
 *    - For first: when found, search left half
 *    - For last: when found, search right half
 *
 * ============================================================================
 * VISUAL: Find first 8 in [5, 7, 7, 8, 8, 10]
 * ============================================================================
 *
 * Round 1: mid=2, arr[2]=7 < 8
 *   Search right: low = 3
 *
 * Round 2: mid=4, arr[4]=8 == 8
 *   Found! But is it first?
 *   Save result=4, search left: high = 3
 *
 * Round 3: mid=3, arr[3]=8 == 8
 *   Found earlier! Save result=3, high = 2
 *
 * Round 4: low > high, STOP
 * First occurrence: 3
 *
 * ============================================================================
 * TIME: O(log n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>

int findFirst(int arr[], int n, int target) {
    int low = 0, high = n - 1;
    int result = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == target) {
            result = mid;      // Save position
            high = mid - 1;    // Keep searching left
        }
        else if (arr[mid] < target) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return result;
}

int findLast(int arr[], int n, int target) {
    int low = 0, high = n - 1;
    int result = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == target) {
            result = mid;      // Save position
            low = mid + 1;     // Keep searching right
        }
        else if (arr[mid] < target) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return result;
}

int main() {
    printf("=== First and Last Occurrence ===\n\n");

    int arr[] = {5, 7, 7, 8, 8, 10};
    int n = 6;

    printf("Array: [5, 7, 7, 8, 8, 10]\n\n");

    int target = 8;
    printf("Target: %d\n", target);
    printf("First occurrence: %d\n", findFirst(arr, n, target));
    printf("Last occurrence:  %d\n", findLast(arr, n, target));

    target = 7;
    printf("\nTarget: %d\n", target);
    printf("First occurrence: %d\n", findFirst(arr, n, target));
    printf("Last occurrence:  %d\n", findLast(arr, n, target));

    target = 6;
    printf("\nTarget: %d\n", target);
    printf("First occurrence: %d\n", findFirst(arr, n, target));
    printf("Last occurrence:  %d\n", findLast(arr, n, target));

    return 0;
}
