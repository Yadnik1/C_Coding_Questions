/*
 * ============================================================================
 * QUESTION 33: How do Interrupt Priorities work with RTOS?
 * ============================================================================
 *
 * DIFFICULTY: Hard | FREQUENCY: High | IMPORTANCE: Critical
 *
 * Understanding interrupt priorities is essential for correct RTOS behavior.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Interrupt priorities are SEPARATE from task priorities - all ISRs preempt
 * all tasks! ARM Cortex-M has configurable priority levels (0 = highest).
 * FreeRTOS has a key concept: configMAX_SYSCALL_INTERRUPT_PRIORITY defines
 * which ISRs can call FreeRTOS APIs. ISRs at or below this priority can use
 * FromISR functions. ISRs ABOVE (lower number) cannot call ANY FreeRTOS API
 * but have lower latency. This split allows critical ISRs to have guaranteed
 * low latency while others can integrate with the RTOS."
 *
 * ============================================================================
 * VISUALIZATION: Priority Levels
 * ============================================================================
 *
 *   ARM CORTEX-M PRIORITY MODEL:
 *
 *   Priority   Type              Can use FreeRTOS?
 *   --------   ----              -----------------
 *   0 (high)   IRQ_A             NO! (above SYSCALL)
 *   1          IRQ_B             NO!
 *   2          IRQ_C             NO!
 *   3          configMAX_SYSCALL <-- Threshold
 *   4          UART_IRQ          YES (FromISR APIs)
 *   5          Timer_IRQ         YES
 *   6          SPI_IRQ           YES
 *   ...
 *   15 (low)   PendSV/SysTick    YES (used by FreeRTOS kernel)
 *
 *   IMPORTANT: Lower number = Higher priority on ARM!
 *
 *
 *   EXECUTION PREEMPTION:
 *
 *   Priority:   0 |████|                     Highest (cannot use RTOS)
 *               1 |████████|
 *               3 |████████████|             configMAX_SYSCALL
 *               5 |████████████████|         Can use FromISR
 *              15 |████████████████████|     SysTick (lowest ISR)
 *   ---------------------------------------------------------------
 *   Task High     |████████████████████████| Highest task
 *   Task Low      |████████████████████████████████████| Lowest task
 *
 *   ANY ISR preempts ANY task!
 *
 * ============================================================================
 * CODE EXAMPLE: Correct Priority Configuration
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
// ============================================================================
// FreeRTOSConfig.h Settings
// ============================================================================

// ARM Cortex-M specific (check your MCU!)
// Most Cortex-M have 3-4 priority bits = 8-16 levels

// Priority bits used by hardware (STM32F4 uses 4 bits)
#define configPRIO_BITS                     4

// Lowest priority (highest number)
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   15

// Highest priority that can use FreeRTOS APIs
// ISRs 0-4 cannot call FreeRTOS, 5-15 can
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY  5

// Convert to hardware format (shift left for ARM)
#define configKERNEL_INTERRUPT_PRIORITY \
    (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))


// ============================================================================
// Correct ISR Configuration
// ============================================================================

// ISR that uses FreeRTOS APIs
void UART_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // This is OK - priority 6 is below configMAX_SYSCALL (5)
    xQueueSendFromISR(rx_queue, &data, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void setup_uart_irq(void) {
    // Priority 6 - can use FreeRTOS APIs
    NVIC_SetPriority(UART_IRQn, 6);
    NVIC_EnableIRQ(UART_IRQn);
}


// Critical ISR that cannot use FreeRTOS
void MOTOR_FAULT_IRQHandler(void) {
    // This ISR has priority 1 - above configMAX_SYSCALL
    // CANNOT call any FreeRTOS API!

    // Just handle the hardware directly
    MOTOR->DISABLE = 1;
    fault_flag = 1;  // Just set a flag - task will check it

    // NO xSemaphoreGiveFromISR here!
}

void setup_motor_fault_irq(void) {
    // Priority 1 - higher priority, but NO FreeRTOS APIs!
    NVIC_SetPriority(MOTOR_FAULT_IRQn, 1);
    NVIC_EnableIRQ(MOTOR_FAULT_IRQn);
}


// ============================================================================
// WRONG: FreeRTOS API from high-priority ISR
// ============================================================================

void WRONG_IRQHandler(void) {
    // CRASH or UNDEFINED BEHAVIOR!
    // This ISR is priority 2, above configMAX_SYSCALL (5)
    BaseType_t wake = pdFALSE;
    xSemaphoreGiveFromISR(sem, &wake);  // WRONG! Will corrupt RTOS!
}


// ============================================================================
// Runtime Priority Check (Debug Helper)
// ============================================================================

void verify_isr_priority(IRQn_Type irq, const char *name) {
    uint32_t priority = NVIC_GetPriority(irq);

    if (priority < configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY) {
        printf("WARNING: %s (prio %lu) cannot use FreeRTOS APIs!\n",
               name, priority);
    } else {
        printf("OK: %s (prio %lu) can use FreeRTOS FromISR APIs\n",
               name, priority);
    }
}

void check_all_irq_priorities(void) {
    verify_isr_priority(UART_IRQn, "UART");
    verify_isr_priority(SPI_IRQn, "SPI");
    verify_isr_priority(TIM2_IRQn, "TIM2");
    verify_isr_priority(MOTOR_FAULT_IRQn, "MOTOR_FAULT");
}
*/

/*
 * ============================================================================
 * ZEPHYR INTERRUPT PRIORITIES
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>
#include <zephyr/irq.h>

// Zephyr also has concept of "zero latency" interrupts
// These bypass the kernel and cannot use Zephyr APIs

// Normal ISR - can use Zephyr APIs
void uart_isr(const struct device *dev, void *user_data) {
    // Can use k_sem_give, k_msgq_put, etc.
    k_sem_give(&uart_sem);
}

// Configure ISR
IRQ_CONNECT(UART_IRQn, 5, uart_isr, NULL, 0);


// Zero-latency ISR - cannot use Zephyr APIs
// Enable with: CONFIG_ZERO_LATENCY_IRQS=y

void motor_fault_isr(void *arg) {
    // Cannot use ANY Zephyr API!
    // Just handle hardware directly
    MOTOR_DISABLE();
    fault_flag = 1;
}

// Configured with IRQ_ZERO_LATENCY flag
IRQ_CONNECT(MOTOR_FAULT_IRQn, 0, motor_fault_isr, NULL, IRQ_ZERO_LATENCY);
*/

/*
 * ============================================================================
 * ISR PRIORITY ASSIGNMENT STRATEGY
 * ============================================================================
 *
 *   Priority   Usage                           FreeRTOS APIs?
 *   --------   -----                           --------------
 *   0-2        Ultra-critical (motor fault,    NO
 *              safety shutdown)
 *
 *   3-4        Time-critical but needs RTOS    NO (borderline)
 *              (fast sensor sampling)
 *
 *   5          configMAX_SYSCALL threshold     YES
 *
 *   6-10       Normal peripheral ISRs          YES
 *              (UART, SPI, I2C, ADC)
 *
 *   11-14      Low-priority ISRs               YES
 *              (Logging, non-critical)
 *
 *   15         SysTick, PendSV                 Used by RTOS
 *              (NEVER change these!)
 *
 * ============================================================================
 * COMMON MISTAKES
 * ============================================================================
 *
 * MISTAKE 1: Using FreeRTOS API from high-priority ISR
 *
 *   void HIGH_PRIO_IRQ(void) {  // Priority 2
 *       xSemaphoreGiveFromISR(sem, &wake);  // CRASH!
 *   }
 *
 *   Fix: Either lower ISR priority to >= configMAX_SYSCALL,
 *        or don't use FreeRTOS APIs (just set a flag)
 *
 *
 * MISTAKE 2: Changing SysTick/PendSV priority
 *
 *   // NEVER do this!
 *   NVIC_SetPriority(SysTick_IRQn, 5);  // Breaks FreeRTOS!
 *
 *   SysTick and PendSV MUST be lowest priority for FreeRTOS
 *
 *
 * MISTAKE 3: Confusing ARM and FreeRTOS priority numbering
 *
 *   ARM: 0 = highest priority
 *   FreeRTOS tasks: 0 = lowest priority
 *
 *   They're completely separate!
 *
 *
 * MISTAKE 4: ISR priority same as configMAX_SYSCALL
 *
 *   // Priority exactly at threshold - behavior may vary
 *   NVIC_SetPriority(MY_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
 *
 *   Best practice: Set priority clearly below threshold (higher number)
 *
 * ============================================================================
 * DEBUGGING ISR PRIORITY ISSUES
 * ============================================================================
 *
 * Symptoms of wrong ISR priority:
 * 1. Random crashes in ISR
 * 2. System freeze/hang
 * 3. configASSERT failure in port code
 * 4. Data corruption
 *
 * Debug steps:
 * 1. Check NVIC priority of all ISRs
 * 2. Verify configMAX_SYSCALL setting
 * 3. Check which ISRs call FreeRTOS APIs
 * 4. Use configASSERT to catch API calls from wrong context
 */

/*
// FreeRTOS configASSERT catches wrong ISR priority
// In FreeRTOSConfig.h:
#define configASSERT(x) if(!(x)) { \
    taskDISABLE_INTERRUPTS(); \
    for(;;); \
}

// The port code checks:
// - That ISR calling FromISR has correct priority
// - That scheduler isn't called from wrong context
*/

/*
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN THE TWO PRIORITY SYSTEMS
 *    "ISR priorities are hardware (ARM NVIC), task priorities are software"
 *
 * 2. MENTION configMAX_SYSCALL
 *    "Threshold dividing ISRs that can/cannot use FreeRTOS APIs"
 *
 * 3. CLARIFY ARM NUMBERING
 *    "In ARM, lower number = higher priority (confusing!)"
 *
 * 4. DISCUSS USE CASES
 *    "Ultra-critical ISRs above threshold for guaranteed latency"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "Why can't high-priority ISRs use FreeRTOS APIs?"
 * A: FreeRTOS uses critical sections (disables interrupts up to
 *    configMAX_SYSCALL). Higher priority ISRs can preempt these sections,
 *    corrupting kernel data structures.
 *
 * Q: "What's the latency for an ISR above configMAX_SYSCALL?"
 * A: Just hardware latency (~12 cycles on Cortex-M). FreeRTOS never
 *    disables these interrupts, giving guaranteed low latency.
 *
 * Q: "Can a high-priority ISR signal a task indirectly?"
 * A: Yes, through a flag. High-prio ISR sets flag, lower-prio ISR
 *    periodically checks flag and calls FreeRTOS API.
 *
 * Q: "What happens if SysTick priority is too high?"
 * A: PendSV can't preempt, context switches don't work properly.
 *    SysTick/PendSV MUST be lowest priority.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 33 ===\n\n");
    printf("Q: How do Interrupt Priorities work with RTOS?\n\n");
    printf("KEY CONCEPTS:\n\n");
    printf("1. ISR vs TASK PRIORITIES:\n");
    printf("   - ISR priorities: Hardware (ARM NVIC)\n");
    printf("   - Task priorities: Software (RTOS scheduler)\n");
    printf("   - ANY ISR preempts ANY task!\n\n");
    printf("2. ARM PRIORITY NUMBERING:\n");
    printf("   - Lower number = Higher priority\n");
    printf("   - 0 = Highest, 15 = Lowest (typical)\n\n");
    printf("3. configMAX_SYSCALL_INTERRUPT_PRIORITY:\n");
    printf("   - Threshold for FreeRTOS API usage\n");
    printf("   - ISRs above (lower num): NO FreeRTOS APIs\n");
    printf("   - ISRs at or below: Can use FromISR APIs\n\n");
    printf("TYPICAL ASSIGNMENT:\n");
    printf("   0-2:  Ultra-critical (no RTOS APIs)\n");
    printf("   3-4:  Time-critical\n");
    printf("   5:    configMAX_SYSCALL threshold\n");
    printf("   6-14: Normal peripherals (use FromISR)\n");
    printf("   15:   SysTick/PendSV (RTOS kernel)\n\n");
    printf("COMMON MISTAKE:\n");
    printf("   Using xSemaphoreGiveFromISR from priority < threshold\n");
    printf("   -> Undefined behavior, crashes, data corruption\n");
    return 0;
}
