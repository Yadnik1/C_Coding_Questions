// Button Debounce - ESSENTIAL for embedded input handling
// Time: O(1) per call, Space: O(1)

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Say: "Debounce time in milliseconds - typical range 10-50ms"
#define DEBOUNCE_TIME_MS 20

// Say: "Button debounce structure"
typedef struct {
    bool current_state;      // Say: "Debounced (stable) state"
    bool last_reading;       // Say: "Last raw reading"
    uint32_t last_change_time;  // Say: "Timestamp of last change"
} ButtonDebounce;

// Say: "Simulated time - in real system use timer/systick"
static uint32_t system_time_ms = 0;

uint32_t get_system_time(void) {
    return system_time_ms;
}

void advance_time(uint32_t ms) {
    system_time_ms += ms;
}

// Say: "Initialize debounce structure"
void debounce_init(ButtonDebounce *btn, bool initial_state) {
    btn->current_state = initial_state;
    btn->last_reading = initial_state;
    btn->last_change_time = get_system_time();
}

// Say: "Process button reading - call periodically (every 1-5ms)"
bool debounce_update(ButtonDebounce *btn, bool raw_reading) {
    uint32_t current_time = get_system_time();

    // Say: "If reading changed, reset timer"
    if (raw_reading != btn->last_reading) {
        btn->last_change_time = current_time;
        btn->last_reading = raw_reading;
    }

    // Say: "If reading stable for debounce time, update state"
    if ((current_time - btn->last_change_time) >= DEBOUNCE_TIME_MS) {
        if (raw_reading != btn->current_state) {
            btn->current_state = raw_reading;
            return true;  // Say: "State changed - return true"
        }
    }

    return false;  // Say: "No state change"
}

// Say: "Get current debounced state"
bool debounce_get_state(ButtonDebounce *btn) {
    return btn->current_state;
}

// Alternative: Integrating/Counting debounce
typedef struct {
    uint8_t history;      // Say: "8-bit shift register of readings"
    bool state;
} ShiftDebounce;

void shift_debounce_init(ShiftDebounce *btn) {
    btn->history = 0x00;  // Say: "All zeros = button released"
    btn->state = false;
}

// Say: "Shift in new reading, check for stability"
bool shift_debounce_update(ShiftDebounce *btn, bool reading) {
    // Say: "Shift history left, add new reading"
    btn->history = (btn->history << 1) | (reading ? 1 : 0);

    bool prev_state = btn->state;

    // Say: "All 1s = stable pressed, all 0s = stable released"
    if (btn->history == 0xFF) {
        btn->state = true;
    } else if (btn->history == 0x00) {
        btn->state = false;
    }
    // Say: "Otherwise keep previous state (not stable yet)"

    return (btn->state != prev_state);  // Say: "Return true if changed"
}

int main() {
    printf("=== Button Debounce Demo ===\n\n");

    ButtonDebounce btn;
    debounce_init(&btn, false);  // Say: "Button starts released"

    // Say: "Simulate button press with bounce"
    printf("Simulating button press with bounce:\n");

    // Bouncing readings (button physically bouncing)
    bool readings[] = {
        0, 0, 0,      // Stable released
        1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1,  // Bouncing then stable pressed
        1, 1, 1,      // Stable pressed
        0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0   // Bouncing then stable released
    };
    int n = sizeof(readings) / sizeof(readings[0]);

    for (int i = 0; i < n; i++) {
        advance_time(5);  // Say: "5ms per reading"

        bool changed = debounce_update(&btn, readings[i]);

        printf("t=%3dms raw=%d debounced=%d %s\n",
               get_system_time(),
               readings[i],
               debounce_get_state(&btn),
               changed ? "<-- STATE CHANGED" : "");
    }

    // Shift register method demo
    printf("\n=== Shift Register Method ===\n");
    ShiftDebounce sbtn;
    shift_debounce_init(&sbtn);

    bool readings2[] = {0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0};
    for (int i = 0; i < 24; i++) {
        bool changed = shift_debounce_update(&sbtn, readings2[i]);
        printf("raw=%d history=0x%02X state=%d %s\n",
               readings2[i], sbtn.history, sbtn.state,
               changed ? "<-- CHANGED" : "");
    }

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Button debouncing filters out mechanical bounce when a button is pressed.

 THE PROBLEM:
 - Mechanical buttons bounce when pressed/released
 - Can produce multiple false edges in 1-50ms
 - Without debouncing, one press might register multiple times

 DEBOUNCING METHODS:

 1. TIME-BASED (shown first):
    - Record timestamp when reading changes
    - Only accept new state if stable for debounce period
    - Simple and effective

 2. SHIFT REGISTER (shown second):
    - Keep history of last N readings in a byte
    - All 1s = pressed, all 0s = released
    - Requires consistent sampling rate

 3. COUNTER-BASED:
    - Increment counter if reading matches expected
    - Reset if reading changes
    - Accept when counter reaches threshold

 4. HARDWARE:
    - RC filter on button pin
    - Schmitt trigger IC
    - Most reliable but adds components

 TYPICAL DEBOUNCE TIMES:
 - 10-20ms for good quality switches
 - 20-50ms for cheap tactile switches
 - Up to 100ms for very noisy switches

 IMPLEMENTATION TIPS:
 1. Sample at regular intervals (1-5ms)
 2. Use timer interrupt for consistent sampling
 3. Debounce period > expected bounce time
 4. Consider both press and release

 DETECTING EDGES:
 - Rising edge: prev=0, current=1 (button pressed)
 - Falling edge: prev=1, current=0 (button released)
 - Store previous state to detect edges

 COMMON MISTAKES:
 - Debounce time too short (still bouncing)
 - Not debouncing release (only press)
 - Blocking delay (wastes CPU time)
 - Not using volatile for ISR shared data

 INTERVIEW TIP:
 Explain that you'd use timer-based approach in real embedded system,
 not blocking delay. Show awareness of interrupt context."
*/
