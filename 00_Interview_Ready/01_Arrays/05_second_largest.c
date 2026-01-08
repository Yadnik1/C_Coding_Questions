/*
 * ============================================================================
 * PROBLEM: Find Second Largest Element
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given an array of integers, find the second largest element.
 * Must handle duplicates correctly - if largest appears multiple times,
 * second largest is the next distinct smaller value.
 *
 * EXAMPLES:
 * - Input: [12, 35, 1, 10, 34, 1]  ->  Output: 34 (largest is 35)
 * - Input: [5, 5, 5, 4]            ->  Output: 4  (5 is largest, 4 is second)
 * - Input: [10]                    ->  Output: None (not enough elements)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Tests single-pass algorithm design
 * - Validates handling of edge cases (duplicates, single element)
 * - Foundation for k-th largest problems
 * - Common in competitive programming and coding rounds
 *
 * KEY CONCEPT:
 * Track Two Values - Maintain first (largest) and second (second largest)
 * in a single pass. Update both appropriately when finding new values.
 *
 * VISUAL:
 *     Array: [12, 35, 1, 10, 34, 1]
 *
 *     i=0: arr[0]=12
 *          first=12, second=INT_MIN
 *
 *     i=1: arr[1]=35 > first
 *          second = first (12)
 *          first = 35
 *          Now: first=35, second=12
 *
 *     i=2: arr[2]=1 < second (12), skip
 *
 *     i=3: arr[3]=10 < second (12), skip
 *
 *     i=4: arr[4]=34 > second (12) but < first (35)
 *          second = 34
 *          Now: first=35, second=34
 *
 *     i=5: arr[5]=1 < second, skip
 *
 *     Result: second = 34
 *
 * ============================================================================
 */

// Find second largest element in array
// Time: O(n), Space: O(1) - Single pass

#include <stdio.h>
#include <limits.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

int second_largest(int arr[], int n) {
    // Say: "Track two values - largest and second largest"
    int first = INT_MIN;   // Say: "Start with smallest possible value"
    int second = INT_MIN;

    // Say: "Single pass through array"
    for (int i = 0; i < n; i++) {
        if (arr[i] > first) {
            // Say: "New largest found - old largest becomes second"
            second = first;
            first = arr[i];
        } else if (arr[i] > second && arr[i] != first) {
            // Say: "New second largest found (not equal to first)"
            second = arr[i];
        }
    }

    return second;  // Say: "Returns INT_MIN if no second largest exists"
}

int main() {
    int arr[] = {12, 35, 1, 10, 34, 1};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array: ");
    print_array(arr, n);

    printf("Second largest: %d\n", second_largest(arr, n));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I track first and second largest in a single pass.

 For each element:
 1. If greater than first: update second=first, first=current
 2. Else if greater than second (and not equal to first): update second

 Why check arr[i] != first?
 - Handles duplicates like [5,5,3] - second should be 3, not 5

 Time: O(n) - single pass
 Space: O(1) - only two variables

 Edge cases to mention:
 - Array with all same elements: returns INT_MIN
 - Array with size 1: returns INT_MIN"
*/
