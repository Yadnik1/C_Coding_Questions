/*
 * ============================================================================
 *        DESIGN: SPI FULL-DUPLEX TRANSFER LOGIC
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★☆ (HIGH)
 *   - Camera module interface
 *   - Flash memory (video storage)
 *   - Display (status indicators)
 *   - High-speed sensor data
 *
 * ============================================================================
 *                    SPI BASICS
 * ============================================================================
 *
 * SIGNALS:
 *   SCLK - Serial Clock (master generates)
 *   MOSI - Master Out Slave In
 *   MISO - Master In Slave Out
 *   CS   - Chip Select (active low, per slave)
 *
 * FULL-DUPLEX:
 *   Data shifts in BOTH directions simultaneously
 *   Master sends byte on MOSI, receives byte on MISO at same time
 *
 *   Master                     Slave
 *   ┌─────┐                   ┌─────┐
 *   │ TX  │──── MOSI ────────►│ RX  │
 *   │Shift│                   │Shift│
 *   │ Reg │◄──── MISO ────────│ Reg │
 *   └─────┘                   └─────┘
 *        │                         │
 *        └──── SCLK ──────────────►│
 *
 * MODES (CPOL, CPHA):
 *   Mode 0: CPOL=0, CPHA=0 - Sample on rising, shift on falling
 *   Mode 1: CPOL=0, CPHA=1 - Shift on rising, sample on falling
 *   Mode 2: CPOL=1, CPHA=0 - Sample on falling, shift on rising
 *   Mode 3: CPOL=1, CPHA=1 - Shift on falling, sample on rising
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// HARDWARE ABSTRACTION
// ============================================================================

typedef struct {
    uint8_t mode;       // 0-3
    uint32_t speed_hz;
    uint8_t cs_pin;
} SpiConfig;

// Simulated SPI
static uint8_t spi_slave_response = 0xFF;

void hal_spi_cs_low(uint8_t pin) {
    printf("[SPI] CS%d LOW\n", pin);
}

void hal_spi_cs_high(uint8_t pin) {
    printf("[SPI] CS%d HIGH\n", pin);
}

// Full-duplex: sends tx_byte, returns received byte
uint8_t hal_spi_transfer(uint8_t tx_byte) {
    printf("[SPI] TX:0x%02X RX:0x%02X\n", tx_byte, spi_slave_response);
    return spi_slave_response;
}

// DMA transfer (non-blocking)
void hal_spi_transfer_dma(uint8_t* tx_buf, uint8_t* rx_buf, uint16_t len,
                          void (*callback)(void)) {
    printf("[SPI] DMA transfer %d bytes\n", len);
    // In real code: configure DMA, start transfer
    // DMA complete ISR calls callback
}

// ============================================================================
// SPI DRIVER
// ============================================================================

typedef struct {
    SpiConfig config;
    volatile bool busy;
    void (*transfer_complete)(void);
} SpiDriver;

void spi_init(SpiDriver* spi, SpiConfig* config) {
    spi->config = *config;
    spi->busy = false;
    hal_spi_cs_high(config->cs_pin);  // Deselect
    printf("[SPI] Initialized: mode %d, %u Hz\n", config->mode, config->speed_hz);
}

// Blocking transfer
void spi_transfer(SpiDriver* spi, uint8_t* tx, uint8_t* rx, uint16_t len) {
    hal_spi_cs_low(spi->config.cs_pin);

    for (uint16_t i = 0; i < len; i++) {
        uint8_t tx_byte = tx ? tx[i] : 0xFF;  // Send 0xFF if no TX data
        uint8_t rx_byte = hal_spi_transfer(tx_byte);
        if (rx) rx[i] = rx_byte;
    }

    hal_spi_cs_high(spi->config.cs_pin);
}

// Write only (ignore received data)
void spi_write(SpiDriver* spi, uint8_t* data, uint16_t len) {
    spi_transfer(spi, data, NULL, len);
}

// Read only (send dummy bytes)
void spi_read(SpiDriver* spi, uint8_t* data, uint16_t len) {
    spi_transfer(spi, NULL, data, len);
}

// ============================================================================
// COMMON SPI DEVICE PATTERNS
// ============================================================================

// Pattern 1: Register write (command + address + data)
void spi_write_reg(SpiDriver* spi, uint8_t reg, uint8_t value) {
    uint8_t tx[2] = { reg & 0x7F, value };  // Clear bit 7 for write
    spi_write(spi, tx, 2);
}

// Pattern 2: Register read
uint8_t spi_read_reg(SpiDriver* spi, uint8_t reg) {
    uint8_t tx[2] = { reg | 0x80, 0xFF };   // Set bit 7 for read
    uint8_t rx[2];
    spi_transfer(spi, tx, rx, 2);
    return rx[1];  // First byte is dummy
}

// Pattern 3: Burst read (single command, multiple data)
void spi_burst_read(SpiDriver* spi, uint8_t start_reg, uint8_t* data, uint16_t len) {
    hal_spi_cs_low(spi->config.cs_pin);

    hal_spi_transfer(start_reg | 0x80);  // Read command
    for (uint16_t i = 0; i < len; i++) {
        data[i] = hal_spi_transfer(0xFF);
    }

    hal_spi_cs_high(spi->config.cs_pin);
}

// ============================================================================
// FLASH MEMORY EXAMPLE (Common in wearables)
// ============================================================================

#define FLASH_CMD_READ          0x03
#define FLASH_CMD_PAGE_PROGRAM  0x02
#define FLASH_CMD_WRITE_ENABLE  0x06
#define FLASH_CMD_READ_STATUS   0x05
#define FLASH_CMD_SECTOR_ERASE  0x20
#define FLASH_STATUS_BUSY       0x01

typedef struct {
    SpiDriver* spi;
    uint32_t size_bytes;
    uint16_t page_size;
    uint16_t sector_size;
} FlashDriver;

void flash_write_enable(FlashDriver* flash) {
    uint8_t cmd = FLASH_CMD_WRITE_ENABLE;
    spi_write(flash->spi, &cmd, 1);
}

bool flash_is_busy(FlashDriver* flash) {
    uint8_t tx[2] = { FLASH_CMD_READ_STATUS, 0xFF };
    uint8_t rx[2];
    spi_transfer(flash->spi, tx, rx, 2);
    return (rx[1] & FLASH_STATUS_BUSY) != 0;
}

void flash_wait_ready(FlashDriver* flash) {
    while (flash_is_busy(flash)) {
        // In real code: add timeout
    }
}

void flash_read(FlashDriver* flash, uint32_t addr, uint8_t* data, uint16_t len) {
    hal_spi_cs_low(flash->spi->config.cs_pin);

    // Send command + 24-bit address
    hal_spi_transfer(FLASH_CMD_READ);
    hal_spi_transfer((addr >> 16) & 0xFF);
    hal_spi_transfer((addr >> 8) & 0xFF);
    hal_spi_transfer(addr & 0xFF);

    // Read data
    for (uint16_t i = 0; i < len; i++) {
        data[i] = hal_spi_transfer(0xFF);
    }

    hal_spi_cs_high(flash->spi->config.cs_pin);
}

void flash_page_program(FlashDriver* flash, uint32_t addr, uint8_t* data, uint16_t len) {
    flash_write_enable(flash);

    hal_spi_cs_low(flash->spi->config.cs_pin);

    hal_spi_transfer(FLASH_CMD_PAGE_PROGRAM);
    hal_spi_transfer((addr >> 16) & 0xFF);
    hal_spi_transfer((addr >> 8) & 0xFF);
    hal_spi_transfer(addr & 0xFF);

    for (uint16_t i = 0; i < len; i++) {
        hal_spi_transfer(data[i]);
    }

    hal_spi_cs_high(flash->spi->config.cs_pin);

    flash_wait_ready(flash);
}

void flash_sector_erase(FlashDriver* flash, uint32_t addr) {
    flash_write_enable(flash);

    hal_spi_cs_low(flash->spi->config.cs_pin);

    hal_spi_transfer(FLASH_CMD_SECTOR_ERASE);
    hal_spi_transfer((addr >> 16) & 0xFF);
    hal_spi_transfer((addr >> 8) & 0xFF);
    hal_spi_transfer(addr & 0xFF);

    hal_spi_cs_high(flash->spi->config.cs_pin);

    flash_wait_ready(flash);
}

// ============================================================================
// DMA-BASED TRANSFER (For high throughput)
// ============================================================================

/*
 * For video data, use DMA:
 *
 *   void camera_frame_read_dma(uint8_t* buffer, uint32_t size) {
 *       hal_spi_cs_low(CAMERA_CS);
 *       hal_spi_transfer(CMD_READ_FRAME);
 *
 *       // DMA handles the rest
 *       hal_spi_transfer_dma(NULL, buffer, size, camera_dma_complete);
 *   }
 *
 *   void camera_dma_complete(void) {
 *       hal_spi_cs_high(CAMERA_CS);
 *       frame_ready_flag = true;
 *   }
 */

// ============================================================================
// DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   SPI FULL-DUPLEX DEMO\n");
    printf("============================================\n");

    // Initialize SPI
    SpiConfig config = {
        .mode = 0,
        .speed_hz = 10000000,  // 10 MHz
        .cs_pin = 0
    };
    SpiDriver spi;
    spi_init(&spi, &config);

    // Demo register access
    printf("\n--- Register Access ---\n");
    spi_write_reg(&spi, 0x01, 0xAB);

    spi_slave_response = 0x42;  // Set simulated response
    uint8_t value = spi_read_reg(&spi, 0x01);
    printf("Read value: 0x%02X\n", value);

    // Demo burst read
    printf("\n--- Burst Read ---\n");
    uint8_t burst_data[4];
    spi_burst_read(&spi, 0x10, burst_data, 4);

    // Demo flash operations
    printf("\n--- Flash Operations ---\n");
    FlashDriver flash = {
        .spi = &spi,
        .size_bytes = 16 * 1024 * 1024,  // 16MB
        .page_size = 256,
        .sector_size = 4096
    };

    uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04};
    printf("Writing to flash...\n");
    flash_page_program(&flash, 0x1000, test_data, 4);

    printf("Reading from flash...\n");
    uint8_t read_data[4];
    flash_read(&flash, 0x1000, read_data, 4);

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "Explain SPI full-duplex operation."
 * A: "Master and slave exchange data simultaneously:
 *    - Master shifts out on MOSI, shifts in on MISO
 *    - Each clock cycle moves 1 bit in each direction
 *    - After 8 clocks, both have new byte
 *
 *    To 'read', master sends dummy (0xFF) and captures MISO.
 *    To 'write', master sends data and ignores MISO."
 *
 * Q: "SPI vs I2C - when to use each?"
 * A: "SPI: High speed (10+ MHz), point-to-point, more pins
 *       Use for: Flash, display, high-speed sensors
 *    I2C: Lower speed (400 kHz), multi-device bus, fewer pins
 *       Use for: Multiple sensors, EEPROM, configuration
 *
 *    SPI needs separate CS for each device.
 *    I2C uses addresses - simpler wiring."
 *
 * Q: "How do you handle SPI with multiple slaves?"
 * A: "Each slave has dedicated CS line.
 *    Only one CS active (low) at a time.
 *    Can use GPIO expander if many slaves.
 *    Some chips support daisy-chain (DOUT→DIN)."
 *
 * ============================================================================
 */
