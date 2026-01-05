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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why use monotonic increasing stack?"
 * A1: When we see a shorter bar, we know the taller bars can't extend further.
 *     So we pop and calculate their max area. Stack maintains potential
 *     "still-extending" bars.
 *
 * -------------------------------------------------------------------------
 * Q2: "What's stored in the stack?"
 * A2: INDICES, not heights! We need index to calculate width.
 *     Height is retrieved via heights[stack[top]].
 *
 * -------------------------------------------------------------------------
 * Q3: "Why add height 0 at the end?"
 * A3: Forces all remaining bars to be popped and processed.
 *     Without it, bars in increasing sequence would stay in stack.
 *
 * -------------------------------------------------------------------------
 * Q4: "What's the relation to Maximal Rectangle in binary matrix?"
 * A4: Build histogram for each row (height = consecutive 1s above).
 *     Run this algorithm on each row's histogram.
 *
 * ============================================================================
 */

#include <stdio.h>

// Define maximum size
// Say: "I'll define the maximum size for arrays and stack"
#define MAX_SIZE 1000

// Find largest rectangle area using stack
// Say: "Now I'll implement the main function using a monotonic stack approach"
int largestRectangleArea(int heights[], int n) {
    // Declare stack array for indices
    // Say: "I declare a stack to store indices of histogram bars"
    int stack[MAX_SIZE];

    // Initialize top to -1
    // Say: "I initialize top to -1 for empty stack"
    int top = -1;

    // Track maximum area
    // Say: "I declare a variable to track the maximum area found"
    int maxArea = 0;

    // Index for iteration
    // Say: "I'll use index i to traverse the histogram"
    int i = 0;

    // Process all bars
    // Say: "Now I'll process each bar in the histogram"
    while (i < n) {
        // Check if we should push current index
        // Say: "If stack is empty or current height is greater or equal, I push the index"
        if (top == -1 || heights[i] >= heights[stack[top]]) {
            // Push current index
            // Say: "I increment top and push the current index"
            stack[++top] = i;

            // Move to next bar
            // Say: "Then I move to the next bar"
            i++;
        } else {
            // Pop and calculate area
            // Say: "Otherwise, current bar is smaller, so I pop and calculate area"

            // Pop the top index
            // Say: "I pop the top index from the stack"
            int heightIndex = stack[top--];

            // Get the height of popped bar
            // Say: "I get the height of the popped bar"
            int height = heights[heightIndex];

            // Calculate width
            // Say: "Now I calculate the width of the rectangle"
            int width;
            if (top == -1) {
                // Stack empty means extends to beginning
                // Say: "If stack is empty, the rectangle extends from beginning to current"
                width = i;
            } else {
                // Width between current and new top
                // Say: "Otherwise, width is from stack top to current, minus 1"
                width = i - stack[top] - 1;
            }

            // Calculate area
            // Say: "I calculate the area as height times width"
            int area = height * width;

            // Update max if needed
            // Say: "I update maxArea if this area is larger"
            if (area > maxArea) {
                maxArea = area;
            }
        }
    }

    // Process remaining elements in stack
    // Say: "After processing all bars, I need to process remaining indices in stack"
    while (top >= 0) {
        // Pop index
        // Say: "I pop the next index from stack"
        int heightIndex = stack[top--];

        // Get height
        // Say: "I get its height"
        int height = heights[heightIndex];

        // Calculate width
        // Say: "I calculate the width"
        int width;
        if (top == -1) {
            // Extends to end
            // Say: "If stack is empty, rectangle extends to the end"
            width = n;
        } else {
            // Width to end from stack top
            // Say: "Otherwise, width extends from stack top to end"
            width = n - stack[top] - 1;
        }

        // Calculate area
        // Say: "I calculate the area"
        int area = height * width;

        // Update max
        // Say: "I update maxArea if needed"
        if (area > maxArea) {
            maxArea = area;
        }
    }

    // Return maximum area found
    // Say: "Finally, I return the maximum area found"
    return maxArea;
}

// Brute force O(n²) for comparison
// Say: "Here's a brute force approach for comparison"
int largestRectangleAreaBrute(int heights[], int n) {
    // Track max area
    // Say: "I initialize maxArea to 0"
    int maxArea = 0;

    // Try all starting positions
    // Say: "I try every starting position"
    for (int i = 0; i < n; i++) {
        // Track minimum height in current range
        // Say: "I track the minimum height in the current range"
        int minHeight = heights[i];

        // Try all ending positions from i
        // Say: "I try all ending positions from current start"
        for (int j = i; j < n; j++) {
            // Update minimum height
            // Say: "I update minimum height if current bar is smaller"
            if (heights[j] < minHeight) {
                minHeight = heights[j];
            }

            // Calculate area for this range
            // Say: "I calculate area as min height times width"
            int area = minHeight * (j - i + 1);

            // Update max
            // Say: "I update maxArea if this is larger"
            if (area > maxArea) {
                maxArea = area;
            }
        }
    }

    // Return max
    // Say: "I return the maximum area"
    return maxArea;
}

// Print histogram visualization (simple version)
// Say: "This helper function prints a simple histogram"
void printHistogram(int heights[], int n) {
    // Find max height
    // Say: "First I find the maximum height"
    int maxH = 0;
    for (int i = 0; i < n; i++) {
        // Update max
        if (heights[i] > maxH) {
            maxH = heights[i];
        }
    }

    // Print histogram from top to bottom
    // Say: "I print the histogram row by row from top to bottom"
    for (int h = maxH; h >= 1; h--) {
        // Print spaces
        printf("  ");

        // Print bar if height >= h
        // Say: "For each position, I print a bar if height is at least h"
        for (int i = 0; i < n; i++) {
            if (heights[i] >= h) {
                // Print bar
                printf("██ ");
            } else {
                // Print space
                printf("   ");
            }
        }
        printf("\n");
    }

    // Print base line
    // Say: "I print the base line"
    printf("  ");
    for (int i = 0; i < n; i++) {
        printf("── ");
    }

    // Print values
    // Say: "And I print the height values below"
    printf("\n  ");
    for (int i = 0; i < n; i++) {
        printf("%d  ", heights[i]);
    }
    printf("\n");
}

int main() {
    // Say: "Let me demonstrate the largest rectangle in histogram algorithm"
    printf("=== Largest Rectangle in Histogram ===\n\n");

    // Test case 1
    // Say: "Here's the first test case"
    int heights1[] = {2, 1, 5, 6, 2, 3};
    int n1 = sizeof(heights1) / sizeof(heights1[0]);
    printf("Histogram 1:\n");
    printHistogram(heights1, n1);
    printf("\nLargest area: %d\n\n", largestRectangleArea(heights1, n1));

    // Test case 2
    // Say: "Here's the second test case"
    int heights2[] = {2, 4};
    int n2 = sizeof(heights2) / sizeof(heights2[0]);
    printf("Histogram 2:\n");
    printHistogram(heights2, n2);
    printf("\nLargest area: %d\n\n", largestRectangleArea(heights2, n2));

    // Test case 3
    // Say: "And here's the third test case"
    int heights3[] = {6, 2, 5, 4, 5, 1, 6};
    int n3 = sizeof(heights3) / sizeof(heights3[0]);
    printf("Histogram 3:\n");
    printHistogram(heights3, n3);
    printf("\nLargest area: %d\n\n", largestRectangleArea(heights3, n3));

    // Print algorithm explanation
    // Say: "Here's a summary of the monotonic stack algorithm"
    printf("=== Algorithm (Monotonic Stack) ===\n");
    printf("1. Push index if height >= stack top\n");
    printf("2. Else pop and calculate area:\n");
    printf("   - height = popped bar's height\n");
    printf("   - width = current_index - stack_top - 1\n");
    printf("3. Track maximum area\n");
    printf("\nTime: O(n), Space: O(n)\n");

    return 0;
}
