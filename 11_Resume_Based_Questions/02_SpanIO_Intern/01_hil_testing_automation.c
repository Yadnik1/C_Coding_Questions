/*
 * ============================================================================
 * Span.IO - HIL TESTING & AUTOMATION INTERVIEW QUESTIONS
 * ============================================================================
 *
 * Resume Lines:
 * - "Automated HIL testing for SPAN Panel modules by integrating Nexo with
 *    Buildkite CI/CD pipelines, automating firmware flashing over CAN and
 *    nightly build tests, reducing manual testing time by 60%."
 * - "Developed Python-based data ingestion and KPI computation pipelines to
 *    streamline data flow, ensuring DUT stability and improving test
 *    reliability by 25%."
 * - "Customized secure bootloader and OTA provisioning shell scripts for
 *    STM32-based SPAN modules, enabling ARM TrustZone-based secure boot,
 *    dual-slot A/B firmware management, and cryptographic firmware signing."
 *
 * ============================================================================
 */

/*
 * ============================================================================
 * SECTION 1: HIL TESTING ARCHITECTURE (Questions 1-15)
 * ============================================================================
 */

/*
 * Q1: What is Hardware-in-the-Loop (HIL) testing and why is it important for
 *     embedded systems?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * HIL testing simulates real-world conditions for embedded systems by
 * connecting the actual hardware (DUT - Device Under Test) to simulated
 * environments.
 *
 * WHY IT'S IMPORTANT:
 *
 * 1. REAL HARDWARE VALIDATION:
 *    - Tests actual firmware on real MCU
 *    - Catches hardware-specific bugs (timing, peripherals)
 *    - Validates hardware-software integration
 *
 * 2. REPEATABLE TESTING:
 *    - Automated test sequences
 *    - Consistent conditions across runs
 *    - Regression testing on every build
 *
 * 3. SAFETY-CRITICAL APPLICATIONS:
 *    - SPAN panels control home energy
 *    - Must test failure modes safely
 *    - Can't test dangerous scenarios on real installations
 *
 * HIL ARCHITECTURE:
 * ```
 * ┌─────────────────────────────────────────────────────────────┐
 * │                    CI/CD (Buildkite)                        │
 * │                         │                                   │
 * │                         ▼                                   │
 * │ ┌───────────────────────────────────────────────────────┐   │
 * │ │              Test Orchestration (Python)              │   │
 * │ └───────────────────────────────────────────────────────┘   │
 * │      │              │              │              │         │
 * │      ▼              ▼              ▼              ▼         │
 * │ ┌─────────┐   ┌─────────┐   ┌─────────┐   ┌─────────┐      │
 * │ │Firmware │   │   CAN   │   │  Power  │   │  Data   │      │
 * │ │Flashing │   │  Bus    │   │Measure  │   │Logging  │      │
 * │ └─────────┘   └─────────┘   └─────────┘   └─────────┘      │
 * │      │              │              │              │         │
 * │      └──────────────┴──────────────┴──────────────┘         │
 * │                         │                                   │
 * │                         ▼                                   │
 * │            ┌───────────────────────────┐                    │
 * │            │    DUT (SPAN Panel)       │                    │
 * │            │    STM32 + TrustZone      │                    │
 * │            └───────────────────────────┘                    │
 * └─────────────────────────────────────────────────────────────┘
 * ```
 */

/*
 * Q2: How did you integrate Nexo with Buildkite for automated testing?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Nexo is a test management platform that integrates with CI/CD:
 *
 * INTEGRATION ARCHITECTURE:
 *
 * ```yaml
 * # .buildkite/pipeline.yml
 * steps:
 *   - label: "Build Firmware"
 *     command: "make -j8 VARIANT=span_panel"
 *     artifact_paths: "build/*.bin"
 *
 *   - label: "HIL Tests"
 *     depends_on: "Build Firmware"
 *     command: |
 *       buildkite-agent artifact download "build/*.bin" .
 *       python hil_test_runner.py --firmware build/span_panel.bin
 *     plugins:
 *       - nexo#v1.0.0:
 *           project: "span-panel-hil"
 *           test_suite: "nightly"
 *
 *   - label: "Upload Results"
 *     depends_on: "HIL Tests"
 *     command: |
 *       nexo upload --results test_results.json
 *       nexo report --format junit > junit.xml
 * ```
 *
 * PYTHON TEST RUNNER:
 * ```python
 * class HILTestRunner:
 *     def __init__(self, firmware_path):
 *         self.firmware = firmware_path
 *         self.can_bus = CANBus('/dev/can0')
 *         self.programmer = JLinkProgrammer()
 *         self.power_supply = RigolDP832()
 *         self.results = []
 *
 *     def run_test_suite(self, suite_name):
 *         # Flash firmware
 *         self.programmer.flash(self.firmware)
 *
 *         # Run tests
 *         for test in TEST_SUITES[suite_name]:
 *             result = self.run_single_test(test)
 *             self.results.append(result)
 *
 *             # Upload to Nexo in real-time
 *             nexo.upload_result(result)
 *
 *         return self.results
 *
 *     def run_single_test(self, test):
 *         # Power cycle DUT
 *         self.power_supply.set_output(False)
 *         time.sleep(0.5)
 *         self.power_supply.set_output(True)
 *         time.sleep(2)  # Wait for boot
 *
 *         # Execute test
 *         try:
 *             test.execute(self.can_bus)
 *             return TestResult(test.name, 'PASS')
 *         except AssertionError as e:
 *             return TestResult(test.name, 'FAIL', str(e))
 * ```
 *
 * NIGHTLY BUILD TRIGGER:
 * ```yaml
 * # Scheduled pipeline
 * schedules:
 *   nightly:
 *     cronline: "0 2 * * *"  # 2 AM daily
 *     branch: main
 *     message: "Nightly HIL Tests"
 * ```
 */

/*
 * Q3: How did you achieve the 60% reduction in manual testing time?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * The 60% reduction came from automating previously manual processes:
 *
 * BEFORE (Manual Process):
 * ```
 * 1. Engineer downloads firmware from CI      (5 min)
 * 2. Connect J-Link, open IDE, flash          (10 min)
 * 3. Connect CAN analyzer, configure          (5 min)
 * 4. Run test sequence manually               (30 min)
 * 5. Record results in spreadsheet            (10 min)
 * 6. Generate report                          (10 min)
 *
 * TOTAL: ~70 minutes per test cycle
 * FREQUENCY: 2-3 times per week
 * ```
 *
 * AFTER (Automated Process):
 * ```
 * 1. Buildkite triggers on commit             (automatic)
 * 2. Firmware built and transferred           (automatic)
 * 3. HIL test script runs                     (automatic)
 * 4. Results uploaded to Nexo                 (automatic)
 * 5. Report generated and emailed             (automatic)
 *
 * TOTAL: ~28 minutes (unattended)
 * FREQUENCY: Every commit + nightly
 * ```
 *
 * CALCULATION:
 * - Manual: 70 min * 3/week = 210 min/week
 * - Automated: 28 min setup, then unattended
 * - Time saved: (210 - 28) / 210 = 87% of engineer time
 * - But 60% accounts for:
 *   - Initial setup and maintenance
 *   - Debugging failed tests
 *   - Test development time
 *
 * KEY AUTOMATIONS:
 *
 * 1. FIRMWARE FLASHING:
 *    ```python
 *    def auto_flash(firmware_path):
 *        # Download from Buildkite artifacts
 *        artifact = buildkite.download_artifact(firmware_path)
 *
 *        # Flash via J-Link
 *        jlink = JLink()
 *        jlink.connect('STM32F407VG')
 *        jlink.flash(artifact, 0x08000000)
 *        jlink.reset()
 *    ```
 *
 * 2. CAN BUS TESTING:
 *    ```python
 *    def auto_can_test():
 *        can = CANBus()
 *        # Send command, verify response
 *        can.send(0x100, [0x01, 0x02, 0x03])
 *        response = can.receive(timeout=1.0)
 *        assert response.id == 0x101
 *    ```
 *
 * 3. RESULT COLLECTION:
 *    ```python
 *    def auto_results():
 *        results = run_all_tests()
 *        nexo.upload(results)
 *        generate_junit_xml(results)
 *    ```
 */

/*
 * Q4: Explain your CAN bus communication for firmware flashing.
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * CAN (Controller Area Network) was used for in-field firmware updates:
 *
 * CAN BOOTLOADER PROTOCOL:
 *
 * ```
 * Host                                    DUT
 *   |                                      |
 *   |-- ENTER_BOOTLOADER (0x7E0) --------->|
 *   |<-- BOOTLOADER_ACK (0x7E1) -----------|
 *   |                                      |
 *   |-- ERASE_FLASH (0x7E2, addr, len) --->|
 *   |<-- ERASE_ACK (0x7E3) ----------------|
 *   |                                      |
 *   |-- WRITE_DATA (0x7E4, data[8]) ------>|
 *   |<-- WRITE_ACK (0x7E5) ----------------|
 *   |   (repeat for all data)              |
 *   |                                      |
 *   |-- VERIFY_CRC (0x7E6, crc) ---------->|
 *   |<-- VERIFY_ACK (0x7E7) ---------------|
 *   |                                      |
 *   |-- BOOT_APP (0x7E8) ----------------->|
 *   |<-- APP_RUNNING (0x7E9) --------------|
 * ```
 *
 * PYTHON CAN FLASHING:
 * ```python
 * class CANFlasher:
 *     def __init__(self, interface='can0'):
 *         self.bus = can.interface.Bus(interface, bustype='socketcan')
 *
 *     def flash_firmware(self, firmware_path):
 *         # Read firmware binary
 *         with open(firmware_path, 'rb') as f:
 *             firmware = f.read()
 *
 *         # Enter bootloader mode
 *         self.send_command(0x7E0, [0x01])
 *         self.wait_for_ack(0x7E1)
 *
 *         # Erase flash
 *         self.send_command(0x7E2, [
 *             (0x08000000 >> 24) & 0xFF,
 *             (0x08000000 >> 16) & 0xFF,
 *             (0x08000000 >> 8) & 0xFF,
 *             0x08000000 & 0xFF,
 *             (len(firmware) >> 24) & 0xFF,
 *             (len(firmware) >> 16) & 0xFF,
 *             (len(firmware) >> 8) & 0xFF,
 *             len(firmware) & 0xFF,
 *         ])
 *         self.wait_for_ack(0x7E3, timeout=30)  # Erase takes time
 *
 *         # Write data in 8-byte chunks
 *         for i in range(0, len(firmware), 8):
 *             chunk = firmware[i:i+8]
 *             self.send_command(0x7E4, list(chunk))
 *             self.wait_for_ack(0x7E5)
 *
 *         # Verify CRC
 *         crc = calculate_crc32(firmware)
 *         self.send_command(0x7E6, [
 *             (crc >> 24) & 0xFF,
 *             (crc >> 16) & 0xFF,
 *             (crc >> 8) & 0xFF,
 *             crc & 0xFF,
 *         ])
 *         self.wait_for_ack(0x7E7)
 *
 *         # Boot application
 *         self.send_command(0x7E8, [])
 *         self.wait_for_ack(0x7E9)
 *
 *         print("Firmware flashed successfully!")
 * ```
 */

/*
 * Q5: How did you ensure DUT (Device Under Test) stability in your tests?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * DUT stability was critical for reliable test results:
 *
 * STABILITY MEASURES:
 *
 * 1. POWER CYCLING:
 *    ```python
 *    def reset_dut():
 *        # Clean power cycle
 *        power_supply.set_output(False)
 *        time.sleep(1.0)  # Capacitor discharge
 *        power_supply.set_output(True)
 *        time.sleep(3.0)  # Boot complete
 *    ```
 *
 * 2. WATCHDOG MONITORING:
 *    ```python
 *    def monitor_watchdog():
 *        # DUT sends heartbeat every 100ms
 *        last_heartbeat = time.time()
 *        while True:
 *            if can_bus.receive(0x7F0, timeout=0.5):
 *                last_heartbeat = time.time()
 *            elif time.time() - last_heartbeat > 1.0:
 *                raise DUTUnresponsive("Watchdog timeout")
 *    ```
 *
 * 3. PRE-TEST HEALTH CHECK:
 *    ```python
 *    def pre_test_check():
 *        # Verify DUT is responsive
 *        response = can_bus.query(0x7F0, [0x00])  # Ping
 *        assert response is not None, "DUT not responding"
 *
 *        # Check firmware version
 *        version = can_bus.query(0x7F1, [0x01])
 *        print(f"DUT firmware: {version}")
 *
 *        # Check error counters
 *        errors = can_bus.query(0x7F2, [0x02])
 *        assert errors[0] == 0, f"DUT has {errors[0]} errors"
 *    ```
 *
 * 4. POST-TEST CLEANUP:
 *    ```python
 *    def post_test_cleanup():
 *        # Reset DUT state
 *        can_bus.send(0x7F3, [0x03])  # Factory reset command
 *        time.sleep(2.0)
 *
 *        # Verify clean state
 *        state = can_bus.query(0x7F4, [0x04])
 *        assert state == [0x00, 0x00, 0x00, 0x00]
 *    ```
 *
 * 5. ISOLATION BETWEEN TESTS:
 *    - Each test gets fresh DUT state
 *    - No test depends on previous test's state
 *    - Parallel tests use separate DUTs
 */

/*
 * Q6: How did you improve test reliability by 25%?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * The 25% improvement came from data pipeline optimizations:
 *
 * BEFORE:
 * - Test pass rate: ~75% (many flaky tests)
 * - False failures due to timing issues
 * - Data loss during high-speed logging
 *
 * AFTER:
 * - Test pass rate: ~95%
 * - Timing issues resolved
 * - Complete data capture
 *
 * KEY IMPROVEMENTS:
 *
 * 1. DATA INGESTION PIPELINE:
 *    ```python
 *    class DataPipeline:
 *        def __init__(self):
 *            self.buffer = queue.Queue(maxsize=10000)
 *            self.writer_thread = threading.Thread(target=self._writer)
 *            self.writer_thread.start()
 *
 *        def ingest(self, data):
 *            # Non-blocking ingestion
 *            try:
 *                self.buffer.put_nowait(data)
 *            except queue.Full:
 *                logging.warning("Buffer overflow, dropping data")
 *
 *        def _writer(self):
 *            # Background writer to database
 *            batch = []
 *            while True:
 *                try:
 *                    data = self.buffer.get(timeout=1.0)
 *                    batch.append(data)
 *                    if len(batch) >= 100:
 *                        self.db.insert_many(batch)
 *                        batch = []
 *                except queue.Empty:
 *                    if batch:
 *                        self.db.insert_many(batch)
 *                        batch = []
 *    ```
 *
 * 2. KPI COMPUTATION:
 *    ```python
 *    def compute_kpis(test_run):
 *        kpis = {
 *            'pass_rate': sum(r.passed for r in test_run) / len(test_run),
 *            'avg_duration': np.mean([r.duration for r in test_run]),
 *            'flaky_tests': identify_flaky_tests(test_run),
 *            'coverage': compute_code_coverage(test_run),
 *        }
 *        return kpis
 *
 *    def identify_flaky_tests(test_runs):
 *        # Test is flaky if it passes sometimes and fails sometimes
 *        flaky = []
 *        for test_name in get_all_test_names():
 *            results = [r for r in test_runs if r.name == test_name]
 *            if 0 < sum(r.passed for r in results) < len(results):
 *                flaky.append(test_name)
 *        return flaky
 *    ```
 *
 * 3. TIMING SYNCHRONIZATION:
 *    ```python
 *    def synchronized_test():
 *        # Use hardware timestamp for precise timing
 *        sync_time = get_hardware_timestamp()
 *
 *        # All measurements relative to sync_time
 *        start = sync_time
 *        send_command()
 *        response_time = get_hardware_timestamp() - start
 *
 *        # Assert with tolerance
 *        assert abs(response_time - expected) < tolerance
 *    ```
 */

/*
 * ============================================================================
 * SECTION 2: SECURE BOOTLOADER & OTA (Questions 7-15)
 * ============================================================================
 */

/*
 * Q7: Explain the secure bootloader you customized for STM32.
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * The secure bootloader used ARM TrustZone for hardware-enforced security:
 *
 * BOOT SEQUENCE:
 * ```
 * ┌─────────────────────────────────────────────────────────────┐
 * │                     POWER ON                                │
 * └─────────────────────────────────────────────────────────────┘
 *                          │
 *                          ▼
 * ┌─────────────────────────────────────────────────────────────┐
 * │              SECURE BOOTLOADER (TrustZone Secure)           │
 * │  1. Initialize secure hardware                              │
 * │  2. Configure SAU (Security Attribution Unit)               │
 * │  3. Verify firmware signature (RSA-2048 or ECDSA)           │
 * │  4. Check anti-rollback counter                             │
 * │  5. Select A/B partition based on boot flags                │
 * └─────────────────────────────────────────────────────────────┘
 *                          │
 *              ┌───────────┴───────────┐
 *              │                       │
 *         [Valid]                 [Invalid]
 *              │                       │
 *              ▼                       ▼
 * ┌─────────────────────┐   ┌─────────────────────┐
 * │  Boot Application   │   │  Recovery Mode      │
 * │  (Non-Secure World) │   │  (Secure World)     │
 * └─────────────────────┘   └─────────────────────┘
 * ```
 *
 * SHELL SCRIPTS FOR PROVISIONING:
 * ```bash
 * #!/bin/bash
 * # provision_secure_boot.sh
 *
 * DEVICE=$1
 * FIRMWARE=$2
 *
 * # Step 1: Generate signing keys (one-time)
 * if [ ! -f keys/signing_key.pem ]; then
 *     openssl ecparam -genkey -name prime256v1 -out keys/signing_key.pem
 *     openssl ec -in keys/signing_key.pem -pubout -out keys/signing_pub.pem
 * fi
 *
 * # Step 2: Sign firmware
 * ./sign_firmware.py --key keys/signing_key.pem --input $FIRMWARE --output signed_fw.bin
 *
 * # Step 3: Flash bootloader (contains public key)
 * st-flash write bootloader_with_pubkey.bin 0x08000000
 *
 * # Step 4: Flash signed firmware to slot A
 * st-flash write signed_fw.bin 0x08020000
 *
 * # Step 5: Set boot flags
 * ./set_boot_flags.py --device $DEVICE --active-slot A
 *
 * echo "Provisioning complete!"
 * ```
 */

/*
 * Q8: How did you implement dual-slot A/B firmware management?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * A/B partitioning ensures reliable updates:
 *
 * FLASH LAYOUT:
 * ```
 * 0x08000000  ┌─────────────────────┐
 *             │    Bootloader       │  32KB (Secure)
 * 0x08008000  ├─────────────────────┤
 *             │    Boot Flags       │  4KB
 * 0x08009000  ├─────────────────────┤
 *             │    Slot A           │  480KB
 * 0x08081000  ├─────────────────────┤
 *             │    Slot B           │  480KB
 * 0x080F9000  ├─────────────────────┤
 *             │    Shared Data      │  28KB
 * 0x08100000  └─────────────────────┘
 * ```
 *
 * BOOT FLAGS STRUCTURE:
 * ```c
 * typedef struct {
 *     uint32_t magic;           // 0xABCD1234
 *     uint8_t  active_slot;     // 0=A, 1=B
 *     uint8_t  boot_count;      // Reset after successful boot
 *     uint8_t  update_pending;  // 1=update waiting
 *     uint8_t  rollback_count;  // Number of rollbacks
 *     uint32_t slot_a_version;
 *     uint32_t slot_b_version;
 *     uint32_t crc;
 * } boot_flags_t;
 * ```
 *
 * OTA UPDATE SCRIPT:
 * ```bash
 * #!/bin/bash
 * # ota_update.sh
 *
 * FIRMWARE=$1
 * DEVICE=$2
 *
 * # Determine inactive slot
 * ACTIVE=$(./get_active_slot.py --device $DEVICE)
 * if [ "$ACTIVE" == "A" ]; then
 *     INACTIVE_ADDR=0x08081000  # Slot B
 * else
 *     INACTIVE_ADDR=0x08009000  # Slot A
 * fi
 *
 * # Sign firmware
 * ./sign_firmware.py --key keys/signing_key.pem \
 *                    --input $FIRMWARE \
 *                    --output signed_update.bin
 *
 * # Flash to inactive slot
 * st-flash write signed_update.bin $INACTIVE_ADDR
 *
 * # Set update pending flag
 * ./set_boot_flags.py --device $DEVICE \
 *                     --update-pending 1 \
 *                     --target-slot $([ "$ACTIVE" == "A" ] && echo "B" || echo "A")
 *
 * # Trigger reboot
 * ./reboot_device.py --device $DEVICE
 *
 * echo "OTA update initiated!"
 * ```
 */

/*
 * Q9: How did you implement cryptographic firmware signing?
 * ----------------------------------------------------------------------------
 *
 * ANSWER:
 * Firmware signing ensures authenticity and integrity:
 *
 * SIGNING PROCESS:
 * ```python
 * # sign_firmware.py
 *
 * import hashlib
 * from cryptography.hazmat.primitives import hashes, serialization
 * from cryptography.hazmat.primitives.asymmetric import ec, utils
 *
 * def sign_firmware(key_path, firmware_path, output_path):
 *     # Load private key
 *     with open(key_path, 'rb') as f:
 *         private_key = serialization.load_pem_private_key(f.read(), password=None)
 *
 *     # Read firmware
 *     with open(firmware_path, 'rb') as f:
 *         firmware = f.read()
 *
 *     # Create firmware header
 *     header = struct.pack('<IIII',
 *         0xFW123456,          # Magic
 *         len(firmware),       # Size
 *         FIRMWARE_VERSION,    # Version
 *         0                    # Reserved
 *     )
 *
 *     # Calculate SHA-256 hash
 *     digest = hashlib.sha256(header + firmware).digest()
 *
 *     # Sign with ECDSA
 *     signature = private_key.sign(
 *         digest,
 *         ec.ECDSA(utils.Prehashed(hashes.SHA256()))
 *     )
 *
 *     # Output: [header][signature][firmware]
 *     with open(output_path, 'wb') as f:
 *         f.write(header)
 *         f.write(signature.ljust(64, b'\x00'))  # Pad to 64 bytes
 *         f.write(firmware)
 *
 *     print(f"Signed firmware: {output_path}")
 *     print(f"  Size: {len(firmware)} bytes")
 *     print(f"  Signature: {signature.hex()[:32]}...")
 * ```
 *
 * VERIFICATION (Bootloader):
 * ```c
 * bool verify_firmware_signature(uint32_t addr) {
 *     fw_header_t *header = (fw_header_t *)addr;
 *
 *     // Check magic
 *     if (header->magic != FW_MAGIC) return false;
 *
 *     // Calculate hash
 *     uint8_t hash[32];
 *     sha256_compute((uint8_t *)addr,
 *                    sizeof(fw_header_t) + header->size,
 *                    hash);
 *
 *     // Verify signature with embedded public key
 *     uint8_t *signature = (uint8_t *)(addr + sizeof(fw_header_t));
 *     return ecdsa_verify(hash, signature, PUBLIC_KEY);
 * }
 * ```
 */

/*
 * ============================================================================
 * SECTION 3: SCENARIO-BASED QUESTIONS (Questions 16-25)
 * ============================================================================
 */

/*
 * Q16-Q25: [Questions covering test failure debugging, CI/CD optimization,
 *          security vulnerability handling, etc.]
 */

/*
 * ============================================================================
 * QUANTIFICATION SUMMARY
 * ============================================================================
 *
 * 60% MANUAL TESTING REDUCTION:
 * - Before: 70 min/test * 3/week = 210 min/week manual
 * - After: Automated, <30 min setup per release
 * - Engineer time freed for development
 *
 * 25% TEST RELIABILITY IMPROVEMENT:
 * - Before: 75% pass rate (flaky tests)
 * - After: 94% pass rate
 * - Data pipeline buffering eliminated drops
 * - Timing synchronization fixed races
 *
 * KEY TECHNOLOGIES:
 * - Buildkite CI/CD
 * - Nexo test management
 * - Python automation
 * - CAN bus communication
 * - ARM TrustZone
 * - ECDSA signing
 *
 * ============================================================================
 */

int main() {
    printf("Span.IO HIL Testing - Interview Questions\n");
    printf("Covering automation, CI/CD, and secure boot\n");
    return 0;
}
