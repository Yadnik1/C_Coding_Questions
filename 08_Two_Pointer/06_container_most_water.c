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
    int n1 = 9;
    printf("Heights: [1, 8, 6, 2, 5, 4, 8, 3, 7]\n");
    printf("Max water: %d\n", maxArea(heights1, n1));
    printf("(Between index 1 and 8: min(8,7) * 7 = 49)\n\n");

    int heights2[] = {1, 1};
    int n2 = 2;
    printf("Heights: [1, 1]\n");
    printf("Max water: %d\n\n", maxArea(heights2, n2));

    int heights3[] = {4, 3, 2, 1, 4};
    int n3 = 5;
    printf("Heights: [4, 3, 2, 1, 4]\n");
    printf("Max water: %d\n", maxArea(heights3, n3));
    printf("(Between index 0 and 4: min(4,4) * 4 = 16)\n");

    return 0;
}
