/*
 * ============================================================================
 * SYSTEM DESIGN: State Machine (FSM)
 * ============================================================================
 *
 * WHAT IS IT?
 * A system that can be in one of several states, transitions between states
 * based on events/inputs. Foundation of embedded firmware design.
 *
 * EXAMPLES:
 *   - Traffic light: RED -> GREEN -> YELLOW -> RED
 *   - Button: IDLE -> PRESSED -> RELEASED -> IDLE
 *   - Communication: IDLE -> CONNECTING -> CONNECTED -> ERROR
 *
 * WHY USE STATE MACHINES?
 *   - Clear, maintainable code
 *   - Easy to debug (just check current state)
 *   - Handles complex behavior systematically
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Define states
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_ERROR
} State;

// Define events
typedef enum {
    EVENT_START,
    EVENT_STOP,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_ERROR,
    EVENT_RESET
} Event;

// State machine structure
typedef struct {
    State current_state;
} StateMachine;

const char* state_to_string(State s) {
    switch(s) {
        case STATE_IDLE:    return "IDLE";
        case STATE_RUNNING: return "RUNNING";
        case STATE_PAUSED:  return "PAUSED";
        case STATE_ERROR:   return "ERROR";
        default:            return "UNKNOWN";
    }
}

void sm_init(StateMachine* sm) {
    sm->current_state = STATE_IDLE;
}

void sm_handle_event(StateMachine* sm, Event event) {
    State old_state = sm->current_state;

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) sm->current_state = STATE_RUNNING;
            break;

        case STATE_RUNNING:
            if (event == EVENT_STOP) sm->current_state = STATE_IDLE;
            else if (event == EVENT_PAUSE) sm->current_state = STATE_PAUSED;
            else if (event == EVENT_ERROR) sm->current_state = STATE_ERROR;
            break;

        case STATE_PAUSED:
            if (event == EVENT_RESUME) sm->current_state = STATE_RUNNING;
            else if (event == EVENT_STOP) sm->current_state = STATE_IDLE;
            break;

        case STATE_ERROR:
            if (event == EVENT_RESET) sm->current_state = STATE_IDLE;
            break;
    }

    if (old_state != sm->current_state) {
        printf("Transition: %s -> %s\n", state_to_string(old_state),
               state_to_string(sm->current_state));
    }
}

int main() {
    StateMachine sm;
    sm_init(&sm);

    printf("Initial: %s\n\n", state_to_string(sm.current_state));

    sm_handle_event(&sm, EVENT_START);   // IDLE -> RUNNING
    sm_handle_event(&sm, EVENT_PAUSE);   // RUNNING -> PAUSED
    sm_handle_event(&sm, EVENT_RESUME);  // PAUSED -> RUNNING
    sm_handle_event(&sm, EVENT_ERROR);   // RUNNING -> ERROR
    sm_handle_event(&sm, EVENT_RESET);   // ERROR -> IDLE

    return 0;
}

/*
 * ============================================================================
 * DRY RUN: State Transitions
 * ============================================================================
 *
 * Initial: STATE_IDLE
 *
 * EVENT_START:
 *   current = IDLE, event = START
 *   case IDLE: event == START? YES -> current = RUNNING
 *   Transition: IDLE -> RUNNING
 *
 * EVENT_PAUSE:
 *   current = RUNNING, event = PAUSE
 *   case RUNNING: event == PAUSE? YES -> current = PAUSED
 *   Transition: RUNNING -> PAUSED
 *
 * EVENT_RESUME:
 *   current = PAUSED, event = RESUME
 *   case PAUSED: event == RESUME? YES -> current = RUNNING
 *   Transition: PAUSED -> RUNNING
 *
 * EVENT_ERROR:
 *   current = RUNNING, event = ERROR
 *   case RUNNING: event == ERROR? YES -> current = ERROR
 *   Transition: RUNNING -> ERROR
 *
 * EVENT_RESET:
 *   current = ERROR, event = RESET
 *   case ERROR: event == RESET? YES -> current = IDLE
 *   Transition: ERROR -> IDLE
 *
 * STATE DIAGRAM:
 *
 *   +------+  START   +---------+  PAUSE  +--------+
 *   | IDLE | -------> | RUNNING | ------> | PAUSED |
 *   +------+          +---------+         +--------+
 *      ^                |    ^                |
 *      |    STOP        |    |    RESUME      |
 *      +----------------+    +----------------+
 *      |                |
 *      |  RESET    +-------+
 *      +-----------| ERROR |
 *                  +-------+
 *                      ^
 *                      | ERROR
 *                      |
 *                  (from RUNNING)
 *
 * ============================================================================
 * INTERVIEW TIPS:
 * - Use enum for states and events (readable, type-safe)
 * - Switch on current_state, then check events
 * - Entry/exit actions: code to run when entering/leaving state
 * - Table-driven approach for complex FSMs (array of function pointers)
 * ============================================================================
 */
