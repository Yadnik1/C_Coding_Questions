/*
 * ============================================================================
 * PROBLEM: Bubble Sort
 * ============================================================================
 *
 * WHAT IS THIS ALGORITHM?
 * Bubble Sort is the simplest sorting algorithm. It repeatedly steps through
 * the array, compares adjacent elements, and swaps them if they are in the
 * wrong order. The largest unsorted element "bubbles up" to its correct
 * position at the end of each pass, like bubbles rising in water.
 *
 * EXAMPLES:
 * - Input:  [64, 34, 25, 12, 22]
 *
 * - Pass 1: Compare adjacent pairs, swap if needed
 *   [64, 34, 25, 12, 22] -> [34, 64, 25, 12, 22] (swap 64,34)
 *   [34, 64, 25, 12, 22] -> [34, 25, 64, 12, 22] (swap 64,25)
 *   [34, 25, 64, 12, 22] -> [34, 25, 12, 64, 22] (swap 64,12)
 *   [34, 25, 12, 64, 22] -> [34, 25, 12, 22, 64] (swap 64,22)
 *   64 is now in final position!
 *
 * - Pass 2: [34, 25, 12, 22, 64] -> [25, 12, 22, 34, 64]
 * - Pass 3: [25, 12, 22, 34, 64] -> [12, 22, 25, 34, 64]
 * - Pass 4: [12, 22, 25, 34, 64] -> [12, 22, 25, 34, 64] (no swaps!)
 *
 * - Output: [12, 22, 25, 34, 64]
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Tests basic algorithm understanding and loop logic
 * - Great for explaining time complexity (why O(n^2) is bad)
 * - Demonstrates optimization thinking (swapped flag for early exit)
 * - Foundation to compare against better algorithms
 * - Shows understanding of in-place sorting
 *
 * KEY CONCEPT:
 * Adjacent Comparison and Swap - compare neighbors, bubble largest to end.
 * The "swapped" flag optimization makes best case O(n) for sorted arrays.
 *
 * VISUAL:
 *
 * Pass 1: Bubble 64 to the end
 * [64, 34, 25, 12, 22]
 *  ^^  ^^
 *  64 > 34? Yes, SWAP
 *
 * [34, 64, 25, 12, 22]
 *      ^^  ^^
 *      64 > 25? Yes, SWAP
 *
 * [34, 25, 64, 12, 22]
 *          ^^  ^^
 *          64 > 12? Yes, SWAP
 *
 * [34, 25, 12, 64, 22]
 *              ^^  ^^
 *              64 > 22? Yes, SWAP
 *
 * [34, 25, 12, 22, 64]  <- 64 is now sorted!
 *                  ~~
 *
 * Pass 2: Bubble 34 to position 3
 * [34, 25, 12, 22, 64]
 *  ^^  ^^
 * [25, 34, 12, 22, 64]
 *      ^^  ^^
 * [25, 12, 34, 22, 64]
 *          ^^  ^^
 * [25, 12, 22, 34, 64]  <- 34 is now sorted!
 *              ~~  ~~
 *
 * TIME COMPLEXITY:
 * - Best:    O(n)   - Array already sorted (with swapped flag optimization)
 * - Average: O(n^2) - Random order
 * - Worst:   O(n^2) - Reverse sorted
 *
 * SPACE COMPLEXITY: O(1) - In-place sorting
 *
 * ============================================================================
 */

// Bubble Sort - Simple sorting algorithm (know for interviews)
// Time: O(n²), Space: O(1)

#include <stdio.h>
#include <stdbool.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

// Say: "Bubble sort repeatedly swaps adjacent elements if wrong order"
void bubble_sort(int arr[], int n) {
    // Say: "Outer loop: n-1 passes needed"
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;  // Say: "Optimization: track if any swap occurred"

        // Say: "Inner loop: compare adjacent pairs"
        // Say: "After pass i, last i elements are sorted"
        for (int j = 0; j < n - 1 - i; j++) {
            // Say: "If left > right, swap them"
            if (arr[j] > arr[j + 1]) {
                // Say: "Standard swap using temp"
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }

        // Say: "If no swaps in a pass, array is sorted - early exit"
        if (!swapped) {
            break;  // Say: "Optimization: O(n) for already sorted array"
        }
    }
}

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original: ");
    print_array(arr, n);

    bubble_sort(arr, n);

    printf("Sorted:   ");
    print_array(arr, n);

    // Test already sorted (shows optimization)
    int sorted[] = {1, 2, 3, 4, 5};
    int n2 = sizeof(sorted) / sizeof(sorted[0]);
    printf("\nAlready sorted: ");
    print_array(sorted, n2);
    bubble_sort(sorted, n2);
    printf("After sort:     ");
    print_array(sorted, n2);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Bubble sort repeatedly steps through the list, compares adjacent elements,
 and swaps them if they're in wrong order. Largest elements 'bubble up' to the end.

 ALGORITHM:
 1. Compare arr[0] with arr[1], swap if out of order
 2. Compare arr[1] with arr[2], swap if out of order
 3. Continue to end - largest element is now at last position
 4. Repeat for remaining unsorted portion
 5. Optimize: if no swaps in a pass, array is sorted

 WHY n-1-i IN INNER LOOP:
 - After pass i, last i elements are in final position
 - No need to compare them again
 - Reduces comparisons each pass

 COMPLEXITY:
 - Worst case: O(n²) - reverse sorted array
 - Average case: O(n²)
 - Best case: O(n) - already sorted (with optimization)
 - Space: O(1) - in-place

 WHEN TO USE:
 - Educational purposes
 - Very small arrays
 - When simplicity matters more than performance
 - Nearly sorted data (with optimization)

 WHY NOT USE IN PRODUCTION:
 - O(n²) is too slow for large data
 - Insertion sort is better for small/nearly sorted data
 - Quick sort or merge sort for general use

 COMPARISON WITH OTHER O(n²) SORTS:
 - Bubble: Most swaps, simplest concept
 - Selection: Fewer swaps, always O(n²)
 - Insertion: Best for nearly sorted, adaptive

 INTERVIEW TIP:
 - Know it exists and why it's O(n²)
 - Mention the swapped flag optimization
 - Explain why better algorithms exist"
*/
