/*
 * ============================================================================
 *        DESIGN: INTERRUPT + DEFERRED EXECUTION MODEL
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★★ (CRITICAL)
 *   - Camera ISR for frame capture
 *   - IMU interrupt for fall detection
 *   - WiFi/BLE callbacks
 *   - Button press handling
 *
 * ============================================================================
 *                    PROBLEM STATEMENT
 * ============================================================================
 *
 * Interrupts must be SHORT to avoid:
 *   - Missing subsequent interrupts
 *   - Blocking higher-priority interrupts
 *   - Watchdog timeouts
 *   - Real-time deadline misses
 *
 * But interrupt events often require COMPLEX processing:
 *   - Parse received packet
 *   - Update state machine
 *   - Communicate with other peripherals
 *   - Log data to storage
 *
 * SOLUTION: Split work into two parts:
 *   1. ISR (Interrupt Service Routine): Fast, minimal work
 *   2. Deferred handler: Runs in main context, does heavy work
 *
 * ============================================================================
 *                    THE GOLDEN RULES
 * ============================================================================
 *
 * IN ISR - DO:
 *   ✓ Clear interrupt flag
 *   ✓ Copy data to buffer
 *   ✓ Set flag or queue event
 *   ✓ Wake up main loop
 *
 * IN ISR - DON'T:
 *   ✗ Malloc/free
 *   ✗ Printf/logging
 *   ✗ Long loops
 *   ✗ Block on mutex/semaphore
 *   ✗ Call non-reentrant functions
 *   ✗ Access slow peripherals (UART TX, flash write)
 *
 * ============================================================================
 *                    DESIGN APPROACHES
 * ============================================================================
 *
 * APPROACH 1: FLAG-BASED (Simplest)
 *
 *   ISR sets flag → Main loop polls flag → Clears flag and processes
 *
 *   Pros: Simple, no RTOS needed
 *   Cons: Latency depends on main loop speed
 *
 *
 * APPROACH 2: EVENT QUEUE (Recommended)
 *
 *   ISR pushes event to queue → Main loop pops and processes
 *
 *   Pros: Multiple events buffered, preserves order
 *   Cons: Queue can overflow if main loop too slow
 *
 *
 * APPROACH 3: RTOS SEMAPHORE/TASK NOTIFICATION
 *
 *   ISR signals semaphore → Blocked task wakes and processes
 *
 *   Pros: Immediate response, clean design
 *   Cons: Requires RTOS
 *
 *
 * APPROACH 4: BOTTOM HALF / WORK QUEUE (Linux style)
 *
 *   ISR schedules work item → Kernel executes in process context
 *
 *   Used in: Linux kernel drivers, Zephyr work queues
 *
 * ============================================================================
 *                    TIMING DIAGRAM
 * ============================================================================
 *
 *   IRQ Pin ─────────┐         ┌─────────┐
 *                    │         │         │
 *   ─────────────────┴─────────┴─────────┴─────────
 *                    │
 *   ISR         [====]  (short: copy data, set flag)
 *                    │
 *   Main loop   ─────────────[========]  (long: process data)
 *                            │
 *                     deferred execution
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// APPROACH 1: FLAG-BASED DEFERRED EXECUTION
// ============================================================================

/*
 * PSEUDOCODE:
 *
 *   // In ISR
 *   void UART_IRQHandler(void) {
 *       rx_buffer[rx_head++] = UART->DR;  // Copy data
 *       uart_rx_pending = true;            // Set flag
 *       clear_interrupt_flag();
 *   }
 *
 *   // In main loop
 *   while (1) {
 *       if (uart_rx_pending) {
 *           uart_rx_pending = false;
 *           process_uart_data();  // Heavy work here
 *       }
 *       // ... other tasks
 *   }
 */

// Volatile because accessed from ISR and main
volatile bool uart_rx_pending = false;
volatile bool button_pressed = false;
volatile bool timer_expired = false;
volatile bool imu_data_ready = false;

// ISR - simulated
void UART_IRQHandler(void) {
    // Quick: just set flag
    uart_rx_pending = true;
    printf("[ISR] UART RX - flag set\n");
}

void BUTTON_IRQHandler(void) {
    button_pressed = true;
    printf("[ISR] Button - flag set\n");
}

void process_uart_data(void) {
    printf("[MAIN] Processing UART data (heavy work)...\n");
    // Parse packet, update state, etc.
}

void process_button_press(void) {
    printf("[MAIN] Processing button (debounce, state change)...\n");
}

void flag_based_main_loop(void) {
    printf("\n--- FLAG-BASED APPROACH ---\n");

    // Simulate interrupts
    UART_IRQHandler();
    BUTTON_IRQHandler();

    // Main loop processes flags
    printf("[MAIN] Checking flags...\n");

    if (uart_rx_pending) {
        uart_rx_pending = false;
        process_uart_data();
    }

    if (button_pressed) {
        button_pressed = false;
        process_button_press();
    }
}


// ============================================================================
// APPROACH 2: EVENT QUEUE (Recommended)
// ============================================================================

/*
 * Better than flags because:
 *   - Can buffer multiple events
 *   - Preserves event order
 *   - Can pass data with event
 *   - Handles burst of interrupts
 */

typedef enum {
    EVT_NONE = 0,
    EVT_UART_RX,
    EVT_BUTTON_PRESS,
    EVT_TIMER_TICK,
    EVT_IMU_DATA,
    EVT_WIFI_CONNECTED,
    EVT_LOW_BATTERY,
} EventType;

typedef struct {
    EventType type;
    uint32_t timestamp;
    union {
        uint8_t uart_byte;
        uint8_t button_id;
        struct { int16_t x, y, z; } imu;
        uint8_t battery_pct;
    } data;
} Event;

#define EVENT_QUEUE_SIZE 16

typedef struct {
    Event buffer[EVENT_QUEUE_SIZE];
    volatile uint8_t head;  // Write position (ISR)
    volatile uint8_t tail;  // Read position (main)
    volatile uint8_t count;
} EventQueue;

EventQueue event_queue = {0};

// Initialize queue
void event_queue_init(EventQueue* q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

// Push event from ISR (lock-free for single producer)
bool event_queue_push_isr(EventQueue* q, Event* evt) {
    if (q->count >= EVENT_QUEUE_SIZE) {
        return false;  // Queue full - event lost!
    }

    q->buffer[q->head] = *evt;
    q->head = (q->head + 1) % EVENT_QUEUE_SIZE;
    q->count++;

    return true;
}

// Pop event from main loop
bool event_queue_pop(EventQueue* q, Event* evt) {
    if (q->count == 0) {
        return false;  // Queue empty
    }

    *evt = q->buffer[q->tail];
    q->tail = (q->tail + 1) % EVENT_QUEUE_SIZE;
    q->count--;

    return true;
}

bool event_queue_is_empty(EventQueue* q) {
    return q->count == 0;
}

// ISR using event queue
void UART_IRQHandler_Queue(void) {
    Event evt = {
        .type = EVT_UART_RX,
        .timestamp = 1000,  // Would be real timestamp
        .data.uart_byte = 0x55
    };

    if (!event_queue_push_isr(&event_queue, &evt)) {
        printf("[ISR] WARNING: Event queue full!\n");
    }
    printf("[ISR] UART event queued\n");
}

void IMU_IRQHandler_Queue(void) {
    Event evt = {
        .type = EVT_IMU_DATA,
        .timestamp = 1001,
        .data.imu = { .x = 100, .y = -50, .z = 980 }
    };

    event_queue_push_isr(&event_queue, &evt);
    printf("[ISR] IMU event queued\n");
}

// Event dispatcher in main loop
void dispatch_event(Event* evt) {
    switch (evt->type) {
        case EVT_UART_RX:
            printf("[MAIN] Handle UART RX: 0x%02X\n", evt->data.uart_byte);
            break;

        case EVT_BUTTON_PRESS:
            printf("[MAIN] Handle button %d\n", evt->data.button_id);
            break;

        case EVT_IMU_DATA:
            printf("[MAIN] Handle IMU: x=%d, y=%d, z=%d\n",
                   evt->data.imu.x, evt->data.imu.y, evt->data.imu.z);
            // Check for fall detection, etc.
            break;

        case EVT_LOW_BATTERY:
            printf("[MAIN] Handle low battery: %d%%\n", evt->data.battery_pct);
            break;

        default:
            printf("[MAIN] Unknown event type: %d\n", evt->type);
            break;
    }
}

void event_queue_main_loop(void) {
    printf("\n--- EVENT QUEUE APPROACH ---\n");

    event_queue_init(&event_queue);

    // Simulate multiple interrupts
    UART_IRQHandler_Queue();
    IMU_IRQHandler_Queue();
    UART_IRQHandler_Queue();

    // Process all events
    printf("[MAIN] Processing event queue...\n");
    Event evt;
    while (event_queue_pop(&event_queue, &evt)) {
        dispatch_event(&evt);
    }
}


// ============================================================================
// APPROACH 3: RTOS SEMAPHORE PATTERN (Pseudo-code)
// ============================================================================

/*
 * FreeRTOS Example:
 *
 *   SemaphoreHandle_t uart_sem;
 *
 *   // ISR
 *   void UART_IRQHandler(void) {
 *       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 *
 *       // Copy data to buffer
 *       rx_buffer[rx_head++] = UART->DR;
 *
 *       // Signal waiting task
 *       xSemaphoreGiveFromISR(uart_sem, &xHigherPriorityTaskWoken);
 *
 *       // Context switch if higher priority task woken
 *       portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
 *   }
 *
 *   // Task
 *   void uart_task(void* params) {
 *       while (1) {
 *           // Block until ISR signals
 *           xSemaphoreTake(uart_sem, portMAX_DELAY);
 *
 *           // Process data
 *           process_uart_data();
 *       }
 *   }
 *
 * KEY POINTS:
 *   - xSemaphoreGiveFromISR() is ISR-safe
 *   - portYIELD_FROM_ISR() triggers immediate context switch
 *   - Task blocks efficiently (no polling)
 */

// ============================================================================
// APPROACH 4: WORK QUEUE (Zephyr style)
// ============================================================================

/*
 * Zephyr Example:
 *
 *   struct k_work uart_work;
 *
 *   void uart_work_handler(struct k_work* work) {
 *       // This runs in system workqueue thread
 *       process_uart_data();
 *   }
 *
 *   void UART_IRQHandler(void) {
 *       // Copy data
 *       rx_buffer[rx_head++] = UART->DR;
 *
 *       // Schedule work
 *       k_work_submit(&uart_work);
 *   }
 *
 *   void init(void) {
 *       k_work_init(&uart_work, uart_work_handler);
 *   }
 *
 * BENEFITS:
 *   - Work runs in thread context (can use any API)
 *   - System manages scheduling
 *   - Can have dedicated work queues for different priorities
 */

// ============================================================================
// REAL-WORLD EXAMPLE: IMU + FALL DETECTION
// ============================================================================

/*
 * SCENARIO: IMU generates interrupt at 100Hz, need fall detection
 *
 * BAD DESIGN (all in ISR):
 *
 *   void IMU_IRQHandler(void) {
 *       read_imu_registers();      // I2C read - SLOW!
 *       apply_filter();            // Math operations
 *       check_fall_condition();    // More computation
 *       if (fall_detected) {
 *           log_to_flash();        // Flash write - VERY SLOW!
 *           send_alert();          // Network - BLOCKING!
 *       }
 *   }
 *
 * GOOD DESIGN (ISR + deferred):
 *
 *   // Pre-configured DMA reads IMU data to buffer
 *
 *   void IMU_IRQHandler(void) {
 *       // DMA already copied data
 *       imu_buffer_ready = true;
 *       // Or: queue event with data
 *   }
 *
 *   void imu_processing_task(void) {
 *       if (imu_buffer_ready) {
 *           imu_buffer_ready = false;
 *           apply_filter(&imu_data);
 *           if (check_fall_condition(&imu_data)) {
 *               queue_event(EVT_FALL_DETECTED);  // Main loop handles alert
 *           }
 *       }
 *   }
 */

// ============================================================================
// ISR LATENCY ANALYSIS
// ============================================================================

/*
 * MEASURING ISR TIME:
 *
 *   void ISR_Handler(void) {
 *       GPIO_SET(DEBUG_PIN);       // Set pin high
 *
 *       // ... ISR work ...
 *
 *       GPIO_CLEAR(DEBUG_PIN);     // Set pin low
 *   }
 *
 *   Measure pulse width with oscilloscope
 *
 *
 * TYPICAL ISR TIME BUDGETS:
 *
 *   Application          | Max ISR Time | Reason
 *   ---------------------|--------------|---------------------------
 *   Audio (48kHz)        | < 10µs       | 20.8µs between samples
 *   Motor control        | < 50µs       | Fast PWM update needed
 *   Video frame          | < 100µs      | High data rate
 *   General GPIO/button  | < 1ms        | Not time-critical
 *
 *
 * IF ISR IS TOO LONG:
 *
 *   1. Move work to deferred handler
 *   2. Use DMA for data transfer
 *   3. Use hardware features (FIFOs)
 *   4. Reduce processing (lower filter order)
 *   5. Increase CPU clock
 */

// ============================================================================
// INTERRUPT PRIORITY DESIGN
// ============================================================================

/*
 * PRIORITY LEVELS (ARM Cortex-M example):
 *
 *   Priority | Use Case
 *   ---------|------------------------------------------
 *   0 (High) | Safety-critical (watchdog, fault handlers)
 *   1        | Time-critical (motor control, audio DMA)
 *   2        | Communication (UART, SPI complete)
 *   3        | Sensor data ready (IMU, ADC)
 *   4 (Low)  | User input (buttons, touch)
 *
 *
 * RULES:
 *   - Higher priority can preempt lower priority ISR
 *   - Same priority: first-come-first-served
 *   - Don't make everything high priority!
 *   - Consider using NVIC priority grouping
 *
 *
 * PRIORITY INVERSION PROBLEM:
 *
 *   High-priority task waiting for resource held by low-priority task
 *
 *   Solution: Avoid sharing resources between ISR and tasks
 *             Or use priority inheritance (RTOS feature)
 */

// ============================================================================
// DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   INTERRUPT + DEFERRED EXECUTION DEMO\n");
    printf("============================================\n");

    // Demo both approaches
    flag_based_main_loop();
    event_queue_main_loop();

    printf("\n--- SUMMARY ---\n");
    printf("Flag-based: Simple, good for few interrupt sources\n");
    printf("Event queue: Scalable, preserves order, handles bursts\n");
    printf("RTOS semaphore: Best latency, clean design, needs RTOS\n");

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "Your ISR takes too long. How do you fix it?"
 * A: "1. Profile ISR time with GPIO toggle + scope
 *    2. Identify heavy operations (I2C reads, flash writes)
 *    3. Move heavy work to deferred handler
 *    4. Use DMA for data transfer
 *    5. Use hardware FIFOs if available
 *    6. Only do: clear flag, copy data, set event"
 *
 * Q: "Event queue is full - what happens?"
 * A: "Events are lost! Solutions:
 *    1. Increase queue size
 *    2. Process events faster (higher priority task)
 *    3. Implement overflow counter for debugging
 *    4. For some events: coalesce (drop intermediate values)
 *    5. Consider: is event rate too high? Reduce at source"
 *
 * Q: "When would you NOT use deferred execution?"
 * A: "When latency is critical and work is minimal:
 *    - Toggling GPIO in response to signal
 *    - Capturing timestamp for precise timing
 *    - Single register write
 *    Rule: If ISR < 1µs, deferred execution adds overhead"
 *
 * Q: "How do you share data between ISR and main safely?"
 * A: "1. Volatile keyword for flags
 *    2. Disable interrupts for multi-byte access (brief!)
 *    3. Use atomic operations where available
 *    4. Ring buffer with separate read/write indices
 *    5. Double buffering (swap pointers)"
 *
 * ============================================================================
 *                    EDGE CASES
 * ============================================================================
 *
 * 1. NESTED INTERRUPTS:
 *    Higher priority ISR interrupts lower priority
 *    Solution: Keep all ISRs short!
 *
 * 2. RE-ENTRANCY:
 *    ISR fires again before previous completes
 *    Solution: Clear flag early, use FIFOs
 *
 * 3. RACE CONDITIONS:
 *    Main loop and ISR both access same data
 *    Solution: Disable interrupts briefly, or use atomics
 *
 * 4. STACK OVERFLOW:
 *    Deep ISR nesting exhausts stack
 *    Solution: Limit nesting levels, monitor stack usage
 *
 * 5. WATCHDOG TIMEOUT:
 *    Long ISR prevents watchdog feed
 *    Solution: Feed watchdog in ISR if necessary (carefully!)
 *
 * ============================================================================
 */
