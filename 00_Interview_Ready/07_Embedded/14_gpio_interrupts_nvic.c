/*
 * ============================================================================
 *                    GPIO & INTERRUPTS (NVIC) GUIDE
 * ============================================================================
 *
 * CRITICAL FOR IRONSITE:
 *   - Button handling (record start/stop)
 *   - Camera VSYNC/HSYNC interrupts
 *   - Sensor data ready signals
 *   - Wireless module interrupts
 *   - Multiple interrupt priority management
 *
 * ============================================================================
 *                    GPIO BASICS
 * ============================================================================
 *
 * GPIO = General Purpose Input/Output
 *
 * PIN MODES:
 *
 *   INPUT:
 *     - Floating (Hi-Z): No pull, reads external level
 *     - Pull-up: Internal resistor to VCC
 *     - Pull-down: Internal resistor to GND
 *
 *   OUTPUT:
 *     - Push-pull: Drives HIGH or LOW actively
 *     - Open-drain: Only pulls LOW, needs external pull-up
 *
 *   ALTERNATE FUNCTION:
 *     - Pin controlled by peripheral (UART, SPI, I2C, etc.)
 *
 *   ANALOG:
 *     - For ADC/DAC, digital buffers disabled
 *
 *
 * PUSH-PULL vs OPEN-DRAIN:
 *
 *   PUSH-PULL:                    OPEN-DRAIN:
 *
 *     VCC                           VCC
 *      |                             |
 *     [P]  <- ON when output=1      [R] External pull-up
 *      |                             |
 *   ---+--- Pin                   ---+--- Pin
 *      |                             |
 *     [N]  <- ON when output=0      [N] <- ON when output=0
 *      |                             |
 *     GND                           GND
 *
 *   Use Push-Pull: Normal outputs (LEDs, chip selects)
 *   Use Open-Drain: I2C, level shifting, wire-OR buses
 *
 * ============================================================================
 *                    GPIO CONFIGURATION (STM32 Example)
 * ============================================================================
 *
 * REGISTERS:
 *
 *   MODER:   Mode (input/output/AF/analog)
 *   OTYPER:  Output type (push-pull/open-drain)
 *   OSPEEDR: Output speed (low/medium/high/very high)
 *   PUPDR:   Pull-up/pull-down
 *   IDR:     Input data register (read pin state)
 *   ODR:     Output data register (write pin state)
 *   BSRR:    Bit set/reset register (atomic set/clear)
 *   AFRL/H:  Alternate function selection
 *
 * COMMON OPERATIONS:
 *
 *   // Set pin 5 high
 *   GPIOA->ODR |= (1 << 5);     // Read-modify-write (not atomic)
 *   GPIOA->BSRR = (1 << 5);     // Atomic set (preferred)
 *
 *   // Set pin 5 low
 *   GPIOA->ODR &= ~(1 << 5);    // Read-modify-write
 *   GPIOA->BSRR = (1 << 21);    // Atomic reset (bit 5 + 16)
 *
 *   // Read pin 5
 *   if (GPIOA->IDR & (1 << 5)) { ... }
 *
 *   // Toggle pin 5
 *   GPIOA->ODR ^= (1 << 5);
 *
 * ============================================================================
 *                    INTERRUPT BASICS
 * ============================================================================
 *
 * WHAT IS AN INTERRUPT?
 *   - Hardware signal that stops normal execution
 *   - CPU saves state, jumps to Interrupt Service Routine (ISR)
 *   - After ISR, returns to interrupted code
 *
 * WHY USE INTERRUPTS?
 *   - React immediately to events (don't miss button press)
 *   - No polling overhead (CPU can sleep until interrupt)
 *   - Handle time-critical events (UART byte received)
 *
 * INTERRUPT FLOW:
 *
 *   1. Event occurs (button press, timer, UART byte)
 *   2. Peripheral sets interrupt pending flag
 *   3. If enabled, NVIC sees the request
 *   4. CPU finishes current instruction
 *   5. CPU saves context (registers) to stack
 *   6. CPU loads ISR address from vector table
 *   7. ISR executes
 *   8. ISR clears pending flag (IMPORTANT!)
 *   9. CPU restores context from stack
 *   10. Normal execution resumes
 *
 * ============================================================================
 *                    NVIC (Nested Vectored Interrupt Controller)
 * ============================================================================
 *
 * NVIC FEATURES:
 *   - Manages all interrupts on Cortex-M
 *   - Priority-based preemption
 *   - Tail-chaining (fast back-to-back interrupts)
 *   - Vector table for ISR addresses
 *
 * INTERRUPT PRIORITY:
 *   - Lower number = higher priority
 *   - Priority 0 is highest (most urgent)
 *   - Configurable bits (usually 4 bits = 16 levels)
 *
 * PREEMPTION:
 *   - Higher priority interrupt can interrupt lower priority ISR
 *   - Called "nesting"
 *
 *   Main code running...
 *        |
 *        v
 *   [IRQ Priority 5] <--- Interrupt!
 *        |
 *        v
 *   [IRQ Priority 2] <--- Higher priority interrupts!
 *        |
 *        v (Priority 2 ISR completes)
 *   [Back to Priority 5 ISR]
 *        |
 *        v (Priority 5 ISR completes)
 *   [Back to Main code]
 *
 *
 * PRIORITY GROUPING:
 *   - Split priority bits into: Preemption priority + Sub-priority
 *   - Preemption: Can interrupt lower preemption priority
 *   - Sub-priority: Tie-breaker when same preemption priority
 *
 *   Example: 4 bits total
 *     Group 0: 0 preemption bits, 4 sub-priority bits
 *     Group 1: 1 preemption bit, 3 sub-priority bits
 *     Group 4: 4 preemption bits, 0 sub-priority bits
 *
 * ============================================================================
 *                    EXTI (External Interrupts)
 * ============================================================================
 *
 * GPIO pins can trigger interrupts via EXTI:
 *
 *   TRIGGER OPTIONS:
 *     - Rising edge (0 -> 1)
 *     - Falling edge (1 -> 0)
 *     - Both edges
 *
 *   CONFIGURATION:
 *     1. Configure GPIO as input
 *     2. Select EXTI line (each pin maps to one EXTI)
 *     3. Configure trigger edge
 *     4. Enable EXTI interrupt
 *     5. Enable NVIC for EXTI IRQ
 *     6. Write ISR
 *
 *   NOTE: EXTI0 = pin 0 of any port, EXTI1 = pin 1, etc.
 *         Only ONE port's pin X can use EXTI X at a time!
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
//                    SIMULATED HARDWARE REGISTERS
// ============================================================================

typedef struct {
    volatile uint32_t MODER;    // Mode register
    volatile uint32_t OTYPER;   // Output type
    volatile uint32_t OSPEEDR;  // Output speed
    volatile uint32_t PUPDR;    // Pull-up/pull-down
    volatile uint32_t IDR;      // Input data
    volatile uint32_t ODR;      // Output data
    volatile uint32_t BSRR;     // Bit set/reset
    volatile uint32_t AFRL;     // Alternate function low
    volatile uint32_t AFRH;     // Alternate function high
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t IMR;      // Interrupt mask
    volatile uint32_t EMR;      // Event mask
    volatile uint32_t RTSR;     // Rising trigger selection
    volatile uint32_t FTSR;     // Falling trigger selection
    volatile uint32_t SWIER;    // Software interrupt event
    volatile uint32_t PR;       // Pending register
} EXTI_TypeDef;

typedef struct {
    volatile uint32_t ISER[8];  // Interrupt set-enable
    volatile uint32_t ICER[8];  // Interrupt clear-enable
    volatile uint32_t ISPR[8];  // Interrupt set-pending
    volatile uint32_t ICPR[8];  // Interrupt clear-pending
    volatile uint32_t IABR[8];  // Interrupt active bit
    volatile uint32_t IP[240];  // Interrupt priority
} NVIC_TypeDef;

// Simulated instances
static GPIO_TypeDef gpioa = {0};
static GPIO_TypeDef gpiob = {0};
static EXTI_TypeDef exti = {0};
static NVIC_TypeDef nvic = {0};

#define GPIOA (&gpioa)
#define GPIOB (&gpiob)
#define EXTI  (&exti)
#define NVIC  (&nvic)

// ============================================================================
//                    GPIO CONFIGURATION FUNCTIONS
// ============================================================================

typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_AF = 2,
    GPIO_MODE_ANALOG = 3
} GPIO_Mode;

typedef enum {
    GPIO_OTYPE_PUSHPULL = 0,
    GPIO_OTYPE_OPENDRAIN = 1
} GPIO_OType;

typedef enum {
    GPIO_PUPD_NONE = 0,
    GPIO_PUPD_PULLUP = 1,
    GPIO_PUPD_PULLDOWN = 2
} GPIO_PuPd;

typedef enum {
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MEDIUM = 1,
    GPIO_SPEED_HIGH = 2,
    GPIO_SPEED_VERY_HIGH = 3
} GPIO_Speed;

void gpio_set_mode(GPIO_TypeDef* gpio, uint8_t pin, GPIO_Mode mode) {
    gpio->MODER &= ~(3 << (pin * 2));
    gpio->MODER |= (mode << (pin * 2));
    printf("[GPIO] Pin %d mode set to %d\n", pin, mode);
}

void gpio_set_output_type(GPIO_TypeDef* gpio, uint8_t pin, GPIO_OType type) {
    if (type == GPIO_OTYPE_PUSHPULL) {
        gpio->OTYPER &= ~(1 << pin);
    } else {
        gpio->OTYPER |= (1 << pin);
    }
}

void gpio_set_pupd(GPIO_TypeDef* gpio, uint8_t pin, GPIO_PuPd pupd) {
    gpio->PUPDR &= ~(3 << (pin * 2));
    gpio->PUPDR |= (pupd << (pin * 2));
}

void gpio_write(GPIO_TypeDef* gpio, uint8_t pin, bool value) {
    if (value) {
        gpio->BSRR = (1 << pin);        // Set
    } else {
        gpio->BSRR = (1 << (pin + 16)); // Reset
    }
    printf("[GPIO] Pin %d = %d\n", pin, value);
}

bool gpio_read(GPIO_TypeDef* gpio, uint8_t pin) {
    return (gpio->IDR >> pin) & 1;
}

void gpio_toggle(GPIO_TypeDef* gpio, uint8_t pin) {
    gpio->ODR ^= (1 << pin);
}

// ============================================================================
//                    EXTI CONFIGURATION
// ============================================================================

typedef enum {
    EXTI_TRIGGER_RISING,
    EXTI_TRIGGER_FALLING,
    EXTI_TRIGGER_BOTH
} EXTI_Trigger;

void exti_configure(uint8_t line, EXTI_Trigger trigger) {
    // Clear existing trigger settings
    EXTI->RTSR &= ~(1 << line);
    EXTI->FTSR &= ~(1 << line);

    // Set new trigger
    switch (trigger) {
        case EXTI_TRIGGER_RISING:
            EXTI->RTSR |= (1 << line);
            break;
        case EXTI_TRIGGER_FALLING:
            EXTI->FTSR |= (1 << line);
            break;
        case EXTI_TRIGGER_BOTH:
            EXTI->RTSR |= (1 << line);
            EXTI->FTSR |= (1 << line);
            break;
    }

    printf("[EXTI] Line %d trigger configured\n", line);
}

void exti_enable(uint8_t line) {
    EXTI->IMR |= (1 << line);
    printf("[EXTI] Line %d enabled\n", line);
}

void exti_disable(uint8_t line) {
    EXTI->IMR &= ~(1 << line);
}

void exti_clear_pending(uint8_t line) {
    EXTI->PR = (1 << line);  // Write 1 to clear
}

bool exti_is_pending(uint8_t line) {
    return (EXTI->PR >> line) & 1;
}

// ============================================================================
//                    NVIC FUNCTIONS
// ============================================================================

void nvic_enable_irq(uint8_t irq) {
    NVIC->ISER[irq / 32] = (1 << (irq % 32));
    printf("[NVIC] IRQ %d enabled\n", irq);
}

void nvic_disable_irq(uint8_t irq) {
    NVIC->ICER[irq / 32] = (1 << (irq % 32));
}

void nvic_set_priority(uint8_t irq, uint8_t priority) {
    NVIC->IP[irq] = priority << 4;  // Upper 4 bits used
    printf("[NVIC] IRQ %d priority set to %d\n", irq, priority);
}

void nvic_set_pending(uint8_t irq) {
    NVIC->ISPR[irq / 32] = (1 << (irq % 32));
}

void nvic_clear_pending(uint8_t irq) {
    NVIC->ICPR[irq / 32] = (1 << (irq % 32));
}

// ============================================================================
//                    EXAMPLE: BUTTON INTERRUPT
// ============================================================================

// IRQ numbers (STM32F4 example)
#define EXTI0_IRQn     6
#define EXTI1_IRQn     7
#define EXTI2_IRQn     8
#define EXTI3_IRQn     9
#define EXTI4_IRQn     10
#define EXTI9_5_IRQn   23
#define EXTI15_10_IRQn 40

#define BUTTON_PIN  0
#define LED_PIN     5

volatile uint32_t button_press_count = 0;

void button_init(void) {
    printf("\n=== Button Interrupt Setup ===\n");

    // 1. Configure button pin as input with pull-up
    gpio_set_mode(GPIOA, BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pupd(GPIOA, BUTTON_PIN, GPIO_PUPD_PULLUP);

    // 2. Configure EXTI for falling edge (button press pulls low)
    exti_configure(BUTTON_PIN, EXTI_TRIGGER_FALLING);
    exti_enable(BUTTON_PIN);

    // 3. Configure NVIC
    nvic_set_priority(EXTI0_IRQn, 5);  // Medium priority
    nvic_enable_irq(EXTI0_IRQn);

    // 4. Configure LED as output
    gpio_set_mode(GPIOA, LED_PIN, GPIO_MODE_OUTPUT);

    printf("Button interrupt configured on PA0\n");
}

// ISR for button (EXTI0)
void EXTI0_IRQHandler(void) {
    printf("\n[ISR] EXTI0 - Button pressed!\n");

    // CRITICAL: Clear pending flag first!
    exti_clear_pending(BUTTON_PIN);

    // Toggle LED
    gpio_toggle(GPIOA, LED_PIN);
    button_press_count++;

    printf("[ISR] Press count: %lu\n", (unsigned long)button_press_count);
}

// ============================================================================
//                    EXAMPLE: MULTIPLE INTERRUPT PRIORITIES
// ============================================================================

/*
 * IRONSITE INTERRUPT PRIORITIES:
 *
 *   Priority 0: Fault handlers (HardFault, etc.) - HIGHEST
 *   Priority 1: System tick (FreeRTOS)
 *   Priority 2: Camera VSYNC (frame timing critical)
 *   Priority 3: DMA complete (data transfer)
 *   Priority 4: IMU data ready
 *   Priority 5: Button press
 *   Priority 6: UART RX
 *   Priority 7: Timer for periodic tasks - LOWEST
 *
 */

#define CAMERA_VSYNC_IRQn  10
#define DMA_COMPLETE_IRQn  11
#define IMU_DRDY_IRQn      23
#define UART_RX_IRQn       37

void configure_interrupt_priorities(void) {
    printf("\n=== Configuring Interrupt Priorities ===\n");

    // Camera VSYNC - highest priority (can't miss frame sync)
    nvic_set_priority(CAMERA_VSYNC_IRQn, 2);

    // DMA complete - high priority
    nvic_set_priority(DMA_COMPLETE_IRQn, 3);

    // IMU data ready - medium-high
    nvic_set_priority(IMU_DRDY_IRQn, 4);

    // Button - medium
    nvic_set_priority(EXTI0_IRQn, 5);

    // UART - lower
    nvic_set_priority(UART_RX_IRQn, 6);

    printf("\nPriority order (lower number = higher priority):\n");
    printf("  Camera VSYNC: 2\n");
    printf("  DMA Complete: 3\n");
    printf("  IMU Data Ready: 4\n");
    printf("  Button: 5\n");
    printf("  UART RX: 6\n");
}

// ============================================================================
//                    COMMON PATTERNS
// ============================================================================

/*
 * PATTERN 1: Debounced button with timer
 *
 *   volatile bool button_pressed = false;
 *   volatile uint32_t last_press_time = 0;
 *   #define DEBOUNCE_MS 50
 *
 *   void EXTI0_IRQHandler(void) {
 *       exti_clear_pending(0);
 *       uint32_t now = get_tick_count();
 *       if (now - last_press_time > DEBOUNCE_MS) {
 *           button_pressed = true;
 *           last_press_time = now;
 *       }
 *   }
 */

/*
 * PATTERN 2: ISR sets flag, main loop processes
 *
 *   volatile bool data_ready = false;
 *
 *   void SENSOR_IRQHandler(void) {
 *       clear_irq_pending();
 *       data_ready = true;  // Just set flag, exit fast
 *   }
 *
 *   void main(void) {
 *       while(1) {
 *           if (data_ready) {
 *               data_ready = false;
 *               process_sensor_data();  // Heavy work in main
 *           }
 *       }
 *   }
 */

/*
 * PATTERN 3: Critical section (disable interrupts)
 *
 *   void modify_shared_data(void) {
 *       __disable_irq();  // Disable all interrupts
 *       // Modify shared variable
 *       shared_counter++;
 *       __enable_irq();   // Re-enable interrupts
 *   }
 *
 *   // Better: disable specific interrupt
 *   nvic_disable_irq(UART_IRQn);
 *   // Modify data
 *   nvic_enable_irq(UART_IRQn);
 */

// ============================================================================
//                    DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   GPIO & INTERRUPTS (NVIC) DEMO\n");
    printf("============================================\n");

    // Setup button with interrupt
    button_init();

    // Configure multiple priorities
    configure_interrupt_priorities();

    // Simulate button press
    printf("\n=== Simulating Button Press ===\n");
    EXTI->PR |= (1 << BUTTON_PIN);  // Set pending
    EXTI0_IRQHandler();

    // Simulate another press
    EXTI->PR |= (1 << BUTTON_PIN);
    EXTI0_IRQHandler();

    printf("\n============================================\n");
    printf("   GPIO/INTERRUPT KEY POINTS\n");
    printf("============================================\n");
    printf("- GPIO modes: Input, Output, AF, Analog\n");
    printf("- Push-pull drives both high and low\n");
    printf("- Open-drain needs external pull-up\n");
    printf("- EXTI: external interrupt from GPIO pins\n");
    printf("- NVIC: manages interrupt priorities\n");
    printf("- Lower priority NUMBER = HIGHER priority\n");
    printf("- ALWAYS clear pending flag in ISR!\n");
    printf("- Keep ISRs short - set flag, exit\n");

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW Q&A
 * ============================================================================
 *
 * Q: What's the difference between push-pull and open-drain?
 * A: "Push-pull can drive both high and low actively. Open-drain can only
 *    pull low - you need an external pull-up for high. Use open-drain for
 *    I2C, level shifting between different voltages, or when multiple
 *    devices share a line (wire-OR)."
 *
 * Q: Explain NVIC interrupt priority.
 * A: "NVIC manages interrupts on Cortex-M. Lower priority number means
 *    higher urgency. A priority 2 interrupt can preempt (interrupt) a
 *    priority 5 ISR that's running. There's also sub-priority for
 *    tie-breaking when same preemption level."
 *
 * Q: What happens if you forget to clear the pending flag in ISR?
 * A: "The ISR will immediately re-enter after returning! The interrupt
 *    stays pending, so CPU finishes ISR, sees pending, enters again.
 *    System appears hung. Always clear pending flag early in ISR."
 *
 * Q: How do you handle a button in firmware?
 * A: "Configure GPIO as input with pull-up (so button pulls low). Use
 *    EXTI for falling edge interrupt. In ISR, clear pending flag and
 *    set a flag or signal semaphore. Handle debouncing with timer or
 *    by checking time since last press."
 *
 * Q: How do you decide interrupt priorities?
 * A: "Based on latency requirements. Time-critical things like camera
 *    sync get highest priority. Things that can wait, like UART, get
 *    lower. Also consider ISR duration - a long high-priority ISR
 *    blocks everything else."
 *
 * Q: What's a critical section and when do you need it?
 * A: "Code that accesses shared data and must not be interrupted.
 *    Disable interrupts before, enable after. Keep it SHORT - disabling
 *    interrupts increases latency for everything. Better to use
 *    atomic operations or disable only specific interrupts."
 *
 * ============================================================================
 */
