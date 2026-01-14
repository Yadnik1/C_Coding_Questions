/*
 * ============================================================================
 *        DESIGN: I2C COMMUNICATION WORKFLOW
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★☆ (HIGH)
 *   - IMU sensors (accelerometer, gyroscope)
 *   - Environmental sensors (temp, humidity, pressure)
 *   - EEPROM for calibration data
 *   - Battery fuel gauge
 *
 * ============================================================================
 *                    I2C PROTOCOL BASICS
 * ============================================================================
 *
 * PHYSICAL:
 *   - Two wires: SDA (data), SCL (clock)
 *   - Open-drain with pull-up resistors (typically 4.7kΩ)
 *   - Multi-master, multi-slave on same bus
 *
 * SPEED MODES:
 *   Standard:   100 kHz
 *   Fast:       400 kHz
 *   Fast Plus:  1 MHz
 *   High Speed: 3.4 MHz
 *
 * FRAME FORMAT:
 *
 *   START  ADDR(7) R/W ACK  DATA(8)  ACK  DATA(8)  ACK  STOP
 *   ┌──┐  ┌───────┬───┬──┐┌───────┬──┐ ┌───────┬──┐ ┌──┐
 *   │S │  │A6...A0│R/W│A ││D7...D0│A │ │D7...D0│A │ │P │
 *   └──┘  └───────┴───┴──┘└───────┴──┘ └───────┴──┘ └──┘
 *
 *   S = Start condition (SDA high→low while SCL high)
 *   P = Stop condition (SDA low→high while SCL high)
 *   A = ACK (receiver pulls SDA low)
 *   R/W: 0=write, 1=read
 *
 * ============================================================================
 *                    COMMON I2C OPERATIONS
 * ============================================================================
 *
 * 1. WRITE REGISTER:
 *    [S][ADDR+W][ACK][REG][ACK][DATA][ACK][P]
 *
 * 2. READ REGISTER:
 *    [S][ADDR+W][ACK][REG][ACK][Sr][ADDR+R][ACK][DATA][NACK][P]
 *    (Sr = repeated start)
 *
 * 3. BURST READ (multiple bytes):
 *    [S][ADDR+W][ACK][REG][ACK][Sr][ADDR+R][ACK][DATA][ACK][DATA][ACK]...[DATA][NACK][P]
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// I2C ERROR CODES
// ============================================================================

typedef enum {
    I2C_OK = 0,
    I2C_ERR_NACK,           // No acknowledge from slave
    I2C_ERR_BUS_BUSY,       // Bus held by another master
    I2C_ERR_ARB_LOST,       // Arbitration lost
    I2C_ERR_TIMEOUT,        // Operation timed out
    I2C_ERR_INVALID_PARAM,  // Bad parameter
} I2cError;

const char* i2c_error_str(I2cError err) {
    switch (err) {
        case I2C_OK:             return "OK";
        case I2C_ERR_NACK:       return "NACK";
        case I2C_ERR_BUS_BUSY:   return "BUS_BUSY";
        case I2C_ERR_ARB_LOST:   return "ARB_LOST";
        case I2C_ERR_TIMEOUT:    return "TIMEOUT";
        case I2C_ERR_INVALID_PARAM: return "INVALID_PARAM";
        default:                 return "UNKNOWN";
    }
}

// ============================================================================
// HARDWARE ABSTRACTION LAYER
// ============================================================================

// Simulated I2C hardware
typedef struct {
    bool bus_busy;
    uint8_t slave_memory[256];  // Simulated slave registers
    uint8_t reg_pointer;
} MockI2cHW;

MockI2cHW mock_i2c = {0};

I2cError hal_i2c_start(void) {
    if (mock_i2c.bus_busy) return I2C_ERR_BUS_BUSY;
    printf("[I2C] START\n");
    return I2C_OK;
}

I2cError hal_i2c_stop(void) {
    printf("[I2C] STOP\n");
    return I2C_OK;
}

I2cError hal_i2c_write_byte(uint8_t byte, bool* ack) {
    printf("[I2C] TX: 0x%02X ", byte);
    *ack = true;  // Simulated ACK
    printf("ACK\n");
    return I2C_OK;
}

I2cError hal_i2c_read_byte(uint8_t* byte, bool ack) {
    *byte = mock_i2c.slave_memory[mock_i2c.reg_pointer++];
    printf("[I2C] RX: 0x%02X %s\n", *byte, ack ? "ACK" : "NACK");
    return I2C_OK;
}

// ============================================================================
// I2C DRIVER LAYER
// ============================================================================

#define I2C_TIMEOUT_MS 100

typedef struct {
    uint8_t address;    // 7-bit address
    uint32_t speed_hz;
    uint8_t retries;
} I2cDevice;

I2cError i2c_write_reg(I2cDevice* dev, uint8_t reg, uint8_t data) {
    I2cError err;
    bool ack;

    // Start
    err = hal_i2c_start();
    if (err != I2C_OK) return err;

    // Address + Write
    err = hal_i2c_write_byte((dev->address << 1) | 0, &ack);
    if (!ack) { hal_i2c_stop(); return I2C_ERR_NACK; }

    // Register address
    err = hal_i2c_write_byte(reg, &ack);
    if (!ack) { hal_i2c_stop(); return I2C_ERR_NACK; }

    // Data
    err = hal_i2c_write_byte(data, &ack);
    if (!ack) { hal_i2c_stop(); return I2C_ERR_NACK; }

    // Stop
    hal_i2c_stop();

    // Update simulation
    mock_i2c.slave_memory[reg] = data;

    return I2C_OK;
}

I2cError i2c_read_reg(I2cDevice* dev, uint8_t reg, uint8_t* data) {
    I2cError err;
    bool ack;

    // Start
    err = hal_i2c_start();
    if (err != I2C_OK) return err;

    // Address + Write (to set register pointer)
    err = hal_i2c_write_byte((dev->address << 1) | 0, &ack);
    if (!ack) { hal_i2c_stop(); return I2C_ERR_NACK; }

    // Register address
    err = hal_i2c_write_byte(reg, &ack);
    if (!ack) { hal_i2c_stop(); return I2C_ERR_NACK; }

    // Repeated Start
    err = hal_i2c_start();

    // Address + Read
    err = hal_i2c_write_byte((dev->address << 1) | 1, &ack);
    if (!ack) { hal_i2c_stop(); return I2C_ERR_NACK; }

    // Read data (NACK to signal end)
    mock_i2c.reg_pointer = reg;  // Simulation
    err = hal_i2c_read_byte(data, false);

    // Stop
    hal_i2c_stop();

    return I2C_OK;
}

I2cError i2c_read_burst(I2cDevice* dev, uint8_t reg, uint8_t* data, uint8_t len) {
    I2cError err;
    bool ack;

    err = hal_i2c_start();
    if (err != I2C_OK) return err;

    // Address + Write
    err = hal_i2c_write_byte((dev->address << 1) | 0, &ack);
    if (!ack) { hal_i2c_stop(); return I2C_ERR_NACK; }

    // Register address
    err = hal_i2c_write_byte(reg, &ack);
    if (!ack) { hal_i2c_stop(); return I2C_ERR_NACK; }

    // Repeated Start + Read
    hal_i2c_start();
    err = hal_i2c_write_byte((dev->address << 1) | 1, &ack);
    if (!ack) { hal_i2c_stop(); return I2C_ERR_NACK; }

    // Read multiple bytes
    mock_i2c.reg_pointer = reg;
    for (uint8_t i = 0; i < len; i++) {
        bool send_ack = (i < len - 1);  // ACK all but last
        hal_i2c_read_byte(&data[i], send_ack);
    }

    hal_i2c_stop();
    return I2C_OK;
}

// ============================================================================
// ERROR HANDLING WITH RETRY
// ============================================================================

I2cError i2c_write_reg_retry(I2cDevice* dev, uint8_t reg, uint8_t data) {
    I2cError err;

    for (uint8_t attempt = 0; attempt <= dev->retries; attempt++) {
        err = i2c_write_reg(dev, reg, data);

        if (err == I2C_OK) {
            return I2C_OK;
        }

        printf("[I2C] Write failed: %s, attempt %d/%d\n",
               i2c_error_str(err), attempt + 1, dev->retries + 1);

        // Brief delay before retry
        // delay_ms(1);
    }

    return err;
}

// ============================================================================
// DEVICE ABSTRACTION LAYER (Example: IMU)
// ============================================================================

#define IMU_ADDR        0x68
#define IMU_REG_WHO_AM_I    0x75
#define IMU_REG_PWR_MGMT    0x6B
#define IMU_REG_ACCEL_XOUT  0x3B
#define IMU_WHO_AM_I_VALUE  0x68

typedef struct {
    I2cDevice i2c_dev;
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
    float accel_scale;
    float gyro_scale;
} ImuDriver;

I2cError imu_init(ImuDriver* imu) {
    uint8_t who_am_i;
    I2cError err;

    printf("\n[IMU] Initializing...\n");

    // Check WHO_AM_I register
    err = i2c_read_reg(&imu->i2c_dev, IMU_REG_WHO_AM_I, &who_am_i);
    if (err != I2C_OK) {
        printf("[IMU] Failed to read WHO_AM_I: %s\n", i2c_error_str(err));
        return err;
    }

    // Simulate correct value
    who_am_i = IMU_WHO_AM_I_VALUE;

    if (who_am_i != IMU_WHO_AM_I_VALUE) {
        printf("[IMU] Wrong WHO_AM_I: 0x%02X (expected 0x%02X)\n",
               who_am_i, IMU_WHO_AM_I_VALUE);
        return I2C_ERR_NACK;
    }
    printf("[IMU] WHO_AM_I: 0x%02X OK\n", who_am_i);

    // Wake up IMU (clear sleep bit)
    err = i2c_write_reg(&imu->i2c_dev, IMU_REG_PWR_MGMT, 0x00);
    if (err != I2C_OK) return err;
    printf("[IMU] Woken from sleep\n");

    // Set scale factors (example values)
    imu->accel_scale = 1.0f / 16384.0f;  // ±2g mode: 16384 LSB/g
    imu->gyro_scale = 1.0f / 131.0f;      // ±250°/s: 131 LSB/(°/s)

    printf("[IMU] Initialized successfully\n");
    return I2C_OK;
}

I2cError imu_read_accel(ImuDriver* imu) {
    uint8_t data[6];
    I2cError err;

    // Burst read 6 bytes: XOUT_H, XOUT_L, YOUT_H, YOUT_L, ZOUT_H, ZOUT_L
    err = i2c_read_burst(&imu->i2c_dev, IMU_REG_ACCEL_XOUT, data, 6);
    if (err != I2C_OK) return err;

    // Combine high and low bytes (big-endian)
    imu->accel_x = (int16_t)((data[0] << 8) | data[1]);
    imu->accel_y = (int16_t)((data[2] << 8) | data[3]);
    imu->accel_z = (int16_t)((data[4] << 8) | data[5]);

    return I2C_OK;
}

// ============================================================================
// I2C BUS SCANNING
// ============================================================================

void i2c_scan_bus(void) {
    printf("\n[I2C] Scanning bus...\n");
    printf("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (uint8_t row = 0; row < 8; row++) {
        printf("%02X: ", row << 4);
        for (uint8_t col = 0; col < 16; col++) {
            uint8_t addr = (row << 4) | col;

            // Skip reserved addresses
            if (addr < 0x08 || addr > 0x77) {
                printf("   ");
                continue;
            }

            // Try to communicate
            I2cDevice test_dev = { .address = addr };
            uint8_t dummy;
            I2cError err = i2c_read_reg(&test_dev, 0x00, &dummy);

            if (err == I2C_OK) {
                printf("%02X ", addr);
            } else {
                printf("-- ");
            }
        }
        printf("\n");
    }
}

// ============================================================================
// TRANSACTION QUEUE (For non-blocking operation)
// ============================================================================

typedef enum {
    I2C_OP_WRITE,
    I2C_OP_READ,
    I2C_OP_BURST_READ
} I2cOperation;

typedef struct {
    I2cDevice* device;
    I2cOperation operation;
    uint8_t reg;
    uint8_t* data;
    uint8_t len;
    void (*callback)(I2cError err, uint8_t* data, uint8_t len);
} I2cTransaction;

#define I2C_QUEUE_SIZE 8

typedef struct {
    I2cTransaction queue[I2C_QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
    bool busy;
} I2cTransactionQueue;

I2cTransactionQueue i2c_queue = {0};

bool i2c_queue_transaction(I2cTransaction* txn) {
    uint8_t next = (i2c_queue.head + 1) % I2C_QUEUE_SIZE;
    if (next == i2c_queue.tail) return false;  // Full

    i2c_queue.queue[i2c_queue.head] = *txn;
    i2c_queue.head = next;

    return true;
}

void i2c_process_queue(void) {
    if (i2c_queue.busy) return;
    if (i2c_queue.head == i2c_queue.tail) return;  // Empty

    I2cTransaction* txn = &i2c_queue.queue[i2c_queue.tail];
    i2c_queue.busy = true;

    I2cError err;
    switch (txn->operation) {
        case I2C_OP_WRITE:
            err = i2c_write_reg(txn->device, txn->reg, txn->data[0]);
            break;
        case I2C_OP_READ:
            err = i2c_read_reg(txn->device, txn->reg, txn->data);
            break;
        case I2C_OP_BURST_READ:
            err = i2c_read_burst(txn->device, txn->reg, txn->data, txn->len);
            break;
    }

    if (txn->callback) {
        txn->callback(err, txn->data, txn->len);
    }

    i2c_queue.tail = (i2c_queue.tail + 1) % I2C_QUEUE_SIZE;
    i2c_queue.busy = false;
}

// ============================================================================
// DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   I2C COMMUNICATION WORKFLOW DEMO\n");
    printf("============================================\n");

    // Initialize mock slave memory
    mock_i2c.slave_memory[IMU_REG_WHO_AM_I] = IMU_WHO_AM_I_VALUE;
    mock_i2c.slave_memory[IMU_REG_ACCEL_XOUT] = 0x10;
    mock_i2c.slave_memory[IMU_REG_ACCEL_XOUT + 1] = 0x00;

    // Create IMU device
    ImuDriver imu = {
        .i2c_dev = {
            .address = IMU_ADDR,
            .speed_hz = 400000,
            .retries = 3
        }
    };

    // Initialize IMU
    I2cError err = imu_init(&imu);
    if (err != I2C_OK) {
        printf("IMU init failed!\n");
        return 1;
    }

    // Read accelerometer
    printf("\n[IMU] Reading accelerometer...\n");
    err = imu_read_accel(&imu);
    if (err == I2C_OK) {
        printf("[IMU] Accel: X=%d, Y=%d, Z=%d\n",
               imu.accel_x, imu.accel_y, imu.accel_z);
    }

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "Explain the I2C read operation."
 * A: "Two-phase operation:
 *    1. Write phase: Send device address (write mode) + register address
 *       This sets the slave's internal register pointer
 *    2. Read phase: Repeated start, send address (read mode), read data
 *       Slave auto-increments register pointer for burst reads
 *
 *    Master ACKs all bytes except the last (NACK signals end)"
 *
 * Q: "How do you handle I2C errors?"
 * A: "1. Check for ACK after each byte sent
 *    2. Implement timeout for stuck bus
 *    3. Retry with exponential backoff
 *    4. Bus recovery: clock out stuck slave with 9 clock pulses
 *    5. Log errors for diagnostics"
 *
 * Q: "What causes I2C bus hang?"
 * A: "Slave holding SDA low (usually interrupted mid-transfer):
 *    - Master reset during transaction
 *    - Noise causing false start/stop
 *    - Slave firmware bug
 *
 *    Recovery: Toggle SCL 9+ times while SDA released
 *    Then generate STOP condition"
 *
 * Q: "How do you choose pull-up resistor value?"
 * A: "Trade-off between rise time and drive strength:
 *    - Lower R = faster rise, but more power
 *    - Higher R = slower rise, less power
 *
 *    Formula: R = tr / (0.847 × Cb)
 *    tr = rise time, Cb = bus capacitance
 *
 *    Typical: 4.7kΩ for 100kHz, 2.2kΩ for 400kHz"
 *
 * ============================================================================
 */
