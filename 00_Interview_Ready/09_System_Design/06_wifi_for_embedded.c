/*
 * ============================================================================
 *              WiFi FOR EMBEDDED/FIRMWARE ENGINEERS
 * ============================================================================
 *
 * What you need to know about WiFi from a firmware perspective.
 * Not RF engineering - practical knowledge for IoT development.
 *
 * ============================================================================
 *                         WiFi BASICS
 * ============================================================================
 *
 * WiFi = Wireless LAN using IEEE 802.11 standards
 *
 * COMMON STANDARDS:
 *   802.11b  - 2.4 GHz, 11 Mbps (old, rarely used)
 *   802.11g  - 2.4 GHz, 54 Mbps
 *   802.11n  - 2.4/5 GHz, 150-600 Mbps (WiFi 4)
 *   802.11ac - 5 GHz, up to 1 Gbps (WiFi 5)
 *   802.11ax - 2.4/5 GHz, up to 10 Gbps (WiFi 6)
 *
 * FOR IoT DEVICES: Usually 802.11b/g/n on 2.4 GHz
 *   - Better range than 5 GHz
 *   - More interference (crowded band)
 *   - Sufficient bandwidth for sensor data
 *
 * ============================================================================
 *                      WiFi MODES OF OPERATION
 * ============================================================================
 *
 * STATION MODE (STA) - Most common for IoT:
 *   Device connects to existing WiFi network (router/AP)
 *   Device gets IP from router's DHCP
 *   Device can access internet through router
 *
 *   YOUR DEVICE                    ROUTER                 INTERNET
 *   [ESP32] ----WiFi----> [Home Router] -----> [Cloud Server]
 *
 * ACCESS POINT MODE (AP):
 *   Device creates its own WiFi network
 *   Other devices connect to YOUR device
 *   Used for: Initial configuration, direct device-to-phone
 *
 *   [Phone] ----WiFi----> [YOUR DEVICE as AP]
 *                         (192.168.4.1)
 *
 * AP+STA MODE (Concurrent):
 *   Device runs both modes simultaneously
 *   Connected to router AND hosting its own network
 *   Used for: Configuration while staying connected
 *
 * ============================================================================
 *                     WiFi CONNECTION SEQUENCE
 * ============================================================================
 *
 * 1. SCAN: Device scans for available networks (SSIDs)
 *    - Active scan: Send probe requests
 *    - Passive scan: Listen for beacons
 *
 * 2. AUTHENTICATE: Prove you're allowed to connect
 *    - Open: No authentication
 *    - WPA2-PSK: Password-based (most common for home)
 *    - WPA2-Enterprise: Username/password/certificate (corporate)
 *
 * 3. ASSOCIATE: Join the network
 *    - Exchange capabilities
 *    - Get assigned to the AP
 *
 * 4. 4-WAY HANDSHAKE (WPA2):
 *    - Derive session keys from password
 *    - Establish encrypted communication
 *
 * 5. DHCP: Get IP address
 *    - DISCOVER -> OFFER -> REQUEST -> ACK
 *    - Now you have IP, subnet mask, gateway, DNS
 *
 * 6. CONNECTED: Ready to send/receive data
 *
 * ============================================================================
 *                      FIRMWARE RESPONSIBILITIES
 * ============================================================================
 *
 * CREDENTIAL MANAGEMENT:
 *   - Store SSID and password securely (NVS, encrypted flash)
 *   - Handle multiple saved networks
 *   - Provisioning: How does user enter credentials?
 *     * Hardcoded (bad for production)
 *     * AP mode + web server (common)
 *     * BLE provisioning (ESP32)
 *     * SmartConfig (ESP32)
 *     * WPS (push button)
 *
 * CONNECTION MANAGEMENT:
 *   - Auto-reconnect on disconnect
 *   - Handle AP not found
 *   - Handle wrong password
 *   - Fallback to AP mode if can't connect
 *
 * POWER MANAGEMENT:
 *   - Modem sleep: WiFi radio sleeps between beacons
 *   - Light sleep: CPU also sleeps
 *   - Deep sleep: Everything off, wake on timer/GPIO
 *   - Power consumption: Active ~100-200mA, sleep ~10-20uA
 *
 * ============================================================================
 *                        COMMON WiFi EVENTS
 * ============================================================================
 *
 * Events your firmware must handle:
 *
 *   WIFI_EVENT_STA_START         - WiFi station started
 *   WIFI_EVENT_STA_CONNECTED     - Connected to AP
 *   WIFI_EVENT_STA_DISCONNECTED  - Disconnected from AP
 *   IP_EVENT_STA_GOT_IP          - Got IP address (NOW ready!)
 *   WIFI_EVENT_AP_STACONNECTED   - Device connected to your AP
 *   WIFI_EVENT_AP_STADISCONNECTED- Device left your AP
 *
 * TYPICAL EVENT FLOW:
 *   START -> try connect -> CONNECTED -> DHCP -> GOT_IP -> Ready!
 *                              |
 *                              v (if fails)
 *                         DISCONNECTED -> retry
 *
 * ============================================================================
 *                     RSSI AND SIGNAL QUALITY
 * ============================================================================
 *
 * RSSI (Received Signal Strength Indicator):
 *   Measured in dBm (negative numbers)
 *
 *   -30 dBm : Excellent (very close to AP)
 *   -50 dBm : Good
 *   -60 dBm : Fair
 *   -70 dBm : Weak (may have issues)
 *   -80 dBm : Very weak (expect drops)
 *   -90 dBm : Unusable
 *
 * USE RSSI FOR:
 *   - Deciding if connection is reliable
 *   - Choosing between multiple known networks
 *   - Warning user about poor signal
 *
 * ============================================================================
 *                      SECURITY CONSIDERATIONS
 * ============================================================================
 *
 * WPA2-PSK: Good for most IoT
 *   - Pre-shared key (password)
 *   - Same password for all devices
 *
 * WPA2-Enterprise: Better for corporate
 *   - Individual credentials per device
 *   - Certificate-based option
 *   - More complex to implement
 *
 * WPA3: Newest, more secure
 *   - Not all IoT chips support it yet
 *
 * NEVER:
 *   - Hardcode credentials in source code
 *   - Store passwords in plain text
 *   - Use WEP (broken) or Open networks for sensitive data
 *
 * ============================================================================
 *                    TROUBLESHOOTING WIFI ISSUES
 * ============================================================================
 *
 * CAN'T CONNECT:
 *   1. Wrong SSID (check case sensitivity)
 *   2. Wrong password
 *   3. AP at capacity (too many devices)
 *   4. 5 GHz only network (device is 2.4 GHz only)
 *   5. Hidden SSID (must specify explicitly)
 *   6. MAC filtering on router
 *
 * KEEPS DISCONNECTING:
 *   1. Weak signal (check RSSI)
 *   2. Interference (microwaves, other 2.4 GHz devices)
 *   3. Router issues
 *   4. Power supply instability on device
 *   5. AP beacon timeout (device not responding fast enough)
 *
 * NO IP ADDRESS:
 *   1. DHCP server not responding
 *   2. DHCP pool exhausted
 *   3. Connected but not authenticated properly
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// WiFi states
typedef enum {
    WIFI_STATE_IDLE,
    WIFI_STATE_CONNECTING,
    WIFI_STATE_CONNECTED,
    WIFI_STATE_GOT_IP,
    WIFI_STATE_DISCONNECTED,
    WIFI_STATE_ERROR
} WiFiState;

// WiFi configuration
typedef struct {
    char ssid[32];
    char password[64];
    int8_t rssi;
    uint8_t channel;
    uint32_t ip_addr;
} WiFiConfig;

// State machine for WiFi connection
typedef struct {
    WiFiState state;
    WiFiConfig config;
    int retry_count;
    int max_retries;
} WiFiManager;

const char* wifi_state_str(WiFiState s) {
    switch(s) {
        case WIFI_STATE_IDLE:         return "IDLE";
        case WIFI_STATE_CONNECTING:   return "CONNECTING";
        case WIFI_STATE_CONNECTED:    return "CONNECTED";
        case WIFI_STATE_GOT_IP:       return "GOT_IP";
        case WIFI_STATE_DISCONNECTED: return "DISCONNECTED";
        case WIFI_STATE_ERROR:        return "ERROR";
        default:                      return "UNKNOWN";
    }
}

void wifi_init(WiFiManager* wm, const char* ssid, const char* password) {
    wm->state = WIFI_STATE_IDLE;
    wm->retry_count = 0;
    wm->max_retries = 5;
    snprintf(wm->config.ssid, 32, "%s", ssid);
    snprintf(wm->config.password, 64, "%s", password);
    printf("WiFi initialized for SSID: %s\n", ssid);
}

void wifi_connect(WiFiManager* wm) {
    printf("Attempting to connect to %s...\n", wm->config.ssid);
    wm->state = WIFI_STATE_CONNECTING;
}

void wifi_event_handler(WiFiManager* wm, const char* event) {
    printf("Event: %s (current state: %s)\n", event, wifi_state_str(wm->state));

    if (strcmp(event, "CONNECTED") == 0) {
        wm->state = WIFI_STATE_CONNECTED;
        wm->retry_count = 0;
        printf("  -> Associated with AP\n");
    }
    else if (strcmp(event, "GOT_IP") == 0) {
        wm->state = WIFI_STATE_GOT_IP;
        wm->config.ip_addr = 0xC0A80164;  // 192.168.1.100
        printf("  -> Got IP: 192.168.1.100\n");
        printf("  -> READY TO COMMUNICATE!\n");
    }
    else if (strcmp(event, "DISCONNECTED") == 0) {
        wm->state = WIFI_STATE_DISCONNECTED;
        wm->retry_count++;
        printf("  -> Disconnect #%d\n", wm->retry_count);
        if (wm->retry_count < wm->max_retries) {
            printf("  -> Retrying...\n");
            wifi_connect(wm);
        } else {
            printf("  -> Max retries reached, entering ERROR state\n");
            wm->state = WIFI_STATE_ERROR;
        }
    }
}

int main() {
    printf("=== WiFi Connection Demo ===\n\n");

    WiFiManager wm;
    wifi_init(&wm, "MyHomeNetwork", "password123");

    // Simulate connection sequence
    printf("\n--- Connection Sequence ---\n");
    wifi_connect(&wm);
    wifi_event_handler(&wm, "CONNECTED");
    wifi_event_handler(&wm, "GOT_IP");

    // Simulate disconnect and reconnect
    printf("\n--- Disconnect/Reconnect ---\n");
    wifi_event_handler(&wm, "DISCONNECTED");
    wifi_event_handler(&wm, "CONNECTED");
    wifi_event_handler(&wm, "GOT_IP");

    return 0;
}

/*
 * ============================================================================
 * DRY RUN: WiFi Connection State Machine
 * ============================================================================
 *
 * Initial: state = IDLE, retry_count = 0
 *
 * wifi_connect():
 *   state = CONNECTING
 *   (Hardware starts scanning, attempting auth)
 *
 * Event "CONNECTED":
 *   state = CONNECTED, retry_count = 0
 *   (Associated with AP, but no IP yet!)
 *
 * Event "GOT_IP":
 *   state = GOT_IP, ip = 192.168.1.100
 *   NOW READY - can create sockets, send data
 *
 * Event "DISCONNECTED":
 *   state = DISCONNECTED, retry_count = 1
 *   retry_count < max_retries? YES -> reconnect
 *
 * (After 5 failures):
 *   retry_count = 5 >= max_retries
 *   state = ERROR
 *   (Maybe switch to AP mode for reconfiguration)
 *
 * ============================================================================
 *                    SITUATIONAL INTERVIEW QUESTIONS
 * ============================================================================
 *
 * Q1: "Device connects to WiFi but can't reach the internet. Debug steps?"
 * A1:
 *   1. Check if GOT_IP event was received (have valid IP?)
 *   2. Verify gateway address is correct
 *   3. Try pinging gateway (local connectivity test)
 *   4. Check DNS resolution (can you resolve hostnames?)
 *   5. Verify no firewall blocking on router
 *   6. Check if router has internet connectivity
 *
 * Q2: "Device keeps disconnecting from WiFi every few hours. Causes?"
 * A2:
 *   1. Check RSSI - weak signal causes drops
 *   2. DHCP lease expiring and renewal failing
 *   3. Router kicking idle devices (keep-alive needed)
 *   4. Interference from other 2.4 GHz devices
 *   5. Power supply issues causing WiFi brownout
 *   6. AP changing channel and device not following
 *
 * Q3: "How would you implement WiFi provisioning for a headless device?"
 * A3:
 *   Options (pick based on requirements):
 *   1. AP mode + web server: Device creates AP, user connects, enters creds
 *   2. BLE provisioning: Phone app sends creds over BLE
 *   3. SmartConfig/ESP-Touch: Encoded in packet timing
 *   4. WPS push button: Simple but less secure
 *   5. QR code: Scan code with app that has WiFi creds
 *
 * Q4: "Customer reports device works at home but not at office. Why?"
 * A4:
 *   1. Office uses WPA2-Enterprise (needs different auth)
 *   2. Office is 5 GHz only, device is 2.4 GHz only
 *   3. Corporate firewall blocking MQTT/cloud ports
 *   4. Hidden SSID requiring explicit configuration
 *   5. MAC filtering on corporate network
 *   6. Captive portal requiring web login
 *
 * Q5: "How to minimize WiFi power consumption on battery device?"
 * A5:
 *   1. Use modem sleep (radio sleeps between beacons)
 *   2. Increase DTIM interval (less frequent wakeups)
 *   3. Batch data and send in bursts, then sleep
 *   4. Use deep sleep, wake only to transmit
 *   5. Consider if WiFi is right choice (BLE might be better)
 *   6. Reduce TX power if range permits
 *
 * Q6: "Device got IP but MQTT connect fails. Troubleshooting?"
 * A6:
 *   1. Verify broker hostname resolves (DNS working?)
 *   2. Check broker port is correct (1883 vs 8883)
 *   3. TLS issue: Verify certificate, check time is set
 *   4. Wrong credentials (username/password)
 *   5. Client ID conflict (another device with same ID)
 *   6. Firewall blocking outbound connection
 *
 * Q7: "How do you handle the case where saved WiFi network is unavailable?"
 * A7:
 *   1. Implement retry with exponential backoff
 *   2. After N retries, switch to AP mode for reconfiguration
 *   3. Store multiple networks, try alternatives
 *   4. Provide visual feedback (LED pattern)
 *   5. Optionally: work offline, cache data, sync later
 *
 * ============================================================================
 * INTERVIEW TIPS:
 * - WiFi connection != having IP (wait for GOT_IP event)
 * - Always implement auto-reconnect with retry limit
 * - RSSI < -70 dBm = expect problems
 * - Store credentials in NVS, not hardcoded
 * - 2.4 GHz for IoT (better range, most chips support only this)
 * ============================================================================
 */
