/*
 * ============================================================================
 * RTOS INTERVIEW PREPARATION - COMPREHENSIVE GUIDE
 * ============================================================================
 *
 * This section contains 120 RTOS interview questions covering:
 *
 * SECTION 1: RTOS FUNDAMENTALS (Questions 01-10)
 *   - What is an RTOS?
 *   - RTOS vs General Purpose OS
 *   - Real-time constraints (hard vs soft)
 *   - Kernel types and architecture
 *
 * SECTION 2: TASK MANAGEMENT (Questions 11-20)
 *   - Task creation and deletion
 *   - Task states and transitions
 *   - Task priorities
 *   - Context switching
 *
 * SECTION 3: SCHEDULING (Questions 21-30)
 *   - Scheduling algorithms
 *   - Preemptive vs Cooperative
 *   - Time slicing
 *   - Rate Monotonic, EDF
 *
 * SECTION 4: SYNCHRONIZATION (Questions 31-45)
 *   - Mutex (Mutual Exclusion)
 *   - Semaphores (Binary & Counting)
 *   - Priority Inversion & Solutions
 *   - Deadlock & Prevention
 *
 * SECTION 5: INTER-TASK COMMUNICATION (Questions 46-60)
 *   - Message Queues
 *   - Event Groups/Flags
 *   - Mailboxes
 *   - Pipes and Streams
 *
 * SECTION 6: MEMORY MANAGEMENT (Questions 61-70)
 *   - Stack allocation
 *   - Heap management
 *   - Memory pools
 *   - Fragmentation
 *
 * SECTION 7: TIMING & DELAYS (Questions 71-80)
 *   - Tick rate and resolution
 *   - Software timers
 *   - Delays and timeouts
 *   - Watchdog timers
 *
 * SECTION 8: INTERRUPTS & ISR (Questions 81-95)
 *   - ISR design principles
 *   - Deferred interrupt handling
 *   - Interrupt latency
 *   - Nested interrupts
 *
 * SECTION 9: FREERTOS vs ZEPHYR (Questions 96-110)
 *   - Architecture differences
 *   - API comparison
 *   - When to use each
 *   - Migration considerations
 *
 * SECTION 10: ADVANCED & DEBUGGING (Questions 111-120)
 *   - System design patterns
 *   - Performance optimization
 *   - Debugging techniques
 *   - Common pitfalls
 *
 * ============================================================================
 * HOW TO USE THIS GUIDE
 * ============================================================================
 *
 * Each question file contains:
 * 1. THE QUESTION - Exactly as an interviewer might ask
 * 2. QUICK ANSWER - 30-second elevator pitch
 * 3. DETAILED EXPLANATION - In-depth technical answer
 * 4. VISUALIZATION - ASCII diagrams where helpful
 * 5. CODE EXAMPLES - FreeRTOS and Zephyr where applicable
 * 6. INTERVIEW TIPS - What interviewers are looking for
 * 7. FOLLOW-UP QUESTIONS - Common follow-ups to prepare for
 *
 * ============================================================================
 * RTOS COMPARISON QUICK REFERENCE
 * ============================================================================
 *
 * +------------------+----------------------+------------------------+
 * | Feature          | FreeRTOS             | Zephyr RTOS            |
 * +------------------+----------------------+------------------------+
 * | License          | MIT                  | Apache 2.0             |
 * | Footprint        | ~6-12KB ROM          | ~8KB+ ROM (minimal)    |
 * | Task API         | xTaskCreate()        | k_thread_create()      |
 * | Mutex API        | xSemaphoreCreateMutex| k_mutex_init()         |
 * | Queue API        | xQueueCreate()       | k_msgq_init()          |
 * | Timer API        | xTimerCreate()       | k_timer_init()         |
 * | Config System    | FreeRTOSConfig.h     | Kconfig/devicetree     |
 * | Build System     | CMake/Make           | West/CMake             |
 * | Certification    | SAFERTOS available   | Safety certification   |
 * | Ecosystem        | Massive, mature      | Growing, modern        |
 * +------------------+----------------------+------------------------+
 *
 * ============================================================================
 * KEY CONCEPTS QUICK REFERENCE
 * ============================================================================
 *
 * TASK STATES (Common to most RTOS):
 * ---------------------------------
 *                    +----------+
 *        create ---->| READY    |<---- preempted
 *                    +----------+
 *                         |
 *                    scheduled
 *                         v
 *                    +----------+
 *                    | RUNNING  |
 *                    +----------+
 *                    /    |    \
 *            blocked/  yield  \delete
 *                  v      v      v
 *           +--------+ +-----+ +------+
 *           |BLOCKED | |READY| |DEAD  |
 *           +--------+ +-----+ +------+
 *
 * PRIORITY INVERSION:
 * ---------------------------------
 *   High priority task blocked by low priority task holding a resource
 *
 *   Time --->
 *   HIGH:  [BLOCKED.............][RUN]
 *   MED:         [RUN][RUN][RUN]
 *   LOW:   [RUN]                      [RUN]
 *          ^                          ^
 *          LOW takes mutex            LOW releases mutex
 *
 *   Solution: Priority Inheritance - LOW temporarily gets HIGH's priority
 *
 * MUTEX vs SEMAPHORE:
 * ---------------------------------
 *   MUTEX:
 *   - Binary (0 or 1)
 *   - Ownership concept (only owner can release)
 *   - Used for mutual exclusion
 *   - Supports priority inheritance
 *
 *   SEMAPHORE:
 *   - Can be binary or counting
 *   - No ownership (anyone can signal)
 *   - Used for signaling/synchronization
 *   - No priority inheritance
 *
 * ============================================================================
 */

// This file serves as an index - see individual question files for details
// File naming convention: XX_topic_name.c where XX is the question number

#include <stdio.h>

int main() {
    printf("=== RTOS Interview Preparation Guide ===\n\n");
    printf("120 comprehensive questions covering:\n");
    printf("- RTOS Fundamentals\n");
    printf("- Task Management & Scheduling\n");
    printf("- Synchronization (Mutex, Semaphore)\n");
    printf("- Inter-Task Communication\n");
    printf("- Memory & Timing\n");
    printf("- Interrupts & ISR\n");
    printf("- FreeRTOS vs Zephyr\n");
    printf("- Advanced Topics & Debugging\n\n");
    printf("See individual files for detailed explanations.\n");
    return 0;
}
