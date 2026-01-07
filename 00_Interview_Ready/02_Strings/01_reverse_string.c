// Reverse a string in-place
// Time: O(n), Space: O(1)

#include <stdio.h>
#include <string.h>

void reverse_string(char *str) {
    // Say: "I use two pointers starting at both ends"
    int left = 0;
    int right = strlen(str) - 1;  // Say: "right starts at last char, not null terminator"

    // Say: "I swap characters moving inward until they meet"
    while (left < right) {
        // Say: "Standard swap using temp variable"
        char temp = str[left];    // Save left char
        str[left] = str[right];   // Move right char to left
        str[right] = temp;        // Move saved char to right

        left++;   // Say: "Move left pointer right"
        right--;  // Say: "Move right pointer left"
    }
    // Say: "When pointers meet or cross, string is reversed"
}

int main() {
    char str[] = "embedded";  // Say: "Need char array, not char* for in-place modify"

    printf("Original: %s\n", str);
    reverse_string(str);
    printf("Reversed: %s\n", str);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I'll use two pointers - one at the start, one at the end.
 I swap characters and move both pointers toward the center.
 When they meet, the string is fully reversed.
 Time is O(n/2) which simplifies to O(n).
 Space is O(1) since I only use one temp variable."
*/
