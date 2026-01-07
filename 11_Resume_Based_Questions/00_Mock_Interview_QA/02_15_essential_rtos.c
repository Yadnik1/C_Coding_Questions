/*******************************************************************************
 * 15 ESSENTIAL RTOS INTERVIEW QUESTIONS
 * Master These Before Any Embedded Interview
 *
 * These 15 questions cover 90% of what interviewers ask about RTOS.
 * Each includes: Core Answer + Code Example + Anticipated Follow-ups
 *
 * LEARNING ORDER (Questions arranged for optimal understanding):
 * 1-3:   Fundamentals (Mutex/Sem, Queues, Scheduling)
 * 4-6:   ISR & Timing (Interrupts, Timers, Watchdog)
 * 7-9:   Synchronization Problems (Priority Inversion, Deadlock, Race Conditions)
 * 10-12: Memory & Resources (Stack, Memory Pools, Real-Time)
 * 13-15: Advanced (Event Groups, Task Notifications, Debugging)
 ******************************************************************************/

/*******************************************************************************
 * QUESTION 1: MUTEX vs SEMAPHORE
 * This is THE most asked RTOS question. Know it cold.
 ******************************************************************************/
/*
INTERVIEWER: "What's the difference between a mutex and a semaphore?"

=== CORE ANSWER (30 seconds) ===

MUTEX (Mutual Exclusion):
- Binary lock: locked or unlocked
- OWNERSHIP: Only the task that locked it can unlock it
- Used for: Protecting shared resources (one task at a time)
- Has priority inheritance to prevent priority inversion

SEMAPHORE:
- Counter: can be 0, 1, 2, ... N
- NO OWNERSHIP: Any task can signal (give) it
- Used for: Signaling between tasks, counting resources
- No priority inheritance

MEMORY AID:
- Mutex = "Key to a bathroom" (you lock, only you unlock)
- Semaphore = "Tickets at a counter" (anyone can take/return)

=== CODE EXAMPLE ===
*/

// MUTEX - Protecting shared I2C bus
SemaphoreHandle_t i2c_mutex;

void task_sensor_read(void *pvParameters) {
    while(1) {
        xSemaphoreTake(i2c_mutex, portMAX_DELAY);  // I take the lock
        i2c_read_sensor();                          // Only I access I2C
        xSemaphoreGive(i2c_mutex);                  // I release the lock
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// SEMAPHORE - Signaling from ISR to task
SemaphoreHandle_t data_ready_sem;

void UART_ISR(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // ISR doesn't "own" the semaphore, just signals
    xSemaphoreGiveFromISR(data_ready_sem, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void task_process_data(void *pvParameters) {
    while(1) {
        xSemaphoreTake(data_ready_sem, portMAX_DELAY);  // Wait for signal
        process_uart_data();
    }
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "Can you use a binary semaphore instead of a mutex?"
A: "Technically yes, but you lose priority inheritance. If a low-priority task
    holds the semaphore and a high-priority task waits, a medium-priority task
    can preempt the low one - causing priority inversion. Mutex prevents this
    by temporarily boosting the low task's priority."

Q2: "What's a counting semaphore used for?"
A: "Two main uses:
    1. Resource counting: e.g., 5 UART ports, semaphore initialized to 5
    2. Event counting: e.g., count how many packets received, process them one by one"

Q3: "What happens if a task tries to give a mutex it doesn't own?"
A: "In FreeRTOS, it's undefined behavior - the mutex may become corrupted.
    Some RTOS (like Zephyr) return an error. This is why ownership matters."

Q4: "Can you use mutex in an ISR?"
A: "NO. Mutexes can block, and ISRs can't block. Use a binary semaphore for
    ISR-to-task signaling, or a queue. Never take a mutex in an ISR."
*/


/*******************************************************************************
 * QUESTION 2: QUEUES (MESSAGE PASSING)
 * Critical for ISR-to-task communication - builds on Q1.
 ******************************************************************************/
/*
INTERVIEWER: "How do you use queues in RTOS? When prefer them over shared variables?"

=== CORE ANSWER (30 seconds) ===

WHAT ARE QUEUES:
- FIFO data structures for passing messages between tasks/ISRs
- Thread-safe: built-in synchronization
- Can block: sender waits if full, receiver waits if empty
- Copy data: each send copies bytes into queue

WHEN TO USE QUEUES vs SHARED VARIABLES:

USE QUEUES:
- Passing data from ISR to task (ISR-safe send)
- Producer-consumer patterns
- Multiple producers or consumers
- When you want automatic synchronization

USE SHARED VARIABLES (+ mutex):
- Single flag or counter
- Very frequent access (queue overhead too high)
- Large data (queue copies, slow for big structs)

=== CODE EXAMPLE ===
*/

// Queue for passing sensor readings from ISR to task
typedef struct {
    uint16_t temperature;
    uint16_t humidity;
    uint32_t timestamp;
} sensor_data_t;

QueueHandle_t sensor_queue;

void init_queue(void) {
    // Queue holds 10 sensor readings
    sensor_queue = xQueueCreate(10, sizeof(sensor_data_t));
}

// ISR sends data to queue (non-blocking)
void ADC_ISR(void) {
    sensor_data_t reading;
    reading.temperature = ADC_READ_TEMP();
    reading.humidity = ADC_READ_HUMID();
    reading.timestamp = get_tick_count();

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(sensor_queue, &reading, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Task receives and processes (blocks until data available)
void sensor_task(void *pvParameters) {
    sensor_data_t reading;
    while(1) {
        // Block forever until data arrives
        if (xQueueReceive(sensor_queue, &reading, portMAX_DELAY) == pdTRUE) {
            process_sensor_data(&reading);
            send_to_cloud(&reading);
        }
    }
}

// Multiple producers, single consumer pattern
void producer_task_1(void *pvParameters) {
    sensor_data_t data;
    while(1) {
        data = read_sensor_1();
        xQueueSend(sensor_queue, &data, pdMS_TO_TICKS(100));  // Timeout if full
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "What happens if queue is full and you try to send?"
A: "Depends on timeout parameter:
    - 0: returns immediately with error (pdFAIL)
    - portMAX_DELAY: blocks forever until space available
    - N ticks: blocks up to N ticks, then returns error
    In ISR, always use 0 timeout (ISRs can't block)."

Q2: "Queues copy data - what about large data structures?"
A: "For large data, send a POINTER instead:
    void* ptr = &large_buffer;
    xQueueSend(queue, &ptr, timeout);

    But careful with lifetime! Buffer must remain valid until receiver processes.
    Often use a pool of static buffers."

Q3: "What's the difference between xQueueSend and xQueueSendToBack/Front?"
A: "xQueueSend = xQueueSendToBack (FIFO, normal case)
    xQueueSendToFront = insert at head (LIFO, for urgent messages)
    Use front for high-priority events that should be processed first."

Q4: "How do you size the queue?"
A: "Consider: burst rate (how fast producer) vs drain rate (how fast consumer).
    If producer can burst 20 items before consumer runs, need 20+ slots.
    Add margin for worst case. Monitor high-water mark in testing."
*/


/*******************************************************************************
 * QUESTION 3: TASK SCHEDULING
 * Fundamentals - know preemptive vs cooperative cold.
 ******************************************************************************/
/*
INTERVIEWER: "Explain preemptive vs cooperative scheduling."

=== CORE ANSWER (30 seconds) ===

PREEMPTIVE SCHEDULING:
- Scheduler CAN interrupt a running task at any time
- Higher priority task becomes ready → immediately preempts current task
- Most RTOS use this (FreeRTOS, Zephyr, VxWorks)
- Pros: Responsive, deterministic for high-priority tasks
- Cons: Need synchronization (mutexes) for shared data

COOPERATIVE SCHEDULING:
- Tasks explicitly yield control: taskYIELD() or blocking call
- Scheduler only switches when task allows it
- Simpler: no unexpected preemption = fewer race conditions
- Cons: One task can starve others; less responsive

ROUND-ROBIN:
- For tasks at SAME priority level
- Each gets a time slice, then next task runs
- Time slice = configTICK_RATE_HZ ticks typically

=== CODE EXAMPLE ===
*/

// Preemptive example - high priority task runs immediately
void high_priority_task(void *pvParameters) {
    while(1) {
        // As soon as this task is ready, it preempts lower priority tasks
        do_critical_work();
        vTaskDelay(pdMS_TO_TICKS(100));  // Sleep, let others run
    }
}

void low_priority_task(void *pvParameters) {
    while(1) {
        // This can be interrupted at ANY point by high_priority_task
        do_background_work();  // May never complete if high priority busy
    }
}

// Cooperative example - explicit yielding
void cooperative_task(void *pvParameters) {
    while(1) {
        do_some_work();
        taskYIELD();  // Explicitly give up CPU
        do_more_work();
        taskYIELD();
    }
}

// Time-slicing for same priority tasks
#define TASK_PRIORITY  2

void task_a(void *pvParameters) {
    while(1) {
        // Runs for one tick, then task_b gets a turn (if same priority)
        compute_intensive_work();
    }
}

void task_b(void *pvParameters) {
    while(1) {
        // Same priority as task_a, shares time
        compute_intensive_work();
    }
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "When would you use cooperative scheduling?"
A: "1. Simple systems with trusted code (all tasks are yours)
    2. When avoiding synchronization complexity is priority
    3. Super memory-constrained (smaller stack per task)
    4. Legacy code migration from super-loop
    Generally, preemptive is preferred for responsiveness."

Q2: "What triggers a context switch in preemptive scheduling?"
A: "1. Higher priority task becomes ready (ISR, semaphore give)
    2. Current task blocks (semaphore take, delay, queue read)
    3. Current task yields (taskYIELD)
    4. Time slice expires (for round-robin at same priority)"

Q3: "How do you prevent a high priority task from starving lower ones?"
A: "1. Design: high priority tasks should block (wait for events)
    2. Rate limiting: high priority does work then delays
    3. Priority ceiling: prevent priority inversion
    4. Monitor: track CPU usage per task, alert if one dominates"

Q4: "What's the cost of a context switch?"
A: "Saving/restoring registers: ~50-200 cycles on Cortex-M
    Cache pollution: can be significant on larger processors
    Typically 1-10 microseconds on embedded MCUs.
    Minimize by: reducing task count, batching work, right priorities."
*/


/*******************************************************************************
 * QUESTION 4: INTERRUPT HANDLING IN RTOS
 * Critical to understand ISR limitations - now you'll see why Q1-2 matter.
 ******************************************************************************/
/*
INTERVIEWER: "How should interrupts interact with RTOS tasks?"

=== CORE ANSWER (45 seconds) ===

GOLDEN RULES:

1. KEEP ISRs SHORT:
   - Do minimum work: set flag, copy data to buffer
   - Defer heavy processing to task
   - Target: <10µs, absolutely <100µs

2. NEVER BLOCK IN ISR:
   - No xSemaphoreTake() - use xSemaphoreGiveFromISR()
   - No xQueueReceive() - use xQueueSendFromISR()
   - No vTaskDelay() - ISR is not a task

3. USE "FromISR" VARIANTS:
   - FreeRTOS provides special ISR-safe functions
   - They don't block, return immediately
   - Pass xHigherPriorityTaskWoken for yield decision

4. YIELD IF NEEDED:
   - If ISR wakes higher priority task, yield to it
   - portYIELD_FROM_ISR(xHigherPriorityTaskWoken)

5. PRIORITY MANAGEMENT:
   - Interrupt priorities are DIFFERENT from task priorities
   - Interrupts above configMAX_SYSCALL_INTERRUPT_PRIORITY can't use RTOS calls

=== CODE EXAMPLE ===
*/

// CORRECT - ISR with deferred processing
SemaphoreHandle_t data_ready_sem_q4;
volatile uint8_t rx_buffer[256];
volatile uint16_t rx_index = 0;

void UART_RX_ISR(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Minimal work: copy byte to buffer
    rx_buffer[rx_index++] = UART->DR;

    // Check if complete message received
    if (rx_buffer[rx_index-1] == '\n') {
        // Signal task to process - don't process here!
        xSemaphoreGiveFromISR(data_ready_sem_q4, &xHigherPriorityTaskWoken);
        rx_index = 0;
    }

    // Clear interrupt flag
    UART->SR &= ~UART_SR_RXNE;

    // Yield if we woke a higher priority task
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_processing_task(void *pvParameters) {
    while(1) {
        // Block until ISR signals
        xSemaphoreTake(data_ready_sem_q4, portMAX_DELAY);

        // Heavy processing happens here, in task context
        parse_message(rx_buffer);
        execute_command(rx_buffer);
        send_response();
    }
}

// WRONG - Don't do this!
void WRONG_ISR(void) {
    xSemaphoreTake(mutex, portMAX_DELAY);  // WRONG: blocking in ISR!
    process_data();                         // WRONG: too much work!
    printf("Done\n");                       // WRONG: printf not ISR-safe!
    xSemaphoreGive(mutex);
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "What is xHigherPriorityTaskWoken for?"
A: "It's an output flag. If the FromISR call woke a task with higher priority
    than the currently running task, this is set to pdTRUE. We then call
    portYIELD_FROM_ISR to switch to that task immediately, ensuring responsive
    handling of the interrupt."

Q2: "What happens if you call xSemaphoreTake (not FromISR) in an ISR?"
A: "It can block, which hangs the ISR. Other interrupts may be blocked (if
    priorities allow). Watchdog may trigger. System becomes unresponsive.
    Some RTOS have configASSERT that catches this in debug builds."

Q3: "How do you handle high-frequency interrupts?"
A: "1. DMA for bulk transfers (zero CPU involvement)
    2. Double buffering: ISR fills buffer A while task processes B
    3. Hardware FIFOs: let hardware buffer reduce interrupt rate
    4. Timer-based polling: sometimes polling beats interrupts at high rates"

Q4: "What's the relationship between interrupt priority and task priority?"
A: "Completely separate! Interrupt priorities are hardware-level (NVIC on ARM).
    Task priorities are RTOS scheduler-level. ALL interrupts preempt ALL tasks.
    A priority-5 interrupt preempts a priority-100 task."
*/


/*******************************************************************************
 * QUESTION 5: TIMER MANAGEMENT
 * Software timers and hardware timer integration.
 ******************************************************************************/
/*
INTERVIEWER: "How do you handle timing in RTOS applications?"

=== CORE ANSWER (30 seconds) ===

THREE APPROACHES:

1. RTOS SOFTWARE TIMERS:
   - Created by RTOS, execute callback when expired
   - Run in timer service task context (not ISR)
   - Good for: non-critical timeouts, periodic tasks
   - Limited: callback shouldn't block

2. HARDWARE TIMERS + ISR:
   - Direct timer peripheral control
   - Precise timing, true interrupt context
   - Good for: motor control, PWM, precise measurements
   - More complex setup

3. vTaskDelay / vTaskDelayUntil:
   - Task blocks for specified duration
   - vTaskDelayUntil: precise periodic execution
   - Good for: simple periodic tasks
   - Limited: only affects calling task

CHOICE GUIDE:
- Need <1ms precision? → Hardware timer
- Periodic task with variable work time? → vTaskDelayUntil
- One-shot timeout? → Software timer
- PWM/motor/timing-critical? → Hardware timer

=== CODE EXAMPLE ===
*/

// SOFTWARE TIMER - Periodic status report
TimerHandle_t status_timer;

void status_timer_callback(TimerHandle_t xTimer) {
    // This runs in timer service task, NOT ISR
    // Can use RTOS calls, but shouldn't block
    send_status_update();  // Keep quick
}

void init_timers(void) {
    status_timer = xTimerCreate(
        "Status",                       // Name
        pdMS_TO_TICKS(1000),           // Period: 1 second
        pdTRUE,                         // Auto-reload (periodic)
        NULL,                           // Timer ID
        status_timer_callback           // Callback function
    );
    xTimerStart(status_timer, 0);
}

// vTaskDelayUntil - Precise periodic execution
void sensor_task_q5(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(100);  // 100ms period

    while(1) {
        // Process takes variable time (20-50ms)
        read_and_process_sensors();

        // This ensures EXACTLY 100ms between wake times
        // regardless of how long processing took
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

// vTaskDelay - Simple delay (NOT periodic-precise)
void other_task(void *pvParameters) {
    while(1) {
        process_something();  // Takes 30ms
        vTaskDelay(pdMS_TO_TICKS(100));  // Wait 100ms after processing
        // Total period: 130ms (processing + delay)
    }
}

// HARDWARE TIMER - Precise microsecond control
void TIM2_Init(void) {
    // Configure for 1µs ticks
    TIM2->PSC = (SystemCoreClock / 1000000) - 1;
    TIM2->ARR = 1000 - 1;  // 1ms period
    TIM2->DIER |= TIM_DIER_UIE;  // Enable interrupt
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        // This is TRUE interrupt context - very precise
        // Don't use blocking RTOS calls here
        toggle_debug_pin();  // Sub-microsecond response
    }
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "What's the difference between vTaskDelay and vTaskDelayUntil?"
A: "vTaskDelay: delays FROM NOW for specified ticks
    vTaskDelayUntil: delays UNTIL specified absolute tick count

    For periodic tasks, vTaskDelayUntil maintains consistent period even if
    task execution time varies. vTaskDelay accumulates drift."

Q2: "Why shouldn't software timer callbacks block?"
A: "All software timers share one service task. If callback blocks,
    no other timer callbacks can execute. It's like blocking in ISR.
    If you need to do blocking work, have callback signal a task."

Q3: "How do you handle timing jitter?"
A: "1. Use vTaskDelayUntil for consistent periods
    2. Raise task priority to reduce preemption
    3. Use hardware timers for microsecond precision
    4. Measure and log actual timing to quantify jitter
    Acceptable jitter depends on application - motor control needs <10µs,
    UI updates can tolerate 10ms."

Q4: "How accurate is RTOS tick timing?"
A: "Limited by tick rate (configTICK_RATE_HZ, typically 1000Hz = 1ms).
    Can't schedule finer than one tick. Hardware timers have no such limit.
    For sub-millisecond: use hardware timers or increase tick rate (costs CPU)."
*/


/*******************************************************************************
 * QUESTION 6: WATCHDOG TIMER
 * Simple concept but interviewers probe for real-world usage.
 ******************************************************************************/
/*
INTERVIEWER: "How do you use watchdog timers in RTOS systems?"

=== CORE ANSWER (30 seconds) ===

WHAT IS IT:
An independent hardware timer that resets the system if not "kicked" periodically.
It's a last-resort recovery from software hangs.

BASIC USAGE:
1. Configure timeout (e.g., 2 seconds)
2. Start watchdog
3. Periodically "kick" it (write to register)
4. If software hangs and can't kick, watchdog expires → system reset

RTOS CHALLENGE:
With multiple tasks, which one kicks the watchdog? If one task kicks but another
is hung, you don't detect the problem.

SOLUTION - TASK WATCHDOG:
- Supervisor task collects "heartbeat" from all other tasks
- Only kicks hardware watchdog if ALL tasks report healthy
- Each task must report within its expected period

=== CODE EXAMPLE ===
*/

// Simple (BAD) - Only one task kicks watchdog
void simple_task_q6(void *pvParameters) {
    while(1) {
        do_work();
        IWDG_KickDog();  // If THIS task runs, watchdog kicked
        // But what if another task is hung?
    }
}

// Better - Task-level watchdog system
#define NUM_TASKS  4
#define WDT_TIMEOUT_MS  2000

volatile uint32_t task_heartbeat[NUM_TASKS];
TaskHandle_t task_handles[NUM_TASKS];

// Each task calls this to report it's alive
void task_report_alive(uint8_t task_id) {
    task_heartbeat[task_id] = xTaskGetTickCount();
}

// Supervisor task checks all heartbeats
void watchdog_supervisor_task(void *pvParameters) {
    while(1) {
        bool all_healthy = true;
        uint32_t now = xTaskGetTickCount();

        for (int i = 0; i < NUM_TASKS; i++) {
            uint32_t age = now - task_heartbeat[i];
            if (age > pdMS_TO_TICKS(WDT_TIMEOUT_MS)) {
                // Task i is hung!
                log_error("Task %d not responding, last heartbeat %u ms ago",
                          i, age * portTICK_PERIOD_MS);
                all_healthy = false;
            }
        }

        if (all_healthy) {
            IWDG_Refresh();  // Kick hardware watchdog
        }
        // If not healthy, don't kick - let system reset

        vTaskDelay(pdMS_TO_TICKS(500));  // Check every 500ms
    }
}

// Example task using heartbeat
void sensor_task_q6(void *pvParameters) {
    while(1) {
        task_report_alive(TASK_ID_SENSOR);
        read_sensors();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "What if the supervisor task itself hangs?"
A: "Hardware watchdog catches it - supervisor must kick within timeout.
    If supervisor hangs, it stops kicking, system resets.
    That's why supervisor should be minimal - just check and kick."

Q2: "What's window watchdog vs independent watchdog?"
A: "Independent (IWDG): Simple, must kick before timeout
    Window (WWDG): Must kick within a TIME WINDOW - not too early, not too late
    Window catches runaway code that kicks too fast (infinite loop)."

Q3: "How do you debug watchdog resets?"
A: "1. Check reset reason register after boot (RCC_CSR on STM32)
    2. Before reset: save diagnostic info to retained RAM
    3. Log last operation, task states, stack traces
    4. Some MCUs have 'debug freeze' - stops watchdog when debugger halts"

Q4: "Should you disable watchdog during debugging?"
A: "Yes, typically. Use #ifdef DEBUG to skip watchdog init.
    Or configure MCU to freeze watchdog when halted.
    But remember to re-enable for production testing!"
*/


/*******************************************************************************
 * QUESTION 7: PRIORITY INVERSION
 * THE classic RTOS problem. They WILL ask how to solve it.
 ******************************************************************************/
/*
INTERVIEWER: "What is priority inversion and how do you prevent it?"

=== CORE ANSWER (45 seconds) ===

WHAT IS IT:
Priority inversion is when a HIGH priority task is blocked waiting for a
LOW priority task, but a MEDIUM priority task runs instead - effectively
making HIGH wait for MEDIUM (priorities inverted).

THE SCENARIO:
1. Low priority task (L) takes mutex M
2. High priority task (H) needs mutex M, blocks
3. Medium priority task (M) becomes ready
4. M preempts L (M > L priority)
5. H is waiting for L, but L can't run because M is running
6. H's effective priority is now BELOW M - INVERTED!

SOLUTIONS:

1. PRIORITY INHERITANCE (most common):
   - When H blocks on mutex held by L, L temporarily inherits H's priority
   - L runs at high priority, releases mutex quickly
   - H continues, L returns to original priority
   - FreeRTOS mutexes do this automatically

2. PRIORITY CEILING:
   - Assign each mutex a "ceiling" priority (highest of any task that uses it)
   - Any task holding mutex runs at ceiling priority immediately
   - More predictable than inheritance, prevents chained inversion

3. AVOID SHARING:
   - Design so high and low priority tasks don't share resources
   - Use message passing instead of shared memory

=== CODE EXAMPLE ===
*/

// BAD - Using binary semaphore (no priority inheritance)
SemaphoreHandle_t shared_resource_sem;  // Binary semaphore
// If L holds this and H waits, M can still preempt L = INVERSION

// GOOD - Using mutex (has priority inheritance)
SemaphoreHandle_t shared_resource_mutex;

void init_q7(void) {
    // This mutex has priority inheritance built-in
    shared_resource_mutex = xSemaphoreCreateMutex();
}

void high_priority_task_q7(void *pvParameters) {
    while(1) {
        // When we block here, low_priority_task inherits our priority
        xSemaphoreTake(shared_resource_mutex, portMAX_DELAY);
        access_shared_resource();
        xSemaphoreGive(shared_resource_mutex);
    }
}

void low_priority_task_q7(void *pvParameters) {
    while(1) {
        xSemaphoreTake(shared_resource_mutex, portMAX_DELAY);
        access_shared_resource();  // Runs at boosted priority if H is waiting
        xSemaphoreGive(shared_resource_mutex);
    }
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "What's the difference between priority inheritance and priority ceiling?"
A: "Inheritance: boost happens WHEN high priority task blocks (reactive)
    Ceiling: boost happens WHEN mutex is taken (proactive)
    Ceiling prevents priority inversion chains but wastes priority if no
    high priority task actually needs the resource."

Q2: "Can priority inversion still happen with priority inheritance?"
A: "Yes, in chained scenarios: H waits on L, L waits on another resource held
    by LL. Inheritance only goes one level in basic implementations. Priority
    ceiling protocol prevents this entirely."

Q3: "Tell me about the Mars Pathfinder bug."
A: "Classic priority inversion case. High priority bus management task was
    blocked by low priority meteorological task sharing a mutex. Medium priority
    tasks preempted the low one. System kept resetting due to watchdog. NASA
    patched it by enabling priority inheritance via remote upload."

Q4: "How would you detect priority inversion in your system?"
A: "Use RTOS trace tools (Tracealyzer, SystemView). Look for patterns where:
    - High priority task blocked time > expected
    - Medium priority task runs while high is blocked
    Also: log mutex hold times, alert if held too long."
*/


/*******************************************************************************
 * QUESTION 8: DEADLOCK
 * Classic CS concept, but interviewers want RTOS-specific answers.
 ******************************************************************************/
/*
INTERVIEWER: "What is deadlock and how do you prevent it?"

=== CORE ANSWER (45 seconds) ===

WHAT IS IT:
Deadlock is when two or more tasks are waiting for each other forever.
Neither can proceed because each holds a resource the other needs.

THE FOUR CONDITIONS (all must be true):
1. Mutual exclusion - resources can't be shared
2. Hold and wait - task holds one resource while waiting for another
3. No preemption - resources can't be forcibly taken
4. Circular wait - A waits for B, B waits for A

CLASSIC SCENARIO:
- Task A holds Mutex 1, wants Mutex 2
- Task B holds Mutex 2, wants Mutex 1
- Both wait forever

PREVENTION STRATEGIES:

1. LOCK ORDERING (most practical):
   - Always acquire mutexes in same order: Mutex1 before Mutex2
   - Eliminates circular wait

2. TIMEOUT:
   - Don't wait forever: xSemaphoreTake(mutex, TIMEOUT)
   - If timeout, release held resources, retry later

3. TRY-LOCK:
   - Non-blocking attempt: if can't get second mutex, release first
   - Retry with backoff

4. SINGLE LOCK:
   - Use one coarse-grained mutex instead of multiple fine-grained
   - Simpler but less concurrent

=== CODE EXAMPLE ===
*/

// BAD - Potential deadlock
SemaphoreHandle_t mutex_uart, mutex_flash;

void task_A(void *pvParameters) {
    xSemaphoreTake(mutex_uart, portMAX_DELAY);   // Takes UART first
    xSemaphoreTake(mutex_flash, portMAX_DELAY);  // Then wants Flash
    // ... work ...
    xSemaphoreGive(mutex_flash);
    xSemaphoreGive(mutex_uart);
}

void task_B(void *pvParameters) {
    xSemaphoreTake(mutex_flash, portMAX_DELAY);  // Takes Flash first
    xSemaphoreTake(mutex_uart, portMAX_DELAY);   // Then wants UART - DEADLOCK!
    // ... work ...
    xSemaphoreGive(mutex_uart);
    xSemaphoreGive(mutex_flash);
}

// GOOD - Consistent lock ordering
void task_A_fixed(void *pvParameters) {
    xSemaphoreTake(mutex_flash, portMAX_DELAY);  // Always Flash first
    xSemaphoreTake(mutex_uart, portMAX_DELAY);   // Then UART
    // ... work ...
    xSemaphoreGive(mutex_uart);
    xSemaphoreGive(mutex_flash);
}

void task_B_fixed(void *pvParameters) {
    xSemaphoreTake(mutex_flash, portMAX_DELAY);  // Same order: Flash first
    xSemaphoreTake(mutex_uart, portMAX_DELAY);   // Then UART
    // ... work ...
    xSemaphoreGive(mutex_uart);
    xSemaphoreGive(mutex_flash);
}

// ALTERNATIVE - Timeout approach
void task_with_timeout(void *pvParameters) {
    while(1) {
        if (xSemaphoreTake(mutex_flash, pdMS_TO_TICKS(100)) == pdTRUE) {
            if (xSemaphoreTake(mutex_uart, pdMS_TO_TICKS(100)) == pdTRUE) {
                // Got both, do work
                xSemaphoreGive(mutex_uart);
                xSemaphoreGive(mutex_flash);
                break;  // Success
            } else {
                // Couldn't get UART, release Flash and retry
                xSemaphoreGive(mutex_flash);
                vTaskDelay(pdMS_TO_TICKS(10));  // Backoff
            }
        }
    }
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "What's the difference between deadlock and livelock?"
A: "Deadlock: tasks blocked forever, doing nothing
    Livelock: tasks actively changing state but making no progress
    Example: Two tasks keep releasing and re-acquiring locks trying to
    avoid deadlock, but keep colliding. Add randomized backoff to fix."

Q2: "How do you detect deadlock at runtime?"
A: "1. Watchdog: if task doesn't kick watchdog, system resets
    2. Stack trace dump: see where tasks are blocked
    3. RTOS trace: visualize blocking relationships
    4. Timeout + logging: log whenever timeout occurs on mutex"

Q3: "What about self-deadlock (recursive locking)?"
A: "Task tries to take mutex it already holds - instant deadlock with self.
    Solutions: Use recursive mutex (xSemaphoreCreateRecursiveMutex) which
    counts lock/unlock calls, or redesign to avoid nested locking."

Q4: "How does your lock ordering scale with many mutexes?"
A: "Assign each mutex a numeric priority (or use memory address). Always
    acquire in ascending order. Document the ordering. For large systems,
    consider hierarchical locking or coarser granularity."
*/


/*******************************************************************************
 * QUESTION 9: RACE CONDITIONS
 * The subtle bug that's hard to find.
 ******************************************************************************/
/*
INTERVIEWER: "What is a race condition? Give an example and how to fix it."

=== CORE ANSWER (30 seconds) ===

WHAT IS IT:
A race condition occurs when program behavior depends on the relative timing
of events - typically multiple tasks/ISRs accessing shared data without
proper synchronization.

THE DANGER:
- Bug may appear rarely (only when timing is "just wrong")
- Hard to reproduce in debugger (timing changes)
- May work for months, then fail in field

COMMON CAUSES:
1. Shared variable without mutex
2. Read-modify-write not atomic
3. Check-then-act sequences
4. ISR and task accessing same data

SOLUTIONS:
1. Mutex for shared resources
2. Disable interrupts briefly for atomic operations
3. Use atomic types/operations
4. Design to avoid sharing (message passing)

=== CODE EXAMPLE ===
*/

// RACE CONDITION - Shared counter
volatile uint32_t packet_count = 0;

void ISR_packet_received(void) {
    packet_count++;  // Read-modify-write: NOT atomic!
}

void main_task_q9(void *pvParameters) {
    while(1) {
        uint32_t local = packet_count;
        packet_count = 0;  // Reset counter
        // RACE: ISR can fire between read and write!
        // Lost packets!
        process_packets(local);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// FIX 1: Disable interrupts
void main_task_fixed_1(void *pvParameters) {
    while(1) {
        taskENTER_CRITICAL();  // Disable interrupts
        uint32_t local = packet_count;
        packet_count = 0;
        taskEXIT_CRITICAL();   // Re-enable

        process_packets(local);  // Do work outside critical section
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// FIX 2: Atomic exchange (if hardware supports)
void main_task_fixed_2(void *pvParameters) {
    while(1) {
        // Atomically read and clear
        uint32_t local = __atomic_exchange_n(&packet_count, 0, __ATOMIC_SEQ_CST);

        process_packets(local);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// FIX 3: Use queue instead of shared variable
QueueHandle_t packet_queue;

void ISR_packet_received_fixed(void) {
    BaseType_t wake;
    uint8_t dummy = 1;
    xQueueSendFromISR(packet_queue, &dummy, &wake);
    portYIELD_FROM_ISR(wake);
}

void main_task_fixed_3(void *pvParameters) {
    uint8_t dummy;
    while(1) {
        // Count how many items in queue
        uint32_t count = 0;
        while (xQueueReceive(packet_queue, &dummy, 0) == pdTRUE) {
            count++;
        }
        process_packets(count);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "What makes read-modify-write non-atomic?"
A: "On most CPUs, 'counter++' compiles to:
    1. LOAD counter into register
    2. INCREMENT register
    3. STORE register back to counter
    If ISR fires between steps 1 and 3, the increment in ISR is lost
    because the main code overwrites with stale value."

Q2: "When is disabling interrupts acceptable?"
A: "Only for VERY short periods (few instructions). Rules:
    - Don't call functions (especially RTOS functions)
    - No loops of unknown length
    - Just read/write/simple math
    Measure the critical section time, keep it <1µs if possible."

Q3: "How do you find race conditions in code review?"
A: "Look for:
    1. Global/shared variables without protection
    2. volatile without synchronization (volatile != thread-safe)
    3. Check-then-act: if (flag) { use_data(); } - flag can change
    4. Any ISR-to-task data sharing
    Ask: 'What if an interrupt fires right HERE?'"

Q4: "What about volatile keyword?"
A: "volatile tells compiler 'don't optimize, always read from memory.'
    It does NOT make access atomic or thread-safe.
    You still need mutex/critical section for multi-word data or
    read-modify-write sequences. volatile is necessary but not sufficient."
*/


/*******************************************************************************
 * QUESTION 10: TASK STACK SIZING
 * Practical knowledge interviewers love.
 ******************************************************************************/
/*
INTERVIEWER: "How do you determine the right stack size for a task?"

=== CORE ANSWER (30 seconds) ===

FACTORS AFFECTING STACK SIZE:

1. LOCAL VARIABLES:
   - Arrays and structs on stack consume space
   - Large local buffers = large stack needed

2. CALL DEPTH:
   - Each function call: ~20-100 bytes (return address, saved registers)
   - Deep recursion = big stack
   - Callback chains (A calls B calls C calls D)

3. INTERRUPT STACKING:
   - On some RTOS, ISR uses interrupted task's stack
   - Add ~64-200 bytes for nested interrupts

4. RTOS OVERHEAD:
   - Context save area: ~64 bytes on Cortex-M
   - Varies by RTOS and configuration

SIZING PROCESS:
1. Start with generous estimate (1KB for simple task)
2. Use uxTaskGetStackHighWaterMark() to measure actual usage
3. Set final size to high water mark + 25% margin
4. Add assertions to catch overflow in testing

=== CODE EXAMPLE ===
*/

// Stack size estimation
#define TASK_STACK_SIZE  512  // 512 words = 2KB on 32-bit system

// Function to check stack usage
void monitor_task_q10(void *pvParameters) {
    while(1) {
        // Get remaining stack (in words) for each task
        UBaseType_t sensor_stack = uxTaskGetStackHighWaterMark(sensor_task_handle);
        UBaseType_t comms_stack = uxTaskGetStackHighWaterMark(comms_task_handle);

        printf("Sensor task: %u words remaining\n", sensor_stack);
        printf("Comms task: %u words remaining\n", comms_stack);

        // Alert if getting low (less than 50 words)
        if (sensor_stack < 50 || comms_stack < 50) {
            log_error("Stack low - increase size!");
        }

        vTaskDelay(pdMS_TO_TICKS(10000));  // Check every 10 seconds
    }
}

// Stack overflow hook (FreeRTOS)
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // This is called when stack overflow detected
    // System is likely corrupt - log and reset
    printf("STACK OVERFLOW in task: %s\n", pcTaskName);
    NVIC_SystemReset();
}

// Enable overflow checking in FreeRTOSConfig.h:
// #define configCHECK_FOR_STACK_OVERFLOW  2

// Calculate stack for specific function
void calculate_example(void) {
    uint8_t buffer[256];       // 256 bytes
    uint32_t values[64];       // 256 bytes
    // Plus saved registers, return addresses...
    // This function alone needs ~600 bytes
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "What's the difference between stack overflow checking method 1 and 2?"
A: "Method 1: Check if stack pointer went past end (fast, catches large overflows)
    Method 2: Fill stack with known pattern (0xA5), check if corrupted
    Method 2 catches small overflows but adds runtime overhead.
    Use method 2 during development."

Q2: "What happens when stack overflows?"
A: "Task's stack overwrites adjacent memory - could be:
    - Another task's stack → that task corrupts
    - Heap → random crashes later
    - Global variables → silent data corruption
    Often hard to debug because symptoms appear elsewhere."

Q3: "How do you reduce stack usage?"
A: "1. Make large buffers static or global (not on stack)
    2. Reduce call depth (inline small functions)
    3. Avoid recursion
    4. Pass large structs by pointer, not value
    5. Share stack via coroutines (FreeRTOS added this)"

Q4: "Can you dynamically increase stack size?"
A: "Not typically - stack is allocated at task creation. To 'resize':
    1. Delete task
    2. Create new task with larger stack
    Some RTOS support stack pools but it's complex. Better to size correctly."
*/


/*******************************************************************************
 * QUESTION 11: MEMORY POOLS AND FRAGMENTATION
 * Shows practical embedded knowledge.
 ******************************************************************************/
/*
INTERVIEWER: "How do you handle dynamic memory in RTOS? What about fragmentation?"

=== CORE ANSWER (45 seconds) ===

THE PROBLEM WITH MALLOC:
1. FRAGMENTATION: After many alloc/free, memory becomes Swiss cheese
   - Have 10KB free but can't allocate 1KB contiguous block
2. NON-DETERMINISTIC: malloc time varies based on heap state
3. FAILURE: malloc returns NULL - many embedded code doesn't handle this

SOLUTIONS:

1. STATIC ALLOCATION (preferred):
   - Allocate everything at compile time
   - No fragmentation, no failure, deterministic
   - Requires knowing max usage upfront

2. MEMORY POOLS:
   - Pre-allocate fixed-size blocks
   - Fast, deterministic O(1) alloc/free
   - No fragmentation (blocks are same size)
   - Downside: some internal fragmentation (32-byte block for 20-byte object)

3. RTOS HEAP SCHEMES:
   - FreeRTOS offers heap_1 through heap_5
   - heap_1: Allocate only, never free (simplest)
   - heap_4: Coalescing free blocks (best general purpose)
   - heap_5: Multiple memory regions

=== CODE EXAMPLE ===
*/

// STATIC ALLOCATION - Best for predictability
#define MAX_MESSAGES  20

typedef struct {
    uint8_t data[64];
    uint32_t timestamp;
} message_t;

static message_t message_pool[MAX_MESSAGES];
static bool message_used[MAX_MESSAGES] = {false};

message_t* message_alloc(void) {
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (!message_used[i]) {
            message_used[i] = true;
            return &message_pool[i];
        }
    }
    return NULL;  // Pool exhausted
}

void message_free(message_t* msg) {
    int idx = msg - message_pool;
    if (idx >= 0 && idx < MAX_MESSAGES) {
        message_used[idx] = false;
    }
}

// FREERTOS MEMORY POOL - Using queue as pool
StaticQueue_t message_queue_buffer;
message_t* message_storage[MAX_MESSAGES];
QueueHandle_t message_pool_queue;

void init_pools(void) {
    // Queue acts as pool: receive = allocate, send = free
    message_pool_queue = xQueueCreateStatic(
        MAX_MESSAGES,
        sizeof(message_t*),
        (uint8_t*)message_storage,
        &message_queue_buffer
    );

    // Pre-populate pool with pointers to all blocks
    for (int i = 0; i < MAX_MESSAGES; i++) {
        message_t* ptr = &message_pool[i];
        xQueueSend(message_pool_queue, &ptr, 0);
    }
}

message_t* pool_alloc(void) {
    message_t* ptr;
    if (xQueueReceive(message_pool_queue, &ptr, 0) == pdTRUE) {
        return ptr;
    }
    return NULL;
}

void pool_free(message_t* ptr) {
    xQueueSend(message_pool_queue, &ptr, 0);
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "When is it OK to use malloc in embedded?"
A: "1. Only at initialization (never during runtime)
    2. Linux-based systems with virtual memory
    3. Prototyping / non-safety-critical applications
    4. When you have generous RAM and can detect/handle failure"

Q2: "What's heap_5 in FreeRTOS?"
A: "Supports multiple non-contiguous memory regions. Useful when you have
    internal SRAM + external SRAM, or memory-mapped regions. You specify
    multiple HeapRegions_t, it manages allocation across them."

Q3: "How do you detect memory leaks in embedded?"
A: "1. Track high-water mark of heap usage over time
    2. Compare allocated vs expected at checkpoints
    3. Valgrind if running on Linux
    4. Custom wrappers around malloc/free that log
    5. Static analysis tools (PC-lint, Coverity)"

Q4: "What about stack vs heap for large buffers?"
A: "Prefer static for large buffers - allocated at link time, no fragmentation.
    Stack: limited size, buffer dies with function - only for temporary, small.
    Heap: last resort, with careful lifecycle management.

    Example: 1KB UART buffer → static global, not stack or heap."
*/


/*******************************************************************************
 * QUESTION 12: REAL-TIME REQUIREMENTS
 * Shows you understand "real-time" isn't just "fast".
 ******************************************************************************/
/*
INTERVIEWER: "What does 'real-time' mean? What's the difference between hard and soft?"

=== CORE ANSWER (30 seconds) ===

REAL-TIME = DETERMINISTIC TIMING, NOT NECESSARILY FAST

It means the system must respond within a guaranteed time bound. A system that
responds in 1ms 99% of the time but takes 1 second 1% is NOT real-time.

HARD REAL-TIME:
- Missing deadline = SYSTEM FAILURE
- Examples: airbag deployment, cardiac pacemaker, flight controls
- Must be mathematically provable
- No exceptions, no averages - EVERY deadline met

SOFT REAL-TIME:
- Missing deadline = DEGRADED PERFORMANCE
- Examples: video streaming (frame drops OK), audio playback
- Occasional misses acceptable
- Statistical guarantees (99.9% within deadline)

FIRM REAL-TIME:
- Missing deadline = RESULT IS USELESS (but not catastrophic)
- Examples: stock trading (late trade is worthless)
- No value after deadline, but system continues

=== CODE EXAMPLE ===
*/

// Hard real-time: Motor control loop - MUST complete every 100µs
// Missing deadline could cause motor damage or injury
void TIM2_IRQHandler_q12(void) {
    // This MUST complete in <100µs
    uint32_t start = get_cycle_count();

    int16_t current = read_motor_current();  // 5µs
    int16_t position = read_encoder();        // 3µs
    int16_t pwm = calculate_pid(current, position);  // 10µs
    set_motor_pwm(pwm);                       // 2µs

    // Total: ~20µs, with 80µs margin - GOOD

    uint32_t elapsed = get_cycle_count() - start;
    if (elapsed > DEADLINE_CYCLES) {
        // In hard real-time, this is FATAL
        trigger_safety_shutdown();
    }

    TIM2->SR &= ~TIM_SR_UIF;
}

// Soft real-time: Audio playback - occasional glitch acceptable
void audio_task(void *pvParameters) {
    while(1) {
        // Should complete in <10ms, but if it takes 15ms occasionally,
        // user hears a small glitch - not great but not fatal
        uint32_t start = xTaskGetTickCount();

        process_audio_buffer();

        uint32_t elapsed = xTaskGetTickCount() - start;
        if (elapsed > pdMS_TO_TICKS(10)) {
            deadline_misses++;  // Log it, but continue
        }

        xSemaphoreTake(audio_ready_sem, portMAX_DELAY);
    }
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "How do you prove you meet hard real-time requirements?"
A: "Worst-Case Execution Time (WCET) analysis:
    1. Identify all execution paths
    2. Find longest path
    3. Account for interrupts, cache misses, bus contention
    4. Add margins
    Tools: static analysis (aiT), measurement with coverage.
    Then verify priority assignment meets all deadlines (Rate Monotonic Analysis)."

Q2: "Can Linux be real-time?"
A: "Standard Linux: NO - scheduler isn't deterministic, unbounded latencies.
    PREEMPT_RT patch: SOFT real-time, ~50µs worst case
    Xenomai/RTAI: Co-kernel approach, HARD real-time possible
    For hard real-time, most use dedicated RTOS or bare-metal."

Q3: "What's Rate Monotonic Scheduling?"
A: "A priority assignment strategy: shorter period = higher priority.
    Proven optimal for fixed-priority preemptive schedulers.
    If task set is schedulable under any fixed-priority scheme, it's
    schedulable under RMS. Utilization bound: ~69% for many tasks."

Q4: "How does your LoRaWAN work handle timing requirements?"
A: "LoRaWAN Class A has receive windows 1-2 seconds after TX - soft real-time.
    We used FreeRTOS timer callbacks for window timing, typically hit within
    ±10ms which is well within spec. Not hard real-time - missing a window
    just means retry on next uplink."
*/


/*******************************************************************************
 * QUESTION 13: EVENT GROUPS
 * Powerful but often misunderstood feature.
 ******************************************************************************/
/*
INTERVIEWER: "What are event groups and when would you use them?"

=== CORE ANSWER (30 seconds) ===

WHAT ARE THEY:
A set of event bits (typically 24 bits) that tasks can set, clear, and wait on.
Multiple tasks can wait on the same event group for different bit combinations.

USE CASES:

1. SYNCHRONIZATION POINT:
   - Wait for multiple events before proceeding
   - "Start processing when UART ready AND sensor ready AND WiFi connected"

2. BROADCAST SIGNAL:
   - One task sets bit, multiple tasks wake
   - Unlike semaphore (one-to-one), event group is one-to-many

3. TASK RENDEZVOUS:
   - Multiple tasks wait for each other before continuing
   - "All 3 init tasks complete before entering main loop"

KEY OPERATIONS:
- xEventGroupSetBits(): Set event bits
- xEventGroupWaitBits(): Block until bit pattern matches
- xEventGroupSync(): Set bits AND wait for pattern (rendezvous)

=== CODE EXAMPLE ===
*/

// Event bits definitions
#define EVENT_UART_READY     (1 << 0)
#define EVENT_SENSOR_READY   (1 << 1)
#define EVENT_WIFI_CONNECTED (1 << 2)
#define EVENT_DATA_AVAILABLE (1 << 3)

EventGroupHandle_t system_events;

void init_q13(void) {
    system_events = xEventGroupCreate();
}

// Tasks set their ready bits
void uart_init_task(void *pvParameters) {
    init_uart();
    xEventGroupSetBits(system_events, EVENT_UART_READY);
    vTaskDelete(NULL);  // Init done, delete self
}

void sensor_init_task(void *pvParameters) {
    init_sensors();
    xEventGroupSetBits(system_events, EVENT_SENSOR_READY);
    vTaskDelete(NULL);
}

void wifi_task(void *pvParameters) {
    connect_wifi();
    xEventGroupSetBits(system_events, EVENT_WIFI_CONNECTED);
    // Continue running for wifi operations
    while(1) {
        handle_wifi_events();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Main task waits for ALL systems ready
void main_task_q13(void *pvParameters) {
    const EventBits_t ALL_READY = EVENT_UART_READY |
                                   EVENT_SENSOR_READY |
                                   EVENT_WIFI_CONNECTED;

    // Wait until ALL bits are set
    EventBits_t bits = xEventGroupWaitBits(
        system_events,
        ALL_READY,          // Bits to wait for
        pdFALSE,            // Don't clear on exit
        pdTRUE,             // Wait for ALL bits (AND)
        portMAX_DELAY       // Wait forever
    );

    if ((bits & ALL_READY) == ALL_READY) {
        printf("System ready, starting main loop\n");
    }

    // Main loop
    while(1) {
        // Wait for data available event
        xEventGroupWaitBits(system_events, EVENT_DATA_AVAILABLE,
                            pdTRUE,  // Clear bit on exit
                            pdFALSE, // Wait for ANY bit
                            portMAX_DELAY);
        process_data();
    }
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "What's the difference between waiting for ANY vs ALL bits?"
A: "xWaitForAllBits parameter:
    pdTRUE = AND logic: ALL specified bits must be set
    pdFALSE = OR logic: ANY of the specified bits being set is enough

    Use AND for 'all systems ready' scenarios.
    Use OR for 'any event happened' scenarios."

Q2: "What's xEventGroupSync used for?"
A: "Task rendezvous - multiple tasks wait for each other:
    - Each task sets its bit AND waits for all bits
    - Atomic: avoids race between set and wait
    - Example: 3 processing tasks must all reach checkpoint before any continues"

Q3: "Why only 24 bits in FreeRTOS event groups?"
A: "FreeRTOS uses top 8 bits of EventBits_t (32-bit) for internal control flags.
    This leaves 24 bits for user events. For most applications, 24 events is plenty."

Q4: "Event groups vs semaphores for signaling?"
A: "Semaphore: one-to-one, counting possible, faster for simple cases
    Event group: one-to-many, bit patterns, more flexible

    Use semaphore for producer-consumer.
    Use event groups for broadcast signals or multi-event synchronization."
*/


/*******************************************************************************
 * QUESTION 14: TASK NOTIFICATION (LIGHTER THAN SEMAPHORE)
 * Modern RTOS feature, shows up-to-date knowledge.
 ******************************************************************************/
/*
INTERVIEWER: "What are task notifications and when would you use them?"

=== CORE ANSWER (30 seconds) ===

WHAT ARE THEY:
Direct-to-task signaling mechanism - each task has a built-in "notification value"
that other tasks or ISRs can set/increment/use as lightweight synchronization.

WHY USE OVER SEMAPHORE:
1. FASTER: ~45% faster than semaphore on FreeRTOS
2. LESS RAM: No separate semaphore object needed (part of TCB)
3. MORE FEATURES: Can pass 32-bit value, not just signal

LIMITATIONS:
1. ONE-TO-ONE: Only one task can wait on notification
2. CAN'T BROADCAST: Unlike event groups
3. TASK-SPECIFIC: Can't share between tasks like semaphore

USE CASES:
- ISR signaling specific task (most common)
- Replacing binary semaphore for single consumer
- Passing small data (32-bit value) along with signal

=== CODE EXAMPLE ===
*/

// Using task notification instead of semaphore
TaskHandle_t data_processing_task_handle;

// ISR notifies task
void UART_RX_ISR_q14(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Option 1: Simple signal (like binary semaphore)
    vTaskNotifyGiveFromISR(data_processing_task_handle,
                           &xHigherPriorityTaskWoken);

    // Option 2: Send value with notification
    // xTaskNotifyFromISR(data_processing_task_handle,
    //                    bytes_received,     // 32-bit value
    //                    eSetValueWithOverwrite,
    //                    &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Task waits for notification
void data_processing_task(void *pvParameters) {
    while(1) {
        // Option 1: Simple wait (like semaphore take)
        ulTaskNotifyTake(pdTRUE,  // Clear notification on exit
                         portMAX_DELAY);

        // Option 2: Wait and receive value
        // uint32_t value;
        // xTaskNotifyWait(0,                    // Don't clear on entry
        //                 ULONG_MAX,            // Clear all bits on exit
        //                 &value,               // Received value
        //                 portMAX_DELAY);

        process_received_data();
    }
}

// COMPARISON: Semaphore approach (more RAM, slower)
SemaphoreHandle_t data_sem;  // Extra RAM allocation

void sem_isr(void) {
    BaseType_t wake;
    xSemaphoreGiveFromISR(data_sem, &wake);
    portYIELD_FROM_ISR(wake);
}

void sem_task(void *pvParameters) {
    while(1) {
        xSemaphoreTake(data_sem, portMAX_DELAY);
        process_received_data();
    }
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "What are the different notification actions?"
A: "eSetBits: OR bits into notification value (like event group)
    eIncrement: Increment value (like counting semaphore)
    eSetValueWithOverwrite: Set value, overwrite existing
    eSetValueWithoutOverwrite: Set only if previous was read
    eNoAction: Just wake task, don't modify value"

Q2: "Can notification replace event groups?"
A: "Partially. Task notification has 32 bits you can set/clear like event group.
    BUT: Only one task can wait on notification vs. multiple tasks on event group.
    For multi-task synchronization, event groups are still needed."

Q3: "What's the RAM saving?"
A: "Semaphore: ~80 bytes (QueueHandle_t structure)
    Task notification: 0 extra bytes (built into TCB)
    For many semaphores, savings add up. But only useful for 1:1 scenarios."

Q4: "Why can't multiple tasks wait on notification?"
A: "Notification is part of Task Control Block (TCB) - it IS the task.
    Semaphore is a separate object that any task can wait on.
    It's a design trade-off: simplicity/performance vs. flexibility."
*/


/*******************************************************************************
 * QUESTION 15: DEBUGGING RTOS ISSUES
 * Practical skills interviewers love to hear about.
 ******************************************************************************/
/*
INTERVIEWER: "How do you debug issues in RTOS-based systems?"

=== CORE ANSWER (45 seconds) ===

COMMON ISSUES AND DEBUG APPROACHES:

1. STACK OVERFLOW:
   - Enable configCHECK_FOR_STACK_OVERFLOW = 2
   - Implement vApplicationStackOverflowHook
   - Monitor uxTaskGetStackHighWaterMark()
   - Look for: hard faults, random crashes, corrupted data

2. PRIORITY INVERSION:
   - Use trace tools (Tracealyzer, SystemView)
   - Look for: high priority task blocked unexpectedly long
   - Check mutex usage, ensure using mutex not binary semaphore

3. DEADLOCK:
   - Add timeout to all blocking calls (never portMAX_DELAY in prod)
   - Log when timeout occurs
   - Check lock ordering consistency

4. RACE CONDITIONS:
   - Hard to reproduce - use stress testing
   - Add assertions around shared data
   - Review all shared variable access for proper protection

5. MEMORY LEAKS:
   - Track heap usage over time: xPortGetFreeHeapSize()
   - Log all alloc/free pairs
   - Prefer static allocation

TOOLS:
- Tracealyzer: Visual timeline of task execution
- SystemView: Segger's trace tool
- Logic analyzer: See actual timing on GPIOs
- RTOS-aware debugger: See task states, queues, etc.

=== CODE EXAMPLE ===
*/

// DEBUG TASK - Monitor system health
void debug_monitor_task(void *pvParameters) {
    while(1) {
        // Check heap
        size_t free_heap = xPortGetFreeHeapSize();
        size_t min_heap = xPortGetMinimumEverFreeHeapSize();
        printf("Heap: %u free, %u min ever\n", free_heap, min_heap);

        // Check each task stack
        TaskStatus_t task_status[10];
        UBaseType_t num_tasks = uxTaskGetSystemState(task_status, 10, NULL);

        for (int i = 0; i < num_tasks; i++) {
            printf("Task %s: stack HWM = %u, state = %d\n",
                   task_status[i].pcTaskName,
                   task_status[i].usStackHighWaterMark,
                   task_status[i].eCurrentState);
        }

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

// Toggle GPIO to measure timing on oscilloscope
#define DEBUG_PIN_TASK_START  GPIO_PIN_0
#define DEBUG_PIN_ISR         GPIO_PIN_1

void critical_task(void *pvParameters) {
    while(1) {
        HAL_GPIO_WritePin(GPIOA, DEBUG_PIN_TASK_START, GPIO_PIN_SET);
        do_critical_work();
        HAL_GPIO_WritePin(GPIOA, DEBUG_PIN_TASK_START, GPIO_PIN_RESET);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Timeout-based deadlock detection
bool acquire_resources(void) {
    if (xSemaphoreTake(mutex_a, pdMS_TO_TICKS(1000)) != pdTRUE) {
        log_error("Timeout on mutex_a - possible deadlock");
        return false;
    }

    if (xSemaphoreTake(mutex_b, pdMS_TO_TICKS(1000)) != pdTRUE) {
        log_error("Timeout on mutex_b - possible deadlock");
        xSemaphoreGive(mutex_a);  // Release already-held mutex
        return false;
    }

    return true;
}

// Runtime statistics (requires configGENERATE_RUN_TIME_STATS = 1)
void print_runtime_stats(void) {
    char buffer[512];
    vTaskGetRunTimeStats(buffer);
    printf("Task run-time stats:\n%s\n", buffer);
    // Shows percentage of CPU time per task
}

/*
=== ANTICIPATED FOLLOW-UPS ===

Q1: "How do you debug a crash that only happens after hours of running?"
A: "1. Add watchdog with diagnostic dump on reset
    2. Store circular log in retained RAM - survives reset
    3. Add periodic health checks that detect drift toward failure
    4. Run accelerated stress tests: 10x event rate, temperature cycling
    5. Use trace tools that can capture last N events before crash"

Q2: "What's your approach to debugging race conditions?"
A: "1. Identify shared resources - global variables, hardware registers
    2. Add assertions: if (counter < 0) assert_failed()
    3. Stress test: run at max load, vary timing
    4. Code review: every shared access should have protection
    5. Try ThreadSanitizer if platform supports it"

Q3: "How do you measure actual task timing vs expected?"
A: "1. GPIO toggles + oscilloscope: see actual timing, jitter
    2. Timestamp logging: record tick count at key points
    3. configGENERATE_RUN_TIME_STATS: get CPU percentage per task
    4. Tracealyzer: visual timeline shows preemptions, delays"

Q4: "What if you can't use trace tools (no JTAG access)?"
A: "Fall back to:
    1. UART debug output (but adds timing perturbation)
    2. GPIO pins + logic analyzer
    3. Retained RAM diagnostics - write state, read after reset
    4. LED patterns for status indication
    5. Add diagnostic commands via CLI/UART"
*/


/*******************************************************************************
 * QUICK REFERENCE CARD
 * Print this out for last-minute review before interview
 ******************************************************************************/
/*
┌─────────────────────────────────────────────────────────────────────────────┐
│                    15 RTOS CONCEPTS - QUICK REFERENCE                       │
├─────────────────────────────────────────────────────────────────────────────┤
│ 1. MUTEX vs SEMAPHORE                                                       │
│    Mutex: Ownership + Priority Inheritance → Use for resource protection    │
│    Semaphore: No ownership, counting → Use for signaling                    │
├─────────────────────────────────────────────────────────────────────────────┤
│ 2. QUEUES                                                                   │
│    Thread-safe FIFO, blocks on empty/full → ISR-to-task communication       │
├─────────────────────────────────────────────────────────────────────────────┤
│ 3. SCHEDULING                                                               │
│    Preemptive: Higher priority runs immediately                             │
│    Cooperative: Tasks explicitly yield                                      │
├─────────────────────────────────────────────────────────────────────────────┤
│ 4. ISR RULES                                                                │
│    Keep short, no blocking, use FromISR variants, yield if needed           │
├─────────────────────────────────────────────────────────────────────────────┤
│ 5. TIMERS                                                                   │
│    Software timer: Callbacks in task context, not ISR                       │
│    vTaskDelayUntil: Precise periodic execution                              │
├─────────────────────────────────────────────────────────────────────────────┤
│ 6. WATCHDOG                                                                 │
│    Task watchdog: Each task heartbeats → Supervisor kicks HW watchdog       │
├─────────────────────────────────────────────────────────────────────────────┤
│ 7. PRIORITY INVERSION                                                       │
│    High waits for Low, but Medium runs → Solution: Priority Inheritance     │
├─────────────────────────────────────────────────────────────────────────────┤
│ 8. DEADLOCK                                                                 │
│    Tasks wait for each other forever → Solution: Lock ordering + timeouts   │
├─────────────────────────────────────────────────────────────────────────────┤
│ 9. RACE CONDITIONS                                                          │
│    Shared data without sync → Solution: Mutex, critical section, atomic     │
├─────────────────────────────────────────────────────────────────────────────┤
│ 10. STACK SIZING                                                            │
│    Measure with high-water mark, add 25% margin, enable overflow check      │
├─────────────────────────────────────────────────────────────────────────────┤
│ 11. MEMORY POOLS                                                            │
│    Fixed-size blocks → No fragmentation, deterministic allocation           │
├─────────────────────────────────────────────────────────────────────────────┤
│ 12. REAL-TIME                                                               │
│    Hard: Miss deadline = failure  |  Soft: Miss = degraded performance      │
├─────────────────────────────────────────────────────────────────────────────┤
│ 13. EVENT GROUPS                                                            │
│    Multiple bits, multiple waiters → Sync point & broadcast                 │
├─────────────────────────────────────────────────────────────────────────────┤
│ 14. TASK NOTIFICATIONS                                                      │
│    Lighter than semaphore, 45% faster, built into TCB                       │
├─────────────────────────────────────────────────────────────────────────────┤
│ 15. DEBUGGING                                                               │
│    Stack overflow hook, trace tools, GPIO + scope, runtime stats            │
└─────────────────────────────────────────────────────────────────────────────┘

MEMORY AIDS:
• Mutex = Bathroom key (you lock, you unlock)
• Semaphore = Ticket counter (anyone can take/give)
• Priority Inversion = VIP waiting behind regular, but another VIP cuts in
• Deadlock = Two people in narrow hallway refusing to step aside
• Event Group = Scoreboard (everyone can read, multiple can wait)
• Race Condition = Two people editing same doc, one overwrites the other
*/
