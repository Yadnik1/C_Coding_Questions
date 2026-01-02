/*
 * ============================================================================
 * PROBLEM: Dutch National Flag (Sort 0, 1, 2)
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: VERY HIGH
 *
 * Given array with only 0s, 1s, and 2s, sort in single pass without extra space.
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   Array: [2, 0, 1, 2, 1, 0]
 *
 *   Three pointers:
 *   - low:  next position for 0
 *   - mid:  current element
 *   - high: next position for 2
 *
 *   [2, 0, 1, 2, 1, 0]
 *    ^              ^
 *   low            high
 *   mid
 *
 *   mid=0: arr[0]=2, swap with high
 *   [0, 0, 1, 2, 1, 2]
 *    ^           ^
 *   low        high
 *   mid
 *
 *   Continue until mid > high...
 *
 *   Final: [0, 0, 1, 1, 2, 2]
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   - If arr[mid] == 0: swap(arr[low], arr[mid]), low++, mid++
 *   - If arr[mid] == 1: mid++ (already in place)
 *   - If arr[mid] == 2: swap(arr[mid], arr[high]), high--
 *     (don't increment mid, need to check swapped element)
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void dutchNationalFlag(int arr[], int n) {
    int low = 0;
    int mid = 0;
    int high = n - 1;

    while (mid <= high) {
        if (arr[mid] == 0) {
            swap(&arr[low], &arr[mid]);
            low++;
            mid++;
        } else if (arr[mid] == 1) {
            mid++;
        } else {  // arr[mid] == 2
            swap(&arr[mid], &arr[high]);
            high--;
            // Don't increment mid - need to check swapped element
        }
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
    printf("=== Dutch National Flag (Sort 0, 1, 2) ===\n\n");

    int arr1[] = {2, 0, 1, 2, 1, 0};
    int n1 = 6;
    printf("Before: "); printArray(arr1, n1);
    dutchNationalFlag(arr1, n1);
    printf("After:  "); printArray(arr1, n1);
    printf("\n");

    int arr2[] = {0, 1, 2, 0, 1, 2};
    int n2 = 6;
    printf("Before: "); printArray(arr2, n2);
    dutchNationalFlag(arr2, n2);
    printf("After:  "); printArray(arr2, n2);
    printf("\n");

    int arr3[] = {2, 2, 1, 1, 0, 0};
    int n3 = 6;
    printf("Before: "); printArray(arr3, n3);
    dutchNationalFlag(arr3, n3);
    printf("After:  "); printArray(arr3, n3);

    return 0;
}
