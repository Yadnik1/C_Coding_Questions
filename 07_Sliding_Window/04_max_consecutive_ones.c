/*
 * ============================================================================
 * PROBLEM: Max Consecutive Ones III (with K flips)
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: HIGH
 *
 * Pattern: VARIABLE SIZE SLIDING WINDOW
 *
 * Given binary array, find longest subarray of 1s if you can flip at most K 0s.
 *
 * ============================================================================
 * VISUALIZATION: arr = [1,1,1,0,0,0,1,1,1,1,0], k = 2
 * ============================================================================
 *
 *   [1,1,1,0,0,0,1,1,1,1,0]
 *    ^       ^
 *   left   right
 *   zeros = 2 (flipped two 0s)
 *   length = 5
 *
 *   Expand right, hit third 0:
 *   zeros = 3 > k, must shrink
 *
 *   Shrink left until zeros <= k:
 *   [0,0,0,1,1,1,1,0]
 *        ^         ^
 *   zeros = 2, length = 6 ← MAX
 *
 * ============================================================================
 * KEY INSIGHT:
 * ============================================================================
 *
 *   Track count of zeros in window.
 *   If zeros > k, shrink from left.
 *   Window always contains at most k zeros (which we "flip" to 1s).
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>

int longestOnes(int arr[], int n, int k) {
    int left = 0;
    int zeros = 0;
    int maxLen = 0;

    for (int right = 0; right < n; right++) {
        // Expand: count zeros
        if (arr[right] == 0) {
            zeros++;
        }

        // Shrink if too many zeros
        while (zeros > k) {
            if (arr[left] == 0) {
                zeros--;
            }
            left++;
        }

        // Update max length
        int len = right - left + 1;
        if (len > maxLen) {
            maxLen = len;
        }
    }

    return maxLen;
}

void printArray(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(",");
    }
    printf("]");
}

int main() {
    printf("=== Max Consecutive Ones (with K flips) ===\n\n");

    int arr1[] = {1,1,1,0,0,0,1,1,1,1,0};
    int n1 = 11, k1 = 2;
    printf("Array: "); printArray(arr1, n1);
    printf(", k = %d\n", k1);
    printf("Max length: %d\n\n", longestOnes(arr1, n1, k1));

    int arr2[] = {0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1};
    int n2 = 19, k2 = 3;
    printf("Array: "); printArray(arr2, n2);
    printf(", k = %d\n", k2);
    printf("Max length: %d\n\n", longestOnes(arr2, n2, k2));

    int arr3[] = {1,1,1,1};
    int n3 = 4, k3 = 0;
    printf("Array: "); printArray(arr3, n3);
    printf(", k = %d\n", k3);
    printf("Max length: %d\n", longestOnes(arr3, n3, k3));

    return 0;
}
