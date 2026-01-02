/*
 * ============================================================================
 * PROBLEM: State Machine - Table-Driven Implementation
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 20 mins | FREQUENCY: VERY HIGH (Embedded)
 *
 * Industry-standard approach using transition table. Scalable and maintainable.
 *
 * ============================================================================
 * EXAMPLE: Door Lock
 * ============================================================================
 *
 *     wrong_code         correct_code
 *    ┌──────────┐      ┌─────────────►┌──────────┐
 *    │          │      │              │          │
 *    │  LOCKED  ├──────┘              │ UNLOCKED │
 *    │          │◄─────────────────────┤          │
 *    └────┬─────┘     lock/timeout    └──────────┘
 *         │ 3 wrong
 *         ▼
 *    ┌──────────┐
 *    │  ALARM   │
 *    └──────────┘
 *
 * ============================================================================
 * ADVANTAGE OVER SWITCH-CASE:
 * ============================================================================
 *
 *   - Easy to add/remove states and events
 *   - Transition logic is data, not code
 *   - Can load transitions from config
 *   - Easier to test and validate
 *
 * ============================================================================
 * TIME: O(1) per transition | SPACE: O(states × events)
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// States
typedef enum {
    STATE_LOCKED,
    STATE_UNLOCKED,
    STATE_ALARM,
    STATE_COUNT
} DoorState;

// Events
typedef enum {
    EVT_CORRECT_CODE,
    EVT_WRONG_CODE,
    EVT_LOCK_CMD,
    EVT_TIMEOUT,
    EVT_RESET,
    EVT_COUNT
} DoorEvent;

// Forward declaration
typedef struct DoorLock DoorLock;

// Action function type
typedef void (*ActionFunc)(DoorLock* lock);

// Transition entry
typedef struct {
    DoorState nextState;
    ActionFunc action;
} Transition;

// Door lock context
struct DoorLock {
    DoorState state;
    int wrongAttempts;
    Transition table[STATE_COUNT][EVT_COUNT];
};

// State/Event names for printing
const char* stateNames[] = {"LOCKED", "UNLOCKED", "ALARM"};
const char* eventNames[] = {"CORRECT_CODE", "WRONG_CODE", "LOCK_CMD", "TIMEOUT", "RESET"};

// Action functions
void action_unlock(DoorLock* lock) {
    printf("  Action: Door UNLOCKED\n");
    lock->wrongAttempts = 0;
}

void action_lock(DoorLock* lock) {
    printf("  Action: Door LOCKED\n");
}

void action_wrongCode(DoorLock* lock) {
    lock->wrongAttempts++;
    printf("  Action: Wrong code! Attempt %d\n", lock->wrongAttempts);
}

void action_alarm(DoorLock* lock) {
    printf("  Action: ALARM TRIGGERED!\n");
    (void)lock;
}

void action_reset(DoorLock* lock) {
    printf("  Action: Alarm reset\n");
    lock->wrongAttempts = 0;
}

void action_none(DoorLock* lock) {
    (void)lock;
}

// Initialize state machine
void doorLock_init(DoorLock* lock) {
    lock->state = STATE_LOCKED;
    lock->wrongAttempts = 0;

    // Default: stay in same state, no action
    for (int s = 0; s < STATE_COUNT; s++) {
        for (int e = 0; e < EVT_COUNT; e++) {
            lock->table[s][e].nextState = s;
            lock->table[s][e].action = action_none;
        }
    }

    // Define transitions
    lock->table[STATE_LOCKED][EVT_CORRECT_CODE] = (Transition){STATE_UNLOCKED, action_unlock};
    lock->table[STATE_LOCKED][EVT_WRONG_CODE] = (Transition){STATE_LOCKED, action_wrongCode};

    lock->table[STATE_UNLOCKED][EVT_LOCK_CMD] = (Transition){STATE_LOCKED, action_lock};
    lock->table[STATE_UNLOCKED][EVT_TIMEOUT] = (Transition){STATE_LOCKED, action_lock};

    lock->table[STATE_ALARM][EVT_RESET] = (Transition){STATE_LOCKED, action_reset};
}

// Process event
void doorLock_processEvent(DoorLock* lock, DoorEvent event) {
    // Special check: 3 wrong attempts triggers alarm
    if (lock->state == STATE_LOCKED && event == EVT_WRONG_CODE && lock->wrongAttempts >= 2) {
        printf("[%s] + %s → [ALARM]\n", stateNames[lock->state], eventNames[event]);
        lock->state = STATE_ALARM;
        action_alarm(lock);
        return;
    }

    // Normal table lookup
    Transition* t = &lock->table[lock->state][event];

    printf("[%s] + %s → [%s]\n",
           stateNames[lock->state],
           eventNames[event],
           stateNames[t->nextState]);

    if (t->action) {
        t->action(lock);
    }

    lock->state = t->nextState;
}

int main() {
    printf("=== State Machine (Table-Driven) ===\n\n");

    DoorLock lock;
    doorLock_init(&lock);

    printf("Initial: %s\n\n", stateNames[lock.state]);

    printf("Scenario: 3 wrong codes → alarm → reset → correct code\n\n");

    doorLock_processEvent(&lock, EVT_WRONG_CODE);
    doorLock_processEvent(&lock, EVT_WRONG_CODE);
    doorLock_processEvent(&lock, EVT_WRONG_CODE);  // Triggers alarm
    printf("\n");

    doorLock_processEvent(&lock, EVT_RESET);
    doorLock_processEvent(&lock, EVT_CORRECT_CODE);
    doorLock_processEvent(&lock, EVT_TIMEOUT);  // Auto-lock

    printf("\n=== When to Use ===\n");
    printf("- Complex state machines (>5 states)\n");
    printf("- Many events and transitions\n");
    printf("- Need to modify transitions at runtime\n");
    printf("- Industry/production code\n");

    return 0;
}
