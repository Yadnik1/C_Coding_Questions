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
    // Handle arrays with 0 or 1 elements
    // Say: "First, I check if the array has zero or one element, which is always sorted"
    if (n <= 1) return true;

    // Loop through array comparing adjacent elements
    // Say: "I'll iterate through the array comparing each element with the next"
    for (int i = 0; i < n - 1; i++) {
        // Check if current element is greater than next
        // Say: "If I find any element greater than its next neighbor"
        if (arr[i] > arr[i + 1]) {
            // Say: "The array is not sorted ascending, so I return false"
            return false;
        }
    }
    // Say: "If all adjacent pairs are in order, the array is sorted"
    return true;
}

// Check if sorted in descending order
bool is_sorted_descending(int arr[], int n) {
    // Handle arrays with 0 or 1 elements
    // Say: "For descending, I also handle the base case of arrays with one or zero elements"
    if (n <= 1) return true;

    // Loop through array comparing adjacent elements
    // Say: "I compare each element to see if it's greater than or equal to the next"
    for (int i = 0; i < n - 1; i++) {
        // Check if current element is less than next
        // Say: "If any element is less than its next neighbor"
        if (arr[i] < arr[i + 1]) {
            // Say: "Then it's not sorted descending, so I return false"
            return false;
        }
    }
    // Say: "All elements are in descending order, so I return true"
    return true;
}

// Check if sorted (either order)
bool is_sorted(int arr[], int n) {
    // Say: "To check if sorted in either direction, I check both ascending and descending"
    return is_sorted_ascending(arr, n) || is_sorted_descending(arr, n);
}

// Check if sorted and rotated
bool is_sorted_rotated(int arr[], int n) {
    // Handle base case
    // Say: "For sorted and rotated, I first handle the base case"
    if (n <= 1) return true;

    // Initialize violation counter
    // Say: "I'll count how many times the order is violated"
    int violations = 0;

    // Track where violation occurs
    // Say: "And track the index where violation happens"
    int violation_index = -1;

    // Count violations in the array
    // Say: "I scan through looking for places where a larger element comes before a smaller one"
    for (int i = 0; i < n - 1; i++) {
        // Check if current is greater than next
        // Say: "Each time I find element greater than its neighbor, I count it as a violation"
        if (arr[i] > arr[i + 1]) {
            // Increment violation count
            violations++;
            // Save the violation index
            violation_index = i;
        }
    }

    // Check wrap-around
    // Say: "Now I check the results"
    if (violations == 0) return true;  // Already sorted
    // Say: "If there's exactly one violation and last element is less than or equal to first"
    if (violations == 1 && arr[n - 1] <= arr[0]) return true;
    // Say: "Then it's a rotated sorted array, otherwise it's not sorted"

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
