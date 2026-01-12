/*
 * ============================================================================
 *           IoT NETWORKING FOR FIRMWARE ENGINEERS (TCP/IP STACK)
 * ============================================================================
 *
 * What a firmware engineer needs to know about networking for IoT devices.
 * Not network engineering - just enough to debug, design, and interview.
 *
 * ============================================================================
 *                      THE TCP/IP MODEL (SIMPLIFIED)
 * ============================================================================
 *
 * Layer 4: APPLICATION   - Your data (MQTT, HTTP, CoAP, custom protocol)
 * Layer 3: TRANSPORT     - TCP (reliable) or UDP (fast)
 * Layer 2: NETWORK       - IP addresses, routing
 * Layer 1: LINK          - WiFi, Ethernet, Cellular, LoRa
 *
 * YOUR FIRMWARE TOUCHES ALL LAYERS:
 *   - Application: Your MQTT publish, HTTP GET
 *   - Transport: Choosing TCP vs UDP
 *   - Network: Setting IP address, DHCP
 *   - Link: WiFi credentials, cellular APN
 *
 * ============================================================================
 *                     TCP vs UDP (THE KEY DECISION)
 * ============================================================================
 *
 * TCP (Transmission Control Protocol):
 *   + Reliable: Data arrives in order, retransmits if lost
 *   + Connection-based: Connect -> Send -> Close
 *   - Overhead: 3-way handshake, ACKs, more data
 *   - Latency: Waits for ACKs
 *   USE FOR: MQTT, HTTP, critical data, cloud communication
 *
 * UDP (User Datagram Protocol):
 *   + Fast: No handshake, no waiting
 *   + Lightweight: Less overhead
 *   - Unreliable: Packets can be lost, duplicated, out of order
 *   - YOU handle reliability (if needed)
 *   USE FOR: Streaming, DNS, NTP, local discovery, sensor broadcast
 *
 * RULE OF THUMB:
 *   - Important data to cloud? TCP (via MQTT/HTTPS)
 *   - Real-time streaming? UDP
 *   - Battery-constrained + local network? UDP might save power
 *
 * ============================================================================
 *                       COMMON IoT PROTOCOLS
 * ============================================================================
 *
 * MQTT (Message Queuing Telemetry Transport):
 *   - Publish/Subscribe model
 *   - Broker-based (device -> broker -> subscribers)
 *   - Lightweight, designed for IoT
 *   - Topics: "sensor/temp", "device/123/status"
 *   - QoS levels: 0 (fire-forget), 1 (at least once), 2 (exactly once)
 *   - Runs over TCP (port 1883, or 8883 with TLS)
 *
 * HTTP/HTTPS:
 *   - Request/Response model
 *   - GET, POST, PUT, DELETE
 *   - Heavy for IoT, but universal
 *   - Use for: REST APIs, OTA updates, web configuration
 *
 * CoAP (Constrained Application Protocol):
 *   - Like HTTP but for constrained devices
 *   - Runs over UDP
 *   - Smaller overhead than HTTP
 *   - Use for: Very constrained devices, LwM2M
 *
 * ============================================================================
 *                      IP ADDRESSING BASICS
 * ============================================================================
 *
 * IPv4 ADDRESS: 192.168.1.100 (4 bytes, dotted decimal)
 *
 * STATIC IP: You configure the address manually.
 *   + Predictable, always the same
 *   - Must manage, can conflict
 *
 * DHCP: Device requests address from router.
 *   + Automatic, no configuration
 *   - Address may change
 *   - Depends on DHCP server availability
 *
 * PORT: Identifies the application (0-65535)
 *   - HTTP: 80, HTTPS: 443
 *   - MQTT: 1883 (unencrypted), 8883 (TLS)
 *   - Your custom protocol: pick a high port (e.g., 5000)
 *
 * SOCKET: IP + Port + Protocol = unique connection endpoint
 *   192.168.1.100:5000 (TCP) is different from 192.168.1.100:5000 (UDP)
 *
 * ============================================================================
 *                    TYPICAL IoT DEVICE NETWORK STACK
 * ============================================================================
 *
 *   +---------------------------+
 *   |    Your Application       |  <- sensor reading, commands
 *   +---------------------------+
 *   |   MQTT / HTTP / CoAP      |  <- protocol library
 *   +---------------------------+
 *   |     TCP / UDP             |  <- socket API
 *   +---------------------------+
 *   |       IP                  |  <- lwIP, built into module
 *   +---------------------------+
 *   |   WiFi / Cellular / ETH   |  <- driver, AT commands, SDK
 *   +---------------------------+
 *   |      Hardware             |  <- ESP32, nRF, cellular modem
 *   +---------------------------+
 *
 * ON ESP32/ESP-IDF:
 *   - lwIP provides TCP/IP stack
 *   - You create sockets, connect, send, recv
 *   - MQTT library (e.g., esp-mqtt) handles MQTT over TCP
 *
 * ON CELLULAR MODEM (via AT commands):
 *   - AT+CIPSTART="TCP","broker.com",1883
 *   - AT+CIPSEND=<length>
 *   - Modem handles TCP/IP internally
 *
 * ============================================================================
 *                      SECURITY CONSIDERATIONS
 * ============================================================================
 *
 * TLS/SSL: Encrypted communication
 *   - HTTPS, MQTTS, etc.
 *   - Requires certificates
 *   - CPU and memory overhead
 *
 * CERTIFICATES:
 *   - CA cert: Verify server identity
 *   - Client cert: Server verifies device (mutual TLS)
 *   - Store in flash, never hardcode in repo!
 *
 * AUTHENTICATION:
 *   - Username/password for MQTT
 *   - API keys for HTTP
 *   - Client certificates for strong auth
 *
 * ============================================================================
 *                      DEBUGGING NETWORK ISSUES
 * ============================================================================
 *
 * COMMON PROBLEMS:
 *   1. Can't connect to WiFi: Check SSID, password, signal strength
 *   2. No IP address: DHCP issue, check router
 *   3. Can't reach server: DNS failure, wrong IP, firewall
 *   4. Connection drops: WiFi range, server timeout, keep-alive
 *   5. Data not arriving: Check port, protocol mismatch, TLS issue
 *
 * DEBUGGING TOOLS:
 *   - ping: Test IP connectivity
 *   - netcat (nc): Raw TCP/UDP testing
 *   - Wireshark: Packet capture (if you can tap the network)
 *   - Serial logs: Print everything!
 *   - mosquitto_sub/pub: Test MQTT broker
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// Pseudo-code demonstration of IoT networking concepts
// (Not real network code - just structure illustration)

// Simulated socket operations
typedef int socket_t;

socket_t socket_create(int tcp_or_udp) {
    printf("Creating %s socket\n", tcp_or_udp ? "TCP" : "UDP");
    return 1; // Fake socket handle
}

int socket_connect(socket_t sock, const char* ip, int port) {
    printf("Connecting to %s:%d\n", ip, port);
    return 0; // Success
}

int socket_send(socket_t sock, const char* data, int len) {
    printf("Sending %d bytes: %s\n", len, data);
    return len;
}

int socket_recv(socket_t sock, char* buf, int maxlen) {
    printf("Receiving data...\n");
    return 0;
}

void socket_close(socket_t sock) {
    printf("Closing socket\n");
}

// MQTT-like publish (simplified)
void mqtt_publish(const char* topic, const char* message) {
    printf("MQTT PUBLISH to '%s': %s\n", topic, message);
}

int main() {
    printf("=== IoT Networking Demo ===\n\n");

    // Step 1: Create TCP socket
    printf("Step 1: Create socket\n");
    socket_t sock = socket_create(1);  // 1 = TCP

    // Step 2: Connect to MQTT broker
    printf("\nStep 2: Connect to broker\n");
    socket_connect(sock, "broker.example.com", 1883);

    // Step 3: Send data (in reality, this would be MQTT CONNECT packet)
    printf("\nStep 3: Send MQTT CONNECT\n");
    socket_send(sock, "CONNECT", 7);

    // Step 4: Publish sensor data
    printf("\nStep 4: Publish sensor reading\n");
    mqtt_publish("sensor/temperature", "25.5");

    // Step 5: Clean up
    printf("\nStep 5: Disconnect\n");
    socket_close(sock);

    return 0;
}

/*
 * ============================================================================
 * DRY RUN: Typical IoT Device Startup Sequence
 * ============================================================================
 *
 * 1. HARDWARE INIT
 *    - Initialize WiFi/cellular module
 *    - Load credentials from flash
 *
 * 2. CONNECT TO NETWORK
 *    - WiFi: Scan -> Connect to SSID -> Wait for IP (DHCP)
 *    - Cellular: Power modem -> Configure APN -> Attach to network
 *
 * 3. RESOLVE SERVER ADDRESS
 *    - DNS lookup: "broker.example.com" -> "54.123.45.67"
 *    - Cache result to avoid repeated lookups
 *
 * 4. ESTABLISH CONNECTION
 *    - Create TCP socket
 *    - Connect to broker IP:port (1883)
 *    - TCP 3-way handshake happens automatically
 *
 * 5. TLS HANDSHAKE (if using MQTTS on 8883)
 *    - Exchange certificates
 *    - Establish encrypted channel
 *
 * 6. MQTT CONNECT
 *    - Send CONNECT packet with client ID, username, password
 *    - Receive CONNACK
 *
 * 7. SUBSCRIBE / PUBLISH
 *    - Subscribe to topics for commands
 *    - Publish sensor data to topics
 *
 * 8. MAINTAIN CONNECTION
 *    - Send PING every N seconds (keep-alive)
 *    - Handle reconnection on disconnect
 *
 * ============================================================================
 * INTERVIEW TIPS:
 * - TCP = reliable, connection-based (MQTT, HTTP)
 * - UDP = fast, connectionless (streaming, discovery)
 * - MQTT uses topics (pub/sub), runs over TCP
 * - Port 1883 = MQTT, 8883 = MQTT over TLS
 * - Always handle: DNS failure, connection drops, reconnection
 * ============================================================================
 */
