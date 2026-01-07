// Reverse an array in-place
// Time: O(n), Space: O(1)

#include <stdio.h>

// Say: "Consistent helper to print arrays for verification"
void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

void reverse_array(int arr[], int n) {
    // Say: "Two pointers - left starts at beginning, right at end"
    int left = 0;
    int right = n - 1;

    // Say: "Continue until pointers meet in the middle"
    while (left < right) {
        // Say: "Swap elements at left and right positions"
        int temp = arr[left];     // Store left element
        arr[left] = arr[right];   // Move right to left
        arr[right] = temp;        // Move stored value to right

        // Say: "Move pointers toward center"
        left++;
        right--;
    }
    // Say: "When left >= right, we've swapped all pairs"
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);  // Say: "Get array length"

    printf("Original: ");
    print_array(arr, n);

    reverse_array(arr, n);

    printf("Reversed: ");
    print_array(arr, n);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use two-pointer technique - left pointer starts at index 0,
 right pointer starts at index n-1. I swap elements at both
 positions and move pointers toward each other. When they
 meet or cross, the array is fully reversed.

 Why this approach?
 - O(n) time: we touch each element once
 - O(1) space: only using temp variable, no extra array
 - In-place: modifies original array directly"
*/
