/*
 * ============================================================================
 * PROBLEM: Rotate Array by K Positions
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given an array, rotate all elements to the right by k positions.
 * Elements that "fall off" the right end wrap around to the left.
 * Must be done in-place with O(1) extra space.
 *
 * EXAMPLES:
 * - Input: [1,2,3,4,5], k=2  ->  Output: [4,5,1,2,3]
 * - Input: [1,2,3,4,5], k=3  ->  Output: [3,4,5,1,2]
 * - Input: [1,2], k=3        ->  Output: [2,1] (k=3 mod 2 = 1)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Tests creative algorithm design (reversal trick is non-obvious)
 * - Validates in-place manipulation skills
 * - Shows understanding of modular arithmetic (k % n)
 * - Foundation for circular buffer and queue implementations
 *
 * KEY CONCEPT:
 * Three-Reversal Method - Clever O(n) time, O(1) space solution.
 * 1. Reverse the entire array
 * 2. Reverse first k elements
 * 3. Reverse remaining n-k elements
 *
 * VISUAL:
 *     Rotate [1,2,3,4,5] right by k=2:
 *
 *     Original:     [1] [2] [3] [4] [5]
 *
 *     Step 1:       [5] [4] [3] [2] [1]    <- Reverse ALL
 *
 *     Step 2:       [4] [5] [3] [2] [1]    <- Reverse first k=2
 *                    ^^^^^^
 *
 *     Step 3:       [4] [5] [1] [2] [3]    <- Reverse rest (n-k=3)
 *                            ^^^^^^^^^^
 *
 *     Result: Last 2 elements moved to front!
 *
 * ============================================================================
 */

// Rotate array right by k positions
// Time: O(n), Space: O(1) - Reverse method

#include <stdio.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

// Say: "Helper function to reverse a portion of array"
void reverse(int arr[], int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

void rotate_array(int arr[], int n, int k) {
    // Say: "Handle k > n case by taking modulo"
    k = k % n;
    if (k == 0) return;  // Say: "No rotation needed"

    // Say: "Three-step reverse trick"
    // Say: "Example: [1,2,3,4,5], k=2"

    reverse(arr, 0, n - 1);      // Say: "Step 1: Reverse all -> [5,4,3,2,1]"
    reverse(arr, 0, k - 1);      // Say: "Step 2: Reverse first k -> [4,5,3,2,1]"
    reverse(arr, k, n - 1);      // Say: "Step 3: Reverse rest -> [4,5,1,2,3]"

    // Say: "Result: rotated right by k positions"
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 3;

    printf("Original: ");
    print_array(arr, n);

    rotate_array(arr, n, k);

    printf("Rotated by %d: ", k);
    print_array(arr, n);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use the reversal algorithm - three reverses in-place.

 To rotate right by k:
 1. Reverse entire array
 2. Reverse first k elements
 3. Reverse remaining n-k elements

 Example: [1,2,3,4,5], k=2
 After step 1: [5,4,3,2,1]
 After step 2: [4,5,3,2,1]
 After step 3: [4,5,1,2,3] ✓

 Why this approach?
 - O(n) time: each element reversed at most twice
 - O(1) space: in-place, no extra array
 - Works for any k value after taking modulo"
*/
