// State Machine - ESSENTIAL pattern for embedded protocol handling
// Time: O(1) per event, Space: O(states * events) for table

#include <stdio.h>
#include <stdint.h>

// Say: "Define states as enum for readability"
typedef enum {
    STATE_IDLE,
    STATE_WAITING,
    STATE_PROCESSING,
    STATE_ERROR,
    STATE_COUNT  // Say: "Useful for array sizing"
} State;

// Say: "Define events that trigger transitions"
typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_COMPLETE,
    EVENT_TIMEOUT,
    EVENT_RESET,
    EVENT_COUNT
} Event;

// Say: "State names for debugging"
const char* state_names[] = {"IDLE", "WAITING", "PROCESSING", "ERROR"};
const char* event_names[] = {"START", "DATA_READY", "COMPLETE", "TIMEOUT", "RESET"};

// Say: "State machine structure"
typedef struct {
    State current_state;
    uint32_t entry_count;  // Say: "Track times state entered"
} StateMachine;

// Say: "Action functions - called on transitions"
void action_start_processing(void) {
    printf("  [Action] Starting processing...\n");
}

void action_process_data(void) {
    printf("  [Action] Processing data...\n");
}

void action_complete(void) {
    printf("  [Action] Operation complete!\n");
}

void action_handle_error(void) {
    printf("  [Action] Handling error...\n");
}

void action_reset(void) {
    printf("  [Action] System reset\n");
}

// Say: "Simple switch-case state machine (common approach)"
void state_machine_process_event(StateMachine *sm, Event event) {
    State next_state = sm->current_state;

    printf("Event: %s in state %s\n",
           event_names[event], state_names[sm->current_state]);

    // Say: "Outer switch on current state"
    switch (sm->current_state) {
        case STATE_IDLE:
            // Say: "Inner switch on event"
            switch (event) {
                case EVENT_START:
                    action_start_processing();
                    next_state = STATE_WAITING;
                    break;
                case EVENT_RESET:
                    action_reset();
                    next_state = STATE_IDLE;
                    break;
                default:
                    // Say: "Ignore invalid events"
                    printf("  [Ignored]\n");
                    break;
            }
            break;

        case STATE_WAITING:
            switch (event) {
                case EVENT_DATA_READY:
                    action_process_data();
                    next_state = STATE_PROCESSING;
                    break;
                case EVENT_TIMEOUT:
                    action_handle_error();
                    next_state = STATE_ERROR;
                    break;
                case EVENT_RESET:
                    action_reset();
                    next_state = STATE_IDLE;
                    break;
                default:
                    printf("  [Ignored]\n");
                    break;
            }
            break;

        case STATE_PROCESSING:
            switch (event) {
                case EVENT_COMPLETE:
                    action_complete();
                    next_state = STATE_IDLE;
                    break;
                case EVENT_TIMEOUT:
                    action_handle_error();
                    next_state = STATE_ERROR;
                    break;
                case EVENT_RESET:
                    action_reset();
                    next_state = STATE_IDLE;
                    break;
                default:
                    printf("  [Ignored]\n");
                    break;
            }
            break;

        case STATE_ERROR:
            switch (event) {
                case EVENT_RESET:
                    action_reset();
                    next_state = STATE_IDLE;
                    break;
                default:
                    printf("  [Ignored - in error state]\n");
                    break;
            }
            break;

        default:
            printf("  [Unknown state!]\n");
            break;
    }

    // Say: "Update state if changed"
    if (next_state != sm->current_state) {
        printf("  Transition: %s -> %s\n",
               state_names[sm->current_state], state_names[next_state]);
        sm->current_state = next_state;
        sm->entry_count++;
    }
}

void state_machine_init(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->entry_count = 0;
}

int main() {
    StateMachine sm;
    state_machine_init(&sm);

    printf("=== State Machine Demo ===\n\n");

    // Normal flow
    state_machine_process_event(&sm, EVENT_START);
    state_machine_process_event(&sm, EVENT_DATA_READY);
    state_machine_process_event(&sm, EVENT_COMPLETE);

    printf("\n--- Testing error path ---\n");
    state_machine_process_event(&sm, EVENT_START);
    state_machine_process_event(&sm, EVENT_TIMEOUT);  // Goes to error
    state_machine_process_event(&sm, EVENT_START);    // Ignored in error
    state_machine_process_event(&sm, EVENT_RESET);    // Recover

    printf("\n--- Testing invalid event ---\n");
    state_machine_process_event(&sm, EVENT_COMPLETE);  // Ignored in IDLE

    printf("\nTotal state changes: %u\n", sm.entry_count);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"State machine is a pattern where behavior depends on current state and input events.

 COMPONENTS:
 1. States: Finite set of conditions (IDLE, WAITING, etc.)
 2. Events: Inputs that trigger transitions
 3. Transitions: Rules for state changes
 4. Actions: Code executed on transitions

 IMPLEMENTATION OPTIONS:

 1. SWITCH-CASE (shown here):
    - Simple, readable
    - Good for small state machines
    - Can get long for complex machines

 2. TABLE-DRIVEN:
    State transition_table[STATE_COUNT][EVENT_COUNT];
    - Compact, data-driven
    - Easy to modify transitions
    - Better for large state machines

 3. FUNCTION POINTER:
    typedef void (*StateHandler)(Event);
    StateHandler handlers[STATE_COUNT];
    - Each state has its own handler function
    - Clean separation of concerns

 WHY STATE MACHINES:
 1. Clear structure for complex logic
 2. Easy to test (known states/transitions)
 3. Debuggable (can log state changes)
 4. Handles unexpected events gracefully

 EMBEDDED APPLICATIONS:
 1. Protocol parsers (UART frame reception)
 2. Button debouncing
 3. Motor control sequences
 4. Menu navigation
 5. Communication state (idle, connecting, connected)
 6. Boot sequences

 BEST PRACTICES:
 - All states handle all events (even if ignored)
 - Have error recovery path
 - Log state transitions for debugging
 - Use enums for type safety
 - Consider entry/exit actions

 COMMON MISTAKES:
 - Missing transitions (unhandled events)
 - No error recovery state
 - State variables scattered (not in struct)
 - Not handling unexpected events"
*/
