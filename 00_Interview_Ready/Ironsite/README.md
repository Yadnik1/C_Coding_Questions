# Ironsite Interview Prep - Embedded Firmware Engineer

## Role: Wearable Camera for Construction Workers
- MCU: ESP32 / STM32
- Sensors: IMU (accelerometer, gyroscope)
- Video: Camera capture, buffer management
- Connectivity: Wi-Fi, Bluetooth
- Features: OTA updates, low power, fall detection

---

## Files in This Folder

### Sensors & Protocols
| File | Topics |
|------|--------|
| 01_imu_basics.c | 6-axis vs 9-axis, Allan variance, noise filtering, sensor fusion, fall detection |
| 02_i2c_driver.c | I2C protocol, START/STOP, repeated START, read/write flows, driver architecture |
| 03_spi_driver.c | SPI modes (CPOL/CPHA), full-duplex, flash driver example |
| 12_adc_dac.c | ADC sampling, resolution, DAC output |

### MCU Core
| File | Topics |
|------|--------|
| 04_dma_basics.c | DMA P2M/M2P/M2M, circular mode, double buffering |
| 06_gpio_nvic.c | GPIO modes, EXTI interrupts, NVIC priorities |
| 09_volatile_usage.c | volatile keyword, memory-mapped registers |

### RTOS & System
| File | Topics |
|------|--------|
| 05_freertos_basics.c | Tasks, queues, semaphores, mutexes, ISR-safe APIs |
| 07_ring_buffer.c | Circular buffer implementation |
| 08_state_machine.c | State machine patterns |
| 10_watchdog.c | Watchdog timer patterns |

### System Design
| File | Topics |
|------|--------|
| 11_wearable_camera_design.c | Complete Ironsite system design |
| 16_boot_initialization.c | Boot sequence, peripheral init |
| 17_low_power.c | Sleep modes, wake sources, power optimization |

### OTA & Boot
| File | Topics |
|------|--------|
| 14_ota_updates.c | OTA architecture, delta updates, rollback |
| 15_bootloaders.c | Bootloader design, secure boot, A/B partitions |

### Hardware
| File | Topics |
|------|--------|
| 13_noise_filtering.c | Coupling caps, decoupling, EMI/EMC basics |

---

## Key Topics to Review

### 1. IMU (01_imu_basics.c)
- 6-axis: Accelerometer (3) + Gyroscope (3)
- 9-axis: + Magnetometer (3)
- Allan Variance: Measures noise characteristics
- Filtering: Low-pass, complementary, Kalman
- Fall Detection: Free fall -> Impact -> Motionless

### 2. I2C Protocol (02_i2c_driver.c)
- START: SDA low while SCL high
- STOP: SDA high while SCL high
- Repeated START: For read operations (no STOP between write and read)
- ACK/NACK: 9th clock pulse

### 3. SPI Protocol (03_spi_driver.c)
- Mode 0: CPOL=0, CPHA=0 (most common)
- Mode 1: CPOL=0, CPHA=1
- Mode 2: CPOL=1, CPHA=0
- Mode 3: CPOL=1, CPHA=1
- Full-duplex: Always send AND receive

### 4. DMA (04_dma_basics.c)
- P2M: Peripheral to Memory (ADC, UART RX)
- M2P: Memory to Peripheral (UART TX)
- M2M: Memory to Memory (fast copy)
- Circular: Auto-restart for streaming
- Double buffer: Ping-pong for video

### 5. FreeRTOS (05_freertos_basics.c)
- xTaskCreate, vTaskDelay, vTaskDelete
- xQueueSend, xQueueReceive
- xSemaphoreTake, xSemaphoreGive
- FromISR variants for interrupt context

### 6. NVIC (06_gpio_nvic.c)
- Priority: Lower number = higher priority
- Preemption vs Sub-priority
- EXTI: External interrupt for GPIO pins
- Pending bit: Clear in ISR

### 7. OTA Updates (14_ota_updates.c)
- A/B partitions for rollback
- Delta updates to save bandwidth
- Signature verification
- Watchdog for recovery

### 8. Low Power (17_low_power.c)
- Sleep modes: Sleep, Stop, Standby
- Wake sources: GPIO, RTC, UART
- Current consumption optimization

---

## Interview Quick Reference

### Volatile Keyword
```c
volatile uint32_t *reg = (volatile uint32_t *)0x40000000;
// Prevents compiler optimization
// Used for: memory-mapped registers, ISR shared variables
```

### Ring Buffer
```c
buffer[head++ % SIZE] = data;  // Write
data = buffer[tail++ % SIZE];  // Read
// Full: (head + 1) % SIZE == tail
// Empty: head == tail
```

### I2C Read Sequence
```
START -> Addr+W -> RegAddr -> REPEATED START -> Addr+R -> Data -> NACK -> STOP
```

### SPI Transaction
```
CS LOW -> Send cmd -> Send/Recv data -> CS HIGH
// MOSI and MISO transfer simultaneously
```

### FreeRTOS ISR Pattern
```c
void ISR_Handler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
```

---

## Study Order (Recommended)
1. 09_volatile_usage.c - Foundation
2. 06_gpio_nvic.c - GPIO and interrupts
3. 02_i2c_driver.c - I2C protocol
4. 03_spi_driver.c - SPI protocol
5. 01_imu_basics.c - IMU sensors
6. 04_dma_basics.c - DMA transfers
7. 05_freertos_basics.c - RTOS basics
8. 07_ring_buffer.c - Buffer patterns
9. 11_wearable_camera_design.c - System design
10. 14_ota_updates.c - OTA concepts
11. 17_low_power.c - Power optimization

Good luck with your Ironsite interview!
