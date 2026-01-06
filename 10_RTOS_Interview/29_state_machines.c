/*
 * ============================================================================
 * QUESTION 29: How do you implement State Machines in RTOS?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: High | IMPORTANCE: High
 *
 * State machines are fundamental to embedded system design.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "State machines in RTOS can be implemented in two main ways: (1) Single task
 * with switch-case on current state, processing events from a queue - good for
 * simpler state machines. (2) Active object pattern where each state machine
 * is a task with its own event queue - better isolation and concurrency.
 * Event groups are useful when state depends on multiple conditions. Key
 * principle: state machine should never block indefinitely in a state - always
 * wait on queue/semaphore for events or timeout. Use hierarchical state
 * machines (HSM) for complex systems with nested states."
 *
 * ============================================================================
 * VISUALIZATION: State Machine Basics
 * ============================================================================
 *
 *   SIMPLE STATE DIAGRAM (Communication Protocol):
 *
 *                    +-----------+
 *                    |   IDLE    |<---------+
 *                    +-----------+          |
 *                          |                |
 *                    send_request()         |
 *                          |                |
 *                          v                |
 *                    +-----------+          |
 *                    | WAIT_ACK  |          |
 *                    +-----------+          |
 *                      |      |             |
 *              ack_rcvd()   timeout()       |
 *                      |      |             |
 *                      v      v             |
 *                    +-----------+          |
 *                    | CONNECTED |          |
 *                    +-----------+          |
 *                          |                |
 *                    disconnect()           |
 *                          |                |
 *                          +----------------+
 *
 *
 *   STATE TRANSITION TABLE:
 *
 *   +-------------+---------------+-------------+---------------+
 *   | State       | Event         | Next State  | Action        |
 *   +-------------+---------------+-------------+---------------+
 *   | IDLE        | SEND_REQUEST  | WAIT_ACK    | send_req()    |
 *   | WAIT_ACK    | ACK_RECEIVED  | CONNECTED   | start_timer() |
 *   | WAIT_ACK    | TIMEOUT       | IDLE        | retry_count++ |
 *   | CONNECTED   | DISCONNECT    | IDLE        | close_conn()  |
 *   | CONNECTED   | DATA_RECV     | CONNECTED   | process()     |
 *   +-------------+---------------+-------------+---------------+
 *
 * ============================================================================
 * PATTERN 1: Switch-Case State Machine
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// State definitions
typedef enum {
    STATE_IDLE,
    STATE_WAIT_ACK,
    STATE_CONNECTED,
    STATE_ERROR,
    STATE_COUNT
} State_t;

// Event definitions
typedef enum {
    EVENT_SEND_REQUEST,
    EVENT_ACK_RECEIVED,
    EVENT_TIMEOUT,
    EVENT_DISCONNECT,
    EVENT_DATA_RECV,
    EVENT_ERROR,
    EVENT_COUNT
} Event_t;

// Event data structure
typedef struct {
    Event_t type;
    uint32_t data;
} EventMsg_t;

/*
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// ============================================================================
// IMPLEMENTATION 1: Single Task with Switch-Case
// ============================================================================

typedef struct {
    State_t current_state;
    uint8_t retry_count;
    TimerHandle_t timeout_timer;
    QueueHandle_t event_queue;
} ProtocolSM_t;

ProtocolSM_t protocol_sm;

// State entry actions
void enter_idle(ProtocolSM_t *sm) {
    printf("Entering IDLE state\n");
    xTimerStop(sm->timeout_timer, 0);
}

void enter_wait_ack(ProtocolSM_t *sm) {
    printf("Entering WAIT_ACK state\n");
    send_request_packet();
    xTimerChangePeriod(sm->timeout_timer, pdMS_TO_TICKS(1000), 0);
    xTimerStart(sm->timeout_timer, 0);
}

void enter_connected(ProtocolSM_t *sm) {
    printf("Entering CONNECTED state\n");
    sm->retry_count = 0;
}

// State machine task
void protocol_sm_task(void *pvParameters) {
    ProtocolSM_t *sm = (ProtocolSM_t *)pvParameters;
    EventMsg_t event;

    // Initialize
    sm->current_state = STATE_IDLE;
    enter_idle(sm);

    for (;;) {
        // Wait for event (with timeout for periodic checks)
        if (xQueueReceive(sm->event_queue, &event, pdMS_TO_TICKS(100)) == pdPASS) {

            // Process event based on current state
            switch (sm->current_state) {

                case STATE_IDLE:
                    switch (event.type) {
                        case EVENT_SEND_REQUEST:
                            sm->current_state = STATE_WAIT_ACK;
                            enter_wait_ack(sm);
                            break;
                        default:
                            // Ignore other events in this state
                            break;
                    }
                    break;

                case STATE_WAIT_ACK:
                    switch (event.type) {
                        case EVENT_ACK_RECEIVED:
                            sm->current_state = STATE_CONNECTED;
                            enter_connected(sm);
                            break;
                        case EVENT_TIMEOUT:
                            if (sm->retry_count < 3) {
                                sm->retry_count++;
                                enter_wait_ack(sm);  // Retry
                            } else {
                                sm->current_state = STATE_IDLE;
                                enter_idle(sm);
                            }
                            break;
                        default:
                            break;
                    }
                    break;

                case STATE_CONNECTED:
                    switch (event.type) {
                        case EVENT_DATA_RECV:
                            process_received_data(event.data);
                            break;
                        case EVENT_DISCONNECT:
                        case EVENT_TIMEOUT:
                            sm->current_state = STATE_IDLE;
                            enter_idle(sm);
                            break;
                        default:
                            break;
                    }
                    break;

                default:
                    // Unknown state - recover
                    sm->current_state = STATE_IDLE;
                    enter_idle(sm);
                    break;
            }
        }
    }
}

// Helper to send events
void send_event(ProtocolSM_t *sm, Event_t type, uint32_t data) {
    EventMsg_t event = {.type = type, .data = data};
    xQueueSend(sm->event_queue, &event, pdMS_TO_TICKS(100));
}

// Timer callback - sends timeout event
void timeout_callback(TimerHandle_t timer) {
    send_event(&protocol_sm, EVENT_TIMEOUT, 0);
}


// ============================================================================
// IMPLEMENTATION 2: Table-Driven State Machine
// ============================================================================

typedef void (*ActionFunc)(void *context);
typedef bool (*GuardFunc)(void *context);

typedef struct {
    State_t current_state;
    Event_t event;
    State_t next_state;
    GuardFunc guard;      // Optional condition
    ActionFunc action;    // Optional action
} Transition_t;

// Action functions
void action_send_request(void *ctx) { send_request_packet(); }
void action_retry(void *ctx) { ProtocolSM_t *sm = ctx; sm->retry_count++; }
void action_reset_retry(void *ctx) { ProtocolSM_t *sm = ctx; sm->retry_count = 0; }

// Guard functions
bool guard_can_retry(void *ctx) { ProtocolSM_t *sm = ctx; return sm->retry_count < 3; }
bool guard_max_retries(void *ctx) { ProtocolSM_t *sm = ctx; return sm->retry_count >= 3; }

// Transition table
const Transition_t transitions[] = {
    // State          Event              Next State       Guard           Action
    {STATE_IDLE,      EVENT_SEND_REQUEST, STATE_WAIT_ACK, NULL,           action_send_request},
    {STATE_WAIT_ACK,  EVENT_ACK_RECEIVED, STATE_CONNECTED, NULL,          action_reset_retry},
    {STATE_WAIT_ACK,  EVENT_TIMEOUT,      STATE_WAIT_ACK, guard_can_retry, action_retry},
    {STATE_WAIT_ACK,  EVENT_TIMEOUT,      STATE_IDLE,     guard_max_retries, NULL},
    {STATE_CONNECTED, EVENT_DISCONNECT,   STATE_IDLE,     NULL,           NULL},
    {STATE_CONNECTED, EVENT_DATA_RECV,    STATE_CONNECTED, NULL,          NULL},
};

#define NUM_TRANSITIONS (sizeof(transitions) / sizeof(transitions[0]))

void process_event_table(ProtocolSM_t *sm, Event_t event) {
    for (int i = 0; i < NUM_TRANSITIONS; i++) {
        const Transition_t *t = &transitions[i];

        if (t->current_state == sm->current_state && t->event == event) {
            // Check guard if present
            if (t->guard == NULL || t->guard(sm)) {
                // Execute action if present
                if (t->action != NULL) {
                    t->action(sm);
                }
                // Transition to next state
                sm->current_state = t->next_state;
                return;
            }
        }
    }
    // No matching transition - ignore event
}
*/

/*
 * ============================================================================
 * PATTERN 2: Active Object Pattern
 * ============================================================================
 *
 * Each state machine is encapsulated in its own task with private queue.
 */

/*
// Active Object - Each has its own task and queue
typedef struct {
    TaskHandle_t task;
    QueueHandle_t event_queue;
    State_t state;
    // State-specific data
    uint8_t retry_count;
} ActiveObject_t;

void active_object_task(void *pvParameters) {
    ActiveObject_t *ao = (ActiveObject_t *)pvParameters;
    EventMsg_t event;

    for (;;) {
        // Wait for event
        xQueueReceive(ao->event_queue, &event, portMAX_DELAY);

        // Process event (state machine logic)
        process_event(ao, &event);
    }
}

// Create active object
ActiveObject_t* active_object_create(uint16_t queue_size, uint16_t stack_size,
                                      UBaseType_t priority) {
    ActiveObject_t *ao = pvPortMalloc(sizeof(ActiveObject_t));

    ao->event_queue = xQueueCreate(queue_size, sizeof(EventMsg_t));
    ao->state = STATE_IDLE;
    ao->retry_count = 0;

    xTaskCreate(active_object_task, "AO", stack_size, ao, priority, &ao->task);

    return ao;
}

// Post event to active object
void active_object_post(ActiveObject_t *ao, Event_t type, uint32_t data) {
    EventMsg_t event = {.type = type, .data = data};
    xQueueSend(ao->event_queue, &event, portMAX_DELAY);
}
*/

/*
 * ============================================================================
 * HIERARCHICAL STATE MACHINE (HSM)
 * ============================================================================
 *
 *   NESTED STATES:
 *
 *   +---------------------------+
 *   |        OPERATIONAL        |
 *   |  +----------+----------+  |
 *   |  |  IDLE    |  ACTIVE  |  |
 *   |  |          | +------+ |  |
 *   |  |          | |RUN   | |  |
 *   |  |          | +------+ |  |
 *   |  |          | |PAUSE | |  |
 *   |  |          | +------+ |  |
 *   |  +----------+----------+  |
 *   +---------------------------+
 *   |          ERROR            |
 *   +---------------------------+
 *
 *   Event "stop" in RUN or PAUSE goes to IDLE
 *   Event "error" from any OPERATIONAL state goes to ERROR
 *   Nested states inherit parent transitions!
 */

/*
// Hierarchical state machine structure
typedef struct StateHandler StateHandler_t;

struct StateHandler {
    StateHandler_t *parent;           // Parent state (for hierarchy)
    void (*entry)(void *ctx);         // Entry action
    void (*exit)(void *ctx);          // Exit action
    StateHandler_t* (*handler)(void *ctx, Event_t event);  // Event handler
};

// HSM processing
void hsm_dispatch(StateHandler_t **current, void *ctx, Event_t event) {
    StateHandler_t *state = *current;
    StateHandler_t *next = NULL;

    // Try to handle event, walk up hierarchy if not handled
    while (state != NULL) {
        next = state->handler(ctx, event);
        if (next != NULL) break;  // Event handled
        state = state->parent;     // Try parent
    }

    // Perform transition if needed
    if (next != NULL && next != *current) {
        // Exit current state
        if ((*current)->exit) (*current)->exit(ctx);
        // Enter new state
        if (next->entry) next->entry(ctx);
        *current = next;
    }
}
*/

/*
 * ============================================================================
 * USING EVENT GROUPS FOR STATE CONDITIONS
 * ============================================================================
 */

/*
#include "event_groups.h"

#define CONDITION_SENSOR_READY   (1 << 0)
#define CONDITION_COMM_READY     (1 << 1)
#define CONDITION_USER_REQUEST   (1 << 2)
#define CONDITION_INITIALIZED    (1 << 3)

#define ALL_READY (CONDITION_SENSOR_READY | CONDITION_COMM_READY | CONDITION_INITIALIZED)

EventGroupHandle_t state_conditions;

void state_machine_with_conditions(void *pvParameters) {
    State_t state = STATE_IDLE;

    for (;;) {
        switch (state) {
            case STATE_IDLE:
                // Wait for ALL conditions to be ready
                xEventGroupWaitBits(state_conditions, ALL_READY,
                                    pdFALSE, pdTRUE, portMAX_DELAY);
                state = STATE_CONNECTED;
                break;

            case STATE_CONNECTED:
                // Wait for ANY event
                EventBits_t bits = xEventGroupWaitBits(state_conditions,
                    CONDITION_USER_REQUEST | CONDITION_ERROR,
                    pdTRUE, pdFALSE, portMAX_DELAY);

                if (bits & CONDITION_ERROR) {
                    state = STATE_ERROR;
                } else if (bits & CONDITION_USER_REQUEST) {
                    handle_user_request();
                }
                break;

            case STATE_ERROR:
                handle_error_recovery();
                state = STATE_IDLE;
                break;
        }
    }
}
*/

/*
 * ============================================================================
 * ZEPHYR STATE MACHINE USING SMGR
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>
#include <zephyr/smf.h>  // State Machine Framework

// Define states
enum demo_state { S_IDLE, S_ACTIVE, S_ERROR };

// State machine context
struct demo_ctx {
    struct smf_ctx smf_ctx;  // Required by SMF
    int32_t counter;
    bool error_flag;
};

// Forward declarations
static const struct smf_state demo_states[];

// State handlers
static void idle_entry(void *o) {
    struct demo_ctx *ctx = o;
    printk("Entering IDLE\n");
    ctx->counter = 0;
}

static void idle_run(void *o) {
    struct demo_ctx *ctx = o;
    // Check for transition conditions
    if (ctx->counter > 10) {
        smf_set_state(SMF_CTX(ctx), &demo_states[S_ACTIVE]);
    }
}

static void active_entry(void *o) {
    printk("Entering ACTIVE\n");
}

static void active_run(void *o) {
    struct demo_ctx *ctx = o;
    if (ctx->error_flag) {
        smf_set_state(SMF_CTX(ctx), &demo_states[S_ERROR]);
    }
}

static void error_entry(void *o) {
    printk("Entering ERROR\n");
}

static void error_run(void *o) {
    // Recovery logic
    struct demo_ctx *ctx = o;
    ctx->error_flag = false;
    smf_set_state(SMF_CTX(ctx), &demo_states[S_IDLE]);
}

// State definitions
static const struct smf_state demo_states[] = {
    [S_IDLE]   = SMF_CREATE_STATE(idle_entry, idle_run, NULL, NULL),
    [S_ACTIVE] = SMF_CREATE_STATE(active_entry, active_run, NULL, NULL),
    [S_ERROR]  = SMF_CREATE_STATE(error_entry, error_run, NULL, NULL),
};

void demo_thread(void *p1, void *p2, void *p3) {
    struct demo_ctx ctx = { .counter = 0, .error_flag = false };

    smf_set_initial(SMF_CTX(&ctx), &demo_states[S_IDLE]);

    while (1) {
        smf_run_state(SMF_CTX(&ctx));
        k_sleep(K_MSEC(100));
    }
}
*/

/*
 * ============================================================================
 * BEST PRACTICES
 * ============================================================================
 *
 * 1. NEVER BLOCK IN STATE HANDLERS
 *    - Use event queue, not polling
 *    - Timeouts via software timers, not delay
 *
 * 2. SINGLE POINT OF STATE CHANGE
 *    - Only state machine task changes state
 *    - External code sends events
 *
 * 3. LOG STATE TRANSITIONS
 *    - Invaluable for debugging
 *    - Include timestamp and event
 *
 * 4. HANDLE UNEXPECTED EVENTS
 *    - Either ignore or transition to error state
 *    - Log unexpected events
 *
 * 5. USE ENTRY/EXIT ACTIONS
 *    - Clean setup in entry
 *    - Clean teardown in exit
 *    - Avoids duplicated code
 *
 * 6. CONSIDER HSM FOR COMPLEX SYSTEMS
 *    - Nested states reduce duplication
 *    - Parent handles common transitions
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. DESCRIBE TWO PATTERNS
 *    "Switch-case for simple, active object for complex"
 *
 * 2. MENTION EVENT QUEUE
 *    "State machine task waits on queue for events"
 *
 * 3. EXPLAIN HSM
 *    "For complex systems, hierarchical states with inherited transitions"
 *
 * 4. DISCUSS TIMEOUT HANDLING
 *    "Use software timers that post events to state machine queue"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "How do you handle concurrent state machines?"
 * A: Active object pattern - each SM in its own task with its own queue.
 *    They communicate via events, avoiding shared state.
 *
 * Q: "What about thread safety?"
 * A: If SM is in dedicated task with event queue, it's naturally thread-safe.
 *    All state changes happen in single task context.
 *
 * Q: "How do you implement timeouts?"
 * A: Software timer that posts TIMEOUT event to SM queue when it expires.
 *    State entry starts timer, exit cancels it.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 29 ===\n\n");
    printf("Q: How do you implement State Machines in RTOS?\n\n");
    printf("PATTERNS:\n\n");
    printf("1. SWITCH-CASE (Simple):\n");
    printf("   - Single task with event queue\n");
    printf("   - Switch on state, then on event\n");
    printf("   - Good for simple protocols\n\n");
    printf("2. TABLE-DRIVEN:\n");
    printf("   - Transition table: state × event -> action, next_state\n");
    printf("   - Cleaner for many states/events\n");
    printf("   - Easy to add new transitions\n\n");
    printf("3. ACTIVE OBJECT:\n");
    printf("   - Each SM in its own task\n");
    printf("   - Private event queue\n");
    printf("   - Better encapsulation\n\n");
    printf("4. HIERARCHICAL (HSM):\n");
    printf("   - Nested states\n");
    printf("   - Parent handles common events\n");
    printf("   - Reduces duplication\n\n");
    printf("KEY PRINCIPLES:\n");
    printf("- Never block in state handlers\n");
    printf("- Use event queue, not polling\n");
    printf("- Software timers for timeouts\n");
    printf("- Log all state transitions\n");
    return 0;
}
