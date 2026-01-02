/*
 * ============================================================================
 * PROBLEM: Largest Rectangle in Histogram
 * ============================================================================
 *
 * DIFFICULTY: Hard | TIME: 20 mins | FREQUENCY: MEDIUM
 *
 * Find the largest rectangular area in a histogram.
 *
 * ============================================================================
 * APPROACH: Monotonic Stack
 * ============================================================================
 *
 * For each bar, find:
 * - Left boundary: First smaller bar on left
 * - Right boundary: First smaller bar on right
 * - Width = right - left - 1
 * - Area = height * width
 *
 * Use stack to efficiently find boundaries in O(n).
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   heights = [2, 1, 5, 6, 2, 3]
 *
 *         _
 *        | |
 *      _ | |
 *     | || |   _
 *   _ | || | _| |
 *  | || || || || |
 *  |_||_||_||_||_|
 *   2  1  5  6  2  3
 *
 *   Largest rectangle: height=5, width=2 → area=10
 *   (using bars at index 2 and 3)
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 * 1. Push indices to stack if height >= stack top
 * 2. When height < stack top, pop and calculate area
 * 3. Width = current_index - stack_top - 1 (or current_index if stack empty)
 * 4. Process remaining elements in stack at end
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(n)
 * ============================================================================
 */

#include <stdio.h>

#define MAX_SIZE 1000

int largestRectangleArea(int heights[], int n) {
    int stack[MAX_SIZE];
    int top = -1;
    int maxArea = 0;

    int i = 0;
    while (i < n) {
        // Push if stack empty or current height >= stack top
        if (top == -1 || heights[i] >= heights[stack[top]]) {
            stack[++top] = i;
            i++;
        } else {
            // Pop and calculate area
            int heightIndex = stack[top--];
            int height = heights[heightIndex];

            // Width: from stack top to current index
            int width;
            if (top == -1) {
                width = i;  // Extends to beginning
            } else {
                width = i - stack[top] - 1;
            }

            int area = height * width;
            if (area > maxArea) {
                maxArea = area;
            }
        }
    }

    // Process remaining elements in stack
    while (top >= 0) {
        int heightIndex = stack[top--];
        int height = heights[heightIndex];

        int width;
        if (top == -1) {
            width = n;
        } else {
            width = n - stack[top] - 1;
        }

        int area = height * width;
        if (area > maxArea) {
            maxArea = area;
        }
    }

    return maxArea;
}

// Brute force O(n²) for comparison
int largestRectangleAreaBrute(int heights[], int n) {
    int maxArea = 0;

    for (int i = 0; i < n; i++) {
        int minHeight = heights[i];

        for (int j = i; j < n; j++) {
            if (heights[j] < minHeight) {
                minHeight = heights[j];
            }

            int area = minHeight * (j - i + 1);
            if (area > maxArea) {
                maxArea = area;
            }
        }
    }

    return maxArea;
}

void printHistogram(int heights[], int n) {
    // Find max height
    int maxH = 0;
    for (int i = 0; i < n; i++) {
        if (heights[i] > maxH) maxH = heights[i];
    }

    // Print histogram
    for (int h = maxH; h >= 1; h--) {
        printf("  ");
        for (int i = 0; i < n; i++) {
            if (heights[i] >= h) {
                printf("██ ");
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }

    // Print base
    printf("  ");
    for (int i = 0; i < n; i++) {
        printf("── ");
    }
    printf("\n  ");
    for (int i = 0; i < n; i++) {
        printf("%d  ", heights[i]);
    }
    printf("\n");
}

int main() {
    printf("=== Largest Rectangle in Histogram ===\n\n");

    int heights1[] = {2, 1, 5, 6, 2, 3};
    int n1 = 6;
    printf("Histogram 1:\n");
    printHistogram(heights1, n1);
    printf("\nLargest area: %d\n\n", largestRectangleArea(heights1, n1));

    int heights2[] = {2, 4};
    int n2 = 2;
    printf("Histogram 2:\n");
    printHistogram(heights2, n2);
    printf("\nLargest area: %d\n\n", largestRectangleArea(heights2, n2));

    int heights3[] = {6, 2, 5, 4, 5, 1, 6};
    int n3 = 7;
    printf("Histogram 3:\n");
    printHistogram(heights3, n3);
    printf("\nLargest area: %d\n\n", largestRectangleArea(heights3, n3));

    printf("=== Algorithm (Monotonic Stack) ===\n");
    printf("1. Push index if height >= stack top\n");
    printf("2. Else pop and calculate area:\n");
    printf("   - height = popped bar's height\n");
    printf("   - width = current_index - stack_top - 1\n");
    printf("3. Track maximum area\n");
    printf("\nTime: O(n), Space: O(n)\n");

    return 0;
}
