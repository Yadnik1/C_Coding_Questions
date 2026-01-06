/*
 * ============================================================================
 * QUESTION 21: What are Spinlocks? When should you use them?
 * ============================================================================
 *
 * DIFFICULTY: Medium-Hard | FREQUENCY: Medium | IMPORTANCE: High (SMP)
 *
 * Spinlocks are critical for multi-core RTOS systems.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "A spinlock is a busy-wait synchronization primitive where a thread
 * continuously polls (spins) checking if the lock is available. Unlike
 * mutexes which put the waiting thread to sleep, spinlocks actively consume
 * CPU cycles while waiting. They're used when: (1) the lock is held for very
 * short duration, (2) in SMP systems where threads on different cores need
 * synchronization, (3) in contexts where sleeping is not allowed (ISRs).
 * Spinlocks are wasteful on single-core systems but essential for multi-core.
 * In Zephyr, k_spin_lock is the primary API."
 *
 * ============================================================================
 * VISUALIZATION: Spinlock vs Mutex
 * ============================================================================
 *
 *   MUTEX (Sleep-Wait):
 *   -------------------
 *
 *   Thread A              Scheduler              Thread B
 *   --------              ---------              --------
 *   Lock mutex
 *   [Holds lock]
 *                                                Try lock -> BLOCKED
 *                                                [Sleeps - no CPU usage]
 *                         Runs other threads
 *                         ...
 *   Release mutex
 *                         Wakes Thread B
 *                                                [Wakes up]
 *                                                Acquires lock
 *
 *   CPU Usage while blocked: 0%
 *   Context switches: 2 (block + wake)
 *
 *
 *   SPINLOCK (Busy-Wait):
 *   --------------------
 *
 *   Thread A (Core 0)                Thread B (Core 1)
 *   -----------------                -----------------
 *   Lock spinlock
 *   [Holds lock]
 *                                    Try lock -> SPINNING
 *                                    while(locked) { }  <- Burns CPU!
 *                                    while(locked) { }
 *   [Short operation]               while(locked) { }
 *   Release spinlock
 *                                    [Acquires lock immediately!]
 *
 *   CPU Usage while spinning: 100% on that core
 *   Context switches: 0 (no scheduler involvement)
 *
 * ============================================================================
 * WHY SPINLOCKS FOR SMP?
 * ============================================================================
 *
 *   SINGLE CORE:
 *   ------------
 *   Spinlock is wasteful - while spinning, the thread holding the lock
 *   can't run (same CPU), so we spin forever!
 *   Only safe if used to synchronize with ISRs (which preempt).
 *
 *   MULTI-CORE (SMP):
 *   -----------------
 *   While Core 1 spins, Core 0 can continue executing and release the lock.
 *   If lock is held briefly, spinning is faster than sleep+wake overhead.
 *
 *
 *   VISUALIZATION:
 *
 *   Single Core:                     Multi-Core (SMP):
 *   +--------+                       +--------+  +--------+
 *   | Core 0 |                       | Core 0 |  | Core 1 |
 *   +--------+                       +--------+  +--------+
 *       |                                |           |
 *   [Task A locks]                   [Task A]    [Task B]
 *   [Task B tries]                   [holds ]    [spins  ]
 *   [B spins...]  <- BLOCKED!        [lock  ]    [waiting]
 *   [A can't run!]                   [release]   [acquire]
 *   [DEADLOCK!]                      [       ]   [has lock]
 *
 *   On single core, Task A can't release because Task B is spinning!
 *   On multi-core, Task A continues on Core 0 while B spins on Core 1.
 *
 * ============================================================================
 * CODE EXAMPLE: Spinlock Implementation
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * ============================================================================
 * BASIC SPINLOCK CONCEPT (Educational - Don't use in production!)
 * ============================================================================
 */

// Simple spinlock structure
typedef struct {
    volatile int locked;
} simple_spinlock_t;

// WRONG WAY - Race condition!
void bad_spin_lock(simple_spinlock_t *lock) {
    while (lock->locked) {
        // Spin
    }
    lock->locked = 1;  // RACE! Another thread could set between check and set
}

// CORRECT WAY - Using atomic operation
// (Requires atomic instruction like test-and-set)
/*
void atomic_spin_lock(simple_spinlock_t *lock) {
    while (atomic_test_and_set(&lock->locked) == 1) {
        // Spin until we successfully set the lock
    }
}

void spin_unlock(simple_spinlock_t *lock) {
    atomic_store(&lock->locked, 0);
}
*/

/*
 * ============================================================================
 * ZEPHYR SPINLOCK API
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>
#include <zephyr/spinlock.h>

// Define spinlock
static struct k_spinlock my_lock;

// Shared data
static int shared_counter = 0;

void thread_function(void) {
    k_spinlock_key_t key;

    // Lock - disables interrupts AND acquires spinlock
    key = k_spin_lock(&my_lock);
    {
        // Critical section
        shared_counter++;
    }
    // Unlock - releases spinlock AND restores interrupt state
    k_spin_unlock(&my_lock, key);
}


// ============================================================================
// ISR-SAFE SPINLOCK USAGE
// ============================================================================

void some_isr(void) {
    k_spinlock_key_t key;

    key = k_spin_lock(&my_lock);
    {
        // Safe to access shared_counter from ISR
        shared_counter = 0;
    }
    k_spin_unlock(&my_lock, key);
}


// ============================================================================
// PRACTICAL EXAMPLE: Multi-Core Ring Buffer
// ============================================================================

#define BUFFER_SIZE 64

typedef struct {
    uint8_t data[BUFFER_SIZE];
    uint32_t head;
    uint32_t tail;
    struct k_spinlock lock;
} RingBuffer_t;

RingBuffer_t ring;

bool ring_buffer_push(RingBuffer_t *rb, uint8_t value) {
    k_spinlock_key_t key;
    bool success = false;

    key = k_spin_lock(&rb->lock);
    {
        uint32_t next_head = (rb->head + 1) % BUFFER_SIZE;
        if (next_head != rb->tail) {
            rb->data[rb->head] = value;
            rb->head = next_head;
            success = true;
        }
    }
    k_spin_unlock(&rb->lock, key);

    return success;
}

bool ring_buffer_pop(RingBuffer_t *rb, uint8_t *value) {
    k_spinlock_key_t key;
    bool success = false;

    key = k_spin_lock(&rb->lock);
    {
        if (rb->head != rb->tail) {
            *value = rb->data[rb->tail];
            rb->tail = (rb->tail + 1) % BUFFER_SIZE;
            success = true;
        }
    }
    k_spin_unlock(&rb->lock, key);

    return success;
}
*/

/*
 * ============================================================================
 * ARM SPINLOCK ASSEMBLY (Educational)
 * ============================================================================
 *
 * ARMv7 and later have LDREX/STREX for atomic operations:
 *
 * spin_lock:
 *     mov     r1, #1
 * 1:
 *     ldrex   r2, [r0]       ; Load exclusive
 *     cmp     r2, #0         ; Is it unlocked?
 *     wfene                  ; Wait for event if locked (power save)
 *     bne     1b             ; If locked, try again
 *     strex   r2, r1, [r0]   ; Try to store 1 (lock)
 *     cmp     r2, #0         ; Did store succeed?
 *     bne     1b             ; If not, try again
 *     dmb                    ; Memory barrier
 *     bx      lr
 *
 * spin_unlock:
 *     mov     r1, #0
 *     dmb                    ; Memory barrier
 *     str     r1, [r0]       ; Store 0 (unlock)
 *     dsb                    ; Data synchronization barrier
 *     sev                    ; Send event (wake waiting cores)
 *     bx      lr
 *
 * ============================================================================
 * SPINLOCK TYPES
 * ============================================================================
 *
 * 1. BASIC SPINLOCK (Test-and-Set)
 *    - Simple but unfair (starvation possible)
 *
 * 2. TICKET SPINLOCK
 *    - Fair - threads served in order
 *    - Each thread gets a "ticket number"
 *
 * 3. MCS SPINLOCK
 *    - Scalable for many cores
 *    - Each core spins on its own cache line
 *
 * 4. READ-WRITE SPINLOCK
 *    - Multiple readers OR one writer
 *    - Good for read-heavy workloads
 *
 * ============================================================================
 * SPINLOCK vs MUTEX COMPARISON
 * ============================================================================
 *
 *   +---------------------+------------------------+------------------------+
 *   | Aspect              | Spinlock               | Mutex                  |
 *   +---------------------+------------------------+------------------------+
 *   | Wait mechanism      | Busy-wait (spinning)   | Sleep-wait (blocked)   |
 *   | CPU usage           | 100% while waiting     | 0% while waiting       |
 *   | Context switches    | None                   | Yes (expensive)        |
 *   | ISR safe            | YES                    | NO                     |
 *   | Good for duration   | Very short (< 1us)     | Longer (> 10us)        |
 *   | Multi-core          | Essential              | Works but overhead     |
 *   | Single-core         | Wasteful               | Efficient              |
 *   | Priority inversion  | N/A (no blocking)      | Yes (need inheritance) |
 *   +---------------------+------------------------+------------------------+
 *
 * ============================================================================
 * WHEN TO USE SPINLOCKS
 * ============================================================================
 *
 * USE SPINLOCKS WHEN:
 *
 * 1. Lock held for VERY SHORT time (< 1-10 us)
 *    - Context switch overhead > spinning time
 *
 * 2. In ISR context
 *    - Cannot sleep in ISR!
 *
 * 3. Multi-core systems
 *    - Other core can release lock while spinning
 *
 * 4. Low-level kernel code
 *    - Before scheduler is running
 *
 *
 * DO NOT USE SPINLOCKS WHEN:
 *
 * 1. Lock held for long time
 *    - Wastes CPU cycles
 *
 * 2. Single-core system (usually)
 *    - Unless synchronizing with ISR
 *
 * 3. Sleeping is acceptable
 *    - Use mutex instead
 *
 * ============================================================================
 * FREERTOS AND SPINLOCKS
 * ============================================================================
 *
 * FreeRTOS traditionally targets single-core MCUs, so it doesn't have
 * explicit spinlock API. Instead:
 *
 * - taskENTER_CRITICAL() disables interrupts (single-core equivalent)
 * - For FreeRTOS SMP port, critical sections use spinlocks internally
 *
 * FreeRTOS SMP (Amazon fork) adds:
 *   portGET_SPINLOCK()
 *   portRELEASE_SPINLOCK()
 *
 * ============================================================================
 * COMMON MISTAKES
 * ============================================================================
 *
 * MISTAKE 1: Long critical sections with spinlock
 *
 *   k_spinlock_key_t key = k_spin_lock(&lock);
 *   process_1000_items();  // WRONG! Too long, wastes CPU!
 *   k_spin_unlock(&lock, key);
 *
 *
 * MISTAKE 2: Sleeping while holding spinlock
 *
 *   key = k_spin_lock(&lock);
 *   k_sleep(K_MSEC(100));  // WRONG! Never sleep with spinlock!
 *   k_spin_unlock(&lock, key);
 *
 *
 * MISTAKE 3: Using spinlock on single-core for task sync
 *
 *   // On single-core, if Task A holds lock and Task B spins,
 *   // Task A never gets CPU time to release! DEADLOCK!
 *
 *
 * MISTAKE 4: Forgetting to release
 *
 *   key = k_spin_lock(&lock);
 *   if (error) {
 *       return;  // WRONG! Lock not released!
 *   }
 *   k_spin_unlock(&lock, key);
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN BUSY-WAIT
 *    "Spinlock spins in a loop checking lock, consumes CPU"
 *
 * 2. CONTRAST WITH MUTEX
 *    "Mutex sleeps while waiting, spinlock burns CPU"
 *
 * 3. DISCUSS SMP RELEVANCE
 *    "Essential for multi-core, wasteful for single-core"
 *
 * 4. MENTION USE CASES
 *    "Very short critical sections, ISR synchronization, kernel code"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "Can you have deadlock with spinlocks?"
 * A: Yes! If Thread A holds lock 1 and waits for lock 2, while Thread B
 *    holds lock 2 and waits for lock 1 - classic deadlock even with spinlocks.
 *
 * Q: "What's a ticket spinlock?"
 * A: Fair spinlock where each waiter gets a ticket number. Lock is granted
 *    in ticket order, preventing starvation.
 *
 * Q: "How does k_spin_lock differ from irq_lock in Zephyr?"
 * A: irq_lock just disables interrupts (single-core protection).
 *    k_spin_lock does irq_lock PLUS atomic spinlock (SMP-safe).
 *
 * Q: "What's the memory barrier in spinlock for?"
 * A: Ensures all memory operations before unlock are visible to other cores
 *    before the lock appears free. Prevents stale data reads.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 21 ===\n\n");
    printf("Q: What are Spinlocks?\n\n");
    printf("SPINLOCK:\n");
    printf("- Busy-wait synchronization primitive\n");
    printf("- Thread spins (loops) checking if lock is free\n");
    printf("- Does NOT sleep - consumes 100%% CPU while waiting\n\n");
    printf("WHEN TO USE:\n");
    printf("- Very short critical sections (< 1-10us)\n");
    printf("- Multi-core (SMP) systems\n");
    printf("- ISR context (cannot sleep)\n");
    printf("- Low-level kernel code\n\n");
    printf("WHEN NOT TO USE:\n");
    printf("- Long critical sections\n");
    printf("- Single-core task synchronization\n");
    printf("- When sleeping is acceptable\n\n");
    printf("SPINLOCK vs MUTEX:\n");
    printf("- Spinlock: Busy-wait, 0 context switches, ISR-safe\n");
    printf("- Mutex: Sleep-wait, context switches, not ISR-safe\n\n");
    printf("ZEPHYR API:\n");
    printf("- k_spin_lock(&lock) - lock and disable IRQ\n");
    printf("- k_spin_unlock(&lock, key) - unlock and restore IRQ\n");
    return 0;
}
