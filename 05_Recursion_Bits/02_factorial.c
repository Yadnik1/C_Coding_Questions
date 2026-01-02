/*
 * ============================================================================
 * PROBLEM: Factorial Using Recursion
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * n! = n × (n-1) × (n-2) × ... × 2 × 1
 * 0! = 1, 1! = 1
 *
 * Example: 5! = 5 × 4 × 3 × 2 × 1 = 120
 *
 * ============================================================================
 * RECURSION VISUALIZATION:
 * ============================================================================
 *
 * factorial(5)
 *   = 5 * factorial(4)
 *   = 5 * (4 * factorial(3))
 *   = 5 * (4 * (3 * factorial(2)))
 *   = 5 * (4 * (3 * (2 * factorial(1))))
 *   = 5 * (4 * (3 * (2 * 1)))   ← Base case reached
 *   = 5 * (4 * (3 * 2))
 *   = 5 * (4 * 6)
 *   = 5 * 24
 *   = 120
 *
 * ============================================================================
 * CALL STACK VISUALIZATION:
 * ============================================================================
 *
 *   +-------------------+
 *   | factorial(1) = 1  | ← Base case
 *   +-------------------+
 *   | factorial(2) = 2*1|
 *   +-------------------+
 *   | factorial(3) = 3*2|
 *   +-------------------+
 *   | factorial(4) = 4*6|
 *   +-------------------+
 *   | factorial(5) = 5*24|
 *   +-------------------+
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(n) for recursion stack
 * ============================================================================
 */

#include <stdio.h>

// Recursive
int factorialRecursive(int n) {
    if (n <= 1) return 1;  // Base case
    return n * factorialRecursive(n - 1);
}

// Iterative
int factorialIterative(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Tail recursive (can be optimized by compiler)
int factorialTail(int n, int acc) {
    if (n <= 1) return acc;
    return factorialTail(n - 1, n * acc);
}

int main() {
    printf("=== Factorial ===\n\n");

    printf("Factorials 0 to 10:\n");
    for (int i = 0; i <= 10; i++) {
        printf("%d! = %d\n", i, factorialRecursive(i));
    }

    printf("\nComparing methods for 5!:\n");
    printf("Recursive: %d\n", factorialRecursive(5));
    printf("Iterative: %d\n", factorialIterative(5));
    printf("Tail rec:  %d\n", factorialTail(5, 1));

    return 0;
}
