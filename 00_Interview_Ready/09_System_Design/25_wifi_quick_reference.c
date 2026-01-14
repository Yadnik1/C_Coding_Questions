/*
 * ============================================================================
 *     Wi-Fi QUICK REFERENCE - ONE-LINERS & SIMPLE QUESTIONS
 * ============================================================================
 *
 * For recruiter screens and rapid-fire technical questions.
 * Memorize these - they come up constantly.
 *
 * ============================================================================
 *                    BASIC DEFINITIONS (1-LINERS)
 * ============================================================================
 *
 * Q: What is Wi-Fi?
 * A: Wireless LAN technology based on IEEE 802.11 standards.
 *
 * Q: What frequency bands does Wi-Fi use?
 * A: 2.4 GHz and 5 GHz (Wi-Fi 6E also uses 6 GHz).
 *
 * Q: What is an SSID?
 * A: Service Set Identifier - the network name you see when scanning.
 *
 * Q: What is a BSSID?
 * A: Basic Service Set Identifier - the MAC address of the access point.
 *
 * Q: What is an AP?
 * A: Access Point - the device that creates the Wi-Fi network.
 *
 * Q: What is a STA?
 * A: Station - a client device that connects to an AP.
 *
 * Q: What is RSSI?
 * A: Received Signal Strength Indicator - measured in dBm (e.g., -50 dBm is good).
 *
 * Q: What is a good RSSI value?
 * A: -30 to -50 dBm is excellent, -60 is fair, below -70 is weak.
 *
 * Q: What is a beacon frame?
 * A: A frame sent by AP periodically (typically every 100ms) announcing its presence.
 *
 * Q: What is the beacon interval?
 * A: Time between beacons, typically 100 TU (102.4 ms). 1 TU = 1024 microseconds.
 *
 * ============================================================================
 *                    802.11 STANDARDS (KNOW THESE!)
 * ============================================================================
 *
 * Q: What is 802.11a?
 * A: 5 GHz, up to 54 Mbps, uses OFDM.
 *
 * Q: What is 802.11b?
 * A: 2.4 GHz, up to 11 Mbps, uses DSSS (oldest, legacy).
 *
 * Q: What is 802.11g?
 * A: 2.4 GHz, up to 54 Mbps, backward compatible with 802.11b.
 *
 * Q: What is 802.11n (Wi-Fi 4)?
 * A: 2.4/5 GHz, up to 600 Mbps, introduced MIMO and 40 MHz channels.
 *
 * Q: What is 802.11ac (Wi-Fi 5)?
 * A: 5 GHz only, up to 6.9 Gbps, 80/160 MHz channels, MU-MIMO.
 *
 * Q: What is 802.11ax (Wi-Fi 6)?
 * A: 2.4/5 GHz, up to 9.6 Gbps, OFDMA, BSS coloring, TWT.
 *
 * Q: What is Wi-Fi 6E?
 * A: Wi-Fi 6 extended to 6 GHz band (more spectrum, less congestion).
 *
 * Q: What does HT stand for?
 * A: High Throughput (802.11n).
 *
 * Q: What does VHT stand for?
 * A: Very High Throughput (802.11ac).
 *
 * Q: What does HE stand for?
 * A: High Efficiency (802.11ax).
 *
 * ============================================================================
 *                    SECURITY (VERY COMMON QUESTIONS)
 * ============================================================================
 *
 * Q: What is WEP?
 * A: Wired Equivalent Privacy - old, broken, never use it.
 *
 * Q: What is WPA?
 * A: Wi-Fi Protected Access - replaced WEP, uses TKIP (also deprecated).
 *
 * Q: What is WPA2?
 * A: Current standard, uses AES-CCMP encryption, 4-way handshake.
 *
 * Q: What is WPA3?
 * A: Latest standard, uses SAE (Dragonfly) for better password protection.
 *
 * Q: What is PSK?
 * A: Pre-Shared Key - same password for all users (home networks).
 *
 * Q: What is 802.1X?
 * A: Enterprise authentication using RADIUS server (individual credentials).
 *
 * Q: What is the 4-way handshake?
 * A: Key exchange after association to derive encryption keys (PTK/GTK).
 *
 * Q: What is PTK?
 * A: Pairwise Transient Key - encrypts unicast traffic between STA and AP.
 *
 * Q: What is GTK?
 * A: Group Temporal Key - encrypts broadcast/multicast traffic.
 *
 * Q: What is SAE?
 * A: Simultaneous Authentication of Equals - WPA3's password-based auth.
 *
 * Q: Why is WPA3 better than WPA2?
 * A: Resistant to offline dictionary attacks, provides forward secrecy.
 *
 * Q: What is CCMP?
 * A: Counter Mode with CBC-MAC Protocol - AES-based encryption in WPA2.
 *
 * Q: What is TKIP?
 * A: Temporal Key Integrity Protocol - older, weaker, used in WPA1.
 *
 * ============================================================================
 *                    CONNECTION PROCESS
 * ============================================================================
 *
 * Q: What are the steps to connect to Wi-Fi?
 * A: Scan → Authenticate → Associate → 4-way handshake → DHCP.
 *
 * Q: What is scanning?
 * A: Finding available networks by listening for beacons or sending probes.
 *
 * Q: What is active scanning?
 * A: Sending probe requests and waiting for probe responses.
 *
 * Q: What is passive scanning?
 * A: Just listening for beacon frames.
 *
 * Q: What is authentication in 802.11?
 * A: First step of joining - Open System or Shared Key (legacy).
 *
 * Q: What is association?
 * A: Second step - exchanging capabilities and joining the BSS.
 *
 * Q: What is AID?
 * A: Association ID - assigned by AP to identify the client.
 *
 * Q: What is disassociation?
 * A: Client leaves the network gracefully.
 *
 * Q: What is deauthentication?
 * A: Forced disconnection (can be from AP or client).
 *
 * Q: What are reason codes?
 * A: Numbers indicating why a disconnect happened (e.g., 1=unspecified).
 *
 * ============================================================================
 *                    FRAME TYPES
 * ============================================================================
 *
 * Q: What are the three main 802.11 frame types?
 * A: Management, Control, and Data frames.
 *
 * Q: What are management frames?
 * A: Beacon, Probe Req/Resp, Auth, Assoc, Deauth, Disassoc.
 *
 * Q: What are control frames?
 * A: ACK, RTS, CTS, Block ACK.
 *
 * Q: What are data frames?
 * A: Frames carrying actual payload (user data).
 *
 * Q: What is RTS/CTS?
 * A: Request to Send / Clear to Send - helps with hidden node problem.
 *
 * Q: What is the hidden node problem?
 * A: Two stations can't hear each other but both can hear the AP.
 *
 * Q: What is a probe request?
 * A: Frame sent by STA to discover networks (active scanning).
 *
 * Q: What is a probe response?
 * A: AP's reply to probe request with network info.
 *
 * ============================================================================
 *                    MIMO & CHANNELS
 * ============================================================================
 *
 * Q: What is MIMO?
 * A: Multiple Input Multiple Output - multiple antennas for better throughput.
 *
 * Q: What does 2x2 MIMO mean?
 * A: 2 transmit antennas, 2 receive antennas.
 *
 * Q: What is spatial streams?
 * A: Independent data streams sent simultaneously over MIMO antennas.
 *
 * Q: What is beamforming?
 * A: Focusing signal toward the client for better range/throughput.
 *
 * Q: What is MU-MIMO?
 * A: Multi-User MIMO - AP talks to multiple clients simultaneously.
 *
 * Q: What is channel bonding?
 * A: Combining adjacent channels for more bandwidth (20+20=40 MHz).
 *
 * Q: What channel widths exist?
 * A: 20 MHz, 40 MHz, 80 MHz, 160 MHz.
 *
 * Q: How many non-overlapping channels in 2.4 GHz?
 * A: 3 (channels 1, 6, 11 in US).
 *
 * Q: Why is 5 GHz better for high bandwidth?
 * A: More non-overlapping channels, less interference.
 *
 * Q: Why is 2.4 GHz better for range?
 * A: Lower frequency penetrates walls better.
 *
 * ============================================================================
 *                    POWER SAVE
 * ============================================================================
 *
 * Q: What is power save mode?
 * A: STA sleeps between beacons to save battery.
 *
 * Q: What is DTIM?
 * A: Delivery Traffic Indication Message - tells STA if buffered data exists.
 *
 * Q: What is DTIM interval?
 * A: Number of beacons between DTIMs (e.g., DTIM=3 means every 3rd beacon).
 *
 * Q: What is PS-Poll?
 * A: Frame sent by STA to retrieve buffered data from AP.
 *
 * Q: What is U-APSD?
 * A: Unscheduled Automatic Power Save Delivery - more efficient than PS-Poll.
 *
 * Q: What is TWT?
 * A: Target Wake Time (Wi-Fi 6) - scheduled wake times for better power save.
 *
 * Q: What is listen interval?
 * A: How often STA wakes to check for beacons (agreed during association).
 *
 * ============================================================================
 *                    QoS (Quality of Service)
 * ============================================================================
 *
 * Q: What is WMM?
 * A: Wi-Fi Multimedia - QoS for prioritizing traffic.
 *
 * Q: What are the 4 access categories?
 * A: Voice (VO), Video (VI), Best Effort (BE), Background (BK).
 *
 * Q: Which access category has highest priority?
 * A: Voice (VO).
 *
 * Q: What is EDCA?
 * A: Enhanced Distributed Channel Access - mechanism for QoS in 802.11e.
 *
 * Q: What is TXOP?
 * A: Transmission Opportunity - time a station can transmit after winning channel.
 *
 * ============================================================================
 *                    P2P (WI-FI DIRECT)
 * ============================================================================
 *
 * Q: What is Wi-Fi Direct?
 * A: Peer-to-peer Wi-Fi connection without infrastructure AP.
 *
 * Q: What is a Group Owner (GO)?
 * A: The device acting as soft AP in a P2P group.
 *
 * Q: What is GO Intent?
 * A: Value (0-15) indicating how much a device wants to be GO.
 *
 * Q: What is GO Negotiation?
 * A: Process to decide which device becomes GO.
 *
 * Q: What is a persistent group?
 * A: Saved P2P group for faster reconnection.
 *
 * Q: What is Wi-Fi Aware (NAN)?
 * A: Low-power discovery protocol for finding nearby devices/services.
 *
 * Q: Difference between Wi-Fi Direct and Wi-Fi Aware?
 * A: Direct is for data transfer, Aware is for discovery.
 *
 * ============================================================================
 *                    ENTERPRISE
 * ============================================================================
 *
 * Q: What is 802.11r?
 * A: Fast BSS Transition - reduces roaming time.
 *
 * Q: What is 802.11k?
 * A: Radio Resource Management - AP provides neighbor reports.
 *
 * Q: What is 802.11v?
 * A: BSS Transition Management - AP can suggest roaming.
 *
 * Q: What is roaming?
 * A: Moving from one AP to another while maintaining connection.
 *
 * Q: What is a RADIUS server?
 * A: Server for enterprise authentication (802.1X).
 *
 * Q: What is EAP?
 * A: Extensible Authentication Protocol - framework for enterprise auth.
 *
 * Q: Name some EAP methods.
 * A: PEAP, EAP-TLS, EAP-TTLS, EAP-FAST.
 *
 * ============================================================================
 *                    LINUX/SOFTWARE STACK
 * ============================================================================
 *
 * Q: What is wpa_supplicant?
 * A: User-space daemon handling Wi-Fi authentication and association.
 *
 * Q: What is hostapd?
 * A: User-space daemon for running AP mode.
 *
 * Q: What is cfg80211?
 * A: Linux kernel configuration API for wireless.
 *
 * Q: What is mac80211?
 * A: Linux kernel software MAC implementation.
 *
 * Q: What is nl80211?
 * A: Netlink interface between user space and cfg80211.
 *
 * Q: What is a wireless driver's job?
 * A: Interface between mac80211 and hardware (register access, DMA, interrupts).
 *
 * Q: What is monitor mode?
 * A: Capturing all Wi-Fi frames without associating.
 *
 * Q: What tool shows Wi-Fi info on Linux?
 * A: iw (modern) or iwconfig (deprecated).
 *
 * ============================================================================
 *                    DEBUGGING / TOOLS
 * ============================================================================
 *
 * Q: How to see available networks on Linux?
 * A: iw dev wlan0 scan
 *
 * Q: How to check connection status?
 * A: iw dev wlan0 link
 *
 * Q: How to see station statistics?
 * A: iw dev wlan0 station dump
 *
 * Q: What tool captures Wi-Fi packets?
 * A: Wireshark (with monitor mode adapter) or tcpdump.
 *
 * Q: What is a spectrum analyzer used for?
 * A: Viewing RF environment, finding interference sources.
 *
 * Q: How to check Android Wi-Fi status?
 * A: adb shell dumpsys wifi
 *
 * ============================================================================
 *                    COMMON ISSUES (TROUBLESHOOTING)
 * ============================================================================
 *
 * Q: Device won't connect - first thing to check?
 * A: Is the AP visible in scan results?
 *
 * Q: Connected but no internet - first thing to check?
 * A: Did you get an IP address? (DHCP working?)
 *
 * Q: Slow speeds - what to check?
 * A: RSSI, channel congestion, negotiated rate.
 *
 * Q: Intermittent disconnects - common causes?
 * A: Weak signal, power save bugs, AP kicking idle clients.
 *
 * Q: 4-way handshake fails - most likely cause?
 * A: Wrong password.
 *
 * Q: What does "beacon loss" mean?
 * A: Client didn't receive expected beacons (signal issue or AP problem).
 *
 * Q: What does "reason code 7" mean?
 * A: Class 3 frame from non-associated STA (common disconnect reason).
 *
 * ============================================================================
 *                    BLUETOOTH COEXISTENCE
 * ============================================================================
 *
 * Q: Why do Wi-Fi and Bluetooth interfere?
 * A: Both use 2.4 GHz band.
 *
 * Q: How is coexistence handled?
 * A: Time-division, frequency avoidance (AFH), priority arbitration.
 *
 * Q: What is AFH?
 * A: Adaptive Frequency Hopping - BT avoids Wi-Fi channels.
 *
 * Q: Which has higher priority - BT voice or Wi-Fi video?
 * A: BT voice (real-time audio is critical).
 *
 * ============================================================================
 *                    QUICK NUMBERS TO REMEMBER
 * ============================================================================
 *
 * Beacon interval:        100 TU (~102.4 ms)
 * 1 TU:                   1024 microseconds
 * Good RSSI:              -30 to -50 dBm
 * Fair RSSI:              -60 dBm
 * Weak RSSI:              -70 dBm or worse
 * 2.4 GHz channels (US):  1-11 (use 1, 6, 11)
 * 5 GHz channels:         36-165 (varies by region)
 * 802.11n max:            600 Mbps
 * 802.11ac max:           6.9 Gbps
 * 802.11ax max:           9.6 Gbps
 * WPA2 encryption:        AES-128 (CCMP)
 * Typical roam time:      50-100 ms (without 11r)
 * Fast roam time (11r):   <10 ms
 *
 * ============================================================================
 *                    ACRONYM CHEAT SHEET
 * ============================================================================
 *
 * AP      - Access Point
 * STA     - Station
 * BSS     - Basic Service Set
 * SSID    - Service Set Identifier
 * BSSID   - Basic Service Set Identifier
 * RSSI    - Received Signal Strength Indicator
 * MIMO    - Multiple Input Multiple Output
 * OFDM    - Orthogonal Frequency Division Multiplexing
 * OFDMA   - Orthogonal Frequency Division Multiple Access
 * WPA     - Wi-Fi Protected Access
 * PSK     - Pre-Shared Key
 * SAE     - Simultaneous Authentication of Equals
 * PTK     - Pairwise Transient Key
 * GTK     - Group Temporal Key
 * CCMP    - Counter Mode with CBC-MAC Protocol
 * TKIP    - Temporal Key Integrity Protocol
 * EAP     - Extensible Authentication Protocol
 * RADIUS  - Remote Authentication Dial-In User Service
 * WMM     - Wi-Fi Multimedia
 * EDCA    - Enhanced Distributed Channel Access
 * TWT     - Target Wake Time
 * DTIM    - Delivery Traffic Indication Message
 * RTS     - Request to Send
 * CTS     - Clear to Send
 * ACK     - Acknowledgment
 * GO      - Group Owner
 * NAN     - Neighbor Awareness Networking
 * AFH     - Adaptive Frequency Hopping
 * HT      - High Throughput (802.11n)
 * VHT     - Very High Throughput (802.11ac)
 * HE      - High Efficiency (802.11ax)
 * MCS     - Modulation and Coding Scheme
 *
 * ============================================================================
 */

// No code needed - this is a reference document
// Print it out or keep it handy for quick review before interviews

int main() {
    printf("Wi-Fi Quick Reference - Review the comments above!\n");
    printf("\nKey things to memorize:\n");
    printf("  - 802.11 standards (a/b/g/n/ac/ax)\n");
    printf("  - Connection flow: Scan -> Auth -> Assoc -> 4-way -> DHCP\n");
    printf("  - Security: WEP (broken) < WPA (old) < WPA2 (current) < WPA3 (best)\n");
    printf("  - Good RSSI: -50 dBm, Weak: -70 dBm\n");
    printf("  - 4-way handshake derives PTK (unicast) and GTK (broadcast)\n");
    printf("  - Linux stack: wpa_supplicant -> nl80211 -> cfg80211 -> mac80211 -> driver\n");
    return 0;
}
