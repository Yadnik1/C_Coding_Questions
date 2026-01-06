/*
 * ============================================================================
 * QUESTION 30: How do you Debug RTOS Applications?
 * ============================================================================
 *
 * DIFFICULTY: Medium-Hard | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * Debugging multi-threaded real-time systems requires specialized techniques.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Debugging RTOS is challenging because timing-dependent bugs disappear when
 * you add breakpoints or printf. Key techniques: (1) Use RTOS-aware debuggers
 * (Segger, IAR) that show task states and stacks. (2) Enable stack overflow
 * checking and use configASSERT liberally. (3) Use trace tools like SEGGER
 * SystemView for non-intrusive timing analysis. (4) Check for priority
 * inversion, deadlocks, and race conditions. (5) Monitor task CPU usage and
 * stack watermarks. (6) Add structured logging with timestamps. The Heisenbug
 * problem is real - some bugs only manifest when not being observed."
 *
 * ============================================================================
 * VISUALIZATION: Common RTOS Bugs
 * ============================================================================
 *
 *   COMMON BUG TYPES AND SYMPTOMS:
 *
 *   +----------------------+----------------------------------+---------------+
 *   | Bug Type             | Symptoms                         | Detection     |
 *   +----------------------+----------------------------------+---------------+
 *   | Stack Overflow       | Random crashes, data corruption  | Watermark     |
 *   | Priority Inversion   | High-prio task delayed           | Trace tools   |
 *   | Deadlock             | System freeze, tasks not running | Task state    |
 *   | Race Condition       | Intermittent data corruption     | Static anal.  |
 *   | Memory Leak          | Gradual heap exhaustion          | Heap monitor  |
 *   | Starvation           | Low-prio task never runs         | CPU usage     |
 *   | ISR too long         | Missed interrupts, latency       | Trace/scope   |
 *   +----------------------+----------------------------------+---------------+
 *
 *
 *   THE HEISENBUG PROBLEM:
 *
 *   Without debugging:          With printf:
 *   Task A  Task B              Task A  Task B
 *   [A1]    [B1]                [A1]
 *   [A2]    [B2]                [print...]     <- Timing changed!
 *       [RACE!]                 [A2]    [B1]
 *                               [A3]    [B2]
 *                                    NO RACE   <- Bug disappears!
 *
 * ============================================================================
 * DEBUGGING TECHNIQUES
 * ============================================================================
 *
 * 1. STACK OVERFLOW DETECTION
 *    - Most common RTOS bug
 *    - Corrupts adjacent memory
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "task.h"

// ============================================================================
// TECHNIQUE 1: Stack Overflow Detection
// ============================================================================

// FreeRTOSConfig.h
#define configCHECK_FOR_STACK_OVERFLOW    2  // Method 2 is most thorough
#define configUSE_MALLOC_FAILED_HOOK      1

// Stack overflow hook - called when overflow detected
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // CRITICAL: This function is called from ISR context!
    // Cannot use printf or blocking functions

    // Option 1: Halt for debugger
    taskDISABLE_INTERRUPTS();
    for (;;) {
        // Breakpoint here to see which task overflowed
        __BKPT(0);
    }

    // Option 2: Store info and reset
    // Store task name in backup RAM or flash
    // NVIC_SystemReset();
}

// Malloc failed hook
void vApplicationMallocFailedHook(void) {
    taskDISABLE_INTERRUPTS();
    for (;;) {
        __BKPT(0);  // Out of heap memory!
    }
}


// ============================================================================
// TECHNIQUE 2: Stack Watermark Monitoring
// ============================================================================

// Check how much stack each task has used
void print_stack_usage(void) {
    TaskHandle_t tasks[10];
    UBaseType_t num_tasks;

    num_tasks = uxTaskGetNumberOfTasks();

    // Get task handles
    char buffer[40];
    printf("Task Stack Usage:\n");
    printf("%-20s %10s %10s\n", "Task", "High Water", "Free Words");
    printf("--------------------------------------------\n");

    // Method 1: Using vTaskList (if configUSE_TRACE_FACILITY = 1)
    vTaskList(buffer);
    printf("%s\n", buffer);

    // Method 2: Individual task check
    TaskStatus_t task_status;
    TaskHandle_t current = xTaskGetCurrentTaskHandle();

    UBaseType_t watermark = uxTaskGetStackHighWaterMark(current);
    printf("Current task watermark: %u words remaining\n", watermark);

    // WARNING: If watermark < 20 words, increase stack size!
}


// ============================================================================
// TECHNIQUE 3: Assert Macros
// ============================================================================

// Enable in FreeRTOSConfig.h
#define configASSERT(x) if(!(x)) { taskDISABLE_INTERRUPTS(); for(;;); }

// Usage in code
void critical_function(void *ptr) {
    configASSERT(ptr != NULL);           // Check parameters
    configASSERT(queue_handle != NULL);  // Check handles
    configASSERT(xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE);
}

// Custom assert with more info
#define MY_ASSERT(x) do { \
    if (!(x)) { \
        printf("ASSERT FAILED: %s, line %d\n", __FILE__, __LINE__); \
        taskDISABLE_INTERRUPTS(); \
        for(;;); \
    } \
} while(0)


// ============================================================================
// TECHNIQUE 4: Runtime Statistics
// ============================================================================

// Enable in FreeRTOSConfig.h
#define configGENERATE_RUN_TIME_STATS     1
#define configUSE_STATS_FORMATTING_FUNCTIONS 1

// Configure timer for stats (example for STM32)
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()  init_stats_timer()
#define portGET_RUN_TIME_COUNTER_VALUE()          get_stats_timer()

volatile uint32_t stats_timer_count = 0;

void init_stats_timer(void) {
    // Configure a timer running at 10-100x tick rate
    // Higher frequency = better resolution
    TIM2->PSC = SystemCoreClock / 100000 - 1;  // 100kHz
    TIM2->ARR = 0xFFFFFFFF;
    TIM2->CR1 = TIM_CR1_CEN;
}

uint32_t get_stats_timer(void) {
    return TIM2->CNT;
}

// Print CPU usage per task
void print_runtime_stats(void) {
    char buffer[512];
    vTaskGetRunTimeStats(buffer);
    printf("Task Runtime Statistics:\n");
    printf("%-20s %10s %10s\n", "Task", "Abs Time", "% Time");
    printf("%s\n", buffer);
}


// ============================================================================
// TECHNIQUE 5: Structured Logging
// ============================================================================

typedef enum {
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel_t;

#define CURRENT_LOG_LEVEL LOG_LEVEL_DEBUG

// Thread-safe logging with mutex
SemaphoreHandle_t log_mutex;

#define LOG(level, fmt, ...) do { \
    if (level <= CURRENT_LOG_LEVEL) { \
        xSemaphoreTake(log_mutex, portMAX_DELAY); \
        printf("[%lu][%s][%s] " fmt "\n", \
               xTaskGetTickCount(), \
               pcTaskGetName(NULL), \
               #level, \
               ##__VA_ARGS__); \
        xSemaphoreGive(log_mutex); \
    } \
} while(0)

#define LOG_ERROR(fmt, ...) LOG(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  LOG(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)

// Usage
void some_function(void) {
    LOG_INFO("Processing started");
    LOG_DEBUG("Buffer size: %d", buffer_size);

    if (error_occurred) {
        LOG_ERROR("Failed with error code: %d", error_code);
    }
}


// ============================================================================
// TECHNIQUE 6: Task State Inspection
// ============================================================================

void print_task_states(void) {
    TaskStatus_t *task_array;
    UBaseType_t num_tasks;
    uint32_t total_runtime;

    num_tasks = uxTaskGetNumberOfTasks();
    task_array = pvPortMalloc(num_tasks * sizeof(TaskStatus_t));

    if (task_array != NULL) {
        num_tasks = uxTaskGetSystemState(task_array, num_tasks, &total_runtime);

        printf("Task States:\n");
        printf("%-16s %8s %6s %10s\n", "Name", "State", "Prio", "Stack");
        printf("--------------------------------------------\n");

        for (int i = 0; i < num_tasks; i++) {
            char state;
            switch (task_array[i].eCurrentState) {
                case eRunning:   state = 'X'; break;
                case eReady:     state = 'R'; break;
                case eBlocked:   state = 'B'; break;
                case eSuspended: state = 'S'; break;
                case eDeleted:   state = 'D'; break;
                default:         state = '?'; break;
            }

            printf("%-16s %8c %6u %10u\n",
                   task_array[i].pcTaskName,
                   state,
                   task_array[i].uxCurrentPriority,
                   task_array[i].usStackHighWaterMark);
        }

        vPortFree(task_array);
    }
}
*/

/*
 * ============================================================================
 * TRACE TOOLS
 * ============================================================================
 *
 * SEGGER SystemView:
 * - Non-intrusive trace recording
 * - Shows task switches, ISRs, API calls
 * - Timeline visualization
 *
 * Percepio Tracealyzer:
 * - Similar to SystemView
 * - Additional analysis views
 *
 * Both work by instrumenting FreeRTOS kernel.
 *
 *
 *   TRACE VISUALIZATION:
 *
 *   Time:  0        10       20       30       40
 *          |        |        |        |        |
 *   ISR:   |█|      |█|      |█|      |█|      |
 *   Task1: |===|    |====|   |==|     |===|    |
 *   Task2:     |===|     |==|   |====|    |===|
 *   Task3:               |█|        |=======|
 *   Idle:                    |.|         |.|
 *
 *   Can identify:
 *   - Task preemption patterns
 *   - ISR duration and frequency
 *   - CPU utilization
 *   - Unexpected delays
 */

/*
// Enable SystemView tracing
#include "SEGGER_SYSVIEW.h"

void init_trace(void) {
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();
}

// Custom markers for important events
void important_function(void) {
    SEGGER_SYSVIEW_PrintfHost("Starting critical section");

    // Mark timing-sensitive code
    SEGGER_SYSVIEW_MarkStart(0);
    time_critical_code();
    SEGGER_SYSVIEW_MarkStop(0);

    SEGGER_SYSVIEW_PrintfHost("Done");
}
*/

/*
 * ============================================================================
 * DEBUGGING SPECIFIC BUGS
 * ============================================================================
 *
 * DEADLOCK DEBUGGING:
 * -------------------
 * 1. Check task states - multiple tasks in Blocked state?
 * 2. Which resources are they waiting for?
 * 3. Is there a cycle? (A waits for B, B waits for A)
 *
 *   Detection code:
 *   - Monitor task states periodically
 *   - If task blocked "too long", log warning
 *   - Track which mutex each task holds
 *
 *
 * RACE CONDITION DEBUGGING:
 * -------------------------
 * 1. Static analysis tools (ThreadSanitizer, Coverity)
 * 2. Add assertions to check data integrity
 * 3. Instrument shared variable access
 * 4. Increase timing variability to expose races
 *
 *
 * PRIORITY INVERSION DEBUGGING:
 * ----------------------------
 * 1. Use trace tool to see actual task execution order
 * 2. Check if low-prio task holds mutex high-prio needs
 * 3. Verify mutex has priority inheritance enabled
 */

/*
// Deadlock detection helper
typedef struct {
    TaskHandle_t task;
    SemaphoreHandle_t waiting_for;
    SemaphoreHandle_t holding;
    TickType_t blocked_since;
} DeadlockInfo_t;

#define MAX_TASKS 10
DeadlockInfo_t deadlock_info[MAX_TASKS];

void check_for_deadlock(void) {
    // If any task blocked for > N seconds, investigate
    TickType_t now = xTaskGetTickCount();
    const TickType_t DEADLOCK_THRESHOLD = pdMS_TO_TICKS(30000);  // 30 sec

    for (int i = 0; i < MAX_TASKS; i++) {
        if (deadlock_info[i].waiting_for != NULL) {
            if ((now - deadlock_info[i].blocked_since) > DEADLOCK_THRESHOLD) {
                printf("WARNING: Potential deadlock!\n");
                printf("Task %s blocked for %lu ms\n",
                       pcTaskGetName(deadlock_info[i].task),
                       (now - deadlock_info[i].blocked_since) * portTICK_PERIOD_MS);
            }
        }
    }
}
*/

/*
 * ============================================================================
 * ZEPHYR DEBUGGING
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/debug/thread_analyzer.h>

LOG_MODULE_REGISTER(my_module, LOG_LEVEL_DBG);

// Structured logging
void zephyr_logging_example(void) {
    LOG_INF("Information message");
    LOG_WRN("Warning: value=%d", some_value);
    LOG_ERR("Error occurred!");
    LOG_DBG("Debug details: ptr=%p", ptr);
}

// Thread analyzer - stack and CPU usage
void print_thread_stats(void) {
    thread_analyzer_print();
}

// Stack sentinel (overflow detection)
// Enable: CONFIG_THREAD_STACK_INFO=y
//         CONFIG_THREAD_ANALYZER=y

// Fault handling
#include <zephyr/fatal.h>

void k_sys_fatal_error_handler(unsigned int reason, const z_arch_esf_t *esf) {
    printk("FATAL ERROR: reason=%u\n", reason);

    // Print register dump
    #if defined(CONFIG_ARM)
    printk("PC:  0x%08x\n", esf->basic.pc);
    printk("LR:  0x%08x\n", esf->basic.lr);
    printk("PSR: 0x%08x\n", esf->basic.xpsr);
    #endif

    // Hang for debugger
    while (1) {
        k_busy_wait(1000);
    }
}
*/

/*
 * ============================================================================
 * DEBUGGING CHECKLIST
 * ============================================================================
 *
 * WHEN SYSTEM CRASHES:
 * □ Enable stack overflow detection
 * □ Check heap exhaustion (malloc failed hook)
 * □ Look at fault registers (HardFault handler)
 * □ Check stack pointer is in valid range
 *
 * WHEN SYSTEM FREEZES:
 * □ Check task states (all blocked = deadlock?)
 * □ Check ISR is not stuck in loop
 * □ Check watchdog is kicking
 * □ Try breaking in debugger - where is each task?
 *
 * WHEN BEHAVIOR IS WRONG:
 * □ Check race conditions on shared data
 * □ Verify task priorities are correct
 * □ Check for priority inversion
 * □ Verify queue/semaphore usage correct
 *
 * WHEN TIMING IS OFF:
 * □ Check tick rate configuration
 * □ Profile actual execution times
 * □ Check for unexpected ISR latency
 * □ Use trace tool to see actual timing
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. MENTION RTOS-SPECIFIC TOOLS
 *    "Use RTOS-aware debuggers that show task states"
 *
 * 2. DISCUSS HEISENBUG PROBLEM
 *    "Printf can change timing, use non-intrusive trace"
 *
 * 3. LIST COMMON BUGS
 *    "Stack overflow, deadlock, priority inversion, race conditions"
 *
 * 4. EMPHASIZE PROACTIVE MEASURES
 *    "Enable stack checking, use configASSERT, monitor heap"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "How do you debug intermittent crashes?"
 * A: Enable all assertions and stack checking. Add logging with timestamps.
 *    Store crash info in backup RAM. Use trace tool to capture events
 *    leading up to crash.
 *
 * Q: "What causes HardFault in ARM?"
 * A: Stack overflow, null pointer dereference, divide by zero, invalid
 *    memory access, unaligned access. Check fault status registers
 *    (CFSR, HFSR, BFAR, MMAR) for details.
 *
 * Q: "How do you find memory leaks?"
 * A: Monitor xPortGetFreeHeapSize() over time. If it decreases steadily,
 *    there's a leak. Track allocations/frees. Use memory allocation hooks.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 30 ===\n\n");
    printf("Q: How do you Debug RTOS Applications?\n\n");
    printf("TECHNIQUES:\n\n");
    printf("1. STACK OVERFLOW DETECTION:\n");
    printf("   - configCHECK_FOR_STACK_OVERFLOW = 2\n");
    printf("   - Implement vApplicationStackOverflowHook\n");
    printf("   - Monitor uxTaskGetStackHighWaterMark\n\n");
    printf("2. ASSERTIONS:\n");
    printf("   - configASSERT(condition)\n");
    printf("   - Check all pointers, handles, return values\n\n");
    printf("3. RUNTIME STATISTICS:\n");
    printf("   - vTaskGetRunTimeStats() for CPU usage\n");
    printf("   - uxTaskGetSystemState() for task states\n\n");
    printf("4. TRACE TOOLS:\n");
    printf("   - SEGGER SystemView\n");
    printf("   - Percepio Tracealyzer\n");
    printf("   - Non-intrusive timing analysis\n\n");
    printf("5. STRUCTURED LOGGING:\n");
    printf("   - Timestamps, task names, log levels\n");
    printf("   - Thread-safe with mutex\n\n");
    printf("COMMON BUGS:\n");
    printf("- Stack overflow -> random crashes\n");
    printf("- Deadlock -> system freeze\n");
    printf("- Race condition -> data corruption\n");
    printf("- Priority inversion -> missed deadlines\n");
    return 0;
}
