/*
 * ============================================================================
 * QUESTION 38: OTA (Over-The-Air) Updates - Interview Deep Dive
 * ============================================================================
 *
 * DIFFICULTY: Advanced | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * "Explain OTA update mechanisms in embedded systems. How do you ensure
 *  reliability and security during firmware updates?"
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * OTA enables wireless firmware updates without physical access. Key components:
 * 1. Dual-bank (A/B) partition scheme for rollback capability
 * 2. Secure boot chain to verify firmware authenticity
 * 3. Delta updates to minimize download size
 * 4. Atomic updates - never leave device in broken state
 *
 * Critical considerations: power failure recovery, version management,
 * signature verification, and rollback on boot failure.
 *
 * ============================================================================
 * DETAILED EXPLANATION
 * ============================================================================
 *
 * WHY OTA MATTERS:
 * ----------------
 * - Field updates without physical access (IoT devices, vehicles)
 * - Security patch deployment
 * - Feature additions post-deployment
 * - Cost savings vs. manual updates
 * - Regulatory compliance (medical, automotive)
 *
 * OTA ARCHITECTURE COMPONENTS:
 * ----------------------------
 *
 * 1. PARTITION SCHEME:
 *
 *    Single Bank (Not Recommended):
 *    +------------------+
 *    | Bootloader       |  <- Fixed, rarely updated
 *    +------------------+
 *    | Application      |  <- Overwritten during update
 *    +------------------+
 *    | Data/Config      |
 *    +------------------+
 *    Problem: Power loss during update = BRICKED DEVICE
 *
 *    Dual Bank (A/B) - Industry Standard:
 *    +------------------+
 *    | Bootloader       |  <- Selects active bank
 *    +------------------+
 *    | Bank A (Active)  |  <- Currently running
 *    +------------------+
 *    | Bank B (Update)  |  <- New firmware written here
 *    +------------------+
 *    | Shared Data      |  <- Persists across updates
 *    +------------------+
 *
 *    A/B/Recovery (Android-style):
 *    +------------------+
 *    | Bootloader       |
 *    +------------------+
 *    | Bank A           |
 *    +------------------+
 *    | Bank B           |
 *    +------------------+
 *    | Recovery         |  <- Minimal OS for emergency
 *    +------------------+
 *
 * 2. UPDATE PROCESS FLOW:
 *
 *    +---------+     +---------+     +---------+     +---------+
 *    | Download| --> | Verify  | --> | Write   | --> | Activate|
 *    | Package |     | Signature|    | to Bank |     | & Reboot|
 *    +---------+     +---------+     +---------+     +---------+
 *         |               |               |               |
 *         v               v               v               v
 *    [Check space]   [Crypto verify] [CRC each block] [Set boot flag]
 *    [Resume support][Version check] [Power-safe]    [Verify boot]
 *
 * 3. VERSION MANAGEMENT:
 *
 *    Version Header Structure:
 *    +------------------+
 *    | Magic Number     |  0xDEADBEEF
 *    +------------------+
 *    | Version Major    |  Breaking changes
 *    +------------------+
 *    | Version Minor    |  New features
 *    +------------------+
 *    | Version Patch    |  Bug fixes
 *    +------------------+
 *    | Build Number     |  CI/CD build ID
 *    +------------------+
 *    | Hardware Rev     |  Compatible HW versions
 *    +------------------+
 *    | Signature        |  256 bytes (RSA-2048)
 *    +------------------+
 *
 * ============================================================================
 * OTA SECURITY
 * ============================================================================
 *
 * THREAT MODEL:
 * -------------
 * 1. Man-in-the-middle (intercept/modify update)
 * 2. Replay attacks (install old vulnerable firmware)
 * 3. Unauthorized firmware (malicious image)
 * 4. Denial of service (corrupt update partition)
 *
 * SECURITY MEASURES:
 * ------------------
 *
 * 1. CODE SIGNING:
 *    - Firmware signed with private key (kept secure)
 *    - Device verifies with public key (embedded in bootloader)
 *    - RSA-2048 or ECDSA-P256 recommended
 *
 *    Signing Process:
 *    [Firmware Binary] -> [SHA-256 Hash] -> [Sign with Private Key] -> [Signature]
 *
 *    Verification:
 *    [Signature] -> [Decrypt with Public Key] -> [Compare with Hash]
 *
 * 2. SECURE TRANSPORT:
 *    - TLS 1.3 for download
 *    - Certificate pinning
 *    - Mutual authentication (device proves identity)
 *
 * 3. ANTI-ROLLBACK:
 *    - Monotonic counter in secure storage
 *    - Version must be >= current counter
 *    - Counter incremented after successful boot
 *
 *    Anti-Rollback Flow:
 *    +-------------------+
 *    | Check new_version |
 *    | >= rollback_cnt   |
 *    +-------------------+
 *            |
 *       Yes  |  No
 *            v
 *    +-------+-------+
 *    |               |
 *    v               v
 *    [Apply]    [Reject]
 *
 * ============================================================================
 * DELTA UPDATES
 * ============================================================================
 *
 * Full update: Download entire firmware (100KB - several MB)
 * Delta update: Download only differences (typically 10-30% of full)
 *
 * DELTA ALGORITHMS:
 * -----------------
 * 1. bsdiff - Good compression, memory intensive
 * 2. xdelta3 - Lower memory, good for embedded
 * 3. JojoDiff - Designed for embedded systems
 *
 * Delta Update Process:
 * +-------------+     +-------------+     +-------------+
 * | Old Firmware| --> | Apply Patch | --> |New Firmware |
 * | (on device) |     | (delta file)|     | (generated) |
 * +-------------+     +-------------+     +-------------+
 *
 * TRADE-OFFS:
 * - Smaller download vs. more processing on device
 * - Requires old firmware present (can't skip versions easily)
 * - Patch generation done on server
 *
 * ============================================================================
 * POWER FAILURE RESILIENCE
 * ============================================================================
 *
 * PROBLEM: Power loss at any point must not brick device
 *
 * SOLUTION: Atomic State Transitions
 *
 * State Machine:
 * +----------+     +----------+     +----------+     +----------+
 * | IDLE     | --> | DOWNLOAD | --> | VERIFY   | --> | PENDING  |
 * +----------+     +----------+     +----------+     +----------+
 *                       |                                  |
 *                  [Power loss]                       [Reboot]
 *                       |                                  |
 *                       v                                  v
 *                  [Resume DL]                       +----------+
 *                                                   | TESTING  |
 *                                                   +----------+
 *                                                        |
 *                                            +-----------+-----------+
 *                                            |                       |
 *                                       [Boot OK]               [Boot Fail]
 *                                            |                       |
 *                                            v                       v
 *                                       +----------+           +----------+
 *                                       | COMPLETE |           | ROLLBACK |
 *                                       +----------+           +----------+
 *
 * STATE STORAGE:
 * - Use wear-leveled flash or dedicated EEPROM
 * - Write state BEFORE action, update AFTER
 * - Always recoverable from any state
 *
 * ============================================================================
 * CODE EXAMPLE: OTA Update Manager
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ============================================================================
 * OTA DATA STRUCTURES
 * ============================================================================ */

typedef enum {
    OTA_STATE_IDLE,
    OTA_STATE_DOWNLOADING,
    OTA_STATE_VERIFYING,
    OTA_STATE_PENDING_REBOOT,
    OTA_STATE_TESTING,
    OTA_STATE_COMPLETE,
    OTA_STATE_ROLLBACK
} ota_state_t;

typedef enum {
    OTA_ERR_NONE = 0,
    OTA_ERR_NO_SPACE,
    OTA_ERR_DOWNLOAD_FAIL,
    OTA_ERR_INVALID_SIGNATURE,
    OTA_ERR_VERSION_ROLLBACK,
    OTA_ERR_HARDWARE_MISMATCH,
    OTA_ERR_WRITE_FAIL,
    OTA_ERR_VERIFY_FAIL,
    OTA_ERR_BOOT_FAIL
} ota_error_t;

typedef struct {
    uint32_t magic;              /* 0xOTA12345 */
    uint32_t version_major;
    uint32_t version_minor;
    uint32_t version_patch;
    uint32_t build_number;
    uint32_t hardware_rev_min;
    uint32_t hardware_rev_max;
    uint32_t image_size;
    uint32_t image_crc32;
    uint8_t  signature[256];     /* RSA-2048 signature */
} ota_image_header_t;

typedef struct {
    ota_state_t state;
    uint32_t    bytes_downloaded;
    uint32_t    total_size;
    uint8_t     target_bank;     /* 0 = Bank A, 1 = Bank B */
    uint32_t    retry_count;
    uint32_t    boot_attempts;   /* For testing phase */
} ota_context_t;

/* Simulated flash banks */
#define FLASH_BANK_SIZE  (256 * 1024)  /* 256KB per bank */
static uint8_t flash_bank_a[FLASH_BANK_SIZE];
static uint8_t flash_bank_b[FLASH_BANK_SIZE];
static uint8_t active_bank = 0;  /* 0 = A, 1 = B */
static ota_context_t ota_ctx;

/* Anti-rollback counter (should be in secure storage) */
static uint32_t rollback_counter = 0;

/* ============================================================================
 * FLASH ABSTRACTION LAYER
 * ============================================================================ */

/*
 * Say: "The flash abstraction layer provides platform-independent
 *       access to flash memory. This allows the same OTA code to
 *       run on different hardware platforms."
 */
static bool flash_erase_bank(uint8_t bank) {
    printf("[FLASH] Erasing bank %c\n", bank ? 'B' : 'A');
    uint8_t *target = bank ? flash_bank_b : flash_bank_a;
    memset(target, 0xFF, FLASH_BANK_SIZE);  /* Flash erases to 0xFF */
    return true;
}

static bool flash_write(uint8_t bank, uint32_t offset,
                        const uint8_t *data, uint32_t len) {
    if (offset + len > FLASH_BANK_SIZE) return false;
    uint8_t *target = bank ? flash_bank_b : flash_bank_a;
    memcpy(target + offset, data, len);
    return true;
}

static bool flash_read(uint8_t bank, uint32_t offset,
                       uint8_t *data, uint32_t len) {
    if (offset + len > FLASH_BANK_SIZE) return false;
    uint8_t *target = bank ? flash_bank_b : flash_bank_a;
    memcpy(data, target + offset, len);
    return true;
}

/* ============================================================================
 * CRYPTOGRAPHIC VERIFICATION (Simplified)
 * ============================================================================ */

/*
 * Say: "In production, we use hardware crypto accelerators and
 *       proper RSA/ECDSA verification. This is a simplified CRC check
 *       for demonstration purposes."
 */
static uint32_t calculate_crc32(const uint8_t *data, uint32_t len) {
    uint32_t crc = 0xFFFFFFFF;
    for (uint32_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
    }
    return ~crc;
}

static bool verify_signature(const ota_image_header_t *header,
                             const uint8_t *image_data) {
    /*
     * In production:
     * 1. Hash the image with SHA-256
     * 2. Verify signature using RSA public key
     * 3. Public key is embedded in bootloader (immutable)
     */
    printf("[CRYPTO] Verifying firmware signature...\n");

    /* Simplified: just check CRC */
    uint32_t calculated_crc = calculate_crc32(image_data, header->image_size);
    return (calculated_crc == header->image_crc32);
}

/* ============================================================================
 * VERSION MANAGEMENT
 * ============================================================================ */

typedef struct {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
} version_t;

static version_t current_version = {1, 0, 0};

/*
 * Say: "Version comparison follows semantic versioning. We compare
 *       major, then minor, then patch. The rollback counter prevents
 *       downgrade attacks."
 */
static int compare_versions(const version_t *a, const version_t *b) {
    if (a->major != b->major) return a->major - b->major;
    if (a->minor != b->minor) return a->minor - b->minor;
    return a->patch - b->patch;
}

static bool check_version_allowed(const ota_image_header_t *header) {
    version_t new_ver = {
        header->version_major,
        header->version_minor,
        header->version_patch
    };

    /* Check against anti-rollback counter */
    uint32_t new_version_num = (new_ver.major << 16) |
                               (new_ver.minor << 8) |
                               new_ver.patch;

    if (new_version_num < rollback_counter) {
        printf("[OTA] REJECTED: Version below rollback counter\n");
        return false;
    }

    /* Allow same or newer version */
    return compare_versions(&new_ver, &current_version) >= 0;
}

/* ============================================================================
 * OTA STATE MACHINE
 * ============================================================================ */

/*
 * Say: "The OTA state machine ensures we can recover from any failure.
 *       State is persisted to flash before each transition, so we
 *       know exactly where to resume after a power failure."
 */

static void save_ota_state(void) {
    /* In production: write to dedicated flash sector with wear leveling */
    printf("[OTA] State saved: %d, downloaded: %u/%u\n",
           ota_ctx.state, ota_ctx.bytes_downloaded, ota_ctx.total_size);
}

static void load_ota_state(void) {
    /* In production: read from flash, verify CRC */
    printf("[OTA] State loaded\n");
}

ota_error_t ota_start_update(const ota_image_header_t *header) {
    printf("\n=== OTA Update Started ===\n");
    printf("New version: %u.%u.%u (build %u)\n",
           header->version_major, header->version_minor,
           header->version_patch, header->build_number);

    /* Step 1: Version check */
    if (!check_version_allowed(header)) {
        return OTA_ERR_VERSION_ROLLBACK;
    }

    /* Step 2: Hardware compatibility */
    uint32_t hw_rev = 1;  /* Get from hardware */
    if (hw_rev < header->hardware_rev_min ||
        hw_rev > header->hardware_rev_max) {
        return OTA_ERR_HARDWARE_MISMATCH;
    }

    /* Step 3: Space check */
    if (header->image_size > FLASH_BANK_SIZE) {
        return OTA_ERR_NO_SPACE;
    }

    /* Step 4: Initialize OTA context */
    ota_ctx.state = OTA_STATE_DOWNLOADING;
    ota_ctx.bytes_downloaded = 0;
    ota_ctx.total_size = header->image_size;
    ota_ctx.target_bank = active_bank ? 0 : 1;  /* Write to inactive bank */
    ota_ctx.retry_count = 0;

    /* Step 5: Erase target bank */
    flash_erase_bank(ota_ctx.target_bank);

    save_ota_state();
    return OTA_ERR_NONE;
}

ota_error_t ota_write_chunk(const uint8_t *data, uint32_t len) {
    if (ota_ctx.state != OTA_STATE_DOWNLOADING) {
        return OTA_ERR_WRITE_FAIL;
    }

    /* Write with verification */
    if (!flash_write(ota_ctx.target_bank, ota_ctx.bytes_downloaded, data, len)) {
        return OTA_ERR_WRITE_FAIL;
    }

    /* Verify write (read back and compare) */
    uint8_t verify_buf[256];
    for (uint32_t i = 0; i < len; i += sizeof(verify_buf)) {
        uint32_t chunk = (len - i < sizeof(verify_buf)) ? len - i : sizeof(verify_buf);
        flash_read(ota_ctx.target_bank, ota_ctx.bytes_downloaded + i,
                   verify_buf, chunk);
        if (memcmp(data + i, verify_buf, chunk) != 0) {
            return OTA_ERR_VERIFY_FAIL;
        }
    }

    ota_ctx.bytes_downloaded += len;

    /* Save progress periodically for resume capability */
    if (ota_ctx.bytes_downloaded % (16 * 1024) == 0) {
        save_ota_state();
        printf("[OTA] Progress: %u/%u bytes (%.1f%%)\n",
               ota_ctx.bytes_downloaded, ota_ctx.total_size,
               100.0 * ota_ctx.bytes_downloaded / ota_ctx.total_size);
    }

    return OTA_ERR_NONE;
}

ota_error_t ota_finalize(const ota_image_header_t *header) {
    printf("\n[OTA] Download complete, verifying...\n");
    ota_ctx.state = OTA_STATE_VERIFYING;
    save_ota_state();

    /* Read back and verify signature */
    uint8_t *image_data = ota_ctx.target_bank ? flash_bank_b : flash_bank_a;

    if (!verify_signature(header, image_data)) {
        printf("[OTA] SIGNATURE VERIFICATION FAILED!\n");
        ota_ctx.state = OTA_STATE_IDLE;
        save_ota_state();
        return OTA_ERR_INVALID_SIGNATURE;
    }

    printf("[OTA] Signature verified successfully\n");

    /* Mark update as pending */
    ota_ctx.state = OTA_STATE_PENDING_REBOOT;
    ota_ctx.boot_attempts = 0;
    save_ota_state();

    printf("[OTA] Update ready. Reboot to apply.\n");
    return OTA_ERR_NONE;
}

/* ============================================================================
 * BOOTLOADER INTEGRATION
 * ============================================================================ */

/*
 * Say: "The bootloader handles the final step of activating new firmware.
 *       It checks the boot flag, validates the image, and either boots
 *       the new firmware or rolls back to the old one."
 */

void bootloader_check_update(void) {
    load_ota_state();

    if (ota_ctx.state == OTA_STATE_PENDING_REBOOT) {
        printf("\n[BOOT] New firmware pending, attempting boot...\n");
        ota_ctx.state = OTA_STATE_TESTING;
        ota_ctx.boot_attempts++;
        save_ota_state();

        /* Switch to new bank */
        active_bank = ota_ctx.target_bank;
        printf("[BOOT] Booting from bank %c\n", active_bank ? 'B' : 'A');
    }
    else if (ota_ctx.state == OTA_STATE_TESTING) {
        printf("\n[BOOT] In testing phase, attempt %u\n", ota_ctx.boot_attempts);

        if (ota_ctx.boot_attempts >= 3) {
            printf("[BOOT] Too many failed attempts, rolling back!\n");
            ota_ctx.state = OTA_STATE_ROLLBACK;
            active_bank = ota_ctx.target_bank ? 0 : 1;  /* Revert to old bank */
            save_ota_state();
        }
    }
}

void application_confirm_boot(void) {
    /*
     * Say: "The application must confirm successful boot within a timeout.
     *       This is called after self-tests pass. If not called, watchdog
     *       resets and bootloader rolls back."
     */
    if (ota_ctx.state == OTA_STATE_TESTING) {
        printf("[APP] Boot confirmed successful!\n");
        ota_ctx.state = OTA_STATE_COMPLETE;

        /* Update anti-rollback counter */
        rollback_counter = (current_version.major << 16) |
                          (current_version.minor << 8) |
                          current_version.patch;

        save_ota_state();
        printf("[APP] Rollback counter updated to %u\n", rollback_counter);
    }
}

/* ============================================================================
 * DELTA UPDATE SUPPORT
 * ============================================================================ */

/*
 * Say: "Delta updates reduce download size by sending only the differences.
 *       The device reconstructs the full image by applying the patch to
 *       the current firmware."
 */

typedef struct {
    uint32_t old_offset;
    uint32_t new_offset;
    uint32_t length;
    uint8_t  operation;  /* 0=copy, 1=add, 2=skip */
} delta_instruction_t;

ota_error_t apply_delta_patch(const delta_instruction_t *instructions,
                              uint32_t num_instructions,
                              const uint8_t *diff_data) {
    printf("[DELTA] Applying %u patch instructions\n", num_instructions);

    uint8_t *old_image = active_bank ? flash_bank_b : flash_bank_a;
    uint8_t *new_image = active_bank ? flash_bank_a : flash_bank_b;

    for (uint32_t i = 0; i < num_instructions; i++) {
        const delta_instruction_t *inst = &instructions[i];

        switch (inst->operation) {
            case 0:  /* Copy from old */
                memcpy(new_image + inst->new_offset,
                       old_image + inst->old_offset,
                       inst->length);
                break;
            case 1:  /* Add from diff */
                memcpy(new_image + inst->new_offset,
                       diff_data,
                       inst->length);
                diff_data += inst->length;
                break;
            case 2:  /* Skip (zeros or known pattern) */
                memset(new_image + inst->new_offset, 0, inst->length);
                break;
        }
    }

    return OTA_ERR_NONE;
}

/* ============================================================================
 * DEMONSTRATION
 * ============================================================================ */

int main() {
    printf("=== OTA Update System Demo ===\n\n");

    /* Simulate firmware update */
    ota_image_header_t new_firmware = {
        .magic = 0x0TA12345,
        .version_major = 1,
        .version_minor = 1,
        .version_patch = 0,
        .build_number = 100,
        .hardware_rev_min = 1,
        .hardware_rev_max = 3,
        .image_size = 1024,
        .image_crc32 = 0  /* Will be calculated */
    };

    /* Create dummy firmware image */
    uint8_t firmware_data[1024];
    for (int i = 0; i < 1024; i++) {
        firmware_data[i] = i & 0xFF;
    }
    new_firmware.image_crc32 = calculate_crc32(firmware_data, 1024);

    /* Start OTA */
    ota_error_t err = ota_start_update(&new_firmware);
    if (err != OTA_ERR_NONE) {
        printf("OTA start failed: %d\n", err);
        return 1;
    }

    /* Write firmware in chunks */
    for (uint32_t offset = 0; offset < 1024; offset += 256) {
        err = ota_write_chunk(firmware_data + offset, 256);
        if (err != OTA_ERR_NONE) {
            printf("OTA write failed: %d\n", err);
            return 1;
        }
    }

    /* Finalize */
    err = ota_finalize(&new_firmware);
    if (err != OTA_ERR_NONE) {
        printf("OTA finalize failed: %d\n", err);
        return 1;
    }

    /* Simulate reboot */
    printf("\n--- Simulating Reboot ---\n");
    bootloader_check_update();

    /* Application startup and confirmation */
    printf("\n--- Application Running ---\n");
    application_confirm_boot();

    printf("\n=== OTA Update Complete ===\n");

    return 0;
}

/*
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. ALWAYS mention A/B partitioning - it's the industry standard
 *
 * 2. Emphasize security: signing, verification, anti-rollback
 *
 * 3. Power failure resilience is CRITICAL - discuss state machine
 *
 * 4. Know the trade-offs:
 *    - A/B: Simple but doubles flash requirement
 *    - Delta: Saves bandwidth but complex
 *    - Full: Simple but large downloads
 *
 * 5. Real-world examples:
 *    - Tesla OTA updates (cars)
 *    - ESP32 OTA (IoT)
 *    - Android A/B updates
 *
 * 6. Common follow-up questions:
 *    - "How do you handle partial downloads?"
 *    - "What if signature verification fails?"
 *    - "How do you prevent rollback attacks?"
 *    - "How do you minimize update time?"
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS ON OTA
 * ============================================================================
 *
 * Q: What happens if power fails during flash write?
 * A: With A/B partitioning, old firmware remains intact. On reboot,
 *    bootloader detects incomplete update and either resumes or
 *    continues with old firmware.
 *
 * Q: How do you verify firmware authenticity?
 * A: Code signing with asymmetric crypto (RSA/ECDSA). Private key
 *    signs on server, public key in bootloader verifies on device.
 *
 * Q: What's the difference between OTA and FOTA?
 * A: OTA is general term, FOTA specifically means Firmware OTA
 *    (vs. configuration or data updates).
 *
 * Q: How do you handle version compatibility?
 * A: Version header with min/max compatible versions, hardware
 *    revision check, and anti-rollback counter.
 *
 * ============================================================================
 */
