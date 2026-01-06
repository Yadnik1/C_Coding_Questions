/*
 * ============================================================================
 * QUESTION 40: ARM TrustZone - Interview Deep Dive
 * ============================================================================
 *
 * DIFFICULTY: Advanced | FREQUENCY: High | IMPORTANCE: Critical
 *
 * "Explain ARM TrustZone architecture. How does it provide hardware-based
 *  security isolation in embedded systems?"
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * TrustZone is ARM's hardware security extension that creates two isolated
 * worlds: Secure and Non-Secure. Key features:
 *
 * 1. Hardware-enforced isolation (CPU, memory, peripherals)
 * 2. Secure world runs trusted code (keys, crypto, secure boot)
 * 3. Non-secure world runs normal OS/application
 * 4. Secure Monitor manages world transitions
 *
 * Used for: secure boot, key storage, DRM, payment, TEE (Trusted Execution
 * Environment). Available in Cortex-A (ARMv7+) and Cortex-M (ARMv8-M).
 *
 * ============================================================================
 * DETAILED EXPLANATION
 * ============================================================================
 *
 * TRUSTZONE ARCHITECTURE:
 * -----------------------
 *
 *   +--------------------------------------------------+
 *   |                   HARDWARE                        |
 *   +--------------------------------------------------+
 *   |  CPU with NS bit  |  Memory  |  Peripherals      |
 *   |  (Secure/Non-Sec) |  (SAU/   |  (Secure/Non-Sec) |
 *   |                   |   IDAU)  |                   |
 *   +--------------------------------------------------+
 *                          |
 *            +-------------+-------------+
 *            |                           |
 *            v                           v
 *   +------------------+       +------------------+
 *   |   SECURE WORLD   |       | NON-SECURE WORLD |
 *   +------------------+       +------------------+
 *   | - Secure Boot    |       | - RTOS/Linux     |
 *   | - Key Storage    |       | - Applications   |
 *   | - Crypto Ops     | <---> | - UI/Network     |
 *   | - TEE            |       | - General Code   |
 *   +------------------+       +------------------+
 *           ^                          |
 *           |    Secure Monitor Call   |
 *           +--------------------------+
 *
 * NS BIT (Non-Secure Bit):
 * ------------------------
 * - Hardware signal on every bus transaction
 * - Determines which world initiated the access
 * - Cannot be spoofed by software
 * - Memory controllers use NS bit to enforce access control
 *
 * MEMORY PARTITIONING:
 * --------------------
 *
 *   Address Space
 *   +------------------------+
 *   | Secure Flash           |  Secure boot, TEE code
 *   | (NS=0 access only)     |  Keys, crypto firmware
 *   +------------------------+
 *   | Non-Secure Flash       |  RTOS/App code
 *   | (NS=0 or NS=1 access)  |
 *   +------------------------+
 *   | Secure RAM             |  Secure stack, crypto buffers
 *   | (NS=0 access only)     |  Runtime secrets
 *   +------------------------+
 *   | Non-Secure RAM         |  Normal app memory
 *   | (NS=0 or NS=1 access)  |
 *   +------------------------+
 *   | Secure Peripherals     |  Crypto engine, RNG
 *   | (NS=0 access only)     |  Secure GPIO, Timers
 *   +------------------------+
 *   | Non-Secure Peripherals |  UART, SPI, I2C
 *   +------------------------+
 *
 * ============================================================================
 * CORTEX-M TRUSTZONE (ARMv8-M)
 * ============================================================================
 *
 * SECURITY ATTRIBUTION UNIT (SAU):
 * ---------------------------------
 * - Configurable memory security regions (up to 8)
 * - Each region: base address, limit, secure/non-secure
 * - Secure code configures SAU during boot
 *
 * SAU Configuration:
 * +---------+------------+------------+-----------+
 * | Region  | Base       | Limit      | Attribute |
 * +---------+------------+------------+-----------+
 * | 0       | 0x00000000 | 0x0003FFFF | Secure    |
 * | 1       | 0x00040000 | 0x000FFFFF | Non-Sec   |
 * | 2       | 0x20000000 | 0x20007FFF | Secure    |
 * | 3       | 0x20008000 | 0x2001FFFF | Non-Sec   |
 * +---------+------------+------------+-----------+
 *
 * IMPLEMENTATION DEFINED ATTRIBUTION UNIT (IDAU):
 * ------------------------------------------------
 * - Fixed by chip vendor (not configurable)
 * - Provides default security attribution
 * - SAU can make regions less secure, not more
 *
 * STATE TRANSITIONS:
 * ------------------
 *
 *   Non-Secure State                 Secure State
 *   +-------------+                  +-------------+
 *   |             |  -- SG + NSC --> |             |
 *   | NS Code     |                  | S Code      |
 *   |             | <-- BXNS/BLXNS --|             |
 *   +-------------+                  +-------------+
 *
 *   SG = Secure Gateway instruction
 *   NSC = Non-Secure Callable region
 *   BXNS = Branch and exchange to non-secure
 *
 * ============================================================================
 * SECURE FUNCTION CALLS (NSC VENEERS)
 * ============================================================================
 *
 * Non-secure code calls secure functions via "veneers" in NSC region:
 *
 *   Non-Secure                  NSC Region              Secure
 *   +---------+                +-----------+           +--------+
 *   | BL func |  ----------->  | SG        |  ------>  | func   |
 *   |         |                | B.W func  |           | code   |
 *   |         | <------------  |           | <------   | BXNS   |
 *   +---------+                +-----------+           +--------+
 *
 * ============================================================================
 * CODE EXAMPLE: TrustZone Secure Service
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ============================================================================
 * TRUSTZONE CONFIGURATION (Simulated)
 * ============================================================================ */

/* Security states */
typedef enum {
    STATE_SECURE = 0,
    STATE_NON_SECURE = 1
} security_state_t;

/* Memory region security attributes */
typedef enum {
    MEM_SECURE,
    MEM_NON_SECURE,
    MEM_NSC              /* Non-Secure Callable */
} mem_security_t;

/* SAU Region */
typedef struct {
    uint32_t base;
    uint32_t limit;
    mem_security_t attribute;
    bool enabled;
} sau_region_t;

#define SAU_MAX_REGIONS 8

/* Simulated SAU */
static sau_region_t sau_regions[SAU_MAX_REGIONS];
static security_state_t current_state = STATE_SECURE;

/* ============================================================================
 * SAU CONFIGURATION
 * ============================================================================ */

/*
 * Say: "The SAU is configured by secure code during boot. It defines
 *       which memory regions are secure, non-secure, or non-secure callable.
 *       Once configured, the SAU enforces these boundaries in hardware."
 */

void SAU_Init(void) {
    printf("[SAU] Initializing Security Attribution Unit\n");

    /* Region 0: Secure Flash (0x00000000 - 0x0003FFFF) */
    sau_regions[0].base = 0x00000000;
    sau_regions[0].limit = 0x0003FFFF;
    sau_regions[0].attribute = MEM_SECURE;
    sau_regions[0].enabled = true;

    /* Region 1: NSC Region (0x0003F000 - 0x0003FFFF) - Secure Gateway */
    sau_regions[1].base = 0x0003F000;
    sau_regions[1].limit = 0x0003FFFF;
    sau_regions[1].attribute = MEM_NSC;
    sau_regions[1].enabled = true;

    /* Region 2: Non-Secure Flash (0x00040000 - 0x000FFFFF) */
    sau_regions[2].base = 0x00040000;
    sau_regions[2].limit = 0x000FFFFF;
    sau_regions[2].attribute = MEM_NON_SECURE;
    sau_regions[2].enabled = true;

    /* Region 3: Secure RAM (0x20000000 - 0x20007FFF) */
    sau_regions[3].base = 0x20000000;
    sau_regions[3].limit = 0x20007FFF;
    sau_regions[3].attribute = MEM_SECURE;
    sau_regions[3].enabled = true;

    /* Region 4: Non-Secure RAM (0x20008000 - 0x2001FFFF) */
    sau_regions[4].base = 0x20008000;
    sau_regions[4].limit = 0x2001FFFF;
    sau_regions[4].attribute = MEM_NON_SECURE;
    sau_regions[4].enabled = true;

    /*
     * On real hardware:
     * SAU->RNR = region_number;
     * SAU->RBAR = base_address;
     * SAU->RLAR = (limit_address & ~0x1F) | (nsc << 1) | 1;
     * SAU->CTRL = SAU_CTRL_ENABLE;
     */

    printf("[SAU] Configuration complete:\n");
    for (int i = 0; i < 5; i++) {
        printf("  Region %d: 0x%08X - 0x%08X [%s]\n",
               i, sau_regions[i].base, sau_regions[i].limit,
               sau_regions[i].attribute == MEM_SECURE ? "SECURE" :
               sau_regions[i].attribute == MEM_NSC ? "NSC" : "NON-SECURE");
    }
}

/* Check if address is accessible from current security state */
bool check_memory_access(uint32_t address, bool is_write) {
    for (int i = 0; i < SAU_MAX_REGIONS; i++) {
        if (!sau_regions[i].enabled) continue;

        if (address >= sau_regions[i].base &&
            address <= sau_regions[i].limit) {

            /* Secure state can access everything */
            if (current_state == STATE_SECURE) return true;

            /* Non-secure can only access non-secure regions */
            if (sau_regions[i].attribute == MEM_SECURE) {
                printf("[FAULT] Non-secure access to secure region!\n");
                return false;
            }
            return true;
        }
    }

    /* Default: check IDAU (chip-specific) */
    return (current_state == STATE_SECURE);
}

/* ============================================================================
 * SECURE SERVICES (TEE Implementation)
 * ============================================================================ */

/*
 * Say: "Secure services are implemented in the secure world and exposed
 *       to non-secure code via Non-Secure Callable (NSC) entry points.
 *       Each service validates inputs before processing."
 */

/* Simulated secure key storage */
#define MAX_KEYS 8
#define KEY_SIZE 32

typedef struct {
    uint8_t key[KEY_SIZE];
    uint32_t key_id;
    bool in_use;
} secure_key_t;

static secure_key_t key_storage[MAX_KEYS];

/* Secure service: Store key (called from non-secure) */

/*
 * Say: "The __attribute__((cmse_nonsecure_entry)) tells the compiler to
 *       generate a secure gateway veneer. This is the only way non-secure
 *       code can enter secure functions."
 */

/* In real code: __attribute__((cmse_nonsecure_entry)) */
int32_t secure_store_key(uint32_t key_id, const uint8_t *key_data) {
    printf("[SECURE] store_key called (id=%u)\n", key_id);

    /* Validate pointer is in non-secure memory */
    /* cmse_check_address_range(key_data, KEY_SIZE, CMSE_NONSECURE); */

    /* Find empty slot */
    for (int i = 0; i < MAX_KEYS; i++) {
        if (!key_storage[i].in_use) {
            key_storage[i].key_id = key_id;
            memcpy(key_storage[i].key, key_data, KEY_SIZE);
            key_storage[i].in_use = true;
            printf("[SECURE] Key stored in slot %d\n", i);
            return 0;  /* Success */
        }
    }

    return -1;  /* No space */
}

/* Secure service: Encrypt data using stored key */
int32_t secure_encrypt(uint32_t key_id,
                       const uint8_t *plaintext, uint32_t len,
                       uint8_t *ciphertext) {
    printf("[SECURE] encrypt called (key_id=%u, len=%u)\n", key_id, len);

    /* Find key */
    secure_key_t *key = NULL;
    for (int i = 0; i < MAX_KEYS; i++) {
        if (key_storage[i].in_use && key_storage[i].key_id == key_id) {
            key = &key_storage[i];
            break;
        }
    }

    if (!key) {
        printf("[SECURE] Key not found!\n");
        return -1;
    }

    /* Simple XOR "encryption" for demo (use real crypto in production!) */
    for (uint32_t i = 0; i < len; i++) {
        ciphertext[i] = plaintext[i] ^ key->key[i % KEY_SIZE];
    }

    printf("[SECURE] Encryption complete\n");
    return 0;
}

/* Secure service: Get random number from hardware RNG */
int32_t secure_get_random(uint8_t *buffer, uint32_t len) {
    printf("[SECURE] get_random called (len=%u)\n", len);

    /*
     * On real hardware:
     * while (!(RNG->SR & RNG_SR_DRDY));
     * *buffer++ = RNG->DR;
     */

    /* Simulated random */
    for (uint32_t i = 0; i < len; i++) {
        buffer[i] = (uint8_t)(i * 17 + 42);  /* Not actually random! */
    }

    return 0;
}

/* ============================================================================
 * SECURE BOOT VERIFICATION
 * ============================================================================ */

/*
 * Say: "Secure boot runs entirely in secure world. It verifies the
 *       non-secure image signature before allowing execution. The
 *       verification key is stored in secure memory or OTP fuses."
 */

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t image_size;
    uint8_t  signature[64];
} ns_image_header_t;

#define NS_IMAGE_MAGIC 0x4E534543  /* "NSEC" */

bool secure_verify_ns_image(const ns_image_header_t *header,
                            const uint8_t *image) {
    printf("[SECURE_BOOT] Verifying non-secure image...\n");

    /* Check magic */
    if (header->magic != NS_IMAGE_MAGIC) {
        printf("[SECURE_BOOT] Invalid magic!\n");
        return false;
    }

    /* In production: verify ECDSA/RSA signature using secure key */
    /* bool valid = ecdsa_verify(image, header->image_size,
     *                           header->signature, public_key);
     */

    printf("[SECURE_BOOT] Image verified successfully\n");
    return true;
}

/* ============================================================================
 * STATE TRANSITIONS AND SECURE MONITOR
 * ============================================================================ */

/*
 * Say: "On Cortex-A, the Secure Monitor (EL3) handles world switches.
 *       On Cortex-M with TrustZone-M, transitions happen via SG instruction
 *       and BXNS/BLXNS instructions."
 */

/* Cortex-A: Secure Monitor Call handler */
typedef enum {
    SMC_SECURE_BOOT_VERIFY = 0x100,
    SMC_GET_RANDOM = 0x101,
    SMC_ENCRYPT = 0x102,
    SMC_DECRYPT = 0x103,
    SMC_STORE_KEY = 0x104
} smc_function_id_t;

int32_t secure_monitor_call(uint32_t func_id, uint32_t arg0,
                            uint32_t arg1, uint32_t arg2) {
    printf("[SMC] Function 0x%X called\n", func_id);

    switch (func_id) {
        case SMC_GET_RANDOM:
            return secure_get_random((uint8_t *)arg0, arg1);

        case SMC_ENCRYPT:
            return secure_encrypt(arg0, (uint8_t *)arg1, arg2, (uint8_t *)arg1);

        case SMC_STORE_KEY:
            return secure_store_key(arg0, (uint8_t *)arg1);

        default:
            printf("[SMC] Unknown function!\n");
            return -1;
    }
}

/* ============================================================================
 * NON-SECURE CODE SIMULATION
 * ============================================================================ */

/*
 * Say: "Non-secure code runs normally but cannot access secure resources
 *       directly. It must use secure services via SMC (Cortex-A) or
 *       NSC entry points (Cortex-M)."
 */

void non_secure_application(void) {
    printf("\n=== Non-Secure Application ===\n\n");

    /* Switch to non-secure state for simulation */
    current_state = STATE_NON_SECURE;
    printf("[NS] Running in non-secure state\n");

    /* Try to access secure memory (will fail) */
    printf("\n[NS] Attempting to read secure memory...\n");
    if (!check_memory_access(0x00010000, false)) {
        printf("[NS] Access denied (expected)\n");
    }

    /* Call secure services (allowed via SMC/NSC) */
    printf("\n[NS] Calling secure services...\n");

    /* Store a key */
    uint8_t my_key[KEY_SIZE] = {0x01, 0x02, 0x03, 0x04};  /* Truncated for brevity */
    current_state = STATE_SECURE;  /* Simulate SMC transition */
    secure_store_key(1, my_key);
    current_state = STATE_NON_SECURE;

    /* Encrypt data */
    uint8_t plaintext[] = "Hello, TrustZone!";
    uint8_t ciphertext[32];
    current_state = STATE_SECURE;
    secure_encrypt(1, plaintext, sizeof(plaintext), ciphertext);
    current_state = STATE_NON_SECURE;

    printf("[NS] Encryption done via secure service\n");

    /* Get random number */
    uint8_t random_buf[16];
    current_state = STATE_SECURE;
    secure_get_random(random_buf, sizeof(random_buf));
    current_state = STATE_NON_SECURE;

    printf("[NS] Got random data via secure service\n");

    printf("\n[NS] Application complete\n");
}

/* ============================================================================
 * TRUSTZONE-M SPECIFIC: CMSE FUNCTIONS
 * ============================================================================ */

/*
 * Say: "CMSE (C library support for Cortex-M Security Extensions) provides
 *       functions for checking pointer security and creating non-secure
 *       function pointers."
 */

/*
 * In real TrustZone-M code:
 *
 * #include <arm_cmse.h>
 *
 * // Check if pointer is in non-secure memory
 * void *cmse_check_address_range(void *p, size_t s, int flags);
 *
 * // Check if address is non-secure callable
 * void *cmse_is_nsc(void *p);
 *
 * // Create non-secure function pointer
 * typedef void (*ns_func_t)(void) __attribute__((cmse_nonsecure_call));
 * ns_func_t ns_entry = (ns_func_t)cmse_nsfptr_create(ns_func);
 */

/* Example: Secure function that calls back to non-secure */
typedef void (*ns_callback_t)(uint32_t result);

void secure_async_operation(ns_callback_t callback) {
    printf("[SECURE] Starting async operation...\n");

    /*
     * In real code:
     * // Verify callback is a valid non-secure address
     * ns_callback_t safe_callback = cmse_nsfptr_create(callback);
     * if (safe_callback == NULL) {
     *     return; // Invalid callback
     * }
     *
     * // Do secure operation...
     * uint32_t result = do_secure_work();
     *
     * // Call back to non-secure (BLXNS)
     * safe_callback(result);
     */

    printf("[SECURE] Async operation complete\n");
}

/* ============================================================================
 * FAULT HANDLING
 * ============================================================================ */

/*
 * Say: "Security faults trigger SecureFault exception (priority -1, just
 *       below HardFault). The secure fault handler must not leak information
 *       to the non-secure world."
 */

void SecureFault_Handler(void) {
    printf("[FAULT] SecureFault triggered!\n");

    /*
     * Check SFSR (SecureFault Status Register) for cause:
     * - INVEP: Invalid entry point
     * - INVIS: Invalid transition
     * - INVER: Invalid exception return
     * - AUVIOL: Attribution unit violation
     * - INVTRAN: Invalid transition from secure
     * - LSPERR: Lazy state preservation error
     * - LSERR: Lazy state error
     *
     * uint32_t sfsr = SAU->SFSR;
     * uint32_t sfar = SAU->SFAR;  // Faulting address
     *
     * // Log fault (to secure-only log)
     * // Reset system or enter safe state
     */

    printf("[FAULT] System reset required\n");
    /* NVIC_SystemReset(); */
}

/* ============================================================================
 * MAIN DEMONSTRATION
 * ============================================================================ */

int main() {
    printf("========================================\n");
    printf("  ARM TrustZone Security Demo\n");
    printf("========================================\n\n");

    /* Phase 1: Secure boot (runs in secure state) */
    printf("=== Phase 1: Secure Boot ===\n\n");
    current_state = STATE_SECURE;

    /* Initialize SAU */
    SAU_Init();

    /* Verify non-secure image (would be loaded from flash) */
    ns_image_header_t ns_header = {
        .magic = NS_IMAGE_MAGIC,
        .version = 0x010000,
        .image_size = 1024
    };
    printf("\n");
    secure_verify_ns_image(&ns_header, NULL);

    /* Phase 2: Start non-secure application */
    printf("\n=== Phase 2: Non-Secure Execution ===\n");
    non_secure_application();

    /* Phase 3: Show security violation */
    printf("\n=== Phase 3: Security Violation Demo ===\n\n");
    current_state = STATE_NON_SECURE;
    printf("[NS] Attempting to access secure peripheral...\n");
    if (!check_memory_access(0x40000000, true)) {
        printf("[NS] Access blocked by TrustZone!\n");
    }

    printf("\n========================================\n");
    printf("  Demo Complete\n");
    printf("========================================\n");

    return 0;
}

/*
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. Understand the two worlds: Secure (trusted) vs Non-Secure (normal)
 *
 * 2. Know the NS bit: hardware signal, cannot be spoofed
 *
 * 3. Explain SAU/IDAU: How memory security is configured
 *
 * 4. Know NSC (Non-Secure Callable): Entry points for secure functions
 *
 * 5. Understand secure boot: Chain of trust, verification in secure world
 *
 * 6. Be familiar with use cases:
 *    - TEE (Trusted Execution Environment)
 *    - Key storage and crypto operations
 *    - DRM (Digital Rights Management)
 *    - Secure payments
 *    - Secure firmware updates
 *
 * 7. Know the limitations:
 *    - Side-channel attacks still possible
 *    - Secure code bugs can compromise system
 *    - Needs careful design of secure/non-secure interface
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS
 * ============================================================================
 *
 * Q: What's the difference between TrustZone-A and TrustZone-M?
 * A: TrustZone-A (Cortex-A): Uses Secure Monitor (EL3), ATF, large TCB
 *    TrustZone-M (Cortex-M): Simpler, no monitor, SAU-based, smaller TCB
 *
 * Q: How does non-secure code call secure functions?
 * A: Cortex-A: SMC (Secure Monitor Call) instruction
 *    Cortex-M: Branch to NSC region containing SG + branch to secure func
 *
 * Q: What is a TEE?
 * A: Trusted Execution Environment - secure OS running in secure world
 *    Examples: OP-TEE, Trusty, Qualcomm QSEE
 *
 * Q: Can secure world access non-secure memory?
 * A: Yes, secure world can access everything. Non-secure cannot access
 *    secure resources.
 *
 * Q: What happens on security violation?
 * A: SecureFault exception (Cortex-M) or abort (Cortex-A). System must
 *    handle gracefully without leaking secure information.
 *
 * Q: How do you protect against attacks?
 * A: Minimize secure attack surface, validate all inputs, use secure
 *    coding practices, implement secure boot, keep secure code small.
 *
 * ============================================================================
 * KEY CONCEPTS SUMMARY
 * ============================================================================
 *
 * +------------------+-------------------------+------------------------+
 * | Concept          | Cortex-A                | Cortex-M               |
 * +------------------+-------------------------+------------------------+
 * | Security States  | Secure, Non-Secure      | Secure, Non-Secure     |
 * | Exception Levels | EL0-EL3                 | Handler, Thread mode   |
 * | Monitor          | EL3 Secure Monitor      | No monitor (direct)    |
 * | World Switch     | SMC instruction         | SG/BXNS instructions   |
 * | Memory Config    | TZASC, TZC-400          | SAU + IDAU             |
 * | Typical Use      | TEE, Secure OS          | IoT, Secure Services   |
 * +------------------+-------------------------+------------------------+
 *
 * ============================================================================
 */
