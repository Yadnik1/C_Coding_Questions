/*
 * ============================================================================
 *        DESIGN: TIMER-BASED EVENT SCHEDULER
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★☆ (HIGH)
 *   - Periodic sensor sampling
 *   - Timeout handling
 *   - LED blink patterns
 *   - Delayed operations
 *   - Watchdog feeding
 *
 * ============================================================================
 *                    SCHEDULER ARCHITECTURES
 * ============================================================================
 *
 * 1. SINGLE TIMER + SOFTWARE SCHEDULER
 *    One hardware timer ticks at fixed rate (e.g., 1ms)
 *    Software tracks multiple virtual timers
 *    Pro: Uses only one hardware timer
 *    Con: Resolution limited to tick rate
 *
 * 2. MULTIPLE HARDWARE TIMERS
 *    Each event uses dedicated hardware timer
 *    Pro: Precise timing, true concurrency
 *    Con: Limited hardware timers available
 *
 * 3. SORTED TIMER LIST
 *    Timer sorted by expiry time
 *    Only check head of list each tick
 *    Pro: Efficient O(1) tick handling
 *    Con: O(n) insertion
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// BASIC SOFTWARE TIMER
// ============================================================================

typedef void (*TimerCallback)(void* arg);

typedef struct {
    uint32_t expires_at;        // When timer fires (in ticks)
    uint32_t interval;          // For periodic timers (0 = one-shot)
    TimerCallback callback;
    void* arg;
    bool active;
    const char* name;           // For debugging
} SoftTimer;

#define MAX_TIMERS 16

typedef struct {
    SoftTimer timers[MAX_TIMERS];
    uint8_t count;
    volatile uint32_t tick_count;
} TimerScheduler;

TimerScheduler scheduler = {0};

// Called from hardware timer ISR (every 1ms typically)
void timer_tick_isr(void) {
    scheduler.tick_count++;
}

uint32_t timer_get_ticks(void) {
    return scheduler.tick_count;
}

int timer_create(const char* name, TimerCallback cb, void* arg) {
    if (scheduler.count >= MAX_TIMERS) return -1;

    int id = scheduler.count++;
    scheduler.timers[id].name = name;
    scheduler.timers[id].callback = cb;
    scheduler.timers[id].arg = arg;
    scheduler.timers[id].active = false;

    return id;
}

void timer_start_oneshot(int timer_id, uint32_t delay_ms) {
    if (timer_id < 0 || timer_id >= scheduler.count) return;

    SoftTimer* t = &scheduler.timers[timer_id];
    t->expires_at = scheduler.tick_count + delay_ms;
    t->interval = 0;
    t->active = true;
}

void timer_start_periodic(int timer_id, uint32_t interval_ms) {
    if (timer_id < 0 || timer_id >= scheduler.count) return;

    SoftTimer* t = &scheduler.timers[timer_id];
    t->expires_at = scheduler.tick_count + interval_ms;
    t->interval = interval_ms;
    t->active = true;
}

void timer_stop(int timer_id) {
    if (timer_id < 0 || timer_id >= scheduler.count) return;
    scheduler.timers[timer_id].active = false;
}

// Call from main loop
void timer_process(void) {
    uint32_t now = scheduler.tick_count;

    for (int i = 0; i < scheduler.count; i++) {
        SoftTimer* t = &scheduler.timers[i];

        if (!t->active) continue;

        // Check if expired (handle wrap-around)
        if ((int32_t)(now - t->expires_at) >= 0) {
            // Timer expired
            if (t->callback) {
                t->callback(t->arg);
            }

            if (t->interval > 0) {
                // Periodic: reschedule
                t->expires_at = now + t->interval;
            } else {
                // One-shot: deactivate
                t->active = false;
            }
        }
    }
}

// ============================================================================
// SORTED TIMER LIST (More efficient for many timers)
// ============================================================================

typedef struct TimerNode {
    uint32_t expires_at;
    TimerCallback callback;
    void* arg;
    uint32_t interval;
    struct TimerNode* next;
} TimerNode;

typedef struct {
    TimerNode* head;
    TimerNode nodes[MAX_TIMERS];
    uint8_t free_count;
} SortedTimerList;

TimerNode* sorted_list_insert(SortedTimerList* list, uint32_t expires_at,
                              TimerCallback cb, void* arg, uint32_t interval) {
    // Allocate node (simple linear search for free)
    TimerNode* node = NULL;
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (list->nodes[i].callback == NULL) {
            node = &list->nodes[i];
            break;
        }
    }
    if (!node) return NULL;

    node->expires_at = expires_at;
    node->callback = cb;
    node->arg = arg;
    node->interval = interval;

    // Insert in sorted order
    if (list->head == NULL || expires_at < list->head->expires_at) {
        node->next = list->head;
        list->head = node;
    } else {
        TimerNode* curr = list->head;
        while (curr->next && curr->next->expires_at <= expires_at) {
            curr = curr->next;
        }
        node->next = curr->next;
        curr->next = node;
    }

    return node;
}

void sorted_list_process(SortedTimerList* list, uint32_t now) {
    while (list->head && (int32_t)(now - list->head->expires_at) >= 0) {
        TimerNode* expired = list->head;
        list->head = expired->next;

        // Call callback
        if (expired->callback) {
            expired->callback(expired->arg);
        }

        // Reschedule if periodic
        if (expired->interval > 0) {
            sorted_list_insert(list, now + expired->interval,
                              expired->callback, expired->arg, expired->interval);
        }

        // Free node
        expired->callback = NULL;
    }
}

// ============================================================================
// COMMON USE PATTERNS
// ============================================================================

// Pattern 1: Delayed execution
void delayed_wifi_connect(void* arg) {
    (void)arg;
    printf("[TIMER] Starting WiFi connection (delayed start)\n");
}

// Pattern 2: Periodic sensor read
void sensor_read_callback(void* arg) {
    int* counter = (int*)arg;
    (*counter)++;
    printf("[TIMER] Sensor read #%d\n", *counter);
}

// Pattern 3: Timeout handling
typedef struct {
    bool* timeout_flag;
    int operation_id;
} TimeoutContext;

void operation_timeout(void* arg) {
    TimeoutContext* ctx = (TimeoutContext*)arg;
    *ctx->timeout_flag = true;
    printf("[TIMER] Operation %d TIMEOUT!\n", ctx->operation_id);
}

// Pattern 4: LED blink pattern
typedef struct {
    int gpio_pin;
    bool state;
} LedContext;

void led_toggle(void* arg) {
    LedContext* led = (LedContext*)arg;
    led->state = !led->state;
    printf("[TIMER] LED %d: %s\n", led->gpio_pin, led->state ? "ON" : "OFF");
}

// Pattern 5: Debounced callback
typedef struct {
    bool pending;
    int timer_id;
    void (*actual_callback)(void);
} DebouncedAction;

void debounce_callback(void* arg) {
    DebouncedAction* action = (DebouncedAction*)arg;
    if (action->pending) {
        action->pending = false;
        if (action->actual_callback) {
            action->actual_callback();
        }
    }
}

// ============================================================================
// TIMEOUT WITH CANCELLATION
// ============================================================================

typedef struct {
    int timer_id;
    bool active;
    void (*on_timeout)(void* arg);
    void* arg;
} CancellableTimeout;

void cancellable_timeout_callback(void* arg) {
    CancellableTimeout* to = (CancellableTimeout*)arg;
    if (to->active) {
        to->active = false;
        if (to->on_timeout) {
            to->on_timeout(to->arg);
        }
    }
}

void timeout_start(CancellableTimeout* to, uint32_t timeout_ms) {
    to->active = true;
    timer_start_oneshot(to->timer_id, timeout_ms);
}

void timeout_cancel(CancellableTimeout* to) {
    to->active = false;
    timer_stop(to->timer_id);
}

// ============================================================================
// DEMO
// ============================================================================

static int sensor_count = 0;
static LedContext led1 = {.gpio_pin = 1, .state = false};
static bool op_timeout = false;
static TimeoutContext timeout_ctx = {&op_timeout, 42};

void demo_callbacks_setup(void) {
    // One-shot: delayed WiFi connect
    int wifi_timer = timer_create("wifi_delay", delayed_wifi_connect, NULL);
    timer_start_oneshot(wifi_timer, 1000);

    // Periodic: sensor reading every 500ms
    int sensor_timer = timer_create("sensor", sensor_read_callback, &sensor_count);
    timer_start_periodic(sensor_timer, 500);

    // Periodic: LED blink every 250ms
    int led_timer = timer_create("led", led_toggle, &led1);
    timer_start_periodic(led_timer, 250);

    // One-shot: operation timeout
    int timeout_timer = timer_create("timeout", operation_timeout, &timeout_ctx);
    timer_start_oneshot(timeout_timer, 2000);
}

int main() {
    printf("============================================\n");
    printf("   TIMER EVENT SCHEDULER DEMO\n");
    printf("============================================\n");

    // Setup timers
    demo_callbacks_setup();
    printf("Registered %d timers\n\n", scheduler.count);

    // Simulate time passing
    printf("--- Simulating 3 seconds ---\n\n");
    for (uint32_t ms = 0; ms < 3000; ms += 50) {
        scheduler.tick_count = ms;

        if (ms % 250 == 0) {
            printf("[TIME] %u ms\n", ms);
        }

        timer_process();
    }

    printf("\n--- Statistics ---\n");
    printf("Sensor readings: %d\n", sensor_count);
    printf("Timeout occurred: %s\n", op_timeout ? "yes" : "no");

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "How do you handle timer wrap-around?"
 * A: "Use signed comparison: (int32_t)(now - expires_at) >= 0
 *    This works correctly even when now wraps past 0.
 *    With 32-bit counter at 1ms tick, wraps every 49 days."
 *
 * Q: "Sorted list vs linear scan - when to use each?"
 * A: "Linear scan: Simple, good for < 10 timers
 *    Sorted list: Better for many timers
 *      - O(1) tick processing (only check head)
 *      - O(n) insertion (must find position)
 *    Delta list: Store time until next, not absolute time"
 *
 * Q: "How do you ensure timer callbacks don't miss deadlines?"
 * A: "1. Keep callbacks short (defer heavy work)
 *    2. Process timers at high priority
 *    3. For periodic, add interval to EXPECTED time, not current
 *       (prevents drift from processing delays)
 *    4. Monitor for overruns"
 *
 * ============================================================================
 */
