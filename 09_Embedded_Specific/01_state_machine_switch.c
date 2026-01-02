/*
 * ============================================================================
 * PROBLEM: State Machine - Switch-Case Implementation
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: VERY HIGH (Embedded)
 *
 * Simple state machine using switch-case. Good for small state machines (<5 states).
 *
 * ============================================================================
 * EXAMPLE: Traffic Light
 * ============================================================================
 *
 *                    timeout
 *         ┌──────────────────────────┐
 *         │                          │
 *         ▼                          │
 *     ┌───────┐   timeout    ┌───────┴─┐   timeout    ┌────────┐
 *     │  RED  │ ───────────► │ GREEN   │ ───────────► │ YELLOW │
 *     └───────┘              └─────────┘              └────────┘
 *
 * ============================================================================
 * TIME: O(1) per transition | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>

// States
typedef enum {
    LIGHT_RED,
    LIGHT_GREEN,
    LIGHT_YELLOW
} TrafficLightState;

// Events
typedef enum {
    EVENT_TIMEOUT
} TrafficEvent;

// State machine context
typedef struct {
    TrafficLightState state;
} TrafficLight;

void trafficLight_init(TrafficLight* light) {
    light->state = LIGHT_RED;
}

const char* stateToString(TrafficLightState state) {
    switch (state) {
        case LIGHT_RED:    return "RED";
        case LIGHT_GREEN:  return "GREEN";
        case LIGHT_YELLOW: return "YELLOW";
        default:           return "UNKNOWN";
    }
}

// Switch-case state machine
void trafficLight_handleEvent(TrafficLight* light, TrafficEvent event) {
    TrafficLightState oldState = light->state;

    switch (light->state) {
        case LIGHT_RED:
            if (event == EVENT_TIMEOUT) {
                light->state = LIGHT_GREEN;
                // Action: Turn on green LED
            }
            break;

        case LIGHT_GREEN:
            if (event == EVENT_TIMEOUT) {
                light->state = LIGHT_YELLOW;
                // Action: Turn on yellow LED
            }
            break;

        case LIGHT_YELLOW:
            if (event == EVENT_TIMEOUT) {
                light->state = LIGHT_RED;
                // Action: Turn on red LED
            }
            break;
    }

    printf("%s → %s\n", stateToString(oldState), stateToString(light->state));
}

int main() {
    printf("=== State Machine (Switch-Case) ===\n\n");

    TrafficLight light;
    trafficLight_init(&light);

    printf("Initial state: %s\n\n", stateToString(light.state));

    printf("Sending TIMEOUT events:\n");
    trafficLight_handleEvent(&light, EVENT_TIMEOUT);  // RED → GREEN
    trafficLight_handleEvent(&light, EVENT_TIMEOUT);  // GREEN → YELLOW
    trafficLight_handleEvent(&light, EVENT_TIMEOUT);  // YELLOW → RED
    trafficLight_handleEvent(&light, EVENT_TIMEOUT);  // RED → GREEN

    printf("\n=== When to Use ===\n");
    printf("- Small state machines (<5 states)\n");
    printf("- Simple transition logic\n");
    printf("- Quick prototyping\n");

    return 0;
}
