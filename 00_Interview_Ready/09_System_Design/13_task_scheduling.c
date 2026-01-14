/*
 * ============================================================================
 *        DESIGN: PERIODIC + APERIODIC TASK SCHEDULING
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★☆☆ (MEDIUM)
 *   - Periodic: Sensor sampling, watchdog, LED blink
 *   - Aperiodic: Button press, network events, errors
 *
 * ============================================================================
 *                    TASK TYPES
 * ============================================================================
 *
 * PERIODIC TASKS:
 *   - Execute at fixed intervals
 *   - Examples: Sensor sampling (100Hz), watchdog feed (1Hz)
 *   - Must meet deadlines consistently
 *
 * APERIODIC TASKS:
 *   - Execute on demand (event-triggered)
 *   - Examples: Button press, packet received
 *   - Variable timing, can be sporadic
 *
 * SCHEDULING APPROACHES:
 *
 * 1. SUPER LOOP (No OS):
 *    while(1) {
 *        if (time_for_task1()) task1();
 *        if (time_for_task2()) task2();
 *        if (event_pending()) handle_event();
 *    }
 *
 * 2. COOPERATIVE SCHEDULER:
 *    Tasks yield control voluntarily
 *    Simple, predictable, no preemption
 *
 * 3. PREEMPTIVE SCHEDULER (RTOS):
 *    Higher priority tasks interrupt lower
 *    More complex, requires careful design
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// SIMPLE COOPERATIVE SCHEDULER
// ============================================================================

typedef void (*TaskFunction)(void);

typedef struct {
    const char* name;
    TaskFunction func;
    uint32_t period_ms;         // 0 = aperiodic
    uint32_t last_run;
    uint32_t next_run;
    uint8_t priority;           // Lower = higher priority
    bool enabled;
    bool pending;               // For aperiodic: ready to run

    // Statistics
    uint32_t run_count;
    uint32_t max_duration;
    uint32_t missed_deadlines;
} Task;

#define MAX_TASKS 16

typedef struct {
    Task tasks[MAX_TASKS];
    uint8_t task_count;
    volatile uint32_t tick_count;
    bool running;
} Scheduler;

Scheduler scheduler = {0};

// Simulate system tick
uint32_t get_tick(void) {
    return scheduler.tick_count;
}

// ============================================================================
// TASK REGISTRATION
// ============================================================================

int scheduler_add_periodic(const char* name, TaskFunction func,
                           uint32_t period_ms, uint8_t priority) {
    if (scheduler.task_count >= MAX_TASKS) return -1;

    int id = scheduler.task_count++;
    Task* t = &scheduler.tasks[id];

    t->name = name;
    t->func = func;
    t->period_ms = period_ms;
    t->priority = priority;
    t->enabled = true;
    t->pending = false;
    t->next_run = get_tick() + period_ms;
    t->run_count = 0;
    t->max_duration = 0;
    t->missed_deadlines = 0;

    printf("[SCHED] Added periodic task '%s': period=%u ms, prio=%d\n",
           name, period_ms, priority);
    return id;
}

int scheduler_add_aperiodic(const char* name, TaskFunction func,
                            uint8_t priority) {
    if (scheduler.task_count >= MAX_TASKS) return -1;

    int id = scheduler.task_count++;
    Task* t = &scheduler.tasks[id];

    t->name = name;
    t->func = func;
    t->period_ms = 0;  // Aperiodic
    t->priority = priority;
    t->enabled = true;
    t->pending = false;
    t->run_count = 0;

    printf("[SCHED] Added aperiodic task '%s': prio=%d\n", name, priority);
    return id;
}

void scheduler_trigger(int task_id) {
    if (task_id >= 0 && task_id < scheduler.task_count) {
        scheduler.tasks[task_id].pending = true;
    }
}

// ============================================================================
// SCHEDULER CORE
// ============================================================================

void scheduler_run_once(void) {
    uint32_t now = get_tick();
    Task* ready_task = NULL;
    uint8_t highest_priority = 255;

    // Find highest priority ready task
    for (int i = 0; i < scheduler.task_count; i++) {
        Task* t = &scheduler.tasks[i];

        if (!t->enabled) continue;

        bool should_run = false;

        if (t->period_ms > 0) {
            // Periodic: check if time to run
            if ((int32_t)(now - t->next_run) >= 0) {
                should_run = true;
            }
        } else {
            // Aperiodic: check if triggered
            if (t->pending) {
                should_run = true;
            }
        }

        if (should_run && t->priority < highest_priority) {
            ready_task = t;
            highest_priority = t->priority;
        }
    }

    // Execute ready task
    if (ready_task) {
        printf("[SCHED] Running '%s' at tick %u\n", ready_task->name, now);

        uint32_t start = now;
        ready_task->func();
        uint32_t duration = get_tick() - start;

        ready_task->run_count++;
        if (duration > ready_task->max_duration) {
            ready_task->max_duration = duration;
        }

        if (ready_task->period_ms > 0) {
            // Periodic: schedule next run
            ready_task->next_run += ready_task->period_ms;

            // Check for missed deadline
            if ((int32_t)(get_tick() - ready_task->next_run) > 0) {
                ready_task->missed_deadlines++;
                printf("[SCHED] WARNING: '%s' missed deadline!\n",
                       ready_task->name);
            }
        } else {
            // Aperiodic: clear pending
            ready_task->pending = false;
        }
    }
}

void scheduler_run(void) {
    scheduler.running = true;
    printf("\n[SCHED] Scheduler started\n");

    while (scheduler.running) {
        scheduler_run_once();

        // For demo: simulate time passing
        scheduler.tick_count += 10;

        // Exit after some time for demo
        if (scheduler.tick_count > 1000) break;
    }

    printf("[SCHED] Scheduler stopped\n");
}

// ============================================================================
// RATE MONOTONIC SCHEDULING (RMS) ANALYSIS
// ============================================================================

/*
 * RMS is optimal for fixed-priority preemptive scheduling.
 *
 * RULE: Shorter period = higher priority
 *
 * SCHEDULABILITY TEST (Liu & Layland):
 *   U = Σ(Ci/Ti) <= n(2^(1/n) - 1)
 *
 *   Where:
 *   Ci = Execution time of task i
 *   Ti = Period of task i
 *   n = Number of tasks
 *
 *   For n=1: U <= 1.0 (100%)
 *   For n=2: U <= 0.828 (82.8%)
 *   For n=3: U <= 0.780 (78%)
 *   For n→∞: U <= 0.693 (69.3%)
 *
 * EXAMPLE:
 *   Task A: Period=100ms, Exec=20ms  → U = 0.2
 *   Task B: Period=500ms, Exec=100ms → U = 0.2
 *   Total U = 0.4 < 0.828 → Schedulable with 2 tasks
 */

float calculate_utilization(void) {
    float total_u = 0;

    for (int i = 0; i < scheduler.task_count; i++) {
        Task* t = &scheduler.tasks[i];
        if (t->period_ms > 0 && t->enabled) {
            // Estimate execution time as max_duration or 1ms
            float exec = t->max_duration > 0 ? t->max_duration : 1;
            float u = exec / t->period_ms;
            total_u += u;
            printf("  %s: C=%u, T=%u, U=%.3f\n",
                   t->name, (uint32_t)exec, t->period_ms, u);
        }
    }

    return total_u;
}

// ============================================================================
// EXAMPLE TASKS
// ============================================================================

void task_sensor_read(void) {
    printf("    -> Reading sensor\n");
    // Simulated work
}

void task_watchdog_feed(void) {
    printf("    -> Feeding watchdog\n");
}

void task_led_blink(void) {
    static bool led_state = false;
    led_state = !led_state;
    printf("    -> LED %s\n", led_state ? "ON" : "OFF");
}

void task_data_upload(void) {
    printf("    -> Uploading data (aperiodic)\n");
}

void task_button_handler(void) {
    printf("    -> Handling button press (aperiodic)\n");
}

// ============================================================================
// DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   TASK SCHEDULING DEMO\n");
    printf("============================================\n");

    // Add periodic tasks (higher priority = lower number)
    scheduler_add_periodic("sensor",   task_sensor_read,   100, 1);  // 10Hz
    scheduler_add_periodic("watchdog", task_watchdog_feed, 500, 0);  // 2Hz, highest
    scheduler_add_periodic("led",      task_led_blink,     250, 2);  // 4Hz

    // Add aperiodic tasks
    int upload_task = scheduler_add_aperiodic("upload", task_data_upload, 3);
    int button_task = scheduler_add_aperiodic("button", task_button_handler, 1);

    // Trigger some aperiodic tasks
    scheduler_trigger(button_task);

    // Run scheduler
    scheduler_run();

    // Statistics
    printf("\n--- Task Statistics ---\n");
    for (int i = 0; i < scheduler.task_count; i++) {
        Task* t = &scheduler.tasks[i];
        printf("%s: runs=%u, max_time=%u, missed=%u\n",
               t->name, t->run_count, t->max_duration, t->missed_deadlines);
    }

    // Utilization analysis
    printf("\n--- Utilization Analysis ---\n");
    float u = calculate_utilization();
    printf("Total utilization: %.1f%%\n", u * 100);

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "How do you handle periodic and aperiodic tasks together?"
 * A: "Priority-based scheduling:
 *    - Periodic tasks: Fixed priorities, guaranteed execution
 *    - Aperiodic: Run in gaps, or use background priority
 *
 *    Aperiodic can also use servers:
 *    - Polling server: Check each period
 *    - Deferrable server: Save budget for aperiodic"
 *
 * Q: "How do you ensure periodic tasks meet deadlines?"
 * A: "1. Use Rate Monotonic: shorter period = higher priority
 *    2. Calculate utilization, ensure < 69% for safety
 *    3. Measure actual execution times
 *    4. Monitor for missed deadlines
 *    5. Leave margin for interrupt handling"
 *
 * Q: "Cooperative vs preemptive - when to use each?"
 * A: "Cooperative: Simple, predictable, no race conditions
 *       Good for: Simple systems, power-constrained
 *    Preemptive: Better responsiveness, complex
 *       Good for: Hard real-time, complex systems
 *
 *    Cooperative requires short tasks - can't block!"
 *
 * ============================================================================
 */
