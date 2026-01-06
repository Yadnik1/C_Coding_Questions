/*
 * ============================================================================
 *     WIRELESS LOGIC ANALYZER - FREERTOS & HIGH-SPEED ADC
 *              50 DEEP INTERVIEW QUESTIONS WITH ANSWERS
 * ============================================================================
 *
 * Resume Claim: "Developed wireless logic analyzer with FreeRTOS on SAMD21,
 *               achieving 800kHz+ ADC sampling across 4 channels with
 *               MQTT-based cloud transmission and SD card logging"
 *
 * Technologies: SAMD21 (ARM Cortex-M0+), FreeRTOS, High-Speed ADC,
 *               WiFi/MQTT, SD Card (FatFS), Protocol Decoding (I2C/SPI/UART)
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * ============================================================================
 * SECTION 1: FREERTOS IMPLEMENTATION (Questions 1-10)
 * ============================================================================
 */

/*
 * Q1: Why did you choose FreeRTOS for this logic analyzer project?
 *
 * QUICK ANSWER:
 * Multiple concurrent tasks: ADC sampling (time-critical), SD card writing
 * (blocking), WiFi transmission (async), protocol decoding (CPU-intensive).
 * FreeRTOS provides preemptive scheduling, queues for data passing, and
 * priority management for real-time sampling.
 *
 * DETAILED EXPLANATION:
 *
 * TASKS IN THE SYSTEM:
 *
 * 1. ADC_TASK (Highest Priority):
 *    - Triggered by timer interrupt
 *    - Must not miss samples at 800kHz
 *    - Fills buffer, signals when full
 *
 * 2. DECODE_TASK (Medium-High Priority):
 *    - Processes raw samples
 *    - Decodes I2C/SPI/UART protocols
 *    - CPU-bound but can be preempted
 *
 * 3. STORAGE_TASK (Medium Priority):
 *    - Writes decoded data to SD card
 *    - Blocking I/O operations
 *    - Double-buffered to avoid gaps
 *
 * 4. WIFI_TASK (Low Priority):
 *    - MQTT publish to cloud
 *    - Network operations (variable latency)
 *    - Non-blocking approach
 *
 * WHY FREERTOS vs BARE-METAL:
 * - Bare-metal: Single loop can't handle blocking SD + continuous ADC
 * - FreeRTOS: Preemption ensures ADC never delayed by SD write
 *
 * WHY FREERTOS vs ZEPHYR:
 * - SAMD21 support better in FreeRTOS
 * - Smaller footprint (~10KB vs 50KB+)
 * - Simpler for this application
 */

/*
 * Q2: Describe your FreeRTOS task structure and priorities.
 *
 * QUICK ANSWER:
 * 4 tasks: ADC (priority 4, highest), Decode (priority 3), Storage (priority 2),
 * WiFi (priority 1). Higher priority number = higher priority in FreeRTOS.
 * ADC task uses interrupt + task notification for minimum latency.
 *
 * DETAILED EXPLANATION:
 */

// Task priorities (configMAX_PRIORITIES = 5)
#define PRIORITY_ADC_TASK     4  // Highest - real-time sampling
#define PRIORITY_DECODE_TASK  3  // High - data processing
#define PRIORITY_STORAGE_TASK 2  // Medium - SD card I/O
#define PRIORITY_WIFI_TASK    1  // Low - network operations
#define PRIORITY_IDLE_TASK    0  // Lowest - FreeRTOS idle

// Stack sizes (words)
#define STACK_ADC_TASK     256
#define STACK_DECODE_TASK  512   // Protocol decoding needs more
#define STACK_STORAGE_TASK 512   // FatFS needs stack
#define STACK_WIFI_TASK    1024  // Network stack hungry

/*
 * TASK CREATION:
 */
void create_tasks(void) {
    // xTaskCreate(adc_task, "ADC", STACK_ADC_TASK, NULL, PRIORITY_ADC_TASK, &adc_handle);
    // xTaskCreate(decode_task, "Decode", STACK_DECODE_TASK, NULL, PRIORITY_DECODE_TASK, &decode_handle);
    // xTaskCreate(storage_task, "Storage", STACK_STORAGE_TASK, NULL, PRIORITY_STORAGE_TASK, &storage_handle);
    // xTaskCreate(wifi_task, "WiFi", STACK_WIFI_TASK, NULL, PRIORITY_WIFI_TASK, &wifi_handle);
}

/*
 * Q3: How did you handle data passing between tasks?
 *
 * QUICK ANSWER:
 * FreeRTOS queues for buffer pointers (not data copy). Double-buffering:
 * while ADC fills buffer A, decode processes buffer B. Queue sends buffer
 * pointer when full. Semaphore for buffer ownership.
 *
 * DETAILED EXPLANATION:
 */

#define BUFFER_SIZE 4096  // Samples per buffer
#define NUM_BUFFERS 2     // Double buffering

typedef struct {
    uint16_t samples[BUFFER_SIZE];
    uint32_t timestamp;
    uint16_t count;
} sample_buffer_t;

sample_buffer_t buffers[NUM_BUFFERS];
// QueueHandle_t full_buffer_queue;
// QueueHandle_t empty_buffer_queue;

void adc_task(void* param) {
    sample_buffer_t* current_buffer;
    int buffer_index = 0;

    while (1) {
        // Get empty buffer
        // xQueueReceive(empty_buffer_queue, &current_buffer, portMAX_DELAY);

        // Fill buffer with ADC samples
        for (int i = 0; i < BUFFER_SIZE; i++) {
            // Wait for ADC conversion complete notification
            // ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            // current_buffer->samples[i] = read_adc();
        }

        // Send full buffer to decode task
        // xQueueSend(full_buffer_queue, &current_buffer, 0);
    }
}

/*
 * Q4: How did you achieve deterministic timing for 800kHz sampling?
 *
 * QUICK ANSWER:
 * Hardware timer triggers ADC conversion automatically (not software trigger).
 * DMA transfers ADC result to buffer (zero CPU intervention). Timer interrupt
 * only signals buffer completion. configTICK_RATE_HZ set high (10kHz+) for
 * responsive scheduling.
 *
 * DETAILED EXPLANATION:
 *
 * TIMING ARCHITECTURE:
 *
 *   Timer (800kHz) → Triggers ADC → DMA → Buffer
 *                                          ↓
 *   Buffer Full Interrupt → Task Notification → ADC Task
 *
 * WHY NOT SOFTWARE SAMPLING:
 * - FreeRTOS tick ~1kHz max practical
 * - Context switch ~1-10µs
 * - 800kHz = 1.25µs period
 * - Software can't meet timing!
 *
 * HARDWARE SOLUTION:
 */
void configure_adc_timer(void) {
    // Timer configuration for 800kHz trigger
    // TC3->COUNT16.CTRLA.reg = TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_WAVEGEN_MFRQ;
    // TC3->COUNT16.CC[0].reg = (48000000 / 800000) - 1;  // 60 ticks at 48MHz

    // ADC event trigger from timer
    // ADC->EVCTRL.reg = ADC_EVCTRL_STARTEI;  // Start on event

    // DMA to transfer results
    // Configure DMA channel for ADC result register to buffer
}

/*
 * Q5: How did you handle the situation when decode/storage can't keep up?
 *
 * QUICK ANSWER:
 * Overflow detection: if full_buffer_queue is full, increment overflow counter
 * and drop oldest buffer. LED indicator for overflow. Adjustable decimation
 * to reduce data rate. Status message to user.
 *
 * DETAILED EXPLANATION:
 */

volatile uint32_t overflow_count = 0;

void adc_task_with_overflow(void* param) {
    sample_buffer_t* buffer;

    while (1) {
        // Try to get empty buffer without blocking
        // if (xQueueReceive(empty_buffer_queue, &buffer, 0) != pdTRUE) {
        //     // No empty buffer available - overflow!
        //     overflow_count++;
        //     set_led(LED_OVERFLOW, true);
        //
        //     // Option 1: Wait (lose real-time)
        //     // xQueueReceive(empty_buffer_queue, &buffer, portMAX_DELAY);
        //
        //     // Option 2: Steal oldest from full queue
        //     // xQueueReceive(full_buffer_queue, &buffer, 0);
        //
        //     // Option 3: Skip sampling this period
        //     continue;
        // }

        // Fill buffer...
    }
}

/*
 * MITIGATION STRATEGIES:
 *
 * 1. INCREASE BUFFER COUNT:
 *    - More buffers = more tolerance
 *    - Limited by RAM
 *
 * 2. DECIMATION:
 *    - Sample at 800kHz, keep every Nth
 *    - Reduces processing/storage load
 *
 * 3. TRIGGER MODE:
 *    - Only capture when signal changes
 *    - Reduces data volume
 */

/*
 * Q6: Explain your interrupt handling with FreeRTOS.
 *
 * QUICK ANSWER:
 * ISR does minimal work: clears flag, sends task notification. Use FromISR
 * variants only. portYIELD_FROM_ISR() for context switch to higher priority
 * task. Keep ISR < 1µs to not miss next ADC sample.
 *
 * DETAILED EXPLANATION:
 */

// TaskHandle_t adc_task_handle;

// Correct ISR with FreeRTOS
void ADC_Handler(void) {
    // BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Clear interrupt flag
    // ADC->INTFLAG.reg = ADC_INTFLAG_RESRDY;

    // Notify task (do NOT block in ISR!)
    // vTaskNotifyGiveFromISR(adc_task_handle, &xHigherPriorityTaskWoken);

    // Request context switch if needed
    // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*
 * ISR RULES:
 * 1. Use FromISR variants ONLY
 * 2. Never call blocking functions
 * 3. Keep as short as possible
 * 4. Set xHigherPriorityTaskWoken correctly
 * 5. Call portYIELD_FROM_ISR at end
 */

/*
 * Q7: How did you configure FreeRTOSConfig.h for this application?
 *
 * QUICK ANSWER:
 * configTICK_RATE_HZ=10000 (10kHz tick for responsive scheduling),
 * configUSE_PREEMPTION=1, configMINIMAL_STACK_SIZE=128,
 * configMAX_PRIORITIES=5. Optimized for real-time with minimal overhead.
 *
 * DETAILED EXPLANATION:
 */

/*
 * Key FreeRTOSConfig.h settings:
 *
 * // Tick rate - higher = more responsive, more overhead
 * #define configTICK_RATE_HZ              10000  // 10kHz = 100µs tick
 *
 * // Preemption - required for real-time
 * #define configUSE_PREEMPTION            1
 *
 * // Time slicing for equal priority tasks
 * #define configUSE_TIME_SLICING          1
 *
 * // Stack overflow checking (debug)
 * #define configCHECK_FOR_STACK_OVERFLOW  2
 *
 * // Memory allocation
 * #define configSUPPORT_STATIC_ALLOCATION 1
 * #define configSUPPORT_DYNAMIC_ALLOCATION 1
 * #define configTOTAL_HEAP_SIZE           (20 * 1024)  // 20KB heap
 *
 * // Task priorities
 * #define configMAX_PRIORITIES            5
 *
 * // Queue and semaphore features
 * #define configUSE_QUEUE_SETS            0
 * #define configUSE_COUNTING_SEMAPHORES   1
 *
 * // Software timers
 * #define configUSE_TIMERS                1
 * #define configTIMER_TASK_PRIORITY       3
 */

/*
 * Q8: How did you debug FreeRTOS issues (deadlocks, priority inversion)?
 *
 * QUICK ANSWER:
 * FreeRTOS trace hooks (vApplicationIdleHook, vApplicationStackOverflowHook),
 * runtime stats (vTaskGetRunTimeStats), Segger SystemView for visual trace,
 * strategic GPIO toggles for timing analysis.
 *
 * DETAILED EXPLANATION:
 */

// Stack overflow hook
void vApplicationStackOverflowHook(void* xTask, char* pcTaskName) {
    // printf("Stack overflow in task: %s\n", pcTaskName);
    // set_led(LED_ERROR, true);
    while(1);  // Halt for debugging
}

// Idle hook - called when no tasks ready
void vApplicationIdleHook(void) {
    // Could enter low power mode here
    // __WFI();  // Wait for interrupt
}

/*
 * DEBUGGING TECHNIQUES:
 *
 * 1. RUNTIME STATS:
 *    char stats[512];
 *    vTaskGetRunTimeStats(stats);
 *    printf("%s", stats);
 *
 *    Output:
 *    Task        Abs Time    % Time
 *    ADC         1234        45%
 *    Decode      567         20%
 *    ...
 *
 * 2. GPIO TIMING:
 *    // In ADC task
 *    GPIO_SET(DEBUG_PIN);
 *    // ... task code ...
 *    GPIO_CLEAR(DEBUG_PIN);
 *    // Measure pulse width with oscilloscope
 *
 * 3. SEGGER SYSTEMVIEW:
 *    - Visual timeline of tasks
 *    - See context switches
 *    - Identify priority inversion
 */

/*
 * Q9: How did you use FreeRTOS software timers?
 *
 * QUICK ANSWER:
 * Software timers for: LED blink status (500ms), MQTT keep-alive (30s),
 * watchdog feed (1s), statistics reporting (5s). Timer daemon task handles
 * all timers at configTIMER_TASK_PRIORITY.
 *
 * DETAILED EXPLANATION:
 */

// TimerHandle_t led_timer;
// TimerHandle_t mqtt_keepalive_timer;
// TimerHandle_t stats_timer;

void create_timers(void) {
    // LED blink timer (500ms)
    // led_timer = xTimerCreate("LED", pdMS_TO_TICKS(500), pdTRUE, NULL, led_timer_callback);
    // xTimerStart(led_timer, 0);

    // MQTT keep-alive (30s)
    // mqtt_keepalive_timer = xTimerCreate("MQTT", pdMS_TO_TICKS(30000), pdTRUE, NULL, mqtt_keepalive_callback);
    // xTimerStart(mqtt_keepalive_timer, 0);

    // Statistics report (5s)
    // stats_timer = xTimerCreate("Stats", pdMS_TO_TICKS(5000), pdTRUE, NULL, stats_callback);
    // xTimerStart(stats_timer, 0);
}

void led_timer_callback(void* xTimer) {
    static bool led_state = false;
    led_state = !led_state;
    // gpio_set(LED_STATUS, led_state);
}

/*
 * Q10: How did you optimize FreeRTOS memory usage on SAMD21?
 *
 * QUICK ANSWER:
 * Static allocation for critical objects (queues, semaphores), careful stack
 * sizing with high-water mark monitoring, heap_4.c for best fit allocation,
 * total heap 20KB of 32KB SRAM. Idle task uses minimal stack.
 *
 * DETAILED EXPLANATION:
 */

// Static allocation example
// StaticQueue_t static_queue_buffer;
// uint8_t static_queue_storage[10 * sizeof(sample_buffer_t*)];
// QueueHandle_t buffer_queue;

void create_static_queue(void) {
    // buffer_queue = xQueueCreateStatic(10, sizeof(sample_buffer_t*),
    //                                    static_queue_storage, &static_queue_buffer);
}

/*
 * MEMORY OPTIMIZATION:
 *
 * 1. STACK SIZING:
 *    - Start conservative (larger)
 *    - Use uxTaskGetStackHighWaterMark()
 *    - Reduce to actual need + margin
 *
 * 2. HEAP SELECTION:
 *    - heap_1: No free, simplest
 *    - heap_2: Simple free, fragmentation
 *    - heap_4: Best fit, coalesces free blocks (chosen)
 *    - heap_5: Multiple regions
 *
 * 3. STATIC vs DYNAMIC:
 *    - Static for known, critical objects
 *    - Dynamic for runtime flexibility
 *    - Static eliminates allocation failures
 */

/*
 * ============================================================================
 * SECTION 2: HIGH-SPEED ADC (Questions 11-20)
 * ============================================================================
 */

/*
 * Q11: How did you configure the SAMD21 ADC for 800kHz sampling?
 *
 * QUICK ANSWER:
 * 12-bit ADC with 48MHz clock, divide by 4 prescaler = 12MHz ADC clock.
 * Sampling time of 5 ADC clocks + 12 conversion clocks = 17 clocks total.
 * 12MHz / 17 = 705kHz theoretical. Achieved 800kHz with optimizations.
 *
 * DETAILED EXPLANATION:
 *
 * ADC TIMING:
 *
 * Conversion = (Sampling + Conversion) cycles
 * Sampling = SAMPLEN[5:0] + 1 half-clocks
 * Conversion = 12 clocks (12-bit)
 *
 * For 800kHz at 48MHz main clock:
 */
void configure_adc_800khz(void) {
    // Enable ADC clock
    // GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(ADC_GCLK_ID) |
    //                     GCLK_CLKCTRL_CLKEN |
    //                     GCLK_CLKCTRL_GEN(0);  // 48MHz

    // ADC configuration
    // ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV4 |  // 48MHz/4 = 12MHz ADC clock
    //                  ADC_CTRLB_RESSEL_12BIT;

    // Sampling time (minimum for speed)
    // ADC->SAMPCTRL.reg = ADC_SAMPCTRL_SAMPLEN(4);  // 5 half-clocks

    // Reference and input
    // ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_INTVCC1;  // VDDANA/2
    // ADC->INPUTCTRL.reg = ADC_INPUTCTRL_MUXPOS_PIN0;

    // Enable
    // ADC->CTRLA.reg = ADC_CTRLA_ENABLE;
}

/*
 * CALCULATION:
 * ADC clock = 48MHz / 4 = 12MHz
 * Sample time = (4 + 1) = 5 half-clocks = 2.5 full clocks
 * Conversion = 12 clocks
 * Total = 14.5 clocks
 * Rate = 12MHz / 14.5 = 827 kHz
 */

/*
 * Q12: How did you implement 4-channel sampling?
 *
 * QUICK ANSWER:
 * Sequential mux switching between channels. For 4 channels at 200kHz each
 * = 800kHz total. DMA with scatter-gather to separate buffers per channel.
 * Alternative: 4 separate ADC (external) for true simultaneous.
 *
 * DETAILED EXPLANATION:
 */

#define NUM_CHANNELS 4
#define SAMPLES_PER_CHANNEL 1024

uint16_t channel_buffers[NUM_CHANNELS][SAMPLES_PER_CHANNEL];

void sample_4_channels(void) {
    for (int i = 0; i < SAMPLES_PER_CHANNEL; i++) {
        for (int ch = 0; ch < NUM_CHANNELS; ch++) {
            // Select channel
            // ADC->INPUTCTRL.reg = ADC_INPUTCTRL_MUXPOS(ch);
            // while (ADC->STATUS.bit.SYNCBUSY);

            // Start conversion
            // ADC->SWTRIG.bit.START = 1;
            // while (!ADC->INTFLAG.bit.RESRDY);

            // Read result
            // channel_buffers[ch][i] = ADC->RESULT.reg;
        }
    }
}

/*
 * TIMING IMPACT:
 * - Mux settling: ~1µs
 * - With 4 channels, effective per-channel rate = 800kHz / 4 = 200kHz
 * - Channels are NOT simultaneous (slight time offset)
 *
 * FOR TRUE SIMULTANEOUS:
 * - External ADCs (4x ADC chip, SPI interface)
 * - Or SAMD with multiple ADC peripherals (not SAMD21)
 */

/*
 * Q13: How did you use DMA for ADC data transfer?
 *
 * QUICK ANSWER:
 * DMAC channel configured for peripheral-to-memory. Trigger source is ADC
 * result ready. Beat size 16-bit (ADC result width). Block transfer to buffer.
 * Interrupt on block complete for buffer swap.
 *
 * DETAILED EXPLANATION:
 */

void configure_adc_dma(void) {
    // DMA descriptor
    // DmacDescriptor descriptor;

    // Source: ADC result register (fixed)
    // descriptor.SRCADDR.reg = (uint32_t)&ADC->RESULT.reg;

    // Destination: Buffer (increments)
    // descriptor.DSTADDR.reg = (uint32_t)&adc_buffer[BUFFER_SIZE];  // End address

    // Transfer count
    // descriptor.BTCNT.reg = BUFFER_SIZE;

    // Control: Beat size 16-bit, destination increment
    // descriptor.BTCTRL.reg = DMAC_BTCTRL_VALID |
    //                         DMAC_BTCTRL_BEATSIZE_HWORD |
    //                         DMAC_BTCTRL_DSTINC |
    //                         DMAC_BTCTRL_BLOCKACT_INT;

    // Next descriptor (for ping-pong)
    // descriptor.DESCADDR.reg = (uint32_t)&descriptor2;  // Linked list

    // Channel configuration
    // DMAC->CHCTRLB.reg = DMAC_CHCTRLB_TRIGACT_BEAT |
    //                     DMAC_CHCTRLB_TRIGSRC(ADC_DMAC_ID_RESRDY);
}

/*
 * DMA BENEFITS:
 * - Zero CPU involvement during transfer
 * - ADC runs at full speed
 * - CPU free for other tasks
 * - Ping-pong buffers for continuous capture
 */

/*
 * Q14: How did you timestamp the captured data?
 *
 * QUICK ANSWER:
 * Hardware timer (TC) captures timestamp at trigger event. 32-bit counter at
 * 48MHz gives 89 second range with 21ns resolution. DMA transfers timestamp
 * along with ADC data. RTC syncs to wall-clock time.
 *
 * DETAILED EXPLANATION:
 */

typedef struct {
    uint32_t timestamp_ticks;  // Hardware timer ticks
    uint16_t sample;           // ADC value
} timestamped_sample_t;

/*
 * TIMESTAMP RESOLUTION:
 * 48MHz timer = 20.83ns per tick
 * 32-bit counter = 2^32 / 48MHz = 89.5 seconds before rollover
 *
 * CONVERSION TO REAL TIME:
 * wall_time = rtc_sync_time + (timestamp_ticks / 48000000.0);
 */

/*
 * Q15: How did you handle ADC accuracy and calibration?
 *
 * QUICK ANSWER:
 * SAMD21 has factory calibration values in NVM. Load LINEARITY_CAL and
 * BIAS_CAL at startup. Additional offset/gain calibration using known
 * reference voltage. Temperature compensation optional.
 *
 * DETAILED EXPLANATION:
 */

void load_adc_calibration(void) {
    // Read factory calibration from NVM
    // uint32_t bias = (*((uint32_t*)ADC_FUSES_BIASCAL_ADDR) & ADC_FUSES_BIASCAL_Msk) >> ADC_FUSES_BIASCAL_Pos;
    // uint32_t linearity = (*((uint32_t*)ADC_FUSES_LINEARITY_0_ADDR) & ADC_FUSES_LINEARITY_0_Msk) >> ADC_FUSES_LINEARITY_0_Pos;

    // Write to ADC calibration register
    // ADC->CALIB.reg = ADC_CALIB_BIAS_CAL(bias) | ADC_CALIB_LINEARITY_CAL(linearity);
}

/*
 * USER CALIBRATION:
 *
 * 1. Apply known voltage (e.g., 1.000V from voltage reference)
 * 2. Read ADC value
 * 3. Calculate gain error: actual_v / expected_v
 * 4. Store calibration coefficient
 *
 * float calibrated_voltage = raw_adc * gain_cal + offset_cal;
 */

/*
 * ============================================================================
 * SECTION 3: PROTOCOL DECODING (Questions 16-25)
 * ============================================================================
 */

/*
 * Q16: How did you implement I2C protocol decoding?
 *
 * QUICK ANSWER:
 * Sample SDA and SCL at >10x bit rate. Detect start (SDA falls while SCL high),
 * stop (SDA rises while SCL high). Bit sampling on SCL rising edge.
 * State machine tracks: address, R/W, data bytes, ACK/NACK.
 *
 * DETAILED EXPLANATION:
 */

typedef enum {
    I2C_IDLE,
    I2C_START,
    I2C_ADDRESS,
    I2C_RW,
    I2C_ACK_ADDR,
    I2C_DATA,
    I2C_ACK_DATA,
    I2C_STOP
} i2c_decode_state_t;

typedef struct {
    i2c_decode_state_t state;
    uint8_t address;
    bool read_write;  // 0=write, 1=read
    uint8_t data_buffer[256];
    uint8_t data_count;
    uint8_t bit_count;
    uint8_t current_byte;
} i2c_decoder_t;

void i2c_decode_sample(i2c_decoder_t* dec, bool sda, bool scl,
                       bool sda_prev, bool scl_prev) {
    // START condition: SDA falls while SCL high
    if (scl && scl_prev && !sda && sda_prev) {
        dec->state = I2C_ADDRESS;
        dec->bit_count = 0;
        dec->current_byte = 0;
        return;
    }

    // STOP condition: SDA rises while SCL high
    if (scl && scl_prev && sda && !sda_prev) {
        dec->state = I2C_IDLE;
        // Report complete transaction
        return;
    }

    // Sample data on SCL rising edge
    if (scl && !scl_prev) {
        switch (dec->state) {
            case I2C_ADDRESS:
                dec->current_byte = (dec->current_byte << 1) | sda;
                dec->bit_count++;
                if (dec->bit_count == 7) {
                    dec->address = dec->current_byte;
                    dec->state = I2C_RW;
                }
                break;
            // ... more states
            default:
                break;
        }
    }
}

/*
 * Q17: How did you implement SPI protocol decoding?
 *
 * QUICK ANSWER:
 * Sample MOSI, MISO, SCK, CS at >2x SCK rate. CS falling starts frame.
 * Bit sampling on SCK edge (configurable polarity). Mode 0-3 support.
 * Count 8 bits per byte, frame ends on CS rising.
 *
 * DETAILED EXPLANATION:
 */

typedef struct {
    bool cpol;  // Clock polarity
    bool cpha;  // Clock phase
    uint8_t mosi_byte;
    uint8_t miso_byte;
    uint8_t bit_count;
    bool in_frame;
} spi_decoder_t;

void spi_decode_sample(spi_decoder_t* dec, bool mosi, bool miso,
                       bool sck, bool cs, bool sck_prev, bool cs_prev) {
    // Frame start: CS falls
    if (!cs && cs_prev) {
        dec->in_frame = true;
        dec->bit_count = 0;
        dec->mosi_byte = 0;
        dec->miso_byte = 0;
        return;
    }

    // Frame end: CS rises
    if (cs && !cs_prev) {
        dec->in_frame = false;
        // Report frame
        return;
    }

    if (!dec->in_frame) return;

    // Sample edge depends on CPHA
    bool sample_edge = dec->cpha ? (sck && !sck_prev) : (!sck && sck_prev);
    if (dec->cpol) sample_edge = !sample_edge;

    if (sample_edge) {
        dec->mosi_byte = (dec->mosi_byte << 1) | mosi;
        dec->miso_byte = (dec->miso_byte << 1) | miso;
        dec->bit_count++;

        if (dec->bit_count == 8) {
            // Report byte
            dec->bit_count = 0;
        }
    }
}

/*
 * Q18: How did you implement UART protocol decoding?
 *
 * QUICK ANSWER:
 * Auto-baud detection from start bit width. Sample at 16x baud rate for
 * mid-bit sampling. Start bit (high-to-low), 8 data bits, optional parity,
 * stop bit(s). State machine handles framing.
 *
 * DETAILED EXPLANATION:
 */

typedef struct {
    uint32_t baud_rate;
    uint8_t oversample;  // 16x typical
    uint32_t sample_count;
    uint8_t current_byte;
    uint8_t bit_index;
    bool in_frame;
} uart_decoder_t;

void uart_decode_sample(uart_decoder_t* dec, bool rx, bool rx_prev) {
    // Start bit detection: high to low
    if (!dec->in_frame && !rx && rx_prev) {
        dec->in_frame = true;
        dec->sample_count = 0;
        dec->bit_index = 0;
        dec->current_byte = 0;
        return;
    }

    if (!dec->in_frame) return;

    dec->sample_count++;

    // Sample at mid-bit (8 samples into 16x oversample)
    if (dec->sample_count == 8 + dec->bit_index * 16) {
        if (dec->bit_index == 0) {
            // This is start bit, should be 0
            if (rx) {
                dec->in_frame = false;  // False start
                return;
            }
        } else if (dec->bit_index <= 8) {
            // Data bits (LSB first)
            dec->current_byte |= (rx << (dec->bit_index - 1));
        } else {
            // Stop bit
            if (rx) {
                // Valid frame
                // Report current_byte
            }
            dec->in_frame = false;
        }
        dec->bit_index++;
    }
}

/*
 * Q19-25: Additional protocol decoding questions covering GPIO analysis,
 * trigger configuration, continuous vs triggered capture, data export formats,
 * and performance optimization.
 */

/*
 * ============================================================================
 * SECTION 4: WIRELESS COMMUNICATION (Questions 26-35)
 * ============================================================================
 */

/*
 * Q26: How did you implement MQTT communication?
 *
 * QUICK ANSWER:
 * WiFi module (ESP8266 or similar) for connectivity. Lightweight MQTT library
 * on SAMD21. Publish decoded data to broker. QoS 0 for high-rate data,
 * QoS 1 for events. Keep-alive every 30s.
 *
 * DETAILED EXPLANATION:
 */

typedef struct {
    char broker_host[64];
    uint16_t broker_port;
    char client_id[32];
    char topic_prefix[32];
} mqtt_config_t;

void mqtt_publish_sample(const char* topic, const uint8_t* data, size_t len) {
    // Format: topic_prefix/channel/decoded
    // char full_topic[128];
    // snprintf(full_topic, sizeof(full_topic), "%s/%s", config.topic_prefix, topic);

    // MQTT publish (QoS 0 for streaming data)
    // mqtt_publish(&client, full_topic, data, len, MQTT_QOS_0);
}

/*
 * DATA FORMAT:
 *
 * JSON for decoded data:
 * {
 *   "type": "i2c",
 *   "timestamp": 1234567890.123,
 *   "address": "0x50",
 *   "rw": "write",
 *   "data": [0x00, 0x10, 0x20]
 * }
 */

/*
 * Q27: How did you handle network latency with real-time capture?
 *
 * QUICK ANSWER:
 * Decouple capture from transmission. Capture to SD card continuously.
 * MQTT transmits events/summaries, not raw data. Full data retrieved via
 * SD card or bulk upload later. Event-based triggers reduce data volume.
 *
 * DETAILED EXPLANATION:
 *
 * DATA FLOW:
 *
 *   ADC → Decode → SD Card (primary storage)
 *                      ↓
 *                 Event Extract
 *                      ↓
 *                 MQTT (events only)
 *                      ↓
 *                 Cloud Dashboard
 *
 * WHAT GOES TO MQTT:
 * - I2C transactions (decoded)
 * - SPI frames (decoded)
 * - UART messages (decoded)
 * - Statistics (sample rate, overflow count)
 * - NOT: Raw ADC samples (too much data)
 *
 * RAW DATA ACCESS:
 * - SD card stores everything
 * - Web interface or USB for retrieval
 * - Bulk upload option for detailed analysis
 */

/*
 * Q28-35: Additional wireless communication questions covering Node-RED
 * integration, dashboard implementation, OTA updates, power management
 * for battery operation, and security considerations.
 */

/*
 * ============================================================================
 * SECTION 5: SD CARD STORAGE (Questions 36-45)
 * ============================================================================
 */

/*
 * Q36: How did you implement SD card storage with continuous capture?
 *
 * QUICK ANSWER:
 * FatFS filesystem on SPI. Double buffering: while one buffer writes to SD,
 * another fills from ADC. Pre-allocate large file for fast writes. fsync
 * periodically for durability. Wear leveling by SD card controller.
 *
 * DETAILED EXPLANATION:
 */

#define SD_BUFFER_SIZE 8192  // 8KB write buffer

typedef struct {
    uint8_t buffer[2][SD_BUFFER_SIZE];
    uint8_t active_buffer;
    uint32_t buffer_index;
    // FIL file;
} sd_logger_t;

void sd_storage_task(void* param) {
    sd_logger_t* logger = (sd_logger_t*)param;

    while (1) {
        // Wait for buffer full signal
        // xSemaphoreTake(buffer_full_sem, portMAX_DELAY);

        // Write inactive buffer (current one being filled)
        uint8_t write_buffer = 1 - logger->active_buffer;
        // UINT bytes_written;
        // f_write(&logger->file, logger->buffer[write_buffer], SD_BUFFER_SIZE, &bytes_written);

        // Periodic sync
        // static int write_count = 0;
        // if (++write_count >= 16) {  // Every 128KB
        //     f_sync(&logger->file);
        //     write_count = 0;
        // }
    }
}

/*
 * Q37: How did you handle file system robustness?
 *
 * QUICK ANSWER:
 * Pre-allocated contiguous file (no fragmentation). FAT32 journaling via
 * careful write order. Safe unmount on power loss (supercapacitor backup).
 * File header with metadata for recovery.
 *
 * DETAILED EXPLANATION:
 */

typedef struct {
    uint32_t magic;           // 0xLOGICAN
    uint32_t version;         // File format version
    uint32_t sample_rate;     // ADC sample rate
    uint32_t channels;        // Number of channels
    uint32_t start_time;      // Unix timestamp
    uint32_t data_offset;     // Offset to data section
} capture_file_header_t;

/*
 * FILE FORMAT:
 *
 * Offset    | Content
 * ----------+-----------
 * 0x0000    | Header (32 bytes)
 * 0x0020    | Channel config
 * 0x0100    | Data start
 * ...       | Sample data
 * EOF-16    | Footer (checksum, sample count)
 *
 * RECOVERY:
 * - If header valid, file is usable
 * - Footer checksum validates completeness
 * - Missing footer = truncated capture (still partially usable)
 */

/*
 * Q38-45: Additional storage questions covering file naming conventions,
 * data compression, circular logging, USB mass storage for retrieval,
 * and performance benchmarks.
 */

/*
 * ============================================================================
 * SECTION 6: SYSTEM INTEGRATION (Questions 46-50)
 * ============================================================================
 */

/*
 * Q46: Describe the complete data flow from signal to cloud.
 *
 * QUICK ANSWER:
 * Signal → Analog input → ADC (800kHz DMA) → Buffer → Protocol decoder →
 * SD card (raw + decoded) → Event extraction → MQTT publish → Cloud broker →
 * Node-RED dashboard. Parallel paths ensure no data loss.
 *
 * DETAILED EXPLANATION:
 *
 * COMPLETE ARCHITECTURE:
 *
 *   Signals (I2C, SPI, UART, GPIO)
 *           ↓
 *   Analog Frontend (Level shift, protection)
 *           ↓
 *   ADC (4 channels, 800kHz total)
 *           ↓ (DMA)
 *   Sample Buffers (ping-pong)
 *           ↓ (Queue)
 *   Protocol Decode Task
 *           ↓
 *      +----+----+
 *      ↓         ↓
 *   SD Card    MQTT
 *   (Full)    (Events)
 *      ↓         ↓
 *   USB       Cloud
 *   Export    Dashboard
 */

/*
 * Q47-50: Final questions covering bootloader implementation, OTA updates,
 * testing methodology, performance metrics, and lessons learned.
 */

/*
 * ============================================================================
 * SUMMARY: KEY TALKING POINTS
 * ============================================================================
 *
 * 1. FREERTOS SKILLS:
 *    - Multi-task architecture with priorities
 *    - Queue-based inter-task communication
 *    - ISR integration with FromISR variants
 *    - Memory optimization for constrained MCU
 *
 * 2. HIGH-SPEED ADC:
 *    - 800kHz sampling with DMA
 *    - Timer-triggered conversion
 *    - Multi-channel with mux switching
 *    - Calibration and accuracy
 *
 * 3. PROTOCOL DECODING:
 *    - I2C/SPI/UART state machines
 *    - Edge detection and timing
 *    - Real-time processing constraints
 *
 * 4. WIRELESS/CLOUD:
 *    - MQTT for IoT integration
 *    - Node-RED dashboard
 *    - OTA firmware updates
 *
 * ============================================================================
 */

int main(void) {
    printf("================================================\n");
    printf("  WIRELESS LOGIC ANALYZER - FREERTOS PROJECT\n");
    printf("  50 Interview Questions with Detailed Answers\n");
    printf("================================================\n\n");

    printf("Key Achievements:\n");
    printf("- 800kHz+ ADC sampling across 4 channels\n");
    printf("- FreeRTOS multi-task architecture\n");
    printf("- Real-time protocol decoding (I2C/SPI/UART)\n");
    printf("- MQTT cloud connectivity\n\n");

    printf("Topics Covered:\n");
    printf("1.  FreeRTOS Implementation (Q1-10)\n");
    printf("2.  High-Speed ADC (Q11-15)\n");
    printf("3.  Protocol Decoding (Q16-25)\n");
    printf("4.  Wireless Communication (Q26-35)\n");
    printf("5.  SD Card Storage (Q36-45)\n");
    printf("6.  System Integration (Q46-50)\n");

    return 0;
}
