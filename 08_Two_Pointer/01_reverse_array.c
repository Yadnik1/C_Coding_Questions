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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why use two pointers instead of extra array?"
 * A1: Extra array needs O(n) space. Two pointers achieve O(1) space by swapping
 *     in-place. In embedded systems with limited memory, this is crucial!
 *
 * -------------------------------------------------------------------------
 * Q2: "Can you reverse using XOR without temp variable?"
 * A2: Yes, but not recommended! arr[left] ^= arr[right]; arr[right] ^= arr[left];
 *     arr[left] ^= arr[right]; - but fails if left==right (same location).
 *
 * -------------------------------------------------------------------------
 * Q3: "How do you reverse only part of an array?"
 * A3: Pass start and end indices instead of 0 and n-1. Same algorithm!
 *     This is used in rotate array: reverse whole, reverse first k, reverse rest.
 *
 * -------------------------------------------------------------------------
 * Q4: "What about reversing a linked list?"
 * A4: Can't use array indices! Use three pointers: prev, curr, next.
 *     Iterate and reverse each link. Still O(n) time, O(1) space.
 *
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
