/*
 * ============================================================================
 * QUESTION 36: Common RTOS Interview Questions & Answers
 * ============================================================================
 *
 * DIFFICULTY: Various | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * Rapid-fire common questions with concise answers for interview prep.
 *
 * ============================================================================
 * CATEGORY 1: BASIC CONCEPTS
 * ============================================================================
 *
 * Q1: What is an RTOS?
 * A1: An operating system that guarantees task completion within specific
 *     time constraints. Key difference from GPOS: deterministic timing.
 *
 * Q2: What's the difference between hard and soft real-time?
 * A2: Hard: Missing deadline = system failure (airbag, pacemaker)
 *     Soft: Missing deadline = degraded quality (video streaming)
 *
 * Q3: What is determinism in RTOS context?
 * A3: Predictable, bounded response time. Operations complete within known
 *     worst-case time regardless of system load.
 *
 * Q4: Why use RTOS instead of bare-metal?
 * A4: - Task management (multiple independent activities)
 *     - Timing control (delays, periodic tasks)
 *     - Synchronization primitives (mutex, semaphore)
 *     - Better code organization
 *     - Easier debugging and maintenance
 *
 * Q5: What's the typical RTOS footprint?
 * A5: FreeRTOS: ~6-10KB ROM, Zephyr minimal: ~8KB ROM
 *     Full-featured Zephyr: 50-100KB+
 *
 * ============================================================================
 * CATEGORY 2: TASKS AND SCHEDULING
 * ============================================================================
 *
 * Q6: What is a task (or thread) in RTOS?
 * A6: Independent execution context with own stack. Multiple tasks share
 *     code and data space. RTOS scheduler determines which task runs.
 *
 * Q7: What are the task states?
 * A7: Running: Currently executing on CPU
 *     Ready: Can run, waiting for CPU
 *     Blocked: Waiting for event/resource
 *     Suspended: Explicitly paused
 *     Deleted: Terminated
 *
 * Q8: What is preemptive scheduling?
 * A8: Higher priority task immediately takes CPU from lower priority task.
 *     Context switch happens when higher priority task becomes ready.
 *
 * Q9: What is cooperative scheduling?
 * A9: Tasks explicitly yield CPU. Current task runs until it blocks or yields.
 *     No involuntary preemption.
 *
 * Q10: What is round-robin scheduling?
 * A10: Tasks of same priority share CPU time. Each gets time slice (quantum).
 *      After quantum expires, next same-priority task runs.
 *
 * Q11: What is Rate Monotonic Scheduling (RMS)?
 * A11: Fixed-priority algorithm where shorter period = higher priority.
 *      Optimal for fixed-priority periodic tasks with D=T.
 *
 * Q12: What is the context switch?
 * A12: Saving current task's CPU state (registers, SP, PC) and restoring
 *      another task's state. Typically ~1-10 microseconds in RTOS.
 *
 * ============================================================================
 * CATEGORY 3: SYNCHRONIZATION
 * ============================================================================
 *
 * Q13: What is a semaphore?
 * A13: Signaling primitive with counter. Binary (0/1) for signaling,
 *      counting for resource pools. No ownership - any task can give.
 *
 * Q14: What is a mutex?
 * A14: Mutual exclusion lock with ownership. Only owner can release.
 *      Has priority inheritance to prevent priority inversion.
 *
 * Q15: When to use semaphore vs mutex?
 * A15: Semaphore: Signaling between tasks, counting resources
 *      Mutex: Protecting shared resource, need priority inheritance
 *
 * Q16: What is priority inversion?
 * A16: High-priority task waits for low-priority task holding resource.
 *      Medium-priority task preempts low, causing high to wait for medium.
 *      Solution: Priority inheritance (mutex owner inherits waiter's priority)
 *
 * Q17: What is a deadlock?
 * A17: Two or more tasks waiting for each other's resources, forever.
 *      Requires: Mutual exclusion, hold and wait, no preemption, circular wait.
 *
 * Q18: How to prevent deadlock?
 * A18: - Always acquire locks in same order
 *      - Use timeout on lock acquisition
 *      - Avoid holding multiple locks
 *      - Use deadlock detection
 *
 * Q19: What is a race condition?
 * A19: Outcome depends on non-deterministic timing of operations.
 *      Multiple tasks access shared data without proper synchronization.
 *
 * Q20: What is a critical section?
 * A20: Code that accesses shared resources and must execute atomically.
 *      Protected by disabling interrupts or using mutex.
 *
 * ============================================================================
 * CATEGORY 4: INTER-TASK COMMUNICATION
 * ============================================================================
 *
 * Q21: What is a queue (message queue)?
 * A21: FIFO buffer for passing data between tasks. Provides buffering and
 *      synchronization. Producer adds to back, consumer takes from front.
 *
 * Q22: What are event groups?
 * A22: Bit flags for complex synchronization. Tasks can wait for ANY or
 *      ALL of multiple bits. Good for state machines.
 *
 * Q23: What are task notifications?
 * A23: FreeRTOS lightweight signaling. Built into TCB, zero extra RAM.
 *      Can replace semaphore, event bits, or mailbox for simple cases.
 *
 * Q24: How to pass data from ISR to task?
 * A24: Use queue (xQueueSendFromISR) or semaphore (xSemaphoreGiveFromISR).
 *      Always use FromISR variants in ISR context.
 *
 * ============================================================================
 * CATEGORY 5: INTERRUPTS
 * ============================================================================
 *
 * Q25: Why keep ISR short?
 * A25: - Minimize interrupt latency for other interrupts
 *      - Scheduler can't run during ISR
 *      - ISR context has limited stack
 *      - Can't use blocking RTOS APIs
 *
 * Q26: What is deferred interrupt processing?
 * A26: ISR does minimal work (acknowledge, signal), task does heavy processing.
 *      Pattern: ISR signals semaphore, task waits and processes.
 *
 * Q27: What is portYIELD_FROM_ISR?
 * A27: Triggers context switch if higher-priority task became ready.
 *      Called at end of ISR after signaling task.
 *
 * Q28: What's the difference between ISR and task priority?
 * A28: Completely separate! ISR priorities are hardware (NVIC).
 *      Task priorities are software (RTOS). ANY ISR preempts ANY task.
 *
 * Q29: What is configMAX_SYSCALL_INTERRUPT_PRIORITY?
 * A29: FreeRTOS threshold. ISRs above can't use FreeRTOS APIs but have
 *      guaranteed low latency. ISRs at or below can use FromISR APIs.
 *
 * ============================================================================
 * CATEGORY 6: MEMORY
 * ============================================================================
 *
 * Q30: How does stack work in RTOS?
 * A30: Each task has own stack for local variables, function calls, context.
 *      Stack size specified at task creation. Overflow corrupts memory.
 *
 * Q31: How to determine stack size?
 * A31: Consider: local variables, call depth, ISR usage if shared stack,
 *      worst-case path. Use stack watermarking to verify.
 *
 * Q32: What is stack overflow?
 * A32: Task uses more stack than allocated. Corrupts adjacent memory.
 *      Enable configCHECK_FOR_STACK_OVERFLOW for detection.
 *
 * Q33: What are FreeRTOS heap implementations?
 * A33: heap_1: Allocate only, no free
 *      heap_2: Best fit, no coalescence (deprecated)
 *      heap_3: Wrapper for malloc
 *      heap_4: First fit + coalescence (most common)
 *      heap_5: Multiple non-contiguous regions
 *
 * Q34: What is fragmentation?
 * A34: Free memory split into small non-contiguous pieces. Large allocation
 *      fails even if total free is sufficient. Solution: Memory pools.
 *
 * Q35: What are memory pools?
 * A35: Pre-allocated fixed-size blocks. O(1) allocation, no fragmentation.
 *      Ideal for real-time with known allocation sizes.
 *
 * ============================================================================
 * CATEGORY 7: TIMING
 * ============================================================================
 *
 * Q36: What is tick rate?
 * A36: Frequency of RTOS timer interrupt. Typically 100-1000Hz.
 *      Higher = better resolution but more CPU overhead.
 *
 * Q37: What's the difference between vTaskDelay and vTaskDelayUntil?
 * A37: vTaskDelay: Delay N ticks from NOW (relative)
 *      vTaskDelayUntil: Delay to specific tick (absolute, for periodic tasks)
 *
 * Q38: What is tickless idle?
 * A38: Don't generate ticks when all tasks blocked. Calculate next wake time,
 *      sleep until then. Saves power on battery devices.
 *
 * Q39: What is WCET?
 * A39: Worst-Case Execution Time. Maximum time a task can take.
 *      Critical for real-time analysis and schedulability.
 *
 * ============================================================================
 * CATEGORY 8: SPECIFIC PATTERNS
 * ============================================================================
 *
 * Q40: What is the Producer-Consumer pattern?
 * A40: Producer generates data to queue, consumer retrieves and processes.
 *      Queue decouples timing, absorbs bursts.
 *
 * Q41: What is a state machine in RTOS?
 * A41: Task that waits on event queue, processes events based on current state.
 *      Uses switch-case on state, then on event.
 *
 * Q42: What is the Active Object pattern?
 * A42: Each object is a task with private event queue. Events processed
 *      in object's thread context. Provides encapsulation.
 *
 * Q43: What is double buffering?
 * A43: Two buffers, one being written while other is read. ISR writes to
 *      write buffer, task reads from read buffer. Lock-free for simple cases.
 *
 * ============================================================================
 * CATEGORY 9: FREERTOS vs ZEPHYR
 * ============================================================================
 *
 * Q44: Key API differences?
 * A44: Task creation: xTaskCreate vs K_THREAD_DEFINE
 *      Queue: xQueueSend vs k_msgq_put
 *      Priority: Higher number = higher (FreeRTOS) vs lower = higher (Zephyr)
 *
 * Q45: Stack size specification?
 * A45: FreeRTOS: Words (multiply by 4 for bytes on 32-bit)
 *      Zephyr: Bytes
 *
 * Q46: Mutex behavior?
 * A46: FreeRTOS: Separate recursive vs non-recursive types
 *      Zephyr: k_mutex is always recursive
 *
 * Q47: When to choose FreeRTOS vs Zephyr?
 * A47: FreeRTOS: Simpler apps, tight memory, quick learning curve
 *      Zephyr: Complex IoT, BLE/WiFi, need device tree, Linux-like build
 *
 * ============================================================================
 * CATEGORY 10: DEBUGGING
 * ============================================================================
 *
 * Q48: How to debug RTOS applications?
 * A48: - Stack overflow detection (configCHECK_FOR_STACK_OVERFLOW)
 *      - Runtime statistics (vTaskGetRunTimeStats)
 *      - RTOS-aware debugger
 *      - Trace tools (SEGGER SystemView)
 *      - configASSERT for catching errors
 *
 * Q49: What is the Heisenbug problem?
 * A49: Bug disappears when observed. Adding printf or breakpoint changes
 *      timing, hiding race condition. Use non-intrusive trace instead.
 *
 * Q50: Common RTOS bugs?
 * A50: - Stack overflow
 *      - Priority inversion
 *      - Deadlock
 *      - Race condition
 *      - Using wrong API (non-FromISR in ISR)
 *      - Memory leak/fragmentation
 *
 * ============================================================================
 */

#include <stdio.h>

int main() {
    printf("=== RTOS Interview Question 36 ===\n\n");
    printf("Common RTOS Interview Q&A Summary\n\n");
    printf("CATEGORIES COVERED:\n");
    printf("1. Basic Concepts (Q1-Q5)\n");
    printf("2. Tasks and Scheduling (Q6-Q12)\n");
    printf("3. Synchronization (Q13-Q20)\n");
    printf("4. Inter-Task Communication (Q21-Q24)\n");
    printf("5. Interrupts (Q25-Q29)\n");
    printf("6. Memory (Q30-Q35)\n");
    printf("7. Timing (Q36-Q39)\n");
    printf("8. Specific Patterns (Q40-Q43)\n");
    printf("9. FreeRTOS vs Zephyr (Q44-Q47)\n");
    printf("10. Debugging (Q48-Q50)\n\n");
    printf("KEY INTERVIEW TIPS:\n");
    printf("- Start with 30-second quick answer\n");
    printf("- Provide specific examples when possible\n");
    printf("- Mention trade-offs and alternatives\n");
    printf("- Relate to real-world experience\n");
    return 0;
}
