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

// Search in rotated sorted array using modified binary search
// Say: "I'll use binary search modified for rotated arrays"
int searchRotated(int arr[], int n, int target) {
    // Initialize search boundaries
    // Say: "I initialize low to 0 and high to n minus 1"
    int low = 0, high = n - 1;

    // Binary search loop
    // Say: "I search while the search space is valid"
    while (low <= high) {
        // Calculate middle index
        // Say: "I calculate mid to avoid overflow"
        int mid = low + (high - low) / 2;

        // Check if we found the target
        // Say: "If mid equals target, I found it"
        if (arr[mid] == target) {
            return mid;
        }

        // Check if left half is sorted
        // Say: "Now I check if the left half from low to mid is sorted"
        if (arr[low] <= arr[mid]) {
            // Target in left sorted half?
            // Say: "If target is within the sorted left half range"
            if (target >= arr[low] && target < arr[mid]) {
                // Search left half
                // Say: "I search the left half by updating high"
                high = mid - 1;
            } else {
                // Search right half
                // Say: "Otherwise, I search the right half by updating low"
                low = mid + 1;
            }
        }
        // Right half is sorted
        // Say: "Otherwise, the right half must be sorted"
        else {
            // Target in right sorted half?
            // Say: "If target is within the sorted right half range"
            if (target > arr[mid] && target <= arr[high]) {
                // Search right half
                // Say: "I search the right half by updating low"
                low = mid + 1;
            } else {
                // Search left half
                // Say: "Otherwise, I search the left half by updating high"
                high = mid - 1;
            }
        }
    }

    // Element not found
    // Say: "If I exit the loop, the element wasn't found"
    return -1;
}

int main() {
    printf("=== Search in Rotated Sorted Array ===\n\n");

    // Initialize rotated sorted array
    // Say: "I'll test with a rotated sorted array"
    int arr[] = {4, 5, 6, 7, 0, 1, 2};
    // Array size
    int n = 7;

    printf("Array: [4, 5, 6, 7, 0, 1, 2]\n\n");

    // Test cases - various targets to search
    int targets[] = {0, 3, 7, 2};
    int numTargets = 4;

    // Test each target
    for (int i = 0; i < numTargets; i++) {
        // Search for current target
        int result = searchRotated(arr, n, targets[i]);
        // Display result
        printf("Search %d: ", targets[i]);
        // Check if found
        if (result != -1) {
            // Print index where found
            printf("Found at index %d\n", result);
        } else {
            // Element not in array
            printf("Not found\n");
        }
    }

    return 0;
}
