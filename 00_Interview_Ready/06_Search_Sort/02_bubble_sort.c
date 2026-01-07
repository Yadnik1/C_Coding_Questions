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
