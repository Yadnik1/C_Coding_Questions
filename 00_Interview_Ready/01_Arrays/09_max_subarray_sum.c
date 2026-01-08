/*
 * ============================================================================
 * PROBLEM: Maximum Subarray Sum (Kadane's Algorithm)
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given an array of integers (may include negatives), find the contiguous
 * subarray with the largest sum. Return that maximum sum.
 *
 * EXAMPLES:
 * - Input: [-2, 1, -3, 4, -1, 2, 1, -5, 4]  ->  Output: 6 (subarray [4,-1,2,1])
 * - Input: [1, 2, 3, 4]                      ->  Output: 10 (entire array)
 * - Input: [-1, -2, -3]                      ->  Output: -1 (least negative)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Classic dynamic programming problem
 * - Tests optimal substructure thinking
 * - Has real-world applications (stock prices, signal processing)
 * - Most famous array algorithm - LeetCode #53
 *
 * KEY CONCEPT:
 * Kadane's Algorithm - At each position, decide: extend current subarray
 * or start fresh? If current sum goes negative, reset to 0 (start new).
 * A negative prefix can never improve any future subarray.
 *
 * VISUAL:
 *     Array: [-2, 1, -3, 4, -1, 2, 1, -5, 4]
 *
 *     i=0: curr=-2, max=-2, reset curr=0 (negative prefix useless)
 *     i=1: curr=1,  max=1
 *     i=2: curr=-2, max=1,  reset curr=0
 *     i=3: curr=4,  max=4   <- Start of best subarray
 *     i=4: curr=3,  max=4
 *     i=5: curr=5,  max=5
 *     i=6: curr=6,  max=6   <- Best sum found! [4,-1,2,1]=6
 *     i=7: curr=1,  max=6
 *     i=8: curr=5,  max=6
 *
 *     The key insight:
 *     [... -3] [4, -1, 2, 1] [-5, ...]
 *              ^^^^^^^^^^^^^^
 *              Best subarray - negative parts on sides excluded
 *
 * ============================================================================
 */

// Maximum subarray sum using Kadane's Algorithm
// Time: O(n), Space: O(1) - Single pass dynamic programming

#include <stdio.h>
#include <limits.h>

// Say: "Consistent helper to print arrays for verification"
void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

int max_subarray_sum(int arr[], int n) {
    // Say: "Kadane's algorithm - dynamic programming approach"
    int max_sum = INT_MIN;   // Say: "Global maximum - best answer found so far"
    int current_sum = 0;     // Say: "Local maximum - sum of current subarray"

    // Say: "Process each element, deciding to extend or start fresh"
    for (int i = 0; i < n; i++) {
        // Say: "Add current element to running sum"
        current_sum += arr[i];

        // Say: "Update global max if current sum is better"
        if (current_sum > max_sum) {
            max_sum = current_sum;
        }

        // Say: "Key insight: if sum goes negative, start fresh"
        // Say: "A negative prefix can never improve future subarrays"
        if (current_sum < 0) {
            current_sum = 0;  // Say: "Reset - start new subarray from next element"
        }
    }

    return max_sum;  // Say: "Return the maximum sum found"
}

// Say: "Extended version that also returns the subarray indices"
int max_subarray_with_indices(int arr[], int n, int *start, int *end) {
    int max_sum = INT_MIN;
    int current_sum = 0;
    int temp_start = 0;  // Say: "Track start of current subarray"

    for (int i = 0; i < n; i++) {
        current_sum += arr[i];

        if (current_sum > max_sum) {
            max_sum = current_sum;
            *start = temp_start;  // Say: "Record where best subarray starts"
            *end = i;             // Say: "Record where best subarray ends"
        }

        if (current_sum < 0) {
            current_sum = 0;
            temp_start = i + 1;  // Say: "Next element starts new subarray"
        }
    }

    return max_sum;
}

int main() {
    int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int n = sizeof(arr) / sizeof(arr[0]);  // Say: "Calculate array length"

    printf("Array: ");
    print_array(arr, n);

    printf("Max subarray sum: %d\n", max_subarray_sum(arr, n));

    // Say: "Demonstrate extended version with indices"
    int start, end;
    int sum = max_subarray_with_indices(arr, n, &start, &end);
    printf("Subarray [%d to %d]: sum = %d\n", start, end, sum);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use Kadane's Algorithm - a beautiful dynamic programming solution.

 Key Insight:
 At each position, we make a choice:
 1. Extend the previous subarray by including current element
 2. Start a new subarray from current element

 Decision rule:
 - If current_sum becomes negative, reset to 0
 - A negative sum can never help future subarrays

 Algorithm:
 1. Track current_sum (local max) and max_sum (global max)
 2. For each element:
    - Add element to current_sum
    - Update max_sum if current_sum is larger
    - If current_sum < 0, reset to 0 (start fresh)

 Example walkthrough for [-2, 1, -3, 4, -1, 2, 1, -5, 4]:
 - i=0: curr=-2, max=-2, reset curr=0
 - i=1: curr=1, max=1
 - i=2: curr=-2, max=1, reset curr=0
 - i=3: curr=4, max=4
 - i=4: curr=3, max=4
 - i=5: curr=5, max=5
 - i=6: curr=6, max=6  <-- Maximum found here
 - i=7: curr=1, max=6
 - i=8: curr=5, max=6
 - Result: 6 (subarray [4, -1, 2, 1])

 Time: O(n) - single pass
 Space: O(1) - only two variables

 Edge cases:
 - All negative: returns largest single element
 - All positive: returns sum of entire array
 - Empty array: need to handle separately

 Why INT_MIN for max_sum?
 - Handles all-negative arrays correctly
 - First element will always update max_sum

 Real-world applications:
 - Stock price analysis (max profit in period)
 - Signal processing (finding strongest signal)
 - Image processing (brightest region)"
*/
