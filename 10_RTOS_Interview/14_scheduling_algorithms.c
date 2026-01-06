/*
 * ============================================================================
 * QUESTION 14: What scheduling algorithms are used in RTOS?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: High | IMPORTANCE: High
 *
 * Understanding scheduling algorithms shows deeper RTOS knowledge.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Most RTOS use PRIORITY-BASED PREEMPTIVE scheduling - the highest priority
 * ready task always runs. Within the same priority, ROUND-ROBIN time slicing
 * is common. For theoretical analysis, RATE MONOTONIC assigns priorities based
 * on period (shorter period = higher priority), and EARLIEST DEADLINE FIRST
 * schedules based on deadlines. FreeRTOS and Zephyr both use fixed-priority
 * preemptive scheduling with optional round-robin. The scheduler runs in O(1)
 * time complexity."
 *
 * ============================================================================
 * SCHEDULING ALGORITHMS OVERVIEW
 * ============================================================================
 *
 * 1. FIXED-PRIORITY PREEMPTIVE (Most Common in RTOS)
 *    - Each task has a fixed priority
 *    - Highest priority ready task always runs
 *    - Used by: FreeRTOS, Zephyr, VxWorks, most commercial RTOS
 *
 * 2. ROUND-ROBIN (Within Same Priority)
 *    - Tasks of equal priority share CPU time
 *    - Each gets a "time slice" before next task runs
 *    - Ensures fairness among same-priority tasks
 *
 * 3. RATE MONOTONIC SCHEDULING (RMS)
 *    - Theoretical algorithm for periodic tasks
 *    - Shorter period = higher priority
 *    - Optimal for fixed-priority systems
 *
 * 4. EARLIEST DEADLINE FIRST (EDF)
 *    - Dynamic priority based on deadline
 *    - Task closest to deadline runs first
 *    - Higher CPU utilization possible, but more overhead
 *
 * ============================================================================
 * VISUALIZATION: Fixed-Priority Preemptive
 * ============================================================================
 *
 * Tasks: A (Pri 3), B (Pri 2), C (Pri 1)   [Higher = More Important]
 *
 *   Time --->
 *
 *   Task A (3): [====]............[====]........
 *   Task B (2): ......[==].[==]........[======]
 *   Task C (1): ..........X............X........
 *                         ^            ^
 *                         |            |
 *                    C can't run   C can't run
 *                    (A or B ready) (A running)
 *
 * RULE: Highest priority READY task always runs!
 *
 * ============================================================================
 * VISUALIZATION: Round-Robin (Same Priority)
 * ============================================================================
 *
 * Tasks: A, B, C (all Priority 2)
 * Time slice: 10ms
 *
 *   Time --->  0    10   20   30   40   50   60   70   80   90
 *              |----|----|----|----|----|----|----|----|----|----|
 *
 *   Task A:    [====]          [====]          [====]
 *   Task B:         [====]          [====]          [====]
 *   Task C:              [====]          [====]          [====]
 *                   ^    ^    ^
 *                   |    |    |
 *              Time slice boundaries
 *
 * Each task gets equal CPU time when same priority.
 *
 * ============================================================================
 * RATE MONOTONIC SCHEDULING (RMS)
 * ============================================================================
 *
 * RULE: Shorter period → Higher priority
 *
 * Tasks:
 *   Task A: Period = 10ms, Execution = 2ms  → Priority: HIGHEST
 *   Task B: Period = 20ms, Execution = 3ms  → Priority: MEDIUM
 *   Task C: Period = 50ms, Execution = 5ms  → Priority: LOWEST
 *
 *   Time --->
 *   0    5   10   15   20   25   30   35   40   45   50
 *   |----|----|----|----|----|----|----|----|----|----|----|
 *
 *   Task A: [A]      [A]      [A]      [A]      [A]      (every 10ms)
 *   Task B:    [BB]           [BB]           [BB]        (every 20ms)
 *   Task C:       [CCCCC]                         [CCCCC](every 50ms)
 *
 *
 * SCHEDULABILITY TEST:
 *
 *   CPU Utilization = Σ (Ci / Ti)
 *
 *   U = 2/10 + 3/20 + 5/50 = 0.2 + 0.15 + 0.1 = 0.45 (45%)
 *
 *   RMS bound for n tasks: U ≤ n(2^(1/n) - 1)
 *   For 3 tasks: U ≤ 3(2^(1/3) - 1) ≈ 0.78 (78%)
 *
 *   0.45 < 0.78 → SCHEDULABLE ✓
 *
 * ============================================================================
 * EARLIEST DEADLINE FIRST (EDF)
 * ============================================================================
 *
 * RULE: Task with nearest deadline runs first (dynamic priority)
 *
 * Tasks:
 *   Task A: Period = 10ms, Deadline = 10ms
 *   Task B: Period = 20ms, Deadline = 20ms
 *
 *   At time 0:
 *     A's deadline: 10ms
 *     B's deadline: 20ms
 *     → A runs first (deadline sooner)
 *
 *   At time 10 (A completes, new instance):
 *     A's deadline: 20ms
 *     B's deadline: 20ms (same!)
 *     → Either can run (tie)
 *
 *
 * ADVANTAGE: Can achieve 100% CPU utilization (vs ~69% for RMS)
 * DISADVANTAGE: More overhead (recompute priorities), harder to analyze
 *
 * ============================================================================
 * FREERTOS SCHEDULER DETAILS
 * ============================================================================
 *
 * DATA STRUCTURE: Array of ready lists (one per priority level)
 *
 *   Priority    Ready List
 *   +-------+   +------+------+------+
 *   |   7   |-->| TaskA| TaskD|      |
 *   +-------+   +------+------+------+
 *   |   6   |-->|      |      |      |
 *   +-------+   +------+------+------+
 *   |   5   |-->| TaskB|      |      |
 *   +-------+   +------+------+------+
 *   |   4   |-->|      |      |      |
 *   +-------+   +------+------+------+
 *   |   3   |-->| TaskC| TaskE| TaskF|
 *   +-------+   +------+------+------+
 *   |   2   |-->|      |      |      |
 *   +-------+   +------+------+------+
 *   |   1   |-->| TaskG|      |      |
 *   +-------+   +------+------+------+
 *   |   0   |-->| Idle |      |      |  ← Always ready
 *   +-------+   +------+------+------+
 *
 * SCHEDULER ALGORITHM:
 * 1. Find highest priority non-empty list: O(1) with uxTopReadyPriority
 * 2. Get first task from that list: O(1)
 * 3. Round-robin: rotate list after time slice
 *
 * TOTAL: O(1) - constant time regardless of task count!
 *
 * ============================================================================
 * CONFIGURATION
 * ============================================================================
 */

#include <stdio.h>

/*
// FreeRTOS Configuration

// Enable preemption
#define configUSE_PREEMPTION            1

// Enable time slicing (round-robin) for same priority
#define configUSE_TIME_SLICING          1

// Number of priority levels
#define configMAX_PRIORITIES            7

// Tick rate (affects time slice)
#define configTICK_RATE_HZ              1000


// Zephyr Configuration (prj.conf)

// Enable preemption
CONFIG_PREEMPT_ENABLED=y

// Number of preemptive priorities
CONFIG_NUM_PREEMPT_PRIORITIES=15

// Cooperative priorities (negative)
CONFIG_NUM_COOP_PRIORITIES=16

// Time slicing
CONFIG_TIMESLICING=y
CONFIG_TIMESLICE_SIZE=10        # 10ms time slice
CONFIG_TIMESLICE_PRIORITY=0     # Apply to all priorities >= 0
*/

/*
 * ============================================================================
 * COOPERATIVE vs PREEMPTIVE PRIORITIES (Zephyr)
 * ============================================================================
 *
 * Zephyr unique feature: Negative priorities are COOPERATIVE
 *
 *   Priority -3: Cooperative (highest coop)
 *   Priority -2: Cooperative
 *   Priority -1: Cooperative (lowest coop)
 *   Priority  0: Preemptive (highest preempt)
 *   Priority  1: Preemptive
 *   ...
 *   Priority 14: Preemptive (lowest preempt)
 *
 * Cooperative threads:
 * - Not preempted by other threads
 * - Must explicitly yield (k_yield())
 * - Useful for: interrupt handlers that become threads, critical sequences
 *
 * ============================================================================
 * IDLE TASK
 * ============================================================================
 *
 * Special task that runs when no other task is ready.
 *
 * Purpose:
 * 1. Keep CPU busy (never truly idle)
 * 2. Housekeeping (FreeRTOS: free deleted task memory)
 * 3. Power management (enter low-power mode)
 *
 * FreeRTOS:
 *   - Created automatically at priority 0 (lowest)
 *   - Can hook into it: vApplicationIdleHook()
 *
 * Zephyr:
 *   - System idle thread
 *   - Triggers power management: pm_system_suspend()
 *
 * ============================================================================
 * CHOOSING PRIORITIES
 * ============================================================================
 *
 * GUIDELINES:
 *
 * 1. CRITICAL/SAFETY: Highest priority
 *    - Emergency stop, safety monitoring
 *
 * 2. REAL-TIME I/O: High priority
 *    - Sensor reading, motor control
 *    - Short, frequent tasks
 *
 * 3. COMMUNICATION: Medium priority
 *    - Protocol handling, networking
 *
 * 4. USER INTERFACE: Medium-low priority
 *    - Display updates, button handling
 *
 * 5. BACKGROUND/LOGGING: Low priority
 *    - Data logging, diagnostics
 *    - Non-time-critical
 *
 * 6. IDLE TASKS: Lowest priority
 *    - Housekeeping, power management
 *
 *
 * EXAMPLE SYSTEM:
 *
 *   Priority 7: Emergency shutdown handler
 *   Priority 6: Motor control (1kHz)
 *   Priority 5: Sensor reading (100Hz)
 *   Priority 4: Communication (Modbus/CAN)
 *   Priority 3: User input handling
 *   Priority 2: Display update
 *   Priority 1: Data logging
 *   Priority 0: Idle (power management)
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. START WITH BASICS
 *    "Fixed-priority preemptive with round-robin for same priority"
 *
 * 2. MENTION RMS IF ASKED ABOUT THEORY
 *    "Rate Monotonic assigns priority based on period"
 *
 * 3. KNOW O(1) COMPLEXITY
 *    "FreeRTOS scheduler is O(1), constant time"
 *
 * 4. DISCUSS ZEPHYR COOPERATIVE
 *    "Zephyr has cooperative threads with negative priorities"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "What happens if two tasks have the same priority?"
 * A: Round-robin time slicing (if enabled). Each gets a time slice,
 *    then switches to the next. Without time slicing, first one runs
 *    until it blocks.
 *
 * Q: "Why is O(1) scheduler important?"
 * A: Predictable timing. Scheduler time doesn't depend on number of
 *    tasks - critical for real-time systems where worst-case matters.
 *
 * Q: "What's the advantage of EDF over RMS?"
 * A: EDF can achieve 100% CPU utilization (theoretically), RMS tops
 *    out around 69%. But EDF has more runtime overhead and is harder
 *    to analyze.
 *
 * Q: "How do you implement RMS in FreeRTOS?"
 * A: Manually assign priorities based on task periods. Shortest period
 *    gets highest priority. FreeRTOS doesn't do this automatically.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 14 ===\n\n");
    printf("Q: What scheduling algorithms are used in RTOS?\n\n");
    printf("MAIN ALGORITHMS:\n\n");
    printf("1. FIXED-PRIORITY PREEMPTIVE (Most Common)\n");
    printf("   - Highest priority ready task always runs\n");
    printf("   - FreeRTOS, Zephyr, VxWorks use this\n\n");
    printf("2. ROUND-ROBIN (Same Priority)\n");
    printf("   - Time slicing among equal priority tasks\n");
    printf("   - configUSE_TIME_SLICING in FreeRTOS\n\n");
    printf("3. RATE MONOTONIC (RMS)\n");
    printf("   - Shorter period = higher priority\n");
    printf("   - Theoretical, for analysis\n\n");
    printf("4. EARLIEST DEADLINE FIRST (EDF)\n");
    printf("   - Nearest deadline runs first\n");
    printf("   - Dynamic priority, higher utilization\n\n");
    printf("FreeRTOS SCHEDULER:\n");
    printf("- O(1) complexity (constant time)\n");
    printf("- Array of ready lists per priority\n");
    printf("- uxTopReadyPriority optimization\n");
    return 0;
}
