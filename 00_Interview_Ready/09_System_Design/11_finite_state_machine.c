/*
 * ============================================================================
 *        DESIGN: FINITE STATE MACHINE FOR REAL-WORLD SYSTEM
 * ============================================================================
 *
 * PROBLEM STATEMENT:
 *   Design a robust, maintainable FSM for embedded systems. The example
 *   implements a DOOR LOCK system, but the pattern applies to any system:
 *   - Device modes (idle, active, sleep, error)
 *   - Communication protocols (connect, authenticate, transfer)
 *   - User interfaces (menu navigation)
 *   - Motor control (off, accelerating, running, braking)
 *
 * ============================================================================
 *                    FSM THEORY
 * ============================================================================
 *
 * COMPONENTS OF AN FSM:
 *   1. STATES: Distinct modes the system can be in
 *   2. EVENTS: Inputs that can trigger transitions
 *   3. TRANSITIONS: Rules for moving between states
 *   4. ACTIONS: Operations performed during transitions
 *   5. GUARDS: Conditions that must be true for transition
 *
 * TYPES OF FSMs:
 *
 *   1. MEALY MACHINE:
 *      - Output depends on current state AND input
 *      - Actions on transitions
 *      - More responsive, can have fewer states
 *
 *   2. MOORE MACHINE:
 *      - Output depends only on current state
 *      - Actions on state entry/exit
 *      - Simpler to understand, more predictable
 *
 *   3. HIERARCHICAL STATE MACHINE (HSM):
 *      - States can contain sub-states
 *      - Reduces complexity for large systems
 *      - Parent state handles common transitions
 *
 * ============================================================================
 *                    DESIGN APPROACHES
 * ============================================================================
 *
 * APPROACH 1: SWITCH-CASE (Simple, but doesn't scale)
 *
 *   switch(state) {
 *       case STATE_A:
 *           if (event == EVENT_X) state = STATE_B;
 *           break;
 *       case STATE_B:
 *           ...
 *   }
 *
 *   Pros: Simple, no overhead, easy to debug
 *   Cons: Gets messy with many states/events, hard to maintain
 *
 *
 * APPROACH 2: STATE TABLE (Recommended for medium complexity)
 *
 *   Define transitions in a table:
 *   { current_state, event, next_state, action_function }
 *
 *   Pros: Clear structure, easy to modify, self-documenting
 *   Cons: Table can get large, all transitions explicit
 *
 *
 * APPROACH 3: FUNCTION POINTERS (Most flexible)
 *
 *   Each state is a function that handles its own events
 *   Returns next state or pointer to next state function
 *
 *   Pros: Encapsulated, each state handles own logic
 *   Cons: Harder to see full picture, debugging trickier
 *
 * ============================================================================
 *                    EXAMPLE: SMART DOOR LOCK
 * ============================================================================
 *
 * STATE DIAGRAM:
 *
 *                    timeout
 *              ┌─────────────────────┐
 *              │                     │
 *              ▼        valid_pin    │
 *         ┌────────┐ ─────────────► ┌──────────┐
 *   ─────►│ LOCKED │                │ UNLOCKED │
 *         └────────┘ ◄───────────── └──────────┘
 *              │        lock_cmd         │
 *              │                         │
 *   invalid_pin│                         │ handle_inside
 *   3x         │                         │
 *              ▼                         │
 *         ┌────────┐                     │
 *         │ ALARM  │◄────────────────────┘
 *         └────────┘     tamper
 *              │
 *              │ master_reset
 *              ▼
 *         ┌────────┐
 *         │ LOCKED │
 *         └────────┘
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// STATE AND EVENT DEFINITIONS
// ============================================================================

typedef enum {
    STATE_LOCKED,
    STATE_UNLOCKED,
    STATE_ALARM,
    STATE_PROGRAMMING,
    STATE_COUNT  // For array sizing
} DoorState;

typedef enum {
    EVENT_VALID_PIN,
    EVENT_INVALID_PIN,
    EVENT_LOCK_CMD,
    EVENT_TIMEOUT,
    EVENT_TAMPER,
    EVENT_MASTER_RESET,
    EVENT_PROGRAM_MODE,
    EVENT_COUNT  // For array sizing
} DoorEvent;

// String representations for debugging
const char* state_names[] = {
    "LOCKED", "UNLOCKED", "ALARM", "PROGRAMMING"
};

const char* event_names[] = {
    "VALID_PIN", "INVALID_PIN", "LOCK_CMD", "TIMEOUT",
    "TAMPER", "MASTER_RESET", "PROGRAM_MODE"
};


// ============================================================================
// APPROACH 1: SWITCH-CASE FSM (Simple)
// ============================================================================

typedef struct {
    DoorState state;
    uint8_t invalid_attempts;
    uint32_t unlock_time;
} SwitchCaseFSM;

void switch_fsm_init(SwitchCaseFSM* fsm) {
    fsm->state = STATE_LOCKED;
    fsm->invalid_attempts = 0;
    fsm->unlock_time = 0;
}

DoorState switch_fsm_handle(SwitchCaseFSM* fsm, DoorEvent event) {
    DoorState old_state = fsm->state;

    switch (fsm->state) {
        case STATE_LOCKED:
            switch (event) {
                case EVENT_VALID_PIN:
                    fsm->state = STATE_UNLOCKED;
                    fsm->invalid_attempts = 0;
                    printf("  Action: Unlock door, reset attempts\n");
                    break;

                case EVENT_INVALID_PIN:
                    fsm->invalid_attempts++;
                    printf("  Action: Invalid PIN, attempt %d\n", fsm->invalid_attempts);
                    if (fsm->invalid_attempts >= 3) {
                        fsm->state = STATE_ALARM;
                        printf("  Action: Trigger alarm!\n");
                    }
                    break;

                case EVENT_TAMPER:
                    fsm->state = STATE_ALARM;
                    printf("  Action: Tamper detected, alarm!\n");
                    break;

                case EVENT_PROGRAM_MODE:
                    fsm->state = STATE_PROGRAMMING;
                    printf("  Action: Enter programming mode\n");
                    break;

                default:
                    break;
            }
            break;

        case STATE_UNLOCKED:
            switch (event) {
                case EVENT_LOCK_CMD:
                    fsm->state = STATE_LOCKED;
                    printf("  Action: Lock door\n");
                    break;

                case EVENT_TIMEOUT:
                    fsm->state = STATE_LOCKED;
                    printf("  Action: Auto-lock on timeout\n");
                    break;

                case EVENT_TAMPER:
                    fsm->state = STATE_ALARM;
                    printf("  Action: Tamper while unlocked!\n");
                    break;

                default:
                    break;
            }
            break;

        case STATE_ALARM:
            switch (event) {
                case EVENT_MASTER_RESET:
                    fsm->state = STATE_LOCKED;
                    fsm->invalid_attempts = 0;
                    printf("  Action: Master reset, clear alarm\n");
                    break;

                default:
                    // Ignore other events in alarm state
                    printf("  Action: Event ignored in ALARM state\n");
                    break;
            }
            break;

        case STATE_PROGRAMMING:
            switch (event) {
                case EVENT_TIMEOUT:
                    fsm->state = STATE_LOCKED;
                    printf("  Action: Exit programming mode\n");
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

    if (fsm->state != old_state) {
        printf("  Transition: %s -> %s\n",
               state_names[old_state], state_names[fsm->state]);
    }

    return fsm->state;
}


// ============================================================================
// APPROACH 2: STATE TABLE FSM (Recommended)
// ============================================================================

// Forward declaration
typedef struct TableFSM TableFSM;

// Action function type
typedef void (*ActionFunc)(TableFSM* fsm);

// Transition entry in table
typedef struct {
    DoorState current_state;
    DoorEvent event;
    DoorState next_state;
    ActionFunc action;
} Transition;

// FSM context
struct TableFSM {
    DoorState state;
    uint8_t invalid_attempts;
    uint32_t timestamp;
};

// Action functions
void action_unlock(TableFSM* fsm) {
    printf("  Action: Unlocking door\n");
    fsm->invalid_attempts = 0;
}

void action_lock(TableFSM* fsm) {
    (void)fsm;
    printf("  Action: Locking door\n");
}

void action_invalid_attempt(TableFSM* fsm) {
    fsm->invalid_attempts++;
    printf("  Action: Invalid PIN, attempt %d\n", fsm->invalid_attempts);
}

void action_trigger_alarm(TableFSM* fsm) {
    (void)fsm;
    printf("  Action: ALARM TRIGGERED!\n");
}

void action_clear_alarm(TableFSM* fsm) {
    fsm->invalid_attempts = 0;
    printf("  Action: Alarm cleared\n");
}

void action_auto_lock(TableFSM* fsm) {
    (void)fsm;
    printf("  Action: Auto-lock timeout\n");
}

// State transition table
// NOTE: Order matters - first matching transition is used
const Transition transition_table[] = {
    // From LOCKED
    { STATE_LOCKED,    EVENT_VALID_PIN,    STATE_UNLOCKED,   action_unlock },
    { STATE_LOCKED,    EVENT_INVALID_PIN,  STATE_LOCKED,     action_invalid_attempt },
    { STATE_LOCKED,    EVENT_TAMPER,       STATE_ALARM,      action_trigger_alarm },

    // From UNLOCKED
    { STATE_UNLOCKED,  EVENT_LOCK_CMD,     STATE_LOCKED,     action_lock },
    { STATE_UNLOCKED,  EVENT_TIMEOUT,      STATE_LOCKED,     action_auto_lock },
    { STATE_UNLOCKED,  EVENT_TAMPER,       STATE_ALARM,      action_trigger_alarm },

    // From ALARM
    { STATE_ALARM,     EVENT_MASTER_RESET, STATE_LOCKED,     action_clear_alarm },
};

#define TRANSITION_COUNT (sizeof(transition_table) / sizeof(Transition))

void table_fsm_init(TableFSM* fsm) {
    fsm->state = STATE_LOCKED;
    fsm->invalid_attempts = 0;
    fsm->timestamp = 0;
}

DoorState table_fsm_handle(TableFSM* fsm, DoorEvent event) {
    DoorState old_state = fsm->state;

    // Search for matching transition
    for (size_t i = 0; i < TRANSITION_COUNT; i++) {
        const Transition* t = &transition_table[i];

        if (t->current_state == fsm->state && t->event == event) {
            // Execute action
            if (t->action) {
                t->action(fsm);
            }

            // Transition to new state
            fsm->state = t->next_state;

            printf("  Transition: %s -> %s\n",
                   state_names[old_state], state_names[fsm->state]);
            return fsm->state;
        }
    }

    // No transition found
    printf("  No transition for event %s in state %s\n",
           event_names[event], state_names[fsm->state]);
    return fsm->state;
}

// Special handling: check for alarm condition after invalid attempts
DoorState table_fsm_handle_with_guard(TableFSM* fsm, DoorEvent event) {
    DoorState result = table_fsm_handle(fsm, event);

    // Guard condition: Too many invalid attempts
    if (event == EVENT_INVALID_PIN && fsm->invalid_attempts >= 3) {
        printf("  Guard: 3 invalid attempts reached\n");
        fsm->state = STATE_ALARM;
        action_trigger_alarm(fsm);
        printf("  Transition: %s -> %s (via guard)\n",
               state_names[result], state_names[fsm->state]);
    }

    return fsm->state;
}


// ============================================================================
// APPROACH 3: FUNCTION POINTER FSM (Most Flexible)
// ============================================================================

// Forward declaration
typedef struct FuncPtrFSM FuncPtrFSM;

// State handler function type
typedef void (*StateHandler)(FuncPtrFSM* fsm, DoorEvent event);

struct FuncPtrFSM {
    StateHandler current_handler;
    DoorState state;  // For debugging/logging
    uint8_t invalid_attempts;
};

// Forward declarations of state handlers
void state_locked_handler(FuncPtrFSM* fsm, DoorEvent event);
void state_unlocked_handler(FuncPtrFSM* fsm, DoorEvent event);
void state_alarm_handler(FuncPtrFSM* fsm, DoorEvent event);

void funcptr_fsm_transition(FuncPtrFSM* fsm, DoorState new_state,
                            StateHandler handler) {
    printf("  Transition: %s -> %s\n",
           state_names[fsm->state], state_names[new_state]);
    fsm->state = new_state;
    fsm->current_handler = handler;
}

void state_locked_handler(FuncPtrFSM* fsm, DoorEvent event) {
    switch (event) {
        case EVENT_VALID_PIN:
            printf("  Action: Unlock\n");
            fsm->invalid_attempts = 0;
            funcptr_fsm_transition(fsm, STATE_UNLOCKED, state_unlocked_handler);
            break;

        case EVENT_INVALID_PIN:
            fsm->invalid_attempts++;
            printf("  Action: Invalid attempt %d\n", fsm->invalid_attempts);
            if (fsm->invalid_attempts >= 3) {
                funcptr_fsm_transition(fsm, STATE_ALARM, state_alarm_handler);
            }
            break;

        case EVENT_TAMPER:
            funcptr_fsm_transition(fsm, STATE_ALARM, state_alarm_handler);
            break;

        default:
            break;
    }
}

void state_unlocked_handler(FuncPtrFSM* fsm, DoorEvent event) {
    switch (event) {
        case EVENT_LOCK_CMD:
        case EVENT_TIMEOUT:
            printf("  Action: Lock\n");
            funcptr_fsm_transition(fsm, STATE_LOCKED, state_locked_handler);
            break;

        case EVENT_TAMPER:
            funcptr_fsm_transition(fsm, STATE_ALARM, state_alarm_handler);
            break;

        default:
            break;
    }
}

void state_alarm_handler(FuncPtrFSM* fsm, DoorEvent event) {
    if (event == EVENT_MASTER_RESET) {
        printf("  Action: Clear alarm\n");
        fsm->invalid_attempts = 0;
        funcptr_fsm_transition(fsm, STATE_LOCKED, state_locked_handler);
    }
    // Ignore all other events in alarm state
}

void funcptr_fsm_init(FuncPtrFSM* fsm) {
    fsm->current_handler = state_locked_handler;
    fsm->state = STATE_LOCKED;
    fsm->invalid_attempts = 0;
}

void funcptr_fsm_handle(FuncPtrFSM* fsm, DoorEvent event) {
    printf("  Event: %s in state %s\n",
           event_names[event], state_names[fsm->state]);
    fsm->current_handler(fsm, event);
}


// ============================================================================
// ENTRY/EXIT ACTIONS (Moore Machine Enhancement)
// ============================================================================

/*
 * Many FSMs need actions when entering/exiting states, not just on transitions.
 *
 * Example:
 *   - On enter UNLOCKED: Turn on green LED, start timeout timer
 *   - On exit UNLOCKED: Turn off green LED, stop timer
 *   - On enter ALARM: Sound buzzer, flash red LED
 *   - On exit ALARM: Stop buzzer
 */

typedef void (*StateAction)(void);

typedef struct {
    StateAction on_enter;
    StateAction on_exit;
} StateActions;

void on_enter_locked(void)   { printf("  [ENTER] Lock engaged, LED red\n"); }
void on_exit_locked(void)    { printf("  [EXIT] Lock disengaging\n"); }
void on_enter_unlocked(void) { printf("  [ENTER] LED green, timer started\n"); }
void on_exit_unlocked(void)  { printf("  [EXIT] LED off, timer stopped\n"); }
void on_enter_alarm(void)    { printf("  [ENTER] BUZZER ON, LED flashing\n"); }
void on_exit_alarm(void)     { printf("  [EXIT] Buzzer off\n"); }

const StateActions state_actions[STATE_COUNT] = {
    [STATE_LOCKED]   = { on_enter_locked,   on_exit_locked },
    [STATE_UNLOCKED] = { on_enter_unlocked, on_exit_unlocked },
    [STATE_ALARM]    = { on_enter_alarm,    on_exit_alarm },
    [STATE_PROGRAMMING] = { NULL, NULL },
};

void transition_with_actions(DoorState* current, DoorState next) {
    if (*current == next) return;  // No transition

    // Exit current state
    if (state_actions[*current].on_exit) {
        state_actions[*current].on_exit();
    }

    printf("  Transition: %s -> %s\n", state_names[*current], state_names[next]);

    // Enter new state
    if (state_actions[next].on_enter) {
        state_actions[next].on_enter();
    }

    *current = next;
}


// ============================================================================
// DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   FINITE STATE MACHINE DEMONSTRATION\n");
    printf("============================================\n");

    // Demo 1: Switch-case FSM
    printf("\n--- SWITCH-CASE FSM ---\n");
    SwitchCaseFSM sw_fsm;
    switch_fsm_init(&sw_fsm);

    printf("\nEvent: VALID_PIN\n");
    switch_fsm_handle(&sw_fsm, EVENT_VALID_PIN);

    printf("\nEvent: TIMEOUT\n");
    switch_fsm_handle(&sw_fsm, EVENT_TIMEOUT);

    printf("\nEvent: INVALID_PIN x3\n");
    switch_fsm_handle(&sw_fsm, EVENT_INVALID_PIN);
    switch_fsm_handle(&sw_fsm, EVENT_INVALID_PIN);
    switch_fsm_handle(&sw_fsm, EVENT_INVALID_PIN);

    // Demo 2: Table-based FSM
    printf("\n--- TABLE-BASED FSM ---\n");
    TableFSM tbl_fsm;
    table_fsm_init(&tbl_fsm);

    printf("\nEvent: VALID_PIN\n");
    table_fsm_handle(&tbl_fsm, EVENT_VALID_PIN);

    printf("\nEvent: TAMPER\n");
    table_fsm_handle(&tbl_fsm, EVENT_TAMPER);

    printf("\nEvent: MASTER_RESET\n");
    table_fsm_handle(&tbl_fsm, EVENT_MASTER_RESET);

    // Demo 3: Function pointer FSM
    printf("\n--- FUNCTION POINTER FSM ---\n");
    FuncPtrFSM fp_fsm;
    funcptr_fsm_init(&fp_fsm);

    funcptr_fsm_handle(&fp_fsm, EVENT_VALID_PIN);
    funcptr_fsm_handle(&fp_fsm, EVENT_LOCK_CMD);

    // Demo 4: Entry/Exit actions
    printf("\n--- ENTRY/EXIT ACTIONS ---\n");
    DoorState state = STATE_LOCKED;
    transition_with_actions(&state, STATE_UNLOCKED);
    transition_with_actions(&state, STATE_ALARM);
    transition_with_actions(&state, STATE_LOCKED);

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "Which FSM approach would you use and why?"
 * A: "For medium complexity (5-15 states), I prefer table-based:
 *    1. Self-documenting - table shows all transitions
 *    2. Easy to modify - add row to table
 *    3. Easy to test - iterate through table
 *    4. Clear separation of logic and data
 *
 *    For simple systems (< 5 states): switch-case is fine.
 *    For complex systems or HSM: function pointer approach."
 *
 * Q: "How do you handle unexpected events?"
 * A: "Options:
 *    1. Ignore silently (log for debugging)
 *    2. Stay in current state (safe default)
 *    3. Transition to error state
 *    4. Assert/reset in debug builds
 *
 *    Choice depends on criticality. Safety-critical = error state."
 *
 * Q: "How do you test an FSM?"
 * A: "1. Enumerate all state-event combinations
 *    2. Verify expected transitions occur
 *    3. Verify unexpected events handled gracefully
 *    4. Test guard conditions at boundaries
 *    5. Verify entry/exit actions execute correctly
 *    6. Test state persistence across power cycles"
 *
 * Q: "What's a hierarchical state machine?"
 * A: "States can have sub-states. Parent state handles common events,
 *    child states handle specific events. Reduces code duplication.
 *
 *    Example: UNLOCKED has sub-states { NORMAL, TIMED_HOLD }
 *    Both respond to TAMPER the same way (handled by parent)."
 *
 * ============================================================================
 *                    EDGE CASES
 * ============================================================================
 *
 * 1. SELF-TRANSITIONS:
 *    Same state -> Same state, but action still executes
 *    May or may not run entry/exit actions (design decision)
 *
 * 2. GUARD CONDITIONS:
 *    Transition only if condition is true
 *    Example: unlock only if battery > 10%
 *
 * 3. DEFERRED EVENTS:
 *    Event can't be handled now, queue for later
 *    Example: Can't lock while programming
 *
 * 4. INTERNAL TRANSITIONS:
 *    Action without state change (Mealy machine behavior)
 *    Don't trigger entry/exit actions
 *
 * 5. TIMEOUT EVENTS:
 *    Need timer integration
 *    Reset timer on state entry, generate event on expiry
 *
 * ============================================================================
 */
