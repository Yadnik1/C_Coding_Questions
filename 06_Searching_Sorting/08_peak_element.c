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

// Find peak element using binary search
// Say: "I'll use binary search to find a peak in logarithmic time"
int findPeakElement(int arr[], int n) {
    // Initialize search boundaries
    // Say: "I initialize low to 0 and high to n minus 1"
    int low = 0, high = n - 1;

    // Binary search loop (note: low < high, not <=)
    // Say: "I search while low is less than high, not less than or equal"
    while (low < high) {
        // Calculate middle index
        // Say: "I calculate mid to avoid overflow"
        int mid = low + (high - low) / 2;

        // Check if peak is on the right
        // Say: "If the element after mid is greater than mid"
        if (arr[mid] < arr[mid + 1]) {
            // Peak must be on right side (including mid+1)
            // Say: "The peak must be on the right, so I update low to mid plus 1"
            low = mid + 1;
        } else {
            // Peak is on left side (including mid)
            // Say: "Otherwise, the peak is on the left or at mid, so I update high to mid"
            high = mid;
        }
    }

    // Return peak index (low and high converge to peak)
    // Say: "When the loop ends, low and high meet at a peak, so I return low"
    return low;  // or high, they're equal
}

// Linear search for comparison (not used in interview)
// Say: "Here's a linear solution for comparison, but binary search is better"
int findPeakLinear(int arr[], int n) {
    // Check each element
    for (int i = 0; i < n; i++) {
        // Check if left neighbor is smaller (or doesn't exist)
        // Say: "I check if the left neighbor is smaller or doesn't exist"
        int leftOk = (i == 0) || (arr[i] > arr[i - 1]);
        // Check if right neighbor is smaller (or doesn't exist)
        // Say: "And if the right neighbor is smaller or doesn't exist"
        int rightOk = (i == n - 1) || (arr[i] > arr[i + 1]);

        // Found a peak
        // Say: "If both conditions are true, this is a peak"
        if (leftOk && rightOk) {
            return i;
        }
    }
    // No peak found (shouldn't happen)
    return -1;
}

int main() {
    printf("=== Find Peak Element ===\n\n");

    // Test case 1: Simple peak in middle
    // Say: "First test case has a simple peak in the middle"
    int arr1[] = {1, 2, 3, 1};
    // Calculate array size using sizeof
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("Array: [1, 2, 3, 1]\n");
    // Find peak using binary search
    int peak1 = findPeakElement(arr1, n1);
    printf("Peak at index %d (value %d)\n\n", peak1, arr1[peak1]);

    // Test case 2: Multiple peaks, find any one
    // Say: "Second test case has multiple peaks"
    int arr2[] = {1, 2, 1, 3, 5, 6, 4};
    // Calculate array size
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("Array: [1, 2, 1, 3, 5, 6, 4]\n");
    // Find any peak
    int peak2 = findPeakElement(arr2, n2);
    printf("Peak at index %d (value %d)\n\n", peak2, arr2[peak2]);

    // Test case 3: Single element array
    // Say: "Third test case has just one element which is a peak by definition"
    int arr3[] = {1};
    printf("Array: [1]\n");
    printf("Peak at index %d\n", findPeakElement(arr3, 1));

    return 0;
}
