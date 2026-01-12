/*
 * ============================================================================
 *            BLUETOOTH FOR EMBEDDED/FIRMWARE ENGINEERS
 * ============================================================================
 *
 * Practical Bluetooth knowledge for IoT and embedded development.
 * Focus on BLE (Bluetooth Low Energy) - most common for IoT.
 *
 * ============================================================================
 *                   BLUETOOTH CLASSIC vs BLE
 * ============================================================================
 *
 * BLUETOOTH CLASSIC (BR/EDR):
 *   - Higher data rate (1-3 Mbps)
 *   - Continuous connection
 *   - Higher power consumption
 *   - Used for: Audio streaming, file transfer, serial port
 *   - Profiles: A2DP (audio), SPP (serial), HID (keyboard/mouse)
 *
 * BLUETOOTH LOW ENERGY (BLE):
 *   - Lower data rate (~1 Mbps, effectively ~100 kbps)
 *   - Intermittent connection (connect, transfer, disconnect)
 *   - Very low power (can run on coin cell for years)
 *   - Used for: Sensors, beacons, wearables, IoT
 *   - Based on GATT (Generic Attribute Profile)
 *
 * FOR IoT: Use BLE unless you need audio or high throughput
 *
 * ============================================================================
 *                        BLE ARCHITECTURE
 * ============================================================================
 *
 * ROLES:
 *
 *   PERIPHERAL (Server):
 *     - Advertises its presence
 *     - Waits for connections
 *     - Hosts data (GATT server)
 *     - Example: Your sensor device
 *
 *   CENTRAL (Client):
 *     - Scans for peripherals
 *     - Initiates connection
 *     - Reads/writes data from peripheral
 *     - Example: Phone app, gateway
 *
 *   [Phone/Gateway]  <----BLE---->  [Your IoT Device]
 *      CENTRAL                        PERIPHERAL
 *      (Client)                       (Server)
 *
 * ============================================================================
 *                      GATT - THE DATA MODEL
 * ============================================================================
 *
 * GATT = Generic Attribute Profile
 * Defines how data is organized and accessed over BLE.
 *
 * HIERARCHY:
 *
 *   PROFILE (collection of services for a use case)
 *       |
 *       +-- SERVICE (group of related data)
 *              |
 *              +-- CHARACTERISTIC (single data point)
 *                      |
 *                      +-- VALUE (the actual data)
 *                      +-- DESCRIPTOR (metadata)
 *
 * EXAMPLE - Temperature Sensor:
 *
 *   Profile: Environmental Sensing
 *       |
 *       +-- Service: Temperature (UUID: 0x1809)
 *              |
 *              +-- Characteristic: Temperature Measurement
 *                      |
 *                      +-- Value: 25.5 (°C)
 *                      +-- Descriptor: CCCD (notifications enabled)
 *
 * UUIDs:
 *   - 16-bit: Standard Bluetooth SIG defined (0x1809 = Health Thermometer)
 *   - 128-bit: Custom/vendor specific
 *
 * ============================================================================
 *                  CHARACTERISTIC PROPERTIES
 * ============================================================================
 *
 * READ:        Central can read value
 * WRITE:       Central can write value
 * WRITE_NR:    Write without response (faster)
 * NOTIFY:      Peripheral pushes updates (no ACK)
 * INDICATE:    Peripheral pushes updates (with ACK)
 *
 * COMMON PATTERNS:
 *
 *   Sensor reading:  READ + NOTIFY
 *     - Central can poll (read) or subscribe (notify)
 *
 *   Command:         WRITE or WRITE_NR
 *     - Central sends command to peripheral
 *
 *   Config value:    READ + WRITE
 *     - Central can get/set configuration
 *
 * ============================================================================
 *                    BLE CONNECTION SEQUENCE
 * ============================================================================
 *
 * 1. ADVERTISING (Peripheral):
 *    - Broadcast packets every N milliseconds
 *    - Contains: Device name, service UUIDs, flags
 *    - Advertising interval: 20ms - 10s (trade power vs discovery time)
 *
 * 2. SCANNING (Central):
 *    - Listen for advertising packets
 *    - Filter by name, UUID, RSSI
 *
 * 3. CONNECT:
 *    - Central sends connect request
 *    - Connection established
 *    - Connection interval: 7.5ms - 4s
 *
 * 4. SERVICE DISCOVERY:
 *    - Central queries peripheral for services
 *    - Gets list of services and characteristics
 *
 * 5. DATA EXCHANGE:
 *    - Read characteristics
 *    - Write characteristics
 *    - Enable notifications
 *
 * 6. DISCONNECT:
 *    - Either side can disconnect
 *    - Peripheral returns to advertising
 *
 * ============================================================================
 *                     POWER CONSIDERATIONS
 * ============================================================================
 *
 * ADVERTISING POWER:
 *   - Advertising interval affects power dramatically
 *   - 100ms interval: ~1mA average
 *   - 1000ms interval: ~0.1mA average
 *
 * CONNECTED POWER:
 *   - Connection interval matters
 *   - Shorter interval = more responsive, more power
 *   - Longer interval = slower, less power
 *
 * TYPICAL VALUES:
 *   - Advertising: 100-500ms for quick discovery
 *   - Connected: 30-100ms for responsive app
 *   - Low power: 500ms+ advertising, 500ms+ connection
 *
 * POWER MODES:
 *   - Advertising only: Lowest
 *   - Connected idle: Low
 *   - Connected transferring: Higher
 *   - Scanning (Central): Highest
 *
 * ============================================================================
 *                        MTU AND THROUGHPUT
 * ============================================================================
 *
 * MTU (Maximum Transmission Unit):
 *   - Default: 23 bytes (20 bytes payload)
 *   - Can negotiate up to 517 bytes
 *   - Larger MTU = fewer packets = faster transfer
 *
 * THROUGHPUT:
 *   - Theoretical: ~1 Mbps
 *   - Practical: 10-100 kbps typical
 *   - Depends on: MTU, connection interval, protocol overhead
 *
 * FOR LARGE DATA:
 *   - Request larger MTU after connect
 *   - Use WRITE_NR (no ACK) for speed
 *   - Consider chunking data
 *
 * ============================================================================
 *                     COMMON BLE ISSUES
 * ============================================================================
 *
 * CONNECTION FAILS:
 *   1. Device not advertising
 *   2. Wrong address type (public vs random)
 *   3. Device already connected to another central
 *   4. Out of range
 *
 * DISCONNECTS:
 *   1. Range issue (RSSI too low)
 *   2. Supervision timeout (no response in time)
 *   3. Connection interval too aggressive
 *   4. Interference
 *
 * NOTIFICATIONS NOT WORKING:
 *   1. CCCD not enabled (must write 0x0001 to CCCD descriptor)
 *   2. Wrong characteristic
 *   3. Bonding required but not done
 *
 * PAIRING/BONDING ISSUES:
 *   1. Keys not stored
 *   2. Device removed from phone but not from peripheral
 *   3. MITM required but no display/keyboard
 *
 * ============================================================================
 *                    SECURITY (PAIRING & BONDING)
 * ============================================================================
 *
 * PAIRING: Exchange keys to encrypt connection
 * BONDING: Store keys for future connections
 *
 * SECURITY LEVELS:
 *   - No security: Anyone can connect and read
 *   - Encrypted: Data encrypted but no authentication
 *   - Authenticated: MITM protection (PIN, comparison)
 *
 * PAIRING METHODS:
 *   - Just Works: No user interaction (no MITM protection)
 *   - Passkey: Enter 6-digit code
 *   - Numeric Comparison: Confirm matching numbers
 *   - OOB: Out-of-band (NFC, QR code)
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// BLE UUIDs (16-bit standard UUIDs shown)
#define UUID_TEMP_SERVICE       0x1809
#define UUID_TEMP_MEASUREMENT   0x2A1C
#define UUID_BATTERY_SERVICE    0x180F
#define UUID_BATTERY_LEVEL      0x2A19

// Characteristic properties
#define PROP_READ       0x02
#define PROP_WRITE      0x08
#define PROP_NOTIFY     0x10
#define PROP_INDICATE   0x20

// BLE states
typedef enum {
    BLE_STATE_IDLE,
    BLE_STATE_ADVERTISING,
    BLE_STATE_CONNECTED,
    BLE_STATE_DISCONNECTED
} BLEState;

// Characteristic structure
typedef struct {
    uint16_t uuid;
    uint8_t properties;
    uint8_t value[20];
    uint8_t value_len;
    bool notifications_enabled;
} BLECharacteristic;

// Service structure
typedef struct {
    uint16_t uuid;
    BLECharacteristic chars[4];
    int char_count;
} BLEService;

// BLE peripheral
typedef struct {
    BLEState state;
    char device_name[20];
    BLEService services[3];
    int service_count;
    int8_t rssi;
} BLEPeripheral;

void ble_init(BLEPeripheral* ble, const char* name) {
    ble->state = BLE_STATE_IDLE;
    strncpy(ble->device_name, name, 19);
    ble->service_count = 0;
    printf("BLE initialized: %s\n", name);
}

void ble_add_service(BLEPeripheral* ble, uint16_t uuid) {
    if (ble->service_count < 3) {
        ble->services[ble->service_count].uuid = uuid;
        ble->services[ble->service_count].char_count = 0;
        ble->service_count++;
        printf("Added service: 0x%04X\n", uuid);
    }
}

void ble_add_characteristic(BLEPeripheral* ble, int svc_idx, uint16_t uuid, uint8_t props) {
    BLEService* svc = &ble->services[svc_idx];
    if (svc->char_count < 4) {
        svc->chars[svc->char_count].uuid = uuid;
        svc->chars[svc->char_count].properties = props;
        svc->chars[svc->char_count].value_len = 0;
        svc->chars[svc->char_count].notifications_enabled = false;
        svc->char_count++;
        printf("  Added characteristic: 0x%04X (props: 0x%02X)\n", uuid, props);
    }
}

void ble_start_advertising(BLEPeripheral* ble) {
    ble->state = BLE_STATE_ADVERTISING;
    printf("Started advertising: %s\n", ble->device_name);
}

void ble_on_connect(BLEPeripheral* ble) {
    ble->state = BLE_STATE_CONNECTED;
    printf("Central connected!\n");
}

void ble_on_disconnect(BLEPeripheral* ble) {
    ble->state = BLE_STATE_DISCONNECTED;
    printf("Central disconnected, resuming advertising...\n");
    ble_start_advertising(ble);
}

void ble_update_characteristic(BLEPeripheral* ble, uint16_t char_uuid, uint8_t* data, uint8_t len) {
    for (int s = 0; s < ble->service_count; s++) {
        for (int c = 0; c < ble->services[s].char_count; c++) {
            if (ble->services[s].chars[c].uuid == char_uuid) {
                memcpy(ble->services[s].chars[c].value, data, len);
                ble->services[s].chars[c].value_len = len;
                if (ble->services[s].chars[c].notifications_enabled) {
                    printf("Notifying: 0x%04X = %d\n", char_uuid, data[0]);
                }
                return;
            }
        }
    }
}

int main() {
    printf("=== BLE Peripheral Demo ===\n\n");

    BLEPeripheral ble;
    ble_init(&ble, "TempSensor");

    // Add Temperature Service
    printf("\n--- Setting up GATT ---\n");
    ble_add_service(&ble, UUID_TEMP_SERVICE);
    ble_add_characteristic(&ble, 0, UUID_TEMP_MEASUREMENT, PROP_READ | PROP_NOTIFY);

    // Add Battery Service
    ble_add_service(&ble, UUID_BATTERY_SERVICE);
    ble_add_characteristic(&ble, 1, UUID_BATTERY_LEVEL, PROP_READ | PROP_NOTIFY);

    // Start advertising
    printf("\n--- Start Advertising ---\n");
    ble_start_advertising(&ble);

    // Simulate connection
    printf("\n--- Central Connects ---\n");
    ble_on_connect(&ble);

    // Enable notifications (central would write to CCCD)
    ble.services[0].chars[0].notifications_enabled = true;
    printf("Notifications enabled for temperature\n");

    // Update and notify
    printf("\n--- Sensor Updates ---\n");
    uint8_t temp = 25;
    ble_update_characteristic(&ble, UUID_TEMP_MEASUREMENT, &temp, 1);
    temp = 26;
    ble_update_characteristic(&ble, UUID_TEMP_MEASUREMENT, &temp, 1);

    // Disconnect
    printf("\n--- Disconnect ---\n");
    ble_on_disconnect(&ble);

    return 0;
}

/*
 * ============================================================================
 * DRY RUN: BLE Connection Flow
 * ============================================================================
 *
 * PERIPHERAL SIDE (Your IoT device):
 *
 * 1. Initialize GATT structure
 *    - Create services
 *    - Add characteristics with properties
 *
 * 2. Start advertising
 *    - Broadcast device name, service UUIDs
 *    - state = ADVERTISING
 *
 * 3. Central connects
 *    - state = CONNECTED
 *    - Stop advertising
 *
 * 4. Central discovers services
 *    - Peripheral responds with service list
 *    - Central discovers characteristics
 *
 * 5. Central enables notifications
 *    - Writes 0x0001 to CCCD descriptor
 *    - notifications_enabled = true
 *
 * 6. Peripheral sends data
 *    - Update characteristic value
 *    - If notifications enabled, send notification
 *
 * 7. Disconnect
 *    - state = DISCONNECTED
 *    - Resume advertising
 *
 * ============================================================================
 *                    SITUATIONAL INTERVIEW QUESTIONS
 * ============================================================================
 *
 * Q1: "Your BLE sensor connects but notifications don't work. Debug steps?"
 * A1:
 *   1. Check if CCCD (0x2902) was written with 0x0001
 *   2. Verify characteristic has NOTIFY property
 *   3. Check if bonding is required but not done
 *   4. Ensure peripheral is actually calling notify function
 *   5. Check phone app is subscribing correctly
 *
 * Q2: "Device drains battery too fast in advertising mode. How to fix?"
 * A2:
 *   1. Increase advertising interval (100ms -> 500ms or 1000ms)
 *   2. Reduce TX power if range permits
 *   3. Use directed advertising if central address known
 *   4. Implement sleep between advertising events
 *   5. Consider advertising only when needed (button press)
 *
 * Q3: "Central can't find your peripheral during scanning. Causes?"
 * A3:
 *   1. Device not advertising (check state)
 *   2. Wrong advertising data (service UUID not included)
 *   3. Device connected to another central (stops advertising)
 *   4. Out of range or RF interference
 *   5. Scan filter on central too restrictive
 *
 * Q4: "Data throughput is too slow for your application. Solutions?"
 * A4:
 *   1. Negotiate larger MTU after connection
 *   2. Use WRITE_NR instead of WRITE (no ACK)
 *   3. Reduce connection interval
 *   4. Send data in larger chunks
 *   5. Consider using BLE 5.0 2M PHY if supported
 *
 * Q5: "How would you implement secure OTA over BLE?"
 * A5:
 *   1. Require bonding before OTA access
 *   2. Use encrypted characteristic for firmware data
 *   3. Implement CRC/checksum per chunk
 *   4. Verify firmware signature before applying
 *   5. Keep backup partition for rollback
 *
 * Q6: "Phone says 'pairing failed'. Troubleshooting?"
 * A6:
 *   1. Check if previous bond exists (delete and retry)
 *   2. Verify IO capabilities match (display, keyboard, etc.)
 *   3. Check security requirements on characteristic
 *   4. Ensure both devices support required security level
 *   5. Check for pairing timeout issues
 *
 * ============================================================================
 * INTERVIEW TIPS:
 * - BLE peripheral = server (has data), central = client (requests data)
 * - GATT: Profile > Service > Characteristic > Value
 * - Notifications: peripheral pushes to central (must enable CCCD)
 * - Connection interval affects latency and power
 * - Advertising interval: 100ms quick discovery, 1000ms+ power saving
 * - MTU default 23 bytes, negotiate higher for throughput
 * ============================================================================
 */
