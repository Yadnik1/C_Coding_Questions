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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Fill zeros vs swap approach - which is better?"
 * A1: Fill zeros does fewer writes when many zeros (writes n non-zeros + zeros).
 *     Swap does more writes but handles special values besides 0 naturally.
 *     Fill is cleaner; swap is more general.
 *
 * -------------------------------------------------------------------------
 * Q2: "Can we minimize total number of writes?"
 * A2: Yes! Only swap when fast != slow. If arr is [1,2,3,0,0], swap approach
 *     does unnecessary swaps for 1,2,3. Check if fast != slow before swapping.
 *
 * -------------------------------------------------------------------------
 * Q3: "How do you move zeros to the beginning instead?"
 * A3: Same algorithm but iterate from end! Start fast from n-1, slow from n-1,
 *     move non-zeros to the right, fill zeros on left. Or reverse, move to end,
 *     reverse again.
 *
 * -------------------------------------------------------------------------
 * Q4: "What if we need to maintain relative order of zeros too?"
 * A4: The problem asks to maintain non-zero order, zeros go to end.
 *     If zeros had different values (say 0, -0), this approach would merge them.
 *     For distinct zero types, you'd need a stable partition algorithm.
 *
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
