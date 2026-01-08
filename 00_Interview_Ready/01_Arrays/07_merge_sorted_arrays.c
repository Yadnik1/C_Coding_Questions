/*
 * ============================================================================
 * PROBLEM: Merge Two Sorted Arrays
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given two sorted arrays, merge them into a single sorted array.
 * This is the core operation in merge sort and has many real-world uses.
 *
 * EXAMPLES:
 * - Input: [1, 3, 5], [2, 4, 6]    ->  Output: [1, 2, 3, 4, 5, 6]
 * - Input: [1, 2, 3], [4, 5]       ->  Output: [1, 2, 3, 4, 5]
 * - Input: [], [1, 2]              ->  Output: [1, 2]
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Core algorithm in merge sort (divide-and-conquer)
 * - Tests understanding of linear merge technique
 * - Foundation for merging sorted database results
 * - Common in external sorting scenarios (large files)
 *
 * KEY CONCEPT:
 * Three-Pointer Merge - Use one pointer for each input array and one
 * for the result. Compare current elements, pick smaller, advance that
 * pointer. When one array is exhausted, copy remaining from the other.
 *
 * VISUAL:
 *     Array A: [1, 3, 5]      Array B: [2, 4]
 *              i                       j
 *     Result:  [ ]
 *              k
 *
 *     Step 1: Compare A[i]=1 vs B[j]=2, pick 1
 *             Result: [1], i++
 *
 *     Step 2: Compare A[i]=3 vs B[j]=2, pick 2
 *             Result: [1, 2], j++
 *
 *     Step 3: Compare A[i]=3 vs B[j]=4, pick 3
 *             Result: [1, 2, 3], i++
 *
 *     Step 4: Compare A[i]=5 vs B[j]=4, pick 4
 *             Result: [1, 2, 3, 4], j++
 *
 *     Array B exhausted, copy remaining from A:
 *             Result: [1, 2, 3, 4, 5]
 *
 * ============================================================================
 */

// Merge two sorted arrays into one sorted array
// Time: O(m+n), Space: O(m+n) - Linear merge

#include <stdio.h>

// Say: "Consistent helper to print arrays for verification"
void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

void merge_sorted(int a[], int m, int b[], int n, int result[]) {
    // Say: "Three pointers - one for each array and one for result"
    int i = 0;  // Say: "Pointer for array a"
    int j = 0;  // Say: "Pointer for array b"
    int k = 0;  // Say: "Pointer for result array"

    // Say: "Compare elements from both arrays, pick smaller one"
    while (i < m && j < n) {
        // Say: "Use <= to maintain stability (equal elements keep original order)"
        if (a[i] <= b[j]) {
            result[k] = a[i];  // Say: "Element from a is smaller or equal"
            i++;               // Say: "Move a's pointer forward"
        } else {
            result[k] = b[j];  // Say: "Element from b is smaller"
            j++;               // Say: "Move b's pointer forward"
        }
        k++;  // Say: "Always advance result pointer"
    }

    // Say: "Copy remaining elements from array a (if any)"
    while (i < m) {
        result[k] = a[i];
        i++;
        k++;
    }

    // Say: "Copy remaining elements from array b (if any)"
    while (j < n) {
        result[k] = b[j];
        j++;
        k++;
    }
    // Say: "Only one of the above two loops will execute"
}

int main() {
    int a[] = {1, 3, 5, 7};
    int b[] = {2, 4, 6, 8, 10};
    int m = sizeof(a) / sizeof(a[0]);  // Say: "Length of array a"
    int n = sizeof(b) / sizeof(b[0]);  // Say: "Length of array b"
    int result[9];  // Say: "Result array of size m + n"

    printf("Array A: ");
    print_array(a, m);
    printf("Array B: ");
    print_array(b, n);

    merge_sorted(a, m, b, n, result);

    printf("Merged:  ");
    print_array(result, m + n);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"This is the merge step from merge sort - a fundamental algorithm.

 Approach:
 - Use three pointers: i for array a, j for array b, k for result
 - Compare a[i] and b[j], pick smaller one for result[k]
 - Advance the pointer of the array we picked from
 - When one array is exhausted, copy remaining from the other

 Why does this work?
 - Both input arrays are already sorted
 - By always picking the smaller of two current elements,
   we build a sorted result

 Example walkthrough:
 a = [1, 3, 5], b = [2, 4]
 - Compare 1 vs 2: pick 1, result=[1]
 - Compare 3 vs 2: pick 2, result=[1,2]
 - Compare 3 vs 4: pick 3, result=[1,2,3]
 - Compare 5 vs 4: pick 4, result=[1,2,3,4]
 - a has 5 remaining: result=[1,2,3,4,5]

 Time: O(m+n) - each element processed once
 Space: O(m+n) - new array for result

 Real-world use:
 - Core of merge sort algorithm
 - Merging sorted database results
 - Combining sorted log files by timestamp

 In-place variant exists but is O(m*n) or complex O(m+n)"
*/
