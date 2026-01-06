/*
 * ============================================================================
 * QUESTION 26: How do you decide Task Priorities?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * Proper priority assignment is crucial for real-time system correctness.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Priority assignment depends on deadlines and criticality. For Rate Monotonic
 * Scheduling, shorter period tasks get higher priority. For safety-critical
 * systems, safety tasks are highest regardless of period. I categorize tasks
 * into levels: Safety/Emergency (highest), Hard real-time (time-critical),
 * Soft real-time (important but flexible), Background (lowest). Within
 * each category, use RMS or deadline-based ordering. Important: avoid too
 * many priority levels - typically 3-8 distinct levels are manageable.
 * Also leave gaps for future tasks."
 *
 * ============================================================================
 * VISUALIZATION: Priority Levels
 * ============================================================================
 *
 *   TYPICAL PRIORITY HIERARCHY:
 *
 *   Priority
 *      ^
 *   MAX |  [SAFETY] - Watchdog, Emergency Stop
 *      |
 *      |  [HARD RT] - Motor Control, Sensor Sampling
 *      |
 *      |  [COMM] - Protocol Stack, UART/CAN
 *      |
 *      |  [SOFT RT] - Display Update, LED Blink
 *      |
 *      |  [BACKGROUND] - Logging, Self-Test
 *      |
 *   0  |  [IDLE] - RTOS Idle Task
 *      +---------------------------------------->
 *
 *
 *   FREERTOS PRIORITY NUMBERING:
 *
 *   configMAX_PRIORITIES = 7 (0 to 6)
 *
 *   6 - Highest ████████ Safety/Critical
 *   5 -         ███████  Hard Real-Time
 *   4 -         ██████   Communication
 *   3 -         █████    Soft Real-Time
 *   2 -         ████     Normal
 *   1 -         ███      Low
 *   0 - Idle    ██       Idle Task (reserved)
 *
 *
 *   ZEPHYR PRIORITY NUMBERING (INVERTED!):
 *
 *   -16 - Highest ████████ Most Critical (negative = cooperative)
 *    0 -          ███████  High Priority
 *    1 -          ██████   Medium-High
 *    5 -          █████    Medium
 *   10 -          ████     Normal
 *   14 -          ███      Low
 *   15 - Lowest   ██       Lowest Preemptible
 *
 * ============================================================================
 * PRIORITY ASSIGNMENT METHODS
 * ============================================================================
 *
 * 1. RATE MONOTONIC SCHEDULING (RMS)
 * -----------------------------------
 * Rule: Shorter period = Higher priority
 *
 *   Task        Period      Priority
 *   ---------------------------------
 *   MotorCtrl   1ms         Highest
 *   Sensor      10ms        High
 *   Display     100ms       Medium
 *   Logging     1000ms      Low
 *
 *   Proven optimal for fixed-priority with D=T
 *
 *
 * 2. DEADLINE MONOTONIC SCHEDULING (DMS)
 * ---------------------------------------
 * Rule: Shorter deadline = Higher priority
 *
 *   Task        Period   Deadline   Priority
 *   ------------------------------------------
 *   Alarm       100ms    10ms       Highest (tight deadline)
 *   Control     10ms     10ms       High
 *   Display     100ms    100ms      Medium
 *
 *   Better than RMS when D < T
 *
 *
 * 3. CRITICALITY-BASED
 * --------------------
 * Rule: More critical = Higher priority (regardless of timing)
 *
 *   Safety-Critical > Hard Real-Time > Soft Real-Time > Background
 *
 *   Used when safety is more important than timing
 *
 *
 * 4. HYBRID APPROACH (Most Common)
 * --------------------------------
 * Combine criticality levels with RMS within each level
 *
 * ============================================================================
 * CODE EXAMPLE: Priority Definition
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "task.h"

// ============================================================================
// METHOD 1: Define Priority Levels (Recommended)
// ============================================================================

// Define priority levels as base values
#define PRIORITY_IDLE           0   // Reserved for idle task
#define PRIORITY_BACKGROUND     1
#define PRIORITY_LOW            2
#define PRIORITY_NORMAL         3
#define PRIORITY_HIGH           4
#define PRIORITY_REALTIME       5
#define PRIORITY_CRITICAL       (configMAX_PRIORITIES - 1)

// Task-specific priorities within levels
#define PRIORITY_LOGGING        PRIORITY_BACKGROUND
#define PRIORITY_DISPLAY        PRIORITY_LOW
#define PRIORITY_COMM           PRIORITY_NORMAL
#define PRIORITY_SENSOR         PRIORITY_HIGH
#define PRIORITY_MOTOR_CTRL     PRIORITY_REALTIME
#define PRIORITY_WATCHDOG       PRIORITY_CRITICAL
#define PRIORITY_EMERGENCY      PRIORITY_CRITICAL

// Create tasks
void create_system_tasks(void) {
    xTaskCreate(motor_control_task, "Motor",    256, NULL, PRIORITY_MOTOR_CTRL, NULL);
    xTaskCreate(sensor_task,        "Sensor",   256, NULL, PRIORITY_SENSOR,     NULL);
    xTaskCreate(comm_task,          "Comm",     512, NULL, PRIORITY_COMM,       NULL);
    xTaskCreate(display_task,       "Display",  256, NULL, PRIORITY_DISPLAY,    NULL);
    xTaskCreate(logging_task,       "Log",      512, NULL, PRIORITY_LOGGING,    NULL);
    xTaskCreate(watchdog_task,      "WDog",     128, NULL, PRIORITY_WATCHDOG,   NULL);
}


// ============================================================================
// METHOD 2: Calculate from Period (RMS)
// ============================================================================

// Calculate priority based on period
// Shorter period = higher priority
#define PERIOD_TO_PRIORITY(period_ms) \
    ((period_ms < 10) ? PRIORITY_REALTIME : \
     (period_ms < 50) ? PRIORITY_HIGH : \
     (period_ms < 200) ? PRIORITY_NORMAL : \
     (period_ms < 1000) ? PRIORITY_LOW : PRIORITY_BACKGROUND)

typedef struct {
    TaskFunction_t func;
    const char *name;
    uint16_t stack_size;
    uint32_t period_ms;
    uint8_t priority;  // Calculated
} TaskConfig_t;

TaskConfig_t task_configs[] = {
    {motor_ctrl_task, "Motor",   256,   1,    0},  // Priority TBD
    {sensor_task,     "Sensor",  256,   10,   0},
    {display_task,    "Display", 256,   100,  0},
    {logging_task,    "Log",     512,   1000, 0},
};

void create_tasks_from_config(void) {
    for (int i = 0; i < sizeof(task_configs)/sizeof(task_configs[0]); i++) {
        // Calculate priority from period (RMS)
        task_configs[i].priority = PERIOD_TO_PRIORITY(task_configs[i].period_ms);

        xTaskCreate(
            task_configs[i].func,
            task_configs[i].name,
            task_configs[i].stack_size,
            &task_configs[i],  // Pass config as parameter
            task_configs[i].priority,
            NULL
        );
    }
}
*/

/*
 * ============================================================================
 * ZEPHYR PRIORITY EXAMPLE
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>

// Zephyr: Lower number = Higher priority!
// Negative priorities are cooperative (non-preemptible)

#define PRIO_CRITICAL   -1   // Cooperative, highest
#define PRIO_REALTIME    0   // Preemptible, very high
#define PRIO_HIGH        2
#define PRIO_NORMAL      5
#define PRIO_LOW        10
#define PRIO_BACKGROUND 14   // Lowest preemptible

K_THREAD_DEFINE(motor_tid,  512, motor_thread,   NULL, NULL, NULL, PRIO_REALTIME, 0, 0);
K_THREAD_DEFINE(sensor_tid, 512, sensor_thread,  NULL, NULL, NULL, PRIO_HIGH,     0, 0);
K_THREAD_DEFINE(comm_tid,   1024, comm_thread,   NULL, NULL, NULL, PRIO_NORMAL,   0, 0);
K_THREAD_DEFINE(log_tid,    512, logging_thread, NULL, NULL, NULL, PRIO_BACKGROUND, 0, 0);
*/

/*
 * ============================================================================
 * COMMON PRIORITY PATTERNS
 * ============================================================================
 *
 * PATTERN 1: Layered System
 * --------------------------
 *
 *   +------------------+
 *   | Application      |  <- Priority 3-4
 *   +------------------+
 *   | Protocol Stack   |  <- Priority 4-5
 *   +------------------+
 *   | Device Drivers   |  <- Priority 5-6
 *   +------------------+
 *   | Hardware (ISRs)  |  <- Interrupt Priority
 *   +------------------+
 *
 *
 * PATTERN 2: Producer-Consumer
 * ----------------------------
 *
 *   Producer (High Prio) ---Queue---> Consumer (Lower Prio)
 *
 *   Producer should be higher to not lose data.
 *   Or use sufficiently large queue buffer.
 *
 *
 * PATTERN 3: Control Loop
 * -----------------------
 *
 *   Sensor Read (Highest) -> Control Calc (High) -> Actuator (High)
 *
 *   All parts of control loop at same high priority,
 *   or sensor slightly higher to ensure fresh data.
 *
 *
 * PATTERN 4: GUI Application
 * --------------------------
 *
 *   Event Handler (High)      - Responsive to user
 *   Screen Update (Medium)    - Visual feedback
 *   Animation (Low)           - Nice but not critical
 *   Analytics (Background)    - Can wait
 *
 * ============================================================================
 * PRIORITY ASSIGNMENT CHECKLIST
 * ============================================================================
 *
 * 1. IDENTIFY HARD DEADLINES
 *    - What happens if deadline is missed?
 *    - Catastrophic = highest priority
 *
 * 2. IDENTIFY TIMING REQUIREMENTS
 *    - Period and deadline of each task
 *    - Apply RMS for periodic tasks
 *
 * 3. CONSIDER DEPENDENCIES
 *    - Task A depends on Task B's output?
 *    - B should generally be higher priority
 *
 * 4. AVOID PRIORITY INVERSION
 *    - If low-priority task holds resource high-priority needs
 *    - Use mutex with priority inheritance
 *
 * 5. LEAVE GAPS
 *    - Don't use consecutive priorities
 *    - Leave room for future tasks
 *
 * 6. DOCUMENT DECISIONS
 *    - Why each task has its priority
 *    - Makes maintenance easier
 *
 * ============================================================================
 * COMMON MISTAKES
 * ============================================================================
 *
 * MISTAKE 1: All tasks at same priority
 *
 *   // BAD: No differentiation
 *   xTaskCreate(motor_task, "Motor", 256, NULL, 3, NULL);
 *   xTaskCreate(log_task,   "Log",   256, NULL, 3, NULL);
 *
 *   Result: Round-robin, logging delays motor control!
 *
 *
 * MISTAKE 2: Communication lower than application
 *
 *   // BAD: App can't communicate!
 *   xTaskCreate(app_task,  "App",  256, NULL, 5, NULL);
 *   xTaskCreate(comm_task, "Comm", 256, NULL, 2, NULL);
 *
 *   If app is always ready, comm never runs!
 *
 *
 * MISTAKE 3: Too many priority levels
 *
 *   // BAD: 20 different priorities hard to manage
 *   #define PRIORITY_TASK_A  17
 *   #define PRIORITY_TASK_B  18
 *   #define PRIORITY_TASK_C  19
 *
 *   Keep to 4-8 distinct levels
 *
 *
 * MISTAKE 4: Not considering blocking
 *
 *   High-priority task that blocks indefinitely
 *   is effectively lower priority during that time.
 *   But when it unblocks, it preempts immediately.
 *
 *
 * MISTAKE 5: Forgetting ISR priority
 *
 *   ISRs are separate from task priorities!
 *   Even lowest-priority ISR preempts highest-priority task.
 *
 * ============================================================================
 * PRIORITY INVERSION REVIEW
 * ============================================================================
 *
 *   Classic Problem:
 *
 *   Task H (High) waits for mutex held by Task L (Low)
 *   Task M (Medium) runs and prevents L from releasing mutex
 *   H waits for M even though H > M priority!
 *
 *
 *   Solution: Priority Inheritance
 *
 *   When H waits for L's mutex, L inherits H's priority
 *   L runs at high priority, releases mutex
 *   H continues
 *
 *
 *   FreeRTOS: Use xSemaphoreCreateMutex(), not binary semaphore!
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. MENTION RMS
 *    "For periodic tasks, shorter period gets higher priority"
 *
 * 2. DISCUSS CRITICALITY
 *    "Safety-critical tasks highest, regardless of period"
 *
 * 3. EXPLAIN LAYERING
 *    "I use priority bands: critical, real-time, normal, background"
 *
 * 4. ADDRESS INVERSION
 *    "Use mutex with priority inheritance for shared resources"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "What if two tasks have same deadline and period?"
 * A: Either can be higher. If they share data, consider which produces
 *    and which consumes - producer typically higher.
 *
 * Q: "How do you handle aperiodic tasks?"
 * A: Assign priority based on deadline urgency. Or use server tasks
 *    like Sporadic Server to bound aperiodic task interference.
 *
 * Q: "Can priority be changed at runtime?"
 * A: Yes, vTaskPrioritySet() in FreeRTOS. Useful for implementing
 *    priority inheritance manually or dynamic scheduling.
 *
 * Q: "What's the idle task's priority?"
 * A: Always lowest (0 in FreeRTOS). Runs only when no other task ready.
 *    Good place for power management, watchdog feeding.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 26 ===\n\n");
    printf("Q: How do you decide Task Priorities?\n\n");
    printf("METHODS:\n\n");
    printf("1. RATE MONOTONIC (RMS):\n");
    printf("   - Shorter period = Higher priority\n");
    printf("   - Optimal for fixed-priority, D=T\n\n");
    printf("2. DEADLINE MONOTONIC (DMS):\n");
    printf("   - Shorter deadline = Higher priority\n");
    printf("   - Better when D < T\n\n");
    printf("3. CRITICALITY-BASED:\n");
    printf("   - Safety > Hard RT > Soft RT > Background\n\n");
    printf("TYPICAL HIERARCHY:\n");
    printf("   MAX: Safety/Emergency (Watchdog)\n");
    printf("       Hard Real-Time (Motor Control)\n");
    printf("       Communication (Protocol Stack)\n");
    printf("       Soft Real-Time (Display)\n");
    printf("    0: Background/Idle (Logging)\n\n");
    printf("BEST PRACTICES:\n");
    printf("- Use 4-8 distinct priority levels\n");
    printf("- Leave gaps for future tasks\n");
    printf("- Document priority decisions\n");
    printf("- Use mutex for priority inheritance\n");
    return 0;
}
