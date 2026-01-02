/*
 * ============================================================================
 * PROBLEM: Find Pair with Given Sum
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH
 *
 * Two versions:
 * 1. Sorted array - Two pointer O(n)
 * 2. Unsorted array - Hash/Brute force O(n²) or O(n) with hash
 *
 * ============================================================================
 * VISUALIZATION (Sorted - Two Pointer):
 * ============================================================================
 *
 *   arr = [1, 2, 4, 6, 8, 10], target = 10
 *          ^              ^
 *         left          right
 *
 *   sum = 1 + 10 = 11 > 10 → right--
 *   sum = 1 + 8 = 9 < 10  → left++
 *   sum = 2 + 8 = 10 == target → FOUND!
 *
 * ============================================================================
 * TIME: O(n) sorted, O(n²) unsorted brute | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Sorted array - Two pointer approach
bool pairSumSorted(int arr[], int n, int target, int* idx1, int* idx2) {
    int left = 0;
    int right = n - 1;

    while (left < right) {
        int sum = arr[left] + arr[right];

        if (sum == target) {
            *idx1 = left;
            *idx2 = right;
            return true;
        } else if (sum < target) {
            left++;
        } else {
            right--;
        }
    }

    return false;
}

// Unsorted array - Brute force O(n²)
bool pairSumUnsorted(int arr[], int n, int target, int* idx1, int* idx2) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] + arr[j] == target) {
                *idx1 = i;
                *idx2 = j;
                return true;
            }
        }
    }
    return false;
}

// Unsorted array - Hash approach O(n)
// Using simple hash for small positive numbers
#define HASH_SIZE 10000

bool pairSumHash(int arr[], int n, int target, int* idx1, int* idx2) {
    int hash[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; i++) {
        hash[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        int complement = target - arr[i];

        if (complement >= 0 && complement < HASH_SIZE && hash[complement] != -1) {
            *idx1 = hash[complement];
            *idx2 = i;
            return true;
        }

        if (arr[i] >= 0 && arr[i] < HASH_SIZE) {
            hash[arr[i]] = i;
        }
    }

    return false;
}

int main() {
    printf("=== Find Pair with Given Sum ===\n\n");

    // Sorted array
    printf("1. Sorted Array (Two Pointer):\n");
    int sorted[] = {1, 2, 4, 6, 8, 10};
    int n1 = 6, target1 = 10;
    int idx1, idx2;

    printf("   Array: [1, 2, 4, 6, 8, 10], Target: %d\n", target1);
    if (pairSumSorted(sorted, n1, target1, &idx1, &idx2)) {
        printf("   Found: arr[%d] + arr[%d] = %d + %d = %d\n\n",
               idx1, idx2, sorted[idx1], sorted[idx2], target1);
    }

    // Unsorted array - Brute force
    printf("2. Unsorted Array (Brute Force):\n");
    int unsorted[] = {8, 3, 5, 2, 9, 1};
    int n2 = 6, target2 = 7;

    printf("   Array: [8, 3, 5, 2, 9, 1], Target: %d\n", target2);
    if (pairSumUnsorted(unsorted, n2, target2, &idx1, &idx2)) {
        printf("   Found: arr[%d] + arr[%d] = %d + %d = %d\n\n",
               idx1, idx2, unsorted[idx1], unsorted[idx2], target2);
    }

    // Unsorted array - Hash
    printf("3. Unsorted Array (Hash O(n)):\n");
    printf("   Array: [8, 3, 5, 2, 9, 1], Target: %d\n", target2);
    if (pairSumHash(unsorted, n2, target2, &idx1, &idx2)) {
        printf("   Found: arr[%d] + arr[%d] = %d + %d = %d\n\n",
               idx1, idx2, unsorted[idx1], unsorted[idx2], target2);
    }

    printf("=== Summary ===\n");
    printf("Sorted:   Two pointer - O(n) time, O(1) space\n");
    printf("Unsorted: Brute force - O(n²) time, O(1) space\n");
    printf("Unsorted: Hash - O(n) time, O(n) space\n");

    return 0;
}
