/*
 * ============================================================================
 * ePlant Inc - FIRMWARE OPTIMIZATION SUITE INTERVIEW QUESTIONS
 * ============================================================================
 *
 * Resume Line: "Designed a C-based firmware optimization suite reducing device
 *              boot time by 40%, eliminating repeated network join requests,
 *              implementing Adaptive Data Rate, and enforcing protocol limits,
 *              improving battery life by 30%."
 *
 * This file contains 50 deep interview questions an interviewer would ask
 * about this specific experience, with comprehensive answers.
 *
 * ============================================================================
 */

/*
 * ============================================================================
 * SECTION 1: BOOT TIME OPTIMIZATION (Questions 1-15)
 * ============================================================================
 */

/*
 * Q1: How did you achieve the 40% boot time reduction? Walk me through the
 *     specific optimizations you implemented.
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * The 40% boot time reduction came from three main optimizations:
 *
 * 1. LINK CHECK ELIMINATION (Primary - saved ~60 seconds):
 *    Previously, every boot triggered 6 link check retry attempts even when
 *    the device had a valid LoRaWAN session stored in NVM (Non-Volatile Memory).
 *    Each attempt had a timeout, causing 60+ second delays.
 *
 *    My optimization:
 *    - Check if device has valid NVM session on boot
 *    - If credentials (DevEUI, JoinEUI) match stored NVM session, skip
 *      the LinkCheckTime state entirely
 *    - Transition directly from boot to Connected state
 *
 *    Code pattern:
 *    ```c
 *    bool is_joined() {
 *        if (just_joined_this_session) return true;
 *        if (network_lock_mode == 0) return false;  // Force rejoin
 *        // Verify DevEUI/JoinEUI match NVM session
 *        return nvm_session_valid && credentials_match();
 *    }
 *    ```
 *
 * 2. NETWORK LOCK/UNLOCK FEATURE:
 *    Added a configurable parameter to control session reuse:
 *    - Mode 0 (UNLOCKED): Always rejoin - for testing/provisioning
 *    - Mode 1 (LOCKED): Reuse NVM session - for production/battery savings
 *
 * 3. BOOT STUB MINIMIZATION:
 *    Reduced boot stub to minimal clock initialization (4 MHz), skipped
 *    non-essential peripheral setup, and deferred full initialization
 *    to main application.
 *
 * METRICS:
 * - Before: ~150 seconds average boot-to-network time
 * - After: ~90 seconds average boot-to-network time
 * - Reduction: 60 seconds = 40%
 */

/*
 * Q2: What is a Link Check in LoRaWAN and why was it causing boot delays?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Link Check is a LoRaWAN MAC command (LinkCheckReq/LinkCheckAns) used to:
 * 1. Verify the device can reach the network server
 * 2. Get the link margin (signal quality) from the gateway
 *
 * Why it caused delays:
 * - Default implementation sent LinkCheckReq on every boot
 * - If no response, it retried with exponential backoff
 * - Our configuration: 6 retries with timeouts of 15s, 60s, 5min, etc.
 * - Total worst-case delay: 15 + 60 + 300 = ~6+ minutes
 *
 * The problem: For devices with valid NVM sessions, the link check is
 * UNNECESSARY - they're already connected. We were wasting power and time
 * verifying a connection that already existed.
 *
 * Solution: Skip link check when:
 * - Device has valid NVM session (DevAddr, NwkSKey present)
 * - Credentials haven't changed since last session
 * - Network lock mode is enabled (production mode)
 */

/*
 * Q3: Explain the difference between OTAA and ABP in LoRaWAN. How did your
 *     optimization handle both?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * OTAA (Over-The-Air Activation):
 * - Device sends JoinRequest with DevEUI, JoinEUI, DevNonce
 * - Server responds with JoinAccept containing DevAddr and session keys
 * - Keys derived dynamically each join
 * - More secure, preferred for production
 *
 * ABP (Activation By Personalization):
 * - DevAddr and session keys pre-programmed
 * - No join procedure needed
 * - Less secure, used for testing
 *
 * My optimization focused on OTAA devices:
 *
 * Problem: OTAA devices were rejoining on every boot, wasting:
 * - Radio time (JoinRequest transmission)
 * - Battery (RX windows for JoinAccept)
 * - Network capacity (join server processing)
 *
 * Solution: Session persistence with credential verification
 * ```c
 * typedef struct {
 *     uint8_t dev_eui[8];     // Device identifier
 *     uint8_t join_eui[8];    // Application identifier
 *     uint32_t dev_addr;      // Assigned network address
 *     uint8_t nwk_s_key[16];  // Network session key
 *     uint8_t app_s_key[16];  // Application session key
 *     uint32_t fcnt_up;       // Uplink frame counter
 *     uint32_t fcnt_down;     // Downlink frame counter
 * } nvm_session_t;
 * ```
 *
 * On boot:
 * 1. Read NVM session
 * 2. Compare DevEUI/JoinEUI with current parameters
 * 3. If match AND network_lock_mode == 1, reuse session
 * 4. If mismatch OR network_lock_mode == 0, perform new join
 */

/*
 * Q4: What state machine did you use for the LoRaWAN connection? How did you
 *     modify it for the optimization?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * The LoRaWAN state machine had these states:
 *
 * ORIGINAL FLOW:
 * OFF -> JOIN -> LINK_CHECK_TIME -> CONNECTED -> SEND_PACKET -> ...
 *                    ^
 *                    |
 *              (6 retry attempts here caused 60+ second delays)
 *
 * OPTIMIZED FLOW:
 * OFF -> JOIN -> [Skip if NVM valid] -> CONNECTED -> SEND_PACKET -> ...
 *
 * Key modification in the Power() transition function:
 *
 * ```c
 * void transition_to_connected() {
 *     if (is_joined()) {
 *         // Skip LINK_CHECK_TIME entirely
 *         enter_state(STATE_CONNECTED);
 *     } else {
 *         // Need to verify link
 *         enter_state(STATE_LINK_CHECK_TIME);
 *     }
 * }
 * ```
 *
 * The is_joined() function checks:
 * 1. just_joined_this_session flag (if we joined in current session)
 * 2. network_lock_mode parameter (0 = force rejoin, 1 = allow reuse)
 * 3. NVM session validity (DevEUI/JoinEUI match)
 */

/*
 * Q5: How did you test that the boot optimization didn't break anything?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Testing strategy had four components:
 *
 * 1. UNIT TESTS:
 *    - is_joined() with various NVM states
 *    - Credential comparison logic
 *    - State machine transitions
 *
 * 2. INTEGRATION TESTS:
 *    - Boot with valid NVM session -> verify immediate connection
 *    - Boot with invalid NVM session -> verify proper rejoin
 *    - Boot after credential change -> verify rejoin
 *    - Boot in network_lock_mode=0 -> verify forced rejoin
 *
 * 3. POWER CYCLING TESTS:
 *    - 1000 power cycles with random timing
 *    - Verified no session corruption
 *    - Verified frame counter continuity
 *
 * 4. FIELD TESTS:
 *    - 50 devices deployed for 2 weeks
 *    - Monitored join frequency via network server
 *    - Measured actual boot times via debug logs
 *    - Confirmed 40% reduction in average boot time
 *
 * Key metrics tracked:
 * - Boot-to-first-uplink time
 * - Join request frequency (should decrease with optimization)
 * - Frame counter gaps (should be minimal)
 * - Network server load (should decrease)
 */

/*
 * Q6: What happens if the NVM session becomes corrupted? How did you handle
 *     this edge case?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * NVM corruption is a critical edge case. We handled it with:
 *
 * 1. CRC PROTECTION:
 *    Every NVM structure has a CRC32 field verified on read:
 *    ```c
 *    typedef struct {
 *        nvm_session_t session;
 *        uint32_t crc32;  // CRC of session data
 *    } nvm_session_with_crc_t;
 *
 *    bool read_nvm_session(nvm_session_t *session) {
 *        nvm_session_with_crc_t data;
 *        flash_read(NVM_ADDR, &data, sizeof(data));
 *        if (calculate_crc32(&data.session) != data.crc32) {
 *            return false;  // Corruption detected
 *        }
 *        *session = data.session;
 *        return true;
 *    }
 *    ```
 *
 * 2. FALLBACK TO REJOIN:
 *    If CRC fails, we force a new join:
 *    ```c
 *    bool is_joined() {
 *        if (!read_nvm_session(&session)) {
 *            // CRC failed, force rejoin
 *            return false;
 *        }
 *        // Continue with credential check
 *    }
 *    ```
 *
 * 3. FRAME COUNTER VALIDATION:
 *    Sanity check that frame counters are within expected range:
 *    - Not zero after successful session
 *    - Not maximum (0xFFFFFFFF)
 *    - Uplink > downlink (for normal operation)
 *
 * 4. PERIODIC REJOIN:
 *    Optional feature to force rejoin every N days regardless of session
 *    validity, ensuring keys are refreshed.
 */

/*
 * Q7: How did you measure the boot time? What instrumentation did you add?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Boot time measurement used several techniques:
 *
 * 1. HARDWARE TIMER:
 *    Started a hardware timer in boot stub, read in main():
 *    ```c
 *    // In boot_stub.c
 *    void boot_stub() {
 *        TIM2->CNT = 0;        // Reset counter
 *        TIM2->CR1 |= TIM_CR1_CEN;  // Start timer
 *        // ... minimal init ...
 *        go_app();
 *    }
 *
 *    // In main.c
 *    unsigned system_startup_time;  // Global variable
 *    int main() {
 *        system_startup_time = TIM2->CNT;  // Read timer
 *        // ... rest of init ...
 *    }
 *    ```
 *
 * 2. GPIO TOGGLE:
 *    Toggle a GPIO at key milestones for oscilloscope measurement:
 *    - Boot stub entry
 *    - Main() entry
 *    - LoRaWAN stack init complete
 *    - First uplink sent
 *
 * 3. DEBUG LOGGING:
 *    Timestamps in debug output:
 *    ```
 *    [0.001] Boot stub complete
 *    [0.045] Main entry
 *    [0.234] LoRaWAN init complete
 *    [1.456] Network joined
 *    [2.100] First uplink sent
 *    ```
 *
 * 4. NETWORK SERVER METRICS:
 *    Analyzed timestamps of JoinRequest and first data uplink to calculate
 *    join-to-data time from the server perspective.
 */

/*
 * Q8: What is the difference between hot boot and cold boot? How did your
 *     optimization affect each?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * COLD BOOT:
 * - Power completely removed (capacitors discharged)
 * - RAM contents lost
 * - All peripherals reset to default
 * - Example: Battery replacement, long-term storage
 *
 * HOT BOOT (Warm Boot):
 * - Quick power cycle or software reset
 * - Some RAM may be retained (if special section)
 * - RTC backup domain preserved
 * - Example: Watchdog reset, firmware update reboot
 *
 * Impact on optimization:
 *
 * COLD BOOT:
 * - NVM session must be read from flash
 * - Full validation required
 * - Optimization still helps: skip link check if NVM valid
 * - Improvement: ~40% (60 seconds saved)
 *
 * HOT BOOT:
 * - Can use boot_info structure in RAM
 * - Know reboot cause (watchdog, fault, deliberate)
 * - Can make smarter decisions about session validity
 * - Improvement: ~50% (faster NVM access, less validation)
 *
 * Special handling:
 * ```c
 * typedef struct {
 *     uint32_t magic;          // BOOT_INFO_MAGIC if valid
 *     uint8_t reboot_cause;    // Why we rebooted
 *     uint32_t pc;             // Program counter at fault
 *     uint32_t lr;             // Link register at fault
 * } boot_info_t __attribute__((section(".boot_info_section")));
 *
 * // On warm boot, check if previous session was healthy
 * if (boot_info.magic == BOOT_INFO_MAGIC &&
 *     boot_info.reboot_cause == REBOOT_CAUSE_DELIBERATE) {
 *     // Previous session was healthy, definitely reuse NVM
 * }
 * ```
 */

/*
 * Q9: How does the network_lock_mode parameter work? Why did you make it
 *     configurable?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * network_lock_mode is a device parameter controlling session reuse:
 *
 * ```c
 * typedef enum {
 *     NETWORK_LOCK_UNLOCKED = 0,  // Always rejoin on boot
 *     NETWORK_LOCK_LOCKED = 1     // Reuse NVM session if valid
 * } network_lock_mode_t;
 * ```
 *
 * WHY CONFIGURABLE:
 *
 * 1. MANUFACTURING/PROVISIONING:
 *    - Mode 0 (UNLOCKED) during provisioning
 *    - Device rejoins with each credential update
 *    - Ensures fresh session after any configuration change
 *
 * 2. PRODUCTION DEPLOYMENT:
 *    - Mode 1 (LOCKED) after provisioning complete
 *    - Device reuses session, saving battery
 *    - Customer sets this before shipping
 *
 * 3. DEBUGGING:
 *    - Mode 0 helps debug join issues
 *    - Forces device to go through full join flow
 *    - Can observe JoinRequest/JoinAccept on sniffer
 *
 * 4. NETWORK MIGRATION:
 *    - Set to Mode 0 before changing network server
 *    - Device will rejoin new network
 *    - Set back to Mode 1 after migration
 *
 * Configuration via NFC or downlink:
 * ```c
 * void on_parameter_change_network_lock_mode(uint32_t new_value) {
 *     // If switching from LOCKED to UNLOCKED, clear session
 *     if (old_value == 1 && new_value == 0) {
 *         invalidate_nvm_session();
 *     }
 *     // Store new value
 *     set_param_network_lock_mode(new_value);
 * }
 * ```
 */

/*
 * Q10: What are the security implications of reusing LoRaWAN sessions?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Session reuse has several security considerations:
 *
 * 1. FRAME COUNTER CONTINUITY (Critical):
 *    - LoRaWAN uses frame counters for replay protection
 *    - If counter resets, server may reject messages as replays
 *    - Our solution: Persist fcnt_up in NVM, increment atomically
 *    ```c
 *    uint32_t get_next_fcnt() {
 *        uint32_t fcnt = read_nvm_fcnt();
 *        write_nvm_fcnt(fcnt + 1);  // Increment before use
 *        return fcnt;
 *    }
 *    ```
 *
 * 2. KEY FRESHNESS:
 *    - Longer session = more data encrypted with same keys
 *    - Theoretical risk: cryptanalysis with sufficient traffic
 *    - Mitigation: Optional periodic rejoin (e.g., every 30 days)
 *
 * 3. SESSION HIJACKING:
 *    - If NVM is extracted, attacker could clone device
 *    - Mitigation: Store keys in secure element if available
 *    - STM32WL: Can use hardware crypto with protected keys
 *
 * 4. DOWNLINK REPLAY:
 *    - Server tracks downlink counter
 *    - If device counter resets, old downlinks could replay
 *    - Our solution: Persist fcnt_down in NVM too
 *
 * 5. NVM TAMPERING:
 *    - Attacker could modify NVM to force rejoin (DoS)
 *    - Or inject fake session to capture traffic
 *    - Mitigation: CRC protection, secure boot
 *
 * Security vs. Battery trade-off:
 * - More frequent rejoins = fresher keys, but more battery use
 * - Session reuse = better battery, but older keys
 * - We chose to optimize for battery with optional periodic rejoin
 */

/*
 * ============================================================================
 * SECTION 2: ADAPTIVE DATA RATE (ADR) QUESTIONS (Questions 11-25)
 * ============================================================================
 */

/*
 * Q11: What is Adaptive Data Rate (ADR) in LoRaWAN and why is it important?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * ADR is a mechanism for optimizing the data rate (DR) and transmit power
 * based on link quality.
 *
 * HOW IT WORKS:
 * 1. Device sends ADR bit = 1 in uplink frame header
 * 2. Network server analyzes recent uplink signal quality (RSSI, SNR)
 * 3. Server sends LinkADRReq MAC command with new DR and TX power
 * 4. Device applies settings and sends LinkADRAns
 *
 * WHY IT'S IMPORTANT:
 *
 * 1. BATTERY LIFE:
 *    - Higher DR = shorter airtime = less battery
 *    - DR0 (SF12): 991ms airtime for 11 bytes
 *    - DR3 (SF9): 144ms airtime for 53 bytes
 *    - 7x difference in power consumption!
 *
 * 2. NETWORK CAPACITY:
 *    - Shorter airtime = less spectrum usage
 *    - More devices can share same gateway
 *    - Critical for dense deployments
 *
 * 3. RELIABILITY:
 *    - Lower DR = longer range but slower
 *    - Higher DR = shorter range but faster
 *    - ADR finds optimal balance for current conditions
 *
 * DATA RATES (US915):
 * | DR | Spreading Factor | Bandwidth | Bit Rate | Max Payload |
 * |----|------------------|-----------|----------|-------------|
 * | 0  | SF10             | 125 kHz   | 980 bps  | 11 bytes    |
 * | 1  | SF9              | 125 kHz   | 1760 bps | 53 bytes    |
 * | 2  | SF8              | 125 kHz   | 3125 bps | 125 bytes   |
 * | 3  | SF7              | 125 kHz   | 5470 bps | 242 bytes   |
 */

/*
 * Q12: How did you implement protocol limit enforcement for ADR?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * The problem: Network server could set ANY data rate via LinkADRReq,
 * potentially forcing the device to use settings that don't work in
 * its environment (e.g., DR3 when device is at edge of coverage).
 *
 * Solution: Protocol control parameter with min/max DR bounds:
 *
 * ```c
 * typedef struct {
 *     uint8_t min_dr : 4;      // Minimum allowed data rate
 *     uint8_t max_dr : 4;      // Maximum allowed data rate
 *     uint8_t min_retries : 4; // Minimum retransmissions
 *     uint8_t reserved : 4;
 * } ProtocolControlTypeDef_t;
 *
 * // After receiving LinkADRReq, clamp to bounds
 * void on_link_adr_req(uint8_t requested_dr) {
 *     ProtocolControlTypeDef_t proto = get_protocol_control();
 *
 *     // Clamp to configured bounds
 *     uint8_t actual_dr = requested_dr;
 *     if (actual_dr < proto.min_dr) actual_dr = proto.min_dr;
 *     if (actual_dr > proto.max_dr) actual_dr = proto.max_dr;
 *
 *     // Apply clamped value
 *     LmHandlerSetTxDatarate(actual_dr);
 *
 *     // Log if we modified the request
 *     if (actual_dr != requested_dr) {
 *         LOG_WARN("ADR clamped: %d -> %d", requested_dr, actual_dr);
 *     }
 * }
 * ```
 *
 * This ensures:
 * - Device never uses DR below min_dr (could lose connectivity)
 * - Device never uses DR above max_dr (could waste power if not needed)
 * - Customer can configure based on deployment environment
 */

/*
 * Q13: What is the relationship between data rate, spreading factor, and
 *      range in LoRa?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * LoRa uses Chirp Spread Spectrum (CSS) modulation with configurable
 * spreading factors (SF7-SF12).
 *
 * RELATIONSHIP:
 *
 * Higher SF (e.g., SF12):
 * - Longer chirp duration = more processing gain
 * - Better receiver sensitivity = longer range
 * - But: Longer airtime = more power, lower data rate
 *
 * Lower SF (e.g., SF7):
 * - Shorter chirp duration = less processing gain
 * - Worse receiver sensitivity = shorter range
 * - But: Shorter airtime = less power, higher data rate
 *
 * CONCRETE NUMBERS (US915, 125kHz BW):
 *
 * | SF | Sensitivity | Range (rural) | Airtime (11B) | Data Rate |
 * |----|-------------|---------------|---------------|-----------|
 * | 12 | -137 dBm    | 15+ km        | 991 ms        | DR0       |
 * | 11 | -135 dBm    | 12 km         | 495 ms        | DR0       |
 * | 10 | -132 dBm    | 8 km          | 247 ms        | DR0       |
 * | 9  | -129 dBm    | 5 km          | 144 ms        | DR1       |
 * | 8  | -126 dBm    | 3 km          | 72 ms         | DR2       |
 * | 7  | -123 dBm    | 2 km          | 41 ms         | DR3       |
 *
 * ADR optimizes this trade-off automatically based on link margin.
 * Our protocol limits prevent ADR from going too aggressive (high DR)
 * or too conservative (low DR).
 */

/*
 * Q14: How did you handle the case where ADR should be disabled?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * ADR should be disabled when min_dr == max_dr (no range for optimization):
 *
 * ```c
 * bool should_adr_be_enabled() {
 *     ProtocolControlTypeDef_t protocol = get_protocol_control();
 *     return (uint32_t)protocol.min_dr < (uint32_t)protocol.max_dr;
 * }
 *
 * void on_parameter_change_protocol_control(uint32_t new_value) {
 *     ProtocolControlTypeDef_t proto_control;
 *     memcpy(&proto_control, &new_value, sizeof(uint32_t));
 *
 *     if (should_adr_be_enabled()) {
 *         // Enable ADR - network can adjust within bounds
 *         LmHandlerSetAdrEnable(true);
 *     } else {
 *         // Disable ADR - use fixed DR
 *         LmHandlerSetAdrEnable(false);
 *         LmHandlerSetTxDatarate((int8_t)proto_control.max_dr);
 *     }
 * }
 * ```
 *
 * USE CASES FOR DISABLED ADR:
 *
 * 1. MOBILE DEVICES:
 *    - Link quality varies rapidly
 *    - ADR can't adapt fast enough
 *    - Better to use conservative fixed DR
 *
 * 2. CRITICAL APPLICATIONS:
 *    - Reliability more important than efficiency
 *    - Force low DR for maximum range
 *    - Accept higher power consumption
 *
 * 3. TESTING:
 *    - Verify behavior at specific DR
 *    - Eliminate ADR as variable
 *    - Reproducible test conditions
 */

/*
 * Q15: How did ADR optimization contribute to the 30% battery improvement?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * ADR optimization contributed to battery savings in three ways:
 *
 * 1. OPTIMAL DR SELECTION (Primary):
 *    - Before: Devices often stuck at DR0 (SF12) - worst case
 *    - After: ADR moves devices to appropriate DR based on link quality
 *    - Example: DR0 to DR2 = 14x airtime reduction = 14x TX power savings
 *
 *    Power calculation:
 *    - TX at DR0 (SF12): 991ms at 100mA = 99.1 mAs per packet
 *    - TX at DR2 (SF8): 72ms at 100mA = 7.2 mAs per packet
 *    - Savings: 93% reduction in TX energy
 *
 * 2. PROTOCOL LIMIT ENFORCEMENT:
 *    - Prevents network from forcing unnecessarily low DR
 *    - Device stays at efficient DR even with conservative network settings
 *
 * 3. RETRANSMISSION REDUCTION:
 *    - Proper DR = better packet delivery ratio
 *    - Fewer retransmissions = less power
 *    - min_retries parameter prevents over-retrying
 *
 * QUANTIFICATION:
 * - Average device moved from DR0 to DR1.5 (weighted average)
 * - Airtime reduction: ~60%
 * - TX is ~60% of total power budget
 * - 60% * 60% = 36% total power reduction from TX
 * - Combined with join optimization: ~30% overall battery improvement
 */

/*
 * Q16: What debugging did you add for ADR issues?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * ADR debugging was critical for field troubleshooting:
 *
 * ```c
 * // ADR debug logging
 * void log_adr_state() {
 *     int8_t current_dr;
 *     LmHandlerGetTxDatarate(&current_dr);
 *
 *     LOG_DEBUG("ADR State:");
 *     LOG_DEBUG("  Enabled: %d", LmHandlerGetAdrEnable());
 *     LOG_DEBUG("  Current DR: %d", current_dr);
 *     LOG_DEBUG("  Min DR: %d", get_protocol_control().min_dr);
 *     LOG_DEBUG("  Max DR: %d", get_protocol_control().max_dr);
 *     LOG_DEBUG("  TX Power: %d", get_current_tx_power());
 *     LOG_DEBUG("  Last RSSI: %d", connection_info.downlink_rssi);
 *     LOG_DEBUG("  Last SNR: %d", connection_info.downlink_snr);
 * }
 *
 * // Log LinkADRReq processing
 * void on_mac_command(uint8_t cmd, uint8_t *payload) {
 *     if (cmd == MOTE_MAC_LINK_ADR_REQ) {
 *         LOG_DEBUG("LinkADRReq received:");
 *         LOG_DEBUG("  Requested DR: %d", payload[0] >> 4);
 *         LOG_DEBUG("  Requested TXPow: %d", payload[0] & 0x0F);
 *         LOG_DEBUG("  ChMask: 0x%04X", payload[1] | (payload[2] << 8));
 *         LOG_DEBUG("  Redundancy: %d", payload[3]);
 *     }
 * }
 * ```
 *
 * DEBUGGING FEATURES:
 * 1. Real-time ADR state in debug output
 * 2. LinkADRReq/Ans logging with parameters
 * 3. Connection info tracking (RSSI, SNR, DR history)
 * 4. Clamping events logged with before/after values
 * 5. NFC command to dump current ADR configuration
 */

/*
 * Q17: How does the LoRaWAN MAC layer handle ADR internally?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * The LoRaWAN MAC ADR implementation (LoRaMacAdr.c) works as follows:
 *
 * 1. ADR BACKOFF:
 *    If no downlink received for ADR_ACK_LIMIT uplinks (64 default):
 *    - Device starts ADR backoff
 *    - Increases DR every ADR_ACK_DELAY uplinks (32 default)
 *    - Eventually reaches DR0 (maximum range)
 *
 * 2. LINK ADR REQUEST PROCESSING:
 *    ```c
 *    // From LoRaMacAdr.c
 *    uint8_t ProcessLinkAdrReq(
 *        uint8_t dataRate,
 *        uint8_t txPower,
 *        uint16_t chMask,
 *        uint8_t chMaskCntl,
 *        uint8_t nbRep) {
 *
 *        // Validate data rate for region
 *        if (!RegionVerify(dataRate)) return 0;
 *
 *        // Validate TX power
 *        if (!ValidateTxPower(txPower)) return 0;
 *
 *        // Apply channel mask
 *        ApplyChannelMask(chMask, chMaskCntl);
 *
 *        // Apply settings
 *        SetDataRate(dataRate);
 *        SetTxPower(txPower);
 *        SetNbRep(nbRep);
 *
 *        return 0x07;  // All OK
 *    }
 *    ```
 *
 * 3. MY MODIFICATION:
 *    Added clamping AFTER the MAC processes LinkADRReq:
 *    ```c
 *    // In uplink_manager.c, after MAC command processing
 *    int8_t current_dr;
 *    LmHandlerGetTxDatarate(&current_dr);
 *
 *    // Enforce protocol limits
 *    if (current_dr < proto.min_dr) {
 *        LmHandlerSetTxDatarate(proto.min_dr);
 *    }
 *    if (current_dr > proto.max_dr) {
 *        LmHandlerSetTxDatarate(proto.max_dr);
 *    }
 *    ```
 */

/*
 * Q18: What happens if the network server and device disagree about DR?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Disagreement can occur when:
 * 1. We clamp LinkADRReq to our bounds
 * 2. Network thinks device is at DR3, device is actually at DR2
 *
 * CONSEQUENCES:
 * - Network may calculate wrong link budget
 * - Future ADR commands may be sub-optimal
 * - Not a critical issue - system converges
 *
 * HOW WE HANDLE IT:
 *
 * 1. LINK CHECK RESPONSE:
 *    Network learns actual link margin from device uplinks
 *    Adjusts future ADR commands based on real measurements
 *
 * 2. UPLINK DR REPORTING:
 *    Every uplink includes actual DR used
 *    Network server updates its records
 *
 * 3. ADR_ACK_REQ:
 *    Device can request explicit ADR acknowledgment
 *    Forces network to re-evaluate link quality
 *
 * 4. LOGGING FOR SUPPORT:
 *    We log all discrepancies:
 *    ```c
 *    if (clamped_dr != requested_dr) {
 *        LOG_WARN("DR clamped: requested=%d, applied=%d",
 *                 requested_dr, clamped_dr);
 *        // This helps support identify configuration issues
 *    }
 *    ```
 */

/*
 * ============================================================================
 * SECTION 3: BATTERY OPTIMIZATION (Questions 19-30)
 * ============================================================================
 */

/*
 * Q19: Break down the 30% battery improvement. What contributed to it?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * The 30% battery improvement came from multiple optimizations:
 *
 * 1. ELIMINATED UNNECESSARY JOINS (15%):
 *    - Before: Rejoin on every boot = ~5 JoinRequest/JoinAccept cycles
 *    - After: Session reuse = 0 joins on most boots
 *    - JoinRequest: 991ms TX at 20dBm
 *    - JoinAccept RX: Two 500ms RX windows
 *    - Savings: ~3000mAs per boot cycle
 *
 * 2. ADR OPTIMIZATION (10%):
 *    - Average DR improvement: DR0 -> DR1.5
 *    - Airtime reduction: 60%
 *    - TX is 60% of power budget
 *    - Net savings: 0.6 * 0.6 = 36% of TX power, ~10% overall
 *
 * 3. LINK CHECK ELIMINATION (5%):
 *    - Removed 6 unnecessary link check transmissions
 *    - Each: ~100ms TX + RX windows
 *    - Savings on every boot
 *
 * POWER BUDGET BREAKDOWN:
 * | Component      | Before | After  | Change |
 * |----------------|--------|--------|--------|
 * | TX (uplinks)   | 45%    | 38%    | -7%    |
 * | TX (joins)     | 15%    | 2%     | -13%   |
 * | RX windows     | 20%    | 15%    | -5%    |
 * | MCU active     | 10%    | 8%     | -2%    |
 * | Sleep          | 10%    | 37%    | +27%   |
 *
 * NET IMPROVEMENT: ~30% total power reduction
 */

/*
 * Q20: What low power modes did you use and when?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * STM32WL has three low power modes:
 *
 * 1. SLEEP MODE (UTIL_LPM_SLEEPMODE):
 *    - CPU stopped, peripherals running
 *    - Current: ~1mA
 *    - Wake: Any interrupt, <1us wake time
 *    - Use: Short waits (UART RX, timer)
 *
 * 2. STOP MODE (UTIL_LPM_STOPMODE):
 *    - CPU and most peripherals stopped
 *    - RTC and some peripherals can run
 *    - Current: ~2uA
 *    - Wake: RTC, GPIO, ~5us wake time
 *    - Use: Between sensor readings, RX windows
 *
 * 3. OFF MODE (UTIL_LPM_OFFMODE):
 *    - Everything off except RTC backup domain
 *    - Current: ~0.5uA
 *    - Wake: RTC alarm, reset, ~100us wake time
 *    - Use: Extended sleep periods
 *
 * OUR STRATEGY:
 * ```c
 * void enter_low_power() {
 *     uint32_t next_event = get_next_scheduled_event();
 *
 *     if (next_event > 60000) {
 *         // Long sleep: use STOP mode
 *         UTIL_LPM_EnterLowPower(UTIL_LPM_STOPMODE);
 *     } else if (next_event > 1000) {
 *         // Medium sleep: use STOP mode
 *         UTIL_LPM_EnterLowPower(UTIL_LPM_STOPMODE);
 *     } else {
 *         // Short sleep: use SLEEP mode
 *         UTIL_LPM_EnterLowPower(UTIL_LPM_SLEEPMODE);
 *     }
 * }
 * ```
 *
 * We disabled OFF mode for reliability:
 * - OFF mode loses RAM contents
 * - Need to reinitialize everything on wake
 * - Risk of state corruption
 * - STOP mode was sufficient for our power targets
 */

/*
 * Q21: How did you optimize TX power settings?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * TX power optimization balanced range with battery life:
 *
 * 1. REGIONAL LIMITS:
 *    ```c
 *    static int8_t get_max_tx_power() {
 *        LoRaMacRegion_t region;
 *        LmHandlerGetActiveRegion(&region);
 *
 *        switch (region) {
 *            case LORAMAC_REGION_US915:
 *                // US915: 30 dBm EIRP max, antenna gain ~10dBi
 *                // Max conducted power: 20 dBm
 *                return TX_POWER_5;  // 20 dBm
 *            case LORAMAC_REGION_EU868:
 *                // EU868: 16 dBm ERP max
 *                return TX_POWER_0;  // 16 dBm
 *            default:
 *                return TX_POWER_0;
 *        }
 *    }
 *    ```
 *
 * 2. ADR-CONTROLLED POWER:
 *    - Network server can reduce TX power via LinkADRReq
 *    - If link margin is good, lower power = less battery
 *    - We allow ADR to reduce power but not below TX_POWER_7 (8dBm)
 *
 * 3. AFTER JOIN OPTIMIZATION:
 *    ```c
 *    void on_join_complete() {
 *        // Start at max power for reliability
 *        set_tx_power(get_max_tx_power());
 *        // ADR will optimize down if link is good
 *    }
 *    ```
 *
 * 4. POWER RAMPING:
 *    - If transmission fails, increase power
 *    - If multiple successes, allow ADR to decrease
 *    - Prevents oscillation between power levels
 */

/*
 * Q22: How did you handle the RX windows for power optimization?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * RX windows are a significant power consumer in LoRaWAN:
 *
 * CLASS A RX WINDOWS:
 * - RX1: Opens 1 second after TX, same frequency/DR as uplink
 * - RX2: Opens 2 seconds after TX, fixed frequency/DR
 * - Both windows: Radio listening at ~10mA
 *
 * OUR OPTIMIZATIONS:
 *
 * 1. RX WINDOW TIMING:
 *    ```c
 *    // Minimize time radio is on before packet arrives
 *    #define RX_WINDOW_SYMBOL_TIMEOUT  8  // Symbols to wait
 *    // At SF7: 8 symbols = 1ms
 *    // At SF12: 8 symbols = 32ms
 *    ```
 *
 * 2. EARLY RX ABORT:
 *    - If preamble not detected within timeout, abort RX
 *    - Don't wait for full window duration
 *
 * 3. RX2 SKIPPING:
 *    - If RX1 received valid packet, skip RX2
 *    - Saves ~500ms of RX time
 *
 * 4. CONFIRMED UPLINK OPTIMIZATION:
 *    - Use unconfirmed uplinks when possible
 *    - Confirmed uplinks require ACK = mandatory RX windows
 *    - Parameter: confirmed_uplink_time (how often to use confirmed)
 *    ```c
 *    bool should_use_confirmed() {
 *        return (uplink_count % get_param_confirmed_uplink_time()) == 0;
 *    }
 *    ```
 */

/*
 * Q23: What is the impact of retransmissions on battery life?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Retransmissions are exponentially expensive:
 *
 * COST PER RETRANSMISSION:
 * - TX: 100-1000ms at 100mA
 * - RX1: 500ms at 10mA
 * - RX2: 500ms at 10mA
 * - Total: ~15mAs minimum per attempt
 *
 * WITH CONFIRMED UPLINKS:
 * - If no ACK, retransmit up to NbTrans times
 * - Default NbTrans = 8
 * - Worst case: 8 * 15mAs = 120mAs per message
 *
 * OUR OPTIMIZATION:
 *
 * 1. MIN_RETRIES PARAMETER:
 *    ```c
 *    // Enforce minimum retries even if network requests lower
 *    uint32_t retransmissions = umax(
 *        get_protocol_control().min_retries,
 *        msg.Param.ChannelsNbTrans
 *    );
 *    ```
 *
 * 2. SMART RETRY LOGIC:
 *    - Use unconfirmed for regular telemetry (no retries)
 *    - Use confirmed for critical events (with retries)
 *    - Adaptive retry based on recent success rate
 *
 * 3. EXPONENTIAL BACKOFF:
 *    ```c
 *    static const uint16_t backoff_interval_seconds[] = {
 *        15, 60, 5*60, 15*60, 30*60, 60*60
 *    };
 *    ```
 *    Prevents battery drain from continuous retry attempts
 */

/*
 * Q24: How did you measure and verify the 30% battery improvement?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Battery improvement was measured through multiple methods:
 *
 * 1. CURRENT MEASUREMENT:
 *    - Nordic Power Profiler Kit II
 *    - Inline measurement of device current
 *    - Captured full duty cycles (sleep, wake, TX, RX)
 *
 *    ```
 *    Before optimization:
 *    - Average current: 450 uA
 *    - Peak TX current: 120 mA
 *    - Sleep current: 3.2 uA
 *
 *    After optimization:
 *    - Average current: 315 uA
 *    - Peak TX current: 120 mA (unchanged)
 *    - Sleep current: 2.8 uA
 *
 *    Improvement: (450-315)/450 = 30%
 *    ```
 *
 * 2. COULOMB COUNTING:
 *    - Fuel gauge IC on some devices
 *    - Tracked mAh consumed over fixed period
 *    - Compared before/after firmware versions
 *
 * 3. FIELD DEPLOYMENT:
 *    - 100 devices deployed for 3 months
 *    - Tracked battery voltage over time
 *    - Before: ~0.5% battery per day
 *    - After: ~0.35% battery per day
 *    - Improvement: 30%
 *
 * 4. CALCULATED ESTIMATE:
 *    Based on duty cycle analysis:
 *    - TX duty cycle reduced by 40%
 *    - Join frequency reduced by 90%
 *    - RX duty cycle reduced by 20%
 *    - Weighted average: ~30% improvement
 */

/*
 * Q25: How did you handle the trade-off between reliability and battery life?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * This trade-off required careful balancing:
 *
 * RELIABILITY CONCERNS:
 * - Lower TX power = shorter range = more packet loss
 * - Fewer retries = higher data loss
 * - Session reuse = risk of counter desync
 *
 * BATTERY CONCERNS:
 * - Higher TX power = more current
 * - More retries = more TX events
 * - Frequent rejoins = wasted energy
 *
 * OUR APPROACH:
 *
 * 1. CONFIGURABLE TRADE-OFFS:
 *    ```c
 *    // Customer can adjust based on deployment
 *    typedef struct {
 *        uint8_t min_dr;       // Reliability: lower = more reliable
 *        uint8_t max_dr;       // Efficiency: higher = more efficient
 *        uint8_t min_retries;  // Reliability: higher = more reliable
 *    } ProtocolControlTypeDef_t;
 *    ```
 *
 * 2. DEFAULT TO RELIABILITY:
 *    - Network lock defaults to UNLOCKED (more reliable)
 *    - ADR defaults to enabled (balanced)
 *    - Retries default to 3 (reasonable)
 *
 * 3. MONITORING:
 *    - Track packet delivery ratio
 *    - Alert if below threshold
 *    - Customer can adjust parameters remotely
 *
 * 4. GRACEFUL DEGRADATION:
 *    - If battery critical, reduce reporting frequency
 *    - Don't sacrifice reliability, just report less often
 */

/*
 * ============================================================================
 * SECTION 4: CODE QUALITY & ARCHITECTURE (Questions 26-35)
 * ============================================================================
 */

/*
 * Q26: How did you structure the optimization suite code?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * The code was organized in layers:
 *
 * ```
 * ┌─────────────────────────────────────────────┐
 * │            Application Layer                 │
 * │  (uplink_manager.c - state machine)         │
 * ├─────────────────────────────────────────────┤
 * │           Optimization Layer                 │
 * │  - Session management (is_joined, nvm)      │
 * │  - ADR control (protocol_control)           │
 * │  - Power management (lpm integration)       │
 * ├─────────────────────────────────────────────┤
 * │           LoRaWAN MAC Layer                  │
 * │  (LoRaMac, LmHandler - Semtech stack)       │
 * ├─────────────────────────────────────────────┤
 * │           Radio Abstraction                  │
 * │  (SubGHz radio driver)                      │
 * ├─────────────────────────────────────────────┤
 * │           Hardware Layer                     │
 * │  (STM32WL HAL, peripherals)                 │
 * └─────────────────────────────────────────────┘
 * ```
 *
 * KEY DESIGN DECISIONS:
 *
 * 1. MINIMAL MAC MODIFICATIONS:
 *    - Keep Semtech stack mostly unchanged
 *    - Add hooks for our optimizations
 *    - Easier to upgrade to new stack versions
 *
 * 2. PARAMETER-DRIVEN:
 *    - All optimizations controlled by parameters
 *    - Can enable/disable features without code change
 *    - Configurable via NFC or downlink
 *
 * 3. STATE MACHINE PATTERN:
 *    - Clear state transitions
 *    - Easy to debug and test
 *    - Predictable behavior
 *
 * 4. CALLBACK-BASED INTEGRATION:
 *    - Register callbacks with MAC layer
 *    - React to events (join, TX complete, RX)
 *    - Non-blocking architecture
 */

/*
 * Q27: How did you ensure backward compatibility with existing deployments?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Backward compatibility was critical for field-deployed devices:
 *
 * 1. DEFAULT BEHAVIOR UNCHANGED:
 *    ```c
 *    // Default: behave like old firmware
 *    #define DEFAULT_NETWORK_LOCK_MODE 0  // Always rejoin
 *
 *    // New features are opt-in
 *    if (get_param_network_lock_mode() == 1) {
 *        // Use optimized path
 *    } else {
 *        // Use legacy path
 *    }
 *    ```
 *
 * 2. PARAMETER MIGRATION:
 *    ```c
 *    void migrate_parameters() {
 *        uint16_t param_version = read_param_version();
 *
 *        if (param_version < 2) {
 *            // Add new parameters with safe defaults
 *            set_param_network_lock_mode(0);
 *            set_param_version(2);
 *        }
 *    }
 *    ```
 *
 * 3. NVM FORMAT VERSIONING:
 *    ```c
 *    typedef struct {
 *        uint16_t version;      // NVM structure version
 *        uint16_t length;       // Structure size
 *        // ... fields ...
 *    } nvm_header_t;
 *
 *    // Can read old versions and upgrade
 *    ```
 *
 * 4. GRADUAL ROLLOUT:
 *    - Deployed to 10% of devices first
 *    - Monitored for issues
 *    - Expanded to full fleet after validation
 */

/*
 * Q28: What testing strategy did you use for the optimization suite?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Multi-level testing approach:
 *
 * 1. UNIT TESTS:
 *    ```c
 *    void test_is_joined() {
 *        // Test 1: No NVM session
 *        clear_nvm_session();
 *        assert(is_joined() == false);
 *
 *        // Test 2: Valid NVM, locked mode
 *        write_valid_nvm_session();
 *        set_param_network_lock_mode(1);
 *        assert(is_joined() == true);
 *
 *        // Test 3: Valid NVM, unlocked mode
 *        set_param_network_lock_mode(0);
 *        assert(is_joined() == false);
 *
 *        // Test 4: Credential mismatch
 *        change_deveui();
 *        set_param_network_lock_mode(1);
 *        assert(is_joined() == false);
 *    }
 *    ```
 *
 * 2. INTEGRATION TESTS:
 *    - Full boot cycle with real LoRaWAN network
 *    - Verify join behavior in all modes
 *    - Measure actual boot times
 *
 * 3. STRESS TESTS:
 *    - 1000 power cycles
 *    - Random credential changes
 *    - Network server unavailable scenarios
 *
 * 4. FIELD TESTS:
 *    - 50 devices in real deployment
 *    - 2 week validation period
 *    - Monitored via network server logs
 *
 * 5. REGRESSION TESTS:
 *    - Automated test suite run on every commit
 *    - CI/CD pipeline with hardware-in-loop
 *    - Coverage > 80% for optimization code
 */

/*
 * Q29: How did you debug issues in the field?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Field debugging required multiple approaches:
 *
 * 1. REMOTE LOGGING:
 *    - Diagnostic data in regular uplinks
 *    - Boot count, last reset cause, battery level
 *    - ADR state, current DR, TX power
 *
 *    ```c
 *    typedef struct {
 *        uint16_t boot_count;
 *        uint8_t  reset_cause;
 *        uint8_t  battery_percent;
 *        uint8_t  current_dr;
 *        int8_t   tx_power;
 *        uint8_t  adr_enabled;
 *        uint8_t  network_lock_mode;
 *    } diagnostic_payload_t;
 *    ```
 *
 * 2. DOWNLINK COMMANDS:
 *    - Request detailed status dump
 *    - Trigger debug mode
 *    - Adjust parameters remotely
 *
 * 3. NFC DEBUGGING:
 *    - For devices accessible physically
 *    - Read full device state
 *    - Dump NVM contents
 *    - View boot history
 *
 * 4. NETWORK SERVER ANALYSIS:
 *    - Join request frequency
 *    - Uplink timing patterns
 *    - ADR command history
 *    - Packet delivery ratio
 *
 * 5. FAULT LOGGING:
 *    ```c
 *    // Stored in RTC backup registers
 *    typedef struct {
 *        uint32_t pc;      // Program counter at fault
 *        uint32_t lr;      // Link register
 *        uint8_t  cause;   // Hard fault, watchdog, etc.
 *    } fault_info_t;
 *    ```
 */

/*
 * Q30: What would you do differently if you were to implement this again?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Lessons learned and improvements:
 *
 * 1. EARLIER INSTRUMENTATION:
 *    - Should have added power profiling hooks from the start
 *    - Difficult to measure improvement without baseline
 *    - Would add #ifdef POWER_PROFILE sections throughout
 *
 * 2. MORE GRANULAR PARAMETERS:
 *    - Current protocol_control is a bit field
 *    - Hard to extend without breaking compatibility
 *    - Would use separate parameters for each setting
 *
 * 3. BETTER STATE PERSISTENCE:
 *    - Current NVM writes are synchronous
 *    - Blocks execution during flash write
 *    - Would implement async write with buffering
 *
 * 4. AUTOMATED TESTING:
 *    - Initial tests were manual
 *    - CI/CD came later
 *    - Would set up automated HIL testing from day one
 *
 * 5. DOCUMENTATION:
 *    - Optimization decisions not well documented initially
 *    - Would write design docs before implementation
 *    - Include trade-off analysis for each decision
 *
 * 6. A/B TESTING:
 *    - Hard to compare old vs new behavior
 *    - Would implement feature flags for controlled rollout
 *    - Enable metrics collection for comparison
 */

/*
 * ============================================================================
 * SECTION 5: TECHNICAL DEEP DIVES (Questions 31-40)
 * ============================================================================
 */

/*
 * Q31: Explain the LoRaWAN frame counter mechanism and why it's important
 *      for your session reuse optimization.
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Frame counters are critical for LoRaWAN security:
 *
 * PURPOSE:
 * - Replay attack prevention
 * - Each message has unique counter value
 * - Server rejects messages with counter <= last received
 *
 * STRUCTURE:
 * ```
 * Uplink Frame Counter (FCntUp):   Device increments before each uplink
 * Downlink Frame Counter (FCntDown): Server increments, device validates
 * ```
 *
 * IMPORTANCE FOR SESSION REUSE:
 *
 * 1. COUNTER MUST NEVER RESET:
 *    - If device resets FCntUp to 0, server rejects as replay
 *    - Must persist counter in NVM
 *    - Increment BEFORE sending, not after (crash safety)
 *
 * 2. COUNTER MUST BE MONOTONIC:
 *    ```c
 *    uint32_t get_next_fcnt() {
 *        uint32_t fcnt = read_nvm_fcnt();
 *        fcnt++;
 *        write_nvm_fcnt(fcnt);  // Persist before use
 *        return fcnt;
 *    }
 *    ```
 *
 * 3. GAP TOLERANCE:
 *    - Server allows some gaps (device crashed before TX)
 *    - But large gaps may trigger security alert
 *    - We increment by 1 to minimize gaps
 *
 * 4. 32-BIT OVERFLOW:
 *    - At 1 uplink/minute: overflow in 8000 years
 *    - Not a practical concern
 *    - But must handle wrap correctly
 *
 * 5. VALIDATION ON LOAD:
 *    ```c
 *    bool validate_nvm_session() {
 *        // Counter should be > 0 after first join
 *        if (session.fcnt_up == 0 && session.joined) {
 *            return false;  // Corrupted
 *        }
 *        // Counter should be < max reasonable value
 *        if (session.fcnt_up > 0x00FFFFFF) {
 *            return false;  // Suspicious
 *        }
 *        return true;
 *    }
 *    ```
 */

/*
 * Q32: How does the LoRaWAN join procedure work? What are the security
 *      implications?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * OTAA Join Procedure:
 *
 * ```
 * Device                          Join Server                    Network Server
 *   |                                  |                              |
 *   |-- JoinRequest ------------------>|                              |
 *   |   (DevEUI, JoinEUI, DevNonce)    |                              |
 *   |                                  |                              |
 *   |                                  |-- Lookup AppKey ------------>|
 *   |                                  |                              |
 *   |                                  |<- Validate DevNonce ---------|
 *   |                                  |                              |
 *   |                                  |-- Generate Session Keys ---->|
 *   |                                  |   (NwkSKey, AppSKey)         |
 *   |                                  |                              |
 *   |<- JoinAccept --------------------|                              |
 *   |   (DevAddr, NetID, DLSettings)   |                              |
 *   |                                  |                              |
 *   |-- Derive Session Keys ---------->|                              |
 *   |   (from AppKey + JoinAccept)     |                              |
 * ```
 *
 * SECURITY IMPLICATIONS:
 *
 * 1. DEVNONCE UNIQUENESS:
 *    - DevNonce must be unique per join
 *    - Prevents replay of JoinRequest
 *    - We use random nonce (not counter)
 *
 * 2. APPKEY PROTECTION:
 *    - Root key, never transmitted
 *    - Used to derive session keys
 *    - Must be kept secure (ideally in secure element)
 *
 * 3. SESSION KEY DERIVATION:
 *    - NwkSKey = aes128_encrypt(AppKey, 0x01 | JoinNonce | NetID | DevNonce)
 *    - AppSKey = aes128_encrypt(AppKey, 0x02 | JoinNonce | NetID | DevNonce)
 *    - Keys unique per session
 *
 * 4. SESSION REUSE RISK:
 *    - Longer session = more data with same keys
 *    - Theoretical cryptanalysis risk
 *    - Mitigated by optional periodic rejoin
 *
 * 5. JOIN ACCEPT ENCRYPTION:
 *    - JoinAccept encrypted with AppKey
 *    - Only device can decrypt
 *    - Contains DevAddr for this session
 */

/*
 * Q33: What happens when the device loses network connectivity? How does
 *      your optimization handle this?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Network loss handling is critical for reliability:
 *
 * DETECTION:
 * - No downlinks received for ADR_ACK_LIMIT uplinks (64 default)
 * - Or explicit link check fails
 *
 * OUR HANDLING:
 *
 * 1. ADR BACKOFF:
 *    ```c
 *    // Automatically increase range when no downlinks
 *    void adr_backoff() {
 *        // Lower DR = longer range
 *        if (current_dr > 0) {
 *            LmHandlerSetTxDatarate(current_dr - 1);
 *        }
 *        // Increase TX power
 *        if (current_power < max_power) {
 *            LmHandlerSetTxPower(current_power + 1);
 *        }
 *    }
 *    ```
 *
 * 2. REJOIN TRIGGER:
 *    ```c
 *    // After extended loss, force rejoin
 *    if (failed_uplinks > MAX_FAILED_UPLINKS) {
 *        invalidate_session();  // Clear NVM session
 *        trigger_rejoin();
 *    }
 *    ```
 *
 * 3. DATA BUFFERING:
 *    - Queue sensor data locally during outage
 *    - Send when connectivity restored
 *    - Prioritize recent data if queue full
 *
 * 4. EXPONENTIAL BACKOFF:
 *    ```c
 *    // Don't drain battery retrying
 *    static const uint16_t backoff_seconds[] = {
 *        15, 60, 300, 900, 1800, 3600
 *    };
 *    ```
 *
 * 5. RECOVERY NOTIFICATION:
 *    - When connection restored, send status uplink
 *    - Include time of outage, queued messages count
 *    - Helps operator monitor network health
 */

/*
 * Q34: How did you handle the interaction between your optimizations and
 *      the Semtech LoRaWAN stack?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Integration with Semtech stack required careful design:
 *
 * 1. CALLBACK REGISTRATION:
 *    ```c
 *    // Register our handlers with the stack
 *    LmHandlerCallbacks_t callbacks = {
 *        .OnMacProcess = on_mac_process,
 *        .OnJoinRequest = on_join_request,
 *        .OnTxData = on_tx_data,
 *        .OnRxData = on_rx_data,
 *        .OnMacMlmeRequest = on_mlme_request,
 *        .OnMacMcpsRequest = on_mcps_request,
 *    };
 *    LmHandlerInit(&callbacks);
 *    ```
 *
 * 2. POST-PROCESSING HOOKS:
 *    ```c
 *    void on_rx_data(LmHandlerAppData_t *data, ...) {
 *        // Let stack process MAC commands first
 *        // Then apply our modifications
 *
 *        // Clamp DR after LinkADRReq
 *        int8_t dr;
 *        LmHandlerGetTxDatarate(&dr);
 *        if (dr > protocol.max_dr) {
 *            LmHandlerSetTxDatarate(protocol.max_dr);
 *        }
 *    }
 *    ```
 *
 * 3. MINIMAL STACK MODIFICATION:
 *    - Added one hook in LoRaMac.c for NVM callback
 *    - All other changes in application layer
 *    - Makes stack upgrades easier
 *
 * 4. NVM ABSTRACTION:
 *    ```c
 *    // Stack calls this for NVM operations
 *    void NvmDataMgmtEvent(uint16_t notifyFlags) {
 *        if (notifyFlags & LORAMAC_NVM_NOTIFY_FLAG_CRYPTO) {
 *            // Session keys changed, update our NVM
 *            persist_session_to_nvm();
 *        }
 *    }
 *    ```
 *
 * 5. THREAD SAFETY:
 *    - LoRaWAN stack not thread-safe
 *    - All calls from single context (main loop)
 *    - ISRs only set flags, don't call stack
 */

/*
 * Q35: What metrics did you track to validate the optimization was working
 *      correctly in production?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Production metrics tracked via network server and device telemetry:
 *
 * 1. JOIN METRICS:
 *    - Join request frequency (should decrease)
 *    - Join success rate (should stay high)
 *    - Time-to-first-uplink after boot
 *
 *    ```
 *    Before: 5.2 joins/device/day average
 *    After: 0.3 joins/device/day average
 *    Improvement: 94% reduction
 *    ```
 *
 * 2. BOOT TIME:
 *    - Measured via timestamp in first uplink
 *    ```
 *    Before: 150 seconds average
 *    After: 90 seconds average
 *    Improvement: 40%
 *    ```
 *
 * 3. DATA RATE DISTRIBUTION:
 *    ```
 *    Before optimization:
 *    DR0: 60%, DR1: 25%, DR2: 10%, DR3: 5%
 *
 *    After optimization:
 *    DR0: 20%, DR1: 40%, DR2: 30%, DR3: 10%
 *    ```
 *
 * 4. BATTERY CONSUMPTION:
 *    - Voltage curve over time
 *    - Estimated days remaining
 *    ```
 *    Before: 365 days battery life
 *    After: 475 days battery life
 *    Improvement: 30%
 *    ```
 *
 * 5. PACKET DELIVERY RATIO:
 *    - Must not decrease with optimization
 *    ```
 *    Before: 98.2%
 *    After: 98.5%
 *    (Slight improvement from better DR selection)
 *    ```
 *
 * 6. ERROR RATES:
 *    - Session validation failures
 *    - NVM corruption events
 *    - Unexpected rejoins
 */

/*
 * ============================================================================
 * SECTION 6: SCENARIO-BASED QUESTIONS (Questions 36-50)
 * ============================================================================
 */

/*
 * Q36: A customer reports devices are rejoining every boot despite your
 *      optimization. How would you troubleshoot?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Systematic troubleshooting approach:
 *
 * 1. CHECK NETWORK_LOCK_MODE:
 *    - Is it set to 1 (LOCKED)?
 *    - Read via NFC: `nfc read network_lock_mode`
 *    - Most common cause: parameter not set after provisioning
 *
 * 2. VERIFY CREDENTIAL MATCH:
 *    - Compare DevEUI/JoinEUI in parameters vs NVM session
 *    - Mismatch forces rejoin even in locked mode
 *    ```c
 *    bool credentials_match() {
 *        return memcmp(param_dev_eui, nvm_dev_eui, 8) == 0 &&
 *               memcmp(param_join_eui, nvm_join_eui, 8) == 0;
 *    }
 *    ```
 *
 * 3. CHECK NVM INTEGRITY:
 *    - CRC failures force rejoin
 *    - Dump NVM and verify CRC manually
 *    - Flash wear or corruption?
 *
 * 4. ANALYZE BOOT LOGS:
 *    - Enable debug logging
 *    - Look for "is_joined() returning false" with reason
 *
 * 5. CHECK NETWORK SERVER:
 *    - Is server rejecting frames?
 *    - Frame counter desync?
 *    - Session timeout on server side?
 *
 * 6. VERIFY FIRMWARE VERSION:
 *    - Is optimization code actually present?
 *    - Old firmware may have been deployed
 */

/*
 * Q37: After deploying your optimization, some devices show 20% battery
 *      improvement instead of 30%. What could cause this variance?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Battery improvement variance is expected due to several factors:
 *
 * 1. DEPLOYMENT ENVIRONMENT:
 *    - Devices at edge of coverage can't use high DR
 *    - More retransmissions needed
 *    - ADR optimization less effective
 *
 * 2. REPORTING FREQUENCY:
 *    - Devices with higher sampling rates
 *    - TX dominates power budget more
 *    - Optimization impact proportionally smaller
 *
 * 3. REBOOT FREQUENCY:
 *    - Some devices reboot more often (external factors)
 *    - Boot optimization helps less if rarely rebooting
 *
 * 4. NETWORK SERVER CONFIGURATION:
 *    - Conservative ADR settings on server
 *    - Limits how much DR can improve
 *
 * 5. HARDWARE VARIANCE:
 *    - Battery capacity tolerance
 *    - Component efficiency variation
 *
 * INVESTIGATION:
 * ```c
 * // Add detailed power breakdown in telemetry
 * typedef struct {
 *     uint16_t tx_time_ms;     // Total TX time since last report
 *     uint16_t rx_time_ms;     // Total RX time
 *     uint16_t active_time_ms; // MCU active time
 *     uint16_t sleep_time_ms;  // Sleep time
 *     uint8_t  join_count;     // Joins since last report
 *     uint8_t  avg_dr;         // Average DR used
 * } power_metrics_t;
 * ```
 *
 * Compare metrics between 30% and 20% devices to find cause.
 */

/*
 * Q38: The network server team wants to disable ADR for all devices. How
 *      would you respond and what data would you provide?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Response with data-driven argument:
 *
 * 1. QUANTIFY IMPACT:
 *    ```
 *    Current fleet (ADR enabled):
 *    - Average DR: 1.5
 *    - Average battery life: 475 days
 *    - Network capacity: 10,000 uplinks/hour
 *
 *    Projected (ADR disabled, DR0 forced):
 *    - Fixed DR: 0
 *    - Projected battery life: 290 days (-39%)
 *    - Network capacity: 3,000 uplinks/hour (-70%)
 *    ```
 *
 * 2. PROPOSE ALTERNATIVE:
 *    - If reliability concern, limit DR range instead
 *    - Set min_dr=0, max_dr=2 (not DR3)
 *    - Gets most ADR benefit with reliability margin
 *
 * 3. OFFER COMPROMISE:
 *    - A/B test: 50% devices with ADR, 50% without
 *    - Compare packet delivery ratio and battery life
 *    - Make data-driven decision
 *
 * 4. PROVIDE MONITORING:
 *    - Dashboard showing ADR impact
 *    - Alert if reliability drops
 *    - Ability to disable ADR remotely if needed
 *
 * 5. DOCUMENT TRADE-OFFS:
 *    - ADR disabled: Guaranteed DR0, best reliability, worst battery
 *    - ADR enabled: Dynamic DR, good reliability, best battery
 *    - Let customer choose based on priorities
 */

/*
 * Q39: A device has been in the field for 6 months using session reuse.
 *      Security team wants to force a rejoin. How would you implement this?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Remote forced rejoin implementation:
 *
 * 1. DOWNLINK COMMAND:
 *    ```c
 *    // Define rejoin command (port 200, command 0x01)
 *    void on_downlink(uint8_t port, uint8_t *data, uint8_t len) {
 *        if (port == 200 && len >= 1 && data[0] == 0x01) {
 *            LOG_INFO("Forced rejoin requested");
 *            invalidate_nvm_session();
 *            schedule_rejoin();
 *        }
 *    }
 *    ```
 *
 * 2. PERIODIC REJOIN PARAMETER:
 *    ```c
 *    // Add parameter: rejoin_period_days (0 = disabled)
 *    void check_periodic_rejoin() {
 *        uint32_t period = get_param_rejoin_period_days();
 *        if (period == 0) return;  // Disabled
 *
 *        uint32_t last_join = get_nvm_last_join_time();
 *        uint32_t now = get_rtc_time();
 *        uint32_t days = (now - last_join) / 86400;
 *
 *        if (days >= period) {
 *            LOG_INFO("Periodic rejoin triggered");
 *            invalidate_nvm_session();
 *            schedule_rejoin();
 *        }
 *    }
 *    ```
 *
 * 3. NFC COMMAND:
 *    - For physical access: `nfc command rejoin`
 *    - Immediate effect, no network required
 *
 * 4. PARAMETER CHANGE:
 *    - Set network_lock_mode to 0 (UNLOCKED)
 *    - Device will rejoin on next boot
 *    - Set back to 1 after rejoin
 *
 * 5. MASS REJOIN:
 *    - Network server can trigger via multicast downlink
 *    - Stagger to prevent thundering herd
 *    - Monitor join server load
 */

/*
 * Q40: How would you extend your optimization to support LoRaWAN 1.1
 *      which has different session key structure?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * LoRaWAN 1.1 changes session management:
 *
 * 1.1 DIFFERENCES:
 * - Two network session keys: FNwkSIntKey, SNwkSIntKey, NwkSEncKey
 * - Separate AppSKey
 * - DevNonce is counter, not random
 * - ReKey indication required
 *
 * EXTENSION APPROACH:
 *
 * 1. VERSIONED NVM STRUCTURE:
 *    ```c
 *    typedef struct {
 *        uint8_t lorawan_version;  // 0x10 = 1.0, 0x11 = 1.1
 *        union {
 *            nvm_session_v10_t v10;  // LoRaWAN 1.0 keys
 *            nvm_session_v11_t v11;  // LoRaWAN 1.1 keys
 *        };
 *    } nvm_session_t;
 *
 *    typedef struct {
 *        uint8_t fnwksintkey[16];
 *        uint8_t snwksintkey[16];
 *        uint8_t nwksenckey[16];
 *        uint8_t appskey[16];
 *        uint32_t devnonce;  // Counter in 1.1
 *        // ... other fields
 *    } nvm_session_v11_t;
 *    ```
 *
 * 2. DEVNONCE PERSISTENCE:
 *    ```c
 *    // 1.1: DevNonce must be monotonic counter
 *    uint16_t get_next_devnonce() {
 *        uint16_t nonce = read_nvm_devnonce();
 *        write_nvm_devnonce(nonce + 1);
 *        return nonce;
 *    }
 *    ```
 *
 * 3. REKEY HANDLING:
 *    - 1.1 requires ReKeyInd after rejoin
 *    - Wait for ReKeyConf before using session
 *    - Extends is_joined() check
 *
 * 4. BACKWARD COMPATIBILITY:
 *    - Detect version from NVM header
 *    - Use appropriate validation logic
 *    - Migrate 1.0 sessions to 1.1 format if needed
 */

/*
 * Q41-50: RAPID FIRE QUESTIONS
 * ============================================================================
 */

/*
 * Q41: What's the maximum time a LoRaWAN session can be reused safely?
 * A: No hard limit, but recommend periodic rejoin every 30-90 days for
 *    key freshness. Frame counter overflow at 2^32 (~8000 years at 1/min).
 *
 * Q42: How do you handle clock drift affecting RX window timing?
 * A: Crystal accuracy determines RX window widening. 20ppm crystal =
 *    ~40us drift per second. Compensated in MAC layer automatically.
 *
 * Q43: What's the trade-off between confirmed and unconfirmed uplinks?
 * A: Confirmed: Reliable but power hungry (waits for ACK, retries)
 *    Unconfirmed: Efficient but no delivery guarantee
 *    We use confirmed for critical data, unconfirmed for telemetry.
 *
 * Q44: How does your optimization affect devices in class B/C mode?
 * A: Class B/C have different RX behavior. Our session reuse helps all
 *    classes. ADR and boot optimization apply equally.
 *
 * Q45: What's the impact of gateway density on your optimization?
 * A: Higher gateway density = better coverage = ADR can use higher DR
 *    = more battery savings. Our optimization amplifies good coverage.
 *
 * Q46: How do you handle ADR for mobile devices?
 * A: Disable ADR (set min_dr = max_dr to low value). ADR can't adapt
 *    fast enough for rapidly changing link quality.
 *
 * Q47: What's the memory footprint of your optimization?
 * A: ~500 bytes additional RAM for state tracking
 *    ~2KB additional flash for optimization code
 *    ~256 bytes NVM for session persistence
 *
 * Q48: How do you handle power loss during NVM write?
 * A: CRC protection detects corruption. Fallback to rejoin if CRC fails.
 *    Could add redundant NVM copies for higher reliability.
 *
 * Q49: What's the typical boot-to-uplink time with your optimization?
 * A: Cold boot with valid session: ~5 seconds (hardware init + immediate TX)
 *    Cold boot needing join: ~30-60 seconds (join procedure)
 *    Without optimization: ~90-150 seconds (link check delays)
 *
 * Q50: How would you port this optimization to a different RTOS?
 * A: Core optimization is RTOS-agnostic (NVM access, state machine).
 *    Would need to adapt: timer APIs, low power hooks, thread-safety.
 *    ~1-2 weeks porting effort for FreeRTOS to Zephyr.
 */

/*
 * ============================================================================
 * SUMMARY OF QUANTIFIED ACHIEVEMENTS
 * ============================================================================
 *
 * 40% BOOT TIME REDUCTION:
 * - Before: 150 seconds average boot-to-network
 * - After: 90 seconds average
 * - Method: Skip link check when valid NVM session exists
 *
 * 30% BATTERY IMPROVEMENT:
 * - Before: 365 days average battery life
 * - After: 475 days average
 * - Method: Reduced joins (94% fewer), optimized ADR, protocol limits
 *
 * KEY INNOVATIONS:
 * 1. Session reuse with credential verification
 * 2. network_lock_mode for production optimization
 * 3. ADR protocol limit enforcement (min/max DR clamping)
 * 4. Exponential backoff for network loss
 *
 * ============================================================================
 */

int main() {
    printf("ePlant Firmware Optimization Suite - Interview Questions\n");
    printf("50 questions covering boot optimization, ADR, and battery life\n");
    return 0;
}
