/*
 * ============================================================================
 *                    I2C DRIVER - COMPLETE GUIDE
 * ============================================================================
 *
 * Everything you need to know about I2C for embedded interviews:
 *   - Protocol basics and timing
 *   - Bus conditions (START, STOP, repeated START)
 *   - Read/Write flows with exact bit sequences
 *   - Driver architecture and implementation
 *   - Common problems and debugging
 *
 * ============================================================================
 *                    I2C BASICS
 * ============================================================================
 *
 * WHAT IS I2C?
 *   - Inter-Integrated Circuit (pronounced "I-squared-C" or "I-two-C")
 *   - 2-wire serial bus: SDA (data) + SCL (clock)
 *   - Multi-master, multi-slave capable
 *   - Speeds: Standard 100kHz, Fast 400kHz, Fast+ 1MHz, High 3.4MHz
 *   - Used for: Sensors, EEPROMs, RTCs, port expanders, displays
 *
 * BUS TOPOLOGY:
 *
 *        VCC
 *         |
 *        [Rp]  [Rp]     <- Pull-up resistors (typically 4.7kΩ)
 *         |     |
 *   SDA --+-----+------ [Master] ---- [Slave 1] ---- [Slave 2]
 *         |     |            |             |             |
 *   SCL --+-----+------------+-------------+-------------+
 *
 * WHY OPEN-DRAIN?
 *   - Devices can only pull LOW, resistors pull HIGH
 *   - Allows multiple devices on same bus
 *   - Allows clock stretching (slave holds SCL low)
 *   - Enables multi-master arbitration
 *
 * ============================================================================
 *                    I2C CONDITIONS (CRITICAL TO UNDERSTAND!)
 * ============================================================================
 *
 * DATA VALID RULE:
 *   - SDA must be STABLE when SCL is HIGH
 *   - SDA can only change when SCL is LOW
 *
 *        SCL  ___/‾‾‾‾‾\___
 *        SDA  ==X=======X==    (X = can change, = = must be stable)
 *
 *
 * START CONDITION (S):
 *   - SDA goes LOW while SCL is HIGH
 *   - Signals beginning of transaction
 *   - Only master can generate
 *
 *        SCL  ‾‾‾‾‾‾‾‾‾‾\___
 *        SDA  ‾‾‾‾\_________
 *                  ^ START
 *
 *
 * STOP CONDITION (P):
 *   - SDA goes HIGH while SCL is HIGH
 *   - Signals end of transaction, releases bus
 *
 *        SCL  ___/‾‾‾‾‾‾‾‾‾
 *        SDA  _______/‾‾‾‾‾
 *                    ^ STOP
 *
 *
 * REPEATED START (Sr):
 *   - START without preceding STOP
 *   - Used to change direction or address without releasing bus
 *   - CRITICAL for read operations!
 *
 *        SCL  ___/‾‾‾\___/‾‾‾\___
 *        SDA  ===X===‾‾‾\_______
 *                       ^ REPEATED START
 *
 *
 * ACK/NACK:
 *   - After each 8 data bits, receiver sends ACK (0) or NACK (1)
 *   - ACK: Receiver pulls SDA LOW
 *   - NACK: Receiver leaves SDA HIGH
 *   - Master NACKs last byte of read to signal "done"
 *
 *        Data bits    ACK
 *        D7 D6 D5 D4 D3 D2 D1 D0  A
 *   SDA  X--X--X--X--X--X--X--X--[0]   <- ACK (slave pulls low)
 *   SDA  X--X--X--X--X--X--X--X--[1]   <- NACK (stays high)
 *
 * ============================================================================
 *                    I2C ADDRESS FORMAT
 * ============================================================================
 *
 * 7-BIT ADDRESSING (Most common):
 *
 *   Byte 1: | A6 | A5 | A4 | A3 | A2 | A1 | A0 | R/W |
 *           |<------- 7-bit address ------->|   |
 *                                               |
 *                                     0 = Write, 1 = Read
 *
 *   Example: Device address 0x68 (MPU6050)
 *     Write: 0x68 << 1 | 0 = 0xD0
 *     Read:  0x68 << 1 | 1 = 0xD1
 *
 *
 * 10-BIT ADDRESSING (Rare):
 *   - First byte: 11110 XX 0  (XX = upper 2 bits of address)
 *   - Second byte: Lower 8 bits of address
 *
 * RESERVED ADDRESSES:
 *   0x00: General call
 *   0x01: CBUS address
 *   0x02: Reserved for different bus
 *   0x03: Reserved for future
 *   0x04-0x07: High-speed mode master code
 *   0x78-0x7F: 10-bit addressing / reserved
 *
 * ============================================================================
 *                    I2C WRITE OPERATION (Step by Step)
 * ============================================================================
 *
 * SINGLE BYTE WRITE:
 *   Write value 0x55 to register 0x20 of device 0x68
 *
 *   Master sends:
 *   +-------+-----+-------+-----+-------+-----+-------+-----+------+
 *   |   S   |ADDR |  W    | ACK | REG   | ACK | DATA  | ACK |  P   |
 *   +-------+-----+-------+-----+-------+-----+-------+-----+------+
 *   | START |0xD0 |(0)    | (0) | 0x20  | (0) | 0x55  | (0) | STOP |
 *   +-------+-----+-------+-----+-------+-----+-------+-----+------+
 *           |<-- Slave address -->|<- Register ->|<- Data -->|
 *
 *   Bit-level detail:
 *   1. Master: Generate START (SDA↓ while SCL high)
 *   2. Master: Send 0xD0 (0x68<<1|0) = 1101 0000
 *   3. Slave: ACK (pulls SDA low on 9th clock)
 *   4. Master: Send 0x20 = 0010 0000 (register address)
 *   5. Slave: ACK
 *   6. Master: Send 0x55 = 0101 0101 (data)
 *   7. Slave: ACK
 *   8. Master: Generate STOP (SDA↑ while SCL high)
 *
 *
 * MULTI-BYTE WRITE (Burst/Sequential):
 *   Write 3 bytes starting at register 0x20
 *
 *   +---+------+---+------+---+------+---+------+---+------+---+---+
 *   | S | ADDR | A | REG  | A | DATA | A | DATA | A | DATA | A | P |
 *   +---+------+---+------+---+------+---+------+---+------+---+---+
 *   |   | 0xD0 |   | 0x20 |   | 0x11 |   | 0x22 |   | 0x33 |   |   |
 *   +---+------+---+------+---+------+---+------+---+------+---+---+
 *
 *   - Register auto-increments after each byte (device-specific!)
 *   - Check datasheet if device supports auto-increment
 *
 * ============================================================================
 *                    I2C READ OPERATION (Step by Step)
 * ============================================================================
 *
 * SINGLE BYTE READ:
 *   Read from register 0x20 of device 0x68
 *
 *   THIS IS WHERE REPEATED START IS CRITICAL!
 *
 *   +---+------+---+------+---+----+------+---+------+----+---+
 *   | S | ADDR | A | REG  | A | Sr | ADDR | A | DATA | NA | P |
 *   +---+------+---+------+---+----+------+---+------+----+---+
 *   |   | 0xD0 |   | 0x20 |   |    | 0xD1 |   | 0xXX |    |   |
 *   +---+------+---+------+---+----+------+---+------+----+---+
 *       |<-- WRITE phase -->|    |<----- READ phase ----->|
 *
 *   WHY TWO PHASES?
 *   1. WRITE phase: Tell slave WHICH register to read from
 *   2. READ phase: Actually read the data
 *
 *   WHY REPEATED START (not STOP then START)?
 *   - Keeps bus ownership (prevents other master from taking bus)
 *   - Many devices require it (won't respond to separate transactions)
 *   - Atomic operation: guaranteed to read the register you just set
 *
 *   Step by step:
 *   1. Master: START
 *   2. Master: Send address + WRITE (0xD0)
 *   3. Slave: ACK
 *   4. Master: Send register address (0x20)
 *   5. Slave: ACK
 *   6. Master: REPEATED START (not STOP!)
 *   7. Master: Send address + READ (0xD1)
 *   8. Slave: ACK
 *   9. Slave: Sends data byte
 *   10. Master: NACK (signals "done reading")
 *   11. Master: STOP
 *
 *
 * MULTI-BYTE READ (Burst):
 *   Read 3 bytes starting at register 0x20
 *
 *   +---+------+---+------+---+----+------+---+------+---+------+---+------+----+---+
 *   | S | ADDR | A | REG  | A | Sr | ADDR | A | DATA | A | DATA | A | DATA | NA | P |
 *   +---+------+---+------+---+----+------+---+------+---+------+---+------+----+---+
 *   |   | 0xD0 |   | 0x20 |   |    | 0xD1 |   | 0xXX |   | 0xXX |   | 0xXX |    |   |
 *   +---+------+---+------+---+----+------+---+------+---+------+---+------+----+---+
 *
 *   - Master ACKs all bytes EXCEPT the last one
 *   - NACK on last byte tells slave "stop sending"
 *   - Register auto-increments (device-specific)
 *
 * ============================================================================
 *                    I2C DRIVER ARCHITECTURE
 * ============================================================================
 *
 *   +----------------------------------------------------------+
 *   |  APPLICATION                                             |
 *   |    imu_read_accel(), sensor_get_temp()                   |
 *   +----------------------------------------------------------+
 *                              |
 *                              v
 *   +----------------------------------------------------------+
 *   |  DEVICE DRIVER (sensor-specific)                         |
 *   |    Knows register map, data format, initialization       |
 *   |    mpu6050_init(), mpu6050_read_accel()                  |
 *   +----------------------------------------------------------+
 *                              |
 *                              v
 *   +----------------------------------------------------------+
 *   |  I2C DRIVER (platform-specific)                          |
 *   |    Generic I2C operations, handles bus protocol          |
 *   |    i2c_write_reg(), i2c_read_reg(), i2c_transfer()       |
 *   +----------------------------------------------------------+
 *                              |
 *                              v
 *   +----------------------------------------------------------+
 *   |  I2C HAL (hardware abstraction)                          |
 *   |    Direct register access, interrupt handling            |
 *   |    i2c_hal_start(), i2c_hal_send_byte(), i2c_hal_stop()  |
 *   +----------------------------------------------------------+
 *                              |
 *                              v
 *   +----------------------------------------------------------+
 *   |  HARDWARE (I2C peripheral)                               |
 *   |    I2C1->CR1, I2C1->DR, I2C1->SR1 (STM32 example)        |
 *   +----------------------------------------------------------+
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
//                    I2C HAL (Hardware Abstraction Layer)
// ============================================================================

// I2C status codes
typedef enum {
    I2C_OK = 0,
    I2C_ERROR_NACK,
    I2C_ERROR_BUS_BUSY,
    I2C_ERROR_TIMEOUT,
    I2C_ERROR_ARB_LOST
} I2C_Status;

// Simulated I2C hardware registers (like STM32)
typedef struct {
    volatile uint32_t CR1;      // Control register 1
    volatile uint32_t CR2;      // Control register 2
    volatile uint32_t DR;       // Data register
    volatile uint32_t SR1;      // Status register 1
    volatile uint32_t SR2;      // Status register 2
} I2C_TypeDef;

// Status register bits
#define I2C_SR1_SB      (1 << 0)    // Start bit generated
#define I2C_SR1_ADDR    (1 << 1)    // Address sent/matched
#define I2C_SR1_BTF     (1 << 2)    // Byte transfer finished
#define I2C_SR1_TXE     (1 << 7)    // TX buffer empty
#define I2C_SR1_RXNE    (1 << 6)    // RX buffer not empty
#define I2C_SR1_AF      (1 << 10)   // Acknowledge failure
#define I2C_SR2_BUSY    (1 << 1)    // Bus busy

// Control register bits
#define I2C_CR1_START   (1 << 8)    // Generate START
#define I2C_CR1_STOP    (1 << 9)    // Generate STOP
#define I2C_CR1_ACK     (1 << 10)   // ACK enable
#define I2C_CR1_PE      (1 << 0)    // Peripheral enable

// Simulated hardware instance
static I2C_TypeDef i2c1_hw = {0};
static I2C_TypeDef* I2C1 = &i2c1_hw;

// Simulated device memory (like an EEPROM or sensor)
static uint8_t simulated_device[256] = {
    [0x00] = 0x68,  // WHO_AM_I register
    [0x3B] = 0x12,  // ACCEL_X_H
    [0x3C] = 0x34,  // ACCEL_X_L
    [0x3D] = 0x56,  // ACCEL_Y_H
    [0x3E] = 0x78,  // ACCEL_Y_L
};
static uint8_t sim_register_pointer = 0;

// Simulate device response
static void simulate_device_response(uint8_t byte, bool is_address) {
    if (is_address) {
        uint8_t addr = byte >> 1;
        bool read = byte & 0x01;
        if (addr == 0x68) {
            I2C1->SR1 |= I2C_SR1_ADDR;  // Address matched
            printf("  [SIM] Device 0x68 responded to %s\n", read ? "READ" : "WRITE");
        }
    } else {
        sim_register_pointer = byte;
        I2C1->SR1 |= I2C_SR1_TXE;
        printf("  [SIM] Register pointer set to 0x%02X\n", byte);
    }
}

// ============================================================================
//                    I2C LOW-LEVEL FUNCTIONS
// ============================================================================

I2C_Status i2c_wait_flag(volatile uint32_t* reg, uint32_t flag, uint32_t timeout) {
    // In real code: check flag with timeout
    // Simulated: always succeeds
    *reg |= flag;  // Simulate flag being set
    return I2C_OK;
}

I2C_Status i2c_generate_start(I2C_TypeDef* i2c) {
    printf("  [I2C] Generating START condition\n");
    i2c->CR1 |= I2C_CR1_START;

    // Wait for SB (Start Bit) flag
    // In real hardware: while(!(i2c->SR1 & I2C_SR1_SB));
    i2c->SR1 |= I2C_SR1_SB;

    return I2C_OK;
}

I2C_Status i2c_generate_stop(I2C_TypeDef* i2c) {
    printf("  [I2C] Generating STOP condition\n");
    i2c->CR1 |= I2C_CR1_STOP;
    return I2C_OK;
}

I2C_Status i2c_send_address(I2C_TypeDef* i2c, uint8_t addr, bool read) {
    uint8_t byte = (addr << 1) | (read ? 1 : 0);
    printf("  [I2C] Sending address: 0x%02X (%s)\n", byte, read ? "READ" : "WRITE");

    i2c->DR = byte;
    simulate_device_response(byte, true);

    // Check for ACK
    if (i2c->SR1 & I2C_SR1_AF) {
        printf("  [I2C] NACK received - device not responding!\n");
        return I2C_ERROR_NACK;
    }

    // Clear ADDR flag by reading SR1 and SR2
    volatile uint32_t temp = i2c->SR1;
    temp = i2c->SR2;
    (void)temp;

    return I2C_OK;
}

I2C_Status i2c_send_byte(I2C_TypeDef* i2c, uint8_t data) {
    printf("  [I2C] Sending byte: 0x%02X\n", data);

    // Wait for TXE (TX buffer empty)
    // In real hardware: while(!(i2c->SR1 & I2C_SR1_TXE));

    i2c->DR = data;
    simulate_device_response(data, false);

    // Wait for BTF (Byte Transfer Finished)
    i2c->SR1 |= I2C_SR1_BTF;

    return I2C_OK;
}

I2C_Status i2c_receive_byte(I2C_TypeDef* i2c, uint8_t* data, bool ack) {
    if (ack) {
        i2c->CR1 |= I2C_CR1_ACK;
    } else {
        i2c->CR1 &= ~I2C_CR1_ACK;
        printf("  [I2C] Sending NACK (last byte)\n");
    }

    // Wait for RXNE (RX buffer not empty)
    // In real hardware: while(!(i2c->SR1 & I2C_SR1_RXNE));

    // Simulate reading from device
    *data = simulated_device[sim_register_pointer++];
    printf("  [I2C] Received byte: 0x%02X\n", *data);

    return I2C_OK;
}

// ============================================================================
//                    I2C DRIVER FUNCTIONS
// ============================================================================

// Write single register
I2C_Status i2c_write_reg(I2C_TypeDef* i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    printf("\n[I2C WRITE] Device=0x%02X, Reg=0x%02X, Data=0x%02X\n", dev_addr, reg_addr, data);

    I2C_Status status;

    // 1. Generate START
    status = i2c_generate_start(i2c);
    if (status != I2C_OK) return status;

    // 2. Send device address + WRITE
    status = i2c_send_address(i2c, dev_addr, false);
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    // 3. Send register address
    status = i2c_send_byte(i2c, reg_addr);
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    // 4. Send data
    status = i2c_send_byte(i2c, data);
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    // 5. Generate STOP
    i2c_generate_stop(i2c);

    printf("[I2C WRITE] Complete\n");
    return I2C_OK;
}

// Write multiple registers (burst write)
I2C_Status i2c_write_burst(I2C_TypeDef* i2c, uint8_t dev_addr, uint8_t reg_addr,
                            uint8_t* data, uint16_t len) {
    printf("\n[I2C BURST WRITE] Device=0x%02X, Reg=0x%02X, Len=%d\n", dev_addr, reg_addr, len);

    I2C_Status status;

    // 1. Generate START
    status = i2c_generate_start(i2c);
    if (status != I2C_OK) return status;

    // 2. Send device address + WRITE
    status = i2c_send_address(i2c, dev_addr, false);
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    // 3. Send register address
    status = i2c_send_byte(i2c, reg_addr);
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    // 4. Send all data bytes
    for (uint16_t i = 0; i < len; i++) {
        status = i2c_send_byte(i2c, data[i]);
        if (status != I2C_OK) {
            i2c_generate_stop(i2c);
            return status;
        }
    }

    // 5. Generate STOP
    i2c_generate_stop(i2c);

    printf("[I2C BURST WRITE] Complete\n");
    return I2C_OK;
}

// Read single register - USES REPEATED START!
I2C_Status i2c_read_reg(I2C_TypeDef* i2c, uint8_t dev_addr, uint8_t reg_addr, uint8_t* data) {
    printf("\n[I2C READ] Device=0x%02X, Reg=0x%02X\n", dev_addr, reg_addr);

    I2C_Status status;

    // === WRITE PHASE: Set register pointer ===

    // 1. Generate START
    status = i2c_generate_start(i2c);
    if (status != I2C_OK) return status;

    // 2. Send device address + WRITE
    status = i2c_send_address(i2c, dev_addr, false);
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    // 3. Send register address
    status = i2c_send_byte(i2c, reg_addr);
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    // === READ PHASE: Read data ===

    // 4. Generate REPEATED START (NOT STOP!)
    printf("  [I2C] Generating REPEATED START\n");
    status = i2c_generate_start(i2c);  // Same function, just no STOP before it
    if (status != I2C_OK) return status;

    // 5. Send device address + READ
    status = i2c_send_address(i2c, dev_addr, true);
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    // 6. Read data with NACK (single byte)
    status = i2c_receive_byte(i2c, data, false);  // NACK for last/only byte
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    // 7. Generate STOP
    i2c_generate_stop(i2c);

    printf("[I2C READ] Complete, Data=0x%02X\n", *data);
    return I2C_OK;
}

// Read multiple registers (burst read)
I2C_Status i2c_read_burst(I2C_TypeDef* i2c, uint8_t dev_addr, uint8_t reg_addr,
                           uint8_t* data, uint16_t len) {
    printf("\n[I2C BURST READ] Device=0x%02X, Reg=0x%02X, Len=%d\n", dev_addr, reg_addr, len);

    I2C_Status status;

    // === WRITE PHASE ===
    status = i2c_generate_start(i2c);
    if (status != I2C_OK) return status;

    status = i2c_send_address(i2c, dev_addr, false);
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    status = i2c_send_byte(i2c, reg_addr);
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    // === READ PHASE ===
    printf("  [I2C] Generating REPEATED START\n");
    status = i2c_generate_start(i2c);
    if (status != I2C_OK) return status;

    status = i2c_send_address(i2c, dev_addr, true);
    if (status != I2C_OK) {
        i2c_generate_stop(i2c);
        return status;
    }

    // Read all bytes - ACK all except last
    for (uint16_t i = 0; i < len; i++) {
        bool ack = (i < len - 1);  // ACK for all except last byte
        status = i2c_receive_byte(i2c, &data[i], ack);
        if (status != I2C_OK) {
            i2c_generate_stop(i2c);
            return status;
        }
    }

    i2c_generate_stop(i2c);

    printf("[I2C BURST READ] Complete\n");
    return I2C_OK;
}

// ============================================================================
//                    EXAMPLE DEVICE DRIVER (MPU6050 IMU)
// ============================================================================

#define MPU6050_ADDR        0x68
#define MPU6050_WHO_AM_I    0x75
#define MPU6050_PWR_MGMT_1  0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B

typedef struct {
    I2C_TypeDef* i2c;
    uint8_t address;
} MPU6050;

I2C_Status mpu6050_init(MPU6050* dev, I2C_TypeDef* i2c) {
    dev->i2c = i2c;
    dev->address = MPU6050_ADDR;

    // Read WHO_AM_I to verify device
    uint8_t who_am_i;
    I2C_Status status = i2c_read_reg(dev->i2c, dev->address, MPU6050_WHO_AM_I, &who_am_i);
    if (status != I2C_OK) return status;

    printf("[MPU6050] WHO_AM_I = 0x%02X (expected 0x68)\n", who_am_i);

    // Wake up device (clear sleep bit)
    status = i2c_write_reg(dev->i2c, dev->address, MPU6050_PWR_MGMT_1, 0x00);

    return status;
}

I2C_Status mpu6050_read_accel(MPU6050* dev, int16_t* x, int16_t* y, int16_t* z) {
    uint8_t buffer[6];

    // Burst read 6 bytes starting at ACCEL_XOUT_H
    I2C_Status status = i2c_read_burst(dev->i2c, dev->address, MPU6050_ACCEL_XOUT_H, buffer, 6);
    if (status != I2C_OK) return status;

    // Convert to 16-bit signed (big-endian)
    *x = (int16_t)((buffer[0] << 8) | buffer[1]);
    *y = (int16_t)((buffer[2] << 8) | buffer[3]);
    *z = (int16_t)((buffer[4] << 8) | buffer[5]);

    return I2C_OK;
}

// ============================================================================
//                    DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   I2C DRIVER COMPLETE DEMO\n");
    printf("============================================\n");

    // Initialize simulated device
    simulated_device[0x75] = 0x68;  // WHO_AM_I

    // Demo 1: Single register write
    i2c_write_reg(I2C1, 0x68, 0x6B, 0x00);

    // Demo 2: Single register read
    uint8_t data;
    i2c_read_reg(I2C1, 0x68, 0x75, &data);

    // Demo 3: Burst read
    uint8_t buffer[6];
    i2c_read_burst(I2C1, 0x68, 0x3B, buffer, 6);

    // Demo 4: Device driver usage
    printf("\n--- MPU6050 Device Driver Demo ---\n");
    MPU6050 imu;
    mpu6050_init(&imu, I2C1);

    printf("\n============================================\n");
    printf("   I2C KEY POINTS FOR INTERVIEW\n");
    printf("============================================\n");
    printf("- I2C is 2-wire: SDA (data) + SCL (clock)\n");
    printf("- Open-drain with pull-ups (4.7k typical)\n");
    printf("- START: SDA falls while SCL high\n");
    printf("- STOP: SDA rises while SCL high\n");
    printf("- REPEATED START: START without prior STOP\n");
    printf("- READ needs REPEATED START (not STOP+START)!\n");
    printf("- Master NACKs last byte of read\n");
    printf("- Address byte: 7-bit addr << 1 | R/W bit\n");

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW Q&A
 * ============================================================================
 *
 * Q: What is a repeated START and why is it needed?
 * A: "Repeated START is generating a START condition without first sending
 *    STOP. It's critical for I2C reads because you need two phases: first
 *    write the register address, then read the data. Using repeated START
 *    keeps bus ownership and is required by most devices. If you use
 *    STOP then START, another master could grab the bus, or the device
 *    might reset its register pointer."
 *
 * Q: Why does the master send NACK on the last read byte?
 * A: "The NACK tells the slave 'stop sending data.' If master ACKs, slave
 *    thinks master wants more bytes and keeps the bus busy. NACK signals
 *    end of read, then master sends STOP to release bus."
 *
 * Q: What happens if a slave doesn't ACK?
 * A: "A NACK from slave during address phase means: wrong address, device
 *    not present, or device busy. During data phase it usually means:
 *    device's internal buffer full, or invalid register. Driver should
 *    detect this (AF flag on STM32) and abort with STOP."
 *
 * Q: How is I2C address sent?
 * A: "7-bit address is shifted left by 1, then R/W bit is OR'd into LSB.
 *    So address 0x68 becomes 0xD0 for write or 0xD1 for read."
 *
 * Q: What is clock stretching?
 * A: "Slave holds SCL low to pause the master when it needs more time.
 *    Master must check SCL actually went high before continuing. Used
 *    when slave is slow (like EEPROM during write) or processing data."
 *
 * Q: How would you debug I2C not working?
 * A: "First, check with oscilloscope: are there pull-ups? Is SCL toggling?
 *    Is SDA responding? Common issues: missing pull-ups, wrong address,
 *    address not shifted, SCL/SDA swapped, slave not powered."
 *
 * Q: Write flow for single register?
 * A: "START, address+W, ACK, register address, ACK, data, ACK, STOP."
 *
 * Q: Read flow for single register?
 * A: "START, address+W, ACK, register address, ACK, REPEATED START,
 *    address+R, ACK, data, NACK, STOP."
 *
 * ============================================================================
 */
