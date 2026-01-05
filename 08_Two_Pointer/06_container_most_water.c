/*
 * ============================================================================
 * PROBLEM: Container With Most Water
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: HIGH
 *
 * Pattern: OPPOSITE ENDS (Converging Pointers)
 *
 * Given heights array, find two lines that form container with most water.
 * Area = min(height[left], height[right]) * (right - left)
 *
 * ============================================================================
 * VISUALIZATION: heights = [1, 8, 6, 2, 5, 4, 8, 3, 7]
 * ============================================================================
 *
 *       |         |
 *       |         |     |
 *       | |       |     |
 *       | |   |   |     |
 *       | |   | | |     |
 *       | |   | | | |   |
 *       | | | | | | | | |
 *   |   | | | | | | | | |
 *   1   8 6 2 5 4 8 3 7
 *   ^                   ^
 *  left               right
 *
 *   Area = min(1,7) * 8 = 8
 *
 *   Move left (smaller height) → try to find taller line
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why move the pointer with smaller height?"
 * A1: The area is limited by the shorter line. Moving the taller pointer
 *     can only decrease width while keeping the limiting height same or less.
 *     Moving the shorter pointer might find a taller one, potentially
 *     increasing area despite smaller width.
 *
 * -------------------------------------------------------------------------
 * Q2: "Prove this greedy approach doesn't miss the optimal?"
 * A2: Suppose optimal uses indices i and j. At some point, left <= i and right >= j.
 *     We won't move past both - we only move the smaller, and the optimal pair
 *     has one that's >= the other. The algorithm will find optimal or better.
 *
 * -------------------------------------------------------------------------
 * Q3: "What if heights are equal? Which pointer to move?"
 * A3: Either works! Moving either gives the same next step (both will likely
 *     need to move). Some implementations move both when equal for slight
 *     optimization, but correctness is maintained either way.
 *
 * -------------------------------------------------------------------------
 * Q4: "How is this different from trapping rainwater?"
 * A4: Container: find ONE pair of lines, water between them, nothing in between.
 *     Trapping: water fills all valleys between bars, bars block water.
 *     Trapping needs different approach (prefix max arrays or monotonic stack).
 *
 * ============================================================================
 */

#include <stdio.h>

int maxArea(int height[], int n) {
    int left = 0;
    int right = n - 1;
    int maxWater = 0;

    while (left < right) {
        // Calculate current area
        int h = height[left] < height[right] ? height[left] : height[right];
        int width = right - left;
        int area = h * width;

        if (area > maxWater) {
            maxWater = area;
        }

        // Move the pointer with smaller height
        // (moving taller one can only decrease area)
        if (height[left] < height[right]) {
            left++;
        } else {
            right--;
        }
    }

    return maxWater;
}

int main() {
    printf("=== Container With Most Water ===\n\n");

    int heights1[] = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("Heights: [1, 8, 6, 2, 5, 4, 8, 3, 7]\n");
    printf("Max water: %d\n", maxArea(heights1, n1));
    printf("(Between index 1 and 8: min(8,7) * 7 = 49)\n\n");

    int heights2[] = {1, 1};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("Heights: [1, 1]\n");
    printf("Max water: %d\n\n", maxArea(heights2, n2));

    int heights3[] = {4, 3, 2, 1, 4};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    printf("Heights: [4, 3, 2, 1, 4]\n");
    printf("Max water: %d\n", maxArea(heights3, n3));
    printf("(Between index 0 and 4: min(4,4) * 4 = 16)\n");

    return 0;
}
