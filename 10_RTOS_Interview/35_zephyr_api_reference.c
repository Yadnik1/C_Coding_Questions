/*
 * ============================================================================
 * QUESTION 35: Zephyr RTOS API Quick Reference (Interview Cheat Sheet)
 * ============================================================================
 *
 * DIFFICULTY: Reference | FREQUENCY: High | IMPORTANCE: Critical
 *
 * Quick reference for commonly asked Zephyr APIs in interviews.
 *
 * ============================================================================
 * THREAD MANAGEMENT
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
#include <zephyr/kernel.h>

// ============================================================================
// THREAD CREATION
// ============================================================================

// Static definition (compile-time) - PREFERRED in Zephyr
K_THREAD_DEFINE(
    thread_name,        // Thread identifier
    stack_size,         // Stack size in BYTES
    entry_function,     // Entry point
    param1, param2, param3,  // Three parameters
    priority,           // Priority (lower number = higher priority!)
    options,            // K_ESSENTIAL, K_USER, etc. (usually 0)
    delay               // Start delay in ms (0 = immediate)
);

// Stack definition for runtime creation
K_THREAD_STACK_DEFINE(my_stack, 1024);
struct k_thread my_thread;

// Runtime creation
k_tid_t k_thread_create(
    struct k_thread *new_thread,
    k_thread_stack_t *stack,
    size_t stack_size,
    k_thread_entry_t entry,
    void *p1, void *p2, void *p3,
    int prio,
    uint32_t options,
    k_timeout_t delay
);

// EXAMPLE:
void my_thread_func(void *p1, void *p2, void *p3) {
    int id = (int)p1;
    while (1) {
        do_work(id);
        k_sleep(K_MSEC(100));
    }
}

// Static definition (preferred)
K_THREAD_DEFINE(worker1, 1024, my_thread_func, (void*)1, NULL, NULL, 5, 0, 0);

// Runtime creation
void create_thread_runtime(void) {
    k_tid_t tid = k_thread_create(&my_thread, my_stack,
        K_THREAD_STACK_SIZEOF(my_stack),
        my_thread_func, (void*)2, NULL, NULL,
        5, 0, K_NO_WAIT);

    k_thread_name_set(tid, "worker2");
}


// ============================================================================
// THREAD CONTROL
// ============================================================================

void k_sleep(k_timeout_t timeout);
    // Sleep for duration: K_MSEC(100), K_SECONDS(1), K_FOREVER

int32_t k_usleep(int32_t us);
    // Microsecond sleep

void k_busy_wait(uint32_t usec_to_wait);
    // Busy-wait (doesn't yield)

void k_yield(void);
    // Yield to other threads of same priority

void k_thread_suspend(k_tid_t thread);
    // Suspend thread

void k_thread_resume(k_tid_t thread);
    // Resume thread

void k_thread_abort(k_tid_t thread);
    // Terminate thread

int k_thread_priority_get(k_tid_t thread);
    // Get priority

void k_thread_priority_set(k_tid_t thread, int prio);
    // Set priority


// ============================================================================
// TIMING
// ============================================================================

int64_t k_uptime_get(void);
    // Milliseconds since boot

uint32_t k_uptime_get_32(void);
    // 32-bit version

int64_t k_uptime_ticks(void);
    // Ticks since boot

uint32_t k_cycle_get_32(void);
    // CPU cycles (high resolution)


// TIMEOUT MACROS:
K_NO_WAIT           // Don't wait (return immediately)
K_FOREVER           // Wait forever
K_MSEC(ms)          // Milliseconds
K_SECONDS(s)        // Seconds
K_MINUTES(m)        // Minutes
K_HOURS(h)          // Hours
*/

/*
 * ============================================================================
 * MESSAGE QUEUE (k_msgq)
 * ============================================================================
 */

/*
// Static definition (preferred)
K_MSGQ_DEFINE(my_msgq, msg_size, max_msgs, alignment);

// Runtime initialization
struct k_msgq my_queue;
char my_buffer[10 * sizeof(DataType)];

k_msgq_init(&my_queue, my_buffer, sizeof(DataType), 10);

// Send message (copy data)
int k_msgq_put(struct k_msgq *msgq, const void *data, k_timeout_t timeout);

// Receive message
int k_msgq_get(struct k_msgq *msgq, void *data, k_timeout_t timeout);

// Peek (don't remove)
int k_msgq_peek(struct k_msgq *msgq, void *data);

// Purge all messages
void k_msgq_purge(struct k_msgq *msgq);

// Query
uint32_t k_msgq_num_used_get(struct k_msgq *msgq);
uint32_t k_msgq_num_free_get(struct k_msgq *msgq);


// EXAMPLE:
typedef struct {
    uint32_t timestamp;
    int16_t value;
} SensorMsg_t;

K_MSGQ_DEFINE(sensor_queue, sizeof(SensorMsg_t), 10, 4);

void producer(void) {
    SensorMsg_t msg = {.timestamp = k_uptime_get_32(), .value = 42};
    k_msgq_put(&sensor_queue, &msg, K_FOREVER);
}

void consumer(void) {
    SensorMsg_t msg;
    while (k_msgq_get(&sensor_queue, &msg, K_FOREVER) == 0) {
        process_message(&msg);
    }
}
*/

/*
 * ============================================================================
 * SEMAPHORE (k_sem)
 * ============================================================================
 */

/*
// Static definition
K_SEM_DEFINE(my_sem, initial_count, max_count);

// Runtime initialization
struct k_sem my_sem;
k_sem_init(&my_sem, initial_count, max_count);

// Take (decrement/wait)
int k_sem_take(struct k_sem *sem, k_timeout_t timeout);
    // Returns 0 on success, -EAGAIN on timeout

// Give (increment/signal)
void k_sem_give(struct k_sem *sem);

// Reset to initial
void k_sem_reset(struct k_sem *sem);

// Query
unsigned int k_sem_count_get(struct k_sem *sem);


// EXAMPLE - Binary semaphore for signaling:
K_SEM_DEFINE(data_ready, 0, 1);  // Initial 0, max 1

void isr_handler(void *arg) {
    k_sem_give(&data_ready);  // Signal
}

void thread_func(void) {
    while (1) {
        k_sem_take(&data_ready, K_FOREVER);  // Wait for signal
        process_data();
    }
}


// EXAMPLE - Counting semaphore for resource pool:
K_SEM_DEFINE(resource_pool, 5, 5);  // 5 resources

void use_resource(void) {
    k_sem_take(&resource_pool, K_FOREVER);  // Acquire
    // Use resource
    k_sem_give(&resource_pool);  // Release
}
*/

/*
 * ============================================================================
 * MUTEX (k_mutex)
 * ============================================================================
 */

/*
// Static definition
K_MUTEX_DEFINE(my_mutex);

// Runtime initialization
struct k_mutex my_mutex;
k_mutex_init(&my_mutex);

// Lock (acquire)
int k_mutex_lock(struct k_mutex *mutex, k_timeout_t timeout);

// Unlock (release)
void k_mutex_unlock(struct k_mutex *mutex);


// IMPORTANT: Zephyr k_mutex is ALWAYS recursive!
// Same thread can lock multiple times, must unlock same number

// EXAMPLE:
K_MUTEX_DEFINE(i2c_mutex);

void i2c_transfer(void) {
    k_mutex_lock(&i2c_mutex, K_FOREVER);
    // Only one thread can access I2C
    i2c_write(...);
    i2c_read(...);
    k_mutex_unlock(&i2c_mutex);
}

// Recursive example (works in Zephyr!)
void func_a(void) {
    k_mutex_lock(&i2c_mutex, K_FOREVER);  // count=1
    func_b();  // Nested lock OK
    k_mutex_unlock(&i2c_mutex);  // count=0
}

void func_b(void) {
    k_mutex_lock(&i2c_mutex, K_FOREVER);  // count=2 (same thread!)
    do_something();
    k_mutex_unlock(&i2c_mutex);  // count=1
}
*/

/*
 * ============================================================================
 * CONDITION VARIABLE (k_condvar)
 * ============================================================================
 */

/*
K_CONDVAR_DEFINE(my_condvar);

// Wait for condition (releases mutex while waiting)
int k_condvar_wait(struct k_condvar *condvar, struct k_mutex *mutex,
                   k_timeout_t timeout);

// Signal one waiting thread
void k_condvar_signal(struct k_condvar *condvar);

// Broadcast to all waiting threads
void k_condvar_broadcast(struct k_condvar *condvar);


// EXAMPLE:
K_MUTEX_DEFINE(data_mutex);
K_CONDVAR_DEFINE(data_ready);
bool data_available = false;

void producer(void) {
    k_mutex_lock(&data_mutex, K_FOREVER);
    produce_data();
    data_available = true;
    k_condvar_signal(&data_ready);
    k_mutex_unlock(&data_mutex);
}

void consumer(void) {
    k_mutex_lock(&data_mutex, K_FOREVER);
    while (!data_available) {
        k_condvar_wait(&data_ready, &data_mutex, K_FOREVER);
    }
    consume_data();
    data_available = false;
    k_mutex_unlock(&data_mutex);
}
*/

/*
 * ============================================================================
 * EVENT (k_event)
 * ============================================================================
 */

/*
K_EVENT_DEFINE(my_event);

// Set events (bits)
void k_event_set(struct k_event *event, uint32_t events);

// Post events (alias for set)
uint32_t k_event_post(struct k_event *event, uint32_t events);

// Wait for ANY of the events
uint32_t k_event_wait(struct k_event *event, uint32_t events,
                      bool reset, k_timeout_t timeout);

// Wait for ALL events
uint32_t k_event_wait_all(struct k_event *event, uint32_t events,
                          bool reset, k_timeout_t timeout);

// Clear events
void k_event_clear(struct k_event *event, uint32_t events);


// EXAMPLE:
#define EVENT_SENSOR  BIT(0)
#define EVENT_UART    BIT(1)
#define EVENT_TIMER   BIT(2)

K_EVENT_DEFINE(sys_events);

void sensor_isr(void) {
    k_event_post(&sys_events, EVENT_SENSOR);
}

void handler_thread(void) {
    while (1) {
        uint32_t events = k_event_wait(&sys_events,
            EVENT_SENSOR | EVENT_UART | EVENT_TIMER,
            true,  // Clear after wait
            K_FOREVER);

        if (events & EVENT_SENSOR) handle_sensor();
        if (events & EVENT_UART) handle_uart();
        if (events & EVENT_TIMER) handle_timer();
    }
}
*/

/*
 * ============================================================================
 * TIMER (k_timer)
 * ============================================================================
 */

/*
// Static definition
K_TIMER_DEFINE(my_timer, expiry_fn, stop_fn);

// Runtime initialization
struct k_timer my_timer;
k_timer_init(&my_timer, expiry_fn, stop_fn);

// Callback functions
void expiry_fn(struct k_timer *timer) {
    // Called when timer expires
    // Runs in ISR context!
}

void stop_fn(struct k_timer *timer) {
    // Called when timer is stopped (optional)
}

// Start timer
void k_timer_start(struct k_timer *timer, k_timeout_t duration,
                   k_timeout_t period);
    // duration: Initial delay
    // period: Repeat period (K_NO_WAIT for one-shot)

// Stop timer
void k_timer_stop(struct k_timer *timer);

// Query
uint32_t k_timer_remaining_get(struct k_timer *timer);
uint32_t k_timer_status_get(struct k_timer *timer);
void k_timer_status_sync(struct k_timer *timer);  // Wait for expiry

// User data
void k_timer_user_data_set(struct k_timer *timer, void *user_data);
void *k_timer_user_data_get(const struct k_timer *timer);


// EXAMPLE - Periodic timer:
void my_expiry(struct k_timer *t) {
    printk("Timer expired!\n");
}

K_TIMER_DEFINE(periodic_timer, my_expiry, NULL);

void start_timer(void) {
    // 100ms initial delay, 200ms period
    k_timer_start(&periodic_timer, K_MSEC(100), K_MSEC(200));
}


// EXAMPLE - One-shot timer:
void start_oneshot(void) {
    // 500ms delay, no repeat (K_NO_WAIT)
    k_timer_start(&my_timer, K_MSEC(500), K_NO_WAIT);
}
*/

/*
 * ============================================================================
 * WORK QUEUE (k_work)
 * ============================================================================
 */

/*
// Work item definition
struct k_work my_work;

void work_handler(struct k_work *work) {
    // Runs in system workqueue thread
    // Can use blocking APIs
}

k_work_init(&my_work, work_handler);

// Submit to system workqueue
int k_work_submit(struct k_work *work);

// Submit to specific workqueue
int k_work_submit_to_queue(struct k_work_q *queue, struct k_work *work);

// Check if work is pending
bool k_work_is_pending(const struct k_work *work);


// DELAYABLE WORK:
struct k_work_delayable my_dwork;

void dwork_handler(struct k_work *work) {
    // Handle delayed work
}

k_work_init_delayable(&my_dwork, dwork_handler);

// Schedule after delay
int k_work_schedule(struct k_work_delayable *dwork, k_timeout_t delay);

// Reschedule (cancel and schedule)
int k_work_reschedule(struct k_work_delayable *dwork, k_timeout_t delay);

// Cancel
bool k_work_cancel_delayable(struct k_work_delayable *dwork);


// EXAMPLE - Deferred ISR processing:
K_WORK_DEFINE(uart_work, uart_work_handler);

void uart_isr(void *arg) {
    // ISR context - keep short!
    k_work_submit(&uart_work);  // Defer to workqueue
}

void uart_work_handler(struct k_work *work) {
    // Runs in thread context
    // Can block, use APIs freely
    process_uart_data();
}
*/

/*
 * ============================================================================
 * MEMORY SLAB (k_mem_slab) - Fixed-size blocks
 * ============================================================================
 */

/*
K_MEM_SLAB_DEFINE(my_slab, block_size, num_blocks, alignment);

// Allocate block
int k_mem_slab_alloc(struct k_mem_slab *slab, void **mem, k_timeout_t timeout);

// Free block
void k_mem_slab_free(struct k_mem_slab *slab, void **mem);

// Query
uint32_t k_mem_slab_num_used_get(struct k_mem_slab *slab);
uint32_t k_mem_slab_num_free_get(struct k_mem_slab *slab);


// EXAMPLE:
K_MEM_SLAB_DEFINE(msg_slab, 64, 10, 4);  // 10 blocks of 64 bytes

void alloc_example(void) {
    void *block;
    if (k_mem_slab_alloc(&msg_slab, &block, K_NO_WAIT) == 0) {
        memset(block, 0, 64);
        // Use block...
        k_mem_slab_free(&msg_slab, &block);
    }
}
*/

/*
 * ============================================================================
 * CRITICAL SECTIONS
 * ============================================================================
 */

/*
// Disable/enable interrupts
unsigned int key = irq_lock();
// Critical section
irq_unlock(key);

// Spinlock (for SMP)
struct k_spinlock lock;
k_spinlock_key_t key = k_spin_lock(&lock);
// Critical section
k_spin_unlock(&lock, key);

// Scheduler lock
k_sched_lock();
// Threads can't preempt (ISRs still run)
k_sched_unlock();
*/

/*
 * ============================================================================
 * FREERTOS vs ZEPHYR QUICK COMPARISON
 * ============================================================================
 *
 *   +----------------------+-------------------------+------------------------+
 *   | Concept              | FreeRTOS                | Zephyr                 |
 *   +----------------------+-------------------------+------------------------+
 *   | Thread creation      | xTaskCreate()           | K_THREAD_DEFINE        |
 *   | Sleep                | vTaskDelay()            | k_sleep()              |
 *   | Queue                | xQueueCreate()          | K_MSGQ_DEFINE          |
 *   | Semaphore            | xSemaphoreCreateBinary  | K_SEM_DEFINE           |
 *   | Mutex                | xSemaphoreCreateMutex   | K_MUTEX_DEFINE         |
 *   | Event bits           | xEventGroupCreate       | K_EVENT_DEFINE         |
 *   | Timer                | xTimerCreate()          | K_TIMER_DEFINE         |
 *   | Priority             | Higher num = higher     | Lower num = higher!    |
 *   | Stack size           | Words                   | Bytes                  |
 *   | Recursive mutex      | Separate type           | Default behavior       |
 *   +----------------------+-------------------------+------------------------+
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 35 ===\n\n");
    printf("Zephyr RTOS API Quick Reference\n\n");
    printf("THREAD:\n");
    printf("  K_THREAD_DEFINE(), k_thread_create()\n");
    printf("  k_sleep(K_MSEC(100)), k_yield()\n\n");
    printf("MESSAGE QUEUE:\n");
    printf("  K_MSGQ_DEFINE(name, size, count, align)\n");
    printf("  k_msgq_put(), k_msgq_get()\n\n");
    printf("SEMAPHORE:\n");
    printf("  K_SEM_DEFINE(name, init, max)\n");
    printf("  k_sem_take(), k_sem_give()\n\n");
    printf("MUTEX:\n");
    printf("  K_MUTEX_DEFINE(name)\n");
    printf("  k_mutex_lock(), k_mutex_unlock()\n");
    printf("  Note: Always recursive in Zephyr!\n\n");
    printf("EVENT:\n");
    printf("  K_EVENT_DEFINE(name)\n");
    printf("  k_event_post(), k_event_wait()\n\n");
    printf("TIMER:\n");
    printf("  K_TIMER_DEFINE(name, expiry_fn, stop_fn)\n");
    printf("  k_timer_start(timer, delay, period)\n\n");
    printf("WORK QUEUE:\n");
    printf("  K_WORK_DEFINE(name, handler)\n");
    printf("  k_work_submit(), k_work_schedule()\n\n");
    printf("KEY DIFFERENCE FROM FREERTOS:\n");
    printf("  Priority: Lower number = Higher priority!\n");
    printf("  Stack: Specified in BYTES (not words)\n");
    return 0;
}
