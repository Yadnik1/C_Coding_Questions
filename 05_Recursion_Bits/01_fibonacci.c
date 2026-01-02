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
 */

#include <stdio.h>

// Simple recursion (inefficient but conceptually clear)
int fibRecursive(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

// With memoization (efficient)
int memo[100] = {0};
int fibMemo(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (memo[n] != 0) return memo[n];

    memo[n] = fibMemo(n - 1) + fibMemo(n - 2);
    return memo[n];
}

// Iterative (most efficient)
int fibIterative(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;

    int a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main() {
    printf("=== Fibonacci ===\n\n");

    printf("First 10 Fibonacci numbers:\n");
    for (int i = 0; i < 10; i++) {
        printf("F(%d) = %d\n", i, fibIterative(i));
    }

    printf("\nComparing methods for F(10):\n");
    printf("Recursive: %d\n", fibRecursive(10));
    printf("Memoized:  %d\n", fibMemo(10));
    printf("Iterative: %d\n", fibIterative(10));

    return 0;
}
