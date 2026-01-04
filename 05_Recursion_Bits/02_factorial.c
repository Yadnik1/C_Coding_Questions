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
    // Say: "First, I'll check the base case"
    // Check if n is 0 or 1 (both return 1)
    if (n <= 1) return 1;  // Base case

    // Say: "Otherwise, I'll return n times the factorial of n minus 1"
    // Recursive case: multiply n by factorial(n-1)
    return n * factorialRecursive(n - 1);
}

// Iterative
int factorialIterative(int n) {
    // Say: "I'll initialize the result to 1"
    // Start with result = 1 (since 0! = 1)
    int result = 1;

    // Say: "Then I'll multiply result by each number from 2 to n"
    // Loop from i = 2 up to n
    for (int i = 2; i <= n; i++) {
        // Multiply result by current number i
        result *= i;
    }

    // Say: "Finally, I'll return the accumulated product"
    // Return the final factorial value
    return result;
}

// Tail recursive (can be optimized by compiler)
int factorialTail(int n, int acc) {
    // Say: "In tail recursion, I'll pass the accumulator through each call"
    // Check base case: if n is 0 or 1
    if (n <= 1) return acc;

    // Say: "I'll make a tail-recursive call with n-1 and accumulator times n"
    // Tail recursive call: move n into accumulator
    return factorialTail(n - 1, n * acc);
}

int main() {
    printf("=== Factorial ===\n\n");

    printf("Factorials 0 to 10:\n");
    // Loop from 0 to 10
    for (int i = 0; i <= 10; i++) {
        // Print i! using recursive method
        printf("%d! = %d\n", i, factorialRecursive(i));
    }

    printf("\nComparing methods for 5!:\n");
    // Compute and display 5! using all three methods
    printf("Recursive: %d\n", factorialRecursive(5));
    printf("Iterative: %d\n", factorialIterative(5));
    // Note: tail recursive needs initial accumulator of 1
    printf("Tail rec:  %d\n", factorialTail(5, 1));

    return 0;
}
