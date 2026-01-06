/*
 * ============================================================================
 * QUESTION 22: How do you analyze Real-Time Performance? (WCET, RMS, EDF)
 * ============================================================================
 *
 * DIFFICULTY: Hard | FREQUENCY: Medium | IMPORTANCE: Critical
 *
 * Understanding schedulability analysis is crucial for real-time systems.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Real-time analysis determines if a system can meet all its deadlines.
 * Key concepts: WCET (Worst-Case Execution Time) - maximum time a task takes.
 * CPU Utilization = sum of (WCET/Period) for all tasks. For Rate Monotonic
 * Scheduling (RMS), utilization must be under ~69% (Liu-Layland bound) to
 * guarantee schedulability. For EDF (Earliest Deadline First), can use up
 * to 100% theoretically. Response time analysis is more accurate than
 * utilization bounds but more complex. Always measure WCET empirically and
 * add safety margin."
 *
 * ============================================================================
 * VISUALIZATION: Timing Parameters
 * ============================================================================
 *
 *   TASK TIMING PARAMETERS:
 *
 *   |<--------------- Period (T) ----------------->|
 *   |                                              |
 *   v                                              v
 *   +----+                                         +----+
 *   | Ci |                                         | Ci |  <- Next instance
 *   +----+                                         +----+
 *   ^    ^                              ^
 *   |    |                              |
 *   Release                         Deadline (D)
 *
 *   Ci = Worst-Case Execution Time (WCET)
 *   T  = Period (time between releases)
 *   D  = Deadline (must complete by this time)
 *
 *   Often D = T (deadline equals period)
 *
 *
 *   TIMING DIAGRAM EXAMPLE:
 *
 *   Task A: Period=10ms, WCET=2ms
 *   Task B: Period=20ms, WCET=5ms
 *
 *   Time:   0    5    10   15   20   25   30
 *           |    |    |    |    |    |    |
 *   Task A: [AA]      [AA]      [AA]      [AA]
 *   Task B: [BBBBB]        [BBBBB]        [BBBBB]
 *
 *   CPU:    [AA][BB][BB][AA][B][AA][BB][B]
 *              ^                    ^
 *              |                    |
 *           B preempted by A    B continues
 *
 * ============================================================================
 * CPU UTILIZATION CALCULATION
 * ============================================================================
 *
 *   FORMULA:
 *
 *   U = Σ (Ci / Ti) for all tasks i
 *
 *   Where:
 *   - Ci = WCET of task i
 *   - Ti = Period of task i
 *
 *
 *   EXAMPLE:
 *
 *   Task A: Period=10ms, WCET=2ms  -> U_A = 2/10 = 0.20
 *   Task B: Period=20ms, WCET=5ms  -> U_B = 5/20 = 0.25
 *   Task C: Period=50ms, WCET=10ms -> U_C = 10/50 = 0.20
 *
 *   Total U = 0.20 + 0.25 + 0.20 = 0.65 (65%)
 *
 *   Is this schedulable? Depends on scheduling algorithm!
 *
 * ============================================================================
 * RATE MONOTONIC SCHEDULING (RMS) ANALYSIS
 * ============================================================================
 *
 *   RMS Rule: Higher frequency (shorter period) = Higher priority
 *
 *
 *   LIU-LAYLAND BOUND (Sufficient but not necessary):
 *
 *   U ≤ n(2^(1/n) - 1)
 *
 *   Where n = number of tasks
 *
 *   n=1: U ≤ 1.000 (100%)
 *   n=2: U ≤ 0.828 (82.8%)
 *   n=3: U ≤ 0.780 (78.0%)
 *   n=4: U ≤ 0.757 (75.7%)
 *   n=5: U ≤ 0.743 (74.3%)
 *   n→∞: U ≤ 0.693 (69.3%)  <- Worst case bound
 *
 *
 *   For our example:
 *   - n=3 tasks, U=65%
 *   - Bound for n=3 is 78%
 *   - 65% < 78%  ✓ SCHEDULABLE!
 *
 *
 *   IMPORTANT: If U > bound, doesn't mean NOT schedulable!
 *   Just means we need more detailed analysis (Response Time Analysis).
 *
 * ============================================================================
 * RESPONSE TIME ANALYSIS (More Accurate)
 * ============================================================================
 *
 *   FORMULA (for task i with all higher-priority tasks hp(i)):
 *
 *   R_i = C_i + Σ (⌈R_i / T_j⌉ × C_j)  for all j in hp(i)
 *
 *   This is recursive - solve iteratively!
 *
 *
 *   ALGORITHM:
 *
 *   1. Start with R_i = C_i
 *   2. Calculate interference from higher-priority tasks
 *   3. R_i_new = C_i + interference
 *   4. Repeat until R_i converges
 *   5. If R_i ≤ D_i, task is schedulable
 *
 *
 *   EXAMPLE (Task B, lowest priority):
 *
 *   Task A: T=10ms, C=2ms (highest priority)
 *   Task B: T=20ms, C=5ms
 *
 *   Iteration 1: R_B = 5
 *   Interference from A: ⌈5/10⌉ × 2 = 1 × 2 = 2
 *   R_B = 5 + 2 = 7
 *
 *   Iteration 2: R_B = 7
 *   Interference from A: ⌈7/10⌉ × 2 = 1 × 2 = 2
 *   R_B = 5 + 2 = 7  (converged!)
 *
 *   R_B = 7ms ≤ D_B = 20ms  ✓ SCHEDULABLE!
 *
 * ============================================================================
 * EDF ANALYSIS (Earliest Deadline First)
 * ============================================================================
 *
 *   EDF is OPTIMAL for single-processor dynamic scheduling.
 *
 *   SIMPLE TEST:
 *
 *   If U ≤ 1.0 (100%), system is schedulable with EDF!
 *
 *   This is both necessary AND sufficient!
 *
 *
 *   EDF vs RMS:
 *
 *   +------------------+------------------+------------------+
 *   | Aspect           | RMS              | EDF              |
 *   +------------------+------------------+------------------+
 *   | Priority         | Fixed (static)   | Dynamic          |
 *   | Util bound       | ~69% worst case  | 100%             |
 *   | Overhead         | Lower            | Higher           |
 *   | Implementation   | Simpler          | Complex          |
 *   | Overload behav.  | Predictable      | Chaotic          |
 *   +------------------+------------------+------------------+
 *
 * ============================================================================
 * CODE: WCET MEASUREMENT
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
 * ============================================================================
 * WCET MEASUREMENT TECHNIQUES
 * ============================================================================
 */

/*
#include "FreeRTOS.h"
#include "task.h"

// ============================================================================
// METHOD 1: Tick-based measurement (coarse)
// ============================================================================

void measure_wcet_ticks(void) {
    TickType_t start, end, elapsed;

    start = xTaskGetTickCount();

    // Code to measure
    critical_function();

    end = xTaskGetTickCount();
    elapsed = end - start;

    printf("Elapsed: %u ticks (%u ms)\n", elapsed,
           elapsed * portTICK_PERIOD_MS);
}


// ============================================================================
// METHOD 2: Hardware timer (precise)
// ============================================================================

// Using DWT (Data Watchpoint and Trace) cycle counter on ARM Cortex-M
#define DWT_CYCCNT  (*(volatile uint32_t*)0xE0001004)
#define DWT_CTRL    (*(volatile uint32_t*)0xE0001000)
#define SCB_DEMCR   (*(volatile uint32_t*)0xE000EDFC)

void init_cycle_counter(void) {
    SCB_DEMCR |= 0x01000000;  // Enable DWT
    DWT_CYCCNT = 0;           // Reset counter
    DWT_CTRL |= 1;            // Enable counter
}

void measure_wcet_cycles(void) {
    uint32_t start, end, cycles;
    float time_us;

    start = DWT_CYCCNT;

    // Code to measure
    critical_function();

    end = DWT_CYCCNT;
    cycles = end - start;

    // Convert to microseconds (assuming 72MHz clock)
    time_us = (float)cycles / 72.0f;

    printf("Cycles: %lu, Time: %.2f us\n", cycles, time_us);
}


// ============================================================================
// METHOD 3: Track worst case over time
// ============================================================================

static uint32_t wcet_max = 0;
static uint32_t wcet_min = UINT32_MAX;
static uint32_t wcet_count = 0;
static uint64_t wcet_total = 0;

void critical_function_with_wcet_tracking(void) {
    uint32_t start, end, elapsed;

    start = DWT_CYCCNT;

    // === Actual function code ===
    process_sensor_data();
    update_control_output();
    // === End of function ===

    end = DWT_CYCCNT;
    elapsed = end - start;

    // Update statistics
    if (elapsed > wcet_max) wcet_max = elapsed;
    if (elapsed < wcet_min) wcet_min = elapsed;
    wcet_total += elapsed;
    wcet_count++;
}

void print_wcet_stats(void) {
    float avg = (float)wcet_total / wcet_count;
    float cpu_mhz = 72.0f;

    printf("WCET Statistics:\n");
    printf("  Min:  %lu cycles (%.2f us)\n", wcet_min, wcet_min/cpu_mhz);
    printf("  Max:  %lu cycles (%.2f us)\n", wcet_max, wcet_max/cpu_mhz);
    printf("  Avg:  %.0f cycles (%.2f us)\n", avg, avg/cpu_mhz);
    printf("  Count: %lu measurements\n", wcet_count);
}
*/

/*
 * ============================================================================
 * SCHEDULABILITY ANALYSIS CODE
 * ============================================================================
 */

typedef struct {
    const char *name;
    uint32_t period_ms;      // T
    uint32_t wcet_ms;        // C
    uint32_t deadline_ms;    // D (often same as T)
    uint32_t priority;       // For RMS: derived from period
} Task_t;

// Calculate CPU utilization
float calculate_utilization(Task_t *tasks, int num_tasks) {
    float total_u = 0.0f;

    for (int i = 0; i < num_tasks; i++) {
        float u = (float)tasks[i].wcet_ms / tasks[i].period_ms;
        total_u += u;
        printf("  %s: U = %u/%u = %.3f\n",
               tasks[i].name, tasks[i].wcet_ms, tasks[i].period_ms, u);
    }

    return total_u;
}

// Liu-Layland bound
float liu_layland_bound(int n) {
    float bound = n * (pow(2.0, 1.0/n) - 1.0);
    return bound;
}

// Response Time Analysis for one task
uint32_t response_time_analysis(Task_t *tasks, int task_index) {
    Task_t *task = &tasks[task_index];
    uint32_t R = task->wcet_ms;
    uint32_t R_prev;

    // Iterate until convergence
    do {
        R_prev = R;
        R = task->wcet_ms;

        // Add interference from higher priority tasks
        for (int j = 0; j < task_index; j++) {
            uint32_t interference = ((R_prev + tasks[j].period_ms - 1)
                                    / tasks[j].period_ms)
                                    * tasks[j].wcet_ms;
            R += interference;
        }

        // Check for unschedulable
        if (R > task->deadline_ms) {
            return UINT32_MAX;  // Not schedulable
        }

    } while (R != R_prev);

    return R;
}

void analyze_schedulability(Task_t *tasks, int num_tasks) {
    printf("=== Schedulability Analysis ===\n\n");

    // Calculate utilization
    printf("CPU Utilization:\n");
    float U = calculate_utilization(tasks, num_tasks);
    printf("  Total: %.1f%%\n\n", U * 100);

    // Liu-Layland bound for RMS
    float bound = liu_layland_bound(num_tasks);
    printf("Liu-Layland Bound (n=%d): %.1f%%\n", num_tasks, bound * 100);

    if (U <= bound) {
        printf("Result: U < Bound -> DEFINITELY SCHEDULABLE (RMS)\n\n");
    } else if (U <= 1.0) {
        printf("Result: U > Bound but U < 100%%\n");
        printf("        Need Response Time Analysis to confirm\n\n");
    } else {
        printf("Result: U > 100%% -> NOT SCHEDULABLE!\n\n");
        return;
    }

    // Response Time Analysis
    printf("Response Time Analysis:\n");
    int schedulable = 1;

    for (int i = 0; i < num_tasks; i++) {
        uint32_t R = response_time_analysis(tasks, i);

        if (R == UINT32_MAX) {
            printf("  %s: R > D -> NOT SCHEDULABLE!\n", tasks[i].name);
            schedulable = 0;
        } else {
            printf("  %s: R = %u ms (D = %u ms) %s\n",
                   tasks[i].name, R, tasks[i].deadline_ms,
                   R <= tasks[i].deadline_ms ? "OK" : "FAIL");
        }
    }

    printf("\nFinal Result: %s\n",
           schedulable ? "SYSTEM IS SCHEDULABLE" : "SYSTEM IS NOT SCHEDULABLE");
}

/*
 * ============================================================================
 * FACTORS AFFECTING WCET
 * ============================================================================
 *
 * 1. CODE PATH
 *    - Different branches have different timing
 *    - Must analyze WORST path
 *
 * 2. LOOPS
 *    - Maximum iteration count
 *    - Data-dependent loops are dangerous!
 *
 * 3. CACHE
 *    - Cache hit vs miss is 10-100x difference!
 *    - Assume worst-case (all misses) for safety
 *
 * 4. INTERRUPTS
 *    - ISR execution time adds to WCET
 *    - Include nested interrupt overhead
 *
 * 5. MEMORY ACCESS
 *    - Flash wait states
 *    - External memory latency
 *
 * 6. COMPILER OPTIMIZATION
 *    - Different optimization levels = different timing
 *    - Measure with RELEASE build!
 *
 * ============================================================================
 * WCET SAFETY MARGINS
 * ============================================================================
 *
 * Measured WCET is NEVER the true worst case!
 *
 *   Measured Max: 100us
 *   Add margin:   +20-50%
 *   WCET for analysis: 120-150us
 *
 *
 * WHY ADD MARGIN?
 * - Cache behavior varies
 * - Interrupt timing varies
 * - Hardware variations
 * - Measurement doesn't cover all paths
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. DEFINE WCET
 *    "Worst-Case Execution Time - maximum time task can take"
 *
 * 2. EXPLAIN UTILIZATION
 *    "Sum of WCET/Period for all tasks, must be under scheduling bound"
 *
 * 3. KNOW THE BOUNDS
 *    "RMS: ~69% for many tasks, EDF: 100%"
 *
 * 4. MENTION MEASUREMENT
 *    "Use hardware cycle counter, add safety margin"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "What if utilization exceeds Liu-Layland bound?"
 * A: Doesn't mean unschedulable! Use Response Time Analysis for exact check.
 *    Liu-Layland is sufficient but not necessary.
 *
 * Q: "How do you handle variable execution times?"
 * A: Always use WCET for analysis. If execution varies, must assume worst.
 *    Can use profiling over long periods to estimate true WCET.
 *
 * Q: "What about priority inversion in analysis?"
 * A: Must include blocking time from lower-priority tasks holding resources.
 *    B_i = max blocking time added to response time formula.
 *
 * Q: "Why not just use 69% utilization as rule?"
 * A: That's worst case for infinite tasks. For few tasks, bound is higher.
 *    Also, many systems are schedulable above the bound with RTA.
 *
 * ============================================================================
 */

#include <math.h>

int main() {
    printf("=== RTOS Interview Question 22 ===\n\n");
    printf("Q: How do you analyze Real-Time Performance?\n\n");

    // Example task set
    Task_t tasks[] = {
        {"SensorTask",  10, 2, 10, 3},  // Highest priority (shortest period)
        {"ControlTask", 20, 5, 20, 2},
        {"CommTask",    50, 10, 50, 1}  // Lowest priority
    };
    int num_tasks = sizeof(tasks) / sizeof(tasks[0]);

    analyze_schedulability(tasks, num_tasks);

    printf("\n");
    printf("KEY CONCEPTS:\n");
    printf("- WCET: Worst-Case Execution Time\n");
    printf("- CPU Utilization: U = sum(Ci/Ti)\n");
    printf("- RMS Bound: ~69%% for many tasks\n");
    printf("- EDF Bound: 100%%\n");
    printf("- Response Time Analysis for exact check\n");

    return 0;
}
