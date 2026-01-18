/*
 * ============================================================================
 *                    DMA BASICS FOR EMBEDDED SYSTEMS
 * ============================================================================
 *
 * CRITICAL FOR IRONSITE:
 *   - Video streaming from camera sensor
 *   - Audio capture
 *   - High-speed sensor data (IMU at high rate)
 *   - SD card / Flash writes
 *   - Wireless data transfer
 *
 * ============================================================================
 *                    WHAT IS DMA?
 * ============================================================================
 *
 * DMA = Direct Memory Access
 *
 * PROBLEM WITHOUT DMA:
 *   - CPU must read from peripheral, write to memory (or vice versa)
 *   - Each transfer = interrupt or polling + CPU cycles
 *   - CPU is 100% busy during data transfer
 *   - Can't do anything else while moving data
 *
 * SOLUTION WITH DMA:
 *   - DMA controller moves data WITHOUT CPU involvement
 *   - CPU sets up transfer, DMA handles it
 *   - CPU is FREE to do other work (or sleep!)
 *   - Interrupts only at end of transfer (or error)
 *
 *
 * WITHOUT DMA (CPU-driven):
 *
 *   for each byte:
 *     CPU: read from UART_DR    <- CPU busy
 *     CPU: write to buffer[i]   <- CPU busy
 *     i++
 *   // CPU was 100% occupied!
 *
 *
 * WITH DMA:
 *
 *   CPU: configure DMA (source, dest, count)
 *   CPU: start DMA
 *   CPU: ... do other work ...   <- CPU FREE!
 *   DMA: (moving data in background)
 *   IRQ: DMA complete!
 *
 * ============================================================================
 *                    DMA TRANSFER TYPES
 * ============================================================================
 *
 * 1. PERIPHERAL TO MEMORY (P2M):
 *    - Source: Peripheral data register (fixed address)
 *    - Dest: Memory buffer (incrementing address)
 *    - Example: UART RX, ADC, SPI RX, Camera data
 *
 *    UART_DR ----DMA----> buffer[0]
 *    UART_DR ----DMA----> buffer[1]
 *    UART_DR ----DMA----> buffer[2]
 *    ...
 *
 * 2. MEMORY TO PERIPHERAL (M2P):
 *    - Source: Memory buffer (incrementing)
 *    - Dest: Peripheral data register (fixed)
 *    - Example: UART TX, DAC, SPI TX
 *
 *    buffer[0] ----DMA----> UART_DR
 *    buffer[1] ----DMA----> UART_DR
 *    buffer[2] ----DMA----> UART_DR
 *    ...
 *
 * 3. MEMORY TO MEMORY (M2M):
 *    - Source: Memory (incrementing)
 *    - Dest: Memory (incrementing)
 *    - Example: Fast memcpy, frame buffer copy
 *
 *    src[0] ----DMA----> dst[0]
 *    src[1] ----DMA----> dst[1]
 *    ...
 *
 * ============================================================================
 *                    DMA MODES
 * ============================================================================
 *
 * NORMAL MODE:
 *   - Transfer specified count, then stop
 *   - CPU must reconfigure for next transfer
 *   - Use for: One-shot transfers, command responses
 *
 *   |---Transfer N bytes---|STOP
 *                          ^IRQ
 *
 * CIRCULAR MODE (Critical for streaming!):
 *   - Auto-restart at beginning when done
 *   - Continuous data flow
 *   - Use for: Audio, ADC sampling, UART RX
 *
 *   |---Buffer 0---|---Buffer 1---|---Buffer 0---|...
 *                  ^IRQ          ^IRQ          ^IRQ
 *                  (half-complete)
 *
 * DOUBLE BUFFER MODE:
 *   - Two buffers, hardware switches automatically
 *   - While DMA fills one, CPU processes other
 *   - Use for: Video, high-rate sensors
 *
 *   DMA -> [Buffer A] (filling)
 *   CPU <- [Buffer B] (processing)
 *   ...swap...
 *   DMA -> [Buffer B] (filling)
 *   CPU <- [Buffer A] (processing)
 *
 * ============================================================================
 *                    DMA CONFIGURATION PARAMETERS
 * ============================================================================
 *
 * SOURCE ADDRESS:
 *   - Where to read from
 *   - Fixed (peripheral) or incrementing (memory)
 *
 * DESTINATION ADDRESS:
 *   - Where to write to
 *   - Fixed (peripheral) or incrementing (memory)
 *
 * TRANSFER COUNT:
 *   - Number of data items (not always bytes!)
 *
 * DATA WIDTH:
 *   - Byte (8-bit), Half-word (16-bit), Word (32-bit)
 *   - Must match peripheral's data register width
 *
 * PRIORITY:
 *   - Low, Medium, High, Very High
 *   - When multiple DMA channels compete for bus
 *
 * INTERRUPTS:
 *   - Transfer Complete (TC)
 *   - Half Transfer (HT) - useful for double buffering
 *   - Transfer Error (TE)
 *
 * ============================================================================
 *                    DMA + UART EXAMPLE
 * ============================================================================
 *
 * UART RX with DMA (Circular mode):
 *
 *   1. Configure DMA:
 *      - Source: &UART->DR (fixed, peripheral)
 *      - Dest: rx_buffer (incrementing, memory)
 *      - Count: BUFFER_SIZE
 *      - Mode: Circular
 *      - Interrupts: Half-Transfer, Transfer-Complete
 *
 *   2. Start DMA
 *
 *   3. Enable UART DMA request
 *
 *   4. On HT interrupt: Process first half of buffer
 *      On TC interrupt: Process second half of buffer
 *
 *
 *   rx_buffer:
 *   |----First Half----|----Second Half----|
 *   ^                   ^                   ^
 *   Start              HT IRQ             TC IRQ
 *                      |                   |
 *                      Process             Process
 *                      first half          second half
 *
 * ============================================================================
 *                    DMA + ADC EXAMPLE
 * ============================================================================
 *
 * Continuous ADC sampling:
 *
 *   ADC converts at 1kHz -> DMA stores each result -> Circular buffer
 *
 *   ADC_DR ----DMA----> buffer[0..N-1] (circular)
 *
 *   At HT: Average first half samples
 *   At TC: Average second half samples
 *
 *   This allows continuous sampling with no CPU involvement!
 *
 * ============================================================================
 *                    DMA + SPI EXAMPLE (Camera data)
 * ============================================================================
 *
 * Camera line transfer:
 *
 *   1. Camera signals: "Line ready"
 *   2. DMA configured:
 *      - Source: &SPI->DR
 *      - Dest: line_buffer
 *      - Count: LINE_WIDTH * BYTES_PER_PIXEL
 *
 *   3. Start SPI with DMA
 *
 *   4. DMA handles entire line while CPU prepares for next
 *
 *   5. On TC interrupt: Process line, setup next
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
//                    DMA CONFIGURATION STRUCTURES
// ============================================================================

typedef enum {
    DMA_DIR_P2M,    // Peripheral to Memory
    DMA_DIR_M2P,    // Memory to Peripheral
    DMA_DIR_M2M     // Memory to Memory
} DMA_Direction;

typedef enum {
    DMA_WIDTH_BYTE,      // 8-bit
    DMA_WIDTH_HALFWORD,  // 16-bit
    DMA_WIDTH_WORD       // 32-bit
} DMA_DataWidth;

typedef enum {
    DMA_MODE_NORMAL,
    DMA_MODE_CIRCULAR
} DMA_Mode;

typedef enum {
    DMA_PRIORITY_LOW,
    DMA_PRIORITY_MEDIUM,
    DMA_PRIORITY_HIGH,
    DMA_PRIORITY_VERY_HIGH
} DMA_Priority;

typedef struct {
    volatile void* peripheral_addr;  // Peripheral data register
    volatile void* memory_addr;      // Memory buffer
    uint32_t transfer_count;         // Number of items
    DMA_Direction direction;
    DMA_DataWidth data_width;
    DMA_Mode mode;
    DMA_Priority priority;
    bool mem_increment;              // Increment memory address
    bool periph_increment;           // Increment peripheral address (usually no)
    bool enable_tc_interrupt;        // Transfer complete interrupt
    bool enable_ht_interrupt;        // Half transfer interrupt
} DMA_Config;

// Simulated DMA channel state
typedef struct {
    DMA_Config config;
    uint32_t items_remaining;
    bool active;
    bool half_complete;
    bool complete;
} DMA_Channel;

// Simulated hardware
static DMA_Channel dma_channels[8];

// ============================================================================
//                    DMA DRIVER FUNCTIONS
// ============================================================================

void dma_init_channel(uint8_t channel, DMA_Config* config) {
    printf("[DMA] Initializing channel %d\n", channel);
    printf("  Direction: %s\n",
           config->direction == DMA_DIR_P2M ? "Periph->Mem" :
           config->direction == DMA_DIR_M2P ? "Mem->Periph" : "Mem->Mem");
    printf("  Count: %lu items\n", (unsigned long)config->transfer_count);
    printf("  Mode: %s\n", config->mode == DMA_MODE_CIRCULAR ? "Circular" : "Normal");

    dma_channels[channel].config = *config;
    dma_channels[channel].items_remaining = config->transfer_count;
    dma_channels[channel].active = false;
    dma_channels[channel].half_complete = false;
    dma_channels[channel].complete = false;
}

void dma_start(uint8_t channel) {
    printf("[DMA] Starting channel %d\n", channel);
    dma_channels[channel].active = true;
    dma_channels[channel].items_remaining = dma_channels[channel].config.transfer_count;
}

void dma_stop(uint8_t channel) {
    printf("[DMA] Stopping channel %d\n", channel);
    dma_channels[channel].active = false;
}

uint32_t dma_get_remaining(uint8_t channel) {
    return dma_channels[channel].items_remaining;
}

bool dma_is_complete(uint8_t channel) {
    return dma_channels[channel].complete;
}

bool dma_is_half_complete(uint8_t channel) {
    return dma_channels[channel].half_complete;
}

// Simulate DMA transfer progress
void dma_simulate_progress(uint8_t channel, uint32_t items_transferred) {
    DMA_Channel* ch = &dma_channels[channel];

    if (!ch->active) return;

    if (items_transferred >= ch->items_remaining) {
        ch->items_remaining = 0;
        ch->complete = true;
        printf("[DMA] Channel %d: Transfer COMPLETE\n", channel);

        if (ch->config.mode == DMA_MODE_CIRCULAR) {
            // Restart
            ch->items_remaining = ch->config.transfer_count;
            ch->complete = false;
            ch->half_complete = false;
        }
    } else {
        ch->items_remaining -= items_transferred;

        // Check for half complete
        if (ch->items_remaining <= ch->config.transfer_count / 2 && !ch->half_complete) {
            ch->half_complete = true;
            printf("[DMA] Channel %d: HALF complete\n", channel);
        }
    }
}

// ============================================================================
//                    EXAMPLE: UART RX WITH DMA
// ============================================================================

#define UART_RX_BUFFER_SIZE 64

static uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE];
static volatile uint32_t uart_rx_head = 0;  // Where new data goes
static volatile uint32_t uart_rx_tail = 0;  // Where app reads from

// Simulated UART peripheral
typedef struct {
    volatile uint32_t DR;   // Data register
    volatile uint32_t SR;   // Status register
} UART_TypeDef;

static UART_TypeDef uart1 = {0};
static UART_TypeDef* UART1 = &uart1;

void uart_dma_init(void) {
    printf("\n=== UART DMA Setup ===\n");

    DMA_Config config = {
        .peripheral_addr = &UART1->DR,
        .memory_addr = uart_rx_buffer,
        .transfer_count = UART_RX_BUFFER_SIZE,
        .direction = DMA_DIR_P2M,
        .data_width = DMA_WIDTH_BYTE,
        .mode = DMA_MODE_CIRCULAR,
        .priority = DMA_PRIORITY_HIGH,
        .mem_increment = true,
        .periph_increment = false,
        .enable_tc_interrupt = true,
        .enable_ht_interrupt = true
    };

    dma_init_channel(0, &config);
    dma_start(0);

    printf("UART DMA ready - circular buffer receiving\n");
}

// Called from DMA interrupt handler
void uart_dma_half_complete_handler(void) {
    printf("[UART DMA] First half ready, process bytes 0-%d\n",
           UART_RX_BUFFER_SIZE/2 - 1);
    // Application would process uart_rx_buffer[0..31]
}

void uart_dma_complete_handler(void) {
    printf("[UART DMA] Second half ready, process bytes %d-%d\n",
           UART_RX_BUFFER_SIZE/2, UART_RX_BUFFER_SIZE - 1);
    // Application would process uart_rx_buffer[32..63]
}

// ============================================================================
//                    EXAMPLE: ADC WITH DMA
// ============================================================================

#define ADC_BUFFER_SIZE 128

static uint16_t adc_buffer[ADC_BUFFER_SIZE];

typedef struct {
    volatile uint32_t DR;
} ADC_TypeDef;

static ADC_TypeDef adc1 = {0};
static ADC_TypeDef* ADC1 = &adc1;

void adc_dma_init(void) {
    printf("\n=== ADC DMA Setup ===\n");

    DMA_Config config = {
        .peripheral_addr = &ADC1->DR,
        .memory_addr = adc_buffer,
        .transfer_count = ADC_BUFFER_SIZE,
        .direction = DMA_DIR_P2M,
        .data_width = DMA_WIDTH_HALFWORD,  // ADC is 12-bit, stored as 16-bit
        .mode = DMA_MODE_CIRCULAR,
        .priority = DMA_PRIORITY_MEDIUM,
        .mem_increment = true,
        .periph_increment = false,
        .enable_tc_interrupt = true,
        .enable_ht_interrupt = true
    };

    dma_init_channel(1, &config);
    dma_start(1);

    printf("ADC DMA ready - continuous sampling\n");
}

uint16_t adc_process_buffer_half(uint16_t* data, uint16_t count) {
    uint32_t sum = 0;
    for (uint16_t i = 0; i < count; i++) {
        sum += data[i];
    }
    return sum / count;  // Simple average
}

// ============================================================================
//                    EXAMPLE: DOUBLE BUFFERING FOR VIDEO
// ============================================================================

#define FRAME_WIDTH  320
#define FRAME_HEIGHT 240
#define BYTES_PER_PIXEL 2

#define LINE_BUFFER_SIZE (FRAME_WIDTH * BYTES_PER_PIXEL)

// Double buffer for video lines
static uint8_t line_buffer_a[LINE_BUFFER_SIZE];
static uint8_t line_buffer_b[LINE_BUFFER_SIZE];
static uint8_t* active_buffer = line_buffer_a;
static uint8_t* process_buffer = line_buffer_b;

void swap_buffers(void) {
    uint8_t* temp = active_buffer;
    active_buffer = process_buffer;
    process_buffer = temp;
}

void camera_dma_line_complete(void) {
    printf("[CAM DMA] Line complete\n");

    // DMA just finished filling active_buffer
    // Now swap: DMA uses other buffer, CPU processes completed one
    swap_buffers();

    // Reconfigure DMA for new buffer
    DMA_Config config = {
        .peripheral_addr = (void*)0x40000000,  // Simulated camera
        .memory_addr = active_buffer,
        .transfer_count = LINE_BUFFER_SIZE,
        .direction = DMA_DIR_P2M,
        .data_width = DMA_WIDTH_BYTE,
        .mode = DMA_MODE_NORMAL,  // One line at a time
        .priority = DMA_PRIORITY_VERY_HIGH,
        .mem_increment = true,
        .periph_increment = false,
        .enable_tc_interrupt = true,
        .enable_ht_interrupt = false
    };

    dma_init_channel(2, &config);
    dma_start(2);

    // CPU can now process 'process_buffer' while DMA fills 'active_buffer'
    printf("  CPU processing line while DMA captures next\n");
}

// ============================================================================
//                    DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   DMA BASICS DEMO\n");
    printf("============================================\n");

    // Demo 1: UART DMA
    uart_dma_init();
    dma_simulate_progress(0, 32);  // Simulate half transfer
    uart_dma_half_complete_handler();
    dma_simulate_progress(0, 32);  // Simulate complete
    uart_dma_complete_handler();

    // Demo 2: ADC DMA
    adc_dma_init();
    printf("ADC would continuously sample in background...\n");

    // Demo 3: Camera double buffering
    printf("\n=== Camera Double Buffering Demo ===\n");
    camera_dma_line_complete();
    camera_dma_line_complete();

    printf("\n============================================\n");
    printf("   DMA KEY POINTS FOR INTERVIEW\n");
    printf("============================================\n");
    printf("- DMA moves data WITHOUT CPU involvement\n");
    printf("- CPU sets up transfer, DMA executes independently\n");
    printf("- P2M: Peripheral to Memory (UART RX, ADC)\n");
    printf("- M2P: Memory to Peripheral (UART TX, DAC)\n");
    printf("- Circular mode: Auto-restart for streaming\n");
    printf("- Half-Transfer IRQ enables double buffering\n");
    printf("- Double buffer: DMA fills one, CPU processes other\n");
    printf("- CRITICAL for video/audio - CPU can't keep up otherwise\n");

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW Q&A
 * ============================================================================
 *
 * Q: What is DMA and why is it needed?
 * A: "DMA is Direct Memory Access - it moves data between memory and
 *    peripherals without CPU involvement. Without DMA, the CPU would
 *    have to read every byte from a peripheral and write it to memory,
 *    which wastes CPU cycles. With DMA, the CPU just configures the
 *    transfer and is free to do other work while data moves in background."
 *
 * Q: Explain circular mode DMA.
 * A: "In circular mode, when DMA finishes transferring the configured
 *    count, it automatically restarts from the beginning. This creates
 *    a continuous ring buffer. Combined with half-transfer and complete
 *    interrupts, you can process one half while DMA fills the other.
 *    It's essential for streaming data like audio or continuous ADC."
 *
 * Q: How does double buffering work with DMA?
 * A: "You have two buffers. DMA fills buffer A while CPU processes
 *    buffer B. When DMA completes, you swap: DMA now fills B while
 *    CPU processes A. This ensures continuous data flow with no gaps.
 *    For video, this means capturing the next line while processing
 *    the current one."
 *
 * Q: What's the difference between DMA modes?
 * A: "Normal mode does one transfer and stops - good for one-shot
 *    operations like SPI transactions. Circular mode auto-restarts
 *    for continuous streaming. Double-buffer mode uses hardware
 *    buffer switching for zero-overhead swaps."
 *
 * Q: When would you NOT use DMA?
 * A: "For very small transfers, DMA setup overhead exceeds benefit.
 *    Also if you need to process each byte immediately rather than
 *    batched. For single bytes or short messages, polled or interrupt
 *    I/O is simpler. DMA shines for bulk transfers."
 *
 * Q: How would you use DMA for a camera sensor?
 * A: "Configure DMA in circular or double-buffer mode. Camera trigger
 *    starts DMA transfer for one line. On complete, interrupt fires,
 *    swap buffers, start next line DMA, process previous line.
 *    CPU processes at line rate while DMA captures continuously."
 *
 * ============================================================================
 */
