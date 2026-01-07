// Merge Sort - Stable O(n log n) sorting algorithm
// Time: O(n log n) always, Space: O(n)

#include <stdio.h>
#include <stdlib.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

// Say: "Merge two sorted halves into one sorted array"
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;  // Say: "Size of left half"
    int n2 = right - mid;      // Say: "Size of right half"

    // Say: "Create temporary arrays"
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    // Say: "Copy data to temp arrays"
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Say: "Merge temp arrays back into arr[left..right]"
    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        // Say: "Compare and take smaller element"
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Say: "Copy remaining elements of L[] if any"
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Say: "Copy remaining elements of R[] if any"
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

// Say: "Divide array into halves, sort, then merge"
void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        // Say: "Find middle point"
        int mid = left + (right - left) / 2;

        // Say: "Recursively sort first and second halves"
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        // Say: "Merge the sorted halves"
        merge(arr, left, mid, right);
    }
}

// Wrapper function
void sort(int arr[], int n) {
    merge_sort(arr, 0, n - 1);
}

int main() {
    int arr[] = {38, 27, 43, 3, 9, 82, 10};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original: ");
    print_array(arr, n);

    sort(arr, n);

    printf("Sorted:   ");
    print_array(arr, n);

    // Test stability with equal elements
    int arr2[] = {5, 2, 8, 2, 1, 9};
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
"Merge sort is a divide-and-conquer algorithm that divides the array,
 recursively sorts halves, then merges them.

 ALGORITHM:
 1. Divide: Split array into two halves
 2. Conquer: Recursively sort both halves
 3. Combine: Merge two sorted halves

 MERGE OPERATION:
 - Two pointers, one for each sorted half
 - Compare elements, take smaller one
 - Advance that pointer
 - Copy remaining elements when one half exhausted

 COMPLEXITY:
 - Time: O(n log n) - always! (best, average, worst)
 - Space: O(n) for temporary arrays
 - Recursion depth: O(log n)

 WHY O(n log n):
 - log n levels of recursion (halving each time)
 - O(n) work at each level (merging)
 - Total: O(n log n)

 ADVANTAGES:
 1. Guaranteed O(n log n) - no worst case
 2. Stable sort (preserves order of equal elements)
 3. Good for linked lists (O(1) space)
 4. Parallelizable (merge independent halves)
 5. External sorting (large files on disk)

 DISADVANTAGES:
 1. O(n) extra space for arrays
 2. Not in-place
 3. Slower than quick sort in practice (more memory ops)

 WHEN TO USE MERGE SORT:
 1. When stability is required
 2. Sorting linked lists
 3. External sorting (files larger than RAM)
 4. When worst case O(n log n) is needed
 5. Parallel sorting

 COMPARISON WITH QUICK SORT:
 | Aspect        | Merge Sort    | Quick Sort    |
 |---------------|---------------|---------------|
 | Time (worst)  | O(n log n)    | O(n²)         |
 | Space         | O(n)          | O(log n)      |
 | Stable        | Yes           | No            |
 | Cache         | Less friendly | More friendly |
 | Linked lists  | Excellent     | Poor          |

 EMBEDDED NOTE:
 - O(n) space can be problematic
 - Consider in-place merge sort (complex, slower)
 - Bottom-up iterative version avoids recursion"
*/
