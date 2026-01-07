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
