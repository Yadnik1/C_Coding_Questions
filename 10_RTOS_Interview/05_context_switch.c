/*
 * ============================================================================
 * QUESTION 05: What is a Context Switch? How does it work?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * This question tests your understanding of low-level RTOS operation.
 * Be prepared to explain what gets saved/restored and the hardware role.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "A context switch is the process of saving the current task's CPU state
 * (registers, program counter, stack pointer) and restoring another task's
 * state so it can resume execution. On ARM Cortex-M, when an interrupt occurs,
 * hardware automatically saves 8 registers; software saves the remaining 8.
 * The scheduler then updates the stack pointer to the new task's stack.
 * Context switches are triggered by tick interrupts, API calls, or yields.
 * The time taken is typically 1-10 microseconds."
 *
 * ============================================================================
 * WHAT IS A CONTEXT?
 * ============================================================================
 *
 * A task's "context" is everything needed to resume its execution:
 *
 * 1. CPU REGISTERS
 *    - General purpose registers (R0-R12 on ARM)
 *    - Stack Pointer (SP/R13)
 *    - Link Register (LR/R14) - return address
 *    - Program Counter (PC/R15) - next instruction
 *    - Status Register (xPSR) - flags, interrupt state
 *
 * 2. STACK CONTENTS
 *    - Local variables
 *    - Function call history
 *    - Saved register values
 *
 * 3. TASK CONTROL BLOCK (TCB)
 *    - Priority
 *    - State
 *    - Stack pointer (saved here during switch)
 *
 * ============================================================================
 * VISUALIZATION: Context Switch on ARM Cortex-M
 * ============================================================================
 *
 * BEFORE SWITCH (Task A running):
 *
 *   CPU Registers              Task A's Stack
 *   +----------+              +------------------+
 *   | R0-R3    |              | Local vars       |
 *   | R12      |              | Return addresses |
 *   | LR (R14) |              | ...              |
 *   | PC (R15) |  <-- SP --->  +------------------+
 *   | xPSR     |              | (Stack grows down)|
 *   | R4-R11   |              |                  |
 *   +----------+              +------------------+
 *
 *
 * STEP 1: Hardware Exception Entry (Automatic)
 *
 *   When PendSV exception triggers, hardware AUTOMATICALLY pushes:
 *
 *   Task A's Stack (AFTER hardware push):
 *   +------------------+
 *   | xPSR             | <- Pushed first (highest address)
 *   +------------------+
 *   | PC               |
 *   +------------------+
 *   | LR               |
 *   +------------------+
 *   | R12              |
 *   +------------------+
 *   | R3               |
 *   +------------------+
 *   | R2               |
 *   +------------------+
 *   | R1               |
 *   +------------------+
 *   | R0               | <- SP points here after hardware push
 *   +------------------+
 *   | (space for R4-R11)|
 *   +------------------+
 *
 *
 * STEP 2: Software Context Save (PendSV Handler)
 *
 *   PendSV handler pushes remaining registers:
 *
 *   Task A's Stack (AFTER software push):
 *   +------------------+
 *   | xPSR             |
 *   +------------------+
 *   | PC               |
 *   +------------------+
 *   | LR               |
 *   +------------------+
 *   | R12              |
 *   +------------------+
 *   | R3               |
 *   +------------------+
 *   | R2               |
 *   +------------------+
 *   | R1               |
 *   +------------------+
 *   | R0               | <- Hardware pushed above this
 *   +------------------+
 *   | R4               | <- Software pushes below
 *   +------------------+
 *   | R5               |
 *   +------------------+
 *   | R6               |
 *   +------------------+
 *   | R7               |
 *   +------------------+
 *   | R8               |
 *   +------------------+
 *   | R9               |
 *   +------------------+
 *   | R10              |
 *   +------------------+
 *   | R11              | <- NEW SP saved to Task A's TCB
 *   +------------------+
 *
 *
 * STEP 3: Update TCBs
 *
 *   Task A TCB:                Task B TCB:
 *   +------------------+       +------------------+
 *   | SP = (new value) |       | SP = (saved SP)  | <- Load this
 *   +------------------+       +------------------+
 *
 *
 * STEP 4: Restore Task B's Context
 *
 *   - Load Task B's SP from its TCB
 *   - Software pops R4-R11
 *   - Hardware automatically pops R0-R3, R12, LR, PC, xPSR on exception return
 *   - Task B resumes executing!
 *
 * ============================================================================
 * ARM CORTEX-M PENDSV HANDLER (Simplified)
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
 * Actual FreeRTOS PendSV handler (ARM Cortex-M3/M4):
 *
 * This is assembly code that performs the context switch.
 * Understanding this shows deep RTOS knowledge.
 */

/*
__asm void xPortPendSVHandler(void)
{
    extern pxCurrentTCB;        // Pointer to current task's TCB
    extern vTaskSwitchContext;  // C function to select next task

    PRESERVE8

    // Get current task's stack pointer
    mrs r0, psp                 // r0 = Process Stack Pointer
    isb

    // Get address of pxCurrentTCB
    ldr r3, =pxCurrentTCB
    ldr r2, [r3]                // r2 = current TCB address

    // Save remaining registers (R4-R11) to task's stack
    stmdb r0!, {r4-r11}         // Push R4-R11, decrement r0

    // Save new stack pointer to TCB
    str r0, [r2]                // TCB->pxTopOfStack = r0

    // Save LR and call scheduler
    stmdb sp!, {r3, r14}        // Save r3 (TCB ptr) and LR
    mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY
    msr basepri, r0             // Disable interrupts
    dsb
    isb
    bl vTaskSwitchContext       // Call C scheduler
    mov r0, #0
    msr basepri, r0             // Re-enable interrupts
    ldmia sp!, {r3, r14}        // Restore r3 and LR

    // Get new task's TCB (scheduler may have changed pxCurrentTCB)
    ldr r1, [r3]                // r1 = new TCB address
    ldr r0, [r1]                // r0 = new task's stack pointer

    // Restore R4-R11 from new task's stack
    ldmia r0!, {r4-r11}         // Pop R4-R11, increment r0

    // Update PSP to new task's stack
    msr psp, r0
    isb

    // Return from exception
    // Hardware will automatically pop R0-R3, R12, LR, PC, xPSR
    bx r14
}
*/

/*
 * ============================================================================
 * CONTEXT SWITCH TRIGGERS
 * ============================================================================
 *
 * 1. TICK INTERRUPT
 *    - Periodic timer interrupt (e.g., every 1ms)
 *    - Checks if higher priority task is ready
 *    - Implements time slicing
 *
 *    void SysTick_Handler(void) {
 *        if (xTaskIncrementTick() != pdFALSE) {
 *            // Request context switch
 *            portYIELD_FROM_ISR(pdTRUE);
 *        }
 *    }
 *
 * 2. API CALLS THAT UNBLOCK TASKS
 *    - xSemaphoreGive() - unblocks waiting task
 *    - xQueueSend() - unblocks receiving task
 *    - xTaskNotifyGive() - unblocks notified task
 *
 *    If unblocked task has higher priority, context switch occurs
 *
 * 3. VOLUNTARY YIELD
 *    - taskYIELD() - task gives up CPU
 *    - vTaskDelay() - task sleeps
 *    - xSemaphoreTake() with no semaphore - task blocks
 *
 * 4. TASK PRIORITY CHANGE
 *    - vTaskPrioritySet() - if new priority causes preemption
 *
 * ============================================================================
 * CONTEXT SWITCH TIME
 * ============================================================================
 *
 * Typical context switch times:
 *
 * +------------------+-------------------+
 * | Processor        | Context Switch    |
 * +------------------+-------------------+
 * | ARM Cortex-M0    | ~5-15 us          |
 * | ARM Cortex-M3/M4 | ~1-5 us           |
 * | ARM Cortex-M7    | ~0.5-2 us         |
 * | ARM Cortex-A     | ~1-10 us          |
 * | x86 (RTOS)       | ~1-5 us           |
 * | Linux (GPOS)     | ~1-10+ ms         |
 * +------------------+-------------------+
 *
 * Factors affecting switch time:
 * - Number of registers to save (FPU adds more)
 * - Cache effects
 * - Memory speed
 * - Interrupt latency
 *
 * ============================================================================
 * FPU CONTEXT (Floating Point Unit)
 * ============================================================================
 *
 * If FPU is used, additional registers must be saved:
 *
 * ARM Cortex-M4F FPU registers:
 * - S0-S31 (32 single-precision registers)
 * - FPSCR (FPU status register)
 *
 * FreeRTOS handles this automatically with:
 * #define configUSE_TASK_FPU_SUPPORT 1
 *
 * Lazy stacking: Hardware can defer FPU context save until actually needed
 * (reduces switch time when FPU not used by both tasks)
 *
 * ============================================================================
 * ZEPHYR CONTEXT SWITCH
 * ============================================================================
 */

/*
// Zephyr uses similar mechanism but different naming

// Thread context structure (simplified)
struct _callee_saved {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t psp;
};

// Context switch in Zephyr
void z_arm_pendsv(void) {
    // Similar to FreeRTOS:
    // 1. Save callee-saved registers
    // 2. Save SP to current thread
    // 3. Call scheduler
    // 4. Load SP from next thread
    // 5. Restore callee-saved registers
    // 6. Return from exception
}
*/

/*
 * ============================================================================
 * DEBUGGING CONTEXT SWITCHES
 * ============================================================================
 *
 * FreeRTOS Trace Hooks:
 *
 * #define traceTASK_SWITCHED_IN()  task_switched_in_hook()
 * #define traceTASK_SWITCHED_OUT() task_switched_out_hook()
 *
 * void task_switched_in_hook(void) {
 *     // Log: xTaskGetCurrentTaskHandle(), xTaskGetTickCount()
 * }
 *
 * Tools for analysis:
 * - SEGGER SystemView
 * - Percepio Tracealyzer
 * - Logic analyzer with RTOS-aware decoding
 *
 * ============================================================================
 * MINIMIZING CONTEXT SWITCH OVERHEAD
 * ============================================================================
 *
 * 1. Reduce switch frequency:
 *    - Increase tick period (but reduces timing resolution)
 *    - Use event-driven rather than polling
 *
 * 2. Reduce per-switch overhead:
 *    - Disable FPU if not needed
 *    - Minimize TCB size
 *    - Use efficient scheduler algorithm (FreeRTOS is O(1))
 *
 * 3. Design considerations:
 *    - Combine related work in one task
 *    - Use appropriate task priorities
 *    - Avoid unnecessary blocking/unblocking
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. KNOW THE HARDWARE ROLE
 *    "On ARM Cortex-M, hardware automatically saves 8 registers..."
 *
 * 2. MENTION THE STEPS
 *    Save current context -> Select next task -> Restore context
 *
 * 3. KNOW TRIGGERS
 *    Tick interrupt, API calls, yield, priority change
 *
 * 4. MENTION TIMING
 *    "Typically 1-10 microseconds on Cortex-M"
 *
 * 5. DISCUSS FPU IMPACT
 *    Shows you understand real-world considerations
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "What's the difference between PSP and MSP on Cortex-M?"
 * A: MSP (Main Stack Pointer) - used by handler mode (ISRs, kernel)
 *    PSP (Process Stack Pointer) - used by thread mode (tasks)
 *    This separation protects kernel stack from task stack overflow.
 *
 * Q: "Why use PendSV for context switch?"
 * A: PendSV has lowest priority. This ensures all other ISRs complete
 *    before context switch. Prevents nested context switches.
 *
 * Q: "What happens if you do context switch in a critical section?"
 * A: You shouldn't! Critical sections disable interrupts/scheduler.
 *    Context switch after critical section exits.
 *
 * Q: "How does lazy stacking work for FPU?"
 * A: Hardware sets a flag indicating FPU was used. Actual FPU registers
 *    are only saved/restored if both outgoing and incoming tasks used FPU.
 *    Saves time when only one task uses FPU.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 05 ===\n\n");
    printf("Q: What is a Context Switch? How does it work?\n\n");
    printf("DEFINITION:\n");
    printf("Saving current task's CPU state and restoring another task's state\n\n");
    printf("WHAT GETS SAVED:\n");
    printf("- CPU registers (R0-R12)\n");
    printf("- Stack Pointer (SP)\n");
    printf("- Program Counter (PC)\n");
    printf("- Status Register (xPSR)\n");
    printf("- FPU registers (if used)\n\n");
    printf("ARM CORTEX-M MECHANISM:\n");
    printf("1. Hardware pushes R0-R3, R12, LR, PC, xPSR (automatic)\n");
    printf("2. Software pushes R4-R11 (PendSV handler)\n");
    printf("3. Save SP to current TCB\n");
    printf("4. Scheduler selects next task\n");
    printf("5. Load SP from new TCB\n");
    printf("6. Software pops R4-R11\n");
    printf("7. Hardware pops rest on exception return\n\n");
    printf("TIME: ~1-10 microseconds on Cortex-M\n");
    return 0;
}
