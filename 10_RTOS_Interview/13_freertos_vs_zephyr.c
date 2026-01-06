/*
 * ============================================================================
 * QUESTION 13: Compare FreeRTOS and Zephyr RTOS. When would you use each?
 * ============================================================================
 *
 * DIFFICULTY: Medium | FREQUENCY: Very High | IMPORTANCE: Critical
 *
 * You mentioned experience with both - this question is almost certain!
 * Be prepared to discuss architecture, APIs, and practical tradeoffs.
 *
 * ============================================================================
 * QUICK ANSWER (30 seconds)
 * ============================================================================
 *
 * "FreeRTOS is lightweight, simple, and widely adopted - ideal for resource-
 * constrained MCUs and when you need just a scheduler with basic primitives.
 * Zephyr is a full-featured OS with device drivers, networking stacks,
 * filesystems, and a modern build system - better for complex IoT products.
 * I'd choose FreeRTOS for a simple sensor node with tight memory constraints,
 * and Zephyr for a connected device needing Bluetooth, networking, or
 * complex peripheral management. Zephyr's devicetree and Kconfig make it
 * more scalable for product development."
 *
 * ============================================================================
 * COMPREHENSIVE COMPARISON
 * ============================================================================
 *
 * +------------------------+-------------------------+---------------------------+
 * | Aspect                 | FreeRTOS                | Zephyr RTOS               |
 * +------------------------+-------------------------+---------------------------+
 * | PHILOSOPHY             | Minimal kernel, add     | Full-featured OS with     |
 * |                        | what you need           | integrated components     |
 * +------------------------+-------------------------+---------------------------+
 * | LICENSE                | MIT (very permissive)   | Apache 2.0 (permissive)   |
 * +------------------------+-------------------------+---------------------------+
 * | FOOTPRINT              | ~6-12KB ROM minimal     | ~8KB+ ROM minimal         |
 * |                        | (kernel only)           | (more with drivers)       |
 * +------------------------+-------------------------+---------------------------+
 * | MATURITY               | 20+ years, very mature  | Newer (2015+), evolving   |
 * +------------------------+-------------------------+---------------------------+
 * | ECOSYSTEM              | Massive, AWS support    | Linux Foundation backed   |
 * +------------------------+-------------------------+---------------------------+
 * | BUILD SYSTEM           | CMake/Make/IDE          | West + CMake + Kconfig    |
 * +------------------------+-------------------------+---------------------------+
 * | CONFIGURATION          | FreeRTOSConfig.h        | Kconfig + devicetree      |
 * +------------------------+-------------------------+---------------------------+
 * | DEVICE DRIVERS         | Separate (add yourself) | Integrated, standardized  |
 * +------------------------+-------------------------+---------------------------+
 * | NETWORKING             | FreeRTOS+TCP, lwIP      | Native, full-featured     |
 * +------------------------+-------------------------+---------------------------+
 * | BLUETOOTH              | Separate (NimBLE)       | Native Bluetooth stack    |
 * +------------------------+-------------------------+---------------------------+
 * | FILESYSTEM             | FreeRTOS+FAT, LittleFS  | Native, multiple FS       |
 * +------------------------+-------------------------+---------------------------+
 * | SHELL/LOGGING          | Add yourself            | Built-in shell & logging  |
 * +------------------------+-------------------------+---------------------------+
 * | CERTIFICATION          | SAFERTOS available      | Safety certification WIP  |
 * +------------------------+-------------------------+---------------------------+
 * | LEARNING CURVE         | Low                     | Higher (more concepts)    |
 * +------------------------+-------------------------+---------------------------+
 *
 * ============================================================================
 * API COMPARISON
 * ============================================================================
 *
 * TASK/THREAD CREATION:
 * ---------------------------------
 *
 * FreeRTOS:
 *   xTaskCreate(task_func, "name", stack_words, param, priority, &handle);
 *
 * Zephyr:
 *   K_THREAD_DEFINE(name, stack_size, entry, p1, p2, p3, priority, 0, 0);
 *   // or runtime:
 *   k_thread_create(&thread, stack, size, entry, p1, p2, p3, priority, 0, 0);
 *
 *
 * MUTEX:
 * ---------------------------------
 *
 * FreeRTOS:
 *   SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
 *   xSemaphoreTake(mutex, portMAX_DELAY);
 *   xSemaphoreGive(mutex);
 *
 * Zephyr:
 *   K_MUTEX_DEFINE(mutex);  // or k_mutex_init(&mutex);
 *   k_mutex_lock(&mutex, K_FOREVER);
 *   k_mutex_unlock(&mutex);
 *
 *
 * SEMAPHORE:
 * ---------------------------------
 *
 * FreeRTOS:
 *   SemaphoreHandle_t sem = xSemaphoreCreateBinary();
 *   xSemaphoreGive(sem);
 *   xSemaphoreTake(sem, portMAX_DELAY);
 *
 * Zephyr:
 *   K_SEM_DEFINE(sem, 0, 1);  // initial=0, max=1
 *   k_sem_give(&sem);
 *   k_sem_take(&sem, K_FOREVER);
 *
 *
 * QUEUE/MESSAGE QUEUE:
 * ---------------------------------
 *
 * FreeRTOS:
 *   QueueHandle_t queue = xQueueCreate(10, sizeof(MyData));
 *   xQueueSend(queue, &data, portMAX_DELAY);
 *   xQueueReceive(queue, &data, portMAX_DELAY);
 *
 * Zephyr:
 *   K_MSGQ_DEFINE(queue, sizeof(MyData), 10, 4);
 *   k_msgq_put(&queue, &data, K_FOREVER);
 *   k_msgq_get(&queue, &data, K_FOREVER);
 *
 *
 * DELAY:
 * ---------------------------------
 *
 * FreeRTOS:
 *   vTaskDelay(pdMS_TO_TICKS(100));
 *
 * Zephyr:
 *   k_sleep(K_MSEC(100));
 *
 *
 * TIMER:
 * ---------------------------------
 *
 * FreeRTOS:
 *   TimerHandle_t timer = xTimerCreate("name", period, reload, id, callback);
 *   xTimerStart(timer, 0);
 *
 * Zephyr:
 *   K_TIMER_DEFINE(timer, handler, NULL);
 *   k_timer_start(&timer, K_MSEC(100), K_MSEC(100));
 *
 * ============================================================================
 * PRIORITY NUMBERING (CRITICAL DIFFERENCE!)
 * ============================================================================
 *
 * FreeRTOS:  Higher number = Higher priority
 *            Priority 10 runs before Priority 5
 *            configMAX_PRIORITIES defines the range
 *
 * Zephyr:    Lower number = Higher priority (like Linux/Unix)
 *            Priority 1 runs before Priority 5
 *            Negative priorities = cooperative threads
 *
 *   FreeRTOS                    Zephyr
 *   +-------+                   +-------+
 *   | Pri 7 | ← Highest         | Pri-1 | ← Cooperative
 *   | Pri 6 |                   | Pri 0 | ← Highest preemptive
 *   | Pri 5 |                   | Pri 1 |
 *   | Pri 4 |                   | Pri 2 |
 *   | Pri 3 |                   | Pri 3 |
 *   | Pri 2 |                   | Pri 4 |
 *   | Pri 1 |                   | Pri 5 |
 *   | Pri 0 | ← Idle            | Pri 14| ← Lowest
 *   +-------+                   +-------+
 *
 * ============================================================================
 * CONFIGURATION COMPARISON
 * ============================================================================
 *
 * FREERTOS (FreeRTOSConfig.h):
 * ---------------------------------
 *
 *   #define configUSE_PREEMPTION        1
 *   #define configUSE_TICK_HOOK         0
 *   #define configCPU_CLOCK_HZ          72000000
 *   #define configTICK_RATE_HZ          1000
 *   #define configMAX_PRIORITIES        7
 *   #define configMINIMAL_STACK_SIZE    128
 *   #define configTOTAL_HEAP_SIZE       10240
 *   #define configUSE_MUTEXES           1
 *   #define configUSE_COUNTING_SEMAPHORES 1
 *   #define configUSE_TIMERS            1
 *
 *   PROS: Simple, one header file
 *   CONS: Manual, easy to misconfigure
 *
 *
 * ZEPHYR (prj.conf + Kconfig + devicetree):
 * ---------------------------------
 *
 *   # prj.conf
 *   CONFIG_GPIO=y
 *   CONFIG_I2C=y
 *   CONFIG_SENSOR=y
 *   CONFIG_BT=y
 *   CONFIG_BT_PERIPHERAL=y
 *   CONFIG_LOG=y
 *   CONFIG_SHELL=y
 *
 *   # Hardware configuration via devicetree
 *   # boards/arm/my_board/my_board.dts
 *
 *   PROS: Type-safe, dependency management, hardware abstraction
 *   CONS: More complex, steeper learning curve
 *
 * ============================================================================
 * WHEN TO USE FREERTOS
 * ============================================================================
 *
 * 1. SIMPLE APPLICATIONS
 *    - Just need task scheduling
 *    - Don't need complex drivers
 *
 * 2. EXTREMELY TIGHT MEMORY
 *    - <32KB Flash
 *    - <8KB RAM
 *
 * 3. LEGACY PROJECTS
 *    - Existing FreeRTOS codebase
 *    - Team familiar with FreeRTOS
 *
 * 4. VENDOR SDK USES FREERTOS
 *    - ESP32 (ESP-IDF uses FreeRTOS)
 *    - Many STM32 examples
 *    - AWS IoT integration
 *
 * 5. CERTIFICATION REQUIRED
 *    - SAFERTOS is certifiable version
 *    - Automotive, medical, aviation
 *
 * 6. QUICK PROTOTYPE
 *    - Lower learning curve
 *    - Faster to get started
 *
 * ============================================================================
 * WHEN TO USE ZEPHYR
 * ============================================================================
 *
 * 1. COMPLEX IOT PRODUCTS
 *    - Need networking, BLE, filesystem
 *    - Multiple peripherals
 *
 * 2. PRODUCT DEVELOPMENT
 *    - Need reproducible builds
 *    - Hardware abstraction important
 *    - Multiple board variants
 *
 * 3. BLUETOOTH APPLICATIONS
 *    - Zephyr's BT stack is excellent
 *    - BLE mesh, audio, etc.
 *
 * 4. NEED INTEGRATED FEATURES
 *    - Shell for debugging
 *    - Logging framework
 *    - Settings subsystem
 *    - DFU (Device Firmware Update)
 *
 * 5. MULTIPLE MCU TARGETS
 *    - Same codebase, different boards
 *    - Good portability
 *
 * 6. MODERN DEVELOPMENT PRACTICES
 *    - CI/CD friendly
 *    - Good documentation
 *    - Active community
 *
 * ============================================================================
 * PRACTICAL EXAMPLES
 * ============================================================================
 *
 * EXAMPLE 1: Simple Sensor Node (FreeRTOS)
 * -----------------------------------------
 * - STM32F0 (32KB Flash, 8KB RAM)
 * - Read temperature sensor
 * - Blink LED
 * - No networking
 *
 * → FreeRTOS: Minimal footprint, simple tasks
 *
 *
 * EXAMPLE 2: BLE Fitness Tracker (Zephyr)
 * -----------------------------------------
 * - nRF52840 (1MB Flash, 256KB RAM)
 * - BLE peripheral
 * - Accelerometer driver
 * - File logging
 * - DFU support
 *
 * → Zephyr: Integrated BLE, sensors, DFU
 *
 *
 * EXAMPLE 3: AWS IoT Device (FreeRTOS)
 * -----------------------------------------
 * - ESP32
 * - WiFi connectivity
 * - AWS IoT Core integration
 * - OTA updates
 *
 * → FreeRTOS: ESP-IDF based on FreeRTOS, AWS support
 *
 *
 * EXAMPLE 4: Industrial Gateway (Zephyr)
 * -----------------------------------------
 * - Multiple CAN buses
 * - Ethernet + WiFi
 * - Modbus protocol
 * - Remote shell access
 * - Extensive logging
 *
 * → Zephyr: Complex I/O, networking, shell
 *
 * ============================================================================
 * INTERVIEW TIPS
 * ============================================================================
 *
 * 1. DON'T SAY ONE IS "BETTER"
 *    They're different tools for different needs
 *
 * 2. GIVE CONCRETE EXAMPLES
 *    "For a BLE device, I'd choose Zephyr because..."
 *
 * 3. MENTION YOUR EXPERIENCE
 *    "In my project with Zephyr, I used..."
 *
 * 4. KNOW THE API DIFFERENCES
 *    Especially priority numbering (opposite!)
 *
 * 5. DISCUSS BUILD SYSTEMS
 *    FreeRTOSConfig.h vs Kconfig/devicetree
 *
 * ============================================================================
 * FOLLOW-UP QUESTIONS
 * ============================================================================
 *
 * Q: "Can you mix FreeRTOS code with Zephyr?"
 * A: Not directly - different APIs. Would need abstraction layer.
 *    Some vendors provide shim layers.
 *
 * Q: "Which has better power management?"
 * A: Both support tickless idle. Zephyr has more integrated PM framework
 *    with device power management.
 *
 * Q: "How do you port code from one to the other?"
 * A: Create abstraction layer for OS primitives (tasks, semaphores, etc.)
 *    or use CMSIS-RTOS2 API on both (both support it).
 *
 * Q: "Which is better for certification?"
 * A: FreeRTOS has SAFERTOS (certified variant).
 *    Zephyr is working on safety certification.
 *
 * ============================================================================
 */

#include <stdio.h>

int main() {
    printf("=== RTOS Interview Question 13 ===\n\n");
    printf("Q: Compare FreeRTOS and Zephyr. When use each?\n\n");
    printf("FREERTOS:\n");
    printf("- Lightweight (~6KB), simple API\n");
    printf("- Minimal kernel, add what you need\n");
    printf("- Higher priority number = higher priority\n");
    printf("- FreeRTOSConfig.h configuration\n");
    printf("- Best for: simple apps, tight memory, vendor SDKs\n\n");
    printf("ZEPHYR:\n");
    printf("- Full-featured OS with drivers, stacks\n");
    printf("- Integrated BLE, networking, filesystem\n");
    printf("- Lower priority number = higher priority (!)\n");
    printf("- Kconfig + devicetree configuration\n");
    printf("- Best for: complex IoT, BLE, product development\n\n");
    printf("KEY DIFFERENCES:\n");
    printf("- Priority: FreeRTOS (high=high) vs Zephyr (low=high)\n");
    printf("- Stack size: FreeRTOS (words) vs Zephyr (bytes)\n");
    printf("- Configuration: header file vs Kconfig\n");
    return 0;
}
