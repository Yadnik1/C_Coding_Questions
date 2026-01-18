/*
 * ============================================================================
 *                    SPI DRIVER - COMPLETE GUIDE
 * ============================================================================
 *
 * Everything about SPI for embedded interviews:
 *   - Protocol basics (full-duplex nature)
 *   - Clock polarity/phase (CPOL/CPHA modes)
 *   - Driver architecture
 *   - Transaction flow
 *   - DMA integration
 *
 * ============================================================================
 *                    SPI BASICS
 * ============================================================================
 *
 * WHAT IS SPI?
 *   - Serial Peripheral Interface
 *   - 4-wire synchronous serial bus (can be 3-wire)
 *   - Full-duplex: send AND receive simultaneously
 *   - Much faster than I2C: 10-50+ MHz typical
 *   - Single master, multiple slaves (separate CS lines)
 *
 * THE 4 WIRES:
 *
 *   SCLK (SCK) - Clock: Master generates, slaves receive
 *   MOSI       - Master Out Slave In: Data from master to slave
 *   MISO       - Master In Slave Out: Data from slave to master
 *   CS/SS      - Chip Select/Slave Select: Active LOW, selects slave
 *
 *
 * BUS TOPOLOGY:
 *
 *                    +--------+
 *                    | MASTER |
 *                    +--------+
 *                    |SCLK    |------------+------------+
 *                    |MOSI    |------+-----+------+     |
 *                    |MISO    |--+---|-----|---+  |     |
 *                    |CS0     |--|---|--+  |   |  |     |
 *                    |CS1     |--|---|--|--|---+  |     |
 *                    |CS2     |--|---|--|--|---|--|--+  |
 *                    +--------+  |   |  |  |   |  |  |  |
 *                                |   |  |  |   |  |  |  |
 *                    +-----------+   |  |  |   |  |  |  |
 *                    |  SLAVE 0  |   |  |  |   |  |  |  |
 *                    |  MISO-----+   |  |  |   |  |  |  |
 *                    |  MOSI---------+  |  |   |  |  |  |
 *                    |  SCLK-----------+|  |   |  |  |  |
 *                    |  CS--------------+  |   |  |  |  |
 *                    +---------------------+   |  |  |  |
 *                                              |  |  |  |
 *                    +-------------------------+  |  |  |
 *                    |  SLAVE 1  |                |  |  |
 *                    |  MISO-----+----------------+  |  |
 *                    |  MOSI------------------------+  |
 *                    |  SCLK---------------------------+
 *                    |  CS (directly to CS1)           |
 *                    +---------------------------------+
 *
 * ============================================================================
 *                    SPI MODES (CPOL/CPHA) - MUST KNOW!
 * ============================================================================
 *
 * CPOL (Clock Polarity): Idle state of clock
 *   CPOL=0: Clock idle LOW
 *   CPOL=1: Clock idle HIGH
 *
 * CPHA (Clock Phase): Which edge samples data
 *   CPHA=0: Sample on FIRST edge (leading edge)
 *   CPHA=1: Sample on SECOND edge (trailing edge)
 *
 *
 * MODE 0 (CPOL=0, CPHA=0) - MOST COMMON
 *   - Clock idle LOW
 *   - Sample on rising edge, shift on falling edge
 *
 *        CS   ‾‾‾\_____________________________________/‾‾‾
 *        SCLK ____/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\______
 *        MOSI ----<D7><D6><D5><D4><D3><D2><D1><D0>------
 *        MISO ----<D7><D6><D5><D4><D3><D2><D1><D0>------
 *                  ^ sample points (rising edges)
 *
 *
 * MODE 1 (CPOL=0, CPHA=1)
 *   - Clock idle LOW
 *   - Sample on falling edge, shift on rising edge
 *
 *        SCLK ____/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\______
 *        MOSI -----<D7><D6><D5><D4><D3><D2><D1><D0>-----
 *                    ^ sample points (falling edges)
 *
 *
 * MODE 2 (CPOL=1, CPHA=0)
 *   - Clock idle HIGH
 *   - Sample on falling edge (first edge)
 *
 *        SCLK ‾‾‾‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\‾‾‾‾‾‾
 *        MOSI ----<D7><D6><D5><D4><D3><D2><D1><D0>------
 *                  ^ sample points (falling edges)
 *
 *
 * MODE 3 (CPOL=1, CPHA=1)
 *   - Clock idle HIGH
 *   - Sample on rising edge (second edge)
 *
 *        SCLK ‾‾‾‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\‾‾‾‾‾‾
 *        MOSI -----<D7><D6><D5><D4><D3><D2><D1><D0>-----
 *                    ^ sample points (rising edges)
 *
 *
 * COMMON DEVICE MODES:
 *   - SD Cards: Mode 0
 *   - Most sensors: Mode 0 or Mode 3
 *   - Flash memory: Usually Mode 0
 *   - ADCs/DACs: Varies, check datasheet!
 *
 * ============================================================================
 *                    SPI TRANSACTION FLOW
 * ============================================================================
 *
 * BASIC PRINCIPLE:
 *   - SPI is ALWAYS full-duplex
 *   - Every clock cycle, 1 bit goes out MOSI AND 1 bit comes in MISO
 *   - Even for "write only", you receive (and ignore) data
 *   - Even for "read only", you send (dummy) data
 *
 *
 * WRITE TRANSACTION (e.g., send command to sensor):
 *
 *   CS   ‾‾\_______________________________/‾‾
 *   SCLK ___/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\___
 *   MOSI ---<CMD><  ><  ><  ><  ><  ><  ><  >---  (your data)
 *   MISO ---<XX ><XX><XX><XX><XX><XX><XX><XX>---  (ignored)
 *
 *
 * READ TRANSACTION (e.g., read sensor data):
 *
 *   CS   ‾‾\___________________________________/‾‾
 *   SCLK ___/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\___
 *   MOSI ---<CMD><FF><FF><FF><FF><FF><FF><FF><FF>---  (cmd + dummy 0xFF)
 *   MISO ---<XX ><D7><D6><D5><D4><D3><D2><D1><D0>---  (your data)
 *             ^    ^--- data starts after command
 *             |
 *        command byte
 *
 *
 * TYPICAL SENSOR READ SEQUENCE:
 *   1. Assert CS (LOW)
 *   2. Send read command (usually register address | 0x80)
 *   3. Send dummy bytes while reading response
 *   4. Deassert CS (HIGH)
 *
 *
 * WHY 0xFF FOR DUMMY BYTES?
 *   - Keeps MOSI line HIGH
 *   - Some devices care about MOSI during read
 *   - Convention: 0xFF or 0x00 as dummy
 *
 * ============================================================================
 *                    SPI vs I2C COMPARISON
 * ============================================================================
 *
 * | Feature        | SPI                    | I2C                    |
 * |----------------|------------------------|------------------------|
 * | Wires          | 4 (+ 1 CS per slave)   | 2 (shared)             |
 * | Speed          | 10-50+ MHz             | 100-400 kHz typical    |
 * | Duplex         | Full duplex            | Half duplex            |
 * | Addressing     | CS line per device     | 7-bit address          |
 * | Multi-master   | Complex                | Supported              |
 * | Acknowledgment | None (no ACK)          | ACK after each byte    |
 * | Distance       | Short (PCB only)       | Short (with buffers)   |
 * | Complexity     | Simpler hardware       | More complex protocol  |
 * | Pin count      | High (many CS lines)   | Low (2 wires shared)   |
 *
 * WHEN TO USE SPI:
 *   - High-speed data (displays, flash, ADCs)
 *   - Full-duplex needed
 *   - Few slave devices
 *
 * WHEN TO USE I2C:
 *   - Many devices, limited pins
 *   - Lower speed acceptable
 *   - Need acknowledgment
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
//                    SPI CONFIGURATION
// ============================================================================

typedef enum {
    SPI_MODE_0 = 0,  // CPOL=0, CPHA=0 - Most common
    SPI_MODE_1,      // CPOL=0, CPHA=1
    SPI_MODE_2,      // CPOL=1, CPHA=0
    SPI_MODE_3       // CPOL=1, CPHA=1
} SPI_Mode;

typedef enum {
    SPI_OK = 0,
    SPI_ERROR_TIMEOUT,
    SPI_ERROR_BUSY
} SPI_Status;

typedef struct {
    SPI_Mode mode;
    uint32_t speed_hz;
    uint8_t bits_per_word;    // Usually 8
    bool lsb_first;           // Usually false (MSB first)
} SPI_Config;

// Simulated SPI hardware registers (like STM32)
typedef struct {
    volatile uint32_t CR1;    // Control register 1
    volatile uint32_t CR2;    // Control register 2
    volatile uint32_t SR;     // Status register
    volatile uint32_t DR;     // Data register
} SPI_TypeDef;

#define SPI_SR_TXE    (1 << 1)   // TX buffer empty
#define SPI_SR_RXNE   (1 << 0)   // RX buffer not empty
#define SPI_SR_BSY    (1 << 7)   // Busy flag

static SPI_TypeDef spi1_hw = {0};
static SPI_TypeDef* SPI1 = &spi1_hw;

// GPIO for chip select
typedef struct {
    volatile uint32_t ODR;    // Output data register
} GPIO_TypeDef;

static GPIO_TypeDef gpioa_hw = {.ODR = 0xFFFF};  // All high initially
static GPIO_TypeDef* GPIOA = &gpioa_hw;

#define CS_PIN  4

// ============================================================================
//                    SPI LOW-LEVEL FUNCTIONS
// ============================================================================

void spi_cs_low(GPIO_TypeDef* gpio, uint8_t pin) {
    gpio->ODR &= ~(1 << pin);
    printf("  [CS] LOW (select device)\n");
}

void spi_cs_high(GPIO_TypeDef* gpio, uint8_t pin) {
    gpio->ODR |= (1 << pin);
    printf("  [CS] HIGH (deselect device)\n");
}

void spi_init(SPI_TypeDef* spi, SPI_Config* config) {
    printf("[SPI] Initializing: Mode %d, Speed %lu Hz\n",
           config->mode, (unsigned long)config->speed_hz);

    // Configure CPOL/CPHA based on mode
    spi->CR1 = 0;
    switch (config->mode) {
        case SPI_MODE_0: /* CPOL=0, CPHA=0 */ break;
        case SPI_MODE_1: spi->CR1 |= (1 << 0); break;  // CPHA=1
        case SPI_MODE_2: spi->CR1 |= (1 << 1); break;  // CPOL=1
        case SPI_MODE_3: spi->CR1 |= (3 << 0); break;  // Both
    }

    // Enable SPI
    spi->CR1 |= (1 << 6);  // SPE bit
}

// Core SPI transfer - sends AND receives one byte simultaneously
uint8_t spi_transfer_byte(SPI_TypeDef* spi, uint8_t tx_byte) {
    // Wait for TX buffer empty
    // Real code: while(!(spi->SR & SPI_SR_TXE));
    spi->SR |= SPI_SR_TXE;

    // Write data to transmit
    spi->DR = tx_byte;
    printf("  [SPI] TX: 0x%02X", tx_byte);

    // Wait for RX buffer not empty
    // Real code: while(!(spi->SR & SPI_SR_RXNE));
    spi->SR |= SPI_SR_RXNE;

    // Read received data
    uint8_t rx_byte = (uint8_t)spi->DR;

    // Simulate receiving echo (in real hardware, slave would respond)
    rx_byte = tx_byte;  // Simulation only

    printf(" -> RX: 0x%02X\n", rx_byte);
    return rx_byte;
}

// Transfer multiple bytes
void spi_transfer(SPI_TypeDef* spi, uint8_t* tx_buf, uint8_t* rx_buf, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        uint8_t tx = tx_buf ? tx_buf[i] : 0xFF;  // Send 0xFF if no TX data
        uint8_t rx = spi_transfer_byte(spi, tx);
        if (rx_buf) rx_buf[i] = rx;
    }
}

// ============================================================================
//                    SPI DRIVER FUNCTIONS
// ============================================================================

// Write-only transaction
SPI_Status spi_write(SPI_TypeDef* spi, GPIO_TypeDef* cs_gpio, uint8_t cs_pin,
                     uint8_t* data, uint16_t len) {
    printf("\n[SPI WRITE] %d bytes\n", len);

    spi_cs_low(cs_gpio, cs_pin);

    for (uint16_t i = 0; i < len; i++) {
        spi_transfer_byte(spi, data[i]);
    }

    // Wait for not busy
    // Real code: while(spi->SR & SPI_SR_BSY);

    spi_cs_high(cs_gpio, cs_pin);

    return SPI_OK;
}

// Read-only transaction (sends dummy bytes)
SPI_Status spi_read(SPI_TypeDef* spi, GPIO_TypeDef* cs_gpio, uint8_t cs_pin,
                    uint8_t* data, uint16_t len) {
    printf("\n[SPI READ] %d bytes\n", len);

    spi_cs_low(cs_gpio, cs_pin);

    for (uint16_t i = 0; i < len; i++) {
        data[i] = spi_transfer_byte(spi, 0xFF);  // Send dummy, capture response
    }

    spi_cs_high(cs_gpio, cs_pin);

    return SPI_OK;
}

// Write then Read (common pattern: send command, read response)
SPI_Status spi_write_then_read(SPI_TypeDef* spi, GPIO_TypeDef* cs_gpio, uint8_t cs_pin,
                                uint8_t* tx_data, uint16_t tx_len,
                                uint8_t* rx_data, uint16_t rx_len) {
    printf("\n[SPI WRITE-THEN-READ] TX:%d bytes, RX:%d bytes\n", tx_len, rx_len);

    spi_cs_low(cs_gpio, cs_pin);

    // Send command/address
    for (uint16_t i = 0; i < tx_len; i++) {
        spi_transfer_byte(spi, tx_data[i]);
    }

    // Read response (sending dummy bytes)
    for (uint16_t i = 0; i < rx_len; i++) {
        rx_data[i] = spi_transfer_byte(spi, 0xFF);
    }

    spi_cs_high(cs_gpio, cs_pin);

    return SPI_OK;
}

// ============================================================================
//                    EXAMPLE: SPI FLASH DRIVER
// ============================================================================

#define FLASH_CMD_READ_ID       0x9F
#define FLASH_CMD_READ_DATA     0x03
#define FLASH_CMD_WRITE_ENABLE  0x06
#define FLASH_CMD_PAGE_PROGRAM  0x02
#define FLASH_CMD_SECTOR_ERASE  0x20
#define FLASH_CMD_READ_STATUS   0x05

#define FLASH_STATUS_BUSY       0x01
#define FLASH_STATUS_WEL        0x02

typedef struct {
    SPI_TypeDef* spi;
    GPIO_TypeDef* cs_gpio;
    uint8_t cs_pin;
} SPI_Flash;

void flash_init(SPI_Flash* flash, SPI_TypeDef* spi, GPIO_TypeDef* cs_gpio, uint8_t cs_pin) {
    flash->spi = spi;
    flash->cs_gpio = cs_gpio;
    flash->cs_pin = cs_pin;
}

// Read JEDEC ID (3 bytes: manufacturer, memory type, capacity)
void flash_read_id(SPI_Flash* flash, uint8_t* id) {
    printf("\n--- Flash Read ID ---\n");
    uint8_t cmd = FLASH_CMD_READ_ID;
    spi_write_then_read(flash->spi, flash->cs_gpio, flash->cs_pin,
                        &cmd, 1, id, 3);
    printf("Flash ID: %02X %02X %02X\n", id[0], id[1], id[2]);
}

// Read status register
uint8_t flash_read_status(SPI_Flash* flash) {
    uint8_t cmd = FLASH_CMD_READ_STATUS;
    uint8_t status;
    spi_write_then_read(flash->spi, flash->cs_gpio, flash->cs_pin,
                        &cmd, 1, &status, 1);
    return status;
}

// Wait for flash not busy
void flash_wait_ready(SPI_Flash* flash) {
    printf("  [FLASH] Waiting for ready...\n");
    // In real code: while(flash_read_status(flash) & FLASH_STATUS_BUSY);
}

// Write enable (required before any write/erase)
void flash_write_enable(SPI_Flash* flash) {
    uint8_t cmd = FLASH_CMD_WRITE_ENABLE;
    spi_write(flash->spi, flash->cs_gpio, flash->cs_pin, &cmd, 1);
}

// Read data from flash
void flash_read(SPI_Flash* flash, uint32_t address, uint8_t* data, uint16_t len) {
    printf("\n--- Flash Read @ 0x%06X, %d bytes ---\n", address, len);

    uint8_t cmd[4] = {
        FLASH_CMD_READ_DATA,
        (address >> 16) & 0xFF,
        (address >> 8) & 0xFF,
        address & 0xFF
    };

    spi_write_then_read(flash->spi, flash->cs_gpio, flash->cs_pin,
                        cmd, 4, data, len);
}

// Page program (write up to 256 bytes)
void flash_page_program(SPI_Flash* flash, uint32_t address, uint8_t* data, uint16_t len) {
    printf("\n--- Flash Page Program @ 0x%06X, %d bytes ---\n", address, len);

    if (len > 256) len = 256;  // Max page size

    flash_write_enable(flash);

    spi_cs_low(flash->cs_gpio, flash->cs_pin);

    // Send command + address
    spi_transfer_byte(flash->spi, FLASH_CMD_PAGE_PROGRAM);
    spi_transfer_byte(flash->spi, (address >> 16) & 0xFF);
    spi_transfer_byte(flash->spi, (address >> 8) & 0xFF);
    spi_transfer_byte(flash->spi, address & 0xFF);

    // Send data
    for (uint16_t i = 0; i < len; i++) {
        spi_transfer_byte(flash->spi, data[i]);
    }

    spi_cs_high(flash->cs_gpio, flash->cs_pin);

    flash_wait_ready(flash);
}

// Sector erase (4KB)
void flash_sector_erase(SPI_Flash* flash, uint32_t address) {
    printf("\n--- Flash Sector Erase @ 0x%06X ---\n", address);

    flash_write_enable(flash);

    uint8_t cmd[4] = {
        FLASH_CMD_SECTOR_ERASE,
        (address >> 16) & 0xFF,
        (address >> 8) & 0xFF,
        address & 0xFF
    };

    spi_write(flash->spi, flash->cs_gpio, flash->cs_pin, cmd, 4);

    flash_wait_ready(flash);
}

// ============================================================================
//                    DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   SPI DRIVER COMPLETE DEMO\n");
    printf("============================================\n");

    // Initialize SPI
    SPI_Config config = {
        .mode = SPI_MODE_0,
        .speed_hz = 10000000,  // 10 MHz
        .bits_per_word = 8,
        .lsb_first = false
    };
    spi_init(SPI1, &config);

    // Initialize flash driver
    SPI_Flash flash;
    flash_init(&flash, SPI1, GPIOA, CS_PIN);

    // Demo operations
    uint8_t id[3];
    flash_read_id(&flash, id);

    uint8_t read_buf[8];
    flash_read(&flash, 0x001000, read_buf, 8);

    uint8_t write_data[] = {0x11, 0x22, 0x33, 0x44};
    flash_page_program(&flash, 0x001000, write_data, 4);

    printf("\n============================================\n");
    printf("   SPI KEY POINTS FOR INTERVIEW\n");
    printf("============================================\n");
    printf("- SPI is FULL DUPLEX: send AND receive every clock\n");
    printf("- 4 wires: SCLK, MOSI, MISO, CS (active low)\n");
    printf("- MODE 0 (CPOL=0, CPHA=0) is most common\n");
    printf("- CPOL = clock idle state (0=low, 1=high)\n");
    printf("- CPHA = sample edge (0=first, 1=second)\n");
    printf("- No ACK - must trust CS timing and device behavior\n");
    printf("- Read = send command + dummy bytes while receiving\n");
    printf("- Much faster than I2C but uses more pins\n");

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW Q&A
 * ============================================================================
 *
 * Q: Explain SPI modes (CPOL/CPHA).
 * A: "CPOL is clock polarity - whether clock idles high or low. CPHA is
 *    clock phase - whether you sample on the first or second edge. Mode 0
 *    has clock idle low and samples on rising edge - it's the most common.
 *    You must match the device's expected mode or communication fails."
 *
 * Q: Why is SPI full-duplex and what does that mean for reads?
 * A: "In SPI, every clock cycle shifts one bit out on MOSI and one bit in
 *    on MISO simultaneously. So even for a read, you must send something.
 *    We send dummy bytes, usually 0xFF, and capture what comes back on MISO."
 *
 * Q: How do you read a register from an SPI sensor?
 * A: "First send the register address with read bit set - often the MSB
 *    indicates read. Then send dummy bytes while the slave clocks out
 *    the register value. Keep CS low for the entire transaction."
 *
 * Q: What's the difference between SPI and I2C?
 * A: "SPI is faster (10-50+ MHz vs 400kHz), full-duplex, but needs more
 *    wires - dedicated CS for each slave. I2C uses addresses on 2 shared
 *    wires. SPI has no ACK so you can't detect missing devices easily.
 *    Use SPI for high-speed like displays and flash, I2C for many slow
 *    sensors on limited pins."
 *
 * Q: Why is CS (chip select) important?
 * A: "CS frames the transaction - slave ignores SCLK/MOSI when CS is high.
 *    You must assert CS before clocking data. Many devices reset their
 *    state machine on CS edges, so proper CS handling is critical."
 *
 * Q: How would you debug SPI not working?
 * A: "Use oscilloscope/logic analyzer to check: Is CS going low? Is SCLK
 *    toggling at right frequency and mode? Is MOSI sending correct data?
 *    Common issues: wrong mode, CS not toggling, speed too fast for wires,
 *    MISO not connected (reads all 0xFF or 0x00)."
 *
 * Q: What's a typical SPI flash write sequence?
 * A: "1. Send Write Enable command (0x06)
 *    2. Send Page Program command with 24-bit address
 *    3. Send up to 256 bytes of data
 *    4. Deassert CS to start internal write
 *    5. Poll status register until busy bit clears
 *    Write Enable is needed before EVERY write or erase!"
 *
 * ============================================================================
 */
