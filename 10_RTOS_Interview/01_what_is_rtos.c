/*
 * ============================================================================
 * QUESTION 01: What is an RTOS? How is it different from a general-purpose OS?
 * ============================================================================
 *
 * DIFFICULTY: Basic | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * This is often the FIRST question in any embedded systems interview.
 * Your answer sets the tone for the entire interview.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "An RTOS is an operating system designed to process data and events within
 * guaranteed time constraints. Unlike general-purpose OS like Linux or Windows
 * that optimize for throughput and user experience, an RTOS guarantees that
 * critical tasks will complete within specific deadlines. This determinism
 * is essential for embedded systems where missing a deadline could cause
 * system failure - like an airbag that must deploy within milliseconds."
 *
 * ============================================================================
 * DETAILED EXPLANATION
 * ============================================================================
 *
 * RTOS = Real-Time Operating System
 *
 * KEY CHARACTERISTICS OF AN RTOS:
 * ---------------------------------
 *
 * 1. DETERMINISM (Predictability)
 *    - Operations complete within KNOWN, BOUNDED time
 *    - No unpredictable delays from garbage collection, paging, etc.
 *    - Worst-case execution time (WCET) is calculable
 *
 * 2. PREEMPTIVE SCHEDULING
 *    - Higher priority task ALWAYS preempts lower priority task
 *    - No task can "hog" the CPU indefinitely
 *    - Scheduler runs in O(1) time (constant, predictable)
 *
 * 3. MINIMAL LATENCY
 *    - Fast context switch time (microseconds)
 *    - Fast interrupt response time
 *    - Minimal kernel overhead
 *
 * 4. SMALL FOOTPRINT
 *    - Typically 6KB - 100KB ROM
 *    - Runs on microcontrollers with limited resources
 *    - No virtual memory, minimal abstraction layers
 *
 * ============================================================================
 * COMPARISON: RTOS vs General Purpose OS
 * ============================================================================
 *
 * +--------------------+---------------------------+---------------------------+
 * | Aspect             | RTOS (FreeRTOS/Zephyr)    | GPOS (Linux/Windows)      |
 * +--------------------+---------------------------+---------------------------+
 * | Primary Goal       | Meet timing deadlines     | Maximize throughput       |
 * | Scheduling         | Priority-based, O(1)      | Fair scheduling, complex  |
 * | Determinism        | Guaranteed response time  | Best-effort, variable     |
 * | Latency            | Microseconds              | Milliseconds to seconds   |
 * | Memory             | Static/deterministic      | Virtual memory, paging    |
 * | Footprint          | 6KB - 100KB               | Megabytes to Gigabytes    |
 * | Context Switch     | ~1-10 microseconds        | ~1-10 milliseconds        |
 * | Interrupt Latency  | ~1-5 microseconds         | ~10-100+ microseconds     |
 * | User Interface     | None or minimal           | Rich GUI support          |
 * | File System        | Optional, simple          | Complex, full-featured    |
 * | Networking         | Lightweight (lwIP)        | Full TCP/IP stack         |
 * | Target Hardware    | MCUs (Cortex-M, etc.)     | MPUs with MMU             |
 * +--------------------+---------------------------+---------------------------+
 *
 * ============================================================================
 * VISUALIZATION: Why Determinism Matters
 * ============================================================================
 *
 * GENERAL PURPOSE OS (Non-deterministic):
 * ----------------------------------------
 *
 *   Task Response Times (ms):
 *
 *   Run 1: |████████████████| 16ms
 *   Run 2: |████████| 8ms
 *   Run 3: |████████████████████████████████| 32ms  <-- Unpredictable spike!
 *   Run 4: |██████████| 10ms
 *   Run 5: |████████████████████████| 24ms
 *
 *   Average: 18ms, but could be 32ms+ due to:
 *   - Garbage collection
 *   - Page faults
 *   - Other processes
 *   - Driver delays
 *
 * RTOS (Deterministic):
 * ----------------------------------------
 *
 *   Task Response Times (ms):
 *
 *   Run 1: |████████| 8ms
 *   Run 2: |████████| 8ms
 *   Run 3: |█████████| 9ms   <-- Worst case is bounded!
 *   Run 4: |████████| 8ms
 *   Run 5: |████████| 8ms
 *
 *   Guaranteed: Always < 10ms (WCET is known)
 *
 * ============================================================================
 * REAL-WORLD EXAMPLES
 * ============================================================================
 *
 * Where RTOS is REQUIRED:
 * ---------------------------------
 *
 * 1. AUTOMOTIVE
 *    - Airbag deployment: Must respond within 15-30ms of impact
 *    - Anti-lock brakes: 1000 decisions/second
 *    - Engine control: Precise timing for fuel injection
 *
 * 2. MEDICAL DEVICES
 *    - Pacemaker: Beat timing is life-critical
 *    - Insulin pump: Dosing must be precise
 *    - Patient monitors: Alarms must be immediate
 *
 * 3. INDUSTRIAL CONTROL
 *    - Motor control: PWM timing must be exact
 *    - Robot arms: Position updates at fixed intervals
 *    - Safety systems: Emergency stop within microseconds
 *
 * 4. CONSUMER ELECTRONICS
 *    - Drones: Flight control at 400Hz+
 *    - Fitness trackers: Sensor sampling
 *    - Smart home devices: Responsive to commands
 *
 * Where GPOS is preferred:
 * ---------------------------------
 * - Smartphones (user experience > determinism)
 * - Desktop applications
 * - Web servers (throughput matters more)
 * - Database systems
 *
 * ============================================================================
 * TYPES OF REAL-TIME SYSTEMS
 * ============================================================================
 *
 * 1. HARD REAL-TIME
 *    - Missing deadline = CATASTROPHIC FAILURE
 *    - Examples: Airbags, pacemakers, flight control
 *    - Deadline miss is NEVER acceptable
 *
 * 2. FIRM REAL-TIME
 *    - Missing deadline = RESULT IS USELESS (but not catastrophic)
 *    - Examples: Video streaming, financial trading
 *    - Occasional miss is tolerable
 *
 * 3. SOFT REAL-TIME
 *    - Missing deadline = DEGRADED QUALITY
 *    - Examples: UI responsiveness, audio playback
 *    - System continues but user experience suffers
 *
 * ============================================================================
 * CODE EXAMPLE: Why RTOS Matters
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
 * SCENARIO: Motor Control System
 *
 * Without RTOS (bare-metal polling):
 * - Temperature check takes variable time
 * - Motor control timing becomes unpredictable
 * - Risk of motor damage or instability
 */

// BAD: Bare-metal approach with unpredictable timing
void bad_motor_control_loop(void) {
    while (1) {
        // Problem: These operations have VARIABLE execution time
        // check_temperature();      // Could take 1-100ms depending on sensor
        // update_display();         // Could take 10-50ms depending on content
        // read_user_input();        // Could block indefinitely!

        // Motor control is now unpredictable
        // control_motor();          // WHEN does this actually run??
    }
}

/*
 * GOOD: RTOS approach with guaranteed timing
 *
 * FreeRTOS Example:
 */

/*
// Task priorities ensure motor control ALWAYS runs on time
#define MOTOR_TASK_PRIORITY     (configMAX_PRIORITIES - 1)  // Highest
#define TEMP_TASK_PRIORITY      (configMAX_PRIORITIES - 2)
#define DISPLAY_TASK_PRIORITY   (configMAX_PRIORITIES - 3)  // Lowest

void motor_control_task(void *pvParameters) {
    const TickType_t period = pdMS_TO_TICKS(1);  // 1ms period, GUARANTEED
    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        // This ALWAYS runs at exactly 1ms intervals
        // Even if display task takes 50ms, motor task preempts it
        control_motor();

        // Sleep until next period (deterministic timing)
        vTaskDelayUntil(&lastWakeTime, period);
    }
}

void temperature_task(void *pvParameters) {
    for (;;) {
        check_temperature();  // Can take variable time
        vTaskDelay(pdMS_TO_TICKS(100));  // Check every 100ms
    }
}

void display_task(void *pvParameters) {
    for (;;) {
        update_display();  // Can take variable time
        vTaskDelay(pdMS_TO_TICKS(500));  // Update every 500ms
    }
}
*/

/*
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EMPHASIZE DETERMINISM
 *    Don't just say "it's faster" - the key is PREDICTABILITY
 *
 * 2. USE REAL EXAMPLES
 *    Mention specific use cases (airbags, medical devices)
 *
 * 3. KNOW THE TRADEOFFS
 *    RTOS lacks features GPOS has (filesystem, networking, GUI)
 *
 * 4. MENTION BOTH TYPES
 *    Show you know about FreeRTOS (simple) AND Zephyr (feature-rich)
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS TO PREPARE FOR:
 * ============================================================================
 *
 * Q: "What makes something 'real-time'?"
 * A: Correctness depends on BOTH logical result AND timing. A correct answer
 *    delivered late is still a wrong answer.
 *
 * Q: "Can Linux be real-time?"
 * A: Standard Linux is NOT real-time, but PREEMPT_RT patches can make it
 *    "soft" real-time. For hard real-time, use a proper RTOS or Xenomai.
 *
 * Q: "What's the smallest RTOS?"
 * A: FreeRTOS can run in ~6KB ROM. Zephyr minimal is ~8KB.
 *    Some ultra-minimal RTOS like TNKernel are even smaller.
 *
 * Q: "Is Arduino an RTOS?"
 * A: No, Arduino uses a simple loop() approach (bare-metal). But you can
 *    add FreeRTOS to Arduino boards for real-time capabilities.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 01 ===\n\n");
    printf("Q: What is an RTOS and how is it different from GPOS?\n\n");
    printf("Key Points:\n");
    printf("1. RTOS guarantees DETERMINISTIC response times\n");
    printf("2. Priority-based preemptive scheduling\n");
    printf("3. Small footprint (6KB-100KB)\n");
    printf("4. Used where timing is CRITICAL (airbags, medical devices)\n");
    printf("5. GPOS optimizes throughput, RTOS optimizes predictability\n\n");
    printf("Remember: 'Real-time' means 'right answer at the right time'\n");
    return 0;
}
