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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "What's the difference from regular binary search?"
 * A1: Regular search stops immediately when found. For first occurrence,
 *     when found, save result and keep searching LEFT (high = mid - 1).
 *     For last occurrence, search RIGHT (low = mid + 1). Don't stop early!
 *
 * -------------------------------------------------------------------------
 * Q2: "How do you count occurrences of an element?"
 * A2: count = lastOccurrence - firstOccurrence + 1
 *     Two binary searches = O(log n) total. Much better than linear O(n)!
 *     If either returns -1, count is 0.
 *
 * -------------------------------------------------------------------------
 * Q3: "What if I need to insert while maintaining sorted order?"
 * A3: Use modified binary search to find insertion point! For duplicates,
 *     findFirst gives insert-before position, findLast+1 gives insert-after.
 *     C++ has lower_bound/upper_bound for this.
 *
 * -------------------------------------------------------------------------
 * Q4: "Can you do this in one pass?"
 * A4: Not with O(log n). You need two separate searches. But if you find
 *     one occurrence, you can limit the search range for the other
 *     (first must be <= found, last must be >= found).
 *
 * ============================================================================
 */

#include <stdio.h>

// Find first occurrence of target using modified binary search
// Say: "I'll use modified binary search to find the first occurrence"
int findFirst(int arr[], int n, int target) {
    // Initialize search boundaries
    // Say: "I initialize low to 0 and high to n minus 1"
    int low = 0, high = n - 1;
    // Variable to store result index
    // Say: "I use result to track the first occurrence found so far"
    int result = -1;

    // Binary search loop
    // Say: "I search while the search space is valid"
    while (low <= high) {
        // Calculate middle index
        // Say: "I calculate mid to avoid overflow"
        int mid = low + (high - low) / 2;

        // Found the target
        // Say: "If I find the target at mid"
        if (arr[mid] == target) {
            // Save this position
            // Say: "I save this index as a potential first occurrence"
            result = mid;      // Save position
            // Continue searching left for earlier occurrence
            // Say: "But I keep searching left to find an earlier occurrence"
            high = mid - 1;    // Keep searching left
        }
        // Target is in right half
        // Say: "If mid is less than target, I search right"
        else if (arr[mid] < target) {
            // Move search to right half
            low = mid + 1;
        }
        // Target is in left half
        // Say: "Otherwise, I search left"
        else {
            // Move search to left half
            high = mid - 1;
        }
    }

    // Return first occurrence index (or -1 if not found)
    // Say: "I return the first occurrence index, or negative 1 if not found"
    return result;
}

// Find last occurrence of target using modified binary search
// Say: "Now I'll find the last occurrence by searching right when found"
int findLast(int arr[], int n, int target) {
    // Initialize search boundaries
    // Say: "I initialize low to 0 and high to n minus 1"
    int low = 0, high = n - 1;
    // Variable to store result index
    // Say: "I use result to track the last occurrence found so far"
    int result = -1;

    // Binary search loop
    // Say: "I search while the search space is valid"
    while (low <= high) {
        // Calculate middle index
        // Say: "I calculate mid to avoid overflow"
        int mid = low + (high - low) / 2;

        // Found the target
        // Say: "If I find the target at mid"
        if (arr[mid] == target) {
            // Save this position
            // Say: "I save this index as a potential last occurrence"
            result = mid;      // Save position
            // Continue searching right for later occurrence
            // Say: "But I keep searching right to find a later occurrence"
            low = mid + 1;     // Keep searching right
        }
        // Target is in right half
        // Say: "If mid is less than target, I search right"
        else if (arr[mid] < target) {
            // Move search to right half
            low = mid + 1;
        }
        // Target is in left half
        // Say: "Otherwise, I search left"
        else {
            // Move search to left half
            high = mid - 1;
        }
    }

    // Return last occurrence index (or -1 if not found)
    // Say: "I return the last occurrence index, or negative 1 if not found"
    return result;
}

int main() {
    printf("=== First and Last Occurrence ===\n\n");

    // Initialize sorted array with duplicate values
    int arr[] = {5, 7, 7, 8, 8, 10};
    // Array size
    int n = 6;

    printf("Array: [5, 7, 7, 8, 8, 10]\n\n");

    // Test case 1: Find occurrences of 8
    // Say: "First, I'll find the first and last occurrence of 8"
    int target = 8;
    printf("Target: %d\n", target);
    printf("First occurrence: %d\n", findFirst(arr, n, target));
    printf("Last occurrence:  %d\n", findLast(arr, n, target));

    // Test case 2: Find occurrences of 7
    // Say: "Next, I'll find the first and last occurrence of 7"
    target = 7;
    printf("\nTarget: %d\n", target);
    printf("First occurrence: %d\n", findFirst(arr, n, target));
    printf("Last occurrence:  %d\n", findLast(arr, n, target));

    // Test case 3: Element not in array
    // Say: "Finally, I'll test with an element not in the array"
    target = 6;
    printf("\nTarget: %d\n", target);
    printf("First occurrence: %d\n", findFirst(arr, n, target));
    printf("Last occurrence:  %d\n", findLast(arr, n, target));

    return 0;
}
