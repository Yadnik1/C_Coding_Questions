# Interview Ready - Single Best Solutions

**One solution per problem. No alternatives. Just the most efficient embedded-friendly approach.**

## Key Features

- **One solution per problem** - No confusion about which method to use
- **"Say:" comments** - Tells you exactly what to say when explaining each line
- **Consistent helper functions** - Same `print_array()`, `print_binary()`, `print_list()` across all files
- **Interview explanations** - Detailed `INTERVIEW EXPLANATION` block at the end of each file

## Format
Each file has:
- Problem statement with time/space complexity
- Single best solution with "Say:" comments
- Simple `main()` with test cases
- `INTERVIEW EXPLANATION` block for verbal explanation

## Compile & Run
```bash
gcc filename.c -o out && ./out
```

## Categories (64 Total Problems)

| Folder | Problems | Focus |
|--------|----------|-------|
| 01_Arrays | 12 | Two pointers, Floyd's cycle, Boyer-Moore, Kadane's |
| 02_Strings | 12 | Reverse, palindrome, anagram, strlen/strcpy/atoi, memcpy/memmove, itoa |
| 03_Linked_List | 10 | Reverse, cycle detection, merge, palindrome |
| 04_Stack_Queue | 6 | Stack/queue impl, balanced parentheses, min stack |
| 05_Bits | 10 | Count bits, power of 2, endianness, bitfields |
| 06_Search_Sort | 6 | Binary search, bubble/insertion/quick/merge sort |
| 07_Embedded | 8 | Ring buffer, state machine, debounce, CRC, watchdog |

## Study Order
1. Bits (fundamental for embedded)
2. Arrays (most common)
3. Strings (pointer practice)
4. Linked List (pointer mastery)
5. Stack/Queue (data structure)
6. Search/Sort (algorithms)
7. Embedded (domain specific)

## Example "Say:" Comment Pattern

```c
void reverse_array(int arr[], int n) {
    int left = 0;           // Say: "Start pointer at beginning"
    int right = n - 1;      // Say: "End pointer at last element"

    while (left < right) {  // Say: "Continue until pointers meet"
        // Say: "Swap elements using temp variable"
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;             // Say: "Move left pointer right"
        right--;            // Say: "Move right pointer left"
    }
}
```

## Quick Reference - Time Complexities

| Problem Type | Best Algorithm | Time | Space |
|--------------|----------------|------|-------|
| Array reversal | Two pointers | O(n) | O(1) |
| Find duplicate | Floyd's cycle | O(n) | O(1) |
| Count set bits | Brian Kernighan | O(k) | O(1) |
| Binary search | Iterative | O(log n) | O(1) |
| Quick sort | Lomuto partition | O(n log n) | O(log n) |
| Linked list cycle | Tortoise & Hare | O(n) | O(1) |
