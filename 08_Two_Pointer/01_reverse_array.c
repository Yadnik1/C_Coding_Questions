/*
 * ============================================================================
 * PROBLEM: Reverse Array using Two Pointers
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: HIGH
 *
 * Pattern: OPPOSITE ENDS (Converging Pointers)
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   arr = [1, 2, 3, 4, 5]
 *          ^           ^
 *         left       right
 *
 *   Step 1: Swap arr[0] and arr[4]
 *           [5, 2, 3, 4, 1]
 *              ^     ^
 *
 *   Step 2: Swap arr[1] and arr[3]
 *           [5, 4, 3, 2, 1]
 *                 ^
 *           left == right, STOP
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>

void reverseArray(int arr[], int n) {
    int left = 0;
    int right = n - 1;

    while (left < right) {
        // Swap
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;

        left++;
        right--;
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
    printf("=== Reverse Array (Two Pointer) ===\n\n");

    int arr1[] = {1, 2, 3, 4, 5};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("Before: "); printArray(arr1, n1);
    reverseArray(arr1, n1);
    printf("After:  "); printArray(arr1, n1);
    printf("\n");

    int arr2[] = {1, 2, 3, 4, 5, 6};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("Before: "); printArray(arr2, n2);
    reverseArray(arr2, n2);
    printf("After:  "); printArray(arr2, n2);

    return 0;
}
