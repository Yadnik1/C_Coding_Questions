/*
 * ============================================================================
 *        DESIGN: BOOT-UP INITIALIZATION FLOW
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★★ (CRITICAL)
 *   - Reliable startup after battery swap
 *   - Recovery from field failures
 *   - Fast boot to recording
 *   - Factory provisioning support
 *
 * ============================================================================
 *                    BOOT SEQUENCE PHASES
 * ============================================================================
 *
 *   RESET
 *     │
 *     ▼
 *   ┌──────────────────┐
 *   │ 1. EARLY INIT    │  CPU, clocks, critical GPIO
 *   └────────┬─────────┘
 *            │
 *   ┌────────▼─────────┐
 *   │ 2. HARDWARE INIT │  Peripherals, memory test
 *   └────────┬─────────┘
 *            │
 *   ┌────────▼─────────┐
 *   │ 3. DRIVER INIT   │  UART, SPI, I2C, etc.
 *   └────────┬─────────┘
 *            │
 *   ┌────────▼─────────┐
 *   │ 4. DEVICE INIT   │  Sensors, flash, WiFi
 *   └────────┬─────────┘
 *            │
 *   ┌────────▼─────────┐
 *   │ 5. APP INIT      │  State restore, config load
 *   └────────┬─────────┘
 *            │
 *     ▼
 *   MAIN LOOP
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// BOOT STATUS AND ERROR HANDLING
// ============================================================================

typedef enum {
    BOOT_OK = 0,
    BOOT_ERR_CLOCK,
    BOOT_ERR_MEMORY,
    BOOT_ERR_FLASH,
    BOOT_ERR_SENSOR,
    BOOT_ERR_WIFI,
    BOOT_ERR_CONFIG,
    BOOT_ERR_CRITICAL,
} BootError;

typedef struct {
    uint32_t phase;
    uint32_t step;
    BootError last_error;
    uint32_t error_count;
    uint32_t boot_time_ms;
} BootStatus;

BootStatus boot_status = {0};

const char* boot_error_str(BootError err) {
    switch (err) {
        case BOOT_OK:          return "OK";
        case BOOT_ERR_CLOCK:   return "CLOCK_FAIL";
        case BOOT_ERR_MEMORY:  return "MEMORY_FAIL";
        case BOOT_ERR_FLASH:   return "FLASH_FAIL";
        case BOOT_ERR_SENSOR:  return "SENSOR_FAIL";
        case BOOT_ERR_WIFI:    return "WIFI_FAIL";
        case BOOT_ERR_CONFIG:  return "CONFIG_FAIL";
        default:               return "UNKNOWN";
    }
}

// ============================================================================
// INITIALIZATION TABLE
// ============================================================================

typedef BootError (*InitFunction)(void);

typedef struct {
    const char* name;
    InitFunction init;
    bool critical;      // If true, boot fails if this fails
    uint32_t timeout_ms;
} InitEntry;

// Forward declarations
BootError init_clocks(void);
BootError init_gpio(void);
BootError init_uart(void);
BootError init_spi(void);
BootError init_i2c(void);
BootError init_flash(void);
BootError init_imu(void);
BootError init_camera(void);
BootError init_wifi(void);
BootError init_app(void);

// Initialization table - ORDER MATTERS
const InitEntry init_table[] = {
    // Phase 1: Critical hardware
    { "clocks",   init_clocks,   true,   100  },
    { "gpio",     init_gpio,     true,   50   },

    // Phase 2: Communication
    { "uart",     init_uart,     true,   100  },  // Debug output
    { "spi",      init_spi,      true,   100  },
    { "i2c",      init_i2c,      true,   100  },

    // Phase 3: Devices
    { "flash",    init_flash,    true,   500  },  // Critical for storage
    { "imu",      init_imu,      false,  200  },  // Can work without
    { "camera",   init_camera,   false,  1000 },  // Main feature
    { "wifi",     init_wifi,     false,  2000 },  // Can work offline

    // Phase 4: Application
    { "app",      init_app,      true,   500  },
};

#define INIT_TABLE_SIZE (sizeof(init_table) / sizeof(InitEntry))

// ============================================================================
// SIMULATED INITIALIZATION FUNCTIONS
// ============================================================================

BootError init_clocks(void) {
    printf("  [INIT] Configuring system clocks...\n");
    // Configure PLL, peripheral clocks
    return BOOT_OK;
}

BootError init_gpio(void) {
    printf("  [INIT] Configuring GPIO...\n");
    // LED pins, button pins, chip selects
    return BOOT_OK;
}

BootError init_uart(void) {
    printf("  [INIT] Initializing UART (115200 baud)...\n");
    return BOOT_OK;
}

BootError init_spi(void) {
    printf("  [INIT] Initializing SPI bus...\n");
    return BOOT_OK;
}

BootError init_i2c(void) {
    printf("  [INIT] Initializing I2C bus...\n");
    return BOOT_OK;
}

BootError init_flash(void) {
    printf("  [INIT] Initializing flash storage...\n");
    // Read JEDEC ID, verify flash present
    return BOOT_OK;
}

static bool imu_failed = false;  // For demo
BootError init_imu(void) {
    printf("  [INIT] Initializing IMU sensor...\n");
    if (imu_failed) {
        printf("  [INIT] IMU WHO_AM_I failed!\n");
        return BOOT_ERR_SENSOR;
    }
    return BOOT_OK;
}

BootError init_camera(void) {
    printf("  [INIT] Initializing camera module...\n");
    // Power on, configure resolution, verify communication
    return BOOT_OK;
}

BootError init_wifi(void) {
    printf("  [INIT] Initializing WiFi module...\n");
    // Don't connect yet, just initialize hardware
    return BOOT_OK;
}

BootError init_app(void) {
    printf("  [INIT] Loading application state...\n");
    // Load config, restore state from RTC RAM
    return BOOT_OK;
}

// ============================================================================
// BOOT SEQUENCE EXECUTION
// ============================================================================

typedef enum {
    BOOT_MODE_NORMAL,
    BOOT_MODE_SAFE,
    BOOT_MODE_FACTORY,
    BOOT_MODE_OTA
} BootMode;

BootMode determine_boot_mode(void) {
    // Check boot pins, buttons, flags
    printf("[BOOT] Checking boot mode...\n");

    // Example: Hold button during boot = factory mode
    // if (gpio_read(BUTTON_PIN) == 0) return BOOT_MODE_FACTORY;

    // Example: OTA flag in flash
    // if (ota_pending_flag) return BOOT_MODE_OTA;

    return BOOT_MODE_NORMAL;
}

bool run_initialization(BootMode mode) {
    printf("\n========== BOOT SEQUENCE ==========\n");
    printf("Boot mode: %d\n\n", mode);

    uint32_t start_time = 0;  // Would be real time
    bool all_ok = true;

    for (size_t i = 0; i < INIT_TABLE_SIZE; i++) {
        const InitEntry* entry = &init_table[i];

        printf("[%zu/%zu] %s... ", i + 1, INIT_TABLE_SIZE, entry->name);
        boot_status.step = i;

        BootError err = entry->init();

        if (err != BOOT_OK) {
            printf("FAILED (%s)\n", boot_error_str(err));
            boot_status.last_error = err;
            boot_status.error_count++;

            if (entry->critical) {
                printf("\n*** CRITICAL FAILURE - CANNOT CONTINUE ***\n");
                return false;
            } else {
                printf("    (non-critical, continuing)\n");
            }
        } else {
            printf("OK\n");
        }
    }

    boot_status.boot_time_ms = 100;  // Would be real time difference
    printf("\n========== BOOT COMPLETE ==========\n");
    printf("Total time: %u ms\n", boot_status.boot_time_ms);
    printf("Errors: %u\n\n", boot_status.error_count);

    return all_ok;
}

// ============================================================================
// SELF-TEST DURING BOOT
// ============================================================================

typedef struct {
    const char* name;
    bool (*test)(void);
} SelfTest;

bool test_ram(void) {
    printf("  RAM test: ");
    // Write pattern, read back, verify
    // Test critical regions, not full RAM (takes too long)
    printf("PASS\n");
    return true;
}

bool test_flash(void) {
    printf("  Flash test: ");
    // Read known pattern from test sector
    printf("PASS\n");
    return true;
}

bool test_watchdog(void) {
    printf("  Watchdog test: ");
    // Verify watchdog is running
    printf("PASS\n");
    return true;
}

const SelfTest self_tests[] = {
    { "RAM",      test_ram },
    { "Flash",    test_flash },
    { "Watchdog", test_watchdog },
};

bool run_self_tests(void) {
    printf("\n--- Self-Test ---\n");

    for (size_t i = 0; i < sizeof(self_tests)/sizeof(SelfTest); i++) {
        if (!self_tests[i].test()) {
            printf("Self-test FAILED: %s\n", self_tests[i].name);
            return false;
        }
    }

    printf("All self-tests PASSED\n");
    return true;
}

// ============================================================================
// FACTORY PROVISIONING BOOT
// ============================================================================

void factory_boot(void) {
    printf("\n=== FACTORY MODE ===\n");
    printf("1. Waiting for provisioning data via USB...\n");
    printf("2. Will receive: Device ID, certificates, calibration\n");
    printf("3. Will write to secure storage\n");
    printf("4. Reboot into normal mode after provisioning\n");
}

// ============================================================================
// SAFE MODE BOOT
// ============================================================================

void safe_mode_boot(void) {
    printf("\n=== SAFE MODE ===\n");
    printf("Running with minimal features:\n");
    printf("  - WiFi enabled for remote diagnosis\n");
    printf("  - Camera disabled\n");
    printf("  - Telemetry enabled\n");
    printf("  - Waiting for OTA update or reset\n");
}

// ============================================================================
// DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   BOOT INITIALIZATION FLOW DEMO\n");
    printf("============================================\n");

    // Run self-tests first
    if (!run_self_tests()) {
        printf("Self-test failed, entering safe mode\n");
        safe_mode_boot();
        return 1;
    }

    // Determine boot mode
    BootMode mode = determine_boot_mode();

    switch (mode) {
        case BOOT_MODE_FACTORY:
            factory_boot();
            return 0;

        case BOOT_MODE_SAFE:
            safe_mode_boot();
            return 0;

        default:
            break;
    }

    // Normal initialization
    bool success = run_initialization(mode);

    if (!success) {
        printf("Boot failed, entering safe mode\n");
        safe_mode_boot();
        return 1;
    }

    // Demo with IMU failure
    printf("\n--- Demo: Boot with IMU failure ---\n");
    imu_failed = true;
    boot_status.error_count = 0;
    run_initialization(BOOT_MODE_NORMAL);
    imu_failed = false;

    printf("\nNote: IMU is non-critical, boot continued\n");

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "Walk me through your boot sequence design."
 * A: "Table-driven initialization with phases:
 *    1. Critical hardware (clocks, GPIO)
 *    2. Communication (debug UART first!)
 *    3. Storage and sensors
 *    4. Application state
 *
 *    Each entry marked critical or non-critical.
 *    Non-critical failures logged but don't stop boot.
 *    Table makes it easy to add/reorder init steps."
 *
 * Q: "How do you handle initialization failures?"
 * A: "Depends on criticality:
 *    - Critical (flash, UART): Abort, enter safe mode
 *    - Non-critical (WiFi, camera): Log, continue
 *
 *    Always init debug UART early for visibility.
 *    Store error info for later diagnosis.
 *    Support multiple boot modes (normal, safe, factory)."
 *
 * Q: "How fast should boot be?"
 * A: "Target < 3 seconds to main functionality.
 *    Defer non-essential init (WiFi connect, OTA check).
 *    Profile each phase to find bottlenecks.
 *    For wearable: user expects quick power-on."
 *
 * ============================================================================
 */
