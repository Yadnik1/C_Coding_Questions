// Check if a string is a palindrome using two pointers
// Time: O(n), Space: O(1)

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool is_palindrome(char *str) {
    // Say: "I'll use two pointers from both ends, comparing as they move inward"

    int left = 0;
    int right = strlen(str) - 1;  // Say: "Point to last character, not null terminator"

    // Say: "Compare characters until pointers meet in middle"
    while (left < right) {
        // Say: "If any pair doesn't match, it's not a palindrome"
        if (str[left] != str[right]) {
            return false;  // Say: "Early exit - no need to check rest"
        }

        // Say: "Characters matched, move both pointers toward center"
        left++;
        right--;
    }

    // Say: "All pairs matched, so it's a palindrome"
    return true;
}

// Bonus: Case-insensitive palindrome check
bool is_palindrome_ignore_case(char *str) {
    // Say: "Same approach but convert to lowercase before comparing"

    int left = 0;
    int right = strlen(str) - 1;

    while (left < right) {
        // Say: "Convert both chars to lowercase for comparison"
        // Say: "Add 32 to uppercase letter gives lowercase in ASCII"
        char l = str[left];
        char r = str[right];

        // Say: "Convert to lowercase if uppercase"
        if (l >= 'A' && l <= 'Z') l += 32;
        if (r >= 'A' && r <= 'Z') r += 32;

        if (l != r) {
            return false;
        }

        left++;
        right--;
    }

    return true;
}

int main() {
    // Test cases
    char *test1 = "radar";      // Palindrome
    char *test2 = "hello";      // Not palindrome
    char *test3 = "a";          // Single char - palindrome
    char *test4 = "";           // Empty - palindrome
    char *test5 = "RaceCar";    // Case-insensitive palindrome

    printf("'%s' is palindrome: %s\n", test1, is_palindrome(test1) ? "true" : "false");
    printf("'%s' is palindrome: %s\n", test2, is_palindrome(test2) ? "true" : "false");
    printf("'%s' is palindrome: %s\n", test3, is_palindrome(test3) ? "true" : "false");
    printf("'%s' (empty) is palindrome: %s\n", test4, is_palindrome(test4) ? "true" : "false");
    printf("'%s' is palindrome (case-insensitive): %s\n", test5,
           is_palindrome_ignore_case(test5) ? "true" : "false");

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I'll check if the string reads the same forwards and backwards
 using the two-pointer technique.

 APPROACH:
 - Start with left pointer at beginning, right at end
 - Compare characters at both pointers
 - If mismatch found, return false immediately
 - If all pairs match, return true

 WHY TWO POINTERS:
 - O(1) space - no extra string or array needed
 - O(n/2) time - we only need to check half the string
 - Early exit on first mismatch saves time

 EDGE CASES:
 - Empty string: left=0, right=-1, loop skips, returns true
 - Single char: left=0, right=0, loop skips, returns true
 - Even length 'abba': compares a-a, b-b
 - Odd length 'aba': compares a-a, b is middle (not compared)

 FOLLOW-UP VARIATIONS:
 - Case-insensitive: convert both chars to same case before comparing
 - Ignore spaces/punctuation: skip non-alphanumeric characters
 - Check if can become palindrome by removing one char

 COMPLEXITY:
 - Time: O(n) - each character checked at most once
 - Space: O(1) - only two integer pointers"
*/
