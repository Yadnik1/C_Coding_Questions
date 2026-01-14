/*
 * ============================================================================
 *        DESIGN: LOW-POWER SLEEP/WAKEUP SEQUENCE
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★★ (CRITICAL)
 *   - 8+ hour battery life requirement
 *   - Wearable device = limited battery capacity
 *   - Must wake for: button, IMU, timer, WiFi
 *
 * ============================================================================
 *                    PROBLEM STATEMENT
 * ============================================================================
 *
 * Design a power management system that:
 *   - Maximizes battery life
 *   - Wakes reliably for events
 *   - Saves and restores state correctly
 *   - Handles peripheral power states
 *   - Recovers gracefully from errors
 *
 * ============================================================================
 *                    POWER MODES (ESP32/STM32 typical)
 * ============================================================================
 *
 *   Mode           | Power    | Wake Sources      | Wake Time  | Preserved
 *   ---------------|----------|-------------------|------------|------------
 *   Active         | 100+ mA  | N/A               | N/A        | Everything
 *   Light Sleep    | 0.8 mA   | Any interrupt     | < 1 ms     | RAM, Regs
 *   Deep Sleep     | 10 µA    | RTC, GPIO, touch  | 100+ ms    | RTC RAM only
 *   Hibernation    | 2.5 µA   | RTC timer only    | 100+ ms    | Nothing
 *   Power Off      | 0 µA     | External reset    | Full boot  | Nothing
 *
 * POWER CONSUMPTION BREAKDOWN (typical wearable):
 *
 *   Component      | Active   | Sleep    | Notes
 *   ---------------|----------|----------|------------------
 *   MCU            | 50 mA    | 10 µA    | Deep sleep
 *   Camera         | 150 mA   | 0 mA     | Power gated
 *   WiFi TX        | 200 mA   | 0 mA     | Power gated
 *   IMU            | 0.5 mA   | 3 µA     | Low-power mode
 *   Flash          | 15 mA    | 1 µA     | Standby
 *   LEDs           | 20 mA    | 0 mA     | Off
 *
 * ============================================================================
 *                    STATE DIAGRAM
 * ============================================================================
 *
 *                        ┌──────────────────┐
 *                        │     ACTIVE       │
 *                        │  (Recording)     │
 *                        └────────┬─────────┘
 *                                 │ idle_timeout / low_battery
 *                                 ▼
 *                        ┌──────────────────┐
 *                        │   PREPARE_SLEEP  │
 *                        │  (Save state)    │
 *                        └────────┬─────────┘
 *                                 │ peripherals_off
 *                                 ▼
 *         ┌─────────────────────────────────────────────┐
 *         │                                             │
 *         │              DEEP SLEEP                     │
 *         │         (MCU in low power)                  │
 *         │                                             │
 *         └─────────────────────────────────────────────┘
 *                   │           │           │
 *             button_press  rtc_alarm   imu_motion
 *                   │           │           │
 *                   ▼           ▼           ▼
 *                        ┌──────────────────┐
 *                        │     WAKING       │
 *                        │  (Restore state) │
 *                        └────────┬─────────┘
 *                                 │ init_complete
 *                                 ▼
 *                        ┌──────────────────┐
 *                        │     ACTIVE       │
 *                        └──────────────────┘
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// WAKE SOURCE DEFINITIONS
// ============================================================================

typedef enum {
    WAKE_SOURCE_NONE = 0,
    WAKE_SOURCE_RTC_ALARM,        // Scheduled wakeup
    WAKE_SOURCE_BUTTON,           // User pressed button
    WAKE_SOURCE_IMU_MOTION,       // Movement detected
    WAKE_SOURCE_CHARGER,          // USB/charger connected
    WAKE_SOURCE_EXTERNAL_INT,     // External GPIO interrupt
    WAKE_SOURCE_WATCHDOG,         // Watchdog reset (error!)
} WakeSource;

// ============================================================================
// POWER STATE MACHINE
// ============================================================================

typedef enum {
    POWER_STATE_ACTIVE,
    POWER_STATE_LIGHT_SLEEP,
    POWER_STATE_PREPARE_DEEP_SLEEP,
    POWER_STATE_DEEP_SLEEP,
    POWER_STATE_WAKING,
} PowerState;

typedef struct {
    PowerState state;
    WakeSource last_wake_source;
    uint32_t sleep_duration_ms;
    uint32_t awake_time_ms;
    uint8_t battery_percent;
    bool charger_connected;

    // State to preserve across deep sleep (stored in RTC RAM)
    uint32_t recording_position;
    uint8_t device_mode;
    uint16_t error_count;
} PowerManager;

// RTC RAM survives deep sleep (on ESP32: RTC_DATA_ATTR)
// Simulated here
typedef struct {
    uint32_t magic;              // To detect valid data
    uint32_t recording_position;
    uint8_t device_mode;
    uint16_t error_count;
    uint32_t total_sleep_time_s;
    uint32_t wakeup_count;
} RtcMemory;

#define RTC_MAGIC 0xDEADBEEF

RtcMemory rtc_mem = {0};

// ============================================================================
// HARDWARE ABSTRACTION (Simulated)
// ============================================================================

void hal_enable_rtc_wakeup(uint32_t seconds) {
    printf("[HAL] RTC alarm set for %u seconds\n", seconds);
}

void hal_enable_gpio_wakeup(uint8_t pin, bool rising_edge) {
    printf("[HAL] GPIO %d wakeup enabled (%s edge)\n",
           pin, rising_edge ? "rising" : "falling");
}

void hal_enable_imu_wakeup(void) {
    printf("[HAL] IMU motion wakeup enabled\n");
}

void hal_enter_light_sleep(void) {
    printf("[HAL] >>> ENTERING LIGHT SLEEP <<<\n");
}

void hal_enter_deep_sleep(void) {
    printf("[HAL] >>> ENTERING DEEP SLEEP <<<\n");
}

WakeSource hal_get_wake_source(void) {
    // In real code: read wake cause register
    return WAKE_SOURCE_RTC_ALARM;
}

void hal_camera_power(bool on) {
    printf("[HAL] Camera power: %s\n", on ? "ON" : "OFF");
}

void hal_wifi_power(bool on) {
    printf("[HAL] WiFi power: %s\n", on ? "ON" : "OFF");
}

void hal_imu_low_power_mode(bool enable) {
    printf("[HAL] IMU low power mode: %s\n", enable ? "ENABLED" : "DISABLED");
}

// ============================================================================
// PRE-SLEEP SEQUENCE
// ============================================================================

/*
 * CRITICAL STEPS BEFORE SLEEP:
 *
 * 1. Complete pending operations (TX in progress, flash writes)
 * 2. Save state to RTC memory or flash
 * 3. Configure wake sources
 * 4. Power down peripherals (in correct order!)
 * 5. Enter sleep mode
 */

typedef struct {
    bool uart_tx_complete;
    bool flash_write_complete;
    bool wifi_disconnected;
    bool camera_stopped;
} SleepReadiness;

bool check_ready_for_sleep(SleepReadiness* status) {
    printf("[SLEEP] Checking readiness...\n");

    // Simulate checks
    status->uart_tx_complete = true;
    status->flash_write_complete = true;
    status->wifi_disconnected = true;
    status->camera_stopped = true;

    return status->uart_tx_complete &&
           status->flash_write_complete &&
           status->wifi_disconnected &&
           status->camera_stopped;
}

void save_state_to_rtc(PowerManager* pm) {
    printf("[SLEEP] Saving state to RTC memory\n");

    rtc_mem.magic = RTC_MAGIC;
    rtc_mem.recording_position = pm->recording_position;
    rtc_mem.device_mode = pm->device_mode;
    rtc_mem.error_count = pm->error_count;
    rtc_mem.wakeup_count++;
}

void configure_wake_sources(uint32_t sleep_duration_s) {
    printf("[SLEEP] Configuring wake sources\n");

    // RTC alarm for periodic wakeup
    hal_enable_rtc_wakeup(sleep_duration_s);

    // Button (GPIO 0, falling edge = pressed)
    hal_enable_gpio_wakeup(0, false);

    // IMU motion detection
    hal_enable_imu_wakeup();
}

void power_down_peripherals(void) {
    printf("[SLEEP] Powering down peripherals\n");

    // Order matters! Highest power first
    hal_camera_power(false);
    hal_wifi_power(false);
    hal_imu_low_power_mode(true);

    // Note: Keep enabled:
    // - RTC (for timer wakeup)
    // - GPIO (for button wakeup)
    // - IMU in motion-detect mode
}

void enter_deep_sleep(PowerManager* pm, uint32_t sleep_duration_s) {
    printf("\n===== DEEP SLEEP SEQUENCE =====\n");

    // Step 1: Check readiness
    SleepReadiness status = {0};
    if (!check_ready_for_sleep(&status)) {
        printf("[SLEEP] Not ready! Aborting sleep.\n");
        return;
    }

    // Step 2: Save state
    save_state_to_rtc(pm);

    // Step 3: Configure wake sources
    configure_wake_sources(sleep_duration_s);

    // Step 4: Power down peripherals
    power_down_peripherals();

    // Step 5: Enter sleep
    pm->state = POWER_STATE_DEEP_SLEEP;
    printf("[SLEEP] Good night!\n");
    hal_enter_deep_sleep();

    // In real code, execution stops here until wake
}

// ============================================================================
// POST-WAKE SEQUENCE
// ============================================================================

/*
 * CRITICAL STEPS AFTER WAKE:
 *
 * 1. Identify wake source
 * 2. Restore state from RTC memory
 * 3. Validate restored state (magic number)
 * 4. Power up peripherals (reverse order of power down)
 * 5. Re-initialize hardware
 * 6. Handle wake source event
 */

bool restore_state_from_rtc(PowerManager* pm) {
    printf("[WAKE] Restoring state from RTC memory\n");

    if (rtc_mem.magic != RTC_MAGIC) {
        printf("[WAKE] Invalid RTC magic! Cold boot.\n");
        return false;
    }

    pm->recording_position = rtc_mem.recording_position;
    pm->device_mode = rtc_mem.device_mode;
    pm->error_count = rtc_mem.error_count;

    printf("[WAKE] Restored: pos=%u, mode=%d, errors=%d, wakeups=%u\n",
           pm->recording_position, pm->device_mode,
           pm->error_count, rtc_mem.wakeup_count);

    return true;
}

void power_up_peripherals(void) {
    printf("[WAKE] Powering up peripherals\n");

    // Reverse order of power down
    hal_imu_low_power_mode(false);
    // WiFi: only if needed
    // Camera: only if recording
}

void handle_wakeup(PowerManager* pm) {
    printf("\n===== WAKEUP SEQUENCE =====\n");

    // Step 1: Get wake source
    pm->last_wake_source = hal_get_wake_source();
    printf("[WAKE] Source: %d\n", pm->last_wake_source);

    // Step 2: Restore state
    bool valid_state = restore_state_from_rtc(pm);

    // Step 3: Power up peripherals
    power_up_peripherals();

    // Step 4: Handle wake source
    pm->state = POWER_STATE_ACTIVE;

    switch (pm->last_wake_source) {
        case WAKE_SOURCE_RTC_ALARM:
            printf("[WAKE] Periodic wakeup - check sensors, upload data\n");
            // Quick sensor check, then sleep again
            break;

        case WAKE_SOURCE_BUTTON:
            printf("[WAKE] Button pressed - wake fully\n");
            hal_camera_power(true);  // User wants to record
            break;

        case WAKE_SOURCE_IMU_MOTION:
            printf("[WAKE] Motion detected - possible fall?\n");
            // Check IMU data, potentially wake fully
            break;

        case WAKE_SOURCE_CHARGER:
            printf("[WAKE] Charger connected - enter charging mode\n");
            pm->charger_connected = true;
            hal_wifi_power(true);  // Opportunity to upload
            break;

        case WAKE_SOURCE_WATCHDOG:
            printf("[WAKE] WATCHDOG RESET - ERROR RECOVERY!\n");
            pm->error_count++;
            // Log error, maybe don't sleep for a while
            break;

        default:
            if (!valid_state) {
                printf("[WAKE] Cold boot - full initialization\n");
            }
            break;
    }
}

// ============================================================================
// LIGHT SLEEP (For shorter sleep periods)
// ============================================================================

/*
 * LIGHT SLEEP vs DEEP SLEEP:
 *
 * Light Sleep:
 *   - Faster wake (~1ms)
 *   - All RAM preserved
 *   - Higher power (~1mA)
 *   - Good for: Short idle periods, waiting for events
 *
 * Deep Sleep:
 *   - Slower wake (~100ms)
 *   - Only RTC RAM preserved
 *   - Very low power (~10µA)
 *   - Good for: Long sleep, battery optimization
 */

void enter_light_sleep(PowerManager* pm, uint32_t max_sleep_ms) {
    printf("\n===== LIGHT SLEEP =====\n");

    pm->state = POWER_STATE_LIGHT_SLEEP;

    // Configure timer wakeup
    printf("[LIGHT] Sleeping for up to %u ms\n", max_sleep_ms);

    // In light sleep, peripherals stay configured
    // Just reduce clock, disable CPU

    hal_enter_light_sleep();

    // Execution continues here after wake
    pm->state = POWER_STATE_ACTIVE;
    printf("[LIGHT] Woke up!\n");
}

// ============================================================================
// POWER OPTIMIZATION STRATEGIES
// ============================================================================

/*
 * STRATEGY 1: DUTY CYCLING
 *
 *   Active ────────┐    ┌────────┐    ┌────────
 *                  │    │        │    │
 *   Sleep  ────────┴────┘        └────┘
 *
 *   Periodic: Sleep 10 minutes, wake 30 seconds for sensor read
 *
 *
 * STRATEGY 2: EVENT-DRIVEN SLEEP
 *
 *   Sleep until event (button, motion, timer)
 *   Handle event, then sleep immediately
 *   Good for: Infrequent user interaction
 *
 *
 * STRATEGY 3: TIERED SLEEP
 *
 *   Idle 0-5s:   Stay active (quick response needed)
 *   Idle 5-30s:  Light sleep (user might interact soon)
 *   Idle 30s+:   Deep sleep (save battery)
 *
 *
 * STRATEGY 4: WORKLOAD BATCHING
 *
 *   Collect sensor data in buffer
 *   Wake WiFi once, upload all data
 *   Sleep WiFi again
 *   More efficient than frequent short wake-ups
 */

typedef struct {
    uint32_t idle_threshold_light_ms;   // After this, light sleep
    uint32_t idle_threshold_deep_ms;    // After this, deep sleep
    uint32_t periodic_wakeup_s;         // Wake interval in deep sleep
    uint8_t min_battery_for_wifi;       // Don't use WiFi below this
} PowerPolicy;

PowerPolicy default_policy = {
    .idle_threshold_light_ms = 5000,
    .idle_threshold_deep_ms = 30000,
    .periodic_wakeup_s = 600,           // 10 minutes
    .min_battery_for_wifi = 20,
};

void apply_power_policy(PowerManager* pm, PowerPolicy* policy) {
    printf("\n[POLICY] Checking power policy\n");

    // Check battery
    if (pm->battery_percent < policy->min_battery_for_wifi) {
        printf("[POLICY] Battery low (%d%%) - disabling WiFi\n",
               pm->battery_percent);
        hal_wifi_power(false);
    }

    // Check idle time
    if (pm->awake_time_ms > policy->idle_threshold_deep_ms &&
        !pm->charger_connected) {
        printf("[POLICY] Long idle - entering deep sleep\n");
        enter_deep_sleep(pm, policy->periodic_wakeup_s);
    } else if (pm->awake_time_ms > policy->idle_threshold_light_ms) {
        printf("[POLICY] Short idle - entering light sleep\n");
        enter_light_sleep(pm, policy->idle_threshold_deep_ms);
    }
}

// ============================================================================
// DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   LOW-POWER SLEEP/WAKEUP DEMO\n");
    printf("============================================\n");

    PowerManager pm = {
        .state = POWER_STATE_ACTIVE,
        .battery_percent = 85,
        .charger_connected = false,
        .recording_position = 12345,
        .device_mode = 1,
        .error_count = 0,
    };

    // Demonstrate sleep sequence
    enter_deep_sleep(&pm, 600);

    // Demonstrate wake sequence
    handle_wakeup(&pm);

    // Demonstrate power policy
    pm.awake_time_ms = 35000;  // 35 seconds idle
    apply_power_policy(&pm, &default_policy);

    printf("\n--- Statistics ---\n");
    printf("Total wakeups: %u\n", rtc_mem.wakeup_count);
    printf("Error count: %d\n", pm.error_count);

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "How would you maximize battery life for a wearable camera?"
 * A: "Multi-pronged approach:
 *    1. Aggressive sleep when not recording (deep sleep ~10µA)
 *    2. Power-gate camera when not in use (150mA saved)
 *    3. Batch WiFi uploads (wake, upload all, sleep)
 *    4. Use motion detection to wake from sleep
 *    5. Lower camera resolution/framerate when battery low
 *    6. Disable WiFi below 20% battery
 *
 *    Target: 8+ hours recording = 8h × 300mA = 2400mAh minimum"
 *
 * Q: "What do you save before deep sleep?"
 * A: "Only essential state that can't be reconstructed:
 *    - Recording position (can't lose user's footage!)
 *    - Device mode/configuration
 *    - Error counts for diagnostics
 *    - Timestamp of last sync
 *
 *    Stored in RTC RAM (survives deep sleep) with magic number
 *    to detect corruption/cold boot."
 *
 * Q: "How do you handle wake from different sources?"
 * A: "Read wake cause register immediately on boot.
 *    Different sources need different responses:
 *    - RTC timer: Quick sensor check, sleep again
 *    - Button: Wake fully, user wants interaction
 *    - Motion: Check for fall, maybe alert
 *    - Charger: Wake WiFi for data upload opportunity"
 *
 * Q: "What can go wrong with sleep/wake?"
 * A: "Common issues:
 *    1. Peripheral not ready for sleep (TX in progress)
 *    2. Interrupt fires during sleep entry (race condition)
 *    3. RTC memory corruption (need validation)
 *    4. Wake source not configured (device doesn't wake!)
 *    5. Watchdog fires during sleep (wasn't disabled)
 *    6. Current leak from floating GPIO pins"
 *
 * ============================================================================
 *                    EDGE CASES
 * ============================================================================
 *
 * 1. SLEEP DURING ACTIVE OPERATION:
 *    What if sleep triggered while recording?
 *    - Must complete current frame/chunk
 *    - Save file metadata
 *    - Resume seamlessly on wake
 *
 * 2. RAPID WAKE-SLEEP CYCLES:
 *    User repeatedly presses button quickly
 *    - Add debounce/holdoff before sleeping again
 *    - Minimum awake time (e.g., 5 seconds)
 *
 * 3. WAKE SOURCE DURING SLEEP ENTRY:
 *    Button pressed while entering sleep
 *    - Check wake flags immediately after sleep returns
 *    - Handle the pending event
 *
 * 4. LOW BATTERY IN SLEEP:
 *    Battery depletes to critical level while sleeping
 *    - Brown-out detector should trigger safe shutdown
 *    - Critical data should already be saved
 *
 * 5. MULTIPLE SIMULTANEOUS WAKE SOURCES:
 *    RTC timer and button at same time
 *    - Check all wake flags, not just first
 *    - Handle highest priority first
 *
 * ============================================================================
 */
