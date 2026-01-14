/*
 * ============================================================================
 *        DESIGN: UART DRIVER ARCHITECTURE
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★☆ (HIGH)
 *   - Debug console
 *   - External sensor communication
 *   - GPS modules
 *   - Cellular modem AT commands
 *
 * ============================================================================
 *                    PROBLEM STATEMENT
 * ============================================================================
 *
 * Design a UART driver that:
 *   - Non-blocking TX and RX
 *   - Interrupt-driven (not polling)
 *   - Buffered for burst data
 *   - Line-based or packet-based reception
 *   - Handles errors (framing, overrun, parity)
 *
 * ============================================================================
 *                    UART HARDWARE BASICS
 * ============================================================================
 *
 * UART FRAME:
 *
 *   Idle ──┐    ┌─┬─┬─┬─┬─┬─┬─┬─┬───┬─┬── Idle
 *          │    │ │ │ │ │ │ │ │ │   │ │
 *          └────┴─┴─┴─┴─┴─┴─┴─┴─┴───┴─┘
 *          Start D0 D1 D2 D3 D4 D5 D6 D7 Par Stop
 *            |<-------- 8 data bits ------->|
 *
 * COMMON SETTINGS:
 *   - 8 data bits, No parity, 1 stop bit (8N1)
 *   - Baud rates: 9600, 115200, 921600
 *
 * HARDWARE REGISTERS (typical):
 *   - DR: Data Register (read RX, write TX)
 *   - SR: Status Register (flags)
 *   - CR: Control Register (enable, interrupts)
 *   - BRR: Baud Rate Register
 *
 * INTERRUPTS:
 *   - RXNE: RX Not Empty (data received)
 *   - TXE: TX Empty (can send next byte)
 *   - TC: Transmission Complete (all bits sent)
 *   - IDLE: Line idle detected
 *   - Error: Framing, Overrun, Parity, Noise
 *
 * ============================================================================
 *                    ARCHITECTURE LAYERS
 * ============================================================================
 *
 *   ┌─────────────────────────────────────────┐
 *   │           APPLICATION                   │
 *   │    (AT commands, debug prints)          │
 *   └─────────────────┬───────────────────────┘
 *                     │ uart_send_line(), uart_receive_line()
 *   ┌─────────────────┴───────────────────────┐
 *   │            UART DRIVER                  │
 *   │    (buffer management, callbacks)       │
 *   └─────────────────┬───────────────────────┘
 *                     │ uart_write_byte(), ISR
 *   ┌─────────────────┴───────────────────────┐
 *   │          HARDWARE ABSTRACTION           │
 *   │    (register access, interrupt setup)   │
 *   └─────────────────┬───────────────────────┘
 *                     │
 *   ┌─────────────────┴───────────────────────┐
 *   │           UART HARDWARE                 │
 *   └─────────────────────────────────────────┘
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// HARDWARE ABSTRACTION LAYER (HAL)
// ============================================================================

/*
 * In real code, these would be register definitions:
 *
 *   #define UART1_BASE    0x40011000
 *   #define UART1_DR      (*(volatile uint32_t*)(UART1_BASE + 0x04))
 *   #define UART1_SR      (*(volatile uint32_t*)(UART1_BASE + 0x00))
 *   #define UART1_CR1     (*(volatile uint32_t*)(UART1_BASE + 0x0C))
 */

// Simulated hardware state
typedef struct {
    uint8_t tx_data;
    uint8_t rx_data;
    bool tx_empty;
    bool rx_ready;
    bool tx_complete;
} MockUartHW;

MockUartHW mock_hw = { .tx_empty = true, .rx_ready = false, .tx_complete = true };

// HAL functions
void hal_uart_write_byte(uint8_t byte) {
    mock_hw.tx_data = byte;
    mock_hw.tx_empty = false;
    printf("[HAL] TX: 0x%02X '%c'\n", byte, (byte >= 32 && byte < 127) ? byte : '.');
    // In real HW: UART->DR = byte;
}

uint8_t hal_uart_read_byte(void) {
    mock_hw.rx_ready = false;
    return mock_hw.rx_data;
    // In real HW: return UART->DR;
}

void hal_uart_enable_tx_interrupt(void) {
    printf("[HAL] TX interrupt enabled\n");
    // In real HW: UART->CR1 |= UART_CR1_TXEIE;
}

void hal_uart_disable_tx_interrupt(void) {
    printf("[HAL] TX interrupt disabled\n");
    // In real HW: UART->CR1 &= ~UART_CR1_TXEIE;
}

void hal_uart_enable_rx_interrupt(void) {
    printf("[HAL] RX interrupt enabled\n");
    // In real HW: UART->CR1 |= UART_CR1_RXNEIE;
}


// ============================================================================
// RING BUFFER (from previous design)
// ============================================================================

#define UART_BUF_SIZE 64  // Must be power of 2

typedef struct {
    uint8_t buffer[UART_BUF_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} UartBuffer;

void buf_init(UartBuffer* buf) {
    buf->head = 0;
    buf->tail = 0;
}

bool buf_is_empty(UartBuffer* buf) {
    return buf->head == buf->tail;
}

bool buf_is_full(UartBuffer* buf) {
    return ((buf->head + 1) & (UART_BUF_SIZE - 1)) == buf->tail;
}

uint8_t buf_count(UartBuffer* buf) {
    return (buf->head - buf->tail) & (UART_BUF_SIZE - 1);
}

bool buf_push(UartBuffer* buf, uint8_t data) {
    if (buf_is_full(buf)) return false;
    buf->buffer[buf->head] = data;
    buf->head = (buf->head + 1) & (UART_BUF_SIZE - 1);
    return true;
}

bool buf_pop(UartBuffer* buf, uint8_t* data) {
    if (buf_is_empty(buf)) return false;
    *data = buf->buffer[buf->tail];
    buf->tail = (buf->tail + 1) & (UART_BUF_SIZE - 1);
    return true;
}


// ============================================================================
// UART DRIVER
// ============================================================================

typedef void (*UartRxCallback)(uint8_t byte);
typedef void (*UartLineCallback)(const char* line);

typedef struct {
    // Buffers
    UartBuffer tx_buf;
    UartBuffer rx_buf;

    // Line buffering
    char line_buf[128];
    uint8_t line_idx;

    // Callbacks
    UartRxCallback rx_byte_callback;
    UartLineCallback rx_line_callback;

    // State
    volatile bool tx_in_progress;

    // Error counters
    uint32_t rx_overrun_count;
    uint32_t framing_error_count;
} UartDriver;

UartDriver uart1 = {0};

// Initialize driver
void uart_init(UartDriver* uart) {
    buf_init(&uart->tx_buf);
    buf_init(&uart->rx_buf);
    uart->line_idx = 0;
    uart->tx_in_progress = false;
    uart->rx_overrun_count = 0;
    uart->framing_error_count = 0;

    hal_uart_enable_rx_interrupt();
}

// ============================================================================
// TX PATH
// ============================================================================

/*
 * TX FLOW:
 *
 *   uart_send()
 *       │
 *       ├──► Put bytes in TX buffer
 *       │
 *       ├──► Enable TXE interrupt
 *       │
 *       └──► Return (non-blocking)
 *
 *   TX ISR (TXE interrupt)
 *       │
 *       ├──► Pop byte from TX buffer
 *       │
 *       ├──► Write to HW register
 *       │
 *       └──► If buffer empty: disable TXE interrupt
 */

// Non-blocking send
int uart_send(UartDriver* uart, const uint8_t* data, uint16_t len) {
    int sent = 0;

    for (uint16_t i = 0; i < len; i++) {
        if (!buf_push(&uart->tx_buf, data[i])) {
            break;  // Buffer full
        }
        sent++;
    }

    // Start transmission if not already running
    if (sent > 0 && !uart->tx_in_progress) {
        uart->tx_in_progress = true;
        hal_uart_enable_tx_interrupt();
    }

    return sent;
}

// Convenience: send null-terminated string
int uart_send_string(UartDriver* uart, const char* str) {
    return uart_send(uart, (const uint8_t*)str, strlen(str));
}

// TX interrupt handler
void uart_tx_isr(UartDriver* uart) {
    uint8_t byte;

    if (buf_pop(&uart->tx_buf, &byte)) {
        hal_uart_write_byte(byte);
    } else {
        // Buffer empty, disable TX interrupt
        hal_uart_disable_tx_interrupt();
        uart->tx_in_progress = false;
    }
}


// ============================================================================
// RX PATH
// ============================================================================

/*
 * RX FLOW:
 *
 *   RX ISR (RXNE interrupt)
 *       │
 *       ├──► Read byte from HW register
 *       │
 *       ├──► Push to RX buffer (or call callback)
 *       │
 *       └──► If line mode: check for '\n'
 *
 *   Main loop
 *       │
 *       ├──► Poll: uart_read() / uart_read_line()
 *       │
 *       └──► Or: process callback
 */

// RX interrupt handler
void uart_rx_isr(UartDriver* uart) {
    uint8_t byte = hal_uart_read_byte();

    // Option 1: Byte callback
    if (uart->rx_byte_callback) {
        uart->rx_byte_callback(byte);
        return;
    }

    // Option 2: Buffer + line detection
    if (!buf_push(&uart->rx_buf, byte)) {
        uart->rx_overrun_count++;
        return;  // Buffer full, byte lost
    }

    // Line mode: accumulate until newline
    if (uart->rx_line_callback) {
        if (byte == '\n' || byte == '\r') {
            if (uart->line_idx > 0) {
                uart->line_buf[uart->line_idx] = '\0';
                uart->rx_line_callback(uart->line_buf);
                uart->line_idx = 0;
            }
        } else if (uart->line_idx < sizeof(uart->line_buf) - 1) {
            uart->line_buf[uart->line_idx++] = byte;
        }
    }
}

// Non-blocking read
int uart_read(UartDriver* uart, uint8_t* data, uint16_t max_len) {
    int read = 0;

    while (read < max_len && buf_pop(&uart->rx_buf, &data[read])) {
        read++;
    }

    return read;
}

// Read until newline (blocking-ish, with timeout)
bool uart_read_line(UartDriver* uart, char* line, uint16_t max_len, uint32_t timeout_ms) {
    (void)timeout_ms;  // Would use real timer
    uint16_t idx = 0;
    uint8_t byte;

    while (idx < max_len - 1) {
        if (buf_pop(&uart->rx_buf, &byte)) {
            if (byte == '\n') {
                line[idx] = '\0';
                return true;
            } else if (byte != '\r') {
                line[idx++] = byte;
            }
        }
        // In real code: check timeout
    }

    line[idx] = '\0';
    return false;
}


// ============================================================================
// ERROR HANDLING
// ============================================================================

/*
 * COMMON UART ERRORS:
 *
 * 1. OVERRUN ERROR (ORE):
 *    New byte received before previous was read
 *    Solution: Faster ISR, larger buffer, DMA
 *
 * 2. FRAMING ERROR (FE):
 *    Invalid stop bit detected
 *    Cause: Baud rate mismatch, noise
 *    Solution: Check baud rate config, hardware
 *
 * 3. PARITY ERROR (PE):
 *    Parity bit doesn't match
 *    Solution: Check parity settings both ends
 *
 * 4. NOISE ERROR (NE):
 *    Noise detected during sampling
 *    Solution: Better cabling, lower baud rate
 */

void uart_error_isr(UartDriver* uart, uint32_t error_flags) {
    if (error_flags & 0x01) {  // Overrun
        uart->rx_overrun_count++;
        printf("[ERR] UART overrun!\n");
    }
    if (error_flags & 0x02) {  // Framing
        uart->framing_error_count++;
        printf("[ERR] UART framing error!\n");
    }
    // Clear error flags in real HW
}


// ============================================================================
// DMA MODE (For high throughput)
// ============================================================================

/*
 * For high baud rates or continuous data (video metadata stream),
 * use DMA instead of byte-by-byte interrupts.
 *
 * DMA TX:
 *   1. Set up DMA: source = buffer, dest = UART DR, count = len
 *   2. Start DMA
 *   3. DMA complete interrupt when done
 *
 * DMA RX:
 *   1. Set up DMA: source = UART DR, dest = buffer, count = buf_size
 *   2. Use circular mode or double-buffering
 *   3. Process data in half-transfer and complete interrupts
 *
 * PSEUDO-CODE:
 *
 *   void uart_send_dma(uint8_t* data, uint16_t len) {
 *       DMA_Config(DMA_UART_TX, data, &UART_DR, len);
 *       DMA_Enable(DMA_UART_TX);
 *   }
 *
 *   void DMA_TX_Complete_ISR(void) {
 *       tx_complete_callback();
 *   }
 */


// ============================================================================
// USAGE PATTERNS
// ============================================================================

// Pattern 1: Debug printf redirection
int _write(int file, char* ptr, int len) {
    (void)file;
    uart_send(&uart1, (uint8_t*)ptr, len);
    return len;
}

// Pattern 2: AT command interface
typedef struct {
    UartDriver* uart;
    void (*response_callback)(const char* response);
} ATInterface;

void at_send_command(ATInterface* at, const char* cmd) {
    uart_send_string(at->uart, cmd);
    uart_send_string(at->uart, "\r\n");
}

// Pattern 3: Packet-based protocol
#define PACKET_SYNC 0xAA

typedef struct {
    uint8_t sync;
    uint8_t length;
    uint8_t command;
    uint8_t data[32];
    uint8_t checksum;
} UartPacket;

typedef enum {
    PKT_STATE_SYNC,
    PKT_STATE_LENGTH,
    PKT_STATE_COMMAND,
    PKT_STATE_DATA,
    PKT_STATE_CHECKSUM
} PacketState;

typedef struct {
    PacketState state;
    UartPacket packet;
    uint8_t data_idx;
    void (*packet_callback)(UartPacket* pkt);
} PacketParser;

void packet_parser_feed(PacketParser* parser, uint8_t byte) {
    switch (parser->state) {
        case PKT_STATE_SYNC:
            if (byte == PACKET_SYNC) {
                parser->packet.sync = byte;
                parser->state = PKT_STATE_LENGTH;
            }
            break;

        case PKT_STATE_LENGTH:
            parser->packet.length = byte;
            parser->state = PKT_STATE_COMMAND;
            break;

        case PKT_STATE_COMMAND:
            parser->packet.command = byte;
            parser->data_idx = 0;
            if (parser->packet.length > 0) {
                parser->state = PKT_STATE_DATA;
            } else {
                parser->state = PKT_STATE_CHECKSUM;
            }
            break;

        case PKT_STATE_DATA:
            parser->packet.data[parser->data_idx++] = byte;
            if (parser->data_idx >= parser->packet.length) {
                parser->state = PKT_STATE_CHECKSUM;
            }
            break;

        case PKT_STATE_CHECKSUM:
            parser->packet.checksum = byte;
            // Verify and deliver packet
            if (parser->packet_callback) {
                parser->packet_callback(&parser->packet);
            }
            parser->state = PKT_STATE_SYNC;
            break;
    }
}


// ============================================================================
// DEMO
// ============================================================================

void rx_line_handler(const char* line) {
    printf("[CALLBACK] Received line: '%s'\n", line);
}

int main() {
    printf("============================================\n");
    printf("   UART DRIVER ARCHITECTURE DEMO\n");
    printf("============================================\n");

    // Initialize
    uart_init(&uart1);
    uart1.rx_line_callback = rx_line_handler;

    // Demo TX
    printf("\n--- TX Demo ---\n");
    uart_send_string(&uart1, "Hello, World!\r\n");

    // Simulate TX ISR calls
    printf("\n--- Simulating TX ISR ---\n");
    while (uart1.tx_in_progress) {
        uart_tx_isr(&uart1);
    }

    // Demo RX (simulate incoming data)
    printf("\n--- RX Demo ---\n");
    const char* incoming = "OK\r\n";
    for (int i = 0; incoming[i]; i++) {
        mock_hw.rx_data = incoming[i];
        mock_hw.rx_ready = true;
        uart_rx_isr(&uart1);
    }

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "Walk through your UART driver architecture."
 * A: "Three layers:
 *    1. HAL: Direct register access, platform-specific
 *    2. Driver: Buffering, interrupts, callbacks
 *    3. Application: Line parsing, protocol handling
 *
 *    TX path: App writes to ring buffer, ISR drains to hardware
 *    RX path: ISR fills ring buffer, app reads or gets callback
 *
 *    Non-blocking by design - never busy-wait."
 *
 * Q: "How do you handle buffer overflow?"
 * A: "TX overflow: uart_send() returns count of bytes accepted.
 *       Caller can retry or wait.
 *    RX overflow: ISR drops byte, increments error counter.
 *       Design buffer size for worst-case burst.
 *       Consider flow control (RTS/CTS) for critical data."
 *
 * Q: "When would you use DMA instead of interrupts?"
 * A: "DMA when:
 *    - High baud rate (> 115200)
 *    - Continuous data streams
 *    - CPU needs to do other work
 *    - Large transfers (file download)
 *
 *    Interrupts when:
 *    - Low data rate
 *    - Variable-length messages
 *    - Need immediate byte-level processing"
 *
 * Q: "How do you debug UART communication issues?"
 * A: "1. Check baud rate with oscilloscope (measure bit time)
 *    2. Verify 8N1 settings match on both ends
 *    3. Check TX/RX crossover (TX->RX, RX->TX)
 *    4. Check voltage levels (3.3V vs 5V)
 *    5. Check flow control settings
 *    6. Log error counters (overrun, framing)"
 *
 * ============================================================================
 *                    EDGE CASES
 * ============================================================================
 *
 * 1. BACK-TO-BACK FRAMES:
 *    No idle time between frames (allowed by spec)
 *    Must handle without losing sync
 *
 * 2. BREAK CONDITION:
 *    TX held low for > 1 frame time
 *    Used for attention/reset in some protocols
 *
 * 3. PARTIAL LINE IN BUFFER:
 *    Line mode: what if line > buffer?
 *    Truncate and deliver, or error?
 *
 * 4. ISR DURING INIT:
 *    Enable interrupts last in init sequence
 *
 * 5. TX WHILE SLEEPING:
 *    If MCU sleeps with TX in progress, data corrupted
 *    Wait for TX complete before sleep
 *
 * ============================================================================
 */
