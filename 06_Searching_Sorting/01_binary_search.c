/*
 * ============================================================================
 * PROBLEM: Binary Search
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: VERY HIGH (MUST KNOW!)
 *
 * Find element in sorted array in O(log n) time.
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. PREREQUISITE: Array must be SORTED
 *
 * 2. DIVIDE AND CONQUER:
 *    - Check middle element
 *    - If target < mid, search left half
 *    - If target > mid, search right half
 *
 * 3. AVOID OVERFLOW:
 *    - Use mid = low + (high - low) / 2
 *    - NOT mid = (low + high) / 2
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: Find 7 in [1, 3, 5, 7, 9, 11, 13]
 * ============================================================================
 *
 * Initial: low=0, high=6
 *
 *   [1, 3, 5, 7, 9, 11, 13]
 *    ^        ^          ^
 *   low      mid       high
 *
 *   mid = 3, arr[3] = 7
 *   7 == 7 → FOUND at index 3!
 *
 * ============================================================================
 * VISUAL: Find 9 in [1, 3, 5, 7, 9, 11, 13]
 * ============================================================================
 *
 * Round 1: low=0, high=6, mid=3
 *   arr[3] = 7 < 9
 *   Search right: low = mid + 1 = 4
 *
 *   [1, 3, 5, 7, 9, 11, 13]
 *               ^   ^    ^
 *              low mid  high
 *
 * Round 2: low=4, high=6, mid=5
 *   arr[5] = 11 > 9
 *   Search left: high = mid - 1 = 4
 *
 *   [1, 3, 5, 7, 9, 11, 13]
 *               ^
 *          low=mid=high
 *
 * Round 3: low=4, high=4, mid=4
 *   arr[4] = 9 == 9 → FOUND at index 4!
 *
 * ============================================================================
 * TIME: O(log n) | SPACE: O(1) iterative, O(log n) recursive
 * ============================================================================
 */

#include <stdio.h>

// Iterative (preferred)
int binarySearchIterative(int arr[], int n, int target) {
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == target) {
            return mid;
        }
        else if (arr[mid] < target) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return -1;  // Not found
}

// Recursive
int binarySearchRecursive(int arr[], int low, int high, int target) {
    if (low > high) return -1;

    int mid = low + (high - low) / 2;

    if (arr[mid] == target) return mid;

    if (arr[mid] < target) {
        return binarySearchRecursive(arr, mid + 1, high, target);
    } else {
        return binarySearchRecursive(arr, low, mid - 1, target);
    }
}

void printArray(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]");
}

int main() {
    printf("=== Binary Search ===\n\n");

    int arr[] = {1, 3, 5, 7, 9, 11, 13};
    int n = 7;

    printf("Array: ");
    printArray(arr, n);
    printf("\n\n");

    int targets[] = {7, 9, 1, 13, 6};
    int numTargets = 5;

    for (int i = 0; i < numTargets; i++) {
        int target = targets[i];
        int result = binarySearchIterative(arr, n, target);

        printf("Search %d: ", target);
        if (result != -1) {
            printf("Found at index %d\n", result);
        } else {
            printf("Not found\n");
        }
    }

    return 0;
}
