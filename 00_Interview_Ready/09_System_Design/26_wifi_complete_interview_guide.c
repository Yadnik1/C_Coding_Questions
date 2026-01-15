/*
 * ============================================================================
 *        COMPLETE Wi-Fi INTERVIEW GUIDE - HP IQ WIRELESS SOFTWARE ENGINEER
 * ============================================================================
 *
 *                    ALL Q&A FORMAT - EASY TO RECALL
 *
 * ============================================================================
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 1: BASICS & DEFINITIONS                       █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What is Wi-Fi?
 * A: Wi-Fi is wireless LAN technology based on IEEE 802.11 standards. It
 *    allows devices to connect to a network without physical cables, using
 *    radio waves in the 2.4 GHz, 5 GHz, or 6 GHz frequency bands.
 *
 * Q: What frequency bands does Wi-Fi use?
 * A: 2.4 GHz and 5 GHz are standard. Wi-Fi 6E also adds 6 GHz for more
 *    spectrum and less congestion.
 *
 * Q: What is the difference between 2.4 GHz and 5 GHz?
 * A: 2.4 GHz has better range and wall penetration but more interference
 *    and only 3 non-overlapping channels. 5 GHz has more channels, less
 *    interference, higher speeds, but shorter range.
 *
 * Q: What is an SSID?
 * A: Service Set Identifier - the network name you see when scanning for
 *    Wi-Fi networks. It's a human-readable string up to 32 characters.
 *
 * Q: What is a BSSID?
 * A: Basic Service Set Identifier - the MAC address of the access point.
 *    It uniquely identifies a specific AP on the network.
 *
 * Q: What is an AP?
 * A: Access Point - the device that creates the Wi-Fi network and bridges
 *    wireless clients to the wired network.
 *
 * Q: What is a STA?
 * A: Station - any client device that connects to an AP, like a laptop,
 *    phone, or IoT device.
 *
 * Q: What is a BSS?
 * A: Basic Service Set - one AP and all its associated clients form a BSS.
 *
 * Q: What is an ESS?
 * A: Extended Service Set - multiple APs with the same SSID, allowing
 *    roaming between them.
 *
 * Q: What is RSSI?
 * A: Received Signal Strength Indicator - measures signal strength in dBm.
 *    It's a negative number; closer to zero is stronger.
 *
 * Q: What is a good RSSI value?
 * A: -30 to -50 dBm is excellent, -60 dBm is fair, -70 dBm is weak, and
 *    below -80 dBm is usually unusable.
 *
 * Q: What is a beacon frame?
 * A: A management frame sent by the AP periodically (typically every 100ms)
 *    to announce its presence and broadcast network information.
 *
 * Q: What is the beacon interval?
 * A: Time between beacons, typically 100 TU (about 102.4 ms).
 *    1 TU (Time Unit) = 1024 microseconds.
 *
 * Q: What is a hidden network?
 * A: A network where the AP doesn't broadcast its SSID in beacons. Clients
 *    must know the SSID and explicitly request to connect.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 2: 802.11 STANDARDS                           █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What is 802.11?
 * A: The IEEE standard family for wireless LANs. Different amendments add
 *    features and improvements (a, b, g, n, ac, ax).
 *
 * Q: What is 802.11a?
 * A: 5 GHz band, up to 54 Mbps, uses OFDM modulation. Released 1999.
 *    Less interference than 2.4 GHz but shorter range.
 *
 * Q: What is 802.11b?
 * A: 2.4 GHz band, up to 11 Mbps, uses DSSS modulation. Released 1999.
 *    The oldest standard still sometimes referenced, mostly legacy now.
 *
 * Q: What is 802.11g?
 * A: 2.4 GHz band, up to 54 Mbps, uses OFDM. Released 2003.
 *    Backward compatible with 802.11b.
 *
 * Q: What is 802.11n (Wi-Fi 4)?
 * A: Both 2.4 and 5 GHz, up to 600 Mbps. Released 2009.
 *    Introduced MIMO (multiple antennas) and 40 MHz channel bonding.
 *    Also called HT (High Throughput).
 *
 * Q: What is 802.11ac (Wi-Fi 5)?
 * A: 5 GHz only, up to 6.9 Gbps. Released 2013.
 *    Introduced 80/160 MHz channels, MU-MIMO, 256-QAM.
 *    Also called VHT (Very High Throughput).
 *
 * Q: What is 802.11ax (Wi-Fi 6)?
 * A: Both 2.4 and 5 GHz, up to 9.6 Gbps. Released 2019.
 *    Introduced OFDMA, BSS coloring, TWT (Target Wake Time).
 *    Also called HE (High Efficiency). Optimized for dense deployments.
 *
 * Q: What is Wi-Fi 6E?
 * A: Wi-Fi 6 extended to the 6 GHz band. More spectrum means less
 *    congestion and more available channels.
 *
 * Q: What does HT stand for?
 * A: High Throughput - refers to 802.11n features.
 *
 * Q: What does VHT stand for?
 * A: Very High Throughput - refers to 802.11ac features.
 *
 * Q: What does HE stand for?
 * A: High Efficiency - refers to 802.11ax features.
 *
 * Q: What is OFDM?
 * A: Orthogonal Frequency Division Multiplexing - modulation technique
 *    that splits signal across many subcarriers for efficiency.
 *
 * Q: What is OFDMA?
 * A: Orthogonal Frequency Division Multiple Access - Wi-Fi 6 feature that
 *    allows AP to talk to multiple clients simultaneously by assigning
 *    different subcarriers to different clients.
 *
 * Q: What is MCS?
 * A: Modulation and Coding Scheme - a single index that specifies the
 *    modulation type and coding rate. Higher MCS = faster but needs
 *    better signal.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 3: CONNECTION PROCESS                         █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What are the steps to connect to Wi-Fi?
 * A: Scan -> Authenticate -> Associate -> 4-way handshake -> DHCP.
 *    After DHCP, you have an IP and can send data.
 *
 * Q: What is scanning?
 * A: The process of finding available networks. Can be active (sending
 *    probe requests) or passive (listening for beacons).
 *
 * Q: What is active scanning?
 * A: Client sends probe request frames and waits for probe responses
 *    from APs. Faster but uses more power.
 *
 * Q: What is passive scanning?
 * A: Client just listens for beacon frames from APs. Slower but saves
 *    power and doesn't reveal client's presence.
 *
 * Q: What is a probe request?
 * A: A management frame sent by a client to discover networks. Can be
 *    directed (specific SSID) or broadcast (any network).
 *
 * Q: What is a probe response?
 * A: AP's reply to a probe request, containing network information
 *    similar to a beacon frame.
 *
 * Q: What is authentication in 802.11?
 * A: The first step of joining a network. In modern WPA2/WPA3, this is
 *    typically "Open System" authentication - just a formality. The real
 *    security happens in the 4-way handshake.
 *
 * Q: What is association?
 * A: The second step where client formally joins the BSS. Client and AP
 *    exchange capability information (supported rates, features).
 *
 * Q: What is AID?
 * A: Association ID - a number (1-2007) assigned by the AP to identify
 *    the client within the BSS.
 *
 * Q: What happens after association?
 * A: The 4-way handshake occurs to establish encryption keys, then DHCP
 *    to get an IP address. Only then is the connection fully usable.
 *
 * Q: What is disassociation?
 * A: Client gracefully leaves the network. Sends a disassociation frame
 *    to inform the AP.
 *
 * Q: What is deauthentication?
 * A: Forced disconnection. Can be initiated by AP or client. More abrupt
 *    than disassociation.
 *
 * Q: What are reason codes?
 * A: Numeric codes in deauth/disassoc frames indicating why the
 *    disconnection happened. Example: reason 1 = unspecified,
 *    reason 7 = class 3 frame from non-associated STA.
 *
 *
 * CONNECTION FLOW DIAGRAM:
 *
 *     CLIENT                                              AP
 *        |                                                 |
 *        |  1. Probe Request (or listen for Beacon)        |
 *        |------------------------------------------------>|
 *        |                                                 |
 *        |  2. Probe Response                              |
 *        |<------------------------------------------------|
 *        |                                                 |
 *        |  3. Authentication Request                      |
 *        |------------------------------------------------>|
 *        |                                                 |
 *        |  4. Authentication Response                     |
 *        |<------------------------------------------------|
 *        |                                                 |
 *        |  5. Association Request                         |
 *        |------------------------------------------------>|
 *        |                                                 |
 *        |  6. Association Response (includes AID)         |
 *        |<------------------------------------------------|
 *        |                                                 |
 *        |  7. 4-Way Handshake (EAPOL frames)              |
 *        |<================================================>|
 *        |                                                 |
 *        |  8. DHCP (Discover/Offer/Request/Ack)           |
 *        |<================================================>|
 *        |                                                 |
 *        |  === CONNECTED - Can send/receive data ===      |
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 4: SECURITY                                   █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What is WEP?
 * A: Wired Equivalent Privacy - the original Wi-Fi security. Completely
 *    broken and crackable in minutes. Never use it.
 *
 * Q: What is WPA?
 * A: Wi-Fi Protected Access - replaced WEP. Used TKIP encryption.
 *    Now deprecated, considered weak.
 *
 * Q: What is WPA2?
 * A: Current widely-used standard. Uses AES-CCMP encryption and the
 *    4-way handshake for key establishment. Secure when properly
 *    configured with strong password.
 *
 * Q: What is WPA3?
 * A: Latest standard (2018). Uses SAE (Simultaneous Authentication of
 *    Equals) instead of PSK. Resistant to offline dictionary attacks
 *    and provides forward secrecy.
 *
 * Q: What is PSK?
 * A: Pre-Shared Key - a password shared by all users of the network.
 *    Used in WPA2-Personal (home networks).
 *
 * Q: What is the difference between WPA2-Personal and WPA2-Enterprise?
 * A: Personal uses a shared password (PSK) for everyone. Enterprise uses
 *    individual credentials via 802.1X and a RADIUS server.
 *
 * Q: What is the 4-way handshake?
 * A: A key exchange protocol after association. Both sides prove they
 *    know the password and derive fresh encryption keys (PTK, GTK).
 *    Uses EAPOL frames.
 *
 * Q: Walk me through the 4-way handshake.
 * A: Message 1: AP sends ANonce (AP's random number)
 *    Message 2: Client sends SNonce + MIC (proves it knows password)
 *    Message 3: AP sends GTK encrypted with PTK + MIC
 *    Message 4: Client confirms key installation
 *    After this, both sides have matching PTK and GTK.
 *
 * Q: What is PTK?
 * A: Pairwise Transient Key - encrypts unicast traffic between one
 *    client and the AP. Unique per client.
 *
 * Q: What is GTK?
 * A: Group Temporal Key - encrypts broadcast and multicast traffic.
 *    Shared by all clients on the network.
 *
 * Q: What is ANonce and SNonce?
 * A: Random numbers used in key derivation. ANonce from AP, SNonce from
 *    Station. Combined with password and MACs to derive PTK.
 *
 * Q: What is MIC?
 * A: Message Integrity Code - proves the sender knows the key and the
 *    message hasn't been tampered with.
 *
 * Q: What is SAE?
 * A: Simultaneous Authentication of Equals - WPA3's authentication
 *    method. Uses Dragonfly key exchange. Provides protection against
 *    offline dictionary attacks.
 *
 * Q: Why is WPA3 more secure than WPA2?
 * A: In WPA2, if you capture the 4-way handshake, you can try passwords
 *    offline. WPA3's SAE doesn't leak enough information for offline
 *    attacks, and it provides forward secrecy.
 *
 * Q: What is forward secrecy?
 * A: Even if the password is later compromised, past captured sessions
 *    cannot be decrypted because each session uses unique keys.
 *
 * Q: What is CCMP?
 * A: Counter Mode with CBC-MAC Protocol - the encryption algorithm used
 *    in WPA2. Based on AES-128.
 *
 * Q: What is TKIP?
 * A: Temporal Key Integrity Protocol - older encryption used in WPA1.
 *    Weaker than CCMP, now deprecated.
 *
 * Q: What is 802.1X?
 * A: Port-based network access control. Used in enterprise Wi-Fi for
 *    individual authentication via RADIUS server.
 *
 * Q: What is RADIUS?
 * A: Remote Authentication Dial-In User Service - server that handles
 *    enterprise authentication. Validates user credentials.
 *
 * Q: What is EAP?
 * A: Extensible Authentication Protocol - framework for various
 *    authentication methods in 802.1X.
 *
 * Q: Name some EAP methods.
 * A: PEAP (Protected EAP), EAP-TLS (certificate-based), EAP-TTLS,
 *    EAP-FAST. PEAP and EAP-TLS are most common in enterprise.
 *
 * Q: What is PMF?
 * A: Protected Management Frames (802.11w) - encrypts management frames
 *    to prevent deauth attacks. Mandatory in WPA3.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 5: FRAME TYPES                                █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What are the three main 802.11 frame types?
 * A: Management frames, Control frames, and Data frames.
 *
 * Q: What are management frames?
 * A: Frames for network management - beacons, probe req/resp,
 *    authentication, association, deauthentication, disassociation.
 *    Not encrypted (unless PMF enabled).
 *
 * Q: What are control frames?
 * A: Short frames for channel access and acknowledgment - ACK, RTS,
 *    CTS, Block ACK. Very time-critical.
 *
 * Q: What are data frames?
 * A: Frames carrying actual user payload. Encrypted when security
 *    is enabled.
 *
 * Q: What is an ACK frame?
 * A: Acknowledgment - sent after successfully receiving a frame.
 *    Required for reliable delivery.
 *
 * Q: What is RTS/CTS?
 * A: Request to Send / Clear to Send - optional mechanism to reserve
 *    the channel before sending. Helps with hidden node problem.
 *
 * Q: What is the hidden node problem?
 * A: When two clients can both reach the AP but can't hear each other,
 *    they may transmit simultaneously and cause collisions at the AP.
 *    RTS/CTS helps solve this.
 *
 * Q: What is Block ACK?
 * A: A single ACK for multiple frames (used with aggregation). More
 *    efficient than individual ACKs.
 *
 * Q: What is an EAPOL frame?
 * A: Extensible Authentication Protocol over LAN - used for the 4-way
 *    handshake key exchange. Technically a data frame with special type.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 6: MIMO & CHANNELS                            █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What is MIMO?
 * A: Multiple Input Multiple Output - using multiple antennas at both
 *    transmitter and receiver to increase throughput and reliability.
 *
 * Q: What does 2x2 MIMO mean?
 * A: 2 transmit antennas and 2 receive antennas.
 *
 * Q: What are spatial streams?
 * A: Independent data streams sent simultaneously over MIMO antennas.
 *    More streams = higher throughput. 2x2 MIMO supports 2 streams.
 *
 * Q: What is MU-MIMO?
 * A: Multi-User MIMO - AP can transmit to multiple clients simultaneously
 *    using different spatial streams. Introduced in 802.11ac.
 *
 * Q: What is beamforming?
 * A: Focusing the signal toward a specific client by adjusting antenna
 *    phases. Improves range and throughput to that client.
 *
 * Q: What is channel bonding?
 * A: Combining adjacent channels for more bandwidth. Two 20 MHz channels
 *    become one 40 MHz channel = higher speeds.
 *
 * Q: What channel widths are available?
 * A: 20 MHz (standard), 40 MHz (802.11n+), 80 MHz (802.11ac+),
 *    160 MHz (802.11ac/ax).
 *
 * Q: How many non-overlapping channels in 2.4 GHz?
 * A: Only 3 (channels 1, 6, 11 in US). This is why 2.4 GHz is crowded.
 *
 * Q: Why does 5 GHz have less interference?
 * A: Many more non-overlapping channels available, and fewer legacy
 *    devices and household appliances use 5 GHz.
 *
 * Q: What is DFS?
 * A: Dynamic Frequency Selection - required on some 5 GHz channels
 *    to avoid radar. Device must detect radar and switch channels.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 7: POWER SAVE                                 █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What is power save mode?
 * A: Client tells AP it will sleep between beacons. AP buffers frames
 *    for sleeping clients and indicates pending data in beacons.
 *
 * Q: How does a client know if it has buffered data?
 * A: The TIM (Traffic Indication Map) in the beacon indicates which
 *    AIDs have pending data.
 *
 * Q: What is DTIM?
 * A: Delivery Traffic Indication Message - special beacon that indicates
 *    buffered broadcast/multicast. All clients must wake for DTIM.
 *
 * Q: What is DTIM interval?
 * A: Number of beacons between DTIMs. DTIM=3 means every 3rd beacon
 *    is a DTIM beacon. Higher = more power savings but more latency.
 *
 * Q: What is PS-Poll?
 * A: Power Save Poll - frame sent by client to retrieve one buffered
 *    frame from AP after seeing TIM indication.
 *
 * Q: What is U-APSD?
 * A: Unscheduled Automatic Power Save Delivery - more efficient than
 *    PS-Poll. Client sends trigger frame, AP sends all buffered data.
 *
 * Q: What is TWT?
 * A: Target Wake Time - Wi-Fi 6 feature where client and AP agree on
 *    specific wake times. Very power efficient, great for IoT.
 *
 * Q: What is listen interval?
 * A: How often the client wakes to check beacons, agreed during
 *    association. Longer = more power savings but more latency.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 8: QoS (Quality of Service)                   █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What is QoS in Wi-Fi?
 * A: Prioritizing certain traffic types over others. Voice and video
 *    get priority over background downloads.
 *
 * Q: What is WMM?
 * A: Wi-Fi Multimedia - the certification for 802.11e QoS. Most modern
 *    devices support WMM.
 *
 * Q: What are the 4 access categories?
 * A: Voice (VO) - highest priority
 *    Video (VI)
 *    Best Effort (BE) - default
 *    Background (BK) - lowest priority
 *
 * Q: How does QoS prioritization work?
 * A: Each access category has different contention parameters. Higher
 *    priority traffic waits less before transmitting.
 *
 * Q: What is EDCA?
 * A: Enhanced Distributed Channel Access - the mechanism that implements
 *    QoS by varying contention window and wait times per AC.
 *
 * Q: What is TXOP?
 * A: Transmission Opportunity - after winning channel access, how long
 *    a station can transmit. Higher priority gets longer TXOP.
 *
 * Q: What is DSCP?
 * A: Differentiated Services Code Point - field in IP header that
 *    indicates traffic priority. Maps to Wi-Fi access categories.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 9: AGGREGATION                                █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What is frame aggregation?
 * A: Combining multiple frames into one transmission to reduce overhead.
 *    Introduced in 802.11n.
 *
 * Q: Why is aggregation important?
 * A: Wi-Fi has high per-frame overhead (preamble, IFS, ACK). Sending
 *    many small frames is inefficient. Aggregation amortizes overhead.
 *
 * Q: What is A-MPDU?
 * A: Aggregated MAC Protocol Data Unit - multiple MAC frames in one
 *    PHY transmission. Each subframe can be individually acknowledged
 *    via Block ACK. Most common aggregation type.
 *
 * Q: What is A-MSDU?
 * A: Aggregated MAC Service Data Unit - multiple data payloads in one
 *    MAC frame. Lower overhead than A-MPDU but all-or-nothing
 *    retransmission if frame fails.
 *
 * Q: Which is more commonly used, A-MPDU or A-MSDU?
 * A: A-MPDU is more common because it allows selective retransmission
 *    of failed subframes.
 *
 * Q: What is Block ACK?
 * A: A bitmap acknowledging multiple frames at once. Used with A-MPDU
 *    to indicate which subframes succeeded/failed.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 10: WI-FI DIRECT (P2P)                        █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What is Wi-Fi Direct?
 * A: Peer-to-peer Wi-Fi that lets devices connect directly without an
 *    infrastructure AP. One device becomes Group Owner (soft AP).
 *
 * Q: What is a Group Owner (GO)?
 * A: The device that acts as the access point in a Wi-Fi Direct group.
 *    Runs a soft AP and provides DHCP to clients.
 *
 * Q: What is GO Intent?
 * A: A value from 0-15 indicating how much a device wants to be GO.
 *    Higher intent = more likely to become GO. 15 = must be GO.
 *
 * Q: How is GO decided?
 * A: Through GO Negotiation. Devices exchange intent values. Higher
 *    intent wins. If equal, a tie-breaker bit decides.
 *
 * Q: Walk me through Wi-Fi Direct connection.
 * A: 1. Device Discovery (P2P probe requests)
 *    2. GO Negotiation (decide who's GO)
 *    3. Group Formation (GO starts soft AP)
 *    4. WPS Provisioning (exchange credentials)
 *    5. 4-way handshake (same as regular WPA2)
 *    6. DHCP (GO assigns IP to client)
 *
 * Q: What is a persistent group?
 * A: A saved Wi-Fi Direct group that can reconnect quickly without
 *    full negotiation. Credentials are stored.
 *
 * Q: Can a device be in P2P and infrastructure mode simultaneously?
 * A: Yes, with concurrent operation. Device has two interfaces - one
 *    connected to AP, one in P2P group. Requires driver support.
 *
 * Q: What are use cases for Wi-Fi Direct?
 * A: Printing (HP printers!), screen mirroring (Miracast), file sharing,
 *    gaming, camera-to-phone transfer.
 *
 * Q: What is WPS?
 * A: Wi-Fi Protected Setup - simplified credential exchange. Methods
 *    include Push Button (PBC) and PIN entry.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 11: WI-FI AWARE (NAN)                         █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What is Wi-Fi Aware?
 * A: Also called NAN (Neighbor Awareness Networking). A low-power
 *    discovery protocol to find nearby devices and services without
 *    being connected to any network.
 *
 * Q: How is Wi-Fi Aware different from Wi-Fi Direct?
 * A: Aware is primarily for discovery - finding nearby services.
 *    Direct is for data transfer - establishing connections.
 *    Aware is much more power efficient.
 *
 * Q: How does Wi-Fi Aware work?
 * A: Devices form clusters with synchronized wake times. They wake
 *    together briefly to exchange discovery messages, then sleep.
 *
 * Q: What is Publish/Subscribe in Wi-Fi Aware?
 * A: Publisher announces a service ("I offer printing").
 *    Subscriber looks for services ("Looking for printers").
 *    When matched, they can exchange info or establish connection.
 *
 * Q: What happens after discovery in Wi-Fi Aware?
 * A: Can escalate to NAN Data Path (NDP) for data transfer, or
 *    transition to Wi-Fi Direct or infrastructure.
 *
 * Q: What are use cases for Wi-Fi Aware?
 * A: Finding nearby printers, proximity-based social apps, IoT device
 *    discovery, location services.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 12: ENTERPRISE WI-FI                          █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What is enterprise Wi-Fi architecture?
 * A: Managed Wi-Fi deployments with multiple APs, central control,
 *    individual user authentication (802.1X), and advanced features
 *    like roaming optimization.
 *
 * Q: What is controller-based architecture?
 * A: "Thin" APs that forward traffic to a central controller. Controller
 *    makes all decisions - channel selection, roaming, security.
 *    Examples: Cisco WLC, Aruba controllers.
 *
 * Q: What is cloud-managed architecture?
 * A: "Fat" APs with local intelligence, managed via cloud dashboard.
 *    Cloud handles config, monitoring, updates. APs work even if
 *    cloud unreachable. Examples: Meraki, Mist.
 *
 * Q: What is roaming?
 * A: Moving from one AP to another while maintaining connectivity.
 *    Critical for VoIP and video - can't have long interruptions.
 *
 * Q: What is 802.11r?
 * A: Fast BSS Transition - allows pre-authentication with target AP
 *    before roaming. Reduces roam time from ~100ms to ~10ms.
 *
 * Q: What is 802.11k?
 * A: Radio Resource Management - AP provides neighbor report to client.
 *    Client knows nearby APs without scanning all channels.
 *
 * Q: What is 802.11v?
 * A: BSS Transition Management - AP can suggest client roam to another
 *    AP (load balancing). Client can request preferred APs.
 *
 * Q: How do 11r, 11k, and 11v work together?
 * A: 11k helps client find roam targets quickly, 11r makes the actual
 *    roam fast, 11v lets AP guide roaming decisions. Together they
 *    enable seamless enterprise roaming.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 13: BLUETOOTH COEXISTENCE                     █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: Why do Wi-Fi and Bluetooth interfere?
 * A: Both operate in the 2.4 GHz ISM band. Without coordination,
 *    simultaneous transmissions cause collisions.
 *
 * Q: What problems does interference cause?
 * A: Reduced throughput, increased latency, packet loss, audio glitches
 *    in Bluetooth headphones, Wi-Fi disconnections.
 *
 * Q: How is coexistence handled?
 * A: Three main mechanisms:
 *    1. Time-division - arbiter decides who transmits when
 *    2. Frequency avoidance - BT avoids Wi-Fi channels (AFH)
 *    3. Hardware signaling - GPIO lines for real-time arbitration
 *
 * Q: What is AFH?
 * A: Adaptive Frequency Hopping - Bluetooth avoids channels where Wi-Fi
 *    is operating. Wi-Fi tells BT which channels to skip.
 *
 * Q: How does time-division coexistence work?
 * A: An arbiter gives time slots to each radio based on priority.
 *    BT audio gets guaranteed slots, Wi-Fi gets the rest.
 *
 * Q: What traffic gets higher priority?
 * A: BT voice calls (SCO) and HID (keyboard/mouse) get highest priority.
 *    Wi-Fi VoIP is high. BT audio streaming (A2DP) and Wi-Fi bulk
 *    data are lower.
 *
 * Q: How would you debug coexistence issues?
 * A: Check if both radios are active. Look at coex statistics (grants
 *    vs denies). Try moving Wi-Fi to 5 GHz. Check BT AFH channel map.
 *    Review priority configuration.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 14: LINUX WIRELESS STACK                      █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: Describe the Linux wireless stack.
 * A: User space: wpa_supplicant (authentication), hostapd (AP mode)
 *    Kernel: nl80211 (interface) -> cfg80211 (config) -> mac80211 (MAC)
 *    Driver: Hardware-specific, talks to mac80211
 *    Firmware: Runs on the Wi-Fi chip itself
 *
 * Q: What is wpa_supplicant?
 * A: User-space daemon that handles Wi-Fi authentication, association,
 *    and key management. Talks to kernel via nl80211.
 *
 * Q: What is hostapd?
 * A: User-space daemon for running AP mode. Handles AP-side
 *    authentication and management.
 *
 * Q: What is nl80211?
 * A: Netlink-based interface between user space and cfg80211.
 *    Applications use it to configure wireless.
 *
 * Q: What is cfg80211?
 * A: Kernel configuration layer for wireless. Handles regulatory
 *    domain, scan management, BSS tracking.
 *
 * Q: What is mac80211?
 * A: Kernel software MAC implementation. Handles frame TX/RX,
 *    rate control, power save state machines, aggregation.
 *
 * Q: What does a wireless driver do?
 * A: Interfaces between mac80211 and hardware. Handles register
 *    programming, interrupts, DMA, firmware communication.
 *
 * Q: What is monitor mode?
 * A: Capturing all Wi-Fi frames on a channel without associating.
 *    Used for packet capture and debugging.
 *
 * Q: Where would your code typically live as a wireless engineer?
 * A: Mostly in the driver layer - hardware abstraction, register
 *    access, DMA management, interrupt handling. Sometimes in mac80211
 *    for feature additions or bug fixes.
 *
 *
 * LINUX WIRELESS STACK DIAGRAM:
 *
 *   +------------------------------------------------------------------+
 *   |  USER SPACE                                                      |
 *   |    wpa_supplicant (STA)    hostapd (AP)    NetworkManager        |
 *   |          |                     |                                 |
 *   |          +----------+----------+                                 |
 *   |                     |                                            |
 *   |                     v                                            |
 *   |               [ nl80211 interface ]                              |
 *   +------------------------------------------------------------------+
 *   |  KERNEL                                                          |
 *   |    cfg80211 - Configuration, regulatory, scan management         |
 *   |        |                                                         |
 *   |        v                                                         |
 *   |    mac80211 - Software MAC, rate control, power save, aggregation|
 *   |        |                                                         |
 *   |        v                                                         |
 *   |    DRIVER - Hardware abstraction, register access, DMA, IRQ      |
 *   +------------------------------------------------------------------+
 *   |  HARDWARE                                                        |
 *   |    Wi-Fi Chipset + Firmware                                      |
 *   +------------------------------------------------------------------+
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 15: CROSS-PLATFORM                            █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What platforms does HP IQ work across?
 * A: Android, Windows, Linux, and embedded systems.
 *
 * Q: Describe the Android wireless stack.
 * A: Apps use WifiManager/WifiP2pManager Java APIs
 *    Framework: WifiService, WifiP2pService
 *    Native: wpa_supplicant, wificond
 *    HAL: Hardware Abstraction Layer
 *    Kernel driver
 *
 * Q: What Android APIs are used for Wi-Fi?
 * A: WifiManager - infrastructure connections
 *    WifiP2pManager - Wi-Fi Direct
 *    WifiAwareManager - Wi-Fi Aware/NAN
 *    ConnectivityManager - network selection
 *
 * Q: Describe the Windows wireless stack.
 * A: Apps use WLAN API (WlanOpenHandle, WlanConnect, etc.)
 *    WLAN Service (wlansvc) handles auto-configuration
 *    NDIS provides driver interface
 *    Miniport driver for specific hardware
 *
 * Q: How would you design a cross-platform wireless SDK?
 * A: Common API layer on top (Connect, Scan, GetStatus)
 *    Platform adapters underneath (Android/JNI, Windows/Win32, Linux/nl80211)
 *    Async callbacks for operations
 *    Feature detection for platform differences
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 16: DEBUGGING & TOOLS                         █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: What tools do you use to debug Wi-Fi issues?
 * A: iw (Linux config), wpa_cli (supplicant control), Wireshark (packet
 *    capture), dmesg (kernel logs), vendor-specific tools.
 *
 * Q: How do you scan for networks on Linux?
 * A: iw dev wlan0 scan
 *
 * Q: How do you check connection status on Linux?
 * A: iw dev wlan0 link
 *
 * Q: How do you see station statistics?
 * A: iw dev wlan0 station dump - shows RSSI, TX/RX rates, packet counts.
 *
 * Q: How do you check wpa_supplicant status?
 * A: wpa_cli status
 *
 * Q: How do you capture Wi-Fi packets?
 * A: Put adapter in monitor mode, then use Wireshark or tcpdump.
 *    On Linux: airmon-ng start wlan0, then capture.
 *
 * Q: What Wireshark filters are useful for Wi-Fi?
 * A: wlan.fc.type == 0 (management frames)
 *    wlan.fc.type_subtype == 8 (beacons)
 *    eapol (4-way handshake)
 *    wlan.addr == XX:XX:XX:XX:XX:XX (specific device)
 *
 * Q: How do you debug Wi-Fi on Android?
 * A: adb shell dumpsys wifi - service state
 *    adb logcat -s WifiService - logs
 *    adb shell wpa_cli status - supplicant state
 *
 * Q: What is a spectrum analyzer used for?
 * A: Viewing RF environment, finding interference sources (microwaves,
 *    cameras, other devices). Shows channel utilization and noise floor.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 17: TROUBLESHOOTING                           █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: Device won't connect - how do you debug?
 * A: 1. Check if AP visible in scan (iw dev wlan0 scan | grep SSID)
 *    2. Check auth phase in dmesg (auth failed? timeout?)
 *    3. Check association (denied? what reason code?)
 *    4. Check 4-way handshake (wrong password = handshake fails)
 *    5. Check wpa_supplicant logs for details
 *
 * Q: Connected but no internet - how do you debug?
 * A: 1. Verify you got an IP (ip addr show wlan0)
 *    2. Check gateway (ip route)
 *    3. Ping gateway (local connectivity)
 *    4. Check DNS (can you resolve hostnames?)
 *    5. Check firewall rules
 *
 * Q: Slow speeds - how do you debug?
 * A: 1. Check RSSI (iw station dump) - weak signal = slow
 *    2. Check negotiated rate (tx bitrate in station dump)
 *    3. Check channel congestion (iw survey dump)
 *    4. Verify aggregation is working
 *    5. Check for interference
 *
 * Q: Intermittent disconnections - how do you debug?
 * A: 1. Check signal strength over time
 *    2. Look for deauth/disassoc in logs (iw event or dmesg)
 *    3. Note reason codes
 *    4. Check power save behavior (disable and test)
 *    5. Check for firmware crashes
 *
 * Q: 4-way handshake fails - what's likely wrong?
 * A: Wrong password is the most common cause. Could also be:
 *    timeout issues, driver bugs, incompatible security settings.
 *
 * Q: What does "beacon loss" mean?
 * A: Client didn't receive expected beacons from AP. Usually means
 *    signal dropped or AP went away. Client will disconnect.
 *
 * Q: What is reason code 7?
 * A: "Class 3 frame received from non-associated STA" - very common.
 *    Usually means the AP thinks client isn't associated anymore.
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 18: BEHAVIORAL QUESTIONS                      █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Q: Tell me about yourself.
 * A: "I'm an embedded systems engineer with strong C/C++ background and
 *    experience with real-time systems, drivers, and low-level debugging.
 *    I'm now focusing on wireless connectivity - specifically Wi-Fi software
 *    stacks - because I find the challenges of wireless protocol development,
 *    cross-platform support, and performance optimization fascinating."
 *
 * Q: Why HP IQ?
 * A: "HP IQ combines startup agility with HP's global scale. I'm excited
 *    about working on wireless at the intersection of Wi-Fi, Bluetooth, and
 *    peer-to-peer technologies. The chance to work across Android, Windows,
 *    and embedded platforms on cutting-edge AI devices is exactly the
 *    challenge I'm looking for."
 *
 * Q: Why this role?
 * A: "This role aligns perfectly with my embedded systems background and
 *    my growing expertise in wireless. I have strong fundamentals in driver
 *    development, interrupt handling, DMA, and debugging - skills directly
 *    applicable to Wi-Fi software. I'm excited about P2P technologies and
 *    cross-platform development."
 *
 * Q: What's your Wi-Fi experience?
 * A: Be honest: "I've been studying the Wi-Fi stack extensively - the Linux
 *    wireless architecture, 802.11 standards, association flow, security
 *    protocols, and P2P technologies. My embedded background gives me strong
 *    foundations that transfer directly. I learn quickly and I'm committed
 *    to becoming an expert in this space."
 *
 * Q: Describe a challenging debugging problem you solved.
 * A: [Use your own experience, but structure it as:]
 *    - What was the problem
 *    - What made it hard
 *    - How you approached it systematically
 *    - What tools/techniques you used
 *    - What you learned
 *
 * Q: How do you handle working across teams?
 * A: "I believe in clear communication and shared context. When working
 *    with chipset vendors or other teams, I make sure to document findings
 *    clearly, provide reproducible test cases, and escalate blockers
 *    proactively. I've found that building relationships and understanding
 *    each team's constraints leads to better collaboration."
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 19: QUESTIONS TO ASK THEM                     █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * TECHNICAL:
 * - "What does the Wi-Fi stack look like at HP IQ - driver level,
 *    framework level, or both?"
 * - "Which platforms are the main focus right now?"
 * - "How much of the work involves Wi-Fi Direct versus infrastructure?"
 *
 * TEAM:
 * - "How closely does your team work with chipset vendors?"
 * - "What does the debugging workflow look like for wireless issues?"
 * - "How is the team structured - by platform or cross-functional?"
 *
 * PRODUCT:
 * - "What devices or products is HP IQ focusing on for wireless?"
 * - "What are the biggest wireless challenges you're solving right now?"
 *
 * GROWTH:
 * - "What does success look like in the first 6 months?"
 * - "What's the most interesting wireless problem the team solved recently?"
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 20: QUICK NUMBERS TO REMEMBER                 █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * Beacon interval:        100 TU (~102.4 ms)
 * 1 TU:                   1024 microseconds
 *
 * RSSI values:
 *   Excellent:            -30 to -50 dBm
 *   Good:                 -50 to -60 dBm
 *   Fair:                 -60 to -70 dBm
 *   Weak:                 -70 to -80 dBm
 *   Unusable:             below -80 dBm
 *
 * 2.4 GHz channels (US):  1-11 (use 1, 6, 11 for non-overlap)
 * 5 GHz channels:         36-165 (varies by region)
 *
 * Max speeds:
 *   802.11n:              600 Mbps
 *   802.11ac:             6.9 Gbps
 *   802.11ax:             9.6 Gbps
 *
 * Security:
 *   WPA2 encryption:      AES-128 (CCMP)
 *   WPA3 encryption:      AES-128/256 (CCMP/GCMP)
 *
 * Roaming:
 *   Typical roam time:    50-100 ms
 *   Fast roam (11r):      <10 ms
 *
 * Channel widths:         20, 40, 80, 160 MHz
 *
 * GO Intent range:        0-15 (higher = more likely to be GO)
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    SECTION 21: ACRONYM CHEAT SHEET                       █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 *
 *
 * BASIC:
 * AP      - Access Point
 * STA     - Station (client)
 * BSS     - Basic Service Set
 * ESS     - Extended Service Set
 * SSID    - Service Set Identifier
 * BSSID   - Basic Service Set Identifier
 * RSSI    - Received Signal Strength Indicator
 * AID     - Association ID
 *
 * STANDARDS:
 * HT      - High Throughput (802.11n)
 * VHT     - Very High Throughput (802.11ac)
 * HE      - High Efficiency (802.11ax)
 * MCS     - Modulation and Coding Scheme
 *
 * MODULATION:
 * OFDM    - Orthogonal Frequency Division Multiplexing
 * OFDMA   - Orthogonal Frequency Division Multiple Access
 * DSSS    - Direct Sequence Spread Spectrum
 * QAM     - Quadrature Amplitude Modulation
 *
 * MIMO:
 * MIMO    - Multiple Input Multiple Output
 * MU-MIMO - Multi-User MIMO
 * SU-MIMO - Single-User MIMO
 *
 * SECURITY:
 * WEP     - Wired Equivalent Privacy (broken)
 * WPA     - Wi-Fi Protected Access
 * PSK     - Pre-Shared Key
 * SAE     - Simultaneous Authentication of Equals
 * PTK     - Pairwise Transient Key
 * GTK     - Group Temporal Key
 * PMK     - Pairwise Master Key
 * CCMP    - Counter Mode with CBC-MAC Protocol
 * TKIP    - Temporal Key Integrity Protocol
 * EAP     - Extensible Authentication Protocol
 * RADIUS  - Remote Authentication Dial-In User Service
 * PMF     - Protected Management Frames
 *
 * FRAMES:
 * ACK     - Acknowledgment
 * RTS     - Request to Send
 * CTS     - Clear to Send
 * EAPOL   - EAP over LAN
 *
 * QoS:
 * WMM     - Wi-Fi Multimedia
 * EDCA    - Enhanced Distributed Channel Access
 * TXOP    - Transmission Opportunity
 * DSCP    - Differentiated Services Code Point
 * AC      - Access Category
 *
 * POWER SAVE:
 * TIM     - Traffic Indication Map
 * DTIM    - Delivery Traffic Indication Message
 * TWT     - Target Wake Time
 * U-APSD  - Unscheduled Automatic Power Save Delivery
 *
 * AGGREGATION:
 * A-MPDU  - Aggregated MAC Protocol Data Unit
 * A-MSDU  - Aggregated MAC Service Data Unit
 *
 * P2P:
 * GO      - Group Owner
 * GC      - Group Client
 * NAN     - Neighbor Awareness Networking
 * NDP     - NAN Data Path
 * WPS     - Wi-Fi Protected Setup
 * PBC     - Push Button Configuration
 *
 * ENTERPRISE:
 * BSS     - Basic Service Set
 * FT      - Fast Transition (802.11r)
 * RRM     - Radio Resource Management (802.11k)
 * BTM     - BSS Transition Management (802.11v)
 * DFS     - Dynamic Frequency Selection
 *
 * COEXISTENCE:
 * AFH     - Adaptive Frequency Hopping
 * BLE     - Bluetooth Low Energy
 * SCO     - Synchronous Connection-Oriented (BT voice)
 * A2DP    - Advanced Audio Distribution Profile
 * HID     - Human Interface Device
 *
 * LINUX:
 * nl80211 - Netlink 802.11
 * cfg80211- Configuration 802.11
 * mac80211- MAC 802.11
 * HAL     - Hardware Abstraction Layer
 * NDIS    - Network Driver Interface Specification
 *
 *
 *
 *
 * ████████████████████████████████████████████████████████████████████████████
 * █                                                                          █
 * █                    END OF COMPLETE INTERVIEW GUIDE                       █
 * █                                                                          █
 * ████████████████████████████████████████████████████████████████████████████
 *
 */

#include <stdio.h>

int main() {
    printf("==========================================================\n");
    printf("   COMPLETE Wi-Fi INTERVIEW GUIDE - HP IQ\n");
    printf("==========================================================\n\n");

    printf("This file contains everything you need in Q&A format:\n\n");

    printf("  Section 1:  Basics & Definitions\n");
    printf("  Section 2:  802.11 Standards\n");
    printf("  Section 3:  Connection Process\n");
    printf("  Section 4:  Security\n");
    printf("  Section 5:  Frame Types\n");
    printf("  Section 6:  MIMO & Channels\n");
    printf("  Section 7:  Power Save\n");
    printf("  Section 8:  QoS\n");
    printf("  Section 9:  Aggregation\n");
    printf("  Section 10: Wi-Fi Direct (P2P)\n");
    printf("  Section 11: Wi-Fi Aware (NAN)\n");
    printf("  Section 12: Enterprise Wi-Fi\n");
    printf("  Section 13: Bluetooth Coexistence\n");
    printf("  Section 14: Linux Wireless Stack\n");
    printf("  Section 15: Cross-Platform\n");
    printf("  Section 16: Debugging & Tools\n");
    printf("  Section 17: Troubleshooting\n");
    printf("  Section 18: Behavioral Questions\n");
    printf("  Section 19: Questions to Ask Them\n");
    printf("  Section 20: Quick Numbers\n");
    printf("  Section 21: Acronym Cheat Sheet\n");

    printf("\n==========================================================\n");
    printf("   GOOD LUCK WITH YOUR INTERVIEW!\n");
    printf("==========================================================\n");

    return 0;
}
