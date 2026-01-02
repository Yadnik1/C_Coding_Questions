/*
 * ============================================================================
 * PROBLEM: Check if Array is Sorted
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 3 mins | FREQUENCY: High
 *
 * Check if array is sorted in ascending or descending order.
 *
 * Example:
 * Input:  [1, 2, 3, 4, 5] → Ascending (true)
 * Input:  [5, 4, 3, 2, 1] → Descending (true)
 * Input:  [1, 3, 2, 4, 5] → Not sorted (false)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. ADJACENT COMPARISON:
 *    - Compare each element with its next element
 *    - If any pair violates order, not sorted
 *
 * 2. TWO TYPES:
 *    - Strictly sorted: No equal adjacent elements
 *    - Non-strictly sorted: Equal elements allowed
 *
 * 3. DIRECTION:
 *    - Ascending: arr[i] <= arr[i+1] for all i
 *    - Descending: arr[i] >= arr[i+1] for all i
 *
 * ============================================================================
 * VISUAL UNDERSTANDING:
 * ============================================================================
 *
 * ASCENDING CHECK: [1, 2, 3, 4, 5]
 *
 *   Compare pairs:
 *   [1, 2, 3, 4, 5]
 *    ^--^           1 <= 2 ✓
 *       ^--^        2 <= 3 ✓
 *          ^--^     3 <= 4 ✓
 *             ^--^  4 <= 5 ✓
 *
 *   All pairs satisfy arr[i] <= arr[i+1]
 *   Result: SORTED (ascending)
 *
 * NOT SORTED: [1, 3, 2, 4, 5]
 *
 *   [1, 3, 2, 4, 5]
 *    ^--^           1 <= 3 ✓
 *       ^--^        3 <= 2 ✗ VIOLATION!
 *
 *   Found violation at index 1
 *   Result: NOT SORTED
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   // Check ascending
 *   for i = 0 to n-2:
 *       if arr[i] > arr[i+1]:
 *           return false
 *   return true
 *
 *   // Check descending
 *   for i = 0 to n-2:
 *       if arr[i] < arr[i+1]:
 *           return false
 *   return true
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Single pass through array
 * - Compare n-1 pairs
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only loop variable
 * - Constant space
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Empty array (n=0): Considered sorted
 * 2. Single element (n=1): Considered sorted
 * 3. All same elements: Both ascending and descending
 * 4. Two elements: One comparison
 *
 * ============================================================================
 * VARIATIONS:
 * ============================================================================
 *
 * 1. Check if sorted and rotated:
 *    - Count violations (arr[i] > arr[i+1])
 *    - If exactly one violation AND arr[n-1] <= arr[0], it's rotated sorted
 *
 * 2. Find minimum swaps to sort:
 *    - Different algorithm (graph-based)
 *
 * 3. Check if sorted by removing one element:
 *    - Find violation, try removing either element
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Check if sorted in ascending order
bool is_sorted_ascending(int arr[], int n) {
    if (n <= 1) return true;

    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

// Check if sorted in descending order
bool is_sorted_descending(int arr[], int n) {
    if (n <= 1) return true;

    for (int i = 0; i < n - 1; i++) {
        if (arr[i] < arr[i + 1]) {
            return false;
        }
    }
    return true;
}

// Check if sorted (either order)
bool is_sorted(int arr[], int n) {
    return is_sorted_ascending(arr, n) || is_sorted_descending(arr, n);
}

// Check if sorted and rotated
bool is_sorted_rotated(int arr[], int n) {
    if (n <= 1) return true;

    int violations = 0;
    int violation_index = -1;

    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            violations++;
            violation_index = i;
        }
    }

    // Check wrap-around
    if (violations == 0) return true;  // Already sorted
    if (violations == 1 && arr[n - 1] <= arr[0]) return true;

    return false;
}

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]");
}

int main() {
    // Test ascending sorted
    int arr1[] = {1, 2, 3, 4, 5};
    printf("Array: ");
    print_array(arr1, 5);
    printf("\nAscending: %s\n", is_sorted_ascending(arr1, 5) ? "Yes" : "No");

    // Test descending sorted
    int arr2[] = {5, 4, 3, 2, 1};
    printf("\nArray: ");
    print_array(arr2, 5);
    printf("\nDescending: %s\n", is_sorted_descending(arr2, 5) ? "Yes" : "No");

    // Test not sorted
    int arr3[] = {1, 3, 2, 4, 5};
    printf("\nArray: ");
    print_array(arr3, 5);
    printf("\nSorted: %s\n", is_sorted(arr3, 5) ? "Yes" : "No");

    // Test sorted and rotated
    int arr4[] = {3, 4, 5, 1, 2};
    printf("\nArray: ");
    print_array(arr4, 5);
    printf("\nSorted & Rotated: %s\n", is_sorted_rotated(arr4, 5) ? "Yes" : "No");

    // Test all same
    int arr5[] = {3, 3, 3, 3};
    printf("\nArray: ");
    print_array(arr5, 4);
    printf("\nSorted: %s\n", is_sorted(arr5, 4) ? "Yes" : "No");

    return 0;
}
