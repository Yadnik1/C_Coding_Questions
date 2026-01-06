/*
 * ============================================================================
 *     SAFESCAN - HOSPITAL SURGICAL INSTRUMENT RFID TRACKING SYSTEM
 *              50 DEEP INTERVIEW QUESTIONS WITH ANSWERS
 * ============================================================================
 *
 * Resume Claim: "Developed real-time surgical instrument tracking system using
 *               Zephyr RTOS on nRF7002DK with ARM TrustZone security,
 *               multi-I2C RFID readers, and MQTT cloud connectivity"
 *
 * Technologies: Zephyr RTOS, ARM TrustZone, nRF5340, PN532 RFID, MQTT,
 *               WiFi, I2C, Medical Device Safety
 *
 * Problem Solved: Prevents Retained Foreign Objects (RFOs) - ~1,500 cases/year
 *                 in US hospitals
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * ============================================================================
 * SECTION 1: ZEPHYR RTOS FUNDAMENTALS (Questions 1-10)
 * ============================================================================
 */

/*
 * Q1: Why did you choose Zephyr RTOS over FreeRTOS or bare-metal for this
 *     medical device application?
 *
 * QUICK ANSWER:
 * Zephyr provides built-in networking stack (WiFi, MQTT), device tree
 * configuration, native Nordic nRF support, and security features needed
 * for medical devices. FreeRTOS lacks integrated networking; bare-metal
 * lacks scheduling for multiple concurrent tasks.
 *
 * DETAILED EXPLANATION:
 *
 * 1. NORDIC NRF SUPPORT:
 *    - Native support for nRF7002DK (WiFi chip)
 *    - Nordic SDK (NCS) built on Zephyr
 *    - Out-of-box drivers for all peripherals
 *    - TrustZone configuration for nRF5340
 *
 * 2. NETWORKING STACK:
 *    - Full TCP/IP stack included
 *    - MQTT client built-in
 *    - TLS/SSL via mbedTLS
 *    - WiFi driver for nRF7002
 *
 * 3. DEVICE TREE:
 *    - Hardware abstraction via DTS files
 *    - Multi-board support from single codebase
 *    - Pin multiplexing configuration
 *    - I2C bus definition without code changes
 *
 * 4. SAFETY CERTIFICATIONS:
 *    - IEC 61508 SIL certification path
 *    - Memory protection support
 *    - Static analysis friendly
 *    - Medical device compliance ready
 *
 * CODE EXAMPLE - Zephyr Kernel Usage:
 */
// Semaphore for network synchronization
// K_SEM_DEFINE(nw_connected_sem, 0, 1);

void wait_for_network(void) {
    // Block until WiFi connected
    // k_sem_take(&nw_connected_sem, K_FOREVER);
}

void network_connected_callback(void) {
    // Signal that network is ready
    // k_sem_give(&nw_connected_sem);
}

/*
 * INTERVIEW TIP:
 * "For medical devices, Zephyr's path to IEC 61508 certification and integrated
 * security features were crucial. The Nordic NCS SDK made WiFi and Bluetooth
 * integration seamless compared to porting FreeRTOS+lwIP+WPA supplicant."
 */

/*
 * Q2: Explain the Zephyr kernel primitives you used (semaphores, threads, etc.).
 *
 * QUICK ANSWER:
 * Used K_SEM_DEFINE for network sync, k_sleep() for delays, logging subsystem
 * for debug output. Single-threaded main loop with callback-based networking.
 *
 * DETAILED EXPLANATION:
 *
 * 1. SEMAPHORES:
 *    K_SEM_DEFINE(nw_connected_sem, 0, 1)
 *    - Initial count: 0 (network not ready)
 *    - Max count: 1 (binary semaphore)
 *    - k_sem_take() blocks until k_sem_give()
 *
 * 2. SLEEP/DELAY:
 *    k_sleep(K_MSEC(100))
 *    - Non-blocking delay
 *    - Allows other threads to run
 *    - Replaces busy-wait loops
 *
 * 3. LOGGING:
 *    LOG_MODULE_REGISTER(mqtt_app, LOG_LEVEL_DBG)
 *    LOG_INF("Connected to MQTT broker")
 *    - Hierarchical log levels
 *    - Module-specific filtering
 *    - Multiple backends (UART, RTT)
 *
 * CODE EXAMPLE - Primitive Usage:
 */
// LOG_MODULE_REGISTER(safescan, LOG_LEVEL_DBG);

void rfid_scan_loop(void) {
    while (1) {
        // Non-blocking delay (100ms between scans)
        // k_sleep(K_MSEC(100));

        // Log at different levels
        // LOG_DBG("Scanning RFID readers...");
        // LOG_INF("Tag detected: %08X", uid);
        // LOG_ERR("I2C communication failed");
    }
}

/*
 * Q3: How does the Zephyr device tree work and how did you configure
 *     multiple I2C buses?
 *
 * QUICK ANSWER:
 * Device tree (DTS) defines hardware in text format. Configured 3 independent
 * I2C buses (i2c1, i2c2, i2c3) each with a PN532 RFID reader at address 0x24.
 * DTS overlays modify base board configuration.
 *
 * DETAILED EXPLANATION:
 *
 * Device Tree Structure:
 */
/*
 * project_devicetree.dts:
 *
 * &i2c1 {
 *     status = "okay";
 *     pn532_sens1: as5600@24 {
 *         compatible = "ams,as5600";  // Generic I2C device
 *         reg = <0x24>;               // I2C address
 *     };
 * };
 *
 * &i2c2 {
 *     status = "okay";
 *     pn532_sens2: as5600@24 { reg = <0x24>; };
 * };
 *
 * &i2c3 {
 *     status = "okay";
 *     pn532_sens3: as5600@24 { reg = <0x24>; };
 * };
 */

/*
 * WHY MULTIPLE I2C BUSES:
 * - Each PN532 has fixed address 0x24
 * - Can't put two devices with same address on one bus
 * - nRF5340 has multiple TWIM (I2C) peripherals
 * - Separate buses allow parallel operation
 *
 * ACCESSING IN CODE:
 */
// #define I2C1_NODE DT_NODELABEL(i2c1)
// static const struct device *i2c1_dev = DEVICE_DT_GET(I2C1_NODE);

/*
 * Q4: Explain the Zephyr configuration system (Kconfig and prj.conf).
 *
 * QUICK ANSWER:
 * Kconfig defines configurable options with defaults and dependencies.
 * prj.conf sets project-specific values. Enables/disables features like
 * logging, GPIO, networking at compile time.
 *
 * DETAILED EXPLANATION:
 *
 * Key prj.conf Settings:
 *
 * KERNEL CONFIG:
 *   CONFIG_FPU=y                      # Floating point
 *   CONFIG_MAIN_STACK_SIZE=8192       # 8KB main thread stack
 *   CONFIG_HEAP_MEM_POOL_SIZE=8192    # 8KB heap
 *
 * DRIVER CONFIG:
 *   CONFIG_GPIO=y                     # Enable GPIO driver
 *   CONFIG_I2C=y                      # Enable I2C driver
 *   CONFIG_NRFX_TWIM3=y               # Enable nRF TWIM instance 3
 *
 * NETWORKING:
 *   CONFIG_NETWORKING=y               # Enable network stack
 *   CONFIG_WIFI=y                     # Enable WiFi
 *   CONFIG_MQTT_LIB=y                 # Enable MQTT library
 *   CONFIG_NET_SOCKETS=y              # BSD sockets API
 *
 * SECURITY:
 *   CONFIG_MBEDTLS=y                  # Enable TLS
 *   CONFIG_TLS_CREDENTIAL_FILENAMES=y # Certificate file support
 *
 * LOGGING:
 *   CONFIG_LOG=y
 *   CONFIG_LOG_MODE_IMMEDIATE=y       # Don't buffer logs
 *   CONFIG_LOG_BUFFER_SIZE=2048
 *
 * MEMORY IMPACT:
 *   Each CONFIG_X adds to ROM/RAM usage
 *   Disable unused features to save memory
 */

/*
 * Q5: How did you handle WiFi connectivity and what happens on connection loss?
 *
 * QUICK ANSWER:
 * WiFi managed by Zephyr network management API. Connected via SSID/password
 * in config. On disconnect, MQTT_EVT_DISCONNECT handler triggers reconnection
 * after 5-second delay. Semaphore blocks main loop until reconnected.
 *
 * DETAILED EXPLANATION:
 *
 * WiFi Connection Flow:
 * 1. net_mgmt_init_event_callback() registers for events
 * 2. net_mgmt(NET_REQUEST_WIFI_CONNECT) initiates connection
 * 3. NET_EVENT_WIFI_CONNECT_RESULT callback fires
 * 4. k_sem_give(&nw_connected_sem) unblocks main
 *
 * Reconnection Logic:
 */
void mqtt_evt_handler(void* client, void* evt) {
    // switch (evt->type) {
    //     case MQTT_EVT_DISCONNECT:
    //         LOG_WRN("MQTT disconnected, reconnecting in 5s");
    //         k_sleep(K_SECONDS(5));
    //         mqtt_connect(client);
    //         break;
    // }
}

/*
 * RESILIENCE FEATURES:
 * - DNS resolution retried on failure
 * - Socket timeout for stuck connections
 * - Keep-alive prevents silent disconnects (1200s default)
 * - Reconnect doesn't lose RFID state (local variables)
 */

/*
 * Q6: What is the Zephyr logging subsystem and how did you use it for debugging?
 *
 * QUICK ANSWER:
 * Zephyr's LOG_MODULE provides hierarchical logging with levels (DBG, INF, WRN,
 * ERR). Output to UART/RTT. Used LOG_HEXDUMP_DBG for raw RFID data. Filter by
 * module at runtime.
 *
 * DETAILED EXPLANATION:
 *
 * Logging Levels:
 *   LOG_DBG() - Verbose debug (filtered in production)
 *   LOG_INF() - Normal operation info
 *   LOG_WRN() - Recoverable issues
 *   LOG_ERR() - Error conditions
 *
 * Configuration:
 *   CONFIG_LOG_MODE_IMMEDIATE=y  - Don't buffer (real-time debug)
 *   CONFIG_LOG_BUFFER_SIZE=2048  - Buffer size if deferred
 *   CONFIG_LOG_DEFAULT_LEVEL=3   - INF level default
 *
 * Usage Examples:
 */
void log_rfid_read(uint8_t* uid, uint8_t len) {
    // LOG_INF("Tag detected on reader %d", reader_id);
    // LOG_HEXDUMP_DBG(uid, len, "UID bytes:");
}

/*
 * PRODUCTION OPTIMIZATION:
 * - Set LOG_DEFAULT_LEVEL=2 (WRN only)
 * - Reduces code size and CPU usage
 * - Critical errors still logged
 */

/*
 * Q7: How does Zephyr handle GPIO for buttons and buzzer control?
 *
 * QUICK ANSWER:
 * GPIO via device tree bindings and runtime API. Buttons use interrupt
 * callbacks (dk_buttons_init). Buzzer controlled via gpio_pin_set().
 * Device reference obtained via DEVICE_DT_GET.
 *
 * DETAILED EXPLANATION:
 *
 * GPIO Configuration:
 */
#define BUZZER_PIN 30
#define LED1_PIN   7

void init_gpio(void) {
    // const struct device *gpio0 = DEVICE_DT_GET(DT_NODELABEL(gpio0));

    // Configure buzzer as output
    // gpio_pin_configure(gpio0, BUZZER_PIN, GPIO_OUTPUT);

    // Configure LED as output
    // gpio_pin_configure(gpio0, LED1_PIN, GPIO_OUTPUT);
}

void activate_buzzer(bool on) {
    // gpio_pin_set(gpio0, BUZZER_PIN, on ? 1 : 0);
}

/*
 * BUTTON HANDLING (Nordic DK Library):
 *
 * dk_buttons_init(button_handler);
 *
 * void button_handler(uint32_t button_state, uint32_t has_changed) {
 *     if (has_changed & DK_BTN1_MSK) {
 *         // "REGISTER" button - capture starting inventory
 *         memcpy(rfids_start, rfids_current, sizeof(rfids_start));
 *     }
 *     if (has_changed & DK_BTN2_MSK) {
 *         // "CHECK" button - verify all instruments present
 *         compare_inventory();
 *     }
 * }
 */

/*
 * Q8: Explain the Zephyr scheduler and how it differs from FreeRTOS.
 *
 * QUICK ANSWER:
 * Zephyr uses priority-based preemptive scheduling like FreeRTOS. Key
 * differences: cooperative threads (negative priority), unified kernel
 * (no separate queue/semaphore modules), tickless idle, and power management
 * integration.
 *
 * DETAILED EXPLANATION:
 *
 * ZEPHYR SCHEDULER FEATURES:
 *
 * 1. THREAD PRIORITIES:
 *    - Preemptive: 0 to (CONFIG_NUM_PREEMPT_PRIORITIES - 1)
 *    - Cooperative: -1 to -(CONFIG_NUM_COOP_PRIORITIES)
 *    - Lower number = higher priority (like ARM NVIC)
 *
 * 2. TIME SLICING:
 *    - Optional (CONFIG_TIMESLICING=y)
 *    - Only for equal-priority threads
 *    - Configurable time slice duration
 *
 * 3. TICKLESS IDLE:
 *    - No periodic timer interrupt when idle
 *    - Saves power
 *    - Wakes on next scheduled event
 *
 * COMPARISON:
 *   Feature          | Zephyr           | FreeRTOS
 *   -----------------+------------------+-----------------
 *   Priority order   | Lower = higher   | Higher = higher
 *   Cooperative      | Yes (negative)   | Optional
 *   Tickless         | Built-in         | Requires config
 *   Power mgmt       | Integrated       | Separate
 */

/*
 * Q9: How do you handle memory allocation in Zephyr?
 *
 * QUICK ANSWER:
 * Used static allocation where possible. Heap via CONFIG_HEAP_MEM_POOL_SIZE
 * for dynamic needs (MQTT buffers). Zephyr provides k_malloc/k_free with
 * optional memory slabs for fixed-size allocations.
 *
 * DETAILED EXPLANATION:
 *
 * MEMORY STRATEGIES:
 *
 * 1. STATIC ALLOCATION (Preferred):
 *    - Arrays, structs defined at compile time
 *    - No fragmentation
 *    - Deterministic timing
 *
 * 2. HEAP ALLOCATION:
 *    CONFIG_HEAP_MEM_POOL_SIZE=8192
 *    void *buf = k_malloc(256);  // Allocate 256 bytes
 *    k_free(buf);
 *
 * 3. MEMORY POOLS (Fixed-size):
 *    K_MEM_POOL_DEFINE(my_pool, 64, 256, 4, 4);
 *    - 4 blocks of 64, 128, 256 bytes each
 *    - Fast, no fragmentation
 *
 * SAFESCAN USAGE:
 */
// Static buffers for RFID
uint32_t rfids_connected_current[3];  // Current state
uint32_t rfids_connected_start[3];    // Surgery start state
uint32_t rfids_connected_end[3];      // Surgery end state

// Static MQTT buffers
// static uint8_t mqtt_rx_buffer[256];
// static uint8_t mqtt_tx_buffer[256];

/*
 * Q10: How did you debug Zephyr applications on the nRF5340?
 *
 * QUICK ANSWER:
 * Used J-Link RTT for real-time logging, GDB via J-Link for breakpoints,
 * LED toggle for quick checks, and Zephyr shell for runtime inspection.
 * Nordic nRF Connect SDK provides integrated debugging.
 *
 * DETAILED EXPLANATION:
 *
 * DEBUG TOOLS:
 *
 * 1. RTT (Real-Time Transfer):
 *    - Fast logging via debug probe
 *    - No UART needed
 *    - Bi-directional (shell)
 *    CONFIG_USE_SEGGER_RTT=y
 *
 * 2. GDB DEBUGGING:
 *    - west debug (launches GDB)
 *    - Breakpoints, step, watch
 *    - Memory inspection
 *
 * 3. ZEPHYR SHELL:
 *    CONFIG_SHELL=y
 *    - Runtime commands (device list, thread info)
 *    - Custom commands for testing
 *
 * 4. GPIO DEBUGGING:
 *    - Toggle LED on state changes
 *    - Oscilloscope for timing
 *
 * 5. MEMFAULT (Crash Analytics):
 *    CONFIG_MEMFAULT=y
 *    - Crash dumps to cloud
 *    - Stack traces, register state
 */

/*
 * ============================================================================
 * SECTION 2: ARM TRUSTZONE SECURITY (Questions 11-20)
 * ============================================================================
 */

/*
 * Q11: What is ARM TrustZone and why is it important for medical devices?
 *
 * QUICK ANSWER:
 * TrustZone creates hardware-enforced isolation between Secure and Non-Secure
 * worlds. Medical devices need it for: protecting cryptographic keys, ensuring
 * firmware integrity, isolating safety-critical code from application bugs.
 *
 * DETAILED EXPLANATION:
 *
 * TRUSTZONE ARCHITECTURE:
 *
 *   +-------------------+     +-------------------+
 *   |   SECURE WORLD    |     | NON-SECURE WORLD  |
 *   |-------------------|     |-------------------|
 *   | Crypto Keys       |     | Application Code  |
 *   | Boot Verification |     | WiFi Stack        |
 *   | Secure Storage    |     | MQTT Client       |
 *   | Auth Functions    |     | RFID Reading      |
 *   +-------------------+     +-------------------+
 *           |                          |
 *           +--------+  +--------------+
 *                    |  |
 *              +-----v--v-----+
 *              | TrustZone    |
 *              | Controller   |
 *              | (Hardware)   |
 *              +--------------+
 *
 * WHY FOR MEDICAL DEVICES:
 *
 * 1. REGULATORY COMPLIANCE:
 *    - FDA 510(k) requires security controls
 *    - IEC 62443 cybersecurity standard
 *    - HIPAA data protection
 *
 * 2. CRYPTOGRAPHIC PROTECTION:
 *    - Keys never leave secure world
 *    - Secure boot verification
 *    - Encrypted patient data
 *
 * 3. FAULT ISOLATION:
 *    - Application bug can't corrupt safety code
 *    - Memory protection between worlds
 *    - Secure firmware update
 */

/*
 * Q12: How did you configure TrustZone partitioning on the nRF5340?
 *
 * QUICK ANSWER:
 * Device tree defines secure/non-secure SRAM partitions. Nordic SPM (Secure
 * Partition Manager) runs in secure world, application in non-secure.
 * NSC (Non-Secure Callable) functions provide secure services.
 *
 * DETAILED EXPLANATION:
 *
 * Device Tree Configuration:
 */
/*
 * project_devicetree.dts:
 *
 * chosen {
 *     zephyr,sram-secure-partition = &sram0_s;    // Secure SRAM
 *     zephyr,sram-non-secure-partition = &sram0_ns; // Non-Secure SRAM
 * };
 *
 * MEMORY MAP (nRF5340):
 *
 * Address         | Size    | Region
 * ----------------+---------+------------------
 * 0x00000000      | 256KB   | Secure Flash (SPM)
 * 0x00040000      | 768KB   | Non-Secure Flash (App)
 * 0x20000000      | 64KB    | Secure SRAM
 * 0x20010000      | 192KB   | Non-Secure SRAM
 *
 * BUILD CONFIGURATION:
 *   CONFIG_TRUSTED_EXECUTION_NONSECURE=y
 *   CONFIG_BUILD_WITH_TFM=y  // TF-M secure firmware
 */

/*
 * Q13: What security requirements did TrustZone help you meet?
 *
 * QUICK ANSWER:
 * SEC01: Tag UID immutability (secure storage), SEC03: Authentication in
 * secure world, SEC05: Firmware protection via secure boot. TrustZone
 * ensures these can't be bypassed by application bugs.
 *
 * DETAILED EXPLANATION:
 *
 * SECURITY REQUIREMENT MAPPING:
 *
 * SEC01 - RFID Tag Immutability:
 *   - UID stored in secure world
 *   - Non-secure code can read but not modify
 *   - Prevents spoofing attack
 *
 * SEC03 - Device Authentication:
 *   - Auth code runs in secure world
 *   - Keys stored in secure storage
 *   - Non-secure app can't extract keys
 *
 * SEC05 - Firmware Protection:
 *   - Secure boot verifies signatures
 *   - Only signed firmware executes
 *   - Prevents malicious firmware injection
 *
 * SEC04 - WiFi Certificate Auth:
 *   - TLS keys in secure storage
 *   - X.509 certificates provisioned securely
 *   - MQTT over TLS uses secure credentials
 */

/*
 * Q14: How does the Secure Partition Manager (SPM) work?
 *
 * QUICK ANSWER:
 * SPM is Nordic's secure firmware that configures TrustZone at boot.
 * Defines memory regions, sets up SAU (Security Attribution Unit), provides
 * NSC functions for secure services, then jumps to non-secure application.
 *
 * DETAILED EXPLANATION:
 *
 * SPM BOOT SEQUENCE:
 *
 * 1. HARDWARE RESET:
 *    - CPU starts in Secure state
 *    - SPM executes first
 *
 * 2. TRUSTZONE CONFIGURATION:
 *    - SAU regions defined (Secure/Non-Secure)
 *    - MPC (Memory Protection Controller) setup
 *    - PPC (Peripheral Protection Controller) setup
 *
 * 3. SECURE BOOT (Optional):
 *    - Verify application signature
 *    - Check firmware version (anti-rollback)
 *
 * 4. JUMP TO NON-SECURE:
 *    - Set VTOR to NS vector table
 *    - Clear sensitive registers
 *    - Branch to NS reset handler
 *
 * NSC (Non-Secure Callable) Functions:
 */
// Secure function callable from Non-Secure world
// __attribute__((cmse_nonsecure_entry))
// int secure_get_random(uint8_t *buf, size_t len) {
//     // Generate random in secure world
//     return hw_random_get(buf, len);
// }

/*
 * Q15: What is the difference between Secure and Non-Secure peripheral access?
 *
 * QUICK ANSWER:
 * Peripherals assigned to Secure or Non-Secure via PPC (Peripheral Protection
 * Controller). Secure peripherals (crypto engine) only accessible from secure
 * code. Non-Secure peripherals (I2C, GPIO) accessible from application.
 *
 * DETAILED EXPLANATION:
 *
 * PERIPHERAL CLASSIFICATION:
 *
 * SECURE PERIPHERALS:
 * - Crypto accelerator (AES, SHA)
 * - True Random Number Generator
 * - Secure key storage
 * - Flash controller (for secure regions)
 *
 * NON-SECURE PERIPHERALS:
 * - I2C buses (RFID readers)
 * - UART (debug)
 * - GPIO (buttons, LEDs, buzzer)
 * - WiFi radio
 *
 * CONFIGURATION (Device Tree):
 *
 * &i2c1 {
 *     status = "okay";
 *     // Implicitly non-secure
 * };
 *
 * // In SPM configuration
 * spm: partition@0 {
 *     // Crypto peripheral is secure
 * };
 */

/*
 * Q16: How do you handle secure storage for WiFi credentials and certificates?
 *
 * QUICK ANSWER:
 * Credentials stored in secure flash partition via TLS credential API.
 * Provisioned during manufacturing or via secure channel. Non-secure code
 * references credentials by tag ID, never sees raw keys.
 *
 * DETAILED EXPLANATION:
 *
 * CREDENTIAL STORAGE:
 *
 * 1. PROVISION (Manufacturing):
 *    tls_credential_add(SEC_TAG, TLS_CREDENTIAL_CA_CERTIFICATE, ca_cert);
 *    tls_credential_add(SEC_TAG, TLS_CREDENTIAL_PRIVATE_KEY, priv_key);
 *
 * 2. USE IN CODE (Non-Secure):
 *    sec_tag_t sec_tag_list[] = { SEC_TAG };
 *    // MQTT helper uses sec_tag, never sees actual cert
 *
 * 3. PROTECTION:
 *    - Certificates stored in secure flash
 *    - Read-protected from non-secure code
 *    - TLS operations done by secure-side mbedTLS
 *
 * CONFIGURATION:
 *   CONFIG_MQTT_HELPER_SEC_TAG=201
 *   CONFIG_TLS_CREDENTIAL_FILENAMES=y
 *   CONFIG_MBEDTLS_SSL_MAX_CONTENT_LEN=8192
 */

/*
 * Q17: What happens if non-secure code tries to access secure memory?
 *
 * QUICK ANSWER:
 * Hardware generates SecureFault exception. CPU switches to secure handler.
 * Typically logged and system reset. Prevents information leakage and ensures
 * security boundary integrity.
 *
 * DETAILED EXPLANATION:
 *
 * SECUREFAULT MECHANISM:
 *
 * 1. MEMORY ACCESS ATTEMPT:
 *    Non-Secure code: LDR R0, [0x20000000]  // Secure address
 *
 * 2. SAU CHECK:
 *    - SAU marks 0x20000000 as Secure
 *    - Access from NS world = violation
 *
 * 3. EXCEPTION:
 *    - SecureFault exception (IRQ -14)
 *    - CPU switches to Secure state
 *    - SecureFault_Handler() executes
 *
 * 4. HANDLING:
 */
void SecureFault_Handler(void) {
    // Log the violation
    // uint32_t fault_addr = SCB->SFAR;
    // LOG_ERR("SecureFault at %08X", fault_addr);

    // Reset system (security breach response)
    // NVIC_SystemReset();
}

/*
 * DEFENSE IN DEPTH:
 * - Log for forensics
 * - Reset prevents exploitation
 * - Alert sent before reset (if possible)
 */

/*
 * Q18: How does TrustZone protect against firmware tampering?
 *
 * QUICK ANSWER:
 * Secure boot in SPM verifies application signature before execution.
 * Public key stored in OTP (One-Time Programmable) memory. Unsigned or
 * modified firmware rejected. Anti-rollback prevents downgrade attacks.
 *
 * DETAILED EXPLANATION:
 *
 * SECURE BOOT CHAIN:
 *
 * 1. IMMUTABLE BOOTLOADER:
 *    - ROM bootloader (factory programmed)
 *    - Verifies SPM signature
 *
 * 2. SPM VERIFICATION:
 *    - SPM verifies application signature
 *    - Public key in secure flash/OTP
 *    - ECDSA P-256 or RSA-2048
 *
 * 3. VERSION CHECK:
 *    - Current version stored in secure storage
 *    - New firmware must be >= current
 *    - Prevents rollback to vulnerable version
 *
 * SIGNATURE VERIFICATION:
 */
bool verify_firmware(uint8_t* firmware, size_t len, uint8_t* signature) {
    // Secure world function
    // return ecdsa_verify(firmware, len, signature, public_key);
    return true;
}

/*
 * Q19: What are NSC (Non-Secure Callable) functions and how did you use them?
 *
 * QUICK ANSWER:
 * NSC functions are secure-world functions callable from non-secure code.
 * Marked with special attribute and placed in NSC region. Used for secure
 * services: random number generation, credential access, signature verification.
 *
 * DETAILED EXPLANATION:
 *
 * NSC MECHANISM:
 *
 * 1. FUNCTION DECLARATION:
 */
// __attribute__((cmse_nonsecure_entry))
// void secure_service(void);

/*
 * 2. MEMORY REGION:
 *    - NSC region between Secure and Non-Secure
 *    - Contains veneer instructions
 *    - SG (Secure Gateway) instruction validates entry
 *
 * 3. CALL FLOW:
 *    NS code → NSC veneer → Secure function → Return to NS
 *
 * 4. PARAMETER VALIDATION:
 *    - Pointers checked to be in NS memory
 *    - Prevents NS code from reading Secure memory via pointer
 *
 * EXAMPLE IMPLEMENTATION:
 */
// __attribute__((cmse_nonsecure_entry))
// int nsc_encrypt_data(uint8_t* ns_buffer, size_t len) {
//     // Validate pointer is in NS memory
//     if (cmse_check_address_range(ns_buffer, len, CMSE_NONSECURE) == NULL) {
//         return -EINVAL;  // Invalid pointer
//     }
//     // Perform secure operation
//     return aes_encrypt(ns_buffer, len, secure_key);
// }

/*
 * Q20: How would you extend TrustZone usage for future HIPAA compliance?
 *
 * QUICK ANSWER:
 * Store patient identifiers in secure world. Encrypt RFID-to-patient mapping.
 * Audit log writes from secure world only. Secure erase of patient data.
 * All PHI (Protected Health Information) processing in secure partition.
 *
 * DETAILED EXPLANATION:
 *
 * HIPAA TECHNICAL SAFEGUARDS:
 *
 * 1. ACCESS CONTROL (164.312(a)):
 *    - Authentication in secure world
 *    - Role-based access via NSC APIs
 *    - Automatic logoff timeout
 *
 * 2. AUDIT CONTROLS (164.312(b)):
 *    - Tamper-proof audit log in secure storage
 *    - Only secure code can write logs
 *    - Cryptographic integrity (HMAC)
 *
 * 3. TRANSMISSION SECURITY (164.312(e)):
 *    - TLS keys in secure storage
 *    - Certificate pinning
 *    - Secure channel establishment
 *
 * 4. ENCRYPTION (164.312(a)(2)(iv)):
 *    - PHI encrypted at rest
 *    - Secure key management
 *    - Key never leaves secure world
 */

/*
 * ============================================================================
 * SECTION 3: RFID AND PN532 INTEGRATION (Questions 21-30)
 * ============================================================================
 */

/*
 * Q21: Explain the PN532 RFID reader architecture and why you chose it.
 *
 * QUICK ANSWER:
 * PN532 is NXP's 13.56MHz NFC controller supporting ISO14443A/B, MIFARE,
 * and FeliCa. Chose it for: wide protocol support, I2C interface, proven
 * library ecosystem, and 5cm read range suitable for close-proximity scanning.
 *
 * DETAILED EXPLANATION:
 *
 * PN532 FEATURES:
 *
 * 1. RF PROTOCOLS:
 *    - ISO14443A (MIFARE Classic, Ultralight)
 *    - ISO14443B
 *    - FeliCa (212/424 kbps)
 *    - ISO18092 (peer-to-peer)
 *
 * 2. INTERFACES:
 *    - I2C (used in our design)
 *    - SPI
 *    - HSU (High Speed UART)
 *
 * 3. PERFORMANCE:
 *    - Detection range: 5cm
 *    - Multiple tag detection (limited)
 *    - Fast anti-collision
 *
 * WHY FOR SURGICAL INSTRUMENTS:
 * - MIFARE Classic tags are cheap ($0.10)
 * - Unique 4-byte UID per tag
 * - Close range prevents cross-reads
 * - Well-documented protocol
 */

/*
 * Q22: Describe the I2C communication protocol with the PN532.
 *
 * QUICK ANSWER:
 * PN532 uses frame-based I2C at address 0x24. Frame format: preamble (0x00),
 * start codes (0x00FF), length, LCS, TFI, data, DCS, postamble.
 * ACK/NACK handshaking after each command.
 *
 * DETAILED EXPLANATION:
 *
 * FRAME FORMAT:
 *
 * TX (Host to PN532):
 * [0x00] [0x00] [0xFF] [LEN] [LCS] [TFI=0xD4] [CMD] [DATA...] [DCS] [0x00]
 *
 * RX (PN532 to Host):
 * [0x00] [0x00] [0xFF] [LEN] [LCS] [TFI=0xD5] [CMD+1] [DATA...] [DCS] [0x00]
 *
 * Where:
 * - LEN: Length of TFI + CMD + DATA
 * - LCS: Length Checksum (LEN + LCS = 0x00)
 * - TFI: Frame Identifier (0xD4 = host-to-PN532)
 * - CMD: Command code
 * - DCS: Data Checksum (sum of TFI..DATA + DCS = 0x00)
 *
 * ACK FRAME:
 * [0x00] [0x00] [0xFF] [0x00] [0xFF] [0x00]
 *
 * CODE EXAMPLE:
 */
bool pn532_send_command(uint8_t cmd, uint8_t* data, uint8_t data_len) {
    uint8_t frame[64];
    uint8_t len = 2 + data_len;  // TFI + CMD + DATA

    frame[0] = 0x00;  // Preamble
    frame[1] = 0x00;  // Start code 1
    frame[2] = 0xFF;  // Start code 2
    frame[3] = len;   // Length
    frame[4] = (~len + 1) & 0xFF;  // Length checksum

    frame[5] = 0xD4;  // TFI (host to PN532)
    frame[6] = cmd;   // Command

    uint8_t sum = 0xD4 + cmd;
    for (int i = 0; i < data_len; i++) {
        frame[7 + i] = data[i];
        sum += data[i];
    }

    frame[7 + data_len] = (~sum + 1) & 0xFF;  // Data checksum
    frame[8 + data_len] = 0x00;  // Postamble

    // i2c_write(i2c_dev, frame, 9 + data_len, PN532_I2C_ADDR);
    return true;
}

/*
 * Q23: How do you detect and read multiple RFID tags?
 *
 * QUICK ANSWER:
 * PN532's InListPassiveTarget command with MaxTg=2 detects up to 2 tags.
 * For more tags, poll repeatedly with different timing. Our design uses
 * 3 readers to cover larger area and more tags simultaneously.
 *
 * DETAILED EXPLANATION:
 *
 * SINGLE TAG DETECTION:
 *
 * Command: InListPassiveTarget (0x4A)
 * Parameters:
 *   MaxTg: 1 (detect one tag)
 *   BrTy: 0x00 (ISO14443A, 106kbps)
 *
 * Response contains:
 *   Tg: Target number (1)
 *   SENS_RES: 2 bytes
 *   SEL_RES: 1 byte
 *   NFCIDLength: 4 or 7 bytes
 *   NFCID: The UID we want!
 *
 * MULTI-TAG LIMITATION:
 * - PN532 hardware supports 2 simultaneous
 * - Anti-collision handles multiple tags
 * - But only returns first 2 found
 *
 * OUR SOLUTION - 3 READERS:
 */
uint32_t reader_uids[3];  // One UID per reader

void scan_all_readers(void) {
    for (int i = 0; i < 3; i++) {
        uint8_t uid[7];
        uint8_t uid_len;

        if (pn532_read_passive_target(i, uid, &uid_len)) {
            reader_uids[i] = (uid[0] << 24) | (uid[1] << 16) |
                             (uid[2] << 8) | uid[3];
        } else {
            reader_uids[i] = 0;  // No tag detected
        }
    }
}

/*
 * Q24: How did you handle the fixed I2C address problem with multiple PN532s?
 *
 * QUICK ANSWER:
 * PN532 has fixed address 0x24, can't be changed. Used nRF5340's multiple
 * TWIM (I2C) peripherals - i2c1, i2c2, i2c3 each with one PN532. Device
 * tree configures three independent buses.
 *
 * DETAILED EXPLANATION:
 *
 * THE PROBLEM:
 * - All PN532 modules respond to 0x24
 * - I2C protocol: only one device per address per bus
 * - Can't put 3 PN532s on same bus
 *
 * ALTERNATIVE SOLUTIONS CONSIDERED:
 *
 * 1. I2C MULTIPLEXER (TCA9548A):
 *    - Single MCU I2C bus
 *    - Mux selects which PN532 is active
 *    - Adds component, complexity
 *
 * 2. SPI INTERFACE:
 *    - PN532 supports SPI
 *    - Each PN532 has separate CS pin
 *    - Tried but had reliability issues
 *
 * 3. MULTIPLE I2C BUSES (Chosen):
 *    - nRF5340 has 4 TWIM peripherals
 *    - Each PN532 on its own bus
 *    - Parallel operation possible
 *    - Clean device tree configuration
 *
 * DEVICE TREE SOLUTION:
 *
 * &i2c1 { pn532_1: pn532@24 { reg = <0x24>; }; };
 * &i2c2 { pn532_2: pn532@24 { reg = <0x24>; }; };
 * &i2c3 { pn532_3: pn532@24 { reg = <0x24>; }; };
 */

/*
 * Q25: Explain the surgical workflow integration - start scan vs end scan.
 *
 * QUICK ANSWER:
 * Button 1 (REGISTER): Captures current tag UIDs as starting inventory.
 * Button 2 (CHECK): Captures ending inventory and compares. Mismatch
 * triggers buzzer alert. Prevents retained foreign objects (RFOs).
 *
 * DETAILED EXPLANATION:
 *
 * WORKFLOW:
 *
 * 1. PRE-SURGERY SETUP:
 *    - Place all instruments on RFID readers
 *    - Press REGISTER button
 *    - System captures: rfids_connected_start[]
 *    - LED confirms registration
 *
 * 2. DURING SURGERY:
 *    - Instruments used freely
 *    - System continuously monitors (optional display)
 *
 * 3. POST-SURGERY CHECK:
 *    - Return all instruments to readers
 *    - Press CHECK button
 *    - System captures: rfids_connected_end[]
 *    - Compares with rfids_connected_start[]
 *
 * 4. RESULT:
 *    - Match: Green LED, all clear
 *    - Mismatch: Buzzer alarm, red LED
 *    - MQTT publishes status for logging
 *
 * CODE IMPLEMENTATION:
 */
uint32_t rfids_start[3];
uint32_t rfids_end[3];
uint32_t rfids_current[3];

void on_register_button(void) {
    scan_all_readers();
    for (int i = 0; i < 3; i++) {
        rfids_start[i] = rfids_current[i];
    }
    // LOG_INF("Registered %d instruments", count_tags(rfids_start));
    // publish_mqtt("REGISTER", rfids_start);
}

void on_check_button(void) {
    scan_all_readers();
    for (int i = 0; i < 3; i++) {
        rfids_end[i] = rfids_current[i];
    }

    if (memcmp(rfids_end, rfids_start, sizeof(rfids_start)) != 0) {
        // ALERT! Instrument missing
        // gpio_pin_set(gpio0, BUZZER_PIN, 1);
        // LOG_ERR("MISMATCH! Instrument may be retained");
        // publish_mqtt("ALERT", rfids_end);
    } else {
        // All clear
        // LOG_INF("All instruments accounted for");
        // publish_mqtt("CLEAR", rfids_end);
    }
}

/*
 * Q26: How do you handle RFID read failures or intermittent reads?
 *
 * QUICK ANSWER:
 * Retry logic with 3 attempts per reader. Timeout handling (100ms per read).
 * Debouncing: tag must be present for 2 consecutive reads to be counted.
 * I2C bus recovery on communication errors.
 *
 * DETAILED EXPLANATION:
 *
 * RELIABILITY MEASURES:
 *
 * 1. RETRY LOGIC:
 */
#define MAX_RETRIES 3
#define READ_TIMEOUT_MS 100

bool reliable_tag_read(int reader, uint8_t* uid) {
    for (int retry = 0; retry < MAX_RETRIES; retry++) {
        if (pn532_read_with_timeout(reader, uid, READ_TIMEOUT_MS)) {
            return true;
        }
        // k_sleep(K_MSEC(10));  // Brief delay before retry
    }
    return false;
}

/*
 * 2. DEBOUNCING:
 *    - Prevent flickering reads
 *    - Tag must be stable for N samples
 */
typedef struct {
    uint32_t uid;
    uint8_t stable_count;
} tag_state_t;

tag_state_t tag_states[3];
#define STABLE_THRESHOLD 2

void debounce_tag(int reader, uint32_t new_uid) {
    if (new_uid == tag_states[reader].uid) {
        if (tag_states[reader].stable_count < STABLE_THRESHOLD) {
            tag_states[reader].stable_count++;
        }
    } else {
        tag_states[reader].uid = new_uid;
        tag_states[reader].stable_count = 1;
    }
}

bool is_tag_stable(int reader) {
    return tag_states[reader].stable_count >= STABLE_THRESHOLD;
}

/*
 * 3. I2C BUS RECOVERY:
 *    - Detect stuck bus (SDA held low)
 *    - Toggle SCL to release
 *    - Reinitialize I2C peripheral
 */

/*
 * Q27: What is the detection range and how did you verify it meets requirements?
 *
 * QUICK ANSWER:
 * PN532 with standard antenna: 5cm range. Requirement HRS01: 5cm minimum.
 * Verified with test setup: marked distances, 100 read attempts per distance,
 * 99%+ success at 5cm, drops off sharply at 7cm+.
 *
 * DETAILED EXPLANATION:
 *
 * RANGE FACTORS:
 *
 * 1. ANTENNA SIZE:
 *    - Larger antenna = longer range
 *    - PN532 breakout boards vary
 *    - Our boards: ~50mm x 50mm antenna
 *
 * 2. TAG TYPE:
 *    - MIFARE Classic 1K: Good range
 *    - Smaller tags (stickers): Reduced range
 *    - Metal interference: Reduced range
 *
 * 3. ENVIRONMENTAL:
 *    - Metal surfaces reduce range
 *    - Other RF sources (interference)
 *    - Tag orientation affects coupling
 *
 * TEST RESULTS:
 *
 * Distance | Success Rate | Notes
 * ---------|--------------|-------
 * 1 cm     | 100%         | Optimal
 * 3 cm     | 100%         | Good
 * 5 cm     | 99%          | Meets spec
 * 7 cm     | 85%          | Degraded
 * 10 cm    | 20%          | Unreliable
 *
 * VERIFICATION PROCEDURE:
 * 1. Mount reader on test fixture
 * 2. Mark distances with ruler
 * 3. 100 read attempts per distance
 * 4. Calculate success rate
 * 5. Document in test report
 */

/*
 * Q28: How do you handle the case where a tag is removed and a different
 *      tag is placed (swap detection)?
 *
 * QUICK ANSWER:
 * Each tag has unique UID. System tracks which specific UIDs were registered.
 * Swap detected: end set has different UID than start set, even if same count.
 * MQTT log shows exact UID mismatch for audit trail.
 *
 * DETAILED EXPLANATION:
 *
 * SWAP DETECTION:
 */
void detect_swap(void) {
    // Compare UIDs, not just counts
    for (int i = 0; i < 3; i++) {
        if (rfids_start[i] != 0 && rfids_end[i] != rfids_start[i]) {
            // UID changed - either removed or swapped
            // LOG_WRN("Reader %d: Expected %08X, got %08X",
            //         i, rfids_start[i], rfids_end[i]);

            if (rfids_end[i] != 0) {
                // Different tag present = SWAP
                // LOG_ERR("SWAP DETECTED on reader %d", i);
            } else {
                // No tag present = MISSING
                // LOG_ERR("MISSING instrument on reader %d", i);
            }
        }
    }
}

/*
 * AUDIT LOGGING:
 *
 * Start: [Reader0: 0x12345678, Reader1: 0xABCDEF01, Reader2: 0x00000000]
 * End:   [Reader0: 0x12345678, Reader1: 0x99999999, Reader2: 0x00000000]
 * Alert: Reader1 UID mismatch (expected ABCDEF01, got 99999999)
 *
 * This catches:
 * - Missing instrument (UID = 0 at end)
 * - Swapped instrument (different UID)
 * - Extra instrument (new UID not in start)
 */

/*
 * Q29: How does the SAMConfiguration command work?
 *
 * QUICK ANSWER:
 * SAM (Security Access Module) Configuration sets PN532 operating mode.
 * For basic tag reading, set Normal mode with timeout. Disables SAM chip
 * features we don't need (e.g., secure element).
 *
 * DETAILED EXPLANATION:
 *
 * SAMCONFIGURATION COMMAND (0x14):
 *
 * Parameters:
 *   Mode: 0x01 = Normal mode (no SAM)
 *   Timeout: 0x14 = 20 × 50ms = 1 second
 *   IRQ: 0x01 = Use IRQ pin (or 0x00 for polling)
 *
 * CODE EXAMPLE:
 */
bool pn532_sam_config(void) {
    uint8_t cmd[] = {
        0x14,  // SAMConfiguration command
        0x01,  // Mode: Normal (no SAM)
        0x14,  // Timeout: 20 × 50ms = 1s
        0x01   // Use IRQ
    };

    // Send command and check for ACK
    return pn532_send_command_check_ack(cmd, sizeof(cmd));
}

/*
 * WHY NORMAL MODE:
 * - SAM features for NFC payments (not needed)
 * - Simpler operation
 * - Lower power
 * - Faster response
 */

/*
 * Q30: What MIFARE commands did you use beyond UID reading?
 *
 * QUICK ANSWER:
 * For this MVP, only UID reading (InListPassiveTarget). Future features could
 * use: Authentication (0x60/0x61), Read (0x30), Write (0xA0) for storing
 * instrument metadata on the tag itself.
 *
 * DETAILED EXPLANATION:
 *
 * MIFARE CLASSIC COMMAND SET:
 *
 * 1. AUTHENTICATION (0x60 Key A, 0x61 Key B):
 *    - Required before read/write
 *    - Uses 6-byte key
 *    - Per-sector authentication
 *
 * 2. READ (0x30):
 *    - Read 16-byte block
 *    - After authentication
 *
 * 3. WRITE (0xA0):
 *    - Write 16-byte block
 *    - After authentication
 *
 * FUTURE ENHANCEMENT - Store Metadata:
 */
typedef struct {
    char instrument_name[12];  // e.g., "Scalpel #3"
    uint32_t last_sterilized;  // Unix timestamp
} instrument_data_t;

void write_instrument_data(uint32_t uid, instrument_data_t* data) {
    // 1. Select tag by UID
    // 2. Authenticate with Key A
    // 3. Write data to block 4 (sector 1)
    // 4. Read back to verify
}

/*
 * BENEFITS:
 * - Instrument info stored on tag, not database lookup
 * - Works offline
 * - Self-documenting inventory
 */

/*
 * ============================================================================
 * SECTION 4: MQTT AND CLOUD CONNECTIVITY (Questions 31-40)
 * ============================================================================
 */

/*
 * Q31: Explain your MQTT architecture and why you chose HiveMQ broker.
 *
 * QUICK ANSWER:
 * MQTT for lightweight pub/sub messaging. HiveMQ public broker for prototyping
 * (free, no setup). Production would use AWS IoT Core. Topics: publish tag
 * events, subscribe for commands.
 *
 * DETAILED EXPLANATION:
 *
 * MQTT ARCHITECTURE:
 *
 * SafeScan Device --> WiFi --> Internet --> HiveMQ Broker
 *                                              |
 *                     Dashboard <--------------+
 *                     (Node-RED / Web App)
 *
 * TOPIC STRUCTURE:
 *   nrf7002dk/board/publish - Device publishes tag events
 *   nrf7002dk/board/command - Dashboard sends commands (future)
 *
 * WHY HIVEMQ:
 * - Free public broker for development
 * - No authentication required
 * - Standard MQTT 3.1.1
 * - Easy testing with MQTT Explorer
 *
 * PRODUCTION CONSIDERATIONS:
 * - AWS IoT Core for production
 * - X.509 certificate authentication
 * - Encrypted TLS transport
 * - Device shadow for state sync
 */

/*
 * Q32: How did you implement MQTT connection and reconnection logic?
 *
 * QUICK ANSWER:
 * Socket-based MQTT client with event handler. On MQTT_EVT_DISCONNECT,
 * wait 5 seconds and retry. Keep-alive (1200s) prevents silent drops.
 * Exponential backoff for repeated failures.
 *
 * DETAILED EXPLANATION:
 *
 * CONNECTION FLOW:
 *
 * 1. INITIALIZE:
 *    mqtt_client_init(&client);
 *    broker.sin_family = AF_INET;
 *    broker.sin_port = htons(1883);
 *
 * 2. CONNECT:
 *    mqtt_connect(&client);
 *    // Blocks until connected or timeout
 *
 * 3. EVENT LOOP:
 *    while (1) {
 *        mqtt_input(&client);   // Process incoming
 *        mqtt_live(&client);    // Keep-alive ping
 *        // Application logic
 *    }
 *
 * 4. RECONNECT ON DISCONNECT:
 */
void mqtt_event_handler(void* c, void* evt) {
    static int retry_count = 0;

    // switch (evt->type) {
    //     case MQTT_EVT_CONNACK:
    //         LOG_INF("MQTT connected");
    //         retry_count = 0;
    //         break;
    //
    //     case MQTT_EVT_DISCONNECT:
    //         LOG_WRN("MQTT disconnected");
    //         int delay = min(5 * (1 << retry_count), 60);  // Exponential backoff, max 60s
    //         k_sleep(K_SECONDS(delay));
    //         retry_count++;
    //         mqtt_connect(c);
    //         break;
    // }
}

/*
 * Q33: What message format did you use for MQTT payloads?
 *
 * QUICK ANSWER:
 * Simple text format for MVP: "Reader X :: Tag detected! UID : XXXXXXXX".
 * Production would use JSON for structured data: {"reader": 0, "uid": "...",
 * "timestamp": ..., "event": "detected"}.
 *
 * DETAILED EXPLANATION:
 *
 * CURRENT FORMAT (MVP):
 *   "Reader 0 :: Tag detected! UID : 12345678"
 *   "Reader 1 :: No tag"
 *   "ALERT :: Instrument mismatch detected"
 *
 * PRODUCTION JSON FORMAT:
 */
void publish_event_json(int reader, uint32_t uid, const char* event) {
    char json[256];
    // snprintf(json, sizeof(json),
    //     "{"
    //     "\"device_id\":\"safescan-001\","
    //     "\"reader\":%d,"
    //     "\"uid\":\"%08X\","
    //     "\"event\":\"%s\","
    //     "\"timestamp\":%llu"
    //     "}",
    //     reader, uid, event, get_unix_time());

    // mqtt_publish(&client, TOPIC, json, strlen(json), MQTT_QOS_1);
}

/*
 * PAYLOAD CONSIDERATIONS:
 * - Keep small (bandwidth)
 * - Include timestamp (ordering)
 * - Device ID (multi-device)
 * - Event type (filtering)
 * - UID (inventory tracking)
 */

/*
 * Q34: How do you handle network latency and message queuing?
 *
 * QUICK ANSWER:
 * QoS 1 for reliable delivery (at least once). Local queue for offline
 * buffering. Publish non-blocking, process ACKs in event handler.
 * Critical alerts use QoS 2 (exactly once).
 *
 * DETAILED EXPLANATION:
 *
 * MQTT QOS LEVELS:
 *
 * QoS 0 (At most once):
 *   - Fire and forget
 *   - No ACK
 *   - May lose messages
 *   - Use for: Periodic status updates
 *
 * QoS 1 (At least once):
 *   - Publisher retries until ACK
 *   - May duplicate
 *   - Use for: Tag detection events
 *
 * QoS 2 (Exactly once):
 *   - 4-way handshake
 *   - Guaranteed single delivery
 *   - Use for: Alert notifications
 *
 * OFFLINE BUFFERING:
 */
#define OFFLINE_QUEUE_SIZE 32

typedef struct {
    char payload[128];
    uint8_t qos;
} queued_message_t;

queued_message_t offline_queue[OFFLINE_QUEUE_SIZE];
int queue_head = 0, queue_tail = 0;

void publish_or_queue(const char* payload, uint8_t qos) {
    if (mqtt_connected) {
        // mqtt_publish(&client, TOPIC, payload, strlen(payload), qos);
    } else {
        // Queue for later
        // strncpy(offline_queue[queue_head].payload, payload, 127);
        // offline_queue[queue_head].qos = qos;
        // queue_head = (queue_head + 1) % OFFLINE_QUEUE_SIZE;
    }
}

/*
 * Q35: How would you implement TLS for secure MQTT communication?
 *
 * QUICK ANSWER:
 * Enable CONFIG_MQTT_LIB_TLS and CONFIG_MBEDTLS. Provision CA certificate
 * and client cert/key via tls_credential_add(). Connect to port 8883.
 * Zephyr's socket layer handles TLS handshake.
 *
 * DETAILED EXPLANATION:
 *
 * TLS CONFIGURATION:
 *
 * prj.conf:
 *   CONFIG_MQTT_LIB_TLS=y
 *   CONFIG_MBEDTLS=y
 *   CONFIG_MBEDTLS_SSL_MAX_CONTENT_LEN=8192
 *   CONFIG_NET_SOCKETS_SOCKOPT_TLS=y
 *
 * CERTIFICATE PROVISIONING:
 */
#define TLS_SEC_TAG 201

void provision_certificates(void) {
    // CA Certificate (verify broker)
    // tls_credential_add(TLS_SEC_TAG, TLS_CREDENTIAL_CA_CERTIFICATE,
    //                    ca_cert, sizeof(ca_cert));

    // Client Certificate (identify device)
    // tls_credential_add(TLS_SEC_TAG, TLS_CREDENTIAL_SERVER_CERTIFICATE,
    //                    client_cert, sizeof(client_cert));

    // Client Private Key
    // tls_credential_add(TLS_SEC_TAG, TLS_CREDENTIAL_PRIVATE_KEY,
    //                    client_key, sizeof(client_key));
}

/*
 * CONNECTION WITH TLS:
 *
 * broker.sin_port = htons(8883);  // TLS port
 * client.transport.type = MQTT_TRANSPORT_SECURE;
 * sec_tag_t sec_tags[] = { TLS_SEC_TAG };
 * client.transport.tls.sec_tag_list = sec_tags;
 */

/*
 * Q36: How do you handle MQTT broker unavailability?
 *
 * QUICK ANSWER:
 * Exponential backoff reconnection (5s, 10s, 20s... max 60s). Local operation
 * continues (RFID still works). Offline queue stores events. On reconnect,
 * flush queue to broker.
 *
 * DETAILED EXPLANATION:
 *
 * GRACEFUL DEGRADATION:
 *
 * 1. RFID CONTINUES:
 *    - Tag reading is local
 *    - Button handlers still work
 *    - Buzzer alerts still function
 *
 * 2. QUEUE EVENTS:
 *    - Store detection events locally
 *    - Circular buffer with overflow handling
 *    - Prioritize alerts over status
 *
 * 3. RECONNECT STRATEGY:
 *    - Exponential backoff prevents hammering
 *    - DNS re-resolution in case of IP change
 *    - Verify WiFi connected before MQTT retry
 *
 * 4. FLUSH ON RECONNECT:
 */
void on_mqtt_connected(void) {
    // Flush queued messages
    while (queue_tail != queue_head) {
        // mqtt_publish(&client, TOPIC,
        //              offline_queue[queue_tail].payload,
        //              strlen(offline_queue[queue_tail].payload),
        //              offline_queue[queue_tail].qos);
        queue_tail = (queue_tail + 1) % OFFLINE_QUEUE_SIZE;
    }
}

/*
 * Q37: How would you implement device provisioning (WiFi credentials, etc.)?
 *
 * QUICK ANSWER:
 * Multiple options: Hardcoded for development, BLE provisioning for production,
 * QR code scanning for field setup. Credentials stored in non-volatile storage.
 * Factory reset clears credentials.
 *
 * DETAILED EXPLANATION:
 *
 * PROVISIONING OPTIONS:
 *
 * 1. DEVELOPMENT (Current):
 *    - Credentials in prj.conf
 *    - Compiled into firmware
 *    - Easy but not flexible
 *
 * 2. BLE PROVISIONING (Production):
 *    - Device starts in BLE mode
 *    - Mobile app connects
 *    - User enters WiFi credentials
 *    - Stored in flash
 *    - Device restarts in WiFi mode
 *
 * 3. WIFI PROVISIONING (Alternative):
 *    - Device creates AP on first boot
 *    - User connects to AP
 *    - Web page for credential entry
 *    - SoftAP mode supported by nRF7002
 *
 * SECURE STORAGE:
 */
typedef struct {
    char ssid[32];
    char password[64];
    char mqtt_broker[64];
    uint16_t mqtt_port;
    uint32_t checksum;
} device_config_t;

void save_config(device_config_t* config) {
    // Calculate checksum
    // config->checksum = crc32(config, sizeof(*config) - 4);
    // Write to flash
    // nvs_write(&nvs, CONFIG_ID, config, sizeof(*config));
}

/*
 * Q38: How do you publish tag detection events without blocking RFID reading?
 *
 * QUICK ANSWER:
 * MQTT publish is non-blocking - queues message for transmission. Background
 * work queue handles actual socket operations. Main loop continues RFID
 * reading. Event handler processes ACKs asynchronously.
 *
 * DETAILED EXPLANATION:
 *
 * NON-BLOCKING PUBLISH:
 *
 * mqtt_publish() workflow:
 * 1. Copy message to TX buffer
 * 2. Queue for transmission
 * 3. Return immediately (non-blocking)
 * 4. Background: socket write, wait for ACK
 * 5. Event callback on completion/failure
 *
 * MAIN LOOP:
 */
void main_loop_non_blocking(void) {
    while (1) {
        // RFID reading (fast, local)
        scan_all_readers();

        // Check for tag changes
        if (tag_changed()) {
            // Non-blocking publish
            // mqtt_publish(&client, TOPIC, msg, len, QOS_1);
        }

        // Process MQTT events (non-blocking)
        // mqtt_input(&client);  // Receive
        // mqtt_live(&client);   // Keep-alive

        // Small delay to prevent CPU spin
        // k_sleep(K_MSEC(100));
    }
}

/*
 * Q39: How do you handle large numbers of tag events (event storm)?
 *
 * QUICK ANSWER:
 * Rate limiting: max N messages per second. Aggregation: batch multiple
 * changes into one message. Debouncing: only report stable state changes.
 * Compression: compact payload format.
 *
 * DETAILED EXPLANATION:
 *
 * RATE LIMITING:
 */
#define MAX_EVENTS_PER_SECOND 10
#define RATE_LIMIT_WINDOW_MS 1000

int events_this_window = 0;
uint32_t window_start = 0;

bool rate_limit_check(void) {
    uint32_t now = 0; // k_uptime_get_32();

    if (now - window_start > RATE_LIMIT_WINDOW_MS) {
        // New window
        window_start = now;
        events_this_window = 0;
    }

    if (events_this_window >= MAX_EVENTS_PER_SECOND) {
        return false;  // Rate limited
    }

    events_this_window++;
    return true;  // OK to send
}

/*
 * AGGREGATION:
 *
 * Instead of:
 *   {"reader":0,"event":"detected"}
 *   {"reader":1,"event":"detected"}
 *   {"reader":2,"event":"removed"}
 *
 * Send:
 *   {"readers":[{"id":0,"uid":"..."},{"id":1,"uid":"..."}],"removed":[2]}
 */

/*
 * Q40: How would you integrate with a hospital's existing systems (HL7, FHIR)?
 *
 * QUICK ANSWER:
 * MQTT broker bridges to hospital integration engine. HL7v2 messages for
 * legacy systems, FHIR resources for modern EHR. Middleware translates
 * MQTT events to appropriate format.
 *
 * DETAILED EXPLANATION:
 *
 * INTEGRATION ARCHITECTURE:
 *
 * SafeScan --> MQTT --> Integration Engine --> HL7/FHIR --> EHR
 *                            |
 *                       Translation Layer
 *
 * HL7V2 MESSAGE EXAMPLE:
 *
 * ORM^O01 (Order Message) for instrument tracking:
 * MSH|^~\&|SAFESCAN|OR1|EHR|HOSPITAL|20240115120000||ORM^O01|123|P|2.5
 * ORC|NW|INV001||||||||||
 * OBR|1|INV001||Surgical Instrument Count|||||||
 * OBX|1|ST|TAG_UID^RFID||12345678||||||F
 *
 * FHIR DEVICE RESOURCE:
 *
 * {
 *   "resourceType": "Device",
 *   "identifier": [{"value": "12345678"}],
 *   "deviceName": [{"name": "Surgical Scalpel", "type": "user-friendly-name"}],
 *   "status": "active",
 *   "location": {"reference": "Location/OR-1"}
 * }
 */

/*
 * ============================================================================
 * SECTION 5: MEDICAL DEVICE CONSIDERATIONS (Questions 41-50)
 * ============================================================================
 */

/*
 * Q41: What is an RFO (Retained Foreign Object) and why is it a serious problem?
 *
 * QUICK ANSWER:
 * RFO is a surgical instrument or item accidentally left inside a patient.
 * ~1,500 cases/year in US. Causes: infection, reoperation, death. Costs:
 * $50K-200K per incident. Legal liability. SafeScan prevents this.
 *
 * DETAILED EXPLANATION:
 *
 * RFO STATISTICS:
 * - 1,500 cases/year in US (estimated)
 * - 1 in 5,500 surgeries
 * - 70% discovered within 10 days
 * - 30% discovered months/years later
 *
 * CONSEQUENCES:
 * - Patient harm (infection, perforation, death)
 * - Reoperation required
 * - Extended hospital stay
 * - Legal liability ($500K average settlement)
 * - Hospital reputation damage
 * - CMS "never event" - no Medicare reimbursement
 *
 * COMMON RFO ITEMS:
 * - Sponges (most common, ~2/3)
 * - Instruments (scalpels, clamps)
 * - Needles
 * - Guide wires
 *
 * HOW SAFESCAN HELPS:
 * - Automated count vs manual (human error)
 * - Unique ID tracking (not just count)
 * - Alert before closing patient
 * - Audit trail for compliance
 */

/*
 * Q42: What FDA classification would SafeScan fall under?
 *
 * QUICK ANSWER:
 * Likely Class II medical device requiring 510(k) premarket notification.
 * Similar predicate devices exist (RFID tracking systems). Not Class III
 * because it assists counting, doesn't directly affect patient.
 *
 * DETAILED EXPLANATION:
 *
 * FDA DEVICE CLASSIFICATION:
 *
 * CLASS I (Low Risk):
 * - Examples: Bandages, tongue depressors
 * - General controls only
 * - Usually exempt from 510(k)
 *
 * CLASS II (Moderate Risk) - SafeScan likely here:
 * - Examples: Surgical gloves, RFID tracking systems
 * - General + Special controls
 * - 510(k) required (show substantial equivalence)
 * - Predicate devices exist
 *
 * CLASS III (High Risk):
 * - Examples: Pacemakers, implants
 * - PMA (Premarket Approval) required
 * - Clinical trials usually needed
 *
 * SAFESCAN PATHWAY:
 * 1. Identify predicate device (existing RFID systems)
 * 2. Demonstrate substantial equivalence
 * 3. Submit 510(k) with:
 *    - Device description
 *    - Intended use
 *    - Comparison to predicate
 *    - Performance testing
 *    - Software documentation
 */

/*
 * Q43: What software documentation would FDA require for SafeScan?
 *
 * QUICK ANSWER:
 * Software Requirements Specification (SRS), Design Specification, Risk
 * Analysis (ISO 14971), Verification/Validation testing, Traceability matrix,
 * Cybersecurity documentation, SOUP (Software of Unknown Provenance) analysis.
 *
 * DETAILED EXPLANATION:
 *
 * FDA SOFTWARE DOCUMENTATION (21 CFR Part 820):
 *
 * 1. SOFTWARE REQUIREMENTS SPEC (SRS):
 *    - Functional requirements
 *    - Performance requirements
 *    - Safety requirements
 *    - Interface requirements
 *
 * 2. SOFTWARE DESIGN SPEC (SDS):
 *    - Architecture description
 *    - Module descriptions
 *    - Data flow diagrams
 *    - State machines
 *
 * 3. RISK ANALYSIS (ISO 14971):
 *    - Hazard identification
 *    - Risk estimation
 *    - Risk control measures
 *    - Residual risk acceptance
 *
 * 4. VERIFICATION TESTING:
 *    - Unit tests
 *    - Integration tests
 *    - System tests
 *    - Traceability to requirements
 *
 * 5. VALIDATION TESTING:
 *    - User acceptance testing
 *    - Clinical workflow testing
 *    - Simulated use testing
 *
 * 6. CYBERSECURITY:
 *    - Threat model
 *    - Security controls
 *    - Vulnerability management
 *    - SBOM (Software Bill of Materials)
 */

/*
 * Q44: How do you ensure reliability in a safety-critical application?
 *
 * QUICK ANSWER:
 * Redundancy: multiple readers, local+cloud state. Watchdog timer for hang
 * detection. Fail-safe: alert on any anomaly. Extensive testing: unit,
 * integration, stress. Monitoring: Memfault for crash analytics.
 *
 * DETAILED EXPLANATION:
 *
 * RELIABILITY MEASURES:
 *
 * 1. REDUNDANT DETECTION:
 *    - 3 RFID readers
 *    - Multiple scan attempts
 *    - Debounced readings
 *
 * 2. WATCHDOG TIMER:
 */
void init_watchdog_safety(void) {
    // CONFIG_WATCHDOG=y
    // wdt_feed() in main loop
    // Reset if loop hangs > 5s
}

/*
 * 3. FAIL-SAFE BEHAVIOR:
 *    - Any uncertainty → alert
 *    - Reader failure → alert (can't verify)
 *    - Network failure → local alert still works
 *
 * 4. TESTING:
 *    - Unit tests for each module
 *    - Integration tests for RFID+MQTT
 *    - Stress test: 24hr continuous operation
 *    - EMC testing: interference immunity
 *
 * 5. CRASH ANALYTICS:
 *    - Memfault integration
 *    - Stack traces on crash
 *    - Panic handler logs to flash
 *    - Post-mortem analysis
 */

/*
 * Q45: How do you handle the case where the system itself fails during surgery?
 *
 * QUICK ANSWER:
 * Fallback to manual count (always available). Visual/audible indicator of
 * system failure. Battery backup for critical alerts. Training protocol
 * for staff. Documented backup procedure.
 *
 * DETAILED EXPLANATION:
 *
 * FAILURE MODES AND MITIGATIONS:
 *
 * 1. COMPLETE SYSTEM FAILURE:
 *    - Backup: Manual count (paper checklist)
 *    - Training: Staff knows manual procedure
 *    - Alert: Red LED indicates system down
 *
 * 2. SINGLE READER FAILURE:
 *    - Other readers still work
 *    - Alert: "Reader X offline"
 *    - Procedure: Use adjacent reader
 *
 * 3. NETWORK FAILURE:
 *    - Local operation continues
 *    - Buzzer still works
 *    - Queue events for later sync
 *
 * 4. POWER FAILURE:
 *    - Battery backup (supercap or LiPo)
 *    - Alert sounds on power loss
 *    - Graceful shutdown saves state
 */

/*
 * Q46: What cybersecurity threats does SafeScan face and how do you mitigate?
 *
 * QUICK ANSWER:
 * Threats: WiFi sniffing, MQTT injection, firmware tampering, tag spoofing.
 * Mitigations: TLS encryption, TrustZone secure boot, unique tag UIDs,
 * certificate authentication, network segmentation.
 *
 * DETAILED EXPLANATION:
 *
 * THREAT MODEL:
 *
 * 1. EAVESDROPPING (WiFi sniffing):
 *    - Threat: Attacker captures tag data
 *    - Mitigation: TLS encryption, WPA3
 *
 * 2. MESSAGE INJECTION (MQTT spoofing):
 *    - Threat: Fake "all clear" message
 *    - Mitigation: Client certificates, message signing
 *
 * 3. FIRMWARE TAMPERING:
 *    - Threat: Malicious firmware installed
 *    - Mitigation: Secure boot, signed firmware
 *
 * 4. TAG SPOOFING/CLONING:
 *    - Threat: Fake RFID tag passes as real
 *    - Mitigation: Unique UID verification, tag authentication
 *
 * 5. DENIAL OF SERVICE:
 *    - Threat: Jam RFID, flood network
 *    - Mitigation: Local alerting, rate limiting
 */

/*
 * Q47: How would you handle multi-OR deployment with central monitoring?
 *
 * QUICK ANSWER:
 * Each OR has SafeScan unit with unique device ID. All publish to central
 * MQTT broker. Dashboard subscribes to all topics. Database aggregates
 * for reporting. Role-based access for staff.
 *
 * DETAILED EXPLANATION:
 *
 * SCALABLE ARCHITECTURE:
 *
 *   OR-1              OR-2              OR-3
 * SafeScan-001     SafeScan-002     SafeScan-003
 *     |                |                |
 *     +----------------+----------------+
 *                      |
 *                 MQTT Broker
 *                      |
 *        +-------------+-------------+
 *        |             |             |
 *    Dashboard     Database     Alert System
 *    (Real-time)   (History)    (Pages/SMS)
 *
 * MQTT TOPIC STRUCTURE:
 *   hospital/or/{or_id}/safescan/{device_id}/events
 *   hospital/or/+/safescan/+/events  (wildcard subscribe)
 *
 * DASHBOARD FEATURES:
 * - Real-time status per OR
 * - Alert aggregation
 * - Historical reports
 * - Inventory management
 */

/*
 * Q48: How do you validate that SafeScan actually reduces RFO incidents?
 *
 * QUICK ANSWER:
 * Clinical validation study: track surgeries with/without SafeScan. Measure:
 * RFO near-misses caught, time for count, staff satisfaction. Compare to
 * baseline incident rate. Statistical significance required.
 *
 * DETAILED EXPLANATION:
 *
 * VALIDATION STUDY DESIGN:
 *
 * 1. BASELINE MEASUREMENT:
 *    - Track RFO incidents without SafeScan (historical)
 *    - Document manual count time
 *    - Survey staff satisfaction
 *
 * 2. INTERVENTION:
 *    - Deploy SafeScan in pilot ORs
 *    - Train staff
 *    - Monitor for 6-12 months
 *
 * 3. METRICS:
 *    - Near-misses caught by SafeScan
 *    - Count time reduction
 *    - Staff satisfaction scores
 *    - System reliability (uptime)
 *
 * 4. STATISTICAL ANALYSIS:
 *    - Compare incident rates
 *    - Chi-square test for significance
 *    - Cost-benefit analysis
 *
 * EXPECTED OUTCOMES:
 * - 95%+ reduction in RFO incidents
 * - 50% reduction in count time
 * - 90%+ staff satisfaction
 */

/*
 * Q49: What are the power and environmental requirements for OR deployment?
 *
 * QUICK ANSWER:
 * Medical-grade power supply (IEC 60601), battery backup for >30min surgery.
 * Operating temp: 18-24°C (OR controlled). IP rating: IPX1 (drip-proof).
 * EMC compliance: EN 60601-1-2.
 *
 * DETAILED EXPLANATION:
 *
 * ENVIRONMENTAL REQUIREMENTS:
 *
 * POWER:
 * - Input: 100-240V AC, 50-60Hz
 * - Isolation: 4kV (IEC 60601-1)
 * - Leakage current: <100µA
 * - Battery backup: 30+ minutes
 *
 * TEMPERATURE:
 * - Operating: 18-24°C (OR controlled)
 * - Storage: 0-40°C
 *
 * HUMIDITY:
 * - Operating: 30-60% RH
 * - Non-condensing
 *
 * EMC (EN 60601-1-2):
 * - Emissions: Below limits
 * - Immunity: Resistant to OR equipment
 * - Electrosurgical interference: Immune
 *
 * INGRESS PROTECTION:
 * - IPX1: Dripping water
 * - Wipeable surface for cleaning
 */

/*
 * Q50: How would you extend SafeScan for other hospital tracking applications?
 *
 * QUICK ANSWER:
 * Platform approach: same hardware, different firmware/tags. Applications:
 * Equipment tracking (wheelchairs, pumps), patient wristbands, medication
 * verification, staff location. Reuse RFID infrastructure across hospital.
 *
 * DETAILED EXPLANATION:
 *
 * PLATFORM EXTENSION:
 *
 * 1. EQUIPMENT TRACKING:
 *    - Tag wheelchairs, IV pumps, monitors
 *    - Track location across hospital
 *    - Reduce "lost" equipment
 *
 * 2. PATIENT IDENTIFICATION:
 *    - RFID wristbands
 *    - Verify patient before procedure
 *    - Integration with EHR
 *
 * 3. MEDICATION VERIFICATION:
 *    - Tag medication containers
 *    - Match patient to medication
 *    - Prevent wrong-drug errors
 *
 * 4. STAFF TRACKING:
 *    - Location awareness
 *    - Time-and-attendance
 *    - Contact tracing
 *
 * TECHNICAL REUSE:
 * - Same RFID readers
 * - Same WiFi/MQTT infrastructure
 * - Different tag types (wristband vs instrument)
 * - Firmware configuration per application
 */

/*
 * ============================================================================
 * SUMMARY: KEY TALKING POINTS
 * ============================================================================
 *
 * 1. TECHNOLOGY STACK:
 *    - Zephyr RTOS on nRF5340
 *    - ARM TrustZone for security
 *    - PN532 RFID readers (3x I2C buses)
 *    - MQTT over WiFi for cloud connectivity
 *
 * 2. PROBLEM SOLVED:
 *    - ~1,500 RFO cases/year in US
 *    - Automated instrument counting
 *    - Real-time alerts prevent incidents
 *
 * 3. KEY DESIGN DECISIONS:
 *    - Multiple I2C buses (fixed PN532 address)
 *    - TrustZone for credential protection
 *    - Local alerting independent of network
 *    - Button-based workflow integration
 *
 * ============================================================================
 */

int main(void) {
    printf("================================================\n");
    printf("  SAFESCAN - SURGICAL INSTRUMENT RFID TRACKING\n");
    printf("  50 Interview Questions with Detailed Answers\n");
    printf("================================================\n\n");

    printf("Key Technologies:\n");
    printf("- Zephyr RTOS on nRF7002DK/nRF5340\n");
    printf("- ARM TrustZone security partitioning\n");
    printf("- PN532 RFID readers (3x I2C buses)\n");
    printf("- MQTT over WiFi for cloud connectivity\n\n");

    printf("Topics Covered:\n");
    printf("1.  Zephyr RTOS Fundamentals (Q1-10)\n");
    printf("2.  ARM TrustZone Security (Q11-20)\n");
    printf("3.  RFID/PN532 Integration (Q21-30)\n");
    printf("4.  MQTT Cloud Connectivity (Q31-40)\n");
    printf("5.  Medical Device Considerations (Q41-50)\n");

    return 0;
}
