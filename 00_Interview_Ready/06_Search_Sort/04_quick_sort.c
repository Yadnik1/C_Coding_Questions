/*
 * ============================================================================
 * PROBLEM: Quick Sort
 * ============================================================================
 *
 * WHAT IS THIS ALGORITHM?
 * Quick Sort is a highly efficient divide-and-conquer sorting algorithm.
 * It works by selecting a "pivot" element and partitioning the array so that
 * all elements smaller than pivot go left, and all larger elements go right.
 * The pivot is now in its final sorted position. Recursively apply the same
 * process to the left and right subarrays.
 *
 * EXAMPLES:
 * - Input:  [10, 7, 8, 9, 1, 5]  (pivot = 5, last element)
 *
 * - Partition around pivot 5:
 *   Elements < 5 go left, elements > 5 go right
 *   [1, 5, 8, 9, 10, 7]  Wait, let's trace step by step...
 *
 *   Initial: [10, 7, 8, 9, 1, 5]  pivot=5, i=-1
 *             j
 *   j=0: 10 > 5, no swap
 *   j=1: 7 > 5, no swap
 *   j=2: 8 > 5, no swap
 *   j=3: 9 > 5, no swap
 *   j=4: 1 < 5, i++, swap arr[0] with arr[4] -> [1, 7, 8, 9, 10, 5]
 *   Final: swap pivot with arr[i+1] -> [1, 5, 8, 9, 10, 7]
 *          Pivot 5 is now at index 1 (its final position!)
 *
 * - Recursively sort: [1] and [8, 9, 10, 7]
 * - Output: [1, 5, 7, 8, 9, 10]
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Most widely used sorting algorithm in practice (C's qsort, Java's Arrays.sort)
 * - Tests divide-and-conquer understanding
 * - Partition logic is tricky - common interview coding question
 * - Discusses worst case and how to prevent it (random pivot)
 * - In-place sorting with O(log n) space (recursion stack)
 *
 * KEY CONCEPT:
 * Partition - choose a pivot, rearrange array so smaller elements are left,
 * larger are right. Pivot ends up in its final sorted position. Recursively
 * sort the subarrays.
 *
 * VISUAL:
 *
 * Sorting [10, 7, 8, 9, 1, 5] using Lomuto partition (pivot = last element)
 *
 * Step 1: Partition with pivot = 5
 * +----+----+----+----+----+----+
 * | 10 |  7 |  8 |  9 |  1 |  5 | <- pivot
 * +----+----+----+----+----+----+
 *   j                        pivot
 *   i = -1 (boundary of "smaller" region)
 *
 * Scan with j, when arr[j] < pivot, expand smaller region:
 *
 * j=0: 10 > 5, skip
 * j=1: 7 > 5, skip
 * j=2: 8 > 5, skip
 * j=3: 9 > 5, skip
 * j=4: 1 < 5, i++, swap arr[i] with arr[j]
 *
 * +----+----+----+----+----+----+
 * |  1 |  7 |  8 |  9 | 10 |  5 |
 * +----+----+----+----+----+----+
 *   i                        pivot
 *
 * Final: swap arr[i+1] with pivot
 *
 * +----+----+----+----+----+----+
 * |  1 |  5 |  8 |  9 | 10 |  7 |
 * +----+----+----+----+----+----+
 *        ^
 *      pivot in final position!
 *
 * Left:  [1]           - already sorted
 * Right: [8, 9, 10, 7] - recursively partition
 *
 * TIME COMPLEXITY:
 * - Best:    O(n log n) - Balanced partitions
 * - Average: O(n log n) - Random data
 * - Worst:   O(n^2)     - Already sorted (bad pivot choice)
 *
 * SPACE COMPLEXITY: O(log n) average (recursion stack), O(n) worst case
 *
 * ============================================================================
 */

// Quick Sort - Most commonly used sorting algorithm
// Time: O(n log n) average, O(n²) worst, Space: O(log n) stack

#include <stdio.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Say: "Partition places pivot in correct position"
// Say: "Elements smaller go left, larger go right"
int partition(int arr[], int low, int high) {
    // Say: "Choose last element as pivot (Lomuto scheme)"
    int pivot = arr[high];
    int i = low - 1;  // Say: "i tracks boundary of smaller elements"

    // Say: "Scan through array, moving smaller elements to left"
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);  // Say: "Expand smaller region"
        }
    }

    // Say: "Place pivot in its final position"
    swap(&arr[i + 1], &arr[high]);

    return i + 1;  // Say: "Return pivot's index"
}

// Say: "Quick sort recursively partitions and sorts"
void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        // Say: "Partition array around pivot"
        int pi = partition(arr, low, high);

        // Say: "Recursively sort left and right of pivot"
        quick_sort(arr, low, pi - 1);   // Say: "Sort left part"
        quick_sort(arr, pi + 1, high);  // Say: "Sort right part"
    }
}

// Wrapper function
void sort(int arr[], int n) {
    quick_sort(arr, 0, n - 1);
}

int main() {
    int arr[] = {10, 7, 8, 9, 1, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original: ");
    print_array(arr, n);

    sort(arr, n);

    printf("Sorted:   ");
    print_array(arr, n);

    // Test with duplicates
    int arr2[] = {3, 3, 3, 1, 1, 2, 2};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("\nWith duplicates: ");
    print_array(arr2, n2);
    sort(arr2, n2);
    printf("Sorted:          ");
    print_array(arr2, n2);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Quick sort is a divide-and-conquer algorithm that partitions the array
 around a pivot element.

 ALGORITHM:
 1. Choose a pivot element
 2. Partition: rearrange so smaller elements go left, larger go right
 3. Pivot is now in its final sorted position
 4. Recursively sort left and right subarrays

 PARTITION (Lomuto scheme):
 - Choose last element as pivot
 - Maintain boundary i for smaller elements
 - Scan with j, swap smaller elements to left side
 - Finally place pivot at i+1

 COMPLEXITY:
 - Best/Average: O(n log n)
 - Worst: O(n²) - when array is already sorted (bad pivot)
 - Space: O(log n) for recursion stack

 WHY QUICK SORT IS POPULAR:
 1. O(n log n) average case
 2. In-place (O(1) extra space for data)
 3. Cache friendly (sequential memory access)
 4. Faster in practice than merge sort

 WORST CASE PREVENTION:
 1. Random pivot selection
 2. Median of three (first, middle, last)
 3. Use insertion sort for small subarrays

 COMPARISON WITH MERGE SORT:
 Quick Sort:
 - In-place, O(1) space
 - Not stable
 - O(n²) worst case
 - Cache friendly

 Merge Sort:
 - Needs O(n) extra space
 - Stable
 - Always O(n log n)
 - Good for linked lists

 EMBEDDED CONSIDERATIONS:
 - Watch recursion depth (stack overflow risk)
 - Consider iterative version for safety
 - Insertion sort for small arrays saves stack space

 COMMON MISTAKES:
 - Not handling equal elements properly
 - Stack overflow on nearly sorted input
 - Off-by-one in partition boundaries"
*/
