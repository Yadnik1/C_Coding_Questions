/*
 * ============================================================================
 * SYSTEM DESIGN: Simple Timer/Scheduler
 * ============================================================================
 *
 * WHAT IS IT?
 * System to schedule tasks to run at specific intervals or after delays.
 * Core of any embedded system - blinking LEDs, polling sensors, timeouts.
 *
 * CONCEPT:
 *   - Maintain a list of timers with expiry times
 *   - Each tick, check if any timer expired
 *   - If expired, call the callback function
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_TIMERS 8

typedef void (*TimerCallback)(void);  // Function pointer type

typedef struct {
    bool active;
    uint32_t period;       // Period in ticks (0 = one-shot)
    uint32_t remaining;    // Ticks until expiry
    TimerCallback callback;
} Timer;

typedef struct {
    Timer timers[MAX_TIMERS];
    uint32_t tick_count;
} TimerScheduler;

void scheduler_init(TimerScheduler* sched) {
    sched->tick_count = 0;
    for (int i = 0; i < MAX_TIMERS; i++) {
        sched->timers[i].active = false;
    }
}

int scheduler_add_timer(TimerScheduler* sched, uint32_t delay, uint32_t period,
                        TimerCallback cb) {
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (!sched->timers[i].active) {
            sched->timers[i].active = true;
            sched->timers[i].remaining = delay;
            sched->timers[i].period = period;      // 0 = one-shot
            sched->timers[i].callback = cb;
            return i;  // Return timer ID
        }
    }
    return -1;  // No free slots
}

void scheduler_tick(TimerScheduler* sched) {
    sched->tick_count++;
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (sched->timers[i].active) {
            if (sched->timers[i].remaining > 0) {
                sched->timers[i].remaining--;
            }
            if (sched->timers[i].remaining == 0) {
                sched->timers[i].callback();       // Call the callback
                if (sched->timers[i].period > 0) {
                    sched->timers[i].remaining = sched->timers[i].period;  // Reload
                } else {
                    sched->timers[i].active = false;  // One-shot done
                }
            }
        }
    }
}

// Example callbacks
void led_toggle(void) { printf("  [LED Toggle]\n"); }
void sensor_read(void) { printf("  [Sensor Read]\n"); }
void timeout_handler(void) { printf("  [Timeout!]\n"); }

int main() {
    TimerScheduler sched;
    scheduler_init(&sched);

    // Add periodic timer (every 3 ticks)
    scheduler_add_timer(&sched, 3, 3, led_toggle);

    // Add one-shot timer (after 5 ticks)
    scheduler_add_timer(&sched, 5, 0, timeout_handler);

    // Simulate 10 ticks
    for (int i = 1; i <= 10; i++) {
        printf("Tick %d:\n", i);
        scheduler_tick(&sched);
    }

    return 0;
}

/*
 * ============================================================================
 * DRY RUN: LED timer (period=3), Timeout (delay=5, one-shot)
 * ============================================================================
 *
 * Timer 0: remaining=3, period=3 (LED - periodic)
 * Timer 1: remaining=5, period=0 (Timeout - one-shot)
 *
 * Tick 1: Timer0: 3->2, Timer1: 5->4
 * Tick 2: Timer0: 2->1, Timer1: 4->3
 * Tick 3: Timer0: 1->0 -> FIRE! reload to 3, Timer1: 3->2
 *         Output: [LED Toggle]
 * Tick 4: Timer0: 3->2, Timer1: 2->1
 * Tick 5: Timer0: 2->1, Timer1: 1->0 -> FIRE! one-shot done
 *         Output: [Timeout!]
 * Tick 6: Timer0: 1->0 -> FIRE! reload to 3
 *         Output: [LED Toggle]
 * Tick 7: Timer0: 3->2
 * Tick 8: Timer0: 2->1
 * Tick 9: Timer0: 1->0 -> FIRE! reload to 3
 *         Output: [LED Toggle]
 * Tick 10: Timer0: 3->2
 *
 * EXPECTED OUTPUT:
 *   Tick 3: [LED Toggle]
 *   Tick 5: [Timeout!]
 *   Tick 6: [LED Toggle]
 *   Tick 9: [LED Toggle]
 *
 * ============================================================================
 * INTERVIEW TIPS:
 * - Use function pointers for callbacks
 * - Distinguish periodic vs one-shot timers
 * - Real systems: call scheduler_tick() from hardware timer ISR
 * - Consider: timer cancellation, priority, overflow handling
 * ============================================================================
 */
