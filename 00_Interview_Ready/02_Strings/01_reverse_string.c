// Reverse a string in-place using two pointers
// Time: O(n), Space: O(1)

#include <stdio.h>
#include <string.h>

void reverse_string(char *str) {
    // Say: "I'll use the classic two-pointer technique for in-place reversal"
    // Say: "One pointer starts at beginning, other at end"

    int left = 0;
    int right = strlen(str) - 1;  // Say: "strlen gives length excluding null terminator"
                                   // Say: "So for 'hello', right = 4 (index of 'o')"

    // Say: "We continue until pointers meet or cross in the middle"
    while (left < right) {
        // Say: "Standard three-step swap using temporary variable"
        // Say: "This is the most common swap pattern in embedded interviews"
        char temp = str[left];    // Say: "Save left character before overwriting"
        str[left] = str[right];   // Say: "Copy right char to left position"
        str[right] = temp;        // Say: "Place saved char in right position"

        // Say: "Move both pointers toward center"
        left++;   // Say: "Left moves right"
        right--;  // Say: "Right moves left"
    }
    // Say: "When left >= right, we've swapped all pairs - string is reversed"
    // Say: "For odd length strings, middle char stays in place automatically"
}

int main() {
    // Say: "Must use char array, not char* literal - literals are read-only"
    char str[] = "embedded";

    printf("Original: %s\n", str);
    reverse_string(str);
    printf("Reversed: %s\n", str);  // Expected: "deddebme"

    // Test with odd length
    char str2[] = "hello";
    printf("\nOriginal: %s\n", str2);
    reverse_string(str2);
    printf("Reversed: %s\n", str2);  // Expected: "olleh"

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I'll reverse the string in-place using two pointers.

 APPROACH:
 - Initialize left pointer at index 0
 - Initialize right pointer at last character (length - 1)
 - Swap characters at both pointers
 - Move pointers toward each other until they meet

 WHY TWO POINTERS:
 - Most efficient for in-place operations
 - Only need one temp variable for swapping
 - Each character is touched exactly once

 EDGE CASES I HANDLE:
 - Empty string: left=0, right=-1, loop doesn't execute
 - Single char: left=0, right=0, left < right is false
 - Odd length: middle char stays in place naturally

 COMPLEXITY:
 - Time: O(n/2) = O(n) - we process half the string
 - Space: O(1) - only one temp variable, regardless of input size

 COMMON MISTAKE TO AVOID:
 - Using char* str = 'hello' instead of char str[] = 'hello'
 - String literals are stored in read-only memory!"
*/
