/*
 * ============================================================================
 * PROBLEM: Maximum Subarray Sum (Kadane's Algorithm)
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: VERY HIGH
 *
 * Find the contiguous subarray with the largest sum.
 *
 * Example:
 * Input:  [-2, 1, -3, 4, -1, 2, 1, -5, 4]
 * Output: 6 (subarray [4, -1, 2, 1])
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. KADANE'S ALGORITHM:
 *    - Dynamic programming approach
 *    - Track current sum, reset if it goes negative
 *    - Update max whenever current sum is larger
 *
 * 2. KEY INSIGHT:
 *    - If current sum becomes negative, start fresh
 *    - A negative prefix can only hurt the total
 *
 * 3. DECISION AT EACH ELEMENT:
 *    - Either extend current subarray
 *    - Or start new subarray from current element
 *    - Pick whichever gives larger sum
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * Array: [-2, 1, -3, 4, -1, 2, 1, -5, 4]
 *
 * Initialize: current_sum = 0, max_sum = INT_MIN
 *
 * Index 0: arr[0] = -2
 *   current = max(-2, 0 + -2) = -2
 *   max_sum = max(INT_MIN, -2) = -2
 *
 * Index 1: arr[1] = 1
 *   current = max(1, -2 + 1) = max(1, -1) = 1
 *   "Start fresh with 1 (better than -1)"
 *   max_sum = max(-2, 1) = 1
 *
 * Index 2: arr[2] = -3
 *   current = max(-3, 1 + -3) = max(-3, -2) = -2
 *   max_sum = max(1, -2) = 1
 *
 * Index 3: arr[3] = 4
 *   current = max(4, -2 + 4) = max(4, 2) = 4
 *   "Start fresh with 4"
 *   max_sum = max(1, 4) = 4
 *
 * Index 4: arr[4] = -1
 *   current = max(-1, 4 + -1) = max(-1, 3) = 3
 *   max_sum = max(4, 3) = 4
 *
 * Index 5: arr[5] = 2
 *   current = max(2, 3 + 2) = max(2, 5) = 5
 *   max_sum = max(4, 5) = 5
 *
 * Index 6: arr[6] = 1
 *   current = max(1, 5 + 1) = max(1, 6) = 6
 *   max_sum = max(5, 6) = 6 ← MAXIMUM!
 *
 * Index 7: arr[7] = -5
 *   current = max(-5, 6 + -5) = max(-5, 1) = 1
 *   max_sum = max(6, 1) = 6
 *
 * Index 8: arr[8] = 4
 *   current = max(4, 1 + 4) = max(4, 5) = 5
 *   max_sum = max(6, 5) = 6
 *
 * Result: max_sum = 6 (subarray: [4, -1, 2, 1])
 *
 * ============================================================================
 * SIMPLIFIED VISUALIZATION:
 * ============================================================================
 *
 * Think of it as walking and collecting money:
 *
 *   [-2, 1, -3, 4, -1, 2, 1, -5, 4]
 *
 * Position:  -2   → Pocket: -2 (negative, might want to restart)
 * Position:   1   → Start fresh! Pocket: 1 (better than -2+1=-1)
 * Position:  -3   → Pocket: 1-3 = -2
 * Position:   4   → Start fresh! Pocket: 4 (better than -2+4=2)
 * Position:  -1   → Pocket: 4-1 = 3
 * Position:   2   → Pocket: 3+2 = 5
 * Position:   1   → Pocket: 5+1 = 6 ← Best so far!
 * Position:  -5   → Pocket: 6-5 = 1
 * Position:   4   → Pocket: 1+4 = 5
 *
 * Maximum collected at any point: 6
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   max_sum = arr[0]
 *   current_sum = arr[0]
 *
 *   for i = 1 to n-1:
 *       current_sum = max(arr[i], current_sum + arr[i])
 *       max_sum = max(max_sum, current_sum)
 *
 *   return max_sum
 *
 * ============================================================================
 * ALTERNATE FORM (RESET TO ZERO):
 * ============================================================================
 *
 *   max_sum = INT_MIN
 *   current_sum = 0
 *
 *   for each element:
 *       current_sum += element
 *       max_sum = max(max_sum, current_sum)
 *       if current_sum < 0:
 *           current_sum = 0  // Reset
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Single pass through array
 * - Constant time operations per element
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only two variables
 * - Constant space
 *
 * ============================================================================
 * VARIATIONS:
 * ============================================================================
 *
 * 1. Find the actual subarray (not just sum):
 *    - Track start/end indices when max updates
 *
 * 2. Maximum circular subarray sum:
 *    - max(Kadane's result, total_sum - min_subarray_sum)
 *
 * 3. Maximum product subarray:
 *    - Track both max and min (negative * negative = positive)
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. All negative: Return largest (least negative)
 * 2. All positive: Return sum of all
 * 3. Single element: Return that element
 * 4. Zeros: Handle normally
 *
 * ============================================================================
 */

#include <stdio.h>
#include <limits.h>

// Basic Kadane's Algorithm
int max_subarray_sum(int arr[], int n) {
    int max_sum = arr[0];
    int current_sum = arr[0];

    for (int i = 1; i < n; i++) {
        // Either extend or start fresh
        if (current_sum + arr[i] > arr[i]) {
            current_sum = current_sum + arr[i];
        } else {
            current_sum = arr[i];
        }

        // Update max
        if (current_sum > max_sum) {
            max_sum = current_sum;
        }
    }

    return max_sum;
}

// Kadane's with subarray indices
int max_subarray_with_indices(int arr[], int n, int *start, int *end) {
    int max_sum = arr[0];
    int current_sum = arr[0];
    int temp_start = 0;

    *start = 0;
    *end = 0;

    for (int i = 1; i < n; i++) {
        if (current_sum + arr[i] > arr[i]) {
            current_sum = current_sum + arr[i];
        } else {
            current_sum = arr[i];
            temp_start = i;  // New subarray starts here
        }

        if (current_sum > max_sum) {
            max_sum = current_sum;
            *start = temp_start;
            *end = i;
        }
    }

    return max_sum;
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
    int arr1[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int n1 = 9;

    printf("Array: ");
    print_array(arr1, n1);
    printf("\n\nMaximum subarray sum: %d\n", max_subarray_sum(arr1, n1));

    // With indices
    int start, end;
    int sum = max_subarray_with_indices(arr1, n1, &start, &end);
    printf("Subarray: [%d to %d] = ", start, end);
    print_array(arr1 + start, end - start + 1);
    printf("\n");

    // All negative
    int arr2[] = {-5, -3, -8, -1, -4};
    printf("\nAll negative: ");
    print_array(arr2, 5);
    printf("\nMax sum: %d\n", max_subarray_sum(arr2, 5));

    // All positive
    int arr3[] = {1, 2, 3, 4, 5};
    printf("\nAll positive: ");
    print_array(arr3, 5);
    printf("\nMax sum: %d\n", max_subarray_sum(arr3, 5));

    return 0;
}
