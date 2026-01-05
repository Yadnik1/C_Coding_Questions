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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "When is insertion sort better than quicksort?"
 * A1: - Small arrays (n < 10-20): Less overhead than divide-and-conquer
 *     - Nearly sorted arrays: O(n) vs O(n log n)
 *     - Online sorting: Can sort as data arrives
 *     Many libraries use insertion sort for small subarrays in quicksort!
 *
 * -------------------------------------------------------------------------
 * Q2: "Is insertion sort stable?"
 * A2: Yes! Equal elements maintain their relative order. When we shift,
 *     we only move elements greater than key, not greater-or-equal.
 *     This preserves original order of equal elements.
 *
 * -------------------------------------------------------------------------
 * Q3: "Why is it called 'insertion' sort?"
 * A3: Each element is INSERT-ed into its correct position in the already
 *     sorted portion. Like inserting a card into a sorted hand of cards.
 *     Unlike selection sort which SELECTS the minimum each time.
 *
 * -------------------------------------------------------------------------
 * Q4: "How do you sort a linked list?"
 * A4: Insertion sort works well! No random access needed. Just traverse
 *     and insert each node in correct position in sorted portion.
 *     Still O(n²) but better than array for insertions (no shifting).
 *
 * ============================================================================
 */

#include <stdio.h>

// Insertion sort algorithm
// Say: "I'll implement insertion sort which works like sorting cards in your hand"
void insertionSort(int arr[], int n) {
    // Start from second element (index 1)
    // Say: "I start from index 1 because a single element is already sorted"
    for (int i = 1; i < n; i++) {
        // Store the current element to be inserted
        // Say: "I save the current element as key to insert into sorted portion"
        int key = arr[i];
        // Start comparing from the element before current
        // Say: "I initialize j to i minus 1 to compare with previous elements"
        int j = i - 1;

        // Shift elements greater than key to the right
        // Say: "Now I shift all elements greater than key one position to the right"
        while (j >= 0 && arr[j] > key) {
            // Move current element one position right
            // Say: "I move this element one position right"
            arr[j + 1] = arr[j];
            // Move to next element on the left
            // Say: "And check the next element to the left"
            j--;
        }

        // Insert key at its correct position
        // Say: "Finally, I insert the key at its correct sorted position"
        arr[j + 1] = key;
    }
}

// Helper function to print array
void printArray(int arr[], int n) {
    // Print opening bracket
    printf("[");
    // Iterate through all elements
    for (int i = 0; i < n; i++) {
        // Print current element
        printf("%d", arr[i]);
        // Add comma separator except after last element
        if (i < n - 1) printf(", ");
    }
    // Print closing bracket and newline
    printf("]\n");
}

int main() {
    printf("=== Insertion Sort ===\n\n");

    // Initialize unsorted test array
    // Say: "I'll test with an unsorted array"
    int arr[] = {5, 2, 4, 6, 1, 3};
    // Calculate array size
    int n = 6;

    // Display original unsorted array
    printf("Original: ");
    printArray(arr, n);

    // Sort the array using insertion sort
    // Say: "I call insertion sort to sort the array in-place"
    insertionSort(arr, n);

    // Display sorted array
    printf("Sorted:   ");
    printArray(arr, n);

    return 0;
}
