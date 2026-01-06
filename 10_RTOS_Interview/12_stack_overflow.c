/*
 * ============================================================================
 * QUESTION 12: How do you detect and prevent Stack Overflow in RTOS?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: High | IMPORTANCE: Critical
 *
 * Stack overflow is one of the most common bugs in embedded systems.
 * Interviewers want to know you can prevent and debug this issue.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Stack overflow occurs when a task uses more stack than allocated, corrupting
 * adjacent memory. Prevention: calculate stack requirements considering local
 * variables, function call depth, and worst-case ISR stacking. Detection in
 * FreeRTOS: enable configCHECK_FOR_STACK_OVERFLOW (two methods), implement
 * vApplicationStackOverflowHook(). Runtime monitoring: use watermark checking
 * with uxTaskGetStackHighWaterMark(). Design: use static analysis, avoid deep
 * recursion, minimize large local arrays, and add safety margins to stack sizes."
 *
 * ============================================================================
 * VISUALIZATION: Stack Overflow
 * ============================================================================
 *
 * NORMAL OPERATION:
 *
 *   High Address
 *   +------------------+
 *   | Task A TCB       |  <-- TCB data (priority, state, etc.)
 *   +------------------+
 *   | Task A Stack     |
 *   |  ...             |
 *   |  local vars      |
 *   |  return addrs    |
 *   |  ...             |  <-- SP (stack pointer)
 *   |                  |  <-- Unused stack space (OK!)
 *   +------------------+
 *   | Task B Stack     |  <-- Separate stack
 *   +------------------+
 *   Low Address
 *
 *
 * STACK OVERFLOW:
 *
 *   High Address
 *   +------------------+
 *   | Task A TCB       |  <-- CORRUPTED! Random crashes!
 *   +--██████████████──+
 *   | Task A Stack     |
 *   |  ...             |
 *   |  █OVERFLOW███    |  <-- SP went past boundary!
 *   |  █CORRUPTS██     |
 *   +--██████████████──+
 *   | Task B Stack     |  <-- CORRUPTED! Task B crashes!
 *   +------------------+
 *   Low Address
 *
 *   RESULT: Random crashes, data corruption, hard to debug!
 *
 * ============================================================================
 * CAUSES OF STACK OVERFLOW
 * ============================================================================
 *
 * 1. DEEP FUNCTION NESTING
 *    func_a() -> func_b() -> func_c() -> func_d() -> ...
 *    Each call pushes return address + registers
 *
 * 2. LARGE LOCAL ARRAYS
 *    void function() {
 *        char buffer[4096];  // 4KB on stack! Dangerous!
 *    }
 *
 * 3. RECURSION
 *    void recursive(int n) {
 *        if (n > 0) recursive(n-1);  // Each call uses stack
 *    }
 *
 * 4. ISR USING TASK STACK
 *    On some architectures, ISR uses current task's stack
 *    Nested ISRs multiply stack usage!
 *
 * 5. UNDERESTIMATED STACK SIZE
 *    Worst case not considered (all branches, max nesting)
 *
 * ============================================================================
 * FREERTOS STACK OVERFLOW DETECTION
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
// FreeRTOSConfig.h settings

// Enable stack overflow checking (0 = disabled, 1 or 2 = enabled)
#define configCHECK_FOR_STACK_OVERFLOW  2


// METHOD 1: Check SP on context switch
// #define configCHECK_FOR_STACK_OVERFLOW  1
//
// On each context switch, check if SP went below stack start.
// PROS: Low overhead
// CONS: Might miss overflow that recovered before switch


// METHOD 2: Check stack canary pattern
// #define configCHECK_FOR_STACK_OVERFLOW  2
//
// Fills last 20 bytes of stack with known pattern (0xA5).
// On context switch, checks if pattern is intact.
// PROS: Catches more overflows
// CONS: Slightly higher overhead


// Hook function - called when overflow detected
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // CRITICAL: Stack is already corrupted at this point!

    // Option 1: Log and halt (for debugging)
    printf("STACK OVERFLOW in task: %s\n", pcTaskName);
    while(1);  // Halt for debugger

    // Option 2: Reset system
    // NVIC_SystemReset();

    // Option 3: Log to persistent storage for post-mortem
    // log_to_flash("Stack overflow", pcTaskName);
    // NVIC_SystemReset();
}
*/

/*
 * ============================================================================
 * STACK WATERMARK MONITORING
 * ============================================================================
 *
 * "High water mark" = minimum free stack space ever
 *
 * FreeRTOS fills stack with 0xA5 pattern at creation.
 * Watermark = how much 0xA5 pattern remains.
 */

/*
void monitor_stack_usage(void) {
    TaskHandle_t task = xTaskGetCurrentTaskHandle();

    // Get minimum free stack (in WORDS, not bytes!)
    UBaseType_t watermark = uxTaskGetStackHighWaterMark(task);

    // Convert to bytes (4 bytes per word on 32-bit)
    size_t free_bytes = watermark * sizeof(StackType_t);

    printf("Minimum free stack: %u bytes\n", free_bytes);

    if (free_bytes < 100) {
        printf("WARNING: Stack almost full!\n");
    }
}


// Monitor all tasks
void print_all_stack_usage(void) {
    char buffer[512];

    // Get task list with stack info
    vTaskList(buffer);
    printf("Task\t\tState\tPri\tStack\tNum\n");
    printf("%s\n", buffer);

    // vTaskList output format:
    // TaskName    State   Priority   Stack   TaskNumber
    // Task1       R       1          200     1
    // Task2       B       2          150     2
}
*/

/*
 * ============================================================================
 * ZEPHYR STACK PROTECTION
 * ============================================================================
 */

/*
// prj.conf settings
CONFIG_THREAD_STACK_INFO=y      // Track stack usage
CONFIG_THREAD_ANALYZER=y        // Built-in analyzer
CONFIG_INIT_STACKS=y            // Fill with pattern

// Optional: Hardware stack protection (MPU)
CONFIG_HW_STACK_PROTECTION=y    // Uses MPU to detect overflow


// Get stack usage
void check_zephyr_stack(void) {
    size_t unused = 0;
    int ret = k_thread_stack_space_get(k_current_get(), &unused);

    if (ret == 0) {
        printk("Unused stack: %zu bytes\n", unused);
    }
}

// Thread analyzer - prints all thread stack usage
// Call thread_analyzer_print() or enable auto-print:
CONFIG_THREAD_ANALYZER_AUTO=y
CONFIG_THREAD_ANALYZER_AUTO_INTERVAL=60  // seconds
*/

/*
 * ============================================================================
 * CALCULATING STACK SIZE
 * ============================================================================
 *
 * Stack requirements:
 *
 * 1. LOCAL VARIABLES
 *    - Sum of all local variable sizes in worst-case call path
 *    - Remember arrays and structs!
 *
 * 2. FUNCTION CALLS
 *    - Return address: 4 bytes (32-bit)
 *    - Saved registers: depends on calling convention
 *    - Compiler may add padding
 *
 * 3. RTOS OVERHEAD
 *    - Context save area (R4-R11 on Cortex-M)
 *    - FreeRTOS internal structures
 *
 * 4. ISR OVERHEAD (if ISR uses task stack)
 *    - Hardware pushes 8 registers automatically
 *    - ISR local variables
 *    - Nested interrupts multiply this!
 *
 *
 * EXAMPLE CALCULATION:
 *
 *   Task function:
 *     - 3 levels of function calls
 *     - Max local vars per function: 100 bytes
 *     - Total locals: 300 bytes
 *
 *   Function call overhead:
 *     - 3 calls × (return addr + saved regs) ≈ 3 × 40 = 120 bytes
 *
 *   RTOS context:
 *     - R4-R11 save: 32 bytes
 *     - FreeRTOS internal: ~50 bytes
 *
 *   ISR overhead:
 *     - Hardware push: 32 bytes
 *     - ISR locals: 50 bytes
 *     - One nesting level: × 2 = 164 bytes
 *
 *   TOTAL: 300 + 120 + 82 + 164 = 666 bytes
 *   WITH MARGIN (50%): 1000 bytes
 *
 *
 * RULE OF THUMB:
 *   Minimum: 256 bytes for simple tasks
 *   Typical: 512-1024 bytes
 *   Complex: 2048+ bytes
 *   Add 50% safety margin!
 *
 * ============================================================================
 * STATIC ANALYSIS TOOLS
 * ============================================================================
 *
 * 1. GCC -fstack-usage
 *    gcc -fstack-usage file.c
 *    Generates .su files with stack usage per function
 *
 * 2. ARM Compiler --info=stack
 *    armcc --info=stack file.c
 *
 * 3. Commercial tools
 *    - LDRA
 *    - Polyspace
 *    - Understand (SciTools)
 *
 * 4. Manual call graph analysis
 *    - Draw function call tree
 *    - Sum stack along deepest path
 *
 * ============================================================================
 * BEST PRACTICES
 * ============================================================================
 */

/*
// 1. AVOID large local arrays - use static or heap
void bad_function(void) {
    uint8_t buffer[2048];  // BAD: 2KB on stack
    process(buffer);
}

void good_function(void) {
    static uint8_t buffer[2048];  // GOOD: In .bss, not stack
    process(buffer);
}


// 2. AVOID recursion - use iteration
int bad_factorial(int n) {
    if (n <= 1) return 1;
    return n * bad_factorial(n - 1);  // BAD: n levels of recursion
}

int good_factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;  // GOOD: constant stack usage
    }
    return result;
}


// 3. USE configMINIMAL_STACK_SIZE as baseline
#define MY_TASK_STACK  (configMINIMAL_STACK_SIZE + 256)  // Baseline + my needs


// 4. MONITOR in debug builds
#ifdef DEBUG
void my_task(void *pvParameters) {
    for (;;) {
        do_work();

        // Check stack periodically
        UBaseType_t mark = uxTaskGetStackHighWaterMark(NULL);
        if (mark < 50) {
            printf("WARNING: Task stack low: %u words\n", mark);
        }

        vTaskDelay(100);
    }
}
#endif
*/

/*
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN THE DANGER
 *    "Overflow corrupts adjacent memory - random crashes, hard to debug"
 *
 * 2. MENTION DETECTION METHODS
 *    "configCHECK_FOR_STACK_OVERFLOW = 2, watermark monitoring"
 *
 * 3. DISCUSS PREVENTION
 *    "Calculate requirements, avoid recursion, use static/heap for big data"
 *
 * 4. KNOW THE TOOLS
 *    "uxTaskGetStackHighWaterMark, -fstack-usage, static analysis"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "How do you debug a suspected stack overflow?"
 * A: 1. Enable configCHECK_FOR_STACK_OVERFLOW
 *    2. Check watermarks for all tasks
 *    3. Look for corruption patterns (0xDEADBEEF, etc.)
 *    4. Use memory watchpoints on stack boundaries
 *    5. Enable MPU stack protection (Zephyr)
 *
 * Q: "What's the difference between stack overflow check method 1 and 2?"
 * A: Method 1: Checks SP on context switch (fast but can miss)
 *    Method 2: Fills stack with pattern, checks if corrupted (more thorough)
 *
 * Q: "Why is stack overflow harder to debug than other bugs?"
 * A: Symptoms are random (corrupts whatever is adjacent), may not crash
 *    immediately, corruption pattern varies, no clear error message.
 *
 * Q: "Can you use dynamic stack allocation?"
 * A: Not typically in RTOS - stack size fixed at task creation.
 *    Some RTOS support stack extension but adds complexity.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 12 ===\n\n");
    printf("Q: How do you detect and prevent Stack Overflow?\n\n");
    printf("CAUSES:\n");
    printf("- Deep function nesting\n");
    printf("- Large local arrays\n");
    printf("- Recursion\n");
    printf("- ISR using task stack\n\n");
    printf("DETECTION (FreeRTOS):\n");
    printf("- configCHECK_FOR_STACK_OVERFLOW = 1 or 2\n");
    printf("- vApplicationStackOverflowHook() callback\n");
    printf("- uxTaskGetStackHighWaterMark() monitoring\n\n");
    printf("PREVENTION:\n");
    printf("- Calculate stack needs + 50%% margin\n");
    printf("- Avoid recursion, large local arrays\n");
    printf("- Use static/heap for big data\n");
    printf("- Static analysis (-fstack-usage)\n\n");
    printf("ZEPHYR:\n");
    printf("- CONFIG_HW_STACK_PROTECTION (MPU)\n");
    printf("- CONFIG_THREAD_ANALYZER\n");
    return 0;
}
