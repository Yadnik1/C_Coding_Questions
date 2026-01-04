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

// Iterative binary search (preferred in interviews)
// Say: "I'll implement iterative binary search which uses O(1) space"
int binarySearchIterative(int arr[], int n, int target) {
    // Initialize search boundaries
    // Say: "I initialize low to 0, the start of the array"
    int low = 0;
    // Say: "And high to n minus 1, the last valid index"
    int high = n - 1;

    // Continue searching while we have a valid search space
    // Say: "I keep searching while low is less than or equal to high"
    while (low <= high) {
        // Calculate middle index avoiding integer overflow
        // Say: "I calculate mid as low plus half the distance to avoid overflow"
        int mid = low + (high - low) / 2;

        // Check if middle element is our target
        // Say: "If the middle element equals the target, I found it"
        if (arr[mid] == target) {
            // Return the index where we found it
            return mid;
        }
        // Target is in the right half
        // Say: "If middle is less than target, I search the right half"
        else if (arr[mid] < target) {
            // Move low pointer to exclude left half and mid
            // Say: "So I update low to mid plus 1"
            low = mid + 1;
        }
        // Target is in the left half
        // Say: "Otherwise, I search the left half"
        else {
            // Move high pointer to exclude right half and mid
            // Say: "So I update high to mid minus 1"
            high = mid - 1;
        }
    }

    // Element not found in the array
    // Say: "If I exit the loop, the element doesn't exist, so I return negative 1"
    return -1;  // Not found
}

// Recursive binary search
// Say: "Here's the recursive version which is more elegant but uses stack space"
int binarySearchRecursive(int arr[], int low, int high, int target) {
    // Base case: search space is empty
    // Say: "First I check the base case - if low exceeds high, element not found"
    if (low > high) return -1;

    // Calculate middle index
    // Say: "I calculate the middle index"
    int mid = low + (high - low) / 2;

    // Found the target
    // Say: "If I found the target, I return its index"
    if (arr[mid] == target) return mid;

    // Search right half
    // Say: "If middle is less than target, I recursively search right half"
    if (arr[mid] < target) {
        // Recursive call on right subarray
        return binarySearchRecursive(arr, mid + 1, high, target);
    } else {
        // Search left half
        // Say: "Otherwise, I recursively search the left half"
        // Recursive call on left subarray
        return binarySearchRecursive(arr, low, mid - 1, target);
    }
}

// Helper function to print array
void printArray(int arr[], int n) {
    // Print opening bracket
    printf("[");
    // Iterate through all elements
    for (int i = 0; i < n; i++) {
        // Print current element
        printf("%d", arr[i]);
        // Add comma separator except after last element
        if (i < n - 1) printf(", ");
    }
    // Print closing bracket
    printf("]");
}

int main() {
    printf("=== Binary Search ===\n\n");

    // Initialize sorted test array
    // Say: "I'll test with a sorted array"
    int arr[] = {1, 3, 5, 7, 9, 11, 13};
    // Calculate array size
    int n = 7;

    // Display the array we're searching
    printf("Array: ");
    printArray(arr, n);
    printf("\n\n");

    // Test cases - various targets to search for
    int targets[] = {7, 9, 1, 13, 6};
    // Number of test cases
    int numTargets = 5;

    // Test each target value
    for (int i = 0; i < numTargets; i++) {
        // Get current target to search for
        int target = targets[i];
        // Perform binary search
        int result = binarySearchIterative(arr, n, target);

        // Display search result
        printf("Search %d: ", target);
        // Check if element was found
        if (result != -1) {
            // Print the index where found
            printf("Found at index %d\n", result);
        } else {
            // Element doesn't exist in array
            printf("Not found\n");
        }
    }

    return 0;
}
