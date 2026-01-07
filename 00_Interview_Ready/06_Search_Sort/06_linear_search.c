// Linear Search - Basic search for unsorted arrays
// Time: O(n), Space: O(1)

#include <stdio.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]");
}

// Say: "Linear search checks every element until found"
int linear_search(int arr[], int n, int target) {
    // Say: "Scan array from start to end"
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;  // Say: "Found at index i"
        }
    }
    return -1;  // Say: "Not found"
}

// Say: "Find all occurrences of target"
int find_all(int arr[], int n, int target, int results[]) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            results[count++] = i;
        }
    }
    return count;  // Say: "Return number of occurrences"
}

// Say: "Sentinel search - slight optimization"
// Say: "Avoids bounds check in each iteration"
int sentinel_search(int arr[], int n, int target) {
    // Say: "Save last element and replace with target"
    int last = arr[n - 1];
    arr[n - 1] = target;  // Say: "This is our sentinel"

    int i = 0;
    // Say: "No need to check i < n, sentinel guarantees termination"
    while (arr[i] != target) {
        i++;
    }

    // Say: "Restore last element"
    arr[n - 1] = last;

    // Say: "Check if we found target or just hit sentinel"
    if (i < n - 1 || arr[n - 1] == target) {
        return i;
    }
    return -1;
}

int main() {
    int arr[] = {10, 25, 30, 15, 25, 40, 25, 50};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array: ");
    print_array(arr, n);
    printf("\n\n");

    // Basic search
    int targets[] = {25, 10, 50, 100};
    for (int i = 0; i < 4; i++) {
        int idx = linear_search(arr, n, targets[i]);
        printf("Search %d: %s (index %d)\n",
               targets[i],
               idx != -1 ? "Found" : "Not found",
               idx);
    }

    // Find all occurrences
    printf("\nFind all occurrences of 25:\n");
    int results[n];
    int count = find_all(arr, n, 25, results);
    printf("  Found %d times at indices: ", count);
    for (int i = 0; i < count; i++) {
        printf("%d ", results[i]);
    }
    printf("\n");

    // Sentinel search demo
    printf("\nSentinel search for 40: index %d\n",
           sentinel_search(arr, n, 40));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Linear search checks each element sequentially until target is found.

 ALGORITHM:
 1. Start from first element
 2. Compare each element with target
 3. If match found, return index
 4. If end reached, return -1 (not found)

 COMPLEXITY:
 - Time: O(n) - must check up to n elements
 - Best case: O(1) - target is first element
 - Average case: O(n/2) = O(n)
 - Worst case: O(n) - target at end or not present
 - Space: O(1)

 WHEN TO USE:
 1. Array is unsorted
 2. Array is small
 3. Searching once (not worth sorting)
 4. Linked lists (no random access)
 5. Finding multiple occurrences

 LINEAR vs BINARY SEARCH:
 | Aspect        | Linear    | Binary    |
 |---------------|-----------|-----------|
 | Time          | O(n)      | O(log n)  |
 | Sorted needed | No        | Yes       |
 | Data access   | Sequential| Random    |
 | Linked lists  | Works     | Inefficient|

 SENTINEL SEARCH OPTIMIZATION:
 - Places target at end of array
 - Eliminates bounds check (i < n) in loop
 - Small constant factor improvement
 - Array must be modifiable

 VARIATIONS:
 - Find first occurrence
 - Find last occurrence
 - Find all occurrences
 - Find minimum/maximum
 - Count occurrences

 EMBEDDED APPLICATIONS:
 - Lookup in small configuration tables
 - Search in unsorted sensor readings
 - Finding device in device list
 - Simple hash collision resolution

 COMMON MISTAKE:
 - Using linear search when binary search is possible
 - Not considering whether data is sorted"
*/
