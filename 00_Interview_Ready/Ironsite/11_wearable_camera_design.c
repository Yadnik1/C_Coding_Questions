/*
 * ============================================================================
 *        IRONSITE INTERVIEW PREP: Wearable Camera for Construction
 * ============================================================================
 *
 * COMPANY: Ironsite - Wearable cameras on hard hats for construction workers
 * ROLE: Firmware/Embedded Engineer
 *
 * DEVICE SPECS (from job posting):
 *   - Platforms: Raspberry Pi, ESP32, STM32
 *   - Sensors: Camera, audio, IMU, environment sensing
 *   - Runtime: 8+ hours continuous recording
 *   - Environment: Extreme temps, dust, vibration, outdoor
 *   - Features: OTA updates, telemetry, edge ML inference
 *
 * ============================================================================
 *                    SYSTEM ARCHITECTURE OVERVIEW
 * ============================================================================
 *
 *   +------------------------------------------------------------------+
 *   |                    WEARABLE CAMERA DEVICE                        |
 *   +------------------------------------------------------------------+
 *   |                                                                  |
 *   |  +------------+    +------------+    +------------+              |
 *   |  |   Camera   |    |    IMU     |    |   Mic      |              |
 *   |  | (1080p/30) |    | (Accel+Gyr)|    |  (Audio)   |              |
 *   |  +-----+------+    +-----+------+    +-----+------+              |
 *   |        |                 |                 |                     |
 *   |        v                 v                 v                     |
 *   |  +--------------------------------------------------+           |
 *   |  |              MAIN MCU (ESP32/STM32)               |           |
 *   |  |                                                   |           |
 *   |  |  +-----------+  +-----------+  +-----------+     |           |
 *   |  |  | Video     |  | Sensor    |  | Power     |     |           |
 *   |  |  | Pipeline  |  | Fusion    |  | Manager   |     |           |
 *   |  |  +-----------+  +-----------+  +-----------+     |           |
 *   |  |                                                   |           |
 *   |  |  +-----------+  +-----------+  +-----------+     |           |
 *   |  |  | State     |  | OTA       |  | Edge ML   |     |           |
 *   |  |  | Machine   |  | Manager   |  | Inference |     |           |
 *   |  |  +-----------+  +-----------+  +-----------+     |           |
 *   |  +--------------------------------------------------+           |
 *   |        |                 |                 |                     |
 *   |        v                 v                 v                     |
 *   |  +------------+    +------------+    +------------+              |
 *   |  |  SD Card   |    |   WiFi     |    |  Battery   |              |
 *   |  | (Storage)  |    | (Upload)   |    | (3000mAh)  |              |
 *   |  +------------+    +------------+    +------------+              |
 *   |                                                                  |
 *   +------------------------------------------------------------------+
 *
 * ============================================================================
 *                 QUESTION 1: POWER MANAGEMENT (8+ HOUR RUNTIME)
 * ============================================================================
 *
 * "Design a power management system for 8-hour continuous video recording"
 *
 * POWER BUDGET CALCULATION:
 *
 *   Component         | Active (mA) | Sleep (mA) | Duty Cycle
 *   ------------------|-------------|------------|------------
 *   Camera module     |    150      |     0      |   100%
 *   ESP32 (WiFi off)  |     80      |    10      |   100%
 *   IMU               |      5      |   0.01     |   100%
 *   SD card writes    |    100      |     0      |    20%
 *   WiFi (burst)      |    200      |     0      |     5%
 *   ------------------|-------------|------------|------------
 *   Average total     |   ~280 mA
 *
 *   Battery needed: 280mA × 8h = 2240 mAh (use 3000mAh for margin)
 *
 * POWER STATES:
 *
 *   ACTIVE_RECORDING:  Camera ON, IMU ON, SD writes, WiFi OFF
 *   UPLOAD_MODE:       Camera OFF, WiFi ON, bulk upload
 *   LOW_POWER:         Reduce resolution, lower frame rate
 *   CRITICAL:          Stop recording, save state, alert user
 *   CHARGING:          Full features available
 *
 * STATE TRANSITIONS:
 *
 *   Battery > 20%  → ACTIVE_RECORDING
 *   Battery 10-20% → LOW_POWER (reduce to 720p/15fps)
 *   Battery < 10%  → CRITICAL (stop, save, LED warning)
 *   USB connected  → CHARGING + UPLOAD_MODE
 *
 * ============================================================================
 *                 QUESTION 2: VIDEO PIPELINE DESIGN
 * ============================================================================
 *
 * "Design a system to capture and store 8 hours of video reliably"
 *
 * STORAGE CALCULATION:
 *
 *   1080p @ 30fps, H.264 compressed ≈ 5 Mbps = 0.625 MB/s
 *   8 hours = 8 × 3600 × 0.625 = 18 GB
 *
 *   Use 32GB SD card with wear leveling
 *
 * VIDEO PIPELINE:
 *
 *   Camera → Frame Buffer → Encoder → Chunk Buffer → SD Card
 *              (DMA)        (H.264)    (Ring Buffer)
 *
 *   +--------+     +--------+     +--------+     +--------+
 *   | Camera |---->| Frame  |---->| H.264  |---->| Chunk  |---> SD
 *   | Sensor |     | Buffer |     | Encode |     | Buffer |
 *   +--------+     +--------+     +--------+     +--------+
 *       |              |              |              |
 *       30 fps      2 frames      1 second       5 min chunks
 *                   (ping-pong)   segments
 *
 * CHUNKING STRATEGY:
 *
 *   - Split video into 5-minute chunks
 *   - Each chunk is independent (can upload/delete separately)
 *   - Filename: YYYYMMDD_HHMMSS_chunk001.mp4
 *   - Metadata: timestamp, GPS (if available), worker ID
 *
 * FAILURE HANDLING:
 *
 *   - SD card full: Delete oldest chunks (circular)
 *   - SD write fail: Buffer in RAM, retry, alert if persistent
 *   - Power loss: Each chunk has proper footer, recoverable
 *
 * ============================================================================
 *                 QUESTION 3: STATE MACHINE DESIGN
 * ============================================================================
 *
 * "Design the main state machine for the wearable camera"
 *
 *                           +-------------+
 *                           |    INIT     |
 *                           +------+------+
 *                                  |
 *                           HW init complete
 *                                  |
 *                                  v
 *                           +-------------+
 *            +------------->|    IDLE     |<-------------+
 *            |              +------+------+              |
 *            |                     |                     |
 *         stop_btn           start_btn              upload_done
 *            |                     |                     |
 *            |                     v                     |
 *            |              +-------------+              |
 *            +--------------| RECORDING   |--------------+
 *            |              +------+------+              |
 *            |                     |                     |
 *            |              low_battery/                 |
 *            |              usb_connected                |
 *            |                     |                     |
 *            |                     v                     |
 *            |              +-------------+              |
 *            |              |  UPLOADING  |--------------+
 *            |              +------+------+
 *            |                     |
 *            |              critical_battery
 *            |                     |
 *            |                     v
 *            |              +-------------+
 *            +--------------| LOW_POWER   |
 *            |              +-------------+
 *            |                     |
 *            |              battery < 5%
 *            |                     |
 *            |                     v
 *            |              +-------------+
 *            +--------------| SHUTDOWN    |
 *                           +-------------+
 *
 * EVENTS TO HANDLE:
 *
 *   - Button press (start/stop recording)
 *   - USB connected/disconnected
 *   - Battery level changes
 *   - SD card full/removed
 *   - WiFi connected/disconnected
 *   - Fall detection (IMU trigger)
 *   - Temperature warning
 *
 * ============================================================================
 *                 QUESTION 4: OTA UPDATE SYSTEM
 * ============================================================================
 *
 * "Design a secure OTA system for devices deployed on construction sites"
 *
 * REQUIREMENTS:
 *   - Can't brick device (workers depend on it)
 *   - Update during charging/end of shift
 *   - Verify firmware integrity
 *   - Rollback on failure
 *
 * A/B PARTITION SCHEME:
 *
 *   Flash Layout:
 *   +------------------+
 *   | Bootloader       |  (never updated in field)
 *   +------------------+
 *   | Partition A      |  <- Currently running
 *   | (Firmware v1.2)  |
 *   +------------------+
 *   | Partition B      |  <- Download new here
 *   | (Firmware v1.3)  |
 *   +------------------+
 *   | NVS (config)     |  (persistent across updates)
 *   +------------------+
 *
 * OTA FLOW:
 *
 *   1. Check for update (on WiFi connect or periodic)
 *   2. Download to inactive partition
 *   3. Verify SHA256 + signature
 *   4. Mark new partition as "pending"
 *   5. Reboot into new firmware
 *   6. New firmware marks itself "confirmed" after self-test
 *   7. If no confirm in 3 boots → rollback to previous
 *
 * WHEN TO UPDATE:
 *
 *   - Device charging (USB connected)
 *   - End of shift (detected by: evening time + not moving)
 *   - Never during active recording
 *   - Battery > 50% required
 *
 * ============================================================================
 *                 QUESTION 5: SENSOR FUSION (IMU + CAMERA)
 * ============================================================================
 *
 * "Use IMU to detect if worker fell vs device fell off helmet"
 *
 * IMU DATA:
 *
 *   Accelerometer: X, Y, Z (±16g range)
 *   Gyroscope: Roll, Pitch, Yaw (±2000 dps)
 *   Sample rate: 100 Hz
 *
 * FALL DETECTION ALGORITHM:
 *
 *   1. Monitor acceleration magnitude: |a| = sqrt(x² + y² + z²)
 *   2. Detect free-fall: |a| < 0.5g for > 200ms
 *   3. Detect impact: |a| > 3g spike
 *   4. Check orientation change: > 60° in 1 second
 *
 *   WORKER FALL:
 *     - Free-fall + Impact + Device still moving after
 *     - Alert immediately, mark video timestamp
 *
 *   DEVICE FELL OFF:
 *     - Free-fall + Impact + No movement after (device on ground)
 *     - Alert user to retrieve device
 *
 * FILTERING NOISE:
 *
 *   Use EMA filter on IMU data:
 *   filtered = 0.2 * new_sample + 0.8 * filtered
 *
 *   Why: Construction site has constant vibration from machinery
 *
 * ============================================================================
 *                 QUESTION 6: EDGE ML INFERENCE
 * ============================================================================
 *
 * "Run safety detection model on device (detect if worker wearing PPE)"
 *
 * CONSTRAINTS:
 *
 *   - ESP32: 520KB SRAM, 4MB Flash
 *   - Must not affect video recording
 *   - Battery impact must be minimal
 *
 * APPROACH:
 *
 *   1. Use TensorFlow Lite Micro (TFLite Micro)
 *   2. Quantized INT8 model (smaller, faster than float)
 *   3. Model size: < 500KB
 *   4. Run inference on keyframes only (1 fps, not 30 fps)
 *
 * INFERENCE PIPELINE:
 *
 *   Video Frame (1080p)
 *        |
 *        v
 *   Downsample to 96x96 (model input size)
 *        |
 *        v
 *   TFLite Micro Inference (~100ms on ESP32)
 *        |
 *        v
 *   Results: [hard_hat: 0.95, vest: 0.87, gloves: 0.23]
 *        |
 *        v
 *   Log to metadata, alert if PPE missing
 *
 * WHEN TO RUN INFERENCE:
 *
 *   - Every N seconds (configurable, default 5s)
 *   - On motion detected (person entered frame)
 *   - Not during heavy SD writes (avoid stutter)
 *
 * ============================================================================
 *                 QUESTION 7: DATA SYNC STRATEGY
 * ============================================================================
 *
 * "Device records all day, uploads at end of shift. Design sync system."
 *
 * SYNC SCENARIOS:
 *
 *   1. END OF SHIFT: Worker docks device, WiFi available
 *      - Upload all chunks from today
 *      - Priority: chunks with safety events first
 *
 *   2. OPPORTUNISTIC: Device near known WiFi during break
 *      - Upload high-priority chunks only
 *      - Resume if connection drops
 *
 *   3. CELLULAR (if equipped): Real-time alerts only
 *      - Fall detection → immediate alert
 *      - PPE violation → queue for next WiFi
 *
 * UPLOAD QUEUE:
 *
 *   Priority levels:
 *   1. CRITICAL: Fall detected, injury suspected
 *   2. HIGH: Safety violation (no hard hat)
 *   3. NORMAL: Regular video chunks
 *   4. LOW: Telemetry, logs
 *
 *   +------------+     +------------+     +------------+
 *   | Chunk      |---->| Upload     |---->| Cloud      |
 *   | Queue      |     | Manager    |     | Storage    |
 *   +------------+     +------------+     +------------+
 *        |                   |
 *        |              WiFi status
 *        |              Bandwidth
 *        |              Battery level
 *        v
 *   Priority sort
 *
 * RESUME ON FAILURE:
 *
 *   - Track upload progress per chunk
 *   - Support resumable uploads (byte range)
 *   - Retry with exponential backoff
 *   - After N failures, mark for next sync window
 *
 * ============================================================================
 *                 QUESTION 8: RELIABILITY IN HARSH ENVIRONMENT
 * ============================================================================
 *
 * "Device must work in -10°C to 50°C, dust, vibration. How to ensure reliability?"
 *
 * HARDWARE CONSIDERATIONS:
 *
 *   - Conformal coating on PCB (dust/moisture)
 *   - Industrial temp rated components
 *   - Vibration dampening for camera mount
 *   - Sealed enclosure (IP65+)
 *
 * FIRMWARE SAFEGUARDS:
 *
 *   1. WATCHDOG TIMER:
 *      - Hardware watchdog, 10 second timeout
 *      - Must pet watchdog in main loop
 *      - If firmware hangs → automatic reboot
 *
 *   2. TEMPERATURE MONITORING:
 *      - Read internal temp sensor
 *      - > 60°C: Reduce processing, warn user
 *      - > 70°C: Thermal shutdown, save state
 *      - < -5°C: Battery performance degrades, warn user
 *
 *   3. ERROR LOGGING:
 *      - Circular log buffer in flash
 *      - Survives reboot
 *      - Upload logs with telemetry
 *      - Track: reboot reason, errors, battery cycles
 *
 *   4. SELF-TEST ON BOOT:
 *      - Camera: capture test frame
 *      - IMU: read values, check range
 *      - SD card: write/read test
 *      - WiFi: scan (don't connect)
 *      - If any fail: LED error code, log, continue if possible
 *
 * ============================================================================
 *                 QUESTION 9: FACTORY PROVISIONING
 * ============================================================================
 *
 * "How would you provision devices during manufacturing?"
 *
 * PROVISIONING DATA:
 *
 *   - Unique device ID (serial number)
 *   - Device certificate (for cloud auth)
 *   - Private key (secure element or encrypted flash)
 *   - Factory calibration data (camera, IMU offsets)
 *   - Initial firmware
 *
 * PROVISIONING FLOW:
 *
 *   Factory Station                    Device
 *        |                                |
 *        |-------- Flash firmware ------->|
 *        |                                |
 *        |<------- Device ready ----------|
 *        |                                |
 *        |-------- Send device cert ----->|
 *        |        + private key           |
 *        |                                |
 *        |-------- Send calibration ----->|
 *        |                                |
 *        |<------- Self-test results -----|
 *        |                                |
 *        |-------- Mark provisioned ----->|
 *        |                                |
 *        |         Store in cloud DB      |
 *
 * SECURITY:
 *
 *   - Private keys generated ON device (never leave device)
 *   - Or: use secure element (ATECC608A)
 *   - Public key sent to cloud for registration
 *   - Encrypted flash for sensitive data
 *
 * ============================================================================
 *                 QUESTION 10: DEBUGGING IN THE FIELD
 * ============================================================================
 *
 * "Customer reports device stops recording after 2 hours. How to debug?"
 *
 * REMOTE DIAGNOSTICS:
 *
 *   1. Check telemetry data:
 *      - Battery curve (does it drop suddenly?)
 *      - Temperature (overheating?)
 *      - Storage (SD card full?)
 *      - Error logs (what happened at 2h mark?)
 *
 *   2. Request diagnostic dump:
 *      - Trigger via cloud command
 *      - Device uploads: logs, config, last N errors
 *
 *   3. Remote config change:
 *      - Reduce resolution temporarily
 *      - Enable verbose logging
 *      - See if problem persists
 *
 * ON-DEVICE DIAGNOSTICS:
 *
 *   - LED blink codes for common errors
 *   - Button combo to enter diagnostic mode
 *   - USB serial output for detailed debug
 *
 * COMMON FAILURE MODES:
 *
 *   | Symptom              | Likely Cause                   |
 *   |----------------------|--------------------------------|
 *   | Stops after 2h       | Thermal throttling, bad SD     |
 *   | Random reboots       | Power supply, watchdog trips   |
 *   | Won't start          | Dead battery, corrupted flash  |
 *   | Video corruption     | SD card failing, EMI           |
 *   | WiFi won't connect   | Antenna issue, wrong creds     |
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ============================================================================
// DEMO: Power Manager for 8-hour wearable camera
// ============================================================================

typedef enum {
    POWER_ACTIVE,      // Full recording mode
    POWER_LOW,         // Reduced quality (720p/15fps)
    POWER_CRITICAL,    // Stop recording, save state
    POWER_CHARGING     // USB connected
} PowerState;

typedef struct {
    PowerState state;
    uint8_t battery_percent;
    bool usb_connected;
    bool recording_active;
} PowerManager;

void power_init(PowerManager* pm) {
    pm->state = POWER_ACTIVE;
    pm->battery_percent = 100;
    pm->usb_connected = false;
    pm->recording_active = false;
}

void power_update(PowerManager* pm, uint8_t battery, bool usb) {
    pm->battery_percent = battery;
    pm->usb_connected = usb;

    PowerState prev = pm->state;

    if (usb) {
        pm->state = POWER_CHARGING;
    } else if (battery < 10) {
        pm->state = POWER_CRITICAL;
    } else if (battery < 20) {
        pm->state = POWER_LOW;
    } else {
        pm->state = POWER_ACTIVE;
    }

    if (pm->state != prev) {
        printf("Power state: %d -> %d (battery: %d%%)\n", prev, pm->state, battery);
    }
}

// ============================================================================
// DEMO: Video chunk manager (circular buffer of chunks)
// ============================================================================

#define MAX_CHUNKS 16

typedef struct {
    uint32_t timestamp;
    uint32_t size_bytes;
    uint8_t priority;      // 0=normal, 1=high (has event), 2=critical
    bool uploaded;
    char filename[32];
} VideoChunk;

typedef struct {
    VideoChunk chunks[MAX_CHUNKS];
    int write_idx;
    int count;
} ChunkManager;

void chunk_init(ChunkManager* cm) {
    cm->write_idx = 0;
    cm->count = 0;
}

void chunk_add(ChunkManager* cm, uint32_t timestamp, uint32_t size, uint8_t priority) {
    VideoChunk* c = &cm->chunks[cm->write_idx];
    c->timestamp = timestamp;
    c->size_bytes = size;
    c->priority = priority;
    c->uploaded = false;
    snprintf(c->filename, 32, "chunk_%08X.mp4", timestamp);

    cm->write_idx = (cm->write_idx + 1) % MAX_CHUNKS;
    if (cm->count < MAX_CHUNKS) cm->count++;

    printf("Added chunk: %s (priority: %d)\n", c->filename, priority);
}

VideoChunk* chunk_get_next_upload(ChunkManager* cm) {
    VideoChunk* best = NULL;

    // Find highest priority non-uploaded chunk
    for (int i = 0; i < cm->count; i++) {
        VideoChunk* c = &cm->chunks[i];
        if (!c->uploaded) {
            if (best == NULL || c->priority > best->priority) {
                best = c;
            }
        }
    }
    return best;
}

// ============================================================================
// DEMO: Simple fall detection using accelerometer
// ============================================================================

typedef struct {
    float x, y, z;
} AccelReading;

typedef struct {
    float magnitude_filtered;
    bool in_freefall;
    uint32_t freefall_start;
    bool fall_detected;
} FallDetector;

void fall_init(FallDetector* fd) {
    fd->magnitude_filtered = 1.0f;  // 1g at rest
    fd->in_freefall = false;
    fd->freefall_start = 0;
    fd->fall_detected = false;
}

#include <math.h>

void fall_update(FallDetector* fd, AccelReading* accel, uint32_t timestamp_ms) {
    // Calculate magnitude
    float mag = sqrtf(accel->x * accel->x +
                      accel->y * accel->y +
                      accel->z * accel->z);

    // EMA filter (alpha = 0.3)
    fd->magnitude_filtered = 0.3f * mag + 0.7f * fd->magnitude_filtered;

    // Detect free-fall (< 0.5g)
    if (fd->magnitude_filtered < 0.5f) {
        if (!fd->in_freefall) {
            fd->in_freefall = true;
            fd->freefall_start = timestamp_ms;
        } else if (timestamp_ms - fd->freefall_start > 200) {
            // Free-fall for > 200ms
            printf("Free-fall detected at %u ms\n", timestamp_ms);
        }
    } else {
        // Check for impact after free-fall
        if (fd->in_freefall && fd->magnitude_filtered > 3.0f) {
            printf("FALL DETECTED! Impact at %u ms\n", timestamp_ms);
            fd->fall_detected = true;
        }
        fd->in_freefall = false;
    }
}

// ============================================================================
// DEMO: Device state machine
// ============================================================================

typedef enum {
    STATE_INIT,
    STATE_IDLE,
    STATE_RECORDING,
    STATE_UPLOADING,
    STATE_LOW_POWER,
    STATE_SHUTDOWN
} DeviceState;

typedef enum {
    EVENT_INIT_DONE,
    EVENT_START_BTN,
    EVENT_STOP_BTN,
    EVENT_USB_CONNECT,
    EVENT_USB_DISCONNECT,
    EVENT_BATTERY_LOW,
    EVENT_BATTERY_CRITICAL,
    EVENT_UPLOAD_DONE
} DeviceEvent;

const char* state_names[] = {"INIT", "IDLE", "RECORDING", "UPLOADING", "LOW_POWER", "SHUTDOWN"};

typedef struct {
    DeviceState state;
    bool recording;
    bool uploading;
} DeviceFSM;

void fsm_init(DeviceFSM* fsm) {
    fsm->state = STATE_INIT;
    fsm->recording = false;
    fsm->uploading = false;
}

void fsm_handle_event(DeviceFSM* fsm, DeviceEvent event) {
    DeviceState prev = fsm->state;

    switch (fsm->state) {
        case STATE_INIT:
            if (event == EVENT_INIT_DONE) fsm->state = STATE_IDLE;
            break;

        case STATE_IDLE:
            if (event == EVENT_START_BTN) {
                fsm->state = STATE_RECORDING;
                fsm->recording = true;
            } else if (event == EVENT_USB_CONNECT) {
                fsm->state = STATE_UPLOADING;
                fsm->uploading = true;
            }
            break;

        case STATE_RECORDING:
            if (event == EVENT_STOP_BTN) {
                fsm->state = STATE_IDLE;
                fsm->recording = false;
            } else if (event == EVENT_BATTERY_LOW) {
                fsm->state = STATE_LOW_POWER;
            } else if (event == EVENT_USB_CONNECT) {
                fsm->state = STATE_UPLOADING;
                fsm->recording = false;
                fsm->uploading = true;
            }
            break;

        case STATE_UPLOADING:
            if (event == EVENT_UPLOAD_DONE || event == EVENT_USB_DISCONNECT) {
                fsm->state = STATE_IDLE;
                fsm->uploading = false;
            }
            break;

        case STATE_LOW_POWER:
            if (event == EVENT_BATTERY_CRITICAL) {
                fsm->state = STATE_SHUTDOWN;
                fsm->recording = false;
            } else if (event == EVENT_USB_CONNECT) {
                fsm->state = STATE_UPLOADING;
            }
            break;

        case STATE_SHUTDOWN:
            // Terminal state - device powers off
            break;
    }

    if (fsm->state != prev) {
        printf("State: %s -> %s\n", state_names[prev], state_names[fsm->state]);
    }
}

// ============================================================================
// MAIN: Demo all components
// ============================================================================

int main() {
    printf("=== IRONSITE Wearable Camera System Demo ===\n\n");

    // 1. Power management demo
    printf("--- Power Management ---\n");
    PowerManager pm;
    power_init(&pm);
    power_update(&pm, 100, false);  // Full battery
    power_update(&pm, 18, false);   // Low battery
    power_update(&pm, 8, false);    // Critical
    power_update(&pm, 8, true);     // USB connected

    // 2. Chunk management demo
    printf("\n--- Video Chunk Manager ---\n");
    ChunkManager cm;
    chunk_init(&cm);
    chunk_add(&cm, 1000, 50000000, 0);  // Normal chunk
    chunk_add(&cm, 2000, 50000000, 0);  // Normal chunk
    chunk_add(&cm, 3000, 50000000, 2);  // Critical (fall detected)

    VideoChunk* next = chunk_get_next_upload(&cm);
    if (next) {
        printf("Next upload: %s (priority %d)\n", next->filename, next->priority);
    }

    // 3. State machine demo
    printf("\n--- Device State Machine ---\n");
    DeviceFSM fsm;
    fsm_init(&fsm);
    fsm_handle_event(&fsm, EVENT_INIT_DONE);
    fsm_handle_event(&fsm, EVENT_START_BTN);
    fsm_handle_event(&fsm, EVENT_BATTERY_LOW);
    fsm_handle_event(&fsm, EVENT_USB_CONNECT);
    fsm_handle_event(&fsm, EVENT_UPLOAD_DONE);

    // 4. Fall detection demo
    printf("\n--- Fall Detection ---\n");
    FallDetector fd;
    fall_init(&fd);

    AccelReading readings[] = {
        {0.0f, 0.0f, 1.0f},   // Normal (1g down)
        {0.0f, 0.0f, 0.3f},   // Free-fall start
        {0.0f, 0.0f, 0.2f},   // Free-fall
        {0.0f, 0.0f, 0.2f},   // Free-fall continues
        {2.0f, 2.0f, 3.0f},   // Impact!
        {0.0f, 0.0f, 1.0f},   // Back to rest
    };

    for (int i = 0; i < 6; i++) {
        fall_update(&fd, &readings[i], i * 100);
    }

    return 0;
}

/*
 * ============================================================================
 *                    LIKELY CODING QUESTIONS FOR IRONSITE
 * ============================================================================
 *
 * 1. CIRCULAR BUFFER
 *    - For video chunks, sensor data
 *    - Know: head/tail pointers, wrap-around, full/empty detection
 *
 * 2. STATE MACHINE
 *    - Device modes: recording, uploading, idle, low power
 *    - Event-driven transitions
 *
 * 3. RING BUFFER FOR IMU DATA
 *    - Fixed-size buffer for accelerometer samples
 *    - Calculate rolling average
 *
 * 4. BIT MANIPULATION
 *    - Register configuration for camera/IMU
 *    - Status flags packing
 *
 * 5. TIMER/SCHEDULER
 *    - Periodic sensor reads
 *    - Watchdog petting
 *
 * 6. LINKED LIST
 *    - Upload queue with priorities
 *
 * 7. CRC/CHECKSUM
 *    - Video chunk integrity
 *    - OTA firmware verification
 *
 * 8. FILTERING (EMA)
 *    - IMU noise filtering
 *    - Battery voltage smoothing
 *
 * ============================================================================
 *                    BEHAVIORAL/SITUATIONAL QUESTIONS
 * ============================================================================
 *
 * Q: "Device works in lab but fails on construction site. Debug approach?"
 * A: 1. Check environmental differences (temp, vibration, EMI)
 *    2. Review telemetry from failed devices
 *    3. Add more logging, reproduce in controlled harsh conditions
 *    4. Check power supply stability under vibration
 *    5. EMI from heavy machinery affecting camera/IMU
 *
 * Q: "How do you prioritize features for battery-constrained device?"
 * A: 1. Profile power consumption of each feature
 *    2. Core function (video) is non-negotiable
 *    3. ML inference: reduce frequency, not accuracy
 *    4. WiFi: batch uploads, not continuous
 *    5. Make trade-offs configurable (resolution, frame rate)
 *
 * Q: "OTA update bricks 10% of devices in field. What went wrong?"
 * A: 1. Rollback mechanism failed or wasn't implemented
 *    2. Update interrupted (battery died, connection lost)
 *    3. Insufficient validation before marking update complete
 *    4. Hardware variation not accounted for
 *    FIX: Always have A/B partitions, verify before commit,
 *         require battery > 50%, staged rollout to catch issues
 *
 * Q: "Customer wants 12-hour runtime but current design only does 8. Options?"
 * A: 1. Larger battery (weight/cost trade-off)
 *    2. Reduce resolution/frame rate option
 *    3. Motion-triggered recording (not continuous)
 *    4. Lower power MCU for idle periods
 *    5. External battery pack accessory
 *
 * ============================================================================
 *                         THINGS TO RESEARCH BEFORE INTERVIEW
 * ============================================================================
 *
 * 1. ESP32 power modes (active, modem sleep, light sleep, deep sleep)
 * 2. H.264 encoding basics (I-frames, P-frames, bitrate)
 * 3. TensorFlow Lite Micro for edge ML
 * 4. FreeRTOS basics (tasks, queues, semaphores)
 * 5. I2C/SPI for camera and IMU communication
 * 6. Secure boot and flash encryption on ESP32
 * 7. WiFi provisioning methods (AP mode, BLE, SmartConfig)
 *
 * ============================================================================
 *                              GOOD LUCK!
 * ============================================================================
 */
