# C Coding Questions for Embedded/Firmware Interviews

A comprehensive collection of **76 C programming problems** designed for embedded systems and firmware engineering interviews. Each solution includes detailed explanations, ASCII visualizations, time/space complexity analysis, and working code.

## 📁 Repository Structure

```
C_Coding_Questions/
├── 01_Arrays/              # 11 problems
├── 02_Strings/             # 10 problems
├── 03_Linked_List/         # 8 problems
├── 04_Stack_Queue/         # 9 problems (includes theory)
├── 05_Recursion_Bits/      # 6 problems
├── 06_Searching_Sorting/   # 6 problems (Binary Search + Insertion Sort)
├── 07_Sliding_Window/      # 2 problems
├── 08_Two_Pointer/         # 6 problems
├── 09_Embedded_Specific/   # 7 problems
└── 10_String_Library/      # 11 problems
```

## 🎯 Topics Covered

### Core Data Structures
| Category | Key Problems |
|----------|-------------|
| **Arrays** | Reverse, Rotate, Find duplicate, Circular buffer |
| **Strings** | Palindrome, Anagram, First non-repeating char |
| **Linked Lists** | Reverse, Detect loop (Floyd's), Find middle |
| **Stack/Queue** | Balanced parentheses, Min stack, Queue using stacks |

### Algorithms & Patterns
| Category | Key Problems |
|----------|-------------|
| **Searching** | Binary search, Search in rotated array, Peak element |
| **Sorting** | Insertion sort (best for embedded - O(1) space) |
| **Two Pointer** | Two sum, Remove duplicates, Container with most water |
| **Sliding Window** | Max sum subarray, Max consecutive ones |
| **Bit Manipulation** | Count set bits, Power of 2, Swap without temp |

### Embedded-Specific
| Topic | Files |
|-------|-------|
| **Circular Buffer** | Ring buffer with 3 implementations |
| **State Machines** | Switch-case & table-driven approaches |
| **Protocol Parser** | UART command parsing state machine |
| **Endianness** | Detection, byte swapping, htons/htonl |
| **String Library** | strlen, strcpy, memcpy, memmove, atoi, itoa |

## ⭐ High-Priority Problems for Interviews

If you're short on time, focus on these:

```
✓ Circular Buffer          (01_Arrays/11_circular_buffer.c)
✓ Reverse Linked List       (03_Linked_List/01_reverse_linked_list.c)
✓ Detect Loop (Floyd's)     (03_Linked_List/02_detect_loop.c)
✓ Binary Search             (06_Searching_Sorting/01_binary_search.c)
✓ State Machine             (09_Embedded_Specific/01_state_machine_switch.c)
✓ Endianness & Byte Swap    (09_Embedded_Specific/05_byte_swap.c)
✓ memcpy / memmove          (10_String_Library/06_memcpy.c, 07_memmove.c)
✓ atoi                      (10_String_Library/10_atoi.c)
```

## 🔧 How to Use

Each file is standalone and can be compiled directly:

```bash
gcc 01_Arrays/11_circular_buffer.c -o circular_buffer
./circular_buffer
```

Every file includes:
- Problem description
- Visual walkthrough (ASCII diagrams)
- Time & space complexity
- Working `main()` function with test cases
- Edge cases and common interview traps

## 📊 Complexity Quick Reference

| Algorithm | Time | Space | Notes |
|-----------|------|-------|-------|
| Binary Search | O(log n) | O(1) | THE search algorithm |
| Insertion Sort | O(n²) | O(1) | Best for embedded (in-place) |
| Floyd's Cycle | O(n) | O(1) | Slow/fast pointer |
| Sliding Window | O(n) | O(1) | Fixed or variable size |
| Two Pointer | O(n) | O(1) | Opposite ends or same direction |

## 🎓 Interview Tips

1. **Always check for NULL pointers** - Embedded systems crash hard
2. **Know your edge cases** - Empty arrays, single element, overflow
3. **Explain as you code** - Interviewers want to see your thought process
4. **Practice on paper** - Many embedded interviews are whiteboard-style
5. **Understand memory** - Stack vs heap, endianness, alignment

---

## 💬 Common Interview Questions by Topic

### Linked Lists

#### Reverse Linked List
| Question | Key Points |
|----------|------------|
| Why 3 pointers (prev, curr, next)? | prev=where to point, curr=processing, next=save before breaking link |
| Iterative vs Recursive? | Iterative preferred: O(1) space, no stack overflow risk |
| Empty list/single node? | Works naturally - no special handling needed |

#### Detect Loop (Floyd's Algorithm)
| Question | Key Points |
|----------|------------|
| Why fast moves 2 steps? | Optimal - minimizes iterations, guaranteed to meet in one loop |
| Would 3 steps work? | Yes, but might "jump over" slow in small loops |
| Find loop START? | Reset slow to head, move both by 1 - they meet at start! |
| Hash set alternative? | O(n) space vs Floyd's O(1) - Floyd's better for embedded |
| Remove the loop? | Find node before loop start, set its next to NULL |

#### Find Middle
| Question | Key Points |
|----------|------------|
| Why not count length first? | Slow/fast is single pass (0.5n) vs two pass (1.5n) |
| First vs second middle? | Start fast at head->next for first middle |
| Find 1/3 point? | Fast moves 3 steps, slow moves 1 |

### Arrays & Strings

#### Circular Buffer
| Question | Key Points |
|----------|------------|
| How to detect full vs empty? | Keep count, or waste one slot as sentinel |
| Why modulo operation? | Enables wrap-around: next = (pos + 1) % size |
| Thread safety? | Need mutex/disable interrupts for ISR access |

#### Binary Search
| Question | Key Points |
|----------|------------|
| Why mid = low + (high-low)/2? | Prevents integer overflow vs (low+high)/2 |
| Off-by-one errors? | Check loop condition and boundary updates carefully |
| Iterative vs recursive? | Iterative preferred for embedded (no stack growth) |

### Bit Manipulation

| Question | Key Points |
|----------|------------|
| Check if power of 2? | n & (n-1) == 0 (only one bit set) |
| Count set bits? | Brian Kernighan: n &= (n-1) clears lowest set bit |
| Swap without temp? | a ^= b; b ^= a; a ^= b; (XOR trick) |
| Check even/odd? | n & 1 == 0 (even), n & 1 == 1 (odd) |

### Embedded Specific

#### Endianness
| Question | Key Points |
|----------|------------|
| How to detect at runtime? | Cast int to char*, check first byte |
| Why does it matter? | Network protocols are big-endian, x86 is little-endian |
| Convert between? | Byte swap: ((x>>24) \| ((x>>8)&0xFF00) \| ((x<<8)&0xFF0000) \| (x<<24)) |

#### State Machines
| Question | Key Points |
|----------|------------|
| Switch-case vs table-driven? | Switch for <5 states, table for scalability |
| How to handle invalid input? | Default case, error state, or ignore |

---

## 🔑 Key Patterns to Know

```c
// 1. SLOW/FAST POINTERS (Floyd's)
while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
}

// 2. THREE POINTER REVERSAL
while (curr) {
    next = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next;
}

// 3. CIRCULAR BUFFER
next = (current + 1) % size;

// 4. BIT MANIPULATION
set:    num |= (1 << n)
clear:  num &= ~(1 << n)
toggle: num ^= (1 << n)
check:  (num >> n) & 1
```

---

## 📝 License

Free to use for interview preparation. Good luck! 🚀

---

*Generated for embedded/firmware interview preparation*
