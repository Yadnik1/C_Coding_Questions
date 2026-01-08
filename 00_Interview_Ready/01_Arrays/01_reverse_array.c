/*
 * ============================================================================
 * PROBLEM: Reverse Array In-Place
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given an array, reverse all elements in-place without using extra space.
 * The first element becomes last, second becomes second-last, and so on.
 *
 * EXAMPLES:
 * - Input: [1, 2, 3, 4, 5]  ->  Output: [5, 4, 3, 2, 1]
 * - Input: [10, 20]         ->  Output: [20, 10]
 * - Input: [7]              ->  Output: [7] (single element unchanged)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Tests understanding of two-pointer technique (fundamental pattern)
 * - Validates ability to modify data in-place with O(1) space
 * - Foundation for more complex problems (rotate array, palindrome check)
 * - Common building block in string manipulation algorithms
 *
 * KEY CONCEPT:
 * Two-Pointer Technique - Use two pointers starting from opposite ends.
 * Swap elements at both pointers, then move them toward the center.
 * Stop when pointers meet or cross.
 *
 * VISUAL:
 *     Initial:  [1] [2] [3] [4] [5]
 *                L               R      <- L=left, R=right
 *
 *     Step 1:   [5] [2] [3] [4] [1]     <- swap 1 and 5
 *                    L       R
 *
 *     Step 2:   [5] [4] [3] [2] [1]     <- swap 2 and 4
 *                        LR             <- pointers meet, DONE!
 *
 * ============================================================================
 */

// Reverse an array in-place
// Time: O(n), Space: O(1)

#include <stdio.h>

// Say: "Consistent helper to print arrays for verification"
void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

void reverse_array(int arr[], int n) {
    // Say: "Two pointers - left starts at beginning, right at end"
    int left = 0;
    int right = n - 1;

    // Say: "Continue until pointers meet in the middle"
    while (left < right) {
        // Say: "Swap elements at left and right positions"
        int temp = arr[left];     // Store left element
        arr[left] = arr[right];   // Move right to left
        arr[right] = temp;        // Move stored value to right

        // Say: "Move pointers toward center"
        left++;
        right--;
    }
    // Say: "When left >= right, we've swapped all pairs"
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);  // Say: "Get array length"

    printf("Original: ");
    print_array(arr, n);

    reverse_array(arr, n);

    printf("Reversed: ");
    print_array(arr, n);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use two-pointer technique - left pointer starts at index 0,
 right pointer starts at index n-1. I swap elements at both
 positions and move pointers toward each other. When they
 meet or cross, the array is fully reversed.

 Why this approach?
 - O(n) time: we touch each element once
 - O(1) space: only using temp variable, no extra array
 - In-place: modifies original array directly"
*/
