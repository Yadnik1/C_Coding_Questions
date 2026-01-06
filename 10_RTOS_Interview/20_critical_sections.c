/*
 * ============================================================================
 * QUESTION 20: What are Critical Sections? When and how do you use them?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * Critical sections are fundamental to protecting shared resources.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "A critical section is a block of code that accesses shared resources and
 * must execute atomically - without interruption. In RTOS, there are two main
 * approaches: (1) Disable interrupts - fastest but blocks ALL interrupts and
 * ISRs, use for very short sections only. (2) Disable scheduler - allows ISRs
 * but prevents task switching, less intrusive. FreeRTOS provides
 * taskENTER_CRITICAL/taskEXIT_CRITICAL for interrupt disable, and
 * vTaskSuspendAll/xTaskResumeAll for scheduler disable. Critical sections
 * must be SHORT to maintain system responsiveness."
 *
 * ============================================================================
 * VISUALIZATION: Critical Section Protection
 * ============================================================================
 *
 *   WITHOUT PROTECTION (Race Condition):
 *
 *   Shared variable: count = 5
 *
 *   Task A                         Task B
 *   -------                        -------
 *   Read count (5)
 *   Add 1 (=6)
 *                                  Read count (5)  <- Still sees 5!
 *                                  Add 1 (=6)
 *   Write count (6)
 *                                  Write count (6) <- Overwrites!
 *
 *   Expected: 7, Actual: 6  BUG!
 *
 *
 *   WITH CRITICAL SECTION:
 *
 *   Task A                         Task B
 *   -------                        -------
 *   ENTER_CRITICAL()
 *   Read count (5)
 *   Add 1 (=6)
 *   Write count (6)                [BLOCKED - interrupts disabled
 *   EXIT_CRITICAL()                 or scheduler suspended]
 *                                  ENTER_CRITICAL()
 *                                  Read count (6)  <- Sees correct value!
 *                                  Add 1 (=7)
 *                                  Write count (7)
 *                                  EXIT_CRITICAL()
 *
 *   Expected: 7, Actual: 7  CORRECT!
 *
 * ============================================================================
 * METHODS OF PROTECTION
 * ============================================================================
 *
 *   +----------------------+-------------------+------------------+------------+
 *   | Method               | Blocks Tasks?     | Blocks ISRs?     | Use When   |
 *   +----------------------+-------------------+------------------+------------+
 *   | Disable Interrupts   | Yes               | YES              | Very short |
 *   | (taskENTER_CRITICAL) |                   |                  | ISR-safe   |
 *   +----------------------+-------------------+------------------+------------+
 *   | Suspend Scheduler    | Yes               | No               | Longer but |
 *   | (vTaskSuspendAll)    |                   | ISRs still run!  | no ISRs    |
 *   +----------------------+-------------------+------------------+------------+
 *   | Mutex                | Yes (waiting)     | No               | Longer     |
 *   |                      | Others can run!   |                  | sections   |
 *   +----------------------+-------------------+------------------+------------+
 *
 *
 *   INTERRUPT LATENCY IMPACT:
 *
 *   Without Critical Section:
 *   Interrupt ↓ Response immediate
 *   ---------↓------------------------->
 *            |<- ~1us latency
 *
 *   During Critical Section (Interrupts Disabled):
 *   Interrupt ↓           Response delayed!
 *   ==========[CRITICAL]=↓===============>
 *            |<------- N us latency ------->|
 *              ^                    ^
 *              |                    |
 *           Critical             Exit
 *           Enter                Critical
 *
 *   Rule: Keep critical sections < 10us for real-time systems!
 *
 * ============================================================================
 * CODE EXAMPLE: FreeRTOS Critical Sections
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "task.h"

// ============================================================================
// METHOD 1: Disable Interrupts (taskENTER_CRITICAL)
// ============================================================================

// Use for: Very short code, ISR-safe protection
// Effect: Disables interrupts up to configMAX_SYSCALL_INTERRUPT_PRIORITY

volatile uint32_t shared_counter = 0;

void increment_with_critical(void) {
    taskENTER_CRITICAL();  // Disable interrupts
    {
        // Critical section - keep VERY SHORT!
        shared_counter++;
    }
    taskEXIT_CRITICAL();   // Re-enable interrupts
}

// NESTING IS SUPPORTED:
void nested_critical_example(void) {
    taskENTER_CRITICAL();      // Depth = 1
    {
        // Some code...
        taskENTER_CRITICAL();  // Depth = 2
        {
            // Nested critical section
        }
        taskEXIT_CRITICAL();   // Depth = 1 (still protected!)
    }
    taskEXIT_CRITICAL();       // Depth = 0 (interrupts restored)
}


// ============================================================================
// METHOD 2: From ISR Context
// ============================================================================

// In ISR, use different macros:
void SysTick_Handler(void) {
    UBaseType_t saved_interrupt_status;

    saved_interrupt_status = taskENTER_CRITICAL_FROM_ISR();
    {
        // Modify shared data from ISR
        shared_counter++;
    }
    taskEXIT_CRITICAL_FROM_ISR(saved_interrupt_status);
}


// ============================================================================
// METHOD 3: Suspend Scheduler (vTaskSuspendAll)
// ============================================================================

// Use for: Longer sections where ISRs must still run
// Effect: Prevents task switches but ISRs execute normally

void longer_critical_operation(void) {
    vTaskSuspendAll();  // Suspend scheduler
    {
        // Longer operation - ISRs still run!
        // Other tasks cannot preempt
        // But this task can be interrupted by ISR

        process_large_buffer();  // Takes 1ms+

        // WARNING: Don't call blocking FreeRTOS APIs here!
        // xQueueSend, xSemaphoreTake, vTaskDelay, etc. will FAIL
    }
    xTaskResumeAll();   // Resume scheduler (may trigger immediate switch)

    // If a higher priority task became ready during suspend,
    // context switch happens here
}


// ============================================================================
// PRACTICAL EXAMPLE: Protecting Shared Structure
// ============================================================================

typedef struct {
    uint32_t timestamp;
    int16_t  x;
    int16_t  y;
    int16_t  z;
    uint8_t  status;
} SensorData_t;

volatile SensorData_t sensor_data;

// ISR writes sensor data
void sensor_isr(void) {
    UBaseType_t saved = taskENTER_CRITICAL_FROM_ISR();
    {
        // Update all fields atomically
        sensor_data.timestamp = get_tick();
        sensor_data.x = read_adc_x();
        sensor_data.y = read_adc_y();
        sensor_data.z = read_adc_z();
        sensor_data.status = 0x01;  // New data available
    }
    taskEXIT_CRITICAL_FROM_ISR(saved);
}

// Task reads sensor data
void processing_task(void *pvParameters) {
    SensorData_t local_copy;

    for (;;) {
        taskENTER_CRITICAL();
        {
            // Copy to local variable
            local_copy = sensor_data;
            sensor_data.status = 0x00;  // Mark as read
        }
        taskEXIT_CRITICAL();

        // Process local copy outside critical section
        // (takes variable time, no impact on interrupts)
        process_sensor(&local_copy);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


// ============================================================================
// PRACTICAL EXAMPLE: Read-Modify-Write Protection
// ============================================================================

volatile uint32_t gpio_shadow_register = 0;

void set_gpio_pin(uint8_t pin) {
    taskENTER_CRITICAL();
    {
        // Read-modify-write must be atomic
        gpio_shadow_register |= (1 << pin);
        GPIO->ODR = gpio_shadow_register;
    }
    taskEXIT_CRITICAL();
}

void clear_gpio_pin(uint8_t pin) {
    taskENTER_CRITICAL();
    {
        gpio_shadow_register &= ~(1 << pin);
        GPIO->ODR = gpio_shadow_register;
    }
    taskEXIT_CRITICAL();
}
*/

/*
 * ============================================================================
 * ZEPHYR CRITICAL SECTIONS
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>
#include <zephyr/irq.h>

// ============================================================================
// METHOD 1: IRQ Lock (Disable Interrupts)
// ============================================================================

void zephyr_irq_lock_example(void) {
    unsigned int key;

    key = irq_lock();  // Disable interrupts, save state
    {
        // Critical section - ISRs blocked
        shared_data++;
    }
    irq_unlock(key);   // Restore previous interrupt state
}


// ============================================================================
// METHOD 2: Spinlock (SMP-Safe)
// ============================================================================

// For multi-core systems (SMP)
struct k_spinlock my_lock;

void spinlock_example(void) {
    k_spinlock_key_t key;

    key = k_spin_lock(&my_lock);  // Lock and disable interrupts
    {
        // Critical section - safe on multi-core!
        shared_data++;
    }
    k_spin_unlock(&my_lock, key);
}


// ============================================================================
// METHOD 3: Scheduler Lock
// ============================================================================

void scheduler_lock_example(void) {
    k_sched_lock();  // Prevent thread scheduling
    {
        // Other threads cannot run
        // But ISRs still execute
        complex_operation();
    }
    k_sched_unlock();
}
*/

/*
 * ============================================================================
 * WHEN TO USE EACH METHOD
 * ============================================================================
 *
 *   DECISION FLOWCHART:
 *
 *   Is code in ISR? ──Yes──> Use taskENTER_CRITICAL_FROM_ISR()
 *         │
 *        No
 *         │
 *         v
 *   Need to protect ──Yes──> Use taskENTER_CRITICAL()
 *   from ISRs?                (keep < 10us!)
 *         │
 *        No
 *         │
 *         v
 *   Duration ──Short──> Use vTaskSuspendAll()
 *   of section?          (ISRs still work)
 *         │
 *       Long
 *         │
 *         v
 *   Use Mutex instead!
 *   (allows other tasks to run)
 *
 * ============================================================================
 * CRITICAL SECTION TIMING GUIDELINES
 * ============================================================================
 *
 *   +------------------+-----------------+----------------------------------+
 *   | Duration         | Method          | Notes                            |
 *   +------------------+-----------------+----------------------------------+
 *   | < 10 us          | taskENTER_      | OK, minimal interrupt impact     |
 *   |                  | CRITICAL        |                                  |
 *   +------------------+-----------------+----------------------------------+
 *   | 10-100 us        | taskENTER_      | Use carefully, check latency     |
 *   |                  | CRITICAL        | requirements                     |
 *   +------------------+-----------------+----------------------------------+
 *   | 100 us - 1 ms    | vTaskSuspendAll | ISRs run, no task switches       |
 *   +------------------+-----------------+----------------------------------+
 *   | > 1 ms           | Mutex           | Other tasks can run while waiting|
 *   +------------------+-----------------+----------------------------------+
 *
 * ============================================================================
 * COMMON MISTAKES
 * ============================================================================
 *
 * MISTAKE 1: Calling blocking APIs in critical section
 *
 *   taskENTER_CRITICAL();
 *   xQueueReceive(queue, &data, portMAX_DELAY);  // WRONG! Will hang!
 *   taskEXIT_CRITICAL();
 *
 *
 * MISTAKE 2: Very long critical sections
 *
 *   taskENTER_CRITICAL();
 *   for (int i = 0; i < 10000; i++) {  // WRONG! Too long!
 *       process_item(i);
 *   }
 *   taskEXIT_CRITICAL();
 *
 *
 * MISTAKE 3: Unbalanced enter/exit
 *
 *   taskENTER_CRITICAL();
 *   if (error) {
 *       return;  // WRONG! Exit without EXIT_CRITICAL!
 *   }
 *   taskEXIT_CRITICAL();
 *
 *
 * MISTAKE 4: Forgetting FROM_ISR in ISR context
 *
 *   void SysTick_Handler(void) {
 *       taskENTER_CRITICAL();  // WRONG! Should be FROM_ISR version!
 *       shared_data++;
 *       taskEXIT_CRITICAL();
 *   }
 *
 * ============================================================================
 * CRITICAL SECTION vs MUTEX
 * ============================================================================
 *
 *   +------------------+------------------------+------------------------+
 *   | Aspect           | Critical Section       | Mutex                  |
 *   +------------------+------------------------+------------------------+
 *   | Blocking         | Spin-wait (busy)       | Sleep-wait (efficient) |
 *   | Other tasks      | Cannot run             | Can run while waiting  |
 *   | ISR safe         | Yes (with FROM_ISR)    | No (cannot use in ISR) |
 *   | Priority inherit | No                     | Yes                    |
 *   | Duration         | Very short             | Can be longer          |
 *   | Implementation   | Disable interrupts     | Kernel object          |
 *   +------------------+------------------------+------------------------+
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. KNOW THE TWO TYPES
 *    "Interrupt disable for ISR-safe, scheduler suspend for longer sections"
 *
 * 2. EMPHASIZE DURATION
 *    "Critical sections must be SHORT to maintain real-time response"
 *
 * 3. EXPLAIN USE CASES
 *    "Read-modify-write operations, accessing multi-byte data from ISR"
 *
 * 4. MENTION ALTERNATIVES
 *    "For longer sections, use mutex instead to allow other tasks to run"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "Can you nest critical sections?"
 * A: Yes, FreeRTOS maintains a nesting count. Interrupts are only re-enabled
 *    when the outermost EXIT_CRITICAL is called.
 *
 * Q: "What happens if an ISR occurs during a critical section?"
 * A: With taskENTER_CRITICAL: ISR is delayed until EXIT_CRITICAL.
 *    With vTaskSuspendAll: ISR runs normally but can't cause task switch.
 *
 * Q: "How do you choose between critical section and mutex?"
 * A: Critical section for < 100us operations, especially when ISR protection
 *    needed. Mutex for longer operations or when priority inheritance needed.
 *
 * Q: "What's configMAX_SYSCALL_INTERRUPT_PRIORITY?"
 * A: FreeRTOS only disables interrupts at or below this priority level.
 *    Higher priority interrupts (lower number on ARM) still run!
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 20 ===\n\n");
    printf("Q: What are Critical Sections?\n\n");
    printf("CRITICAL SECTIONS:\n");
    printf("- Code that must execute atomically\n");
    printf("- Protects shared resources from corruption\n");
    printf("- Must be kept SHORT!\n\n");
    printf("METHODS:\n");
    printf("1. taskENTER_CRITICAL() - Disables interrupts\n");
    printf("   + ISR-safe\n");
    printf("   - Blocks ALL tasks and ISRs\n");
    printf("   - Use for < 10us operations\n\n");
    printf("2. vTaskSuspendAll() - Suspends scheduler\n");
    printf("   + ISRs still run\n");
    printf("   - Only blocks task switches\n");
    printf("   - Use for longer operations\n\n");
    printf("3. Mutex - For long critical sections\n");
    printf("   + Other tasks can run\n");
    printf("   + Priority inheritance\n");
    printf("   - Cannot use in ISR\n\n");
    printf("GOLDEN RULE:\n");
    printf("Keep critical sections as SHORT as possible!\n");
    return 0;
}
