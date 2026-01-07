// Insertion Sort - Best for small/nearly sorted arrays
// Time: O(n²) worst, O(n) best, Space: O(1)

#include <stdio.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

// Say: "Insertion sort builds sorted array one element at a time"
void insertion_sort(int arr[], int n) {
    // Say: "Start from second element (first is trivially sorted)"
    for (int i = 1; i < n; i++) {
        int key = arr[i];  // Say: "Element to insert into sorted portion"
        int j = i - 1;

        // Say: "Shift elements greater than key to the right"
        // Say: "This makes room for key at correct position"
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];  // Say: "Shift right"
            j--;
        }

        // Say: "Insert key at its correct position"
        arr[j + 1] = key;
    }
}

// Binary insertion sort - uses binary search to find position
// Say: "Reduces comparisons but still O(n²) due to shifts"
void binary_insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];

        // Say: "Binary search to find insertion position"
        int left = 0, right = i - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (arr[mid] > key) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        // Say: "left is the insertion position"

        // Say: "Shift elements from left to i-1"
        for (int j = i - 1; j >= left; j--) {
            arr[j + 1] = arr[j];
        }

        arr[left] = key;
    }
}

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original: ");
    print_array(arr, n);

    insertion_sort(arr, n);

    printf("Sorted:   ");
    print_array(arr, n);

    // Test nearly sorted
    int nearly[] = {1, 2, 4, 3, 5, 6};
    int n2 = sizeof(nearly) / sizeof(nearly[0]);
    printf("\nNearly sorted: ");
    print_array(nearly, n2);
    insertion_sort(nearly, n2);
    printf("After sort:    ");
    print_array(nearly, n2);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Insertion sort builds the sorted array one element at a time,
 like sorting cards in your hand.

 ALGORITHM:
 1. Start with first element (trivially sorted)
 2. Take next element (key)
 3. Shift all larger elements in sorted portion to the right
 4. Insert key at correct position
 5. Repeat until all elements processed

 VISUALIZATION:
 [64, 34, 25, 12] - start
 [34, 64, 25, 12] - insert 34
 [25, 34, 64, 12] - insert 25
 [12, 25, 34, 64] - insert 12

 COMPLEXITY:
 - Worst case: O(n²) - reverse sorted
 - Average case: O(n²)
 - Best case: O(n) - already sorted (no shifts needed)
 - Space: O(1) - in-place

 WHY INSERTION SORT IS USEFUL:
 1. Best for small arrays (n < 10-20)
 2. Best for nearly sorted arrays
 3. Online algorithm (can sort as data arrives)
 4. Stable sort (preserves order of equal elements)
 5. Used as base case in hybrid sorts (TimSort, IntroSort)

 WHEN TO USE:
 - Small arrays (overhead of complex sorts not worth it)
 - Nearly sorted data
 - When stability matters
 - Real-time systems with small data

 COMPARISON:
 - Bubble sort: More swaps, simpler
 - Selection sort: Fewer swaps but always O(n²)
 - Insertion sort: Adaptive, best for nearly sorted

 BINARY INSERTION SORT:
 - Uses binary search to find position: O(log n) comparisons
 - Still O(n) shifts per element
 - Total: O(n log n) comparisons but O(n²) time overall"
*/
