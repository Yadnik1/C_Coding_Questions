/*
 * ============================================================================
 *          SEARCHING & SORTING - WHAT YOU REALLY NEED TO KNOW
 * ============================================================================
 *
 * FOR EMBEDDED/FIRMWARE INTERVIEWS - FOCUS ON THESE ONLY!
 *
 * ============================================================================
 *                    THE ONE SEARCH: BINARY SEARCH
 * ============================================================================
 *
 * WHY BINARY SEARCH IS THE MOST IMPORTANT:
 * ----------------------------------------
 * 1. O(log n) - extremely fast
 * 2. Asked in 90% of coding interviews
 * 3. Used everywhere in embedded:
 *    - ADC calibration lookup tables
 *    - Sensor threshold detection
 *    - Configuration parameter search
 *    - Finding values in sorted flash data
 *
 * ============================================================================
 * BINARY SEARCH PATTERN - MEMORIZE THIS:
 * ============================================================================
 *
 *   int binarySearch(int arr[], int n, int target) {
 *       int low = 0, high = n - 1;
 *
 *       while (low <= high) {
 *           int mid = low + (high - low) / 2;  // Avoid overflow!
 *
 *           if (arr[mid] == target)
 *               return mid;
 *           else if (arr[mid] < target)
 *               low = mid + 1;    // Search RIGHT half
 *           else
 *               high = mid - 1;   // Search LEFT half
 *       }
 *       return -1;  // Not found
 *   }
 *
 * ============================================================================
 * BINARY SEARCH VISUALIZATION:
 * ============================================================================
 *
 * Find 7 in [1, 3, 5, 7, 9, 11, 13]
 *
 * Round 1:
 *   [1, 3, 5, 7, 9, 11, 13]
 *    ^        ^          ^
 *   low      mid       high
 *   mid = 3, arr[3] = 7 == target → FOUND!
 *
 * Find 9:
 *
 * Round 1: mid=3, arr[3]=7 < 9 → search right, low=4
 * Round 2: mid=5, arr[5]=11 > 9 → search left, high=4
 * Round 3: mid=4, arr[4]=9 == target → FOUND!
 *
 * ============================================================================
 * HOW TO IDENTIFY BINARY SEARCH PROBLEMS:
 * ============================================================================
 *
 * KEYWORDS TO LOOK FOR:
 *   ✓ "Sorted array"
 *   ✓ "Find element"
 *   ✓ "Search in O(log n)"
 *   ✓ "Rotated sorted array"
 *   ✓ "First/last occurrence"
 *   ✓ "Peak element"
 *   ✓ "Minimum in rotated array"
 *
 * PATTERN RECOGNITION:
 *   - If array is SORTED → Think Binary Search
 *   - If asked for O(log n) → Binary Search
 *   - If "find position" in sorted data → Binary Search
 *
 * ============================================================================
 * BINARY SEARCH VARIATIONS (Know these exist):
 * ============================================================================
 *
 * 1. STANDARD: Find exact element
 *
 * 2. LOWER BOUND: First element >= target
 *    - When found, keep searching left
 *
 * 3. UPPER BOUND: First element > target
 *    - When found, keep searching left
 *
 * 4. FIRST OCCURRENCE: First position of target
 *    - When found, save result, search left
 *
 * 5. LAST OCCURRENCE: Last position of target
 *    - When found, save result, search right
 *
 * 6. ROTATED ARRAY: One half is always sorted
 *    - Check which half is sorted, decide direction
 *
 * ============================================================================
 *                  THE ONE SORT: INSERTION SORT
 * ============================================================================
 *
 * WHY INSERTION SORT IS THE BEST FOR EMBEDDED:
 * --------------------------------------------
 * 1. Simple to implement - fewer bugs
 * 2. O(n) for nearly sorted data - common in real world
 * 3. In-place - O(1) extra space
 * 4. Stable - maintains relative order
 * 5. Small data sets in embedded - O(n²) is fine
 *
 * ============================================================================
 * INSERTION SORT PATTERN - MEMORIZE THIS:
 * ============================================================================
 *
 *   void insertionSort(int arr[], int n) {
 *       for (int i = 1; i < n; i++) {
 *           int key = arr[i];
 *           int j = i - 1;
 *
 *           // Shift elements greater than key
 *           while (j >= 0 && arr[j] > key) {
 *               arr[j + 1] = arr[j];
 *               j--;
 *           }
 *
 *           arr[j + 1] = key;  // Insert key
 *       }
 *   }
 *
 * ============================================================================
 * INSERTION SORT VISUALIZATION:
 * ============================================================================
 *
 * Sort [5, 2, 4, 6, 1, 3]
 *
 * Think: Sorting cards in your hand
 *
 * [5] | 2, 4, 6, 1, 3     (5 is sorted)
 *
 * Insert 2:
 *   2 < 5, shift 5 right
 *   [2, 5] | 4, 6, 1, 3
 *
 * Insert 4:
 *   4 < 5, shift 5
 *   4 > 2, stop
 *   [2, 4, 5] | 6, 1, 3
 *
 * Insert 6:
 *   6 > 5, already in place
 *   [2, 4, 5, 6] | 1, 3
 *
 * Insert 1:
 *   1 < 6, 1 < 5, 1 < 4, 1 < 2
 *   Shift all, insert at front
 *   [1, 2, 4, 5, 6] | 3
 *
 * Insert 3:
 *   3 < 6, 3 < 5, 3 < 4, 3 > 2
 *   [1, 2, 3, 4, 5, 6] SORTED!
 *
 * ============================================================================
 * WHEN TO USE WHICH SORT:
 * ============================================================================
 *
 *   Situation                    | Best Sort
 *   -----------------------------|------------------
 *   Small array (< 50 elements)  | Insertion Sort
 *   Nearly sorted data           | Insertion Sort (O(n)!)
 *   General purpose              | Quick Sort
 *   Need stability               | Merge Sort
 *   Memory constrained           | Insertion/Heap Sort
 *   Embedded system              | Insertion Sort
 *
 * ============================================================================
 * COMPLEXITY SUMMARY:
 * ============================================================================
 *
 *   Algorithm      | Best    | Average | Worst   | Space | Stable
 *   ---------------|---------|---------|---------|-------|-------
 *   Insertion Sort | O(n)    | O(n²)   | O(n²)   | O(1)  | Yes
 *   Selection Sort | O(n²)   | O(n²)   | O(n²)   | O(1)  | No
 *   Bubble Sort    | O(n)    | O(n²)   | O(n²)   | O(1)  | Yes
 *   Quick Sort     | O(nlogn)| O(nlogn)| O(n²)   | O(logn)| No
 *   Merge Sort     | O(nlogn)| O(nlogn)| O(nlogn)| O(n)  | Yes
 *   Binary Search  | O(1)    | O(logn) | O(logn) | O(1)  | N/A
 *
 * ============================================================================
 * INTERVIEW RECOMMENDATION:
 * ============================================================================
 *
 * 1. MUST KNOW PERFECTLY:
 *    - Binary Search (and variations)
 *    - Insertion Sort
 *
 * 2. KNOW CONCEPTUALLY:
 *    - Quick Sort (partition logic)
 *    - Merge Sort (merge logic - used in merge sorted lists!)
 *
 * 3. CAN SKIP:
 *    - Heap Sort, Radix Sort, Counting Sort, Bucket Sort
 *    - These are for specialized CS interviews
 *
 * ============================================================================
 */

#include <stdio.h>

// The ONE search you need
int binarySearch(int arr[], int n, int target) {
    int low = 0, high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == target)
            return mid;
        else if (arr[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

// The ONE sort you need
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    printf("=== The ONE Search & ONE Sort You Need ===\n\n");

    // Binary Search demo
    int sorted[] = {1, 3, 5, 7, 9, 11, 13};
    printf("Binary Search in [1,3,5,7,9,11,13]:\n");
    printf("  Find 7: index %d\n", binarySearch(sorted, 7, 7));
    printf("  Find 9: index %d\n", binarySearch(sorted, 7, 9));
    printf("  Find 6: index %d\n\n", binarySearch(sorted, 7, 6));

    // Insertion Sort demo
    int unsorted[] = {5, 2, 4, 6, 1, 3};
    printf("Insertion Sort [5,2,4,6,1,3]:\n");
    printf("  Before: ");
    printArray(unsorted, 6);
    insertionSort(unsorted, 6);
    printf("  After:  ");
    printArray(unsorted, 6);

    return 0;
}
