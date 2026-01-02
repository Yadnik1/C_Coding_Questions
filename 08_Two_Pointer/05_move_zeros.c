/*
 * ============================================================================
 * PROBLEM: Move Zeros to End
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: HIGH
 *
 * Pattern: SAME DIRECTION (Fast & Slow)
 *
 * Move all zeros to end while maintaining order of non-zeros.
 *
 * ============================================================================
 * VISUALIZATION: [0, 1, 0, 3, 12]
 * ============================================================================
 *
 *   slow = 0 (next position for non-zero)
 *
 *   fast=0: arr[0]=0, skip
 *   fast=1: arr[1]=1 ≠ 0
 *           arr[0]=1, slow=1
 *           [1, 1, 0, 3, 12]
 *
 *   fast=2: arr[2]=0, skip
 *
 *   fast=3: arr[3]=3 ≠ 0
 *           arr[1]=3, slow=2
 *           [1, 3, 0, 3, 12]
 *
 *   fast=4: arr[4]=12 ≠ 0
 *           arr[2]=12, slow=3
 *           [1, 3, 12, 3, 12]
 *
 *   Fill zeros from slow to end:
 *   [1, 3, 12, 0, 0]
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>

void moveZeros(int arr[], int n) {
    int slow = 0;  // Next position for non-zero

    // Move all non-zeros to front
    for (int fast = 0; fast < n; fast++) {
        if (arr[fast] != 0) {
            arr[slow] = arr[fast];
            slow++;
        }
    }

    // Fill remaining positions with zeros
    while (slow < n) {
        arr[slow] = 0;
        slow++;
    }
}

// Alternative: Swap approach (slightly more operations but single pass logic)
void moveZerosSwap(int arr[], int n) {
    int slow = 0;

    for (int fast = 0; fast < n; fast++) {
        if (arr[fast] != 0) {
            // Swap arr[slow] and arr[fast]
            int temp = arr[slow];
            arr[slow] = arr[fast];
            arr[fast] = temp;
            slow++;
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
    printf("=== Move Zeros to End (Two Pointer) ===\n\n");

    int arr1[] = {0, 1, 0, 3, 12};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("Before: "); printArray(arr1, n1);
    moveZeros(arr1, n1);
    printf("After:  "); printArray(arr1, n1);
    printf("\n");

    int arr2[] = {0, 0, 1};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("Before: "); printArray(arr2, n2);
    moveZeros(arr2, n2);
    printf("After:  "); printArray(arr2, n2);
    printf("\n");

    int arr3[] = {1, 2, 3};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    printf("Before: "); printArray(arr3, n3);
    moveZeros(arr3, n3);
    printf("After:  "); printArray(arr3, n3);

    return 0;
}
