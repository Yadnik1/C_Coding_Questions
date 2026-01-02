/*
 * ============================================================================
 * PROBLEM: Insertion Sort
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: High
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. CONCEPT: Like sorting cards in hand
 *
 * 2. PROCESS:
 *    - Take element from unsorted portion
 *    - Insert it in correct position in sorted portion
 *
 * 3. ADVANTAGE: Efficient for nearly sorted arrays O(n)
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: [5, 2, 4, 6, 1, 3]
 * ============================================================================
 *
 * Initial: [5] | [2, 4, 6, 1, 3]
 *          sorted | unsorted
 *
 * Insert 2:
 *   2 < 5, shift 5 right, insert 2
 *   [2, 5] | [4, 6, 1, 3]
 *
 * Insert 4:
 *   4 < 5, shift 5 right
 *   4 > 2, insert at position 1
 *   [2, 4, 5] | [6, 1, 3]
 *
 * Insert 6:
 *   6 > 5, already in position
 *   [2, 4, 5, 6] | [1, 3]
 *
 * Insert 1:
 *   1 < 6, 1 < 5, 1 < 4, 1 < 2
 *   Shift all, insert at position 0
 *   [1, 2, 4, 5, 6] | [3]
 *
 * Insert 3:
 *   3 < 6, 3 < 5, 3 < 4, 3 > 2
 *   Insert at position 2
 *   [1, 2, 3, 4, 5, 6] SORTED!
 *
 * ============================================================================
 * TIME: O(n²) worst, O(n) best | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        // Shift elements greater than key
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        // Insert key at correct position
        arr[j + 1] = key;
    }
}

void printArray(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    printf("=== Insertion Sort ===\n\n");

    int arr[] = {5, 2, 4, 6, 1, 3};
    int n = 6;

    printf("Original: ");
    printArray(arr, n);

    insertionSort(arr, n);

    printf("Sorted:   ");
    printArray(arr, n);

    return 0;
}
