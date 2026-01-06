/*
 * ============================================================================
 * QUESTION 16: What are Event Groups/Flags? How do they differ from Semaphores?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: Medium | IMPORTANCE: High
 *
 * Event groups are powerful for complex synchronization scenarios.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "Event groups are a synchronization primitive where multiple bits represent
 * different events. A task can wait for ANY or ALL of multiple bits to be set.
 * Unlike semaphores which are simple counters, event groups allow complex
 * conditions like 'wait until either sensor A is ready OR timeout occurred
 * AND initialization is complete'. They're ideal for state machines and
 * coordinating multiple events. FreeRTOS uses EventGroupHandle_t, Zephyr uses
 * k_event. Event groups don't have ownership like mutexes - any task can
 * set or clear bits."
 *
 * ============================================================================
 * VISUALIZATION: Event Group Bits
 * ============================================================================
 *
 *   Event Group (32-bit on FreeRTOS, 24 usable):
 *
 *   Bit: 7  6  5  4  3  2  1  0
 *        +--+--+--+--+--+--+--+--+
 *        |  |  |  |  |  |  |  |  |
 *        +--+--+--+--+--+--+--+--+
 *           |  |  |  |  |  |  |
 *           |  |  |  |  |  |  +-- SENSOR_READY
 *           |  |  |  |  |  +----- DATA_AVAILABLE
 *           |  |  |  |  +-------- TX_COMPLETE
 *           |  |  |  +----------- RX_COMPLETE
 *           |  |  +-------------- INIT_DONE
 *           |  +----------------- ERROR_OCCURRED
 *           +-------------------- SHUTDOWN_REQ
 *
 *
 * Wait for ANY of bits 0, 1, 2:   (SENSOR | DATA | TX)
 * Wait for ALL of bits 0, 4:      (SENSOR & INIT)
 *
 * ============================================================================
 * EVENT GROUP vs SEMAPHORE
 * ============================================================================
 *
 * +-------------------------+------------------------+------------------------+
 * | Aspect                  | Event Group            | Semaphore              |
 * +-------------------------+------------------------+------------------------+
 * | Data type               | Bit flags (multiple)   | Counter (single)       |
 * | Wait condition          | ANY/ALL of bits        | Count > 0              |
 * | Set/Clear               | Individual bits        | Give/Take              |
 * | Multiple events         | Yes (native)           | Need multiple sems     |
 * | Broadcast               | Yes (all waiters)      | One waiter only        |
 * | ISR safe (FreeRTOS)     | Limited (set only)     | Yes (FromISR)          |
 * | Use case                | State machines         | Signaling, counting    |
 * +-------------------------+------------------------+------------------------+
 *
 * ============================================================================
 * CODE EXAMPLE: FreeRTOS Event Groups
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

/*
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

// Define event bits
#define EVENT_SENSOR_READY    (1 << 0)
#define EVENT_DATA_AVAILABLE  (1 << 1)
#define EVENT_TX_COMPLETE     (1 << 2)
#define EVENT_INIT_DONE       (1 << 3)
#define EVENT_ERROR           (1 << 4)

EventGroupHandle_t system_events;

void init_events(void) {
    system_events = xEventGroupCreate();
}


// Producer task - sets events
void sensor_task(void *pvParameters) {
    for (;;) {
        // Read sensor
        read_sensor();

        // Set event bit
        xEventGroupSetBits(system_events, EVENT_SENSOR_READY);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


// Consumer task - waits for events
void processing_task(void *pvParameters) {
    for (;;) {
        // Wait for sensor ready AND init done
        EventBits_t bits = xEventGroupWaitBits(
            system_events,
            EVENT_SENSOR_READY | EVENT_INIT_DONE,  // Bits to wait for
            pdTRUE,                                  // Clear bits on exit
            pdTRUE,                                  // Wait for ALL bits
            portMAX_DELAY                            // Timeout
        );

        if (bits & (EVENT_SENSOR_READY | EVENT_INIT_DONE)) {
            // Both events occurred
            process_sensor_data();
        }
    }
}


// Error handler - waits for ANY error
void error_task(void *pvParameters) {
    for (;;) {
        EventBits_t bits = xEventGroupWaitBits(
            system_events,
            EVENT_ERROR,
            pdTRUE,   // Clear on exit
            pdFALSE,  // Wait for ANY (just one bit anyway)
            portMAX_DELAY
        );

        if (bits & EVENT_ERROR) {
            handle_error();
        }
    }
}


// ISR - set event (limited, no clear from ISR in FreeRTOS!)
void UART_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Can only SET bits from ISR, not clear!
    xEventGroupSetBitsFromISR(
        system_events,
        EVENT_DATA_AVAILABLE,
        &xHigherPriorityTaskWoken
    );

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
*/

/*
 * ============================================================================
 * CODE EXAMPLE: Zephyr Events
 * ============================================================================
 */

/*
#include <zephyr/kernel.h>

#define EVENT_SENSOR_READY    BIT(0)
#define EVENT_DATA_AVAILABLE  BIT(1)
#define EVENT_TX_COMPLETE     BIT(2)
#define EVENT_INIT_DONE       BIT(3)

K_EVENT_DEFINE(system_events);

// Set events
void sensor_thread(void *p1, void *p2, void *p3) {
    while (1) {
        read_sensor();

        // Post (set) event
        k_event_post(&system_events, EVENT_SENSOR_READY);

        k_sleep(K_MSEC(100));
    }
}

// Wait for events
void processing_thread(void *p1, void *p2, void *p3) {
    while (1) {
        // Wait for ALL specified events
        uint32_t events = k_event_wait_all(
            &system_events,
            EVENT_SENSOR_READY | EVENT_INIT_DONE,
            true,           // Reset events after wait
            K_FOREVER
        );

        // Or wait for ANY event
        // events = k_event_wait(&system_events, mask, true, K_FOREVER);

        process_data();
    }
}

// Clear events
void clear_events(void) {
    k_event_clear(&system_events, EVENT_SENSOR_READY);
}
*/

/*
 * ============================================================================
 * SYNCHRONIZATION PATTERNS WITH EVENT GROUPS
 * ============================================================================
 *
 * PATTERN 1: Rendezvous (All Tasks Reach Point)
 * ------------------------------------------------
 *
 *   Task A: xEventGroupSync(events, BIT_A, ALL_BITS, timeout);
 *   Task B: xEventGroupSync(events, BIT_B, ALL_BITS, timeout);
 *   Task C: xEventGroupSync(events, BIT_C, ALL_BITS, timeout);
 *
 *   All tasks wait until ALL have called sync.
 *
 *
 * PATTERN 2: State Machine
 * ------------------------------------------------
 *
 *   #define STATE_IDLE      (1 << 0)
 *   #define STATE_RUNNING   (1 << 1)
 *   #define STATE_ERROR     (1 << 2)
 *   #define STATE_COMPLETE  (1 << 3)
 *
 *   // Wait for state change
 *   EventBits_t state = xEventGroupWaitBits(state_events, ALL_STATES,
 *                                           pdFALSE, pdFALSE, timeout);
 *   if (state & STATE_ERROR) handle_error();
 *   else if (state & STATE_COMPLETE) finish();
 *
 *
 * PATTERN 3: Broadcast Notification
 * ------------------------------------------------
 *
 *   // Multiple tasks waiting for same event
 *   // All wake up when bit is set
 *
 *   Task 1: xEventGroupWaitBits(events, SHUTDOWN_BIT, ...);
 *   Task 2: xEventGroupWaitBits(events, SHUTDOWN_BIT, ...);
 *   Task 3: xEventGroupWaitBits(events, SHUTDOWN_BIT, ...);
 *
 *   // Controller sets bit - ALL tasks wake up
 *   xEventGroupSetBits(events, SHUTDOWN_BIT);
 *
 * ============================================================================
 * WHEN TO USE EVENT GROUPS vs SEMAPHORES
 * ============================================================================
 *
 * USE EVENT GROUPS WHEN:
 * - Need to wait for multiple conditions
 * - Need ANY/ALL logic
 * - State machine with multiple states
 * - Broadcast notification (wake all waiters)
 *
 * USE SEMAPHORES WHEN:
 * - Simple single-event signaling
 * - Counting (resource pool)
 * - ISR to task notification (more efficient)
 * - Need to signal from ISR with full functionality
 *
 * ============================================================================
 * LIMITATIONS
 * ============================================================================
 *
 * FreeRTOS Event Groups:
 * - Only 24 bits usable (8 reserved for internal use)
 * - Can't CLEAR bits from ISR
 * - SetBitsFromISR requires timer task
 *
 * Zephyr Events:
 * - Full 32 bits available
 * - Can post from ISR
 * - More flexible API
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. EXPLAIN THE CONCEPT
 *    "Multiple bits representing multiple events, wait for ANY or ALL"
 *
 * 2. CONTRAST WITH SEMAPHORE
 *    "Semaphore is single counter, event group is multiple independent bits"
 *
 * 3. GIVE USE CASES
 *    "State machines, waiting for multiple peripherals, broadcast"
 *
 * 4. MENTION LIMITATIONS
 *    "FreeRTOS: only 24 bits usable, can't clear from ISR"
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "How is xEventGroupSync different from xEventGroupWaitBits?"
 * A: Sync combines setting bits and waiting atomically. Used for rendezvous
 *    where all tasks must reach a point before any can proceed.
 *
 * Q: "What happens if multiple tasks wait for the same bit?"
 * A: All waiting tasks are unblocked when the bit is set (broadcast).
 *    This is different from semaphore which only wakes one waiter.
 *
 * Q: "Why only 24 bits in FreeRTOS?"
 * A: Upper 8 bits used internally for task notifications and control.
 *    The actual EventBits_t is 32-bit but 8 are reserved.
 *
 * ============================================================================
 */

int main() {
    printf("=== RTOS Interview Question 16 ===\n\n");
    printf("Q: What are Event Groups? How differ from Semaphores?\n\n");
    printf("EVENT GROUPS:\n");
    printf("- Multiple bits represent multiple events\n");
    printf("- Wait for ANY or ALL bits\n");
    printf("- Broadcast to all waiters\n");
    printf("- Good for state machines\n\n");
    printf("SEMAPHORES:\n");
    printf("- Single counter\n");
    printf("- Wait for count > 0\n");
    printf("- Wakes one waiter\n");
    printf("- Good for simple signaling\n\n");
    printf("FREERTOS API:\n");
    printf("- xEventGroupCreate()\n");
    printf("- xEventGroupSetBits()\n");
    printf("- xEventGroupWaitBits(bits, clearOnExit, waitForAll, timeout)\n");
    printf("- xEventGroupSync() for rendezvous\n\n");
    printf("ZEPHYR API:\n");
    printf("- K_EVENT_DEFINE()\n");
    printf("- k_event_post()\n");
    printf("- k_event_wait() / k_event_wait_all()\n");
    return 0;
}
