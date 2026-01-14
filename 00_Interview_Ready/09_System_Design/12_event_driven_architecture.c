/*
 * ============================================================================
 *        DESIGN: EVENT-DRIVEN APPLICATION ARCHITECTURE
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★★ (CRITICAL)
 *   - Button press → start/stop recording
 *   - IMU motion → wake from sleep / fall detect
 *   - WiFi events → upload data
 *   - Battery events → low power warnings
 *   - Timer events → periodic tasks
 *
 * ============================================================================
 *                    WHY EVENT-DRIVEN?
 * ============================================================================
 *
 * POLLING APPROACH (Bad):
 *   while (1) {
 *       if (button_pressed()) handle_button();
 *       if (imu_has_data()) handle_imu();
 *       if (wifi_connected()) handle_wifi();
 *       // CPU always running, burning power
 *   }
 *
 * EVENT-DRIVEN APPROACH (Good):
 *   while (1) {
 *       event = wait_for_event();  // CPU sleeps until event
 *       dispatch(event);
 *   }
 *
 * BENEFITS:
 *   - Lower power (CPU sleeps between events)
 *   - Responsive (immediate reaction to events)
 *   - Modular (handlers independent of each other)
 *   - Testable (can inject events for testing)
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// EVENT DEFINITIONS
// ============================================================================

typedef enum {
    EVT_NONE = 0,

    // Button events
    EVT_BUTTON_PRESS,
    EVT_BUTTON_RELEASE,
    EVT_BUTTON_LONG_PRESS,
    EVT_BUTTON_DOUBLE_PRESS,

    // Sensor events
    EVT_IMU_DATA_READY,
    EVT_IMU_MOTION_DETECTED,
    EVT_IMU_FALL_DETECTED,
    EVT_TEMP_THRESHOLD,

    // System events
    EVT_TIMER_TICK,
    EVT_TIMER_EXPIRED,
    EVT_WATCHDOG_WARNING,

    // Power events
    EVT_BATTERY_LOW,
    EVT_BATTERY_CRITICAL,
    EVT_CHARGER_CONNECTED,
    EVT_CHARGER_DISCONNECTED,

    // Connectivity events
    EVT_WIFI_CONNECTED,
    EVT_WIFI_DISCONNECTED,
    EVT_WIFI_SCAN_COMPLETE,
    EVT_MQTT_CONNECTED,
    EVT_MQTT_MESSAGE,

    // Storage events
    EVT_STORAGE_FULL,
    EVT_STORAGE_ERROR,

    // Application events
    EVT_RECORDING_START,
    EVT_RECORDING_STOP,
    EVT_UPLOAD_COMPLETE,
    EVT_ERROR,

    EVT_COUNT
} EventType;

typedef struct {
    EventType type;
    uint32_t timestamp;
    union {
        uint8_t button_id;
        struct { int16_t x, y, z; } imu;
        uint8_t battery_percent;
        int8_t wifi_rssi;
        uint32_t error_code;
        uint8_t raw[16];
    } data;
} Event;

const char* event_name(EventType type) {
    static const char* names[] = {
        "NONE", "BTN_PRESS", "BTN_RELEASE", "BTN_LONG", "BTN_DOUBLE",
        "IMU_DATA", "IMU_MOTION", "IMU_FALL", "TEMP_THRESH",
        "TIMER_TICK", "TIMER_EXP", "WDT_WARN",
        "BAT_LOW", "BAT_CRIT", "CHG_CONN", "CHG_DISC",
        "WIFI_CONN", "WIFI_DISC", "WIFI_SCAN", "MQTT_CONN", "MQTT_MSG",
        "STOR_FULL", "STOR_ERR",
        "REC_START", "REC_STOP", "UPL_DONE", "ERROR"
    };
    if (type < EVT_COUNT) return names[type];
    return "UNKNOWN";
}

// ============================================================================
// EVENT QUEUE
// ============================================================================

#define EVENT_QUEUE_SIZE 32

typedef struct {
    Event events[EVENT_QUEUE_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} EventQueue;

EventQueue event_queue = {0};

void event_queue_init(void) {
    event_queue.head = 0;
    event_queue.tail = 0;
}

bool event_post(EventType type, void* data, uint8_t data_len) {
    uint8_t next = (event_queue.head + 1) % EVENT_QUEUE_SIZE;
    if (next == event_queue.tail) {
        printf("[EVT] Queue full! Event %s lost\n", event_name(type));
        return false;
    }

    Event* evt = &event_queue.events[event_queue.head];
    evt->type = type;
    evt->timestamp = 0;  // Would be real time

    if (data && data_len > 0) {
        memcpy(evt->data.raw, data, data_len < 16 ? data_len : 16);
    }

    event_queue.head = next;
    return true;
}

bool event_get(Event* evt) {
    if (event_queue.head == event_queue.tail) {
        return false;
    }

    *evt = event_queue.events[event_queue.tail];
    event_queue.tail = (event_queue.tail + 1) % EVENT_QUEUE_SIZE;
    return true;
}

bool event_pending(void) {
    return event_queue.head != event_queue.tail;
}

// ============================================================================
// EVENT HANDLERS
// ============================================================================

typedef void (*EventHandler)(Event* evt);

// Handler registry
typedef struct {
    EventType type;
    EventHandler handler;
    const char* name;
} HandlerEntry;

// Forward declarations
void handle_button(Event* evt);
void handle_imu(Event* evt);
void handle_battery(Event* evt);
void handle_wifi(Event* evt);
void handle_recording(Event* evt);
void handle_error(Event* evt);

HandlerEntry handlers[] = {
    { EVT_BUTTON_PRESS,       handle_button,    "button" },
    { EVT_BUTTON_LONG_PRESS,  handle_button,    "button" },
    { EVT_IMU_DATA_READY,     handle_imu,       "imu" },
    { EVT_IMU_FALL_DETECTED,  handle_imu,       "imu" },
    { EVT_BATTERY_LOW,        handle_battery,   "battery" },
    { EVT_BATTERY_CRITICAL,   handle_battery,   "battery" },
    { EVT_CHARGER_CONNECTED,  handle_battery,   "battery" },
    { EVT_WIFI_CONNECTED,     handle_wifi,      "wifi" },
    { EVT_WIFI_DISCONNECTED,  handle_wifi,      "wifi" },
    { EVT_RECORDING_START,    handle_recording, "recording" },
    { EVT_RECORDING_STOP,     handle_recording, "recording" },
    { EVT_ERROR,              handle_error,     "error" },
};

#define NUM_HANDLERS (sizeof(handlers) / sizeof(HandlerEntry))

// ============================================================================
// HANDLER IMPLEMENTATIONS
// ============================================================================

// Application state
typedef enum {
    STATE_IDLE,
    STATE_RECORDING,
    STATE_UPLOADING,
    STATE_SLEEPING,
    STATE_ERROR
} AppState;

AppState app_state = STATE_IDLE;

void handle_button(Event* evt) {
    printf("  [BUTTON] ");

    switch (evt->type) {
        case EVT_BUTTON_PRESS:
            printf("Short press\n");
            if (app_state == STATE_IDLE) {
                event_post(EVT_RECORDING_START, NULL, 0);
            } else if (app_state == STATE_RECORDING) {
                event_post(EVT_RECORDING_STOP, NULL, 0);
            }
            break;

        case EVT_BUTTON_LONG_PRESS:
            printf("Long press - entering sleep\n");
            app_state = STATE_SLEEPING;
            break;

        default:
            break;
    }
}

void handle_imu(Event* evt) {
    printf("  [IMU] ");

    switch (evt->type) {
        case EVT_IMU_DATA_READY:
            printf("Data: x=%d y=%d z=%d\n",
                   evt->data.imu.x, evt->data.imu.y, evt->data.imu.z);
            break;

        case EVT_IMU_FALL_DETECTED:
            printf("*** FALL DETECTED ***\n");
            // Emergency action: mark video, send alert
            break;

        default:
            break;
    }
}

void handle_battery(Event* evt) {
    printf("  [BATTERY] ");

    switch (evt->type) {
        case EVT_BATTERY_LOW:
            printf("Low battery: %d%%\n", evt->data.battery_percent);
            // Reduce features, warn user
            break;

        case EVT_BATTERY_CRITICAL:
            printf("CRITICAL: %d%% - Stopping recording!\n",
                   evt->data.battery_percent);
            event_post(EVT_RECORDING_STOP, NULL, 0);
            break;

        case EVT_CHARGER_CONNECTED:
            printf("Charger connected - starting upload\n");
            app_state = STATE_UPLOADING;
            break;

        default:
            break;
    }
}

void handle_wifi(Event* evt) {
    printf("  [WIFI] ");

    switch (evt->type) {
        case EVT_WIFI_CONNECTED:
            printf("Connected (RSSI: %d dBm)\n", evt->data.wifi_rssi);
            break;

        case EVT_WIFI_DISCONNECTED:
            printf("Disconnected\n");
            break;

        default:
            break;
    }
}

void handle_recording(Event* evt) {
    printf("  [RECORDING] ");

    switch (evt->type) {
        case EVT_RECORDING_START:
            if (app_state == STATE_IDLE) {
                printf("Starting recording\n");
                app_state = STATE_RECORDING;
            } else {
                printf("Cannot start - not idle\n");
            }
            break;

        case EVT_RECORDING_STOP:
            if (app_state == STATE_RECORDING) {
                printf("Stopping recording\n");
                app_state = STATE_IDLE;
            }
            break;

        default:
            break;
    }
}

void handle_error(Event* evt) {
    printf("  [ERROR] Code: %u\n", evt->data.error_code);
    app_state = STATE_ERROR;
}

// ============================================================================
// EVENT DISPATCHER
// ============================================================================

void dispatch_event(Event* evt) {
    printf("[DISPATCH] %s\n", event_name(evt->type));

    for (size_t i = 0; i < NUM_HANDLERS; i++) {
        if (handlers[i].type == evt->type) {
            handlers[i].handler(evt);
            return;
        }
    }

    printf("  No handler registered\n");
}

// ============================================================================
// MAIN EVENT LOOP
// ============================================================================

void event_loop(void) {
    Event evt;

    while (1) {
        // Process all pending events
        while (event_get(&evt)) {
            dispatch_event(&evt);
        }

        // Would enter low-power wait here
        // wait_for_interrupt();

        // For demo: exit after processing
        break;
    }
}

// ============================================================================
// DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   EVENT-DRIVEN ARCHITECTURE DEMO\n");
    printf("============================================\n");

    event_queue_init();

    // Simulate events from various sources
    printf("\n--- Posting Events ---\n");

    // Button press to start recording
    event_post(EVT_BUTTON_PRESS, NULL, 0);

    // IMU data
    struct { int16_t x, y, z; } imu_data = {100, -50, 980};
    event_post(EVT_IMU_DATA_READY, &imu_data, sizeof(imu_data));

    // Battery low
    uint8_t battery = 15;
    event_post(EVT_BATTERY_LOW, &battery, 1);

    // WiFi connected
    int8_t rssi = -65;
    event_post(EVT_WIFI_CONNECTED, &rssi, 1);

    // Stop recording
    event_post(EVT_BUTTON_PRESS, NULL, 0);

    // Process events
    printf("\n--- Processing Events ---\n");
    Event evt;
    while (event_get(&evt)) {
        dispatch_event(&evt);
    }

    printf("\n--- Final State ---\n");
    printf("App state: %d\n", app_state);

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "Why event-driven over polling?"
 * A: "1. Power: CPU sleeps between events
 *    2. Responsiveness: Immediate reaction to ISRs
 *    3. Modularity: Handlers are independent
 *    4. Scalability: Easy to add new event types
 *    5. Testability: Can inject events for testing"
 *
 * Q: "How do you prioritize events?"
 * A: "Options:
 *    1. Priority queues: Separate queues per priority
 *    2. Event flags: Process critical events first
 *    3. Event coalescing: Merge multiple same events
 *
 *    For Ironsite: Fall detection > recording > upload"
 *
 * Q: "How do you prevent event queue overflow?"
 * A: "1. Size queue for worst-case burst
 *    2. Monitor queue depth
 *    3. Coalesce repeated events
 *    4. Drop non-critical events if full
 *    5. Log overflow for debugging"
 *
 * ============================================================================
 */
