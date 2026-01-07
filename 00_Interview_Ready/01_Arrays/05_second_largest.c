// Find second largest element in array
// Time: O(n), Space: O(1) - Single pass

#include <stdio.h>
#include <limits.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

int second_largest(int arr[], int n) {
    // Say: "Track two values - largest and second largest"
    int first = INT_MIN;   // Say: "Start with smallest possible value"
    int second = INT_MIN;

    // Say: "Single pass through array"
    for (int i = 0; i < n; i++) {
        if (arr[i] > first) {
            // Say: "New largest found - old largest becomes second"
            second = first;
            first = arr[i];
        } else if (arr[i] > second && arr[i] != first) {
            // Say: "New second largest found (not equal to first)"
            second = arr[i];
        }
    }

    return second;  // Say: "Returns INT_MIN if no second largest exists"
}

int main() {
    int arr[] = {12, 35, 1, 10, 34, 1};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array: ");
    print_array(arr, n);

    printf("Second largest: %d\n", second_largest(arr, n));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I track first and second largest in a single pass.

 For each element:
 1. If greater than first: update second=first, first=current
 2. Else if greater than second (and not equal to first): update second

 Why check arr[i] != first?
 - Handles duplicates like [5,5,3] - second should be 3, not 5

 Time: O(n) - single pass
 Space: O(1) - only two variables

 Edge cases to mention:
 - Array with all same elements: returns INT_MIN
 - Array with size 1: returns INT_MIN"
*/
