// Binary Search - ESSENTIAL algorithm for sorted arrays
// Time: O(log n), Space: O(1)

#include <stdio.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]");
}

// Say: "Binary search halves the search space each iteration"
int binary_search(int arr[], int n, int target) {
    int left = 0;
    int right = n - 1;

    // Say: "Continue while search space is valid"
    while (left <= right) {
        // Say: "Calculate mid to avoid integer overflow"
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;  // Say: "Found at index mid"
        }
        else if (arr[mid] < target) {
            // Say: "Target is in right half, discard left"
            left = mid + 1;
        }
        else {
            // Say: "Target is in left half, discard right"
            right = mid - 1;
        }
    }

    return -1;  // Say: "Target not found"
}

// Say: "Find leftmost occurrence (for duplicates)"
int binary_search_left(int arr[], int n, int target) {
    int left = 0;
    int right = n - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            result = mid;      // Say: "Found, but keep looking left"
            right = mid - 1;   // Say: "Continue searching left half"
        }
        else if (arr[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return result;
}

// Say: "Find rightmost occurrence (for duplicates)"
int binary_search_right(int arr[], int n, int target) {
    int left = 0;
    int right = n - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            result = mid;     // Say: "Found, but keep looking right"
            left = mid + 1;   // Say: "Continue searching right half"
        }
        else if (arr[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return result;
}

int main() {
    int arr[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array: ");
    print_array(arr, n);
    printf("\n\n");

    int targets[] = {23, 2, 91, 15};
    for (int i = 0; i < 4; i++) {
        int idx = binary_search(arr, n, targets[i]);
        printf("Search %d: %s (index %d)\n",
               targets[i],
               idx != -1 ? "Found" : "Not found",
               idx);
    }

    // Test with duplicates
    printf("\n--- Search with duplicates ---\n");
    int arr2[] = {1, 2, 2, 2, 3, 4, 5};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("Array: ");
    print_array(arr2, n2);
    printf("\n");
    printf("Leftmost 2: index %d\n", binary_search_left(arr2, n2, 2));
    printf("Rightmost 2: index %d\n", binary_search_right(arr2, n2, 2));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Binary search eliminates half the search space each iteration.

 PREREQUISITE: Array must be sorted!

 ALGORITHM:
 1. Initialize left=0, right=n-1
 2. Calculate mid = left + (right - left) / 2
 3. If arr[mid] == target: found!
 4. If arr[mid] < target: search right half (left = mid + 1)
 5. If arr[mid] > target: search left half (right = mid - 1)
 6. Repeat while left <= right

 WHY left + (right - left) / 2 instead of (left + right) / 2?
 - Prevents integer overflow when left + right > INT_MAX
 - Same result mathematically
 - Always use this form in interviews!

 TIME COMPLEXITY ANALYSIS:
 - Each iteration halves search space
 - After k iterations: n / 2^k elements remain
 - Search ends when n / 2^k = 1
 - Solving: k = log2(n)
 - Time: O(log n)

 VARIATIONS:
 1. Find leftmost occurrence: keep searching left after finding
 2. Find rightmost occurrence: keep searching right after finding
 3. Lower bound: first element >= target
 4. Upper bound: first element > target

 COMMON MISTAKES:
 - Using (left + right) / 2 (overflow)
 - Using left < right instead of left <= right
 - Off-by-one errors in left/right updates
 - Forgetting array must be sorted

 EMBEDDED APPLICATIONS:
 - Lookup tables for sensor calibration
 - Flash memory page lookup
 - Configuration parameter search"
*/
