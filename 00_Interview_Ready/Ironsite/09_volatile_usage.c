/*
 * ============================================================================
 * PROBLEM: Volatile Keyword Usage
 * ============================================================================
 *
 * WHAT IS THIS?
 * The 'volatile' keyword tells the compiler that a variable's value can change
 * at any time without any action from the code the compiler sees. This prevents
 * the compiler from optimizing away reads/writes, ensuring every access goes
 * to actual memory rather than a cached register value.
 *
 * WHY IS THIS CRITICAL FOR EMBEDDED SYSTEMS?
 * - Hardware Registers: Peripheral status registers change by hardware
 * - ISR Communication: Flags set in ISR must be visible to main loop
 * - DMA Buffers: DMA controller modifies memory without CPU involvement
 * - Memory-Mapped I/O: Reading a register may have side effects (clear flag)
 * - Multi-Core Systems: Shared memory between cores needs volatile
 * - Debugger Watches: Ensures variables are readable during debugging
 *
 * EXAMPLES:
 * Without volatile (BUG):
 *   uint32_t *status = (uint32_t *)0x40000000;
 *   while (*status == 0);  // Compiler reads once, loops forever!
 *
 * With volatile (CORRECT):
 *   volatile uint32_t *status = (volatile uint32_t *)0x40000000;
 *   while (*status == 0);  // Compiler re-reads each iteration
 *
 * ISR Flag Example:
 *   volatile bool data_ready = false;
 *   void UART_ISR() { data_ready = true; }
 *   void main() { while(!data_ready); process(); }  // Works correctly!
 *
 * KEY CONCEPT:
 * Volatile guarantees:
 * 1. Every read/write accesses memory (no caching in registers)
 * 2. Order of volatile accesses is preserved
 * 3. Accesses won't be optimized away
 *
 * Volatile does NOT guarantee:
 * 1. Atomicity (still need critical sections for multi-byte ops)
 * 2. Memory barriers on all architectures
 * 3. Thread safety (need mutexes for that)
 *
 * VISUAL:
 *
 *   WITHOUT VOLATILE (Compiler optimizes):
 *
 *   Source Code:                    Compiled Assembly:
 *   +-------------------------+     +---------------------------+
 *   | while (*status == 0);   |     | LDR R0, [status_addr]     | <- Read ONCE
 *   | // Wait for hardware    |     | loop:                     |
 *   +-------------------------+     |   CMP R0, #0              |
 *                                   |   BEQ loop                | <- Infinite!
 *                                   +---------------------------+
 *
 *
 *   WITH VOLATILE (Compiler obeys):
 *
 *   Source Code:                    Compiled Assembly:
 *   +---------------------------+   +---------------------------+
 *   | volatile uint32_t *status |   | loop:                     |
 *   | while (*status == 0);     |   |   LDR R0, [status_addr]   | <- Read EVERY time
 *   +---------------------------+   |   CMP R0, #0              |
 *                                   |   BEQ loop                | <- Exits when HW sets
 *                                   +---------------------------+
 *
 *
 *   ISR AND MAIN LOOP COMMUNICATION:
 *
 *   Main Loop Memory View:          ISR Memory View:
 *   +------------------+            +------------------+
 *   |  data_ready = 0  |  <------>  |  data_ready = 1  |
 *   +------------------+    RAM     +------------------+
 *           |                               |
 *           v                               v
 *   Without volatile:               ISR writes to RAM:
 *   Main loop reads cached          data_ready = 1
 *   value (always 0)                (change in RAM, not seen!)
 *
 *   With volatile:                  ISR writes to RAM:
 *   Main loop re-reads RAM          data_ready = 1
 *   each iteration (sees 1!)        (main sees change!)
 *
 *
 *   VOLATILE POINTER TYPES:
 *
 *   volatile uint32_t *p;           // Pointer to volatile data
 *   +-------+     +---------+        (data can change unexpectedly)
 *   |   p   | --> | *p DATA |
 *   +-------+     +---------+
 *    normal        volatile
 *
 *   uint32_t * volatile p;          // Volatile pointer to normal data
 *   +-------+     +---------+        (pointer itself can change)
 *   |   p   | --> | *p DATA |
 *   +-------+     +---------+
 *   volatile       normal
 *
 *   volatile uint32_t * volatile p; // Both are volatile
 *   +-------+     +---------+
 *   |   p   | --> | *p DATA |
 *   +-------+     +---------+
 *   volatile      volatile
 *
 * ============================================================================
 */

// Volatile Keyword - ESSENTIAL for embedded memory-mapped I/O and ISR
// Understanding when and why to use volatile

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================
// EXAMPLE 1: Memory-Mapped Hardware Register
// ============================================================

// Say: "Hardware register must be volatile - compiler can't optimize away"
#define GPIO_PORT_A_DATA (*(volatile uint32_t *)0x40020000)
#define GPIO_PORT_A_DIR  (*(volatile uint32_t *)0x40020004)

// Say: "Without volatile, compiler might optimize this to nothing"
void bad_wait_for_button(void) {
    // uint32_t *status = (uint32_t *)0x40020000;  // BAD - not volatile
    // while (*status == 0);  // Compiler may read once and loop forever
}

// Say: "With volatile, compiler reads register every iteration"
void good_wait_for_button(void) {
    // volatile uint32_t *status = (volatile uint32_t *)0x40020000;
    // while (*status == 0);  // Reads register each time - correct!
}

// ============================================================
// EXAMPLE 2: ISR Shared Variable
// ============================================================

// Say: "Shared between ISR and main - MUST be volatile"
volatile bool data_ready = false;
volatile uint8_t received_data = 0;

// Say: "Simulated UART ISR - sets flag when data arrives"
void UART_IRQHandler(void) {
    received_data = 0x42;  // Say: "Data from UART"
    data_ready = true;     // Say: "Signal to main loop"
}

// Say: "Main loop waits for ISR to set flag"
void process_uart_data(void) {
    // Without volatile:
    // while (!data_ready);  // Compiler might read once, infinite loop!

    // With volatile: correct behavior
    while (!data_ready) {
        // Say: "Compiler must re-read data_ready each iteration"
    }

    // Say: "Process the data"
    printf("Received: 0x%02X\n", received_data);
    data_ready = false;
}

// ============================================================
// EXAMPLE 3: Memory Barrier Illustration
// ============================================================

volatile int sequence_counter = 0;
volatile int buffer[10];

// Say: "Volatile prevents reordering around volatile accesses"
void producer(int value) {
    buffer[sequence_counter % 10] = value;  // Say: "Write data first"
    sequence_counter++;                      // Say: "Then update counter"
    // Say: "Volatile ensures this order is preserved"
}

// ============================================================
// WHAT VOLATILE DOES AND DOESN'T DO
// ============================================================

void volatile_demonstration(void) {
    printf("\n=== What volatile does ===\n");

    printf("\nVOLATILE GUARANTEES:\n");
    printf("1. Every read/write goes to memory (no register caching)\n");
    printf("2. Order of volatile accesses is preserved\n");
    printf("3. Compiler won't optimize away accesses\n");

    printf("\nVOLATILE DOES NOT:\n");
    printf("1. Make operations atomic (need atomic types or critical sections)\n");
    printf("2. Provide memory barriers on all architectures\n");
    printf("3. Protect against race conditions (need mutexes)\n");

    printf("\n=== When to use volatile ===\n");
    printf("1. Memory-mapped hardware registers\n");
    printf("2. Variables shared with ISR\n");
    printf("3. Variables modified by external hardware (DMA)\n");
    printf("4. Busy-wait loops checking hardware status\n");
}

// ============================================================
// CORRECT PATTERNS
// ============================================================

// Say: "Volatile pointer to non-volatile data"
// The pointer itself can be cached, but target must be re-read
uint32_t * volatile ptr_is_volatile;

// Say: "Non-volatile pointer to volatile data"
// Pointer can be cached, data at address must be re-read
volatile uint32_t *data_is_volatile;

// Say: "Both pointer and data are volatile"
volatile uint32_t * volatile both_volatile;

int main() {
    printf("=== Volatile Keyword Demo ===\n");

    volatile_demonstration();

    printf("\n=== Type declarations ===\n");
    printf("volatile uint32_t x;           // Variable is volatile\n");
    printf("volatile uint32_t *p;          // Points to volatile data\n");
    printf("uint32_t * volatile p;         // Pointer itself is volatile\n");
    printf("volatile uint32_t * volatile p; // Both are volatile\n");

    // Simulate ISR scenario
    printf("\n=== ISR Simulation ===\n");
    printf("Simulating UART interrupt...\n");
    UART_IRQHandler();  // Simulate ISR call
    process_uart_data();

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Volatile tells the compiler that a variable can change unexpectedly,
 so it must not optimize away reads/writes.

 WHY VOLATILE IS NEEDED:

 1. HARDWARE REGISTERS:
    Registers can change by hardware at any time.
    Without volatile, compiler may read once and cache the value.

    volatile uint32_t *reg = (volatile uint32_t *)0x40000000;
    while (*reg == 0);  // Must re-read each time

 2. ISR SHARED VARIABLES:
    Main loop and ISR share variables.
    Without volatile, main loop may never see ISR's changes.

    volatile bool flag = false;
    // ISR sets flag = true
    // Main: while(!flag);  // Must re-read flag

 3. MULTI-THREADED (limited):
    Variables shared between threads need volatile
    BUT volatile alone is not enough - need synchronization too.

 WHAT VOLATILE GUARANTEES:
 - No read/write optimization (always access memory)
 - Order preserved for volatile accesses
 - Won't be optimized away

 WHAT VOLATILE DOESN'T GUARANTEE:
 - Atomicity (uint32_t read may not be atomic on 8-bit MCU)
 - Memory barriers (may need explicit barriers)
 - Thread safety (need mutexes for that)

 COMMON MISTAKES:
 - Forgetting volatile on hardware registers
 - Using volatile instead of proper synchronization
 - Volatile on local variables (rarely needed)
 - Not understanding it doesn't prevent race conditions

 EMBEDDED BEST PRACTICE:
 - All hardware register accesses through volatile pointers
 - All ISR-shared variables are volatile
 - Still use critical sections for multi-step operations
 - Consider atomic types for simple flags (C11 stdatomic.h)"
*/
