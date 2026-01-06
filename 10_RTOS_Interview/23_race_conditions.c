/*
 * ============================================================================
 * QUESTION 23: What are Race Conditions? How do you prevent them?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * Race conditions are one of the most common bugs in concurrent systems.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "A race condition occurs when two or more threads access shared data
 * concurrently, and at least one of them is writing. The outcome depends
 * on the unpredictable order of execution, leading to bugs that are
 * intermittent and hard to reproduce. Prevention methods include: mutexes
 * (for task-to-task), critical sections/interrupt disable (for ISR-safe),
 * atomic operations for single variables, and message queues to avoid
 * sharing altogether. The key principle is: protect ALL accesses to
 * shared data, both reads AND writes."
 *
 * ============================================================================
 * VISUALIZATION: Race Condition
 * ============================================================================
 *
 *   EXAMPLE: Incrementing a counter (counter = counter + 1)
 *
 *   This single C statement is actually THREE operations:
 *   1. READ counter from memory
 *   2. ADD 1 to value
 *   3. WRITE result back to memory
 *
 *
 *   RACE CONDITION SCENARIO:
 *
 *   Shared: counter = 10
 *
 *   Task A                    Task B
 *   ------                    ------
 *   READ counter (10)
 *                             READ counter (10)  <- Same value!
 *   ADD 1 (=11)
 *                             ADD 1 (=11)        <- Same result!
 *   WRITE counter (11)
 *                             WRITE counter (11) <- Lost update!
 *
 *   Expected: counter = 12
 *   Actual:   counter = 11   <- BUG!
 *
 *
 *   TIME DIAGRAM:
 *
 *   Time:  t0    t1    t2    t3    t4    t5
 *          |     |     |     |     |     |
 *   A:     [READ]     [ADD] [WRITE]
 *   B:           [READ]     [ADD] [WRITE]
 *                 ^                  ^
 *                 |                  |
 *             B reads before    B overwrites A's write
 *             A writes
 *
 * ============================================================================
 * TYPES OF RACE CONDITIONS
 * ============================================================================
 *
 *   1. READ-MODIFY-WRITE RACE
 *   -------------------------
 *   counter++;
 *   flags |= NEW_FLAG;
 *   value *= 2;
 *
 *   Solution: Atomic operations or mutex
 *
 *
 *   2. CHECK-THEN-ACT RACE
 *   ----------------------
 *   if (buffer_empty) {        // Check
 *       write_to_buffer();      // Act
 *   }
 *
 *   Between check and act, another task might fill buffer!
 *
 *   Solution: Combine check and act atomically
 *
 *
 *   3. MULTI-VARIABLE RACE
 *   ----------------------
 *   // Task A                 // Task B
 *   sensor.x = read_x();      value = sensor.x;  // Old x?
 *   sensor.y = read_y();      value += sensor.y; // New y?
 *   sensor.z = read_z();      value += sensor.z; // New z?
 *
 *   Task B reads inconsistent data - mix of old and new values!
 *
 *   Solution: Protect entire structure update
 *
 *
 *   4. ISR-TASK RACE
 *   ----------------
 *   // ISR                    // Task
 *   rx_count++;               if (rx_count > 0) {
 *                                 process_byte();
 *                                 rx_count--;
 *                             }
 *
 *   ISR can modify between task's check and decrement!
 *
 *   Solution: Disable interrupts or use atomic
 *
 * ============================================================================
 * CODE EXAMPLE: Race Conditions
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// ============================================================================
// EXAMPLE 1: Unprotected Shared Variable (BAD!)
// ============================================================================

volatile uint32_t shared_counter = 0;

// BUGGY - Race condition!
void task_increment_bad(void *pvParameters) {
    for (int i = 0; i < 1000; i++) {
        shared_counter++;  // NOT ATOMIC! Read-modify-write race!
    }
}

// After 2 tasks run: Expected 2000, might get 1500-1999


// ============================================================================
// EXAMPLE 2: Protected with Mutex (GOOD)
// ============================================================================

SemaphoreHandle_t counter_mutex;

void task_increment_mutex(void *pvParameters) {
    for (int i = 0; i < 1000; i++) {
        xSemaphoreTake(counter_mutex, portMAX_DELAY);
        shared_counter++;  // Protected
        xSemaphoreGive(counter_mutex);
    }
}


// ============================================================================
// EXAMPLE 3: Protected with Critical Section (ISR-safe)
// ============================================================================

void task_increment_critical(void *pvParameters) {
    for (int i = 0; i < 1000; i++) {
        taskENTER_CRITICAL();
        shared_counter++;  // Protected from tasks AND ISRs
        taskEXIT_CRITICAL();
    }
}


// ============================================================================
// EXAMPLE 4: Check-Then-Act Race (BAD!)
// ============================================================================

typedef struct {
    uint8_t data[256];
    volatile uint8_t head;
    volatile uint8_t tail;
} RingBuffer_t;

RingBuffer_t rx_buffer;

// BUGGY - Race between is_empty check and get!
bool buffer_get_bad(uint8_t *data) {
    if (rx_buffer.head != rx_buffer.tail) {  // Check
        // Another task could consume data HERE!
        *data = rx_buffer.data[rx_buffer.tail];  // Act
        rx_buffer.tail++;
        return true;
    }
    return false;
}


// FIXED - Atomic check-and-act
bool buffer_get_fixed(uint8_t *data) {
    bool success = false;

    taskENTER_CRITICAL();
    if (rx_buffer.head != rx_buffer.tail) {
        *data = rx_buffer.data[rx_buffer.tail];
        rx_buffer.tail++;
        success = true;
    }
    taskEXIT_CRITICAL();

    return success;
}


// ============================================================================
// EXAMPLE 5: Multi-Variable Race (BAD!)
// ============================================================================

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
    uint32_t timestamp;
} SensorReading_t;

volatile SensorReading_t sensor;

// ISR updates sensor data
void sensor_isr(void) {
    // If task reads DURING this update, gets inconsistent data!
    sensor.x = read_x();
    sensor.y = read_y();  // Task could read here - x is new, y/z are old!
    sensor.z = read_z();
    sensor.timestamp = get_tick();
}

// Task reads sensor - BUGGY
SensorReading_t read_sensor_bad(void) {
    SensorReading_t copy;
    copy.x = sensor.x;        // New value
    // ISR fires here!
    copy.y = sensor.y;        // Newer value
    copy.z = sensor.z;        // Newer value
    copy.timestamp = sensor.timestamp;  // Wrong timestamp!
    return copy;  // Inconsistent data!
}


// FIXED - Atomic read with critical section
SensorReading_t read_sensor_fixed(void) {
    SensorReading_t copy;

    taskENTER_CRITICAL();
    copy = sensor;  // Atomic copy of entire structure
    taskEXIT_CRITICAL();

    return copy;
}
*/

/*
 * ============================================================================
 * ATOMIC OPERATIONS
 * ============================================================================
 *
 * For simple variables, atomic operations avoid races without locking:
 *
 * ARM Cortex-M provides:
 * - LDREX/STREX for atomic read-modify-write
 * - DMB/DSB for memory barriers
 *
 * FreeRTOS doesn't have built-in atomic API, but you can use:
 * - GCC built-ins: __atomic_add_fetch(), __atomic_compare_exchange_n()
 * - Platform-specific: CMSIS __LDREXW(), __STREXW()
 *
 * Zephyr provides:
 * - atomic_inc(), atomic_dec()
 * - atomic_set(), atomic_get()
 * - atomic_cas() (compare-and-swap)
 */

/*
// Zephyr Atomic Example
#include <zephyr/sys/atomic.h>

atomic_t counter = ATOMIC_INIT(0);

void increment_atomic(void) {
    atomic_inc(&counter);  // Thread-safe without lock!
}

void decrement_atomic(void) {
    atomic_dec(&counter);
}

// Atomic test-and-set
void claim_resource(void) {
    atomic_t resource_claimed = ATOMIC_INIT(0);

    if (atomic_cas(&resource_claimed, 0, 1)) {
        // Successfully claimed - was 0, now 1
        use_resource();
    } else {
        // Already claimed by someone else
    }
}


// GCC Atomic Built-ins (works in FreeRTOS)
volatile int32_t atomic_counter = 0;

void gcc_atomic_increment(void) {
    __atomic_add_fetch(&atomic_counter, 1, __ATOMIC_SEQ_CST);
}

int32_t gcc_atomic_decrement_and_test(void) {
    return __atomic_sub_fetch(&atomic_counter, 1, __ATOMIC_SEQ_CST);
}
*/

/*
 * ============================================================================
 * PREVENTION STRATEGIES
 * ============================================================================
 *
 *   +-------------------+---------------------+---------------------------+
 *   | Method            | Use When            | Pros/Cons                 |
 *   +-------------------+---------------------+---------------------------+
 *   | Mutex             | Task-to-task sync   | + Sleep-wait              |
 *   |                   | Longer sections     | - Not ISR-safe            |
 *   |                   |                     | - Overhead                |
 *   +-------------------+---------------------+---------------------------+
 *   | Critical Section  | ISR-to-task sync    | + ISR-safe                |
 *   |                   | Very short sections | - Blocks everything       |
 *   |                   |                     | - Must be short           |
 *   +-------------------+---------------------+---------------------------+
 *   | Atomic Operations | Single variables    | + Fastest                 |
 *   |                   | Counters, flags     | + ISR-safe                |
 *   |                   |                     | - Limited operations      |
 *   +-------------------+---------------------+---------------------------+
 *   | Message Queue     | Data passing        | + No shared memory        |
 *   |                   | Producer-consumer   | + Natural synchronization |
 *   |                   |                     | - Copy overhead           |
 *   +-------------------+---------------------+---------------------------+
 *   | Double Buffering  | Sensor data         | + Lock-free               |
 *   |                   | Video buffers       | + Consistent data         |
 *   |                   |                     | - 2x memory               |
 *   +-------------------+---------------------+---------------------------+
 *
 * ============================================================================
 * DOUBLE BUFFERING EXAMPLE
 * ============================================================================
 */

/*
// Lock-free double buffering
typedef struct {
    int16_t x, y, z;
    uint32_t timestamp;
} SensorData_t;

SensorData_t buffers[2];
volatile uint8_t write_index = 0;  // ISR writes here
volatile uint8_t read_index = 1;   // Tasks read from here

// ISR writes to write buffer
void sensor_isr_double_buffer(void) {
    SensorData_t *buf = &buffers[write_index];
    buf->x = read_x();
    buf->y = read_y();
    buf->z = read_z();
    buf->timestamp = get_tick();

    // Atomically swap buffers
    uint8_t temp = write_index;
    write_index = read_index;
    read_index = temp;
}

// Task reads from read buffer (always consistent!)
SensorData_t read_sensor_double_buffer(void) {
    return buffers[read_index];  // Always reads complete, consistent data
}
*/

/*
 * ============================================================================
 * DEBUGGING RACE CONDITIONS
 * ============================================================================
 *
 * Race conditions are HARD to debug because:
 * 1. Timing-dependent - may not occur in debug build
 * 2. Heisenberg - adding printf/breakpoints changes timing
 * 3. Intermittent - may work 99.99% of the time
 *
 * DEBUGGING TECHNIQUES:
 *
 * 1. CODE REVIEW
 *    - Look for shared variables without protection
 *    - Check all accesses, not just writes
 *
 * 2. STATIC ANALYSIS
 *    - Tools like ThreadSanitizer (TSAN)
 *    - Clang thread safety annotations
 *
 * 3. STRESS TESTING
 *    - Run at high load to increase race probability
 *    - Add artificial delays to widen race windows
 *
 * 4. LOGGING WITH TIMESTAMPS
 *    - Log all accesses to suspected variables
 *    - Review sequence of operations
 *
 * 5. ASSERTIONS
 *    - Add consistency checks
 *    - configASSERT() in FreeRTOS
 *
 * ============================================================================
 * COMMON MISTAKES
 * ============================================================================
 *
 * MISTAKE 1: Protecting only writes, not reads
 *
 *   // WRONG: Read is unprotected!
 *   xSemaphoreTake(mutex, portMAX_DELAY);
 *   shared_data = new_value;
 *   xSemaphoreGive(mutex);
 *
 *   // Elsewhere, unprotected read:
 *   local = shared_data;  // Race with above write!
 *
 *   // CORRECT: Protect ALL accesses
 *   xSemaphoreTake(mutex, portMAX_DELAY);
 *   local = shared_data;  // Protected read
 *   xSemaphoreGive(mutex);
 *
 *
 * MISTAKE 2: Assuming single statement is atomic
 *
 *   // WRONG: Assumes counter++ is atomic
 *   // On 32-bit ARM, 64-bit operations are NOT atomic!
 *   volatile uint64_t counter;
 *   counter++;  // NOT atomic on 32-bit CPU!
 *
 *
 * MISTAKE 3: Relying on volatile
 *
 *   // WRONG: volatile prevents compiler optimization but NOT race!
 *   volatile int flag = 0;
 *
 *   // Task A
 *   flag = 1;
 *
 *   // Task B
 *   while (!flag) { }  // May still have race with flag++
 *
 *
 * MISTAKE 4: Wrong scope of protection
 *
 *   // WRONG: Lock released between related operations!
 *   xSemaphoreTake(mutex, portMAX_DELAY);
 *   value = shared_buffer[index];
 *   xSemaphoreGive(mutex);
 *
 *   xSemaphoreTake(mutex, portMAX_DELAY);
 *   shared_buffer[index] = process(value);  // Index might have changed!
 *   xSemaphoreGive(mutex);
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. DEFINE CLEARLY
 *    "Concurrent access where at least one is a write, outcome depends on order"
 *
 * 2. GIVE CONCRETE EXAMPLE
 *    "counter++ is read-modify-write, not atomic, can lose updates"
 *
 * 3. LIST PREVENTION METHODS
 *    "Mutex, critical section, atomic operations, or avoid sharing"
 *
 * 4. MENTION DEBUGGING DIFFICULTY
 *    "Hard to reproduce, timing-dependent, disappear with printf"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "Is reading a 32-bit integer atomic?"
 * A: On 32-bit ARM with aligned access, yes. But 64-bit values are NOT atomic.
 *    And read-modify-write is never atomic regardless of size.
 *
 * Q: "Does volatile prevent race conditions?"
 * A: No! Volatile only prevents compiler optimization. It doesn't provide
 *    atomicity or synchronization between threads.
 *
 * Q: "Can you have race condition in single-threaded program?"
 * A: Yes! Between main code and ISR. ISR is like another thread.
 *
 * Q: "What's the difference between race condition and data race?"
 * A: Data race: technical definition, multiple unsynchronized accesses.
 *    Race condition: broader, any timing-dependent behavior affecting correctness.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 23 ===\n\n");
    printf("Q: What are Race Conditions?\n\n");
    printf("RACE CONDITION:\n");
    printf("- Multiple threads access shared data\n");
    printf("- At least one is writing\n");
    printf("- Outcome depends on execution order\n");
    printf("- Leads to intermittent, hard-to-find bugs\n\n");
    printf("COMMON TYPES:\n");
    printf("1. Read-Modify-Write: counter++\n");
    printf("2. Check-Then-Act: if(empty) write()\n");
    printf("3. Multi-Variable: struct partial update\n");
    printf("4. ISR-Task: interrupt modifies while task reads\n\n");
    printf("PREVENTION:\n");
    printf("- Mutex: Task-to-task, can sleep\n");
    printf("- Critical Section: ISR-safe, short duration\n");
    printf("- Atomic Operations: Single variables\n");
    printf("- Message Queue: Avoid sharing\n");
    printf("- Double Buffering: Lock-free for producers/consumers\n\n");
    printf("KEY PRINCIPLE:\n");
    printf("Protect ALL accesses (reads AND writes) to shared data!\n");
    return 0;
}
