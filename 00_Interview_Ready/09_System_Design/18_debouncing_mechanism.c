/*
 * ============================================================================
 *        DESIGN: DEBOUNCING MECHANISM FOR TACTILE SWITCH
 * ============================================================================
 *
 * PROBLEM STATEMENT:
 *   Tactile switches don't produce clean transitions. When pressed/released,
 *   the mechanical contacts "bounce" producing multiple rapid transitions
 *   before settling. This can cause:
 *   - Multiple button press events from single press
 *   - Erratic behavior in state machines
 *   - Incorrect counts in counters
 *
 * BOUNCING BEHAVIOR:
 *
 *   Ideal (what we want):
 *
 *   VCC ─────┐         ┌─────────
 *            │         │
 *   GND      └─────────┘
 *         Press     Release
 *
 *
 *   Reality (what we get):
 *
 *   VCC ─────┐ ┌┐┌┐    ┌┐┌┐┌─────
 *            │ ││││    ││││
 *   GND      └─┘└┘└────┘└┘└
 *         Press        Release
 *         ~5-20ms      ~5-20ms
 *
 * ============================================================================
 *                    DESIGN APPROACHES
 * ============================================================================
 *
 * APPROACH 1: HARDWARE DEBOUNCING
 *
 *   RC Filter:
 *
 *   VCC ──[R 10k]──┬── MCU Pin
 *                  │
 *   Button ────────┤
 *                  │
 *                 [C 100nF]
 *                  │
 *                 GND
 *
 *   Time constant: τ = R × C = 10k × 100nF = 1ms
 *   Effective debounce: ~5τ = 5ms
 *
 *   Pros: No CPU overhead, simple
 *   Cons: Adds components, fixed timing, slows edge
 *
 *
 * APPROACH 2: SOFTWARE DEBOUNCING - DELAY BASED
 *
 *   On button state change:
 *   1. Wait fixed delay (10-50ms)
 *   2. Read button again
 *   3. If same state, accept it
 *
 *   Pros: Simple to implement
 *   Cons: Blocking delay, misses rapid presses
 *
 *
 * APPROACH 3: SOFTWARE DEBOUNCING - TIMER BASED (RECOMMENDED)
 *
 *   1. Sample button at regular interval (1-10ms)
 *   2. Count consecutive same-state samples
 *   3. Accept state after N consecutive samples
 *
 *   Pros: Non-blocking, configurable, reliable
 *   Cons: Slight complexity, needs timer
 *
 *
 * APPROACH 4: SOFTWARE DEBOUNCING - TIMESTAMP BASED
 *
 *   1. On edge detect, record timestamp
 *   2. Ignore further edges until debounce time passed
 *   3. Accept next edge after timeout
 *
 *   Pros: Efficient, immediate first response
 *   Cons: Needs accurate timestamp
 *
 *
 * APPROACH 5: INTEGRATING DEBOUNCER
 *
 *   1. Add/subtract counter based on pin state
 *   2. Clamp counter between 0 and MAX
 *   3. State = HIGH when counter reaches MAX
 *   4. State = LOW when counter reaches 0
 *
 *   Pros: Noise immune, smooth transitions
 *   Cons: Slower response, more complex
 *
 * ============================================================================
 *                    TRADE-OFFS ANALYSIS
 * ============================================================================
 *
 *   Method        | Response | CPU Load | Reliability | Complexity
 *   --------------|----------|----------|-------------|------------
 *   Hardware RC   | Medium   | None     | Good        | Low
 *   Delay-based   | Slow     | High     | Good        | Very Low
 *   Timer-based   | Fast     | Low      | Excellent   | Medium
 *   Timestamp     | Fast     | Low      | Good        | Medium
 *   Integrating   | Slow     | Low      | Excellent   | Medium
 *
 * RECOMMENDATION:
 *   - Battery device: Hardware RC + simple software check
 *   - General purpose: Timer-based (Approach 3)
 *   - Noisy environment: Integrating debouncer
 *   - Rapid response needed: Timestamp-based
 *
 * ============================================================================
 *                    DESIGN PARAMETERS
 * ============================================================================
 *
 *   DEBOUNCE_TIME:   10-50ms (typical switch bounce duration)
 *   SAMPLE_INTERVAL: 1-5ms (for timer-based)
 *   SAMPLE_COUNT:    5-10 samples (debounce_time / sample_interval)
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// APPROACH 2: SIMPLE DELAY-BASED DEBOUNCING
// ============================================================================

/*
 * WHEN TO USE: Simple applications, blocking is acceptable
 *
 * PSEUDOCODE:
 *   function is_button_pressed():
 *       if read_pin() == LOW:           // Button pressed (active low)
 *           delay(DEBOUNCE_MS)          // Wait for bounce to settle
 *           if read_pin() == LOW:       // Still pressed?
 *               return true
 *       return false
 */

#define DEBOUNCE_DELAY_MS 20

typedef struct {
    uint8_t pin;
    bool last_state;
} SimpleButton;

// Simulated hardware functions
static uint8_t simulated_pin_state = 1;  // 1 = not pressed (active low)

uint8_t read_gpio(uint8_t pin) {
    (void)pin;
    return simulated_pin_state;
}

void delay_ms(uint32_t ms) {
    // In real code: actual delay
    printf("  [Delay %u ms]\n", ms);
}

bool simple_debounce_read(SimpleButton* btn) {
    if (read_gpio(btn->pin) == 0) {  // Active low: 0 = pressed
        delay_ms(DEBOUNCE_DELAY_MS);
        if (read_gpio(btn->pin) == 0) {
            return true;  // Confirmed press
        }
    }
    return false;
}


// ============================================================================
// APPROACH 3: TIMER-BASED DEBOUNCING (RECOMMENDED)
// ============================================================================

/*
 * WHEN TO USE: Most applications, non-blocking required
 *
 * THEORY:
 *   - Sample button every SAMPLE_INTERVAL_MS
 *   - Track consecutive same-state samples
 *   - State changes only after DEBOUNCE_COUNT consecutive samples
 *
 * STATE MACHINE:
 *
 *   ┌─────────────────────────────────────────────┐
 *   │                                             │
 *   │    count < THRESHOLD                        │
 *   │    ┌────────────┐                           │
 *   │    │            │                           │
 *   ▼    ▼            │                           │
 *   RELEASED ◄────────┴── count >= THRESHOLD ──► PRESSED
 *      │                                            │
 *      │              count < THRESHOLD             │
 *      │              ┌────────────┐                │
 *      │              │            │                │
 *      └──────────────┴────────────┴────────────────┘
 *
 * PSEUDOCODE:
 *   every SAMPLE_INTERVAL_MS:
 *       raw = read_pin()
 *       if raw == target_state:
 *           counter++
 *           if counter >= DEBOUNCE_COUNT:
 *               debounced_state = target_state
 *               counter = DEBOUNCE_COUNT  // Clamp
 *       else:
 *           target_state = raw
 *           counter = 0
 */

#define SAMPLE_INTERVAL_MS  5
#define DEBOUNCE_COUNT      4   // 4 samples × 5ms = 20ms debounce

typedef enum {
    BTN_RELEASED = 0,
    BTN_PRESSED = 1
} ButtonState;

typedef enum {
    BTN_EVENT_NONE = 0,
    BTN_EVENT_PRESSED,
    BTN_EVENT_RELEASED
} ButtonEvent;

typedef struct {
    uint8_t pin;
    ButtonState debounced_state;   // Confirmed state
    uint8_t raw_state;             // Current target state
    uint8_t counter;               // Consecutive same-state samples
    bool active_low;               // True if button connects to GND
} DebouncedButton;

void debounced_button_init(DebouncedButton* btn, uint8_t pin, bool active_low) {
    btn->pin = pin;
    btn->active_low = active_low;
    btn->debounced_state = BTN_RELEASED;
    btn->raw_state = active_low ? 1 : 0;  // Initial state = released
    btn->counter = DEBOUNCE_COUNT;        // Start stable
}

/*
 * Call this function every SAMPLE_INTERVAL_MS from timer interrupt or main loop
 * Returns event if state changed
 */
ButtonEvent debounced_button_update(DebouncedButton* btn) {
    // Read raw pin state
    uint8_t current_raw = read_gpio(btn->pin);

    // Convert to logical state (pressed = 1, released = 0)
    uint8_t logical_state = btn->active_low ? !current_raw : current_raw;

    ButtonEvent event = BTN_EVENT_NONE;

    if (logical_state == btn->raw_state) {
        // Same state as before, increment counter
        if (btn->counter < DEBOUNCE_COUNT) {
            btn->counter++;

            // Threshold reached - state is confirmed
            if (btn->counter >= DEBOUNCE_COUNT) {
                ButtonState new_state = logical_state ? BTN_PRESSED : BTN_RELEASED;

                if (new_state != btn->debounced_state) {
                    btn->debounced_state = new_state;
                    event = (new_state == BTN_PRESSED) ?
                            BTN_EVENT_PRESSED : BTN_EVENT_RELEASED;
                }
            }
        }
    } else {
        // State changed, reset counter and track new target
        btn->raw_state = logical_state;
        btn->counter = 0;
    }

    return event;
}

bool debounced_button_is_pressed(DebouncedButton* btn) {
    return btn->debounced_state == BTN_PRESSED;
}


// ============================================================================
// APPROACH 4: TIMESTAMP-BASED DEBOUNCING
// ============================================================================

/*
 * WHEN TO USE: Need immediate response on first edge
 *
 * THEORY:
 *   - Accept first edge immediately
 *   - Ignore subsequent edges until debounce period expires
 *   - Good for buttons that need fast response
 *
 * PSEUDOCODE:
 *   on_edge_interrupt():
 *       current_time = get_time_ms()
 *       if (current_time - last_edge_time) >= DEBOUNCE_MS:
 *           last_edge_time = current_time
 *           debounced_state = current_pin_state
 *           generate_event()
 *       // else: ignore this edge (still bouncing)
 */

#define DEBOUNCE_TIME_MS 20

typedef struct {
    uint8_t pin;
    ButtonState state;
    uint32_t last_edge_time;
    bool active_low;
} TimestampButton;

// Simulated time
static uint32_t system_time_ms = 0;

uint32_t get_time_ms(void) {
    return system_time_ms;
}

void timestamp_button_init(TimestampButton* btn, uint8_t pin, bool active_low) {
    btn->pin = pin;
    btn->active_low = active_low;
    btn->state = BTN_RELEASED;
    btn->last_edge_time = 0;
}

/*
 * Call this from GPIO edge interrupt
 */
ButtonEvent timestamp_button_on_edge(TimestampButton* btn) {
    uint32_t now = get_time_ms();

    // Check if debounce period has passed
    if ((now - btn->last_edge_time) < DEBOUNCE_TIME_MS) {
        return BTN_EVENT_NONE;  // Ignore, still bouncing
    }

    // Accept this edge
    btn->last_edge_time = now;

    // Read current state
    uint8_t raw = read_gpio(btn->pin);
    ButtonState new_state = (btn->active_low ? !raw : raw) ?
                            BTN_PRESSED : BTN_RELEASED;

    if (new_state != btn->state) {
        btn->state = new_state;
        return (new_state == BTN_PRESSED) ?
               BTN_EVENT_PRESSED : BTN_EVENT_RELEASED;
    }

    return BTN_EVENT_NONE;
}


// ============================================================================
// APPROACH 5: INTEGRATING DEBOUNCER
// ============================================================================

/*
 * WHEN TO USE: Noisy environments, need hysteresis
 *
 * THEORY:
 *   - Maintain integrator counter
 *   - Pin HIGH: increment counter (up to MAX)
 *   - Pin LOW: decrement counter (down to 0)
 *   - Output HIGH when counter reaches MAX
 *   - Output LOW when counter reaches 0
 *   - Provides built-in hysteresis
 *
 * PSEUDOCODE:
 *   every sample_interval:
 *       if pin == HIGH:
 *           integrator = min(integrator + 1, MAX)
 *       else:
 *           integrator = max(integrator - 1, 0)
 *
 *       if integrator >= MAX:
 *           output = HIGH
 *       else if integrator <= 0:
 *           output = LOW
 *       // else: output unchanged (hysteresis)
 */

#define INTEGRATOR_MAX 10

typedef struct {
    uint8_t pin;
    int8_t integrator;
    ButtonState state;
    bool active_low;
} IntegratingButton;

void integrating_button_init(IntegratingButton* btn, uint8_t pin, bool active_low) {
    btn->pin = pin;
    btn->active_low = active_low;
    btn->integrator = 0;
    btn->state = BTN_RELEASED;
}

/*
 * Call this every 1-2ms
 */
ButtonEvent integrating_button_update(IntegratingButton* btn) {
    uint8_t raw = read_gpio(btn->pin);
    uint8_t logical = btn->active_low ? !raw : raw;

    // Integrate
    if (logical) {
        if (btn->integrator < INTEGRATOR_MAX) {
            btn->integrator++;
        }
    } else {
        if (btn->integrator > 0) {
            btn->integrator--;
        }
    }

    // Check thresholds
    ButtonEvent event = BTN_EVENT_NONE;

    if (btn->integrator >= INTEGRATOR_MAX && btn->state != BTN_PRESSED) {
        btn->state = BTN_PRESSED;
        event = BTN_EVENT_PRESSED;
    } else if (btn->integrator <= 0 && btn->state != BTN_RELEASED) {
        btn->state = BTN_RELEASED;
        event = BTN_EVENT_RELEASED;
    }

    return event;
}


// ============================================================================
// MULTIPLE BUTTONS WITH SINGLE TIMER
// ============================================================================

/*
 * SCALABLE DESIGN: Handle many buttons efficiently
 *
 * - Single timer interrupt samples all buttons
 * - Each button has its own debounce state
 * - Events queued or callbacks invoked
 */

#define MAX_BUTTONS 8

typedef void (*ButtonCallback)(uint8_t button_id, ButtonEvent event);

typedef struct {
    DebouncedButton buttons[MAX_BUTTONS];
    uint8_t button_count;
    ButtonCallback callback;
} ButtonManager;

void button_manager_init(ButtonManager* mgr, ButtonCallback cb) {
    mgr->button_count = 0;
    mgr->callback = cb;
}

int button_manager_add(ButtonManager* mgr, uint8_t pin, bool active_low) {
    if (mgr->button_count >= MAX_BUTTONS) {
        return -1;  // Full
    }

    debounced_button_init(&mgr->buttons[mgr->button_count], pin, active_low);
    return mgr->button_count++;
}

/*
 * Call from timer interrupt every SAMPLE_INTERVAL_MS
 */
void button_manager_update(ButtonManager* mgr) {
    for (uint8_t i = 0; i < mgr->button_count; i++) {
        ButtonEvent event = debounced_button_update(&mgr->buttons[i]);
        if (event != BTN_EVENT_NONE && mgr->callback) {
            mgr->callback(i, event);
        }
    }
}


// ============================================================================
// DEMO AND TESTING
// ============================================================================

void demo_callback(uint8_t button_id, ButtonEvent event) {
    const char* event_str = (event == BTN_EVENT_PRESSED) ? "PRESSED" : "RELEASED";
    printf("  Button %d: %s\n", button_id, event_str);
}

void simulate_bouncing_press(DebouncedButton* btn) {
    printf("\nSimulating bouncing press:\n");

    // Simulate bouncing sequence: 1,0,1,0,0,0,0 (active low, so 0=pressed)
    uint8_t bounce_sequence[] = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < 10; i++) {
        simulated_pin_state = bounce_sequence[i];
        printf("  Sample %d: pin=%d ", i, simulated_pin_state);

        ButtonEvent event = debounced_button_update(btn);

        printf("counter=%d state=%s",
               btn->counter,
               btn->debounced_state == BTN_PRESSED ? "PRESSED" : "RELEASED");

        if (event != BTN_EVENT_NONE) {
            printf(" -> EVENT: %s",
                   event == BTN_EVENT_PRESSED ? "PRESSED" : "RELEASED");
        }
        printf("\n");
    }
}

void simulate_bouncing_release(DebouncedButton* btn) {
    printf("\nSimulating bouncing release:\n");

    // Simulate bouncing release: 0,1,0,1,1,1,1
    uint8_t bounce_sequence[] = {0, 1, 0, 1, 1, 1, 1, 1, 1, 1};

    for (int i = 0; i < 10; i++) {
        simulated_pin_state = bounce_sequence[i];
        printf("  Sample %d: pin=%d ", i, simulated_pin_state);

        ButtonEvent event = debounced_button_update(btn);

        printf("counter=%d state=%s",
               btn->counter,
               btn->debounced_state == BTN_PRESSED ? "PRESSED" : "RELEASED");

        if (event != BTN_EVENT_NONE) {
            printf(" -> EVENT: %s",
                   event == BTN_EVENT_PRESSED ? "PRESSED" : "RELEASED");
        }
        printf("\n");
    }
}

int main() {
    printf("============================================\n");
    printf("   DEBOUNCING MECHANISM DEMONSTRATION\n");
    printf("============================================\n");

    // Initialize button (pin 0, active low)
    DebouncedButton btn;
    debounced_button_init(&btn, 0, true);

    // Demonstrate bouncing press
    simulate_bouncing_press(&btn);

    // Demonstrate bouncing release
    simulate_bouncing_release(&btn);

    // Demonstrate button manager
    printf("\n--- Button Manager Demo ---\n");
    ButtonManager mgr;
    button_manager_init(&mgr, demo_callback);
    button_manager_add(&mgr, 0, true);  // Button 0
    button_manager_add(&mgr, 1, true);  // Button 1

    printf("Manager initialized with %d buttons\n", mgr.button_count);

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "Walk me through your debouncing design."
 * A: "I'd use timer-based sampling because:
 *    1. Non-blocking - doesn't waste CPU cycles
 *    2. Configurable - can tune debounce time via sample count
 *    3. Scalable - single timer handles multiple buttons
 *    4. Reliable - counts consecutive samples, immune to single glitches
 *
 *    I sample every 5ms and require 4 consecutive same-state samples
 *    for 20ms total debounce time. State only changes after threshold
 *    is reached, providing noise immunity."
 *
 * Q: "What if you need faster response?"
 * A: "I'd use timestamp-based debouncing:
 *    - Accept first edge immediately
 *    - Ignore subsequent edges until debounce period expires
 *    - Good for gaming or UI where latency matters
 *    Trade-off: Slightly less noise immune than integration method."
 *
 * Q: "How would you handle a very noisy environment?"
 * A: "I'd use the integrating debouncer:
 *    - Adds hysteresis naturally
 *    - Gradual state transitions
 *    - Can survive multiple noise glitches
 *    Trade-off: Slower response time."
 *
 * Q: "Hardware vs software debouncing?"
 * A: "Hardware (RC filter):
 *    + Zero CPU overhead
 *    + Works even during ISR-disabled periods
 *    - Adds BOM cost
 *    - Fixed timing (can't adjust in field)
 *    - Slows down edge for interrupt detection
 *
 *    Software:
 *    + No extra components
 *    + Configurable
 *    + Can report press duration
 *    - Uses CPU time
 *    - Needs proper timing source
 *
 *    Best practice: Use both for critical buttons."
 *
 * ============================================================================
 *                    EDGE CASES TO CONSIDER
 * ============================================================================
 *
 * 1. VERY FAST PRESS-RELEASE:
 *    - User taps button very quickly (< debounce time)
 *    - May miss the press entirely with slow debouncing
 *    - Solution: Use faster sample rate or accept trade-off
 *
 * 2. HELD BUTTON:
 *    - Button held for long time
 *    - Should generate single PRESS event, not repeated
 *    - Add long-press detection as separate feature
 *
 * 3. SIMULTANEOUS BUTTONS:
 *    - Multiple buttons pressed at once
 *    - Each needs independent debounce state
 *    - Don't share counters between buttons
 *
 * 4. INTERRUPT LATENCY:
 *    - If using edge interrupt + timestamp
 *    - Interrupt latency adds to response time
 *    - Consider interrupt priority
 *
 * 5. POWER NOISE:
 *    - Noisy power supply can affect GPIO readings
 *    - Add decoupling cap near button
 *    - Consider pull-up value (lower = more noise immune)
 *
 * ============================================================================
 */
