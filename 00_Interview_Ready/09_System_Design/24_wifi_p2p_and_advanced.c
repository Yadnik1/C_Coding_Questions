/*
 * ============================================================================
 *     Wi-Fi P2P, ENTERPRISE & CROSS-PLATFORM - HP IQ INTERVIEW PREP (Part 2)
 * ============================================================================
 *
 * TARGET ROLE: Wireless Software Engineer at HP IQ
 *
 * THIS FILE COVERS (from job description):
 *   - Wi-Fi Direct and Wi-Fi Aware (peer-to-peer technologies)
 *   - Bluetooth coexistence with Wi-Fi
 *   - Cross-platform development (Android, Windows, Linux, embedded)
 *   - Enterprise Wi-Fi architectures
 *   - Wireless analysis tools (Wireshark, RF analyzers)
 *   - SDK/Framework development patterns
 *
 * ============================================================================
 *                    WI-FI DIRECT (P2P) - CRITICAL FOR HP IQ
 * ============================================================================
 *
 * WHAT IS WI-FI DIRECT?
 *   - Allows devices to connect directly without an AP/router
 *   - One device becomes "Group Owner" (soft AP), others are clients
 *   - Used for: Printing, screen mirroring, file transfer, gaming
 *   - HP USE CASE: Laptop to printer, device-to-device collaboration
 *
 * WI-FI DIRECT STACK:
 *
 *   +-----------------------------------------------------------+
 *   |  APPLICATION (Print service, Miracast, File sharing)      |
 *   +-----------------------------------------------------------+
 *   |  Wi-Fi P2P FRAMEWORK (wpa_supplicant P2P / Android P2P)   |
 *   |    - Device discovery                                     |
 *   |    - Group formation                                      |
 *   |    - Service discovery (optional)                         |
 *   +-----------------------------------------------------------+
 *   |  Wi-Fi DRIVER (P2P interface support)                     |
 *   +-----------------------------------------------------------+
 *   |  Wi-Fi HARDWARE (must support P2P)                        |
 *   +-----------------------------------------------------------+
 *
 * WI-FI DIRECT CONNECTION FLOW:
 *
 *   Device A                                          Device B
 *   =========                                         =========
 *       |                                                 |
 *       |  1. P2P DEVICE DISCOVERY                        |
 *       |     (Probe Req/Resp with P2P IE)                |
 *       |<----------------------------------------------->|
 *       |                                                 |
 *       |  2. GROUP OWNER NEGOTIATION                     |
 *       |     GO Negotiation Request                      |
 *       |------------------------------------------------>|
 *       |     GO Negotiation Response                     |
 *       |<------------------------------------------------|
 *       |     GO Negotiation Confirmation                 |
 *       |------------------------------------------------>|
 *       |                                                 |
 *       |  (Device with higher "GO Intent" becomes GO)    |
 *       |                                                 |
 *       |  3. GROUP FORMATION                             |
 *       |     Device B becomes GO (creates soft AP)       |
 *       |     Device A connects as P2P Client             |
 *       |                                                 |
 *       |  4. WPS PROVISIONING                            |
 *       |     (PBC or PIN method)                         |
 *       |<----------------------------------------------->|
 *       |                                                 |
 *       |  5. 4-WAY HANDSHAKE                             |
 *       |     (Same as regular WPA2)                      |
 *       |<----------------------------------------------->|
 *       |                                                 |
 *       |  6. DHCP (GO is DHCP server)                    |
 *       |<------------------------------------------------|
 *       |                                                 |
 *       |  === P2P GROUP ESTABLISHED ===                  |
 *       |  Now can exchange application data              |
 *
 *
 * GO INTENT (0-15):
 *   - Determines which device becomes Group Owner
 *   - Higher intent = more likely to be GO
 *   - 15 = "I must be GO" (tie-breaker bit used if both 15)
 *   - Use cases:
 *     * Printer should be GO (always available, stable)
 *     * Phone might prefer client (save battery)
 *
 * PERSISTENT GROUPS:
 *   - Save group credentials for faster reconnection
 *   - Skip discovery and negotiation on reconnect
 *   - Important for printer scenarios (instant reconnect)
 *
 * CONCURRENT OPERATION:
 *   - Device can be in P2P group AND connected to infrastructure AP
 *   - Requires dual-interface support in driver
 *   - Channel considerations (same channel preferred)
 *
 * ============================================================================
 *                    WI-FI AWARE (NAN - Neighbor Awareness Networking)
 * ============================================================================
 *
 * WHAT IS WI-FI AWARE?
 *   - Discovery protocol without being connected to any network
 *   - Devices form clusters, exchange small discovery messages
 *   - Very low power (synchronized sleep schedules)
 *   - Used for: Proximity-based services, social apps, IoT
 *
 * WI-FI AWARE VS WI-FI DIRECT:
 *   | Aspect          | Wi-Fi Direct       | Wi-Fi Aware           |
 *   |-----------------|--------------------|-----------------------|
 *   | Primary use     | Data transfer      | Discovery             |
 *   | Connection      | Full P2P group     | Discovery only*       |
 *   | Power           | Higher             | Very low              |
 *   | Range           | Longer             | Optimized for nearby  |
 *   | Data rate       | High (Mbps)        | Low (discovery msgs)  |
 *   * Can escalate to Wi-Fi Direct or infrastructure for data
 *
 * WI-FI AWARE ARCHITECTURE:
 *
 *   +-----------------------------------------------------------+
 *   |  DISCOVERY                                                |
 *   |  - Publish: "I offer printing service"                    |
 *   |  - Subscribe: "Looking for printers nearby"               |
 *   +-----------------------------------------------------------+
 *   |  CLUSTER FORMATION                                        |
 *   |  - Master device syncs discovery windows                  |
 *   |  - All devices wake at same time for discovery            |
 *   +-----------------------------------------------------------+
 *   |  DATA PATH (NDP - NAN Data Path)                          |
 *   |  - After discovery, establish secure data connection      |
 *   |  - Similar to Wi-Fi Direct but more efficient             |
 *   +-----------------------------------------------------------+
 *
 * PUBLISH/SUBSCRIBE MODEL:
 *
 *   Publisher (Service Provider):
 *     - Announces service availability
 *     - Solicited: responds only to matching subscriptions
 *     - Unsolicited: broadcasts periodically
 *
 *   Subscriber (Service Seeker):
 *     - Looks for specific services
 *     - Active: sends probe requests
 *     - Passive: listens for announcements
 *
 * HP IQ USE CASES:
 *   - Discover nearby HP printers/devices without network
 *   - Proximity-based collaboration features
 *   - Low-power device discovery for IoT ecosystem
 *
 * ============================================================================
 *                    BLUETOOTH + WI-FI COEXISTENCE
 * ============================================================================
 *
 * THE PROBLEM:
 *   - Bluetooth and Wi-Fi 2.4 GHz share spectrum
 *   - Without coordination, they interfere with each other
 *   - Results in: dropped packets, reduced throughput, audio glitches
 *
 * COEXISTENCE MECHANISMS:
 *
 * 1. TIME-DIVISION (Most common):
 *    - Arbiter decides who transmits when
 *    - Bluetooth gets slots for audio, Wi-Fi gets rest
 *    - Priority schemes based on traffic type
 *
 *    Time: |--BT--|--WiFi--|--BT--|--WiFi--|--WiFi--|--BT--|
 *
 * 2. FREQUENCY AVOIDANCE:
 *    - Bluetooth uses Adaptive Frequency Hopping (AFH)
 *    - Mark Wi-Fi channels as "bad", BT avoids them
 *    - Wi-Fi tells BT which channels it's using
 *
 * 3. COEX SIGNALING (Hardware):
 *    - Dedicated GPIO lines between BT and Wi-Fi chips
 *    - BT_ACTIVE, WLAN_ACTIVE, BT_PRIORITY signals
 *    - Real-time arbitration at hardware level
 *
 * SOFTWARE ROLE IN COEXISTENCE:
 *   - Configure coex parameters (priority weights)
 *   - Monitor performance metrics
 *   - Adjust based on use case (A2DP audio vs file transfer)
 *   - Report coex statistics for debugging
 *
 * COEX PRIORITY EXAMPLES:
 *   | Traffic Type         | Priority |
 *   |----------------------|----------|
 *   | BT HID (keyboard)    | High     |
 *   | BT SCO (voice call)  | High     |
 *   | BT A2DP (audio)      | Medium   |
 *   | Wi-Fi VoIP           | High     |
 *   | Wi-Fi Video          | Medium   |
 *   | Wi-Fi Bulk data      | Low      |
 *
 * DEBUGGING COEX ISSUES:
 *   1. Identify symptom (audio glitch? Wi-Fi drops?)
 *   2. Check if both radios active simultaneously
 *   3. Review coex statistics (grants, denies)
 *   4. Adjust priority or time allocation
 *   5. Consider 5 GHz for Wi-Fi (no interference)
 *
 * ============================================================================
 *                    CROSS-PLATFORM WIRELESS DEVELOPMENT
 * ============================================================================
 *
 * HP IQ WORKS ACROSS: Android, Windows, Linux, Embedded
 *
 * ANDROID WIRELESS STACK:
 *
 *   +-----------------------------------------------------------+
 *   |  JAVA APPS (using WifiManager, WifiP2pManager)            |
 *   +-----------------------------------------------------------+
 *   |  ANDROID FRAMEWORK                                        |
 *   |    WifiService, WifiP2pService, WifiAwareManager          |
 *   +-----------------------------------------------------------+
 *   |  NATIVE LAYER (C++)                                       |
 *   |    wificond, wpa_supplicant, hostapd                      |
 *   +-----------------------------------------------------------+
 *   |  HAL (Hardware Abstraction Layer)                         |
 *   |    android.hardware.wifi@1.x                              |
 *   +-----------------------------------------------------------+
 *   |  KERNEL DRIVER                                            |
 *   +-----------------------------------------------------------+
 *
 *   KEY ANDROID APIs:
 *   - WifiManager: Infrastructure connections
 *   - WifiP2pManager: Wi-Fi Direct
 *   - WifiAwareManager: Wi-Fi Aware/NAN
 *   - ConnectivityManager: Network selection policy
 *
 * WINDOWS WIRELESS STACK:
 *
 *   +-----------------------------------------------------------+
 *   |  WIN32 APPS (using WLAN API)                              |
 *   +-----------------------------------------------------------+
 *   |  WLAN SERVICE (wlansvc)                                   |
 *   |    Native Wi-Fi API, Auto Config                          |
 *   +-----------------------------------------------------------+
 *   |  NDIS (Network Driver Interface Specification)            |
 *   |    NDIS 6.x for wireless                                  |
 *   +-----------------------------------------------------------+
 *   |  MINIPORT DRIVER (vendor-specific)                        |
 *   +-----------------------------------------------------------+
 *
 *   KEY WINDOWS APIs:
 *   - WlanOpenHandle, WlanEnumInterfaces, WlanConnect
 *   - WlanHostedNetworkStartUsing (soft AP)
 *   - WiFi Direct APIs (Windows.Devices.WiFiDirect)
 *
 * LINUX WIRELESS STACK:
 *   (Covered in main Wi-Fi file - cfg80211, mac80211, nl80211)
 *
 * CROSS-PLATFORM SDK DESIGN PRINCIPLES:
 *
 *   +-----------------------------------------------------------+
 *   |  COMMON API LAYER (Platform-independent interface)        |
 *   |    - Connect(ssid, password)                              |
 *   |    - StartP2PDiscovery()                                  |
 *   |    - GetConnectionStatus()                                |
 *   +-----------------------------------------------------------+
 *   |  PLATFORM ADAPTERS                                        |
 *   |    +-------------+  +-------------+  +-------------+      |
 *   |    | Android     |  | Windows     |  | Linux       |      |
 *   |    | Adapter     |  | Adapter     |  | Adapter     |      |
 *   |    | (JNI)       |  | (Win32)     |  | (nl80211)   |      |
 *   |    +-------------+  +-------------+  +-------------+      |
 *   +-----------------------------------------------------------+
 *
 * ============================================================================
 *                    ENTERPRISE WI-FI ARCHITECTURES
 * ============================================================================
 *
 * CONTROLLER-BASED (Traditional):
 *
 *   [AP] [AP] [AP] [AP]        Lightweight APs
 *     \    |    |    /         (just radios, minimal intelligence)
 *      \   |    |   /
 *       +----------+
 *       |Controller|           Central controller
 *       +----------+           (all decisions made here)
 *            |
 *       [Network]
 *
 *   - APs are "thin" - just forward frames to controller
 *   - Controller handles: roaming, security, channel selection
 *   - Protocols: CAPWAP, LWAPP (Cisco)
 *   - Pros: Centralized management, consistent policy
 *   - Cons: Single point of failure, latency for some operations
 *
 * CLOUD-MANAGED (Modern):
 *
 *   [AP] [AP] [AP] [AP]        Smart APs
 *     |    |    |    |         (local intelligence + cloud management)
 *     +----+----+----+
 *           |
 *       [Internet]
 *           |
 *     +-------------+
 *     | Cloud Mgmt  |          Config, monitoring, analytics
 *     +-------------+
 *
 *   - APs are "fat" - make local decisions
 *   - Cloud provides: config push, firmware updates, analytics
 *   - Examples: Meraki, Aruba Central, Mist
 *   - Pros: Scalable, no on-prem controller needed
 *   - Cons: Depends on internet connectivity for management
 *
 * ENTERPRISE FEATURES TO KNOW:
 *
 * 1. 802.11r (Fast BSS Transition):
 *    - Pre-authenticate to target AP before roaming
 *    - Reduces roam time from ~100ms to ~10ms
 *    - Critical for VoIP and video calls
 *
 * 2. 802.11k (Radio Resource Management):
 *    - AP provides neighbor report to client
 *    - Client knows where to roam without scanning all channels
 *    - Faster roaming decisions
 *
 * 3. 802.11v (BSS Transition Management):
 *    - AP can suggest client to roam (load balancing)
 *    - Client can request preferred APs
 *
 * 4. 802.1X / RADIUS:
 *    - Individual user authentication
 *    - EAP methods: PEAP, EAP-TLS, EAP-TTLS
 *    - Driver delivers EAP frames to supplicant
 *
 * 5. QoS (802.11e / WMM):
 *    - Traffic prioritization (voice > video > best effort)
 *    - EDCA parameters in beacon
 *    - Driver must map traffic to correct access category
 *
 * ============================================================================
 *                    WIRELESS ANALYSIS TOOLS
 * ============================================================================
 *
 * WIRESHARK FOR WI-FI:
 *
 *   Capture Setup:
 *   - Need monitor mode capable adapter
 *   - $ airmon-ng start wlan0  (Linux)
 *   - Capture on specific channel or channel hop
 *
 *   Useful Filters:
 *   - wlan.fc.type == 0          (Management frames)
 *   - wlan.fc.type_subtype == 8  (Beacon frames)
 *   - wlan.fc.type_subtype == 0  (Association request)
 *   - eapol                       (4-way handshake)
 *   - wlan.addr == aa:bb:cc:dd:ee:ff  (Specific device)
 *
 *   What to Look For:
 *   - Association sequence (probe, auth, assoc, 4-way)
 *   - Retry flags (indicating packet loss)
 *   - Reason codes in deauth/disassoc
 *   - QoS fields for traffic classification
 *
 * SPECTRUM ANALYZERS:
 *
 *   Purpose: See RF environment, not decoded packets
 *
 *   What They Show:
 *   - Channel utilization (how busy is the spectrum)
 *   - Interference sources (non-Wi-Fi: microwaves, cameras)
 *   - Signal strength across frequencies
 *   - Noise floor
 *
 *   Tools:
 *   - Wi-Spy + Chanalyzer
 *   - Metageek inSSIDer
 *   - Some enterprise APs have built-in spectrum analysis
 *
 * VENDOR DEBUGGING TOOLS:
 *
 *   - Qualcomm: QXDM, QCAT for logging
 *   - Broadcom: wl utility, DHD debug logs
 *   - Intel: iwlwifi debug logs, debugfs entries
 *
 *   Common Debug Info:
 *   - TX/RX statistics per rate
 *   - Retry counts, error counts
 *   - Firmware logs
 *   - Power state transitions
 *
 * ANDROID WIRELESS DEBUGGING:
 *
 *   $ adb shell dumpsys wifi           # Wi-Fi service state
 *   $ adb shell wpa_cli status         # wpa_supplicant state
 *   $ adb logcat -s WifiService        # Wi-Fi logs
 *   $ adb shell cmd wifi status        # Connection status
 *
 * LINUX WIRELESS DEBUGGING:
 *
 *   $ iw dev wlan0 station dump        # Station statistics
 *   $ iw dev wlan0 survey dump         # Channel survey
 *   $ cat /sys/kernel/debug/ieee80211/phy0/statistics/*
 *   $ dmesg | grep -i wifi             # Kernel messages
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
//                    CODE: WI-FI DIRECT STATE MACHINE
// ============================================================================

typedef enum {
    P2P_STATE_IDLE,
    P2P_STATE_DISCOVERING,
    P2P_STATE_GO_NEGOTIATION,
    P2P_STATE_PROVISIONING,
    P2P_STATE_GROUP_STARTED,
    P2P_STATE_CONNECTED,
    P2P_STATE_ERROR
} P2PState;

typedef enum {
    P2P_ROLE_UNDECIDED,
    P2P_ROLE_GO,        // Group Owner (soft AP)
    P2P_ROLE_CLIENT     // P2P Client
} P2PRole;

typedef struct {
    char device_name[32];
    uint8_t device_addr[6];
    uint8_t go_intent;      // 0-15, higher = more likely to be GO
    uint16_t config_methods; // Supported WPS methods
} P2PDevice;

typedef struct {
    P2PState state;
    P2PRole role;
    P2PDevice self;
    P2PDevice peer;
    uint8_t group_ssid[32];
    uint8_t operating_channel;
    bool persistent;
} P2PContext;

const char* p2p_state_str(P2PState s) {
    switch (s) {
        case P2P_STATE_IDLE:           return "IDLE";
        case P2P_STATE_DISCOVERING:    return "DISCOVERING";
        case P2P_STATE_GO_NEGOTIATION: return "GO_NEGOTIATION";
        case P2P_STATE_PROVISIONING:   return "PROVISIONING";
        case P2P_STATE_GROUP_STARTED:  return "GROUP_STARTED";
        case P2P_STATE_CONNECTED:      return "CONNECTED";
        case P2P_STATE_ERROR:          return "ERROR";
        default:                       return "UNKNOWN";
    }
}

// Determine GO based on intent values
P2PRole determine_go_role(uint8_t our_intent, uint8_t peer_intent, bool tie_breaker) {
    if (our_intent > peer_intent) {
        return P2P_ROLE_GO;
    } else if (our_intent < peer_intent) {
        return P2P_ROLE_CLIENT;
    } else {
        // Same intent - use tie breaker bit
        return tie_breaker ? P2P_ROLE_GO : P2P_ROLE_CLIENT;
    }
}

void p2p_start_discovery(P2PContext* ctx) {
    printf("[P2P] Starting device discovery...\n");
    ctx->state = P2P_STATE_DISCOVERING;
    // In real code: start sending P2P probe requests
    // Listen on social channels (1, 6, 11)
}

void p2p_connect_to_peer(P2PContext* ctx, P2PDevice* peer) {
    printf("[P2P] Initiating connection to %s\n", peer->device_name);

    // Store peer info
    memcpy(&ctx->peer, peer, sizeof(P2PDevice));

    // Start GO negotiation
    ctx->state = P2P_STATE_GO_NEGOTIATION;

    // Determine our role (simplified - real code waits for negotiation response)
    bool tie_breaker = (ctx->self.device_addr[5] > peer->device_addr[5]);
    ctx->role = determine_go_role(ctx->self.go_intent, peer->go_intent, tie_breaker);

    printf("[P2P] Our intent: %d, Peer intent: %d -> We are %s\n",
           ctx->self.go_intent, peer->go_intent,
           ctx->role == P2P_ROLE_GO ? "GROUP OWNER" : "CLIENT");
}

void p2p_group_started(P2PContext* ctx) {
    ctx->state = P2P_STATE_GROUP_STARTED;

    if (ctx->role == P2P_ROLE_GO) {
        printf("[P2P] Group Owner started, SSID: DIRECT-%s\n", ctx->self.device_name);
        printf("[P2P] Operating as soft AP, waiting for client...\n");
    } else {
        printf("[P2P] Connecting to Group Owner as client...\n");
    }
}

// ============================================================================
//                    CODE: CROSS-PLATFORM SDK ABSTRACTION
// ============================================================================

// Platform-independent wireless operations
typedef struct WirelessOps {
    int (*scan)(void* ctx);
    int (*connect)(void* ctx, const char* ssid, const char* password);
    int (*disconnect)(void* ctx);
    int (*get_rssi)(void* ctx);
    int (*start_p2p_discovery)(void* ctx);
    int (*p2p_connect)(void* ctx, const uint8_t* peer_addr);
} WirelessOps;

// Platform-specific implementations would fill this
typedef struct {
    WirelessOps* ops;
    void* platform_ctx;  // Platform-specific context
    char interface_name[16];
} WirelessSDK;

// Example: Common API that works across platforms
int sdk_connect_to_network(WirelessSDK* sdk, const char* ssid, const char* password) {
    printf("[SDK] Connecting to %s...\n", ssid);

    if (sdk->ops && sdk->ops->connect) {
        return sdk->ops->connect(sdk->platform_ctx, ssid, password);
    }

    return -1;  // Not implemented
}

int sdk_get_signal_strength(WirelessSDK* sdk) {
    if (sdk->ops && sdk->ops->get_rssi) {
        return sdk->ops->get_rssi(sdk->platform_ctx);
    }
    return -100;  // Invalid
}

// ============================================================================
//                    CODE: COEXISTENCE MANAGER
// ============================================================================

typedef enum {
    COEX_TRAFFIC_BT_HID,
    COEX_TRAFFIC_BT_A2DP,
    COEX_TRAFFIC_BT_SCO,
    COEX_TRAFFIC_WIFI_VOIP,
    COEX_TRAFFIC_WIFI_VIDEO,
    COEX_TRAFFIC_WIFI_DATA
} CoexTrafficType;

typedef struct {
    CoexTrafficType type;
    uint8_t priority;       // 0-255, higher = more important
    uint32_t min_time_us;   // Minimum time slot needed
} CoexProfile;

typedef struct {
    bool bt_active;
    bool wifi_active;
    CoexProfile bt_profile;
    CoexProfile wifi_profile;
    uint32_t bt_grants;
    uint32_t bt_denies;
    uint32_t wifi_grants;
    uint32_t wifi_denies;
} CoexManager;

// Priority table
static const CoexProfile coex_profiles[] = {
    { COEX_TRAFFIC_BT_SCO,     250, 3750 },  // Voice: high priority, ~3.75ms slot
    { COEX_TRAFFIC_BT_HID,     200, 1250 },  // HID: high priority, 1.25ms
    { COEX_TRAFFIC_WIFI_VOIP,  200, 5000 },  // VoIP: high priority
    { COEX_TRAFFIC_BT_A2DP,    150, 5000 },  // Audio: medium
    { COEX_TRAFFIC_WIFI_VIDEO, 150, 8000 },  // Video: medium
    { COEX_TRAFFIC_WIFI_DATA,  100, 10000 }, // Data: low
};

bool coex_request_channel(CoexManager* mgr, bool is_bluetooth) {
    // Simple priority-based arbitration
    if (is_bluetooth) {
        if (!mgr->wifi_active || mgr->bt_profile.priority >= mgr->wifi_profile.priority) {
            mgr->bt_grants++;
            return true;
        }
        mgr->bt_denies++;
        return false;
    } else {
        if (!mgr->bt_active || mgr->wifi_profile.priority >= mgr->bt_profile.priority) {
            mgr->wifi_grants++;
            return true;
        }
        mgr->wifi_denies++;
        return false;
    }
}

void coex_print_stats(CoexManager* mgr) {
    printf("\n=== Coexistence Statistics ===\n");
    printf("BT:   Grants: %u, Denies: %u (%.1f%% success)\n",
           mgr->bt_grants, mgr->bt_denies,
           100.0 * mgr->bt_grants / (mgr->bt_grants + mgr->bt_denies + 1));
    printf("WiFi: Grants: %u, Denies: %u (%.1f%% success)\n",
           mgr->wifi_grants, mgr->wifi_denies,
           100.0 * mgr->wifi_grants / (mgr->wifi_grants + mgr->wifi_denies + 1));
}

// ============================================================================
//                    CODE: QoS / TRAFFIC CLASSIFICATION
// ============================================================================

typedef enum {
    AC_BK = 0,  // Background (lowest)
    AC_BE = 1,  // Best Effort
    AC_VI = 2,  // Video
    AC_VO = 3   // Voice (highest)
} AccessCategory;

typedef struct {
    uint16_t cw_min;        // Minimum contention window
    uint16_t cw_max;        // Maximum contention window
    uint8_t aifsn;          // Arbitration IFS number
    uint16_t txop_limit;    // TX opportunity limit (microseconds)
} EdcaParams;

// Default EDCA parameters (from 802.11 spec)
static const EdcaParams default_edca[] = {
    [AC_BK] = { 15, 1023, 7, 0 },      // Background: lowest priority
    [AC_BE] = { 15, 1023, 3, 0 },      // Best Effort
    [AC_VI] = { 7,  15,   2, 3008 },   // Video: higher priority
    [AC_VO] = { 3,  7,    2, 1504 },   // Voice: highest priority
};

// Classify traffic based on DSCP (IP header)
AccessCategory classify_traffic(uint8_t dscp) {
    // DSCP to 802.11 AC mapping (simplified)
    if (dscp >= 46) return AC_VO;      // EF (Expedited Forwarding) -> Voice
    if (dscp >= 32) return AC_VI;      // AF4x -> Video
    if (dscp >= 24) return AC_VI;      // AF3x -> Video
    if (dscp >= 8)  return AC_BE;      // AF1x, AF2x -> Best Effort
    return AC_BK;                       // Default -> Background
}

// ============================================================================
//                            DEMO
// ============================================================================

int main() {
    printf("==========================================================\n");
    printf("   Wi-Fi P2P, ENTERPRISE & CROSS-PLATFORM DEMO\n");
    printf("==========================================================\n\n");

    // Demo 1: Wi-Fi Direct
    printf("--- Wi-Fi Direct Demo ---\n");
    P2PContext p2p = {0};
    p2p.self.go_intent = 7;  // Medium intent
    strcpy(p2p.self.device_name, "HP-Laptop");
    p2p.self.device_addr[5] = 0x42;

    p2p_start_discovery(&p2p);

    // Simulate finding a printer
    P2PDevice printer = {
        .device_name = "HP-Printer",
        .go_intent = 14,  // Printer wants to be GO
        .device_addr = {0, 0, 0, 0, 0, 0x10}
    };

    p2p_connect_to_peer(&p2p, &printer);
    p2p_group_started(&p2p);

    // Demo 2: Coexistence
    printf("\n--- Coexistence Demo ---\n");
    CoexManager coex = {0};
    coex.bt_active = true;
    coex.wifi_active = true;
    coex.bt_profile = coex_profiles[1];   // BT HID
    coex.wifi_profile = coex_profiles[5]; // WiFi Data

    for (int i = 0; i < 10; i++) {
        coex_request_channel(&coex, i % 2 == 0);  // Alternate BT/WiFi
    }
    coex_print_stats(&coex);

    // Demo 3: QoS Classification
    printf("\n--- QoS Classification Demo ---\n");
    uint8_t test_dscp[] = {0, 10, 26, 34, 46};
    const char* ac_names[] = {"Background", "Best Effort", "Video", "Voice"};

    for (int i = 0; i < 5; i++) {
        AccessCategory ac = classify_traffic(test_dscp[i]);
        printf("DSCP %2d -> %s\n", test_dscp[i], ac_names[ac]);
    }

    return 0;
}

/*
 * ============================================================================
 *                    HP IQ SPECIFIC INTERVIEW QUESTIONS
 * ============================================================================
 *
 * Q1: "Explain Wi-Fi Direct and when you would use it"
 *
 * STRONG ANSWER:
 * "Wi-Fi Direct allows two devices to connect directly without needing
 *  an access point. One device becomes the Group Owner, essentially
 *  acting as a soft AP, and the other connects as a client.
 *
 *  The GO role is determined through negotiation using GO Intent values.
 *  For HP's use case with printers, the printer would typically have high
 *  intent to always be the GO - it's always powered on and ready.
 *
 *  Key advantages over Bluetooth: much higher throughput for things like
 *  printing large documents or screen mirroring. Key advantages over
 *  infrastructure Wi-Fi: works without any network setup.
 *
 *  I'd use it for: printing, Miracast display mirroring, large file
 *  transfers between devices, and any scenario where you want fast
 *  peer-to-peer communication without infrastructure dependency."
 *
 * ---
 *
 * Q2: "How do Wi-Fi and Bluetooth coexist on the same device?"
 *
 * STRONG ANSWER:
 * "Both radios operate in the 2.4 GHz band, so without coordination they'd
 *  interfere badly. There are several coexistence mechanisms:
 *
 *  Time-division is most common - an arbiter gives each radio time slots.
 *  Bluetooth gets guaranteed slots for things like audio, Wi-Fi gets the
 *  rest. Priority is based on traffic type - voice calls and HID input
 *  get high priority.
 *
 *  Frequency avoidance helps too - Bluetooth's adaptive frequency hopping
 *  can avoid channels where Wi-Fi is operating.
 *
 *  At the software level, we configure priority weights and monitor
 *  statistics. If we see high deny rates for one radio, we might need to
 *  adjust parameters or move Wi-Fi to 5 GHz.
 *
 *  For HP products with both radios, this is critical - users expect
 *  Bluetooth audio to work smoothly while downloading over Wi-Fi."
 *
 * ---
 *
 * Q3: "How would you design a cross-platform wireless SDK?"
 *
 * STRONG ANSWER:
 * "I'd use a layered architecture:
 *
 *  Top layer: Platform-independent API. Functions like Connect(), Scan(),
 *  GetSignalStrength() that applications call regardless of platform.
 *
 *  Middle layer: Platform adapters. Each platform (Android, Windows, Linux)
 *  has an adapter that translates the common API to platform-specific calls.
 *  Android uses WifiManager through JNI, Windows uses the WLAN API, Linux
 *  uses nl80211.
 *
 *  Key design decisions:
 *  - Async callbacks for operations that take time (scanning, connecting)
 *  - Error codes that make sense across platforms
 *  - Feature detection since not all platforms support everything
 *  - Thread safety for the common state
 *
 *  For P2P features, I'd provide a unified discovery and connection API
 *  that maps to Wi-Fi Direct on each platform."
 *
 * ---
 *
 * Q4: "Explain enterprise Wi-Fi fast roaming"
 *
 * STRONG ANSWER:
 * "In enterprise environments, seamless roaming is critical for VoIP and
 *  video calls. 802.11r, k, and v work together:
 *
 *  802.11k lets the AP send a neighbor report, so the client knows which
 *  APs are nearby without scanning all channels. Faster decision making.
 *
 *  802.11r enables Fast BSS Transition. The client pre-authenticates with
 *  the target AP before roaming, and key derivation is optimized. Roam
 *  time drops from ~100ms to ~10ms.
 *
 *  802.11v allows the AP to suggest roaming - useful for load balancing.
 *
 *  From a driver perspective, we need to support these features, handle
 *  the pre-authentication frames, and coordinate with wpa_supplicant
 *  for the key management."
 *
 * ---
 *
 * Q5: "Walk through debugging a P2P connection failure"
 *
 * STRONG ANSWER:
 * "I'd work through the P2P state machine:
 *
 *  1. Discovery phase: Are we seeing the peer device? Check P2P probe
 *     responses in Wireshark. Verify we're on the right social channels.
 *
 *  2. GO negotiation: Check for GO negotiation frames. Look at intent
 *     values and confirm negotiation completes. Common issue: one side
 *     times out waiting for response.
 *
 *  3. Group formation: If we're GO, verify soft AP started. If client,
 *     verify we're trying to connect to the right SSID.
 *
 *  4. Provisioning (WPS): Check for WPS M1-M8 exchange. PIN mismatch or
 *     PBC timing can cause failures here.
 *
 *  5. 4-way handshake: Same as regular WPA2 - verify key exchange completes.
 *
 *  Tools I'd use: Wireshark with P2P filters, wpa_supplicant debug logs,
 *  and platform-specific logs (logcat on Android, Event Viewer on Windows)."
 *
 * ---
 *
 * Q6: "What's Wi-Fi Aware and how is it different from Wi-Fi Direct?"
 *
 * STRONG ANSWER:
 * "Wi-Fi Aware, or NAN, is primarily a discovery protocol. Devices form
 *  clusters and exchange small discovery messages on synchronized schedules.
 *  It's very power-efficient because devices sleep most of the time.
 *
 *  The key difference from Wi-Fi Direct:
 *  - Wi-Fi Direct is for data transfer - you form a full P2P group
 *  - Wi-Fi Aware is for discovery - finding nearby services
 *
 *  With Aware, you publish services like 'printer available' and subscribe
 *  to find services. Once you discover something, you can escalate to
 *  a data path - either NAN Data Path or Wi-Fi Direct.
 *
 *  For HP, Aware could enable scenarios like 'find nearby HP printers'
 *  without the user having to join any network first. Very useful for
 *  first-time setup or guest printing."
 *
 * ============================================================================
 *                    TOOLS CHEAT SHEET FOR HP IQ
 * ============================================================================
 *
 * WIRESHARK P2P FILTERS:
 *   wifi.p2p                           # All P2P frames
 *   wifi.p2p.attr.device_name          # Filter by device name
 *   wlan.fc.type_subtype == 0x0d       # Action frames (P2P uses these)
 *
 * ANDROID P2P DEBUGGING:
 *   $ adb shell dumpsys wifip2p        # P2P service state
 *   $ adb logcat -s WifiP2pService     # P2P logs
 *   $ adb shell cmd wifi status        # Overall Wi-Fi status
 *
 * LINUX P2P (wpa_supplicant):
 *   $ wpa_cli p2p_find                 # Start discovery
 *   $ wpa_cli p2p_peers                # List found peers
 *   $ wpa_cli p2p_connect <addr> pbc   # Connect with push button
 *   $ wpa_cli p2p_group_add            # Create autonomous GO
 *
 * WINDOWS P2P:
 *   - WiFi Direct APIs in Windows.Devices.WiFiDirect namespace
 *   - Event Viewer > Microsoft-Windows-WLAN-AutoConfig
 *   - netsh wlan show all
 *
 * ============================================================================
 */
