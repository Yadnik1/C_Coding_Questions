/*
 * ============================================================================
 * PROBLEM: Dutch National Flag (Three-Way Partition)
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given an array containing only 0s, 1s, and 2s, sort it in a single pass.
 * Named after the Dutch flag (red, white, blue = three colors/values).
 * Must be in-place with O(1) space.
 *
 * EXAMPLES:
 * - Input: [2, 0, 1, 2, 1, 0]     ->  Output: [0, 0, 1, 1, 2, 2]
 * - Input: [2, 2, 2, 0, 0, 0, 1]  ->  Output: [0, 0, 0, 1, 2, 2, 2]
 * - Input: [1, 0, 2]              ->  Output: [0, 1, 2]
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Classic algorithm by Dijkstra (foundational CS problem)
 * - Tests three-pointer technique mastery
 * - Core of quicksort's three-way partition (handles duplicates)
 * - Demonstrates single-pass optimization thinking
 *
 * KEY CONCEPT:
 * Three-Way Partitioning - Maintain four regions using three pointers:
 * [0...low-1] = 0s, [low...mid-1] = 1s, [mid...high] = unknown,
 * [high+1...n-1] = 2s. Process unknown region until empty.
 *
 * VISUAL:
 *     Array: [2, 0, 1, 2, 1, 0]
 *     Pointers: low=0, mid=0, high=5
 *
 *     [2, 0, 1, 2, 1, 0]
 *      L                 H       L=low, M=mid, H=high
 *      M
 *
 *     mid=0: arr[0]=2, swap with high
 *     [0, 0, 1, 2, 1, 2]
 *      L              H          high--
 *      M
 *
 *     mid=0: arr[0]=0, swap with low (self)
 *     [0, 0, 1, 2, 1, 2]
 *         L           H          low++, mid++
 *         M
 *
 *     Continue until mid > high...
 *
 *     Final: [0, 0, 1, 1, 2, 2]
 *             ^^^^  ^^^^  ^^^^
 *             0s    1s    2s
 *
 *     KEY INSIGHT: Don't increment mid after swap with high!
 *     (We don't know what came from high - could be 0, 1, or 2)
 *
 * ============================================================================
 */

// Dutch National Flag Problem - Sort array of 0s, 1s, and 2s
// Time: O(n), Space: O(1) - Single pass with three-way partitioning

#include <stdio.h>

// Say: "Consistent helper to print arrays for verification"
void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

// Say: "Helper function for swapping two elements"
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void dutch_flag_sort(int arr[], int n) {
    // Say: "Three pointers divide array into four regions"
    int low = 0;       // Say: "Boundary for 0s - everything before low is 0"
    int mid = 0;       // Say: "Current element being examined"
    int high = n - 1;  // Say: "Boundary for 2s - everything after high is 2"

    // Say: "Process until mid crosses high"
    while (mid <= high) {
        if (arr[mid] == 0) {
            // Say: "Found a 0 - swap it to the 0s region"
            swap(&arr[low], &arr[mid]);
            low++;  // Say: "Expand 0s region"
            mid++;  // Say: "Move to next element - we know what's at low"
            // Say: "We can increment mid because swapped element is 0 or 1"
        }
        else if (arr[mid] == 1) {
            // Say: "1s are in correct region - just move forward"
            mid++;
            // Say: "1s naturally stay in the middle section"
        }
        else {  // arr[mid] == 2
            // Say: "Found a 2 - swap it to the 2s region"
            swap(&arr[mid], &arr[high]);
            high--;  // Say: "Expand 2s region"
            // Say: "DON'T increment mid - need to examine swapped element"
            // Say: "We don't know what came from high, could be 0, 1, or 2"
        }
    }
    // Say: "Array is now partitioned: [0s | 1s | 2s]"
}

int main() {
    int arr[] = {2, 0, 1, 2, 1, 0, 0, 2, 1, 0};
    int n = sizeof(arr) / sizeof(arr[0]);  // Say: "Calculate array length"

    printf("Original: ");
    print_array(arr, n);

    dutch_flag_sort(arr, n);

    printf("Sorted:   ");
    print_array(arr, n);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"This is the Dutch National Flag problem, named by Dijkstra.

 Problem: Sort array containing only 0s, 1s, and 2s in single pass.

 Key Insight - Three-way Partitioning:
 We maintain four regions in the array:
 [0...low-1]     -> all 0s (sorted)
 [low...mid-1]   -> all 1s (sorted)
 [mid...high]    -> unknown (to be processed)
 [high+1...n-1]  -> all 2s (sorted)

 Algorithm:
 1. Initialize: low=0, mid=0, high=n-1
 2. While mid <= high:
    - arr[mid] == 0: swap with low, increment both low and mid
    - arr[mid] == 1: just increment mid
    - arr[mid] == 2: swap with high, decrement high only

 Critical Detail - Why not increment mid after swapping with high?
 - When we swap with high, we don't know what came from high
 - It could be 0, 1, or 2 - we need to examine it
 - But when swapping with low, we know it's 0 or 1 (already processed)

 Example walkthrough for [2, 0, 1, 2, 1, 0]:
 Initial: low=0, mid=0, high=5
 - mid=0: arr[0]=2, swap with high, [0,0,1,2,1,2], high=4
 - mid=0: arr[0]=0, swap with low, [0,0,1,2,1,2], low=1, mid=1
 - mid=1: arr[1]=0, swap with low, [0,0,1,2,1,2], low=2, mid=2
 - mid=2: arr[2]=1, mid=3
 - mid=3: arr[3]=2, swap with high, [0,0,1,1,2,2], high=3
 - mid=3: arr[3]=1, mid=4
 - mid=4 > high=3: done!
 Result: [0, 0, 1, 1, 2, 2]

 Time: O(n) - each element processed at most twice
 Space: O(1) - only three pointer variables

 Why is it called Dutch National Flag?
 - Dutch flag has three horizontal bands: red, white, blue
 - Analogous to sorting three distinct values

 Real-world applications:
 - Quicksort's three-way partition (handles duplicates efficiently)
 - Sorting by priority levels (high, medium, low)
 - Segregating data into categories

 Variants:
 - Four colors: use four pointers
 - Two colors: simple partition (Dutch flag simplifies to this)"
*/
