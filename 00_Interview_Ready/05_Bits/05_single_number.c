// Find single number (all others appear twice)
// Time: O(n), Space: O(1)

#include <stdio.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

int find_single(int arr[], int n) {
    // Say: "XOR all elements - duplicates cancel out"
    // Say: "Key property: A ^ A = 0 and A ^ 0 = A"

    int result = 0;

    for (int i = 0; i < n; i++) {
        result ^= arr[i];  // Say: "XOR with each element"
    }

    // Say: "Only the single number remains"
    return result;
}

int main() {
    int arr[] = {4, 1, 2, 1, 2};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array: ");
    print_array(arr, n);

    printf("Single number: %d\n", find_single(arr, n));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I XOR all elements together.

 Why XOR works:
 - A ^ A = 0 (same numbers cancel)
 - A ^ 0 = A (XOR with 0 gives original)
 - XOR is commutative: A ^ B = B ^ A
 - XOR is associative: (A ^ B) ^ C = A ^ (B ^ C)

 Example: [4, 1, 2, 1, 2]
 - 4 ^ 1 ^ 2 ^ 1 ^ 2
 - = 4 ^ (1 ^ 1) ^ (2 ^ 2)  (reorder by associativity)
 - = 4 ^ 0 ^ 0
 - = 4 ✓

 Time: O(n) - single pass
 Space: O(1) - single variable

 This is THE classic bit manipulation interview question!"
*/
