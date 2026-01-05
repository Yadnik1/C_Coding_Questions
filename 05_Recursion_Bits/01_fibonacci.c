/*
 * ============================================================================
 * PROBLEM: Fibonacci Using Recursion
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * F(0) = 0, F(1) = 1
 * F(n) = F(n-1) + F(n-2)
 *
 * Sequence: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ...
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. RECURSION BASICS:
 *    - Base case: stops recursion
 *    - Recursive case: calls itself
 *
 * 2. SIMPLE RECURSION IS INEFFICIENT:
 *    - Recalculates same values
 *    - Time: O(2^n) - exponential!
 *
 * 3. OPTIMIZATION:
 *    - Memoization: O(n) time, O(n) space
 *    - Iterative: O(n) time, O(1) space
 *
 * ============================================================================
 * RECURSION TREE FOR F(5):
 * ============================================================================
 *
 *                    F(5)
 *                   /    \
 *                F(4)    F(3)
 *               /   \    /   \
 *            F(3)  F(2) F(2) F(1)
 *            /  \   / \   / \
 *         F(2) F(1) ... ... ...
 *         / \
 *      F(1) F(0)
 *
 * Notice: F(3) calculated twice, F(2) calculated three times!
 *
 * ============================================================================
 * TIME COMPLEXITY:
 * ============================================================================
 *
 * Simple recursion: O(2^n) - DON'T use in production!
 * With memoization: O(n)
 * Iterative: O(n)
 *
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why is simple recursion O(2^n)?"
 * A1: Each call branches into TWO more calls. F(5) calls F(4) and F(3).
 *     F(4) calls F(3) and F(2). Tree doubles at each level!
 *     Total calls ≈ 2^n. Very inefficient!
 *
 * -------------------------------------------------------------------------
 * Q2: "What's the difference between memoization and tabulation?"
 * A2: Memoization = top-down with cache (recursion + memo array).
 *     Tabulation = bottom-up iterative (build table from F(0) up).
 *     Both achieve O(n) but tabulation avoids recursion overhead.
 *
 * -------------------------------------------------------------------------
 * Q3: "Can you compute Fibonacci in O(log n)?"
 * A3: Yes! Use matrix exponentiation:
 *     [[1,1],[1,0]]^n gives [[F(n+1),F(n)],[F(n),F(n-1)]].
 *     Use fast exponentiation for O(log n). Rarely asked but impressive!
 *
 * -------------------------------------------------------------------------
 * Q4: "What about integer overflow for large n?"
 * A4: F(47) overflows 32-bit signed int. For large Fibonacci:
 *     Use long long (up to F(92)), or big integer libraries,
 *     or compute modulo some value (common in competitive programming).
 *
 * ============================================================================
 */

#include <stdio.h>

// Simple recursion (inefficient but conceptually clear)
int fibRecursive(int n) {
    // Say: "First, I'll handle the base cases"
    // Check if n is 0 or negative
    if (n <= 0) return 0;

    // Say: "F(1) equals 1 by definition"
    // Check if n is 1
    if (n == 1) return 1;

    // Say: "For any other n, I'll recursively compute F(n-1) plus F(n-2)"
    // Make recursive call: calculate F(n-1) and F(n-2), then sum them
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

// With memoization (efficient)
// Say: "I'll use a global array to cache computed values"
// Declare memoization array initialized to zeros
int memo[100] = {0};

int fibMemo(int n) {
    // Say: "First, handle the base cases just like before"
    // Check if n is 0 or negative
    if (n <= 0) return 0;

    // Check if n is 1
    if (n == 1) return 1;

    // Say: "Before computing, I'll check if we've already calculated this value"
    // Check if memo[n] has been computed (non-zero means computed)
    if (memo[n] != 0) return memo[n];

    // Say: "If not cached, I'll compute it recursively and store the result"
    // Compute F(n) recursively and store in memo array
    memo[n] = fibMemo(n - 1) + fibMemo(n - 2);

    // Return the memoized value
    return memo[n];
}

// Iterative (most efficient)
int fibIterative(int n) {
    // Say: "I'll handle the base cases first"
    // Check if n is 0 or negative
    if (n <= 0) return 0;

    // Check if n is 1
    if (n == 1) return 1;

    // Say: "I'll use two variables to track the previous two Fibonacci numbers"
    // Initialize a to F(0) = 0
    int a = 0;
    // Initialize b to F(1) = 1
    int b = 1;
    // Declare c to hold the current Fibonacci number
    int c;

    // Say: "Now I'll iterate from 2 to n, computing each Fibonacci number"
    // Loop from i = 2 to n
    for (int i = 2; i <= n; i++) {
        // Say: "The current value is the sum of the previous two"
        // Compute c = F(i) = F(i-1) + F(i-2)
        c = a + b;

        // Say: "Then I'll shift the window: a becomes b"
        // Move to next position
        a = b;

        // Say: "And b becomes our newly computed value"
        // Update b to current value
        b = c;
    }

    // Say: "After the loop, b contains F(n)"
    // Return the final result
    return b;
}

int main() {
    printf("=== Fibonacci ===\n\n");

    printf("First 10 Fibonacci numbers:\n");
    // Loop through first 10 numbers
    for (int i = 0; i < 10; i++) {
        // Print F(i) using iterative method
        printf("F(%d) = %d\n", i, fibIterative(i));
    }

    printf("\nComparing methods for F(10):\n");
    // Compute and display F(10) using all three methods
    printf("Recursive: %d\n", fibRecursive(10));
    printf("Memoized:  %d\n", fibMemo(10));
    printf("Iterative: %d\n", fibIterative(10));

    return 0;
}
