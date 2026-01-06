/*
 * ============================================================================
 * QUESTION 25: How does Power Management work in RTOS?
 * ============================================================================
 *
 * DIFFICULTY: Medium-Hard | FREQUENCY: High | IMPORTANCE: Very High
 *
 * Power management is critical for battery-powered embedded devices.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "RTOS power management centers on minimizing CPU activity during idle periods.
 * The key technique is TICKLESS IDLE - instead of waking on every tick, the
 * RTOS calculates when the next task needs to run and sleeps until then,
 * reducing wake-ups from 1000/sec to just a few. When all tasks are blocked,
 * the idle task runs and can enter low-power modes (Sleep, Stop, Standby).
 * The deeper the sleep, the more power saved but longer wake-up time.
 * FreeRTOS uses configUSE_TICKLESS_IDLE, Zephyr has CONFIG_PM. Additional
 * savings come from disabling unused peripherals and using DMA."
 *
 * ============================================================================
 * VISUALIZATION: Power Modes
 * ============================================================================
 *
 *   POWER CONSUMPTION vs WAKE-UP TIME TRADEOFF:
 *
 *   Power
 *     ^
 *     |
 *   High|  [RUN] ████████████████
 *     |
 *     |  [SLEEP] ████████
 *     |
 *   Med |  [STOP] ████
 *     |
 *     |  [STANDBY] ██
 *   Low |
 *     +---------------------------------> Wake-up Time
 *        0    1us   10us  100us  1ms  10ms
 *
 *
 *   STM32 POWER MODES (Example):
 *
 *   +------------+----------+---------------+------------------+
 *   | Mode       | Current  | Wake Sources  | Wake Time        |
 *   +------------+----------+---------------+------------------+
 *   | Run        | 10-50mA  | Always on     | N/A              |
 *   | Sleep      | 1-5mA    | Any interrupt | ~1us             |
 *   | Stop       | 10-100uA | RTC, GPIO     | ~5us             |
 *   | Standby    | 1-5uA    | RTC, WKUP pin | ~50us + reinit   |
 *   +------------+----------+---------------+------------------+
 *
 * ============================================================================
 * TICKLESS IDLE EXPLAINED
 * ============================================================================
 *
 *   NORMAL (Tick every 1ms):
 *   -------------------------
 *
 *   Time:    0    1    2    3    4    5    6    7    8    9   10
 *            |    |    |    |    |    |    |    |    |    |    |
 *   CPU:     ↑    ↑    ↑    ↑    ↑    ↑    ↑    ↑    ↑    ↑    ↑
 *            |    |    |    |    |    |    |    |    |    |    |
 *           Wake every tick, even if nothing to do!
 *
 *   Power wasted on 10 unnecessary wake-ups!
 *
 *
 *   TICKLESS IDLE (Wake only when needed):
 *   --------------------------------------
 *
 *   Task needs to wake at t=10
 *
 *   Time:    0                             10
 *            |                              |
 *   CPU:     ↑                              ↑
 *            |<---- Deep Sleep ------------>|
 *            |      (Timer set for 10ms)    |
 *
 *   One wake-up instead of 10! 90% power saved.
 *
 *
 *   HOW IT WORKS:
 *
 *   1. Scheduler finds no ready tasks
 *   2. Calculate next wake time (min of all blocked task timeouts)
 *   3. Program low-power timer for that duration
 *   4. Enter low-power mode
 *   5. Wake on timer OR external interrupt
 *   6. Compensate tick count for time slept
 *
 * ============================================================================
 * CODE EXAMPLE: FreeRTOS Tickless Idle
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
// FreeRTOSConfig.h settings for tickless idle:

#define configUSE_TICKLESS_IDLE          2    // 1=default, 2=user-defined
#define configEXPECTED_IDLE_TIME_BEFORE_SLEEP  2  // Min ticks before sleeping

// If configUSE_TICKLESS_IDLE = 2, implement these:

// Called before entering sleep
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime) {
    // Calculate how long to sleep (in low-power timer units)
    uint32_t sleep_ticks = xExpectedIdleTime;

    // Enter critical section - no interrupts during setup
    __disable_irq();

    // Check if we should still sleep (might have been woken already)
    if (eTaskConfirmSleepModeStatus() == eAbortSleep) {
        __enable_irq();
        return;
    }

    // Configure low-power timer to wake us
    configure_lptimer(sleep_ticks);

    // Enter low-power mode
    // This returns when timer expires or interrupt occurs
    __WFI();  // Wait For Interrupt

    // Read how long we actually slept
    uint32_t actual_sleep = get_lptimer_count();

    // Restart normal tick timer
    restart_systick();

    // Compensate tick count
    vTaskStepTick(actual_sleep);

    __enable_irq();
}

// Optional: Pre-sleep hook
void configPRE_SLEEP_PROCESSING(TickType_t xExpectedIdleTime) {
    // Disable peripherals to save power
    disable_unused_peripherals();

    // Reduce clock speed if possible
    reduce_system_clock();
}

// Optional: Post-sleep hook
void configPOST_SLEEP_PROCESSING(TickType_t xExpectedIdleTime) {
    // Restore clock speed
    restore_system_clock();

    // Re-enable peripherals
    enable_peripherals();
}
*/

/*
 * ============================================================================
 * ZEPHYR POWER MANAGEMENT
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>
#include <zephyr/pm/pm.h>
#include <zephyr/pm/device.h>

// ============================================================================
// System Power Management
// ============================================================================

// Zephyr automatically manages system power states.
// Configure in prj.conf:
//
// CONFIG_PM=y
// CONFIG_PM_DEVICE=y

// Register custom power state transitions
static int my_pm_policy(enum pm_state state) {
    switch (state) {
        case PM_STATE_RUNTIME_IDLE:
            // Light sleep - fast wake
            return 0;

        case PM_STATE_SUSPEND_TO_IDLE:
            // Deeper sleep - slower wake
            if (can_suspend()) {
                return 0;  // Allow
            }
            return -EBUSY;  // Disallow

        case PM_STATE_SUSPEND_TO_RAM:
            // Deepest sleep - slowest wake
            return -ENOTSUP;  // Not supported

        default:
            return -ENOTSUP;
    }
}


// ============================================================================
// Device Power Management
// ============================================================================

// Define device with PM support
#define MY_DEVICE DT_NODELABEL(my_device)

static int my_device_pm_action(const struct device *dev,
                                enum pm_device_action action) {
    switch (action) {
        case PM_DEVICE_ACTION_SUSPEND:
            // Save device state, disable clocks
            my_device_save_state(dev);
            my_device_clock_disable();
            return 0;

        case PM_DEVICE_ACTION_RESUME:
            // Restore clocks, restore state
            my_device_clock_enable();
            my_device_restore_state(dev);
            return 0;

        default:
            return -ENOTSUP;
    }
}

PM_DEVICE_DT_DEFINE(MY_DEVICE, my_device_pm_action);


// ============================================================================
// Manual Power State Control
// ============================================================================

void enter_low_power(void) {
    // Force specific power state
    pm_state_force(0, &(struct pm_state_info){PM_STATE_SUSPEND_TO_IDLE, 0, 0});
}

void prevent_sleep(void) {
    // Lock out deep sleep (e.g., during critical operation)
    pm_policy_state_lock_get(PM_STATE_SUSPEND_TO_RAM, PM_ALL_SUBSTATES);

    do_critical_operation();

    pm_policy_state_lock_put(PM_STATE_SUSPEND_TO_RAM, PM_ALL_SUBSTATES);
}
*/

/*
 * ============================================================================
 * POWER MANAGEMENT STRATEGIES
 * ============================================================================
 *
 * 1. TICKLESS IDLE
 *    - Don't wake on every tick
 *    - Sleep until next task needs to run
 *    - Huge savings for mostly-idle systems
 *
 * 2. CLOCK GATING
 *    - Disable clocks to unused peripherals
 *    - Each peripheral has clock enable bit
 *    - Very fast to enable/disable
 *
 * 3. VOLTAGE SCALING
 *    - Lower voltage = lower power (P = V²/R)
 *    - Reduce core voltage at lower speeds
 *    - Some MCUs do this automatically (DVFS)
 *
 * 4. CLOCK SCALING
 *    - Lower clock = lower power
 *    - Run at minimum speed needed for task
 *    - Can change dynamically
 *
 * 5. DMA USAGE
 *    - DMA transfers while CPU sleeps
 *    - Wake only when transfer complete
 *    - Excellent for ADC, UART, SPI
 *
 * 6. PERIPHERAL MANAGEMENT
 *    - Power down unused peripherals completely
 *    - Wake on interrupt from needed peripheral
 *
 * ============================================================================
 * CODE: Clock and Peripheral Management
 * ============================================================================
 */

/*
// STM32 Example: Clock gating

// Disable USART2 clock (save power when not using)
RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;

// Enable when needed
RCC->APB1ENR |= RCC_APB1ENR_USART2EN;


// Reduce system clock
void reduce_clock_speed(void) {
    // Switch from 72MHz to 8MHz HSI
    // Saves ~80% dynamic power!

    // Wait for HSI ready
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    // Switch to HSI
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);

    // Disable PLL to save more power
    RCC->CR &= ~RCC_CR_PLLON;

    // Disable HSE if not needed
    RCC->CR &= ~RCC_CR_HSEON;

    // Update SystemCoreClock variable
    SystemCoreClockUpdate();
}


// DMA-based ADC with sleep
void start_adc_dma_and_sleep(uint16_t *buffer, uint32_t count) {
    // Configure ADC with DMA
    adc_dma_configure(buffer, count);

    // Start conversion
    adc_start();

    // CPU can sleep now - DMA handles data
    // Wake when DMA complete interrupt fires
}

void DMA1_Channel1_IRQHandler(void) {
    if (DMA1->ISR & DMA_ISR_TCIF1) {
        DMA1->IFCR = DMA_IFCR_CTCIF1;
        // ADC buffer is full - process it
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(adc_complete_sem, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
*/

/*
 * ============================================================================
 * WAKE SOURCES
 * ============================================================================
 *
 *   +------------------+-------------------+----------------------+
 *   | Sleep Mode       | Wake Sources      | Notes                |
 *   +------------------+-------------------+----------------------+
 *   | Sleep            | Any interrupt     | Fast wake            |
 *   +------------------+-------------------+----------------------+
 *   | Stop             | EXTI (GPIO)       | Most peripherals off |
 *   |                  | RTC               |                      |
 *   |                  | LPUART            |                      |
 *   |                  | I2C/SPI (address) |                      |
 *   +------------------+-------------------+----------------------+
 *   | Standby          | WKUP pins         | RAM lost (usually)   |
 *   |                  | RTC               | Reinit required      |
 *   +------------------+-------------------+----------------------+
 *
 *
 *   WAKE-UP SOURCES DIAGRAM:
 *
 *                    +-------------+
 *   GPIO Pin ------->|             |
 *   RTC Alarm ------>| Power       |-----> Wake CPU
 *   UART Activity -->| Controller  |
 *   Watchdog ------->|             |
 *                    +-------------+
 *
 * ============================================================================
 * POWER BUDGET EXAMPLE
 * ============================================================================
 *
 *   SENSOR NODE - Battery Life Calculation:
 *
 *   Operation                 Duration    Current    Charge
 *   ---------------------------------------------------------
 *   Sleep (95% of time)      950ms       10uA       9.5uAs
 *   Wake + Sample            40ms        5mA        200uAs
 *   Transmit                 10ms        20mA       200uAs
 *   ---------------------------------------------------------
 *   Total per second:                               409.5uAs
 *
 *   Average current: 409.5uAs/1s = 409.5uA ≈ 0.41mA
 *
 *   Battery: 1000mAh CR2032
 *   Lifetime: 1000mAh / 0.41mA = 2439 hours ≈ 100 days
 *
 *
 *   WITHOUT TICKLESS (1000Hz tick):
 *
 *   1000 wake-ups/sec × 50uA per wake = 50mA overhead!
 *   Battery life: ~20 hours  (5x worse!)
 *
 * ============================================================================
 * BEST PRACTICES
 * ============================================================================
 *
 * 1. DESIGN FOR SLEEP
 *    - Default state should be sleeping
 *    - Wake only when work is needed
 *
 * 2. MINIMIZE WAKE FREQUENCY
 *    - Batch operations when possible
 *    - Use longer sleep periods
 *
 * 3. FAST WAKE-PROCESS-SLEEP
 *    - Minimize time in active state
 *    - Pre-calculate values when possible
 *
 * 4. USE HARDWARE WAKE SOURCES
 *    - Configure GPIO/RTC to wake from deep sleep
 *    - Don't poll - use interrupts
 *
 * 5. DISABLE UNUSED PERIPHERALS
 *    - Clock gate unused peripherals
 *    - Power down completely if possible
 *
 * 6. MONITOR POWER CONSUMPTION
 *    - Measure actual current draw
 *    - Identify unexpected wake-ups
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN TICKLESS
 *    "Don't wake every tick, sleep until next task needs to run"
 *
 * 2. MENTION WAKE SOURCES
 *    "RTC, GPIO interrupt, peripheral events can wake from deep sleep"
 *
 * 3. DISCUSS TRADEOFFS
 *    "Deeper sleep = more power saved but longer wake-up time"
 *
 * 4. GIVE REAL NUMBERS
 *    "Sleep mode: 10uA, Run mode: 10mA - 1000x difference!"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "How do you measure power consumption?"
 * A: Use current probe on power supply, or specialized power monitor IC.
 *    Need fast sampling to catch short wake-ups. Some MCUs have internal
 *    measurement capability.
 *
 * Q: "What's the difference between WFI and WFE on ARM?"
 * A: WFI (Wait For Interrupt) wakes on any interrupt.
 *    WFE (Wait For Event) wakes on event flag, can be set by SEV instruction.
 *    WFE useful for spinlock-like waiting in SMP.
 *
 * Q: "Can RTOS tasks prevent sleep?"
 * A: Yes. If any task is Ready (not Blocked), RTOS won't enter idle.
 *    Also, APIs like pm_policy_state_lock_get() in Zephyr explicitly
 *    prevent certain sleep states.
 *
 * Q: "What about RAM retention in deep sleep?"
 * A: Depends on mode. Stop mode usually retains RAM. Standby may not.
 *    Some MCUs have backup RAM that's always retained.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 25 ===\n\n");
    printf("Q: How does Power Management work in RTOS?\n\n");
    printf("KEY CONCEPTS:\n\n");
    printf("1. TICKLESS IDLE:\n");
    printf("   - Don't wake every tick\n");
    printf("   - Sleep until next task needs CPU\n");
    printf("   - Reduces wakeups from 1000/sec to few/sec\n\n");
    printf("2. SLEEP MODES:\n");
    printf("   - Sleep: Fast wake, moderate savings\n");
    printf("   - Stop: Slow wake, good savings\n");
    printf("   - Standby: Slowest wake, best savings\n\n");
    printf("3. ADDITIONAL STRATEGIES:\n");
    printf("   - Clock gating unused peripherals\n");
    printf("   - Voltage/frequency scaling\n");
    printf("   - DMA for background transfers\n\n");
    printf("CONFIGURATION:\n");
    printf("- FreeRTOS: configUSE_TICKLESS_IDLE\n");
    printf("- Zephyr: CONFIG_PM, CONFIG_PM_DEVICE\n\n");
    printf("TYPICAL SAVINGS:\n");
    printf("- Run: 10-50mA\n");
    printf("- Sleep: 1-5mA\n");
    printf("- Stop: 10-100uA\n");
    printf("- Standby: 1-5uA\n");
    return 0;
}
