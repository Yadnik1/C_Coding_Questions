/*
 * ============================================================================
 * QUESTION 39: Bootloaders in Embedded Systems - Interview Deep Dive
 * ============================================================================
 *
 * DIFFICULTY: Advanced | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * "Explain the bootloader architecture in embedded systems. How does the
 *  boot process work from power-on to application execution?"
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * A bootloader is the first code that runs after reset. It:
 * 1. Initializes minimal hardware (clocks, memory)
 * 2. Validates application image integrity
 * 3. Copies/decompresses code if needed
 * 4. Jumps to application entry point
 *
 * Multi-stage bootloaders (ROM -> SPL -> U-Boot -> Kernel) are common
 * for complex systems. Key features: secure boot, update capability,
 * recovery mode, and hardware abstraction.
 *
 * ============================================================================
 * DETAILED EXPLANATION
 * ============================================================================
 *
 * BOOT SEQUENCE OVERVIEW:
 * -----------------------
 *
 *   Power On
 *      |
 *      v
 * +------------+
 * | Reset      |  CPU starts from reset vector
 * | Vector     |  (typically 0x00000000 or 0x08000000)
 * +------------+
 *      |
 *      v
 * +------------+
 * | ROM Boot   |  Vendor-provided, immutable
 * | (Optional) |  Checks boot pins, loads SPL
 * +------------+
 *      |
 *      v
 * +------------+
 * | Bootloader |  Your code: validates, updates, jumps
 * | (SPL/Main) |
 * +------------+
 *      |
 *      v
 * +------------+
 * | Application|  Main firmware
 * +------------+
 *
 * SINGLE-STAGE vs MULTI-STAGE:
 * ----------------------------
 *
 * Single-Stage (Microcontrollers):
 * +---------------+
 * | Bootloader    |  8-32KB, does everything
 * +---------------+
 * | Application   |  Main firmware
 * +---------------+
 *
 * Multi-Stage (Complex SoCs):
 * +---------------+
 * | ROM Bootloader|  In silicon, immutable
 * +---------------+
 * | SPL (MLO)     |  Secondary Program Loader
 * +---------------+         |
 * | U-Boot/Main   | <-------+ Loads from storage
 * +---------------+
 * | Kernel/RTOS   |
 * +---------------+
 *
 * WHY MULTI-STAGE?
 * - ROM can't hold full bootloader
 * - SPL initializes DRAM, then loads main bootloader
 * - Each stage has more resources than previous
 *
 * ============================================================================
 * BOOTLOADER RESPONSIBILITIES
 * ============================================================================
 *
 * 1. HARDWARE INITIALIZATION:
 *    - Clock configuration (PLL, oscillators)
 *    - Memory controller (SDRAM timing)
 *    - Basic peripherals (UART for debug)
 *    - Disable watchdog (or configure it)
 *
 * 2. IMAGE VALIDATION:
 *    - Check magic number / header
 *    - Verify CRC or hash
 *    - Signature verification (secure boot)
 *    - Version compatibility check
 *
 * 3. IMAGE LOADING:
 *    - Copy from flash to RAM (if XIP not used)
 *    - Decompress if compressed
 *    - Relocate if position-independent
 *
 * 4. JUMP TO APPLICATION:
 *    - Set stack pointer
 *    - Clear .bss section (optional)
 *    - Call application entry point
 *
 * ============================================================================
 * MEMORY MAP EXAMPLE (STM32)
 * ============================================================================
 *
 *    0x08000000  +------------------+
 *                | Vector Table     |  Initial SP, Reset Handler
 *                +------------------+
 *                | Bootloader Code  |  16-32KB typical
 *                |                  |
 *    0x08008000  +------------------+  <-- Application Start
 *                | App Vector Table |  Application's vectors
 *                +------------------+
 *                | Application Code |
 *                |                  |
 *                |                  |
 *    0x080FFFFF  +------------------+
 *
 *    0x20000000  +------------------+
 *                | RAM              |  Stack, heap, variables
 *    0x20020000  +------------------+
 *
 * VECTOR TABLE STRUCTURE:
 * -----------------------
 * Offset 0x00: Initial Stack Pointer
 * Offset 0x04: Reset Handler address
 * Offset 0x08: NMI Handler
 * Offset 0x0C: HardFault Handler
 * ... (other exception handlers)
 *
 * ============================================================================
 * SECURE BOOT CHAIN
 * ============================================================================
 *
 * Chain of Trust:
 * ---------------
 *
 * +-------------+     +-------------+     +-------------+
 * | ROM (Root   | --> | Bootloader  | --> | Application |
 * | of Trust)   |     | (Signed)    |     | (Signed)    |
 * +-------------+     +-------------+     +-------------+
 *       |                   |                   |
 *    [Immutable]        [Verified by       [Verified by
 *    [Has Root Key]      ROM Key]           BL Key]
 *
 * SECURE BOOT PROCESS:
 * --------------------
 * 1. ROM reads bootloader from flash
 * 2. ROM verifies BL signature with OTP public key
 * 3. If valid, ROM jumps to bootloader
 * 4. Bootloader reads application from flash
 * 5. Bootloader verifies app signature with its key
 * 6. If valid, bootloader jumps to application
 *
 * KEY STORAGE:
 * - Root public key: OTP (One-Time Programmable) fuses
 * - Bootloader key: Signed into bootloader binary
 * - Application key: Signed into application binary
 *
 * ============================================================================
 * CODE EXAMPLE: Simple Bootloader
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ============================================================================
 * HARDWARE ABSTRACTION (Simulated)
 * ============================================================================ */

#define FLASH_BASE          0x08000000
#define BOOTLOADER_SIZE     0x8000      /* 32KB */
#define APP_START_ADDRESS   (FLASH_BASE + BOOTLOADER_SIZE)
#define APP_MAX_SIZE        0x78000     /* 480KB */
#define RAM_BASE            0x20000000
#define RAM_SIZE            0x20000     /* 128KB */

/* Simulated memory */
static uint8_t flash_memory[512 * 1024];
static uint8_t ram_memory[128 * 1024];

/* Application image header */
typedef struct {
    uint32_t magic;             /* 0xAPP12345 */
    uint32_t version;
    uint32_t image_size;
    uint32_t entry_point;       /* Offset from image start */
    uint32_t load_address;      /* Where to load in RAM */
    uint32_t crc32;
    uint32_t flags;             /* Compression, encryption flags */
    uint8_t  signature[64];     /* ECDSA-P256 signature */
} app_header_t;

#define APP_MAGIC       0xAPP12345
#define FLAG_COMPRESSED 0x01
#define FLAG_ENCRYPTED  0x02
#define FLAG_XIP        0x04    /* Execute In Place */

/* ============================================================================
 * LOW-LEVEL INITIALIZATION
 * ============================================================================ */

/*
 * Say: "The startup code runs before main(). It sets up the stack,
 *       initializes .data from flash, zeros .bss, and may initialize
 *       the C runtime."
 */

/* Reset handler - first code to run */
void Reset_Handler(void) {
    /* 1. Set stack pointer (done by hardware on Cortex-M) */
    /* 2. Copy .data section from flash to RAM */
    /* 3. Zero .bss section */
    /* 4. Call SystemInit for clock setup */
    /* 5. Call main() */
    printf("[STARTUP] Reset_Handler executing\n");
}

/* Clock initialization */
void SystemInit(void) {
    printf("[INIT] Configuring clocks...\n");
    /* Configure PLL, set system clock, peripheral clocks */
    /* Example for STM32:
     * - Enable HSE (external crystal)
     * - Configure PLL multipliers
     * - Switch system clock to PLL
     * - Set flash wait states
     */
}

/* ============================================================================
 * BOOTLOADER CORE FUNCTIONS
 * ============================================================================ */

/*
 * Say: "CRC32 is commonly used for integrity checks. For security-critical
 *       applications, we use SHA-256 and cryptographic signatures."
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

/*
 * Say: "Image validation checks header integrity, version compatibility,
 *       and cryptographic signature. This prevents running corrupted
 *       or malicious firmware."
 */
static bool validate_image(const app_header_t *header, const uint8_t *image) {
    printf("[BL] Validating application image...\n");

    /* Check magic number */
    if (header->magic != APP_MAGIC) {
        printf("[BL] ERROR: Invalid magic (got 0x%08X)\n", header->magic);
        return false;
    }

    /* Check size */
    if (header->image_size > APP_MAX_SIZE) {
        printf("[BL] ERROR: Image too large (%u bytes)\n", header->image_size);
        return false;
    }

    /* Verify CRC */
    uint32_t calculated = calculate_crc32(image, header->image_size);
    if (calculated != header->crc32) {
        printf("[BL] ERROR: CRC mismatch (expected 0x%08X, got 0x%08X)\n",
               header->crc32, calculated);
        return false;
    }

    /* In production: verify cryptographic signature here */
    printf("[BL] Image validation passed\n");
    return true;
}

/*
 * Say: "Jumping to the application requires setting up the vector table,
 *       stack pointer, and calling the reset handler. On Cortex-M, we
 *       also need to relocate the VTOR register."
 */

/* Function pointer type for application entry */
typedef void (*app_entry_t)(void);

static void jump_to_application(uint32_t app_address) {
    printf("[BL] Jumping to application at 0x%08X\n", app_address);

    /*
     * On real hardware (Cortex-M):
     *
     * // Get application's initial stack pointer (first word)
     * uint32_t app_sp = *(volatile uint32_t*)app_address;
     *
     * // Get application's reset handler (second word)
     * uint32_t app_reset = *(volatile uint32_t*)(app_address + 4);
     *
     * // Relocate vector table
     * SCB->VTOR = app_address;
     *
     * // Set stack pointer
     * __set_MSP(app_sp);
     *
     * // Jump to application
     * app_entry_t app_entry = (app_entry_t)app_reset;
     * app_entry();
     */

    /* Simulation */
    printf("[BL] Application would start executing here\n");
}

/* ============================================================================
 * BOOT MODE SELECTION
 * ============================================================================ */

typedef enum {
    BOOT_MODE_NORMAL,
    BOOT_MODE_UPDATE,
    BOOT_MODE_RECOVERY,
    BOOT_MODE_DFU         /* Device Firmware Update */
} boot_mode_t;

/*
 * Say: "Boot mode is determined by checking GPIO pins, flags in flash,
 *       or special patterns in RAM that survive soft reset. This allows
 *       entering update or recovery mode when needed."
 */
static boot_mode_t determine_boot_mode(void) {
    printf("[BL] Checking boot mode...\n");

    /* Method 1: GPIO pin (e.g., button held during boot) */
    /* if (GPIO_ReadPin(BOOT_PIN) == LOW) return BOOT_MODE_DFU; */

    /* Method 2: Flag in RAM (set by application before reset) */
    static const uint32_t MAGIC_UPDATE = 0xUPD8;
    static const uint32_t MAGIC_RECOVERY = 0xREC0;
    uint32_t boot_flag = 0;  /* Read from RAM */

    if (boot_flag == MAGIC_UPDATE) {
        printf("[BL] Update mode requested\n");
        return BOOT_MODE_UPDATE;
    }
    if (boot_flag == MAGIC_RECOVERY) {
        printf("[BL] Recovery mode requested\n");
        return BOOT_MODE_RECOVERY;
    }

    /* Method 3: Check if application is valid */
    /* If not valid, enter recovery */

    return BOOT_MODE_NORMAL;
}

/* ============================================================================
 * UPDATE/DFU MODE
 * ============================================================================ */

/*
 * Say: "DFU (Device Firmware Update) mode allows firmware updates via
 *       USB, UART, or other interfaces. The bootloader implements the
 *       protocol to receive and program new firmware."
 */

typedef enum {
    DFU_STATE_IDLE,
    DFU_STATE_RECEIVING,
    DFU_STATE_VERIFYING,
    DFU_STATE_PROGRAMMING,
    DFU_STATE_COMPLETE,
    DFU_STATE_ERROR
} dfu_state_t;

typedef struct {
    dfu_state_t state;
    uint32_t bytes_received;
    uint32_t total_size;
    uint32_t current_address;
} dfu_context_t;

static dfu_context_t dfu_ctx;

void dfu_init(void) {
    printf("[DFU] Entering DFU mode\n");
    printf("[DFU] Waiting for firmware on UART/USB...\n");

    memset(&dfu_ctx, 0, sizeof(dfu_ctx));
    dfu_ctx.state = DFU_STATE_IDLE;

    /* Initialize communication interface */
    /* UART_Init(115200); */
    /* USB_DFU_Init(); */
}

void dfu_receive_packet(const uint8_t *data, uint32_t len) {
    /* Protocol handling: start, data, end packets */
    /* Program to flash, verify each page */
}

/* ============================================================================
 * WATCHDOG HANDLING
 * ============================================================================ */

/*
 * Say: "The watchdog must be handled carefully in bootloaders. Some systems
 *       enable watchdog in ROM, so we must either feed it or disable it
 *       during long operations like flash programming."
 */

void watchdog_init(void) {
    printf("[WDG] Configuring watchdog\n");
    /* Options:
     * 1. Disable if allowed (not recommended for production)
     * 2. Set long timeout for bootloader operations
     * 3. Feed periodically during flash operations
     */
}

void watchdog_feed(void) {
    /* Reset watchdog timer */
    /* WDG->REFRESH = 0xAAAA; */
    /* WDG->REFRESH = 0x5555; */
}

/* ============================================================================
 * FLASH PROGRAMMING
 * ============================================================================ */

/*
 * Say: "Flash programming requires specific sequences: unlock, erase,
 *       program, lock. Each step may require feeding the watchdog
 *       and checking for errors."
 */

typedef enum {
    FLASH_OK,
    FLASH_ERROR_UNLOCK,
    FLASH_ERROR_ERASE,
    FLASH_ERROR_PROGRAM,
    FLASH_ERROR_VERIFY
} flash_status_t;

flash_status_t flash_unlock(void) {
    printf("[FLASH] Unlocking flash\n");
    /*
     * STM32 example:
     * FLASH->KEYR = 0x45670123;
     * FLASH->KEYR = 0xCDEF89AB;
     */
    return FLASH_OK;
}

flash_status_t flash_lock(void) {
    printf("[FLASH] Locking flash\n");
    /*
     * FLASH->CR |= FLASH_CR_LOCK;
     */
    return FLASH_OK;
}

flash_status_t flash_erase_sector(uint32_t sector) {
    printf("[FLASH] Erasing sector %u\n", sector);
    watchdog_feed();

    /*
     * STM32 example:
     * FLASH->CR &= ~FLASH_CR_PSIZE;
     * FLASH->CR |= FLASH_PSIZE_WORD;
     * FLASH->CR &= ~FLASH_CR_SNB;
     * FLASH->CR |= (sector << FLASH_CR_SNB_Pos);
     * FLASH->CR |= FLASH_CR_SER;
     * FLASH->CR |= FLASH_CR_STRT;
     * while (FLASH->SR & FLASH_SR_BSY);
     */

    return FLASH_OK;
}

flash_status_t flash_program_word(uint32_t address, uint32_t data) {
    /*
     * FLASH->CR &= ~FLASH_CR_PSIZE;
     * FLASH->CR |= FLASH_PSIZE_WORD;
     * FLASH->CR |= FLASH_CR_PG;
     * *(volatile uint32_t*)address = data;
     * while (FLASH->SR & FLASH_SR_BSY);
     * FLASH->CR &= ~FLASH_CR_PG;
     */
    return FLASH_OK;
}

/* ============================================================================
 * COMPRESSION SUPPORT
 * ============================================================================ */

/*
 * Say: "Compressed images save flash space. The bootloader decompresses
 *       to RAM before execution. LZ4 is popular for embedded due to
 *       fast decompression with minimal memory."
 */

/* Simplified RLE decompression for demonstration */
uint32_t decompress_image(const uint8_t *compressed, uint32_t comp_size,
                          uint8_t *output, uint32_t max_output) {
    printf("[BL] Decompressing image...\n");

    /* Real implementation would use LZ4, LZMA, or similar */
    /* This is just a placeholder */
    if (comp_size <= max_output) {
        memcpy(output, compressed, comp_size);
        return comp_size;
    }
    return 0;
}

/* ============================================================================
 * MAIN BOOTLOADER LOGIC
 * ============================================================================ */

void bootloader_main(void) {
    printf("\n========================================\n");
    printf("  BOOTLOADER v1.0.0\n");
    printf("========================================\n\n");

    /* Step 1: Basic hardware init */
    SystemInit();
    watchdog_init();

    /* Step 2: Determine boot mode */
    boot_mode_t mode = determine_boot_mode();

    switch (mode) {
        case BOOT_MODE_DFU:
            dfu_init();
            /* DFU loop would run here */
            printf("[DFU] Would wait for firmware update...\n");
            return;

        case BOOT_MODE_RECOVERY:
            printf("[BL] Entering recovery mode\n");
            /* Load recovery image or minimal shell */
            return;

        case BOOT_MODE_UPDATE:
            printf("[BL] Processing pending update\n");
            /* Copy from update partition to main partition */
            break;

        case BOOT_MODE_NORMAL:
        default:
            printf("[BL] Normal boot\n");
            break;
    }

    /* Step 3: Validate application */
    printf("\n[BL] Checking application at 0x%08X\n", APP_START_ADDRESS);

    /* Create simulated application */
    app_header_t *app_header = (app_header_t *)&flash_memory[BOOTLOADER_SIZE];
    app_header->magic = APP_MAGIC;
    app_header->version = 0x010000;  /* 1.0.0 */
    app_header->image_size = 1024;
    app_header->entry_point = sizeof(app_header_t) + 4;  /* After header */
    app_header->load_address = RAM_BASE;
    app_header->flags = 0;

    /* Fill some dummy code */
    for (int i = 0; i < 1024; i++) {
        flash_memory[BOOTLOADER_SIZE + sizeof(app_header_t) + i] = i & 0xFF;
    }
    app_header->crc32 = calculate_crc32(
        &flash_memory[BOOTLOADER_SIZE + sizeof(app_header_t)],
        app_header->image_size
    );

    uint8_t *app_image = &flash_memory[BOOTLOADER_SIZE + sizeof(app_header_t)];

    if (!validate_image(app_header, app_image)) {
        printf("[BL] ERROR: No valid application!\n");
        printf("[BL] Entering recovery mode...\n");
        dfu_init();
        return;
    }

    /* Step 4: Load application if needed */
    if (!(app_header->flags & FLAG_XIP)) {
        printf("[BL] Loading application to RAM\n");

        if (app_header->flags & FLAG_COMPRESSED) {
            decompress_image(app_image, app_header->image_size,
                           ram_memory, RAM_SIZE);
        } else {
            memcpy(ram_memory, app_image, app_header->image_size);
        }
    }

    /* Step 5: Jump to application */
    printf("\n[BL] Starting application...\n");
    printf("========================================\n\n");

    jump_to_application(APP_START_ADDRESS);
}

/* ============================================================================
 * VECTOR TABLE AND STARTUP
 * ============================================================================ */

/*
 * Say: "The vector table must be at a specific address (usually 0x00000000
 *       or as specified by VTOR). It contains the initial stack pointer
 *       and exception handler addresses."
 */

/* Vector table structure for Cortex-M */
typedef void (*vector_entry_t)(void);

typedef struct {
    uint32_t       *initial_sp;
    vector_entry_t reset_handler;
    vector_entry_t nmi_handler;
    vector_entry_t hardfault_handler;
    vector_entry_t memmanage_handler;
    vector_entry_t busfault_handler;
    vector_entry_t usagefault_handler;
    uint32_t       reserved[4];
    vector_entry_t svc_handler;
    vector_entry_t debugmon_handler;
    uint32_t       reserved2;
    vector_entry_t pendsv_handler;
    vector_entry_t systick_handler;
    /* IRQ handlers follow... */
} vector_table_t;

/*
 * In real code, this would be:
 *
 * __attribute__((section(".isr_vector")))
 * const vector_table_t vector_table = {
 *     .initial_sp = &_estack,
 *     .reset_handler = Reset_Handler,
 *     .nmi_handler = NMI_Handler,
 *     ...
 * };
 */

/* ============================================================================
 * LINKER SCRIPT CONSIDERATIONS
 * ============================================================================
 *
 * Bootloader linker script must define:
 * - FLASH origin and length (bootloader region only)
 * - RAM origin and length
 * - .isr_vector section at flash start
 * - .text, .rodata, .data, .bss sections
 * - _estack symbol for stack top
 *
 * Example:
 *
 * MEMORY
 * {
 *     FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 32K
 *     RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 128K
 * }
 *
 * SECTIONS
 * {
 *     .isr_vector : {
 *         . = ALIGN(4);
 *         KEEP(*(.isr_vector))
 *         . = ALIGN(4);
 *     } > FLASH
 *
 *     .text : {
 *         . = ALIGN(4);
 *         *(.text*)
 *         *(.rodata*)
 *         . = ALIGN(4);
 *     } > FLASH
 *
 *     .data : {
 *         _sdata = .;
 *         *(.data*)
 *         _edata = .;
 *     } > RAM AT > FLASH
 *
 *     .bss : {
 *         _sbss = .;
 *         *(.bss*)
 *         _ebss = .;
 *     } > RAM
 * }
 *
 * ============================================================================
 */

int main() {
    printf("=== Bootloader Interview Demo ===\n");
    bootloader_main();
    return 0;
}

/*
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. Know the boot sequence: Reset -> Vector Table -> Reset Handler ->
 *    SystemInit -> main/bootloader
 *
 * 2. Understand vector table structure and VTOR relocation
 *
 * 3. Explain secure boot chain of trust
 *
 * 4. Discuss A/B partitioning for reliable updates
 *
 * 5. Know watchdog handling during flash operations
 *
 * 6. Be familiar with common protocols: DFU, UART bootloader, SWD
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS
 * ============================================================================
 *
 * Q: How do you jump from bootloader to application?
 * A: Read application's stack pointer (first word of vector table),
 *    read reset handler address (second word), set VTOR to application
 *    base, set MSP to application's SP, jump to reset handler.
 *
 * Q: What is secure boot?
 * A: Chain of trust where each stage verifies the next. ROM verifies
 *    bootloader signature, bootloader verifies application signature.
 *    Root of trust is immutable (ROM/OTP).
 *
 * Q: How do you handle bootloader updates?
 * A: Very carefully! Options: A/B bootloader, golden bootloader in
 *    protected region, or use ROM's built-in update capability.
 *
 * Q: What's the difference between XIP and load-to-RAM?
 * A: XIP (Execute In Place) runs directly from flash - simpler but
 *    slower. Load-to-RAM copies to faster RAM before execution.
 *
 * Q: How do you protect the bootloader from being overwritten?
 * A: Memory protection unit (MPU), flash write protection, secure
 *    boot preventing unsigned updates.
 *
 * ============================================================================
 */
