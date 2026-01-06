/*******************************************************************************
 * MOCK INTERVIEW Q&A ROLE-PLAY SCENARIOS
 * 50 Real Interview Questions with Answers and "What If" Follow-ups
 *
 * How to Use:
 * 1. Have someone read the INTERVIEWER parts to you
 * 2. Practice answering WITHOUT looking at the CANDIDATE response
 * 3. Prepare for the WHAT-IF follow-ups - interviewers love these
 * 4. Time yourself: Initial answer (30-60s), Follow-ups (30s each)
 ******************************************************************************/

/*******************************************************************************
 * SECTION 1: RESUME WALKTHROUGH (Questions 1-10)
 * These open the interview - nail these first impressions
 ******************************************************************************/

// =============================================================================
// QUESTION 1: The Opening
// =============================================================================
/*
INTERVIEWER: "Tell me about yourself and your experience in embedded systems."

CANDIDATE RESPONSE:
"I'm a firmware engineer with hands-on experience across the full embedded stack -
from bare-metal STM32 programming to RTOS-based systems with cloud connectivity.

At ePlant Inc, I optimized LoRaWAN sensor firmware, reducing boot time by 40% and
field failures by 80% through a multi-slot A/B bootloader. I also developed NFC-based
provisioning that cut setup time by 70%.

Before that, at Devlata Technologies, I built a wireless crane control system using
NRF905 RF modules, achieving sub-2ms latency for safety-critical relay control.

I've also worked on research projects at IIT Bombay - a telematics device using IMU
sensors and Allan Variance analysis for driver behavior monitoring.

I'm passionate about writing efficient, reliable firmware for resource-constrained
systems."

WHAT-IF FOLLOW-UPS:

Q: "What draws you to embedded over other software fields?"
A: "I love the tangible aspect - seeing code control physical hardware. There's also
   the optimization challenge: when you only have 64KB of flash, every byte matters.
   And debugging with oscilloscopes and logic analyzers feels like detective work."

Q: "Which project are you most proud of?"
A: "The FUOTA bootloader at ePlant. It took a system with 20% field failures down to
   under 4%. Knowing that 500+ devices in the field are reliably updating because of
   my code - that's satisfying."

Q: "What's your biggest weakness as an embedded engineer?"
A: "I sometimes over-optimize prematurely. I've learned to profile first, then optimize
   the actual bottlenecks rather than guessing. At ePlant, I initially focused on code
   size when boot TIME was the real issue."
*/

// =============================================================================
// QUESTION 2: Deep Dive Request
// =============================================================================
/*
INTERVIEWER: "Pick one project and walk me through it in detail."

CANDIDATE RESPONSE (LoRaWAN Optimization):
"I'll walk you through the boot time optimization at ePlant.

THE PROBLEM:
Our LoRaWAN sensors took 150 seconds to boot - unacceptable for battery-powered
devices. Users would think the device was dead.

MY INVESTIGATION:
I added GPIO toggles at key boot stages and measured with an oscilloscope. I found:
- 40% of time: Link check mechanism waiting for network confirmation
- 30% of time: Full network join even when session was valid
- 30% of time: Sensor initialization and self-tests

MY SOLUTION:
1. Implemented 'network lock mode' - persist session data (DevAddr, frame counters,
   channel config) to flash. On reboot, skip the join if session is valid.

2. Made link check optional - only run it every 24 hours or after communication
   failures, not every boot.

3. Parallelized sensor init with network tasks using FreeRTOS.

THE RESULT:
Boot time: 150s → 90s (40% reduction)
Battery life improved because we weren't wasting power on unnecessary network joins."

WHAT-IF FOLLOW-UPS:

Q: "What if the persisted session becomes invalid?"
A: "I implemented a validation check - if 3 consecutive uplinks fail, we invalidate
   the session and do a fresh join. The frame counter also has a safety margin to
   prevent replay attack rejections."

Q: "What if power fails during flash write of session data?"
A: "I used a shadow copy approach - write new data to slot B, then atomically update
   a validity flag. If power fails mid-write, slot A still has valid data."

Q: "How did you measure the 40%? Could you be more precise?"
A: "I used a logic analyzer on debug GPIOs. Boot start was rising edge on GPIO1,
   boot complete was rising edge on GPIO2. Averaged over 50 boot cycles. Standard
   deviation was about 3 seconds, so the measurement was reliable."
*/

// =============================================================================
// QUESTION 3: Technical Decision Defense
// =============================================================================
/*
INTERVIEWER: "Why did you choose 433MHz over 905MHz for the crane system?"

CANDIDATE RESPONSE:
"It came down to physics and the operating environment.

THE ENVIRONMENT:
The crane operates in a steel mill - lots of metal structures, interference, and
multipath reflections.

WHY 433MHz WON:
1. WAVELENGTH: 433MHz has ~70cm wavelength vs ~33cm for 905MHz. Longer wavelength
   diffracts better around metal obstacles.

2. PENETRATION: Lower frequencies penetrate steel structures better. We tested both
   and 433MHz maintained link where 905MHz dropped completely behind a steel beam.

3. MEASURED RESULTS: Packet success rate went from 62% at 905MHz to 90% at 433MHz -
   a 45% improvement in reliability.

TRADE-OFFS ACCEPTED:
- Larger antenna (17cm vs 8cm quarter-wave)
- Lower data rate (50kbps vs 100kbps) - but we only send 4 bytes per packet
- More crowded spectrum (ISM band) - mitigated with frequency hopping"

WHAT-IF FOLLOW-UPS:

Q: "What if you needed higher data rates?"
A: "For 4-byte relay commands, 50kbps is more than enough - packet takes only 640µs.
   If we needed video or high-speed telemetry, I'd consider 2.4GHz with directional
   antennas, but that's a different application."

Q: "What about regulatory differences between 433MHz and 905MHz?"
A: "In India, both are ISM bands. 433MHz has a 10mW limit, 905MHz allows 1W. But we
   were power-limited by battery anyway, and the better propagation of 433MHz more
   than compensated."

Q: "How did you handle interference from other 433MHz devices?"
A: "NRF905 supports 10 frequency channels. I implemented a simple listen-before-talk:
   check RSSI, if above threshold, hop to next channel. Also used unique addresses
   and CRC validation."
*/

// =============================================================================
// QUESTION 4: Failure and Recovery
// =============================================================================
/*
INTERVIEWER: "Tell me about a time when something went wrong in production."

CANDIDATE RESPONSE:
"At ePlant, we had a silent failure mode that affected about 15% of deployed sensors.

THE SYMPTOM:
Devices would stop sending data after 2-3 weeks. Power cycling fixed it temporarily.

MY INVESTIGATION:
1. Added remote diagnostics - dump frame counters, session state, error counts via
   downlink command.
2. Found that frame counters were wrapping incorrectly - we were using 16-bit counters
   but the LoRaWAN spec uses 32-bit.
3. After 65,535 messages, counter wrapped to 0. Network server rejected as replay attack.

THE FIX:
1. Immediate: OTA update to use 32-bit counters with proper persistence
2. Preventive: Added watchdog that resets session if uplinks fail 10 times consecutively
3. Monitoring: Cloud dashboard now alerts if any device goes silent for 24 hours

LESSON LEARNED:
Always test boundary conditions. I now have a checklist: what happens at 0, 1, max-1,
max, and max+1 for any counter or buffer."

WHAT-IF FOLLOW-UPS:

Q: "How did you push the fix if devices weren't communicating?"
A: "About 85% were still working. For the failed 15%, we had to dispatch field
   technicians to manually power cycle and force a rejoin. The new firmware then
   updated via FUOTA. This is why I later built the robust A/B bootloader."

Q: "What if you couldn't reproduce the bug in the lab?"
A: "I initially couldn't - it took weeks to trigger. So I added time-acceleration
   testing: artificially increment frame counter by 1000 per message. Hit the wrap
   condition in hours instead of weeks."

Q: "How did this affect your testing process going forward?"
A: "I created a 'boundary condition' test suite that specifically tests max values,
   wraparounds, and edge cases. It's now part of our CI pipeline before any release."
*/

// =============================================================================
// QUESTION 5: Collaboration and Conflict
// =============================================================================
/*
INTERVIEWER: "Tell me about a time you disagreed with a teammate on a technical decision."

CANDIDATE RESPONSE:
"At ePlant, we debated the bootloader architecture.

THE DISAGREEMENT:
My colleague wanted a single-slot bootloader with differential updates (only send
changed bytes). I advocated for a dual-slot A/B bootloader with full image updates.

HIS ARGUMENT:
- Smaller OTA packages (save bandwidth)
- Faster updates (less to download)
- We were bandwidth-constrained on LoRaWAN

MY ARGUMENT:
- Differential updates are fragile - one bit error corrupts everything
- A/B allows instant rollback if new firmware crashes
- We had 80% field failure rate; reliability mattered more than bandwidth

HOW WE RESOLVED IT:
I proposed a hybrid: A/B slots for reliability, but with delta compression for the
transfer. We prototyped both approaches over a weekend.

Test results: His approach had 35% failure rate on simulated packet loss. Mine had
2% with A/B rollback catching the failures. Data won the argument.

THE OUTCOME:
We implemented A/B bootloader. Field failures dropped from 20% to under 4%. He
acknowledged the data was convincing, and we've collaborated well since."

WHAT-IF FOLLOW-UPS:

Q: "What if the data had supported his approach?"
A: "I would have accepted it. I care about the right solution, not being right. The
   weekend prototype was specifically designed so data would decide, not opinions."

Q: "What if he had pulled rank or refused to test?"
A: "I'd have escalated to our manager with a clear proposal: 'Can we spend one weekend
   testing both approaches before committing?' Frame it as risk reduction, not conflict."

Q: "How do you handle disagreements with less technical stakeholders?"
A: "I translate to business impact. Instead of 'A/B bootloader has atomic commits,'
   I say 'This approach means 80% fewer truck rolls to fix bricked devices.' Money
   and time resonate with everyone."
*/

// =============================================================================
// QUESTION 6: Resource Constraints
// =============================================================================
/*
INTERVIEWER: "How do you handle working with limited resources - memory, power, etc.?"

CANDIDATE RESPONSE:
"Resource constraints are the core challenge of embedded. Here's my approach:

MEMORY EXAMPLE (ePlant - 256KB Flash, 64KB RAM):
Problem: New Protobuf parsing library needed 40KB RAM - we only had 20KB free.
Solution:
1. Switched from dynamic allocation to static buffers sized for max message
2. Used streaming parser instead of building full message tree in RAM
3. Moved lookup tables to flash using PROGMEM/const
Result: 40KB → 12KB RAM usage

POWER EXAMPLE (LoRaWAN Sensors - 2x AA batteries, 5-year life):
Problem: Device waking every 15 minutes consumed too much power.
Solution:
1. Profiled with current meter - found radio warm-up was 60% of wake cycle
2. Pre-computed TX parameters during previous wake cycle
3. Used RTC for precise timing instead of polling
Result: 8µA average current, achieving 7-year battery life

MY GENERAL PROCESS:
1. MEASURE first - don't guess where resources go
2. PROFILE the top 3 consumers
3. ATTACK the biggest one with smallest code change
4. REPEAT until target met"

WHAT-IF FOLLOW-UPS:

Q: "What if you truly can't fit the feature in available memory?"
A: "First, challenge the requirement - do we need full feature or can we subset it?
   Second, can we execute from external flash or SD card? Third, can we process in
   chunks instead of all at once? If none work, we need different hardware."

Q: "How do you balance optimization time vs. just using bigger hardware?"
A: "Depends on volume. For 10 units, spend $2 more on bigger MCU. For 100,000 units,
   spending a week to save $0.50/unit saves $50,000. I always ask about production
   volume before optimizing."

Q: "What's the most extreme resource constraint you've worked with?"
A: "ATmega328P on the SunDay Wellness Tracker - 2KB RAM, 32KB flash. I had to
   implement sensor fusion, BLE communication, and UI in that space. Used fixed-point
   math instead of floating-point, bit-packed structures, and no dynamic allocation."
*/

// =============================================================================
// QUESTION 7: Learning New Technology
// =============================================================================
/*
INTERVIEWER: "Tell me about a time you had to learn something new quickly."

CANDIDATE RESPONSE:
"At IIT Bombay, I had to learn Allan Variance analysis for IMU characterization -
a statistical technique I'd never used before.

THE CONTEXT:
We were building a telematics device for insurance companies. They needed accurate
acceleration data to detect harsh braking. But our IMU data was noisy.

MY LEARNING PROCESS:
1. READ: Started with IEEE papers on IMU error modeling. Dense, but got the theory.
2. IMPLEMENT: Wrote Python code to compute Allan Variance from sample data.
3. VALIDATE: Compared my results against known IMU datasheets.
4. APPLY: Used it to identify that our noise was primarily bias instability, not
   white noise - which told me Savitzky-Golay filtering would help.

TIMELINE: About 2 weeks from 'what is Allan Variance?' to actionable results.

THE OUTCOME:
20% reduction in IMU RMS noise. The insurance company could now reliably detect
harsh braking events, leading to their 30% premium adjustment program."

WHAT-IF FOLLOW-UPS:

Q: "What if you couldn't find good documentation?"
A: "I'd look for reference implementations on GitHub, reach out to domain experts on
   forums like Stack Exchange or EDAboard, or email paper authors directly -
   academics are often happy to clarify their work."

Q: "How do you balance learning vs. delivering?"
A: "I set a timebox. 'I'll spend 3 days learning, then decide if I know enough to
   proceed or need help.' Prevents both analysis paralysis and rushing in blind."

Q: "What's on your learning list right now?"
A: "Rust for embedded systems. I've done some tutorials, built a blinking LED on
   STM32. The memory safety guarantees are compelling for safety-critical systems,
   but the ecosystem is still maturing."
*/

// =============================================================================
// QUESTION 8: Debugging Strategy
// =============================================================================
/*
INTERVIEWER: "Walk me through how you debug a problem you've never seen before."

CANDIDATE RESPONSE:
"I follow a systematic process. Let me give you a real example.

THE BUG:
NRF905 crane controller worked in the lab but failed intermittently on the factory
floor. Commands would randomly not execute.

MY DEBUGGING PROCESS:

1. REPRODUCE: Spent 2 hours on factory floor. Failures happened near the main
   power switchgear - hypothesis: EMI interference.

2. ISOLATE: Wrapped controller in foil (crude Faraday cage). Failures stopped.
   Confirmed EMI hypothesis.

3. INSTRUMENT: Added SPI bus monitoring. Saw occasional bit flips during
   transmission - noise was coupling into the SPI lines.

4. ROOT CAUSE: 30cm SPI wires were acting as antennas. Clock line was most
   susceptible.

5. FIX:
   - Shortened SPI wires to 5cm
   - Added 100pF capacitors on clock line
   - Reduced SPI clock from 1MHz to 500kHz
   - Added software CRC check and retry

6. VALIDATE: 8-hour stress test near switchgear - zero failures.

KEY PRINCIPLE:
Don't guess. Measure. Every hypothesis should lead to a test that proves or
disproves it."

WHAT-IF FOLLOW-UPS:

Q: "What if you can't reproduce the bug?"
A: "Add extensive logging and wait. For the frame counter wrap bug at ePlant, I
   added remote diagnostics that dumped state every hour to cloud. When bug hit,
   I had the data to analyze."

Q: "What tools do you rely on most?"
A: "Logic analyzer for protocol issues, oscilloscope for timing/analog issues,
   JTAG debugger for software bugs, and current meter for power issues. For this
   EMI bug, the logic analyzer was key - showed the bit flips in real-time."

Q: "How do you debug intermittent issues?"
A: "Statistics. Run 1000 iterations, log everything, look for patterns. Often the
   intermittent bug correlates with temperature, timing, or specific data values.
   The pattern reveals the cause."
*/

// =============================================================================
// QUESTION 9: Code Quality and Testing
// =============================================================================
/*
INTERVIEWER: "How do you ensure code quality in embedded systems?"

CANDIDATE RESPONSE:
"Embedded is hard to test, so I use a multi-layered approach:

LAYER 1 - STATIC ANALYSIS:
- Compile with -Wall -Wextra -Werror
- Run PC-lint or cppcheck for embedded-specific issues
- Catches null pointer dereferences, array bounds, type mismatches

LAYER 2 - UNIT TESTS ON HOST:
- Abstract hardware behind interfaces
- Test algorithms on PC with Unity or Google Test
- Example: Protobuf parsing logic runs 1000x faster on PC than target

LAYER 3 - HIL (Hardware-in-the-Loop):
- At Span.IO, I built HIL rigs with Raspberry Pi controlling relays/signals
- Automated test: 'Apply 12V to pin 3, verify CAN message within 100ms'
- Runs 200+ tests in 10 minutes vs. 2 hours manually

LAYER 4 - PRODUCTION MONITORING:
- Remote diagnostics in firmware
- Cloud dashboard for anomaly detection
- Catches issues that lab testing misses

SPECIFIC METRICS:
At ePlant, this approach caught 85% of bugs before release. The remaining 15%
were environment-specific (temperature, RF interference) that only showed in field."

WHAT-IF FOLLOW-UPS:

Q: "How do you test interrupt handlers?"
A: "Difficult on host, so I focus on: keeping ISRs minimal (just set flags),
   testing the flag-handling code on host, and using HIL with signal generators
   to trigger real interrupts on target."

Q: "What's your code review process?"
A: "All code reviewed before merge. I look for: boundary conditions, error handling,
   race conditions in ISRs, and resource cleanup. For safety-critical code, we do
   two independent reviews."

Q: "How do you balance test coverage vs. shipping speed?"
A: "I prioritize: 100% coverage on safety-critical paths, 80% on core functionality,
   50% on utilities. The Pareto principle - 80% of bugs are in 20% of code. Test
   that 20% thoroughly."
*/

// =============================================================================
// QUESTION 10: Understanding Business Context
// =============================================================================
/*
INTERVIEWER: "How do you connect your technical work to business outcomes?"

CANDIDATE RESPONSE:
"Every technical metric should map to a business metric. Here's how I think about it:

EXAMPLE 1 - BOOT TIME OPTIMIZATION:
Technical: 40% boot time reduction (150s → 90s)
Business: Users don't think device is dead → fewer support calls → lower support cost
Business: Faster deployment → technicians install more units per day → lower labor cost

EXAMPLE 2 - FUOTA BOOTLOADER:
Technical: 80% reduction in field failures
Business: Fewer truck rolls to fix bricked devices → at $150/visit × 500 devices ×
         20% failure rate = $15,000 saved per firmware update cycle

EXAMPLE 3 - NFC PROVISIONING:
Technical: 70% setup time reduction (12 min → 3.5 min)
Business: 100 devices × 8.5 minutes saved = 14 hours saved per deployment batch
Business: Fewer configuration errors → fewer RMAs

I ALWAYS ASK:
- How many units will this ship on?
- What's the cost of a field failure?
- Who is the end user and what frustrates them?

This context helps me prioritize. A 10% improvement on 100,000 units matters more
than a 50% improvement on 100 units."

WHAT-IF FOLLOW-UPS:

Q: "What if engineering and business priorities conflict?"
A: "I present data: 'This technical debt will cost us X hours per release. At our
   release frequency, that's Y dollars over Z months.' Make the cost visible, then
   let leadership decide priorities with full information."

Q: "How do you handle features that are technically cool but low business value?"
A: "I propose them as 'slack time' projects or hackathon work. If I can prototype in
   off hours and show value, it often gets prioritized. But I don't push low-value
   work into the main roadmap."
*/

/*******************************************************************************
 * SECTION 2: TECHNICAL DEEP DIVES (Questions 11-25)
 * Interviewers will probe your knowledge - be ready to go deep
 ******************************************************************************/

// =============================================================================
// QUESTION 11: RTOS Fundamentals
// =============================================================================
/*
INTERVIEWER: "Explain priority inversion and how you've dealt with it."

CANDIDATE RESPONSE:
"Priority inversion is when a high-priority task is blocked waiting for a resource
held by a low-priority task, and a medium-priority task preempts the low-priority
task - effectively inverting the priorities.

CLASSIC EXAMPLE:
- Low priority task L holds mutex M
- High priority task H needs mutex M, blocks
- Medium priority task M runs (doesn't need M)
- H is now waiting on L, but L can't run because M is running
- H's effective priority is below M - inverted!

REAL-WORLD CASE (SafeScan Project):
I had an RFID reading task (high priority) and a logging task (low priority) sharing
an I2C bus mutex. UI refresh (medium priority) would starve the RFID task.

MY SOLUTION:
Enabled priority inheritance in Zephyr RTOS:
k_mutex_init(&i2c_mutex);  // Zephyr uses priority inheritance by default

Now when H blocks on mutex held by L, L temporarily inherits H's priority. L runs,
releases mutex, H continues. M can't preempt during this.

ALTERNATIVE SOLUTIONS:
1. Priority ceiling: Assign mutex a ceiling priority, any holder runs at that level
2. Avoid sharing: Duplicate resources (not always possible)
3. Lock-free algorithms: Use atomic operations instead of mutexes"

WHAT-IF FOLLOW-UPS:

Q: "What if priority inheritance causes a different task to starve?"
A: "That's priority ceiling's advantage - it's more predictable. With inheritance,
   you can get chains: A waits on B waits on C. Priority ceiling prevents chains by
   boosting immediately to ceiling. Trade-off is more time at high priority."

Q: "How do you debug priority inversion in the field?"
A: "Add tracing: log task switches and mutex acquisitions with timestamps. Tools
   like Tracealyzer visualize this beautifully. Look for patterns where high-priority
   task has unexpectedly long blocked periods."

Q: "Can priority inversion happen without mutexes?"
A: "Yes - with any blocking synchronization: semaphores, message queues, event flags.
   Even hardware resources like shared buses. Any time a high-priority task waits
   for a low-priority task to release something."
*/

// =============================================================================
// QUESTION 12: Memory Architecture
// =============================================================================
/*
INTERVIEWER: "Explain the difference between stack and heap, and when you'd use each."

CANDIDATE RESPONSE:
"STACK:
- Automatic, compiler-managed
- Fast allocation (just move stack pointer)
- LIFO - variables die when function returns
- Limited size (typically 1-8KB per task in RTOS)
- Use for: local variables, function parameters, return addresses

HEAP:
- Manual allocation (malloc/free)
- Slower (must search free list)
- Flexible lifetime - survives function return
- Fragmentation risk over time
- Use for: dynamic-sized data, long-lived objects, large buffers

MY EMBEDDED PREFERENCE - AVOID HEAP:
In most embedded projects, I avoid heap entirely:
1. Use static allocation with fixed-size pools
2. Pre-allocate at startup, never during runtime
3. Use stack for small temporary buffers

REAL EXAMPLE (ePlant Protobuf):
Instead of:
  Message* msg = malloc(sizeof(Message));  // Bad - fragmentation risk

I used:
  static Message msg_pool[MAX_CONCURRENT_MESSAGES];  // Good - predictable
  static uint8_t pool_used[MAX_CONCURRENT_MESSAGES];

This gives heap-like flexibility with stack-like predictability."

WHAT-IF FOLLOW-UPS:

Q: "What causes stack overflow and how do you detect it?"
A: "Causes: deep recursion, large local arrays, excessive nested interrupts.
   Detection: paint stack with sentinel value (0xDEADBEEF) at startup, periodically
   check if sentinel is corrupted. FreeRTOS has uxTaskGetStackHighWaterMark()."

Q: "When is heap actually appropriate in embedded?"
A: "1. Linux-based systems with virtual memory and garbage collection
   2. Prototyping where time-to-market beats robustness
   3. Processing variable-length data where max size is unknown
   But even then, I prefer pool allocators over raw malloc."

Q: "How do you size task stacks?"
A: "Start with worst-case analysis: deepest call chain × average frame size. Add
   margin for interrupts. Then validate empirically with high-water mark checking.
   I typically add 25% margin to measured max usage."
*/

// =============================================================================
// QUESTION 13: Communication Protocols
// =============================================================================
/*
INTERVIEWER: "Compare SPI and I2C. When would you choose one over the other?"

CANDIDATE RESPONSE:
"SPI:
- Full duplex, 4 wires (MOSI, MISO, CLK, CS per device)
- Master generates clock, up to 50+ MHz
- No addressing - CS selects device
- Simple protocol, low overhead
- Best for: high-speed, point-to-point (displays, flash, ADCs)

I2C:
- Half duplex, 2 wires (SDA, SCL)
- 100kHz standard, 400kHz fast, 3.4MHz high-speed
- 7-bit addressing - up to 127 devices on same bus
- ACK/NAK for error detection, clock stretching for flow control
- Best for: multiple sensors, limited pins, moderate speed

MY DECISION MATRIX:
- Need >1MHz? → SPI
- Pin-constrained with many devices? → I2C
- Need full duplex? → SPI
- Need multi-master? → I2C
- Simple sensor with occasional reads? → I2C
- Streaming data (ADC, display)? → SPI

REAL EXAMPLE (SafeScan):
I used BOTH on the same system:
- SPI for PN532 RFID reader (needs fast, bursty communication for card reads)
- I2C for temperature sensor and EEPROM (infrequent, small data)"

WHAT-IF FOLLOW-UPS:

Q: "What's clock stretching and when is it a problem?"
A: "Clock stretching: slave holds SCL low to pause master when it needs time.
   Problem: bit-banged I2C masters often don't support it. If slave stretches,
   master keeps clocking, transaction corrupts. Solution: use hardware I2C or
   verify slave doesn't stretch."

Q: "How do you debug I2C issues?"
A: "Logic analyzer on SDA/SCL is essential. Common issues:
   1. No ACK: wrong address or device not powered
   2. Bus stuck low: failed transaction, need to toggle clock to reset
   3. Data corruption: check for noise, add pull-up resistors (2.2k-10k typical)"

Q: "What about UART?"
A: "UART is async (no shared clock), point-to-point, simple but slow. I use it for
   debug consoles, GPS modules, and legacy sensors. Key advantage: works over long
   distances with RS-485 transceivers."
*/

// =============================================================================
// QUESTION 14: Interrupt Handling
// =============================================================================
/*
INTERVIEWER: "What are the rules for writing good ISRs?"

CANDIDATE RESPONSE:
"THE CARDINAL RULES:

1. KEEP IT SHORT
   - Do minimum work: set flags, copy data to buffer
   - Defer processing to main loop or task
   - Target: <10µs for most ISRs, <100µs absolute max

2. NO BLOCKING
   - Never call malloc, printf, or mutex lock
   - No floating-point on some architectures (save/restore overhead)
   - Use non-blocking ring buffers for data transfer

3. VOLATILE FOR SHARED DATA
   - Variables shared with main code must be volatile
   - Prevents compiler from optimizing away reads

4. ATOMIC ACCESS FOR MULTI-BYTE DATA
   - On 8-bit MCU, reading 32-bit variable isn't atomic
   - Either disable interrupts briefly, or use double-buffering

5. ACKNOWLEDGE THE INTERRUPT
   - Clear interrupt flag in hardware
   - If you forget, ISR re-triggers immediately (interrupt storm)

REAL EXAMPLE (SunDay Timer ISR):

volatile uint32_t milliseconds = 0;
volatile uint8_t new_data_flag = 0;

ISR(TIMER0_OVF_vect) {  // ~1ms interval
    milliseconds++;      // Quick increment
    if (milliseconds % 1000 == 0) {
        new_data_flag = 1;  // Signal to main loop
    }
    TCNT0 = TIMER_RELOAD;   // Reset counter
    // Total time: <5µs
}

// Main loop does the heavy lifting
void loop() {
    if (new_data_flag) {
        new_data_flag = 0;
        read_sensors();      // This takes 50ms - never in ISR!
        log_data();
    }
}"

WHAT-IF FOLLOW-UPS:

Q: "What if you need to do significant work in response to an interrupt?"
A: "Use deferred interrupt handling:
   - FreeRTOS: ISR gives semaphore, high-priority task takes it and does work
   - Linux: top half (ISR) schedules bottom half (softirq/tasklet)
   - Bare metal: ISR sets flag, main loop polls flag

   The ISR just signals; another context does the work."

Q: "How do you handle nested interrupts?"
A: "It depends on architecture. On ARM Cortex-M, NVIC handles priority-based nesting
   automatically. I ensure: high-priority ISRs are very short, lower-priority ones
   don't assume uninterrupted execution. I avoid nesting on simpler MCUs entirely."

Q: "What's the worst ISR bug you've seen?"
A: "A colleague had printf() in an ISR. Worked fine in debug builds (slow execution
   hid the problem). In release builds, ISR took too long, next interrupt arrived
   before it finished, stack overflow, crash. Classic violation of rule #2."
*/

// =============================================================================
// QUESTION 15: LoRaWAN Specifics
// =============================================================================
/*
INTERVIEWER: "Explain LoRaWAN's ADR mechanism and why it matters for battery life."

CANDIDATE RESPONSE:
"ADR - Adaptive Data Rate - is how LoRaWAN optimizes the trade-off between range
and battery life automatically.

HOW IT WORKS:
1. Device sends uplinks with ADR bit set ('I accept ADR commands')
2. Network server collects SNR (signal-to-noise ratio) from recent uplinks
3. If SNR shows margin, server sends ADRRequest: 'increase data rate / reduce power'
4. Device adjusts, next uplinks are shorter/lower power
5. If packets start failing, device automatically falls back

THE BATTERY IMPACT:
Example: SF12 (slowest) vs SF7 (fastest)
- SF12: 1.5 second airtime for 10-byte payload
- SF7: 40ms airtime for same payload
- That's 37x less radio-on time, roughly 10x better battery life

AT EPLANT, I IMPLEMENTED:
1. Custom ADR response handling - validated requested settings before applying
2. ADR backoff - if 64 consecutive uplinks fail, reset to SF12 and rejoin
3. Persistence - stored current settings to flash, avoided SF12 on every reboot

GOTCHA I DISCOVERED:
ADR assumes static devices. Our sensors were sometimes moved (technician relocates
unit). SNR suddenly changes, uplinks fail until ADR adapts. Solution: detect
movement via accelerometer, trigger immediate ADR reset."

WHAT-IF FOLLOW-UPS:

Q: "What if you're in a noisy RF environment and ADR keeps pushing data rate up?"
A: "I added hysteresis: require 20 good uplinks before increasing data rate, but
   drop immediately after 3 failures. Asymmetric response prevents oscillation in
   marginal conditions."

Q: "How does ADR interact with your boot time optimization?"
A: "Good catch. By persisting ADR settings, we boot with optimal data rate instead
   of SF12. This also saves battery - otherwise first uplink after reboot is
   unnecessarily long. It's part of why boot time matters."

Q: "What about Class C devices (always listening)?"
A: "Class C ignores battery optimization - they're mains-powered. ADR still helps
   by maximizing throughput. For ePlant, we were Class A (battery), so ADR was
   primarily about power savings."
*/

// =============================================================================
// QUESTION 16: Bootloader Design
// =============================================================================
/*
INTERVIEWER: "Walk me through your A/B bootloader architecture."

CANDIDATE RESPONSE:
"THE ARCHITECTURE:

Memory Layout:
┌─────────────────────────────────────┐
│ Bootloader (16KB) - never updates   │ 0x08000000
├─────────────────────────────────────┤
│ Slot A (120KB) - primary firmware   │ 0x08004000
├─────────────────────────────────────┤
│ Slot B (120KB) - backup/update      │ 0x08022000
├─────────────────────────────────────┤
│ Config (4KB) - boot flags, version  │ 0x0803F000
└─────────────────────────────────────┘

BOOT FLOW:
1. Bootloader reads config: which slot is 'active', which is 'pending'
2. If pending update exists:
   a. Verify pending slot CRC
   b. If valid, set pending as active, boot into it
   c. If invalid, log error, boot into current active
3. If no pending update, boot into active slot
4. Application runs self-test; if pass, marks slot as 'verified'
5. If crash/watchdog before verified, bootloader rolls back on next boot

ROLLBACK LOGIC:
- Boot counter increments each boot
- Application resets counter after successful init
- If counter reaches 3 without reset → slot is bad → revert to other slot

UPDATE PROCESS (via FUOTA):
1. Receive firmware chunks over LoRaWAN
2. Write to inactive slot (B if A is active)
3. After complete, verify CRC
4. Set B as 'pending', reboot
5. Bootloader switches to B, app verifies itself
6. If B crashes, bootloader reverts to A automatically"

WHAT-IF FOLLOW-UPS:

Q: "What if the bootloader itself has a bug?"
A: "Bootloader never updates over-the-air - that's intentional. Field update
   requires physical access (JTAG/SWD). I kept bootloader minimal: just CRC check,
   slot switching, and jump. Less code = fewer bugs."

Q: "How do you handle power failure during slot write?"
A: "The 'pending' flag is only set AFTER complete write and CRC verification. If
   power fails mid-write, pending flag is never set, bootloader ignores the
   partially-written slot. Next FUOTA restarts from scratch."

Q: "What about flash wear leveling?"
A: "STM32F4 flash is rated for 10,000 erase cycles. At one update per week, that's
   192 years - not a concern. But I did minimize config sector writes by batching
   multiple flag updates."
*/

// =============================================================================
// QUESTION 17: Security Considerations
// =============================================================================
/*
INTERVIEWER: "How did you handle security in the SafeScan RFID project?"

CANDIDATE RESPONSE:
"SafeScan handles sensitive hospital data - surgical instruments and their locations.
I implemented defense in depth:

1. ARM TRUSTZONE PARTITIONING:
   - Secure World: RFID cryptographic keys, patient data, attestation
   - Non-Secure World: UI, MQTT, general application logic
   - Hardware-enforced boundary - non-secure can't access secure memory

2. SECURE BOOT CHAIN:
   - Bootloader verifies application signature (ECDSA-256)
   - If signature invalid, refuse to boot, fall back to recovery mode
   - Keys stored in OTP (one-time programmable) - can't be extracted

3. ENCRYPTED COMMUNICATION:
   - MQTT over TLS 1.3 to cloud
   - Certificate pinning - reject unknown servers
   - RFID tags use AES-128 authentication

4. SECURE STORAGE:
   - Patient-linked data encrypted at rest
   - Key derived from device-unique ID + master key
   - Tamper detection: if enclosure opened, wipe keys

5. MINIMAL ATTACK SURFACE:
   - No debug interface in production (JTAG disabled via option bytes)
   - No USB/serial console
   - OTA updates only from signed, authorized server"

WHAT-IF FOLLOW-UPS:

Q: "What if someone clones an RFID tag?"
A: "NTAG 5 tags have originality signature - unique, unforgeable signature from NXP.
   Device verifies signature against NXP root certificate. Cloned tags fail
   verification. Also, each tag has a unique ID that's linked to specific instrument
   in backend database."

Q: "How do you handle key management?"
A: "Device keys provisioned during manufacturing in secure facility. Each device has
   unique keys derived from: master secret (in HSM) + device serial number. If device
   is compromised, only that device's keys are affected, not fleet-wide."

Q: "What if someone extracts firmware and reverse engineers it?"
A: "Read-out protection (RDP level 2) makes it infeasible to extract via debug.
   For determined attackers with decapping: sensitive algorithms are in Secure
   World, inaccessible even with firmware dump. Also, business logic is mostly
   server-side - device is a secure terminal, not the brains."
*/

// =============================================================================
// QUESTION 18: Wireless Protocol Trade-offs
// =============================================================================
/*
INTERVIEWER: "Why LoRaWAN for ePlant? Why not WiFi, Zigbee, or cellular?"

CANDIDATE RESPONSE:
"I evaluated all of them. Here's the analysis:

REQUIREMENTS:
- Range: 2-3km outdoor (agricultural fields)
- Battery: 5+ year life on 2xAA
- Data: 50 bytes every 15 minutes
- Cost: <$10 for radio module
- Infrastructure: minimal (customer shouldn't manage gateways)

WIFI:
✗ Power hungry (~100mA TX) → kills battery in weeks
✗ Range: 50-100m without repeaters
✗ Requires access point management
✓ High data rate (unnecessary for us)

ZIGBEE:
✓ Low power
✗ Range: 100m max, needs mesh for coverage
✗ Mesh complexity, routing overhead
✗ Customer needs coordinator hardware

CELLULAR (LTE-M/NB-IoT):
✓ Great range
✓ No customer infrastructure
✗ Power: still 10x more than LoRa for our payload
✗ Subscription cost: $1-5/month/device × 5 years = significant
✗ Module cost: $15-25 vs $5 for LoRa

LORAWAN:
✓ Range: 3-5km rural, 1-2km urban
✓ Power: 40mA TX but very short airtime
✓ Module cost: $5
✓ Network: public gateways available (The Things Network) or customer can add one
✗ Low data rate - but we only need 50 bytes

Winner: LoRaWAN by large margin for our specific use case."

WHAT-IF FOLLOW-UPS:

Q: "What if the customer needed real-time data?"
A: "Define 'real-time'. If <5s latency: Class C LoRaWAN (always listening) or
   cellular. If <100ms: definitely cellular or WiFi with reliable power. For ePlant,
   15-minute intervals were acceptable - soil moisture doesn't change fast."

Q: "How did you handle areas with no LoRaWAN coverage?"
A: "Two options we offered:
   1. Customer deploys their own gateway ($100-150, covers whole property)
   2. Cellular backhaul variant for remote sites (higher cost but autonomous)
   90% of customers chose option 1."

Q: "What about Bluetooth?"
A: "BLE is great for phone-to-device (I used it for NFC-less provisioning backup).
   But gateway-to-cloud needs infrastructure. BLE mesh exists but is complex and
   shorter range than LoRa. For field sensors, not a good fit."
*/

// =============================================================================
// QUESTION 19: Sensor Integration
// =============================================================================
/*
INTERVIEWER: "Tell me about integrating the IMU in the telematics device."

CANDIDATE RESPONSE:
"THE HARDWARE:
MPU-6050 IMU - 3-axis accelerometer + 3-axis gyroscope, I2C interface, $3 part.

THE CHALLENGE:
Raw IMU data is noisy. We needed to detect harsh braking events (>0.5g deceleration)
reliably, without false positives from road vibrations.

MY APPROACH:

1. CHARACTERIZE THE NOISE (Allan Variance):
   - Collected 24 hours of stationary data
   - Computed Allan Variance curve
   - Identified noise types: white noise + bias instability
   - Conclusion: need filtering, not just averaging

2. FILTER DESIGN:
   - Tested moving average: reduced noise but lagged real events
   - Tested Kalman filter: effective but computationally heavy
   - Selected Savitzky-Golay: polynomial fit preserves peak shape, smooths noise
   - Window: 7 samples, 2nd order polynomial

3. CALIBRATION:
   - Accelerometer bias: device flat for 30s at startup, average readings = offset
   - Scale factor: rotate device in known pattern, compare to expected values
   - Stored calibration in flash, re-run weekly

4. EVENT DETECTION:
   - After filtering, compute magnitude: sqrt(ax² + ay² + az²)
   - Threshold: 0.5g sustained for >100ms = harsh event
   - Log event with timestamp, duration, peak value

RESULTS:
- False positive rate: <1% (road bumps rejected)
- Detection rate: >99% for genuine harsh events
- 20% noise reduction vs raw data"

WHAT-IF FOLLOW-UPS:

Q: "How do you handle IMU orientation changes?"
A: "The device could be mounted in any orientation. I used initial 10s of data to
   detect gravity direction, then transformed all readings to vehicle frame
   (forward/lateral/vertical). This also compensated for sensor mounting angle."

Q: "What's the sampling rate and why?"
A: "100Hz. Nyquist says 2x highest frequency of interest. Harsh braking events have
   dynamics up to ~20Hz (vehicle suspension). 100Hz gives margin. Higher wastes
   power and memory with no accuracy benefit."

Q: "How do you distinguish braking from acceleration?"
A: "Sign of the longitudinal (forward) axis. Braking is negative acceleration in
   vehicle frame. I also correlate with GPS speed decrease for validation."
*/

// =============================================================================
// QUESTION 20: Power Management
// =============================================================================
/*
INTERVIEWER: "How do you design for 5-year battery life?"

CANDIDATE RESPONSE:
"5 years on 2xAA (3000mAh) means average current must be <70µA. Here's my approach:

POWER BUDGET CALCULATION:
Total capacity: 3000mAh × 2 = 6000mAh (parallel cells, nominal)
Usable (to 80% DoD): 4800mAh
5 years = 43,800 hours
Average current: 4800mAh / 43,800h = 110µA
With margin: target 70µA average

POWER MODES (STM32L4):
1. Run mode: 10mA - minimize time here
2. Low-power run: 100µA - used during sensor reads
3. Stop mode: 2µA - used between wake cycles
4. Standby: 300nA - used for deep sleep, loses RAM

OPTIMIZATION STRATEGIES:

1. SLEEP MOST OF THE TIME:
   - Wake every 15 minutes (RTC alarm)
   - Active time: ~5 seconds
   - Sleep time: 895 seconds
   - Duty cycle: 0.5%

2. MINIMIZE RADIO TIME:
   - Pre-compute TX parameters during processing
   - Use ADR to reduce airtime (SF7 vs SF12 = 30x difference)
   - Cache network session (avoid re-joins)

3. PERIPHERAL MANAGEMENT:
   - Power sensors from GPIO (turn off when not reading)
   - Disable unused peripherals (ADC, USART, timers)
   - Reduce clock speed during processing (8MHz vs 80MHz)

4. REDUCE WAKE-UPS:
   - Batch sensor readings (read 3 sensors in one wake)
   - Skip transmission if data unchanged (>5% threshold)
   - Use hardware comparators for threshold wake instead of polling

MEASURED RESULTS:
- Active: 15mA × 5s = 75mAs per cycle
- Sleep: 3µA × 895s = 2.7mAs per cycle
- Per cycle: 77.7mAs
- Per hour: 77.7mAs × 4 cycles = 310mAs = 0.086mAh
- Average: 86µA (with margin, within budget)"

WHAT-IF FOLLOW-UPS:

Q: "What's your biggest power consumer and how did you reduce it?"
A: "Radio TX was 80% of power budget. Reduced via:
   1. ADR optimization (shorter airtime)
   2. Confirmed vs unconfirmed uplinks (unconfirmed = no RX window)
   3. Antenna matching (better efficiency = less TX power needed)"

Q: "How do you measure such low currents accurately?"
A: "Current sense resistor + oscilloscope for active phases. For sleep:
   specialized nanoamp meter (Qoitech Otii) that samples at high rate.
   Standard multimeters can't capture the spiky load profile accurately."

Q: "What about temperature effects on battery capacity?"
A: "Alkaline capacity drops 40% at 0°C. For outdoor agricultural sensors:
   1. Derate capacity in calculations (use -20°C spec)
   2. Consider lithium primary cells (better temp performance)
   3. Add temperature monitoring - alert if battery gets too cold"
*/

// =============================================================================
// QUESTION 21-25: RAPID FIRE TECHNICAL
// =============================================================================

/*
QUESTION 21: "What's the difference between volatile and const?"

ANSWER:
"volatile: tells compiler the value can change unexpectedly (hardware register, ISR).
 Prevents optimization that would cache the value in a register.

const: value won't change through this pointer/variable.
 Compiler can optimize based on this knowledge.

They're not opposites - you can have 'volatile const':
  volatile const uint32_t* TIMER_COUNT = (uint32_t*)0x40000024;

This is a hardware register that we read but don't write. volatile because
hardware changes it; const because we shouldn't modify it."

WHAT-IF: "When would you need volatile?"
"1. Hardware registers (peripheral access)
 2. Variables modified by ISRs
 3. Variables accessed by multiple threads
 4. Memory-mapped I/O"
*/

/*
QUESTION 22: "Explain DMA and when you'd use it."

ANSWER:
"DMA - Direct Memory Access - moves data between memory and peripherals without
CPU involvement. CPU sets up transfer (source, dest, count), DMA handles transfer,
interrupts when done.

USE CASES:
1. ADC continuous sampling (ADC → buffer, zero CPU)
2. SPI bulk transfer (send 1KB to display without bit-banging)
3. UART receive (characters buffer automatically)
4. DAC waveform output (memory → DAC at precise rate)

EXAMPLE (Logic Analyzer):
ADC samples at 800kHz → DMA fills buffer A
When A full, DMA switches to buffer B, interrupts CPU
CPU processes A while DMA fills B (double buffering)
CPU never handles individual samples - 800kHz would overwhelm it."

WHAT-IF: "How do you debug DMA issues?"
"Common problems:
1. Alignment errors - some DMA requires word-aligned buffers
2. Cache coherency - CPU sees stale data if cache not invalidated
3. Priority conflicts - multiple DMA streams compete for bus
Debug: check transfer complete flag, verify buffer contents with debugger"
*/

/*
QUESTION 23: "What is a watchdog timer and why use it?"

ANSWER:
"Watchdog is an independent hardware timer that resets the MCU if not periodically
'kicked' by software. It's a last-resort recovery from software hangs.

HOW IT WORKS:
1. Configure timeout (e.g., 2 seconds)
2. Main loop or task periodically writes to WDT register ('kick')
3. If software hangs, no kick → WDT expires → MCU resets

MY IMPLEMENTATION (ePlant):
- Watchdog timeout: 8 seconds (slow radio operations allowed)
- Kick in main loop after successful work
- Before long operations: disable WDT, re-enable after
- On reset: check reset reason register → if WDT, log fault and enter safe mode

ADVANCED: Window watchdog
- Can't kick too early OR too late
- Catches bugs where code runs too fast (infinite loop) as well as hangs"

WHAT-IF: "What if the watchdog keeps resetting in a boot loop?"
"That's why I store reset counter in non-volatile memory. If WDT reset 3 times
consecutively without reaching 'verified' state, enter recovery mode -
minimal firmware that just accepts OTA updates."
*/

/*
QUESTION 24: "Explain SPI modes (CPOL/CPHA)."

ANSWER:
"SPI has 4 modes based on clock polarity and phase:

CPOL (Clock Polarity): idle state of clock
- CPOL=0: clock idles low
- CPOL=1: clock idles high

CPHA (Clock Phase): which edge samples data
- CPHA=0: sample on first edge (leading)
- CPHA=1: sample on second edge (trailing)

MODES:
Mode 0 (CPOL=0, CPHA=0): clock idles low, sample on rising edge ← Most common
Mode 1 (CPOL=0, CPHA=1): clock idles low, sample on falling edge
Mode 2 (CPOL=1, CPHA=0): clock idles high, sample on falling edge
Mode 3 (CPOL=1, CPHA=1): clock idles high, sample on rising edge ← Second most common

HOW I DETERMINE CORRECT MODE:
1. Read device datasheet for timing diagram
2. Look for 'CPOL/CPHA' or 'SPI mode' specification
3. If ambiguous, try mode 0 first (most devices use it)
4. Use logic analyzer to verify - wrong mode shows garbled data"

WHAT-IF: "What happens if you use the wrong mode?"
"Data is sampled at wrong time → bit shifts or garbage. Often see:
- All 0xFF or 0x00 reads
- Data shifted by 1 bit
- Works intermittently at low speed, fails at high speed"
*/

/*
QUESTION 25: "What's a race condition? Give an example."

ANSWER:
"A race condition occurs when outcome depends on timing/ordering of uncontrollable
events - typically multiple threads or ISR/main accessing shared data.

EXAMPLE:
uint32_t counter = 0;

void ISR() {
    counter++;  // Increment in interrupt
}

void main() {
    uint32_t local = counter;  // Read counter
    process(local);
    counter = 0;  // Reset counter
}

THE RACE:
1. main() reads counter (value: 5)
2. ISR fires, increments counter (value: 6)
3. main() process(5), then writes counter = 0
4. We just lost an increment! (6 → 0, not 6 → 1)

SOLUTION:
void main() {
    __disable_irq();
    uint32_t local = counter;
    counter = 0;
    __enable_irq();
    process(local);  // Safe to process outside critical section
}

Or use atomic operations if available."

WHAT-IF: "How do you detect race conditions?"
"They're notoriously hard to find because they're timing-dependent:
1. Code review - look for shared variables without protection
2. Static analysis tools (ThreadSanitizer, PC-lint)
3. Stress testing - run thousands of iterations, look for anomalies
4. Add assertions: if counter goes negative, something's wrong"
*/

/*******************************************************************************
 * SECTION 3: BEHAVIORAL QUESTIONS (Questions 26-40)
 * These assess fit and soft skills
 ******************************************************************************/

// =============================================================================
// QUESTION 26: Time Management
// =============================================================================
/*
INTERVIEWER: "Tell me about a time you had to balance multiple priorities."

CANDIDATE RESPONSE:
"At ePlant, I was simultaneously working on:
1. Critical bug fix - devices bricking in the field (high urgency)
2. New feature - NFC provisioning (high value, committed deadline)
3. Documentation update - for new team member onboarding

MY APPROACH:

1. TRIAGE BY IMPACT:
   - Bricking bug: 500+ devices affected, revenue at risk → TOP
   - NFC feature: customer demo in 2 weeks → SECOND
   - Documentation: important but flexible → THIRD

2. COMMUNICATE:
   - Told manager: NFC might slip 3 days due to bug priority
   - Told new team member: docs delayed, here's existing resources
   - Documented trade-off in writing

3. EXECUTE:
   - Spent 2 days on bug - found frame counter wrap issue, pushed fix
   - Returned to NFC with 8 days to demo
   - Did documentation in evenings (my choice, not expected)

RESULT:
- Bug fixed, no device bricked after update
- NFC demo successful, on original date
- Documentation ready by team member's second week

LESSON: Explicit prioritization beats implicit juggling. When I said 'these are my
priorities in this order,' everyone aligned."

WHAT-IF FOLLOW-UPS:

Q: "What if your manager disagreed with your prioritization?"
A: "I'd present my reasoning: 'Here's the impact of each option.' If they still
   disagreed, I'd follow their direction - they may have context I lack. But I'd
   document the discussion in case priorities need revisiting later."

Q: "How do you handle interruptions to focus time?"
A: "I block 'deep work' hours on my calendar - usually mornings. For urgent items,
   I have an agreement with team: message me with 'urgent' and I'll respond.
   Everything else waits until my focus block ends."
*/

// =============================================================================
// QUESTION 27: Receiving Feedback
// =============================================================================
/*
INTERVIEWER: "Tell me about a time you received critical feedback."

CANDIDATE RESPONSE:
"During code review at ePlant, my colleague flagged that my bootloader code had
no error handling for flash write failures.

HIS FEEDBACK:
'This assumes flash writes always succeed. What if power fails or flash is worn?
You need to handle these cases or devices will brick.'

MY INITIAL REACTION:
Honestly, defensive. I thought 'flash writes rarely fail.' But I took a breath
and asked: 'Can you walk me through the failure scenarios?'

HIS EXPLANATION:
1. Power failure during write → partial data
2. Flash sector worn out → write fails silently on some parts
3. Bit errors accumulate → data corruption over time

MY RESPONSE:
He was right. I added:
1. CRC verification after every write
2. Shadow copy mechanism (write to B, verify, then swap pointer from A to B)
3. Wear leveling for frequently-updated config sector

OUTCOME:
This directly contributed to our 80% failure rate reduction. His feedback prevented
a class of bugs I hadn't considered.

LESSON: Defensive reactions kill learning. 'Tell me more' is always the right first
response to feedback."

WHAT-IF FOLLOW-UPS:

Q: "What if you disagreed with the feedback?"
A: "I'd still explore it - 'Help me understand your concern.' Sometimes I'm wrong.
   If after discussion I still disagree, I'd explain my reasoning and propose we
   get a third opinion or run a test to resolve it objectively."

Q: "How do you give feedback to others?"
A: "Specific, actionable, and timely. Not 'this code is confusing' but 'this function
   does three things - can we split it?' I focus on the code, not the person, and
   always acknowledge what's done well alongside suggestions."
*/

// =============================================================================
// QUESTION 28: Working with Non-Technical Stakeholders
// =============================================================================
/*
INTERVIEWER: "How do you explain technical concepts to non-technical people?"

CANDIDATE RESPONSE:
"At ePlant, I regularly presented to the CEO and sales team on firmware updates.

MY APPROACH:

1. START WITH OUTCOME:
   Technical: 'We implemented A/B bootloader with CRC verification and rollback'
   Business: 'Firmware updates now have a safety net - if anything goes wrong,
   devices automatically recover. No more bricked sensors.'

2. USE ANALOGIES:
   For OTA updates: 'It's like installing phone updates. Before, if it failed
   mid-install, the phone was toast. Now it's like having a backup - worst case,
   we go back to what worked.'

3. QUANTIFY IN THEIR TERMS:
   Not: '80% reduction in update failures'
   But: 'Instead of 100 truck rolls to fix bricked devices, we'll need 20. At
   $150 per visit, that's $12,000 saved per update cycle.'

4. VISUAL AIDS:
   I drew a simple diagram: old device with X on it (failed), new device with
   two slots and arrows showing automatic recovery.

5. INVITE QUESTIONS:
   'Does this make sense? What concerns do you have?'
   Often their questions reveal what really matters to them."

WHAT-IF FOLLOW-UPS:

Q: "What if they ask a question you don't know the answer to?"
A: "'That's a good question - I'll need to check and get back to you.' Never bluff.
   If it's critical, I'll say 'Let me find out right now' and make a quick call
   to someone who knows."

Q: "How do you handle pushback on technical timelines?"
A: "I explain the constraints without jargon: 'Testing takes 2 weeks because we
   need to simulate 6 months of field operation. Skipping this is how we got
   the bricking bug last time.' Connect timeline to risk they understand."
*/

// =============================================================================
// QUESTION 29: Initiative
// =============================================================================
/*
INTERVIEWER: "Tell me about a time you went beyond your job description."

CANDIDATE RESPONSE:
"At IIT Bombay, my role was implementing firmware for the telematics device.
The data analysis was supposed to be handled by another team.

THE SITUATION:
The analysis team was overwhelmed. We had 3 months of data but no insights.
The project sponsor was asking for results.

MY INITIATIVE:
1. Learned Python data analysis in evenings (pandas, matplotlib)
2. Built Allan Variance analysis pipeline for IMU characterization
3. Created automated reports showing driver behavior patterns
4. Presented initial findings to the insurance company

THE RESULT:
- Identified that 20% noise reduction was achievable with Savitzky-Golay filtering
- Insurance company saw actionable data, committed to pilot program
- My analysis became the template for future data processing

WHAT I LEARNED:
Going beyond your role has risks - I could have stepped on toes. I mitigated this by:
1. Asking permission from my supervisor first
2. Sharing my work openly with the analysis team
3. Offering to hand off once they had bandwidth
They were grateful, not territorial."

WHAT-IF FOLLOW-UPS:

Q: "How do you balance initiative with not overstepping?"
A: "Communication. I ask before starting: 'I see X problem, I could help with Y -
   would that be useful or is there a reason not to?' This respects boundaries
   while showing willingness to help."

Q: "What if your initiative had failed?"
A: "I'd own it. 'I tried X, it didn't work because Y, here's what I learned.'
   Failure is okay if you learn from it and don't hide it."
*/

// =============================================================================
// QUESTION 30: Learning from Mistakes
// =============================================================================
/*
INTERVIEWER: "What's the biggest mistake you've made in your career?"

CANDIDATE RESPONSE:
"At Devlata Technologies, I shipped a crane controller without adequate EMI testing.

THE MISTAKE:
The controller worked perfectly in the lab. I was confident and eager to deploy.
I didn't do extended testing on the factory floor.

THE CONSEQUENCE:
On the factory floor, near the main power switchgear, RF communication became
unreliable. Commands would intermittently fail. For a crane carrying heavy loads,
this was a safety concern. We had to recall the units and retrofit EMI shielding.

ROOT CAUSE ANALYSIS:
1. Overconfidence: 'It works in the lab' isn't 'it works everywhere'
2. Environment blindness: I didn't consider the actual operating conditions
3. Schedule pressure: I cut corners on testing to meet delivery date

WHAT I CHANGED:
1. Created an 'environment checklist': What RF interference exists? Temperature
   extremes? Vibration? Power quality?
2. Mandated on-site testing before sign-off - even if schedule slips
3. Added explicit EMI testing to validation procedure

LONG-TERM IMPACT:
This mistake made me a better engineer. At ePlant, I insisted on field testing
before any production release - even when pushed to ship faster."

WHAT-IF FOLLOW-UPS:

Q: "How did your manager react?"
A: "He was disappointed but professional. We focused on the fix, not blame. After
   it was resolved, we had a retrospective to prevent recurrence. He appreciated
   that I took ownership rather than making excuses."

Q: "What would you do if a teammate was making a similar mistake?"
A: "Speak up privately: 'Hey, I made a similar mistake once - have you considered
   X?' Frame it as sharing experience, not criticism. If they dismiss it and it's
   a safety issue, I'd escalate to our lead."
*/

// =============================================================================
// QUESTION 31-35: SHORT BEHAVIORAL SCENARIOS
// =============================================================================

/*
QUESTION 31: "Describe a time you had to work with incomplete requirements."

ANSWER:
"At Span.IO, the HIL test requirements were vague: 'test the CAN bus functionality.'

MY APPROACH:
1. Asked clarifying questions: What messages? What pass/fail criteria?
2. When answers weren't available, I documented my assumptions
3. Built a minimal test suite based on assumptions
4. Reviewed with stakeholders: 'Here's what I assumed - does this match your intent?'

OUTCOME:
My assumptions were 80% correct. The review caught gaps, we added 10 more test cases.
Better than waiting for perfect requirements that never came."

WHAT-IF: "What if stakeholders couldn't agree on requirements?"
"I'd identify the disagreement, propose a decision meeting, and ask each party
to state their constraints. Often disagreements are about priorities, not
absolutes - we can satisfy most needs with creative solutions."
*/

/*
QUESTION 32: "How do you stay current with technology?"

ANSWER:
"Multi-channel approach:

1. READING:
   - Embedded Artistry blog (weekly)
   - Memfault's 'Interrupt' newsletter
   - Selected papers from ACM/IEEE on specific topics

2. DOING:
   - Personal projects: SunDay wellness tracker was my weekend project
   - Experimenting with new tools: tried Rust for embedded recently

3. COMMUNITY:
   - Reddit r/embedded for discussions
   - Local embedded meetups (pre-pandemic)
   - Conference talks on YouTube (Embedded World, ARM DevSummit)

4. WORK:
   - Push to use new technologies on real projects
   - Zephyr RTOS on SafeScan was deliberately chosen to learn it"

WHAT-IF: "How do you decide what's worth learning vs. what's hype?"
"I ask: Will this solve a problem I have or expect to have? For Rust: memory
safety in embedded is a real problem, and adoption is growing. That's worth
learning. For blockchain-in-everything: I don't have problems it solves."
*/

/*
QUESTION 33: "Tell me about a time you had to convince someone of your idea."

ANSWER:
"At ePlant, I proposed migrating from JSON to Protobuf for sensor data encoding.

THE RESISTANCE:
'JSON is human-readable, easy to debug. Protobuf is overkill.'

MY APPROACH:
1. Quantified the problem: JSON payload was 180 bytes, Protobuf was 45 bytes
2. Showed LoRaWAN cost: 180 bytes = 6 packets vs 2 packets = 3x airtime = 3x power
3. Offered to prototype: 'Let me build it and show you'

THE PROTOTYPE:
- Demonstrated encoding/decoding working
- Showed debugging approach: Protobuf → JSON converter for debugging
- Measured: 15% smaller code size, 4x faster parsing

OUTCOME:
Team adopted Protobuf. The 'human-readable' concern was addressed with debug tools."

WHAT-IF: "What if your prototype had shown Protobuf was worse?"
"I'd have reported that honestly and moved on. The goal was best solution, not
proving I was right. Sometimes ideas fail - that's valuable data too."
*/

/*
QUESTION 34: "How do you handle a situation where you're stuck?"

ANSWER:
"My escalation ladder:

1. RE-READ THE DOCUMENTATION (15 min)
   - Often missed something first time

2. RUBBER DUCK DEBUGGING (15 min)
   - Explain problem out loud to stuffed duck (or notepad)
   - Often realize assumption that's wrong

3. SIMPLIFY (30 min)
   - Strip to minimal reproduction case
   - Remove complexity until it works, then add back

4. SEARCH (30 min)
   - Google exact error message
   - Check GitHub issues for the library
   - Stack Overflow

5. ASK FOR HELP (if above fail)
   - Present what I've tried
   - Specific question, not 'it doesn't work'

REAL EXAMPLE:
NRF905 wasn't transmitting. I spent 2 hours on steps 1-4. Asking a colleague,
he said 'Did you check the CE pin timing?' I hadn't. 10µs delay fixed it.
Sometimes fresh eyes see what you've missed."
*/

/*
QUESTION 35: "Where do you see yourself in 5 years?"

ANSWER:
"I want to be a technical lead on embedded systems projects - not just writing
code, but making architectural decisions, mentoring junior engineers, and
connecting technical work to business outcomes.

SPECIFICALLY:
1. TECHNICAL DEPTH: Deep expertise in RTOS, low-power design, and wireless
2. TECHNICAL BREADTH: Understanding of adjacent areas (cloud, mobile apps)
3. LEADERSHIP: Able to scope projects, decompose into tasks, guide team execution
4. BUSINESS: Understand how technical decisions impact cost, schedule, market

I'm deliberately building toward this: at ePlant, I mentored an intern. On
SafeScan, I made architecture decisions. Each role should expand my scope."

WHAT-IF: "What if this role doesn't lead to technical leadership?"
"I'd want to understand the growth path. Every company is different. I'm open
to roles that expand my impact, even if the title differs. But if there's no
growth path at all, I'd eventually need to reassess fit."
*/

/*******************************************************************************
 * SECTION 4: PROBLEM-SOLVING SCENARIOS (Questions 36-45)
 * Hypothetical situations to test thinking process
 ******************************************************************************/

// =============================================================================
// QUESTION 36: System Design
// =============================================================================
/*
INTERVIEWER: "Design a battery-powered GPS tracker that reports position every hour."

CANDIDATE RESPONSE:
"Let me work through this systematically.

REQUIREMENTS CLARIFICATION:
- Battery life target? (I'll assume 1 year on 2xAA)
- Accuracy needed? (I'll assume 10m consumer GPS)
- Environment? (I'll assume vehicle - temperature, vibration)
- Connectivity? (I'll assume cellular - wide area coverage)

COMPONENT SELECTION:

1. MCU: STM32L4 (ultra-low power, ARM Cortex-M4)
   - 30µA active, 300nA standby
   - Sufficient peripherals (UART for GPS, SPI for modem)

2. GPS: u-blox MAX-M10S
   - 25mW acquisition, 5mW tracking
   - Cold start: 27s, Hot start: 1s
   - Key: store ephemeris in MCU flash → always hot start

3. Cellular: SIMCOM SIM7080G (LTE Cat-M1 + NB-IoT)
   - Low power for IoT
   - PSM (Power Saving Mode) support: 3µA when idle

POWER BUDGET (per hour):
- Sleep: 3µA × 3600s = 10.8mAs
- GPS acquisition: 25mA × 30s = 750mAs (cold start worst case)
- With hot start: 25mA × 5s = 125mAs
- Cellular TX: 200mA × 2s = 400mAs
- Total per cycle (hot start): ~540mAs
- Daily: 540mAs × 24 = 13Ah... wait, that's way too much.

LET ME RECONSIDER:
Need to be smarter about GPS. Options:
1. Accelerometer-based motion detection: only GPS when moving
2. Wake-on-location: cell tower location first, GPS only if area entered
3. Assisted GPS: download ephemeris via cellular, not over air

REVISED DESIGN:
- Accelerometer (ADXL362, 2µA) wakes on motion
- If motion detected, get LTE fix first (~5s, medium accuracy)
- GPS only for geofence triggers or explicit commands
- Report via LTE-M PSM (attach once/day, report multiple fixes)

REVISED POWER:
With motion detection, static vehicle consumes only 5µA average.
This easily meets 1-year requirement."

WHAT-IF FOLLOW-UPS:

Q: "What if GPS can't get a fix (tunnel, underground)?"
A: "Fallback to cell tower triangulation (200-500m accuracy). Store last-known
   good GPS fix. If no cell coverage either, buffer fixes locally and upload
   when connectivity returns. Alert user after N hours without fix."

Q: "How do you handle the device being stolen/disabled?"
A: "Tamper detection (accelerometer detects removal from vehicle). If tampered,
   increase reporting frequency and send alert. For sophisticated attackers
   with Faraday bags, there's no technical solution - need physical security."
*/

// =============================================================================
// QUESTION 37: Debugging Unknown System
// =============================================================================
/*
INTERVIEWER: "You inherit a codebase with an intermittent crash. How do you approach it?"

CANDIDATE RESPONSE:
"Let me walk through my systematic process.

STEP 1: GATHER INFORMATION (Day 1)
- How intermittent? Once per day? Week? Random or correlated to actions?
- What's the crash symptom? Hard fault? Watchdog reset? Hang?
- Any logs, core dumps, or crash reports available?
- When did it start? New code? New environment?

STEP 2: REPRODUCE (Days 2-3)
- Run system under observation (debugger attached if possible)
- If rare, add instrumentation: log buffer for last 100 operations
- Try stress testing: increase load, run edge cases
- Try environmental triggers: temperature, power cycling, memory pressure

STEP 3: ANALYZE CRASH ARTIFACTS (when it happens)
- Hard fault register: tells you fault type (bus error, mem access, etc.)
- Stack trace: where was it when it crashed?
- Memory dump: look for corruption patterns (0xDEADBEEF overwritten?)

STEP 4: FORM HYPOTHESES
Based on evidence, prioritize:
- Stack overflow (check high-water mark)
- Null pointer dereference (scan for unchecked returns)
- Race condition (review shared data access)
- Memory corruption (check buffer sizes, DMA boundaries)

STEP 5: TEST EACH HYPOTHESIS
- Add assertions around suspected area
- Enable memory protection (MPU) to catch invalid access
- Add sanity checks that catch corruption early

STEP 6: FIX AND VALIDATE
- Implement fix
- Run extended stress test (10x normal usage)
- Monitor production for recurrence"

WHAT-IF FOLLOW-UPS:

Q: "What if you can't reproduce it?"
A: "Enhance field telemetry: capture more state before crashes, add remote
   watchdog reporting. Sometimes you need to wait for next field occurrence
   with better instrumentation. It's frustrating but sometimes necessary."

Q: "What if the codebase has no tests?"
A: "That's an opportunity. Write tests for the suspected area before fixing.
   This validates your understanding and prevents regression. Even 10% coverage
   on critical paths is better than 0%."
*/

// =============================================================================
// QUESTION 38: Trade-off Decision
// =============================================================================
/*
INTERVIEWER: "You can improve performance by 50% or improve maintainability. Which
do you choose and why?"

CANDIDATE RESPONSE:
"It depends on context. Let me walk through the decision factors.

CHOOSE PERFORMANCE IF:
1. Current performance is inadequate (missing real-time deadlines)
2. Product is in production at scale (optimization pays dividends)
3. Performance is customer-facing differentiator
4. Team is experienced (can handle complexity)

CHOOSE MAINTAINABILITY IF:
1. Current performance is acceptable (headroom exists)
2. Code will be heavily modified in near future
3. Team is growing or changing (onboarding cost matters)
4. Bug rate is high (complexity is causing defects)

MY REAL-WORLD EXAMPLE:
At ePlant, I had this exact choice for the Protobuf parser.
- Option A: Hand-optimized assembly, 2x faster parsing
- Option B: Clean C with generator-based code, 1x speed

I chose B because:
1. Parsing was 2% of overall CPU time - 2x improvement was 1% total
2. Schema was changing quarterly - regenerating was easier than updating assembly
3. Team included junior engineers who needed to understand the code

If parsing had been 80% of CPU time, I'd have chosen A."

WHAT-IF FOLLOW-UPS:

Q: "What if your stakeholder insists on the option you disagree with?"
A: "I'd present my analysis clearly: 'Here are the trade-offs. I recommend X
   because Y. If we go with Z, here are the risks we're accepting.' Then
   respect their decision - they may have context I don't."

Q: "How do you measure maintainability?"
A: "Time-to-onboard new developer. Time to implement a typical feature. Bug rate
   per line of code. Code review feedback patterns. These are proxies, but
   more objective than 'it feels clean.'"
*/

// =============================================================================
// QUESTION 39: Resource Constraint
// =============================================================================
/*
INTERVIEWER: "You have 2 weeks to ship a feature that typically takes 4 weeks. What do you do?"

CANDIDATE RESPONSE:
"First, I need to understand whether this is negotiable or hard constraint.

IF NEGOTIABLE (most common):
1. Clarify: What's driving the 2-week deadline? Demo? Trade show? Competitor?
2. Propose: Can we ship subset in 2 weeks, full feature in 4 weeks?
3. Trade: What can we de-scope? Cut nice-to-haves, keep must-haves

IF HARD CONSTRAINT:
1. DEFINE MVP: What's the absolute minimum that delivers value?
   - Cut edge case handling (hardcode defaults)
   - Skip UI polish (functional over pretty)
   - Manual config instead of auto-discovery

2. PARALLELIZE: Can I bring in help?
   - Split feature into independent parts
   - I do core logic, colleague does integration
   - Clear interface contracts up front

3. TIMEBOX RISKS: What could blow up the schedule?
   - Identify biggest unknowns (new hardware, external API)
   - Tackle those first - fail fast

4. COMMUNICATE: Daily updates on progress vs plan
   - No surprises - if slipping, stakeholder knows early

REAL EXAMPLE (NFC Provisioning):
Had 2 weeks, estimated 3. Cut:
- Device autoconfiguration → manual CSV upload
- Animated provisioning progress → simple success/fail LED
- Multi-device queue → one device at a time

Delivered core functionality on time. Shipped polish features in next sprint."

WHAT-IF FOLLOW-UPS:

Q: "What if the whole 4 weeks of work is truly essential?"
A: "Then the deadline is impossible. I'd escalate: 'Here's what 2 weeks gets you.
   Here's what 4 weeks gets you. What do you want?' Better to set expectations
   now than miss deadline later or ship broken code."

Q: "How do you handle the stress of impossible deadlines?"
A: "Focus on what I control: my output. Communicate clearly, work efficiently,
   ask for help. Burnout comes from ambiguity and isolation. With clear scope
   and teamwork, 'hard' is manageable."
*/

// =============================================================================
// QUESTION 40: Ethical Dilemma
// =============================================================================
/*
INTERVIEWER: "You discover a security vulnerability in your product that management
wants to ship anyway. What do you do?"

CANDIDATE RESPONSE:
"This is a serious situation. My approach:

STEP 1: ENSURE I UNDERSTAND THE VULNERABILITY
- What's the actual risk? Can it be exploited remotely? Does it require physical
  access? What data is at risk?
- Am I certain it's a real vulnerability, not a theoretical concern?

STEP 2: DOCUMENT AND ESCALATE
- Write up the vulnerability, impact, and proposed fix
- Present to immediate manager with clear recommendation
- If dismissed, ask: 'Can you help me understand why this is acceptable?'

STEP 3: ESCALATE HIGHER IF NEEDED
- If manager dismisses, go to their manager or security team
- Many companies have anonymous reporting channels
- Document everything - dates, meetings, decisions

STEP 4: CONSIDER LEGAL AND ETHICAL OBLIGATIONS
- Some industries have mandatory reporting (medical devices, financial)
- Some vulnerabilities have legal implications (GDPR, HIPAA)
- Consult legal/compliance if unsure

STEP 5: PERSONAL DECISION
If the company truly refuses to address a critical vulnerability:
- I'd document my objection in writing
- Consider whether I can ethically continue working there
- In extreme cases (life safety), external reporting may be warranted

REAL CONTEXT:
At ePlant, I found a vulnerability where OTA updates weren't cryptographically
signed. I raised it, we delayed launch by 2 weeks to add signing. Management
was initially resistant (timeline), but accepted when I quantified the risk
of malicious firmware being pushed to customer devices."

WHAT-IF FOLLOW-UPS:

Q: "What if fixing it really would delay launch by 3 months?"
A: "Then we have a real trade-off. Can we ship with a warning? Disable the
   vulnerable feature? Ship to limited audience while fixing? The answer isn't
   always 'don't ship' but it's also not always 'ship anyway.'"

Q: "What if you're overruled and told to ship?"
A: "I'd document my objection in email to create a record. If it's truly a safety
   issue, I'd escalate to the highest level I can reach. If still overruled, I'd
   need to decide if I can stay. Some things aren't worth my integrity."
*/

// =============================================================================
// QUESTION 41-45: RAPID FIRE PROBLEM SOLVING
// =============================================================================

/*
QUESTION 41: "A customer reports your device works in New York but fails in Singapore.
What's your hypothesis?"

ANSWER:
"Top hypotheses in order:
1. RF regulatory: Different ISM bands/power limits (915MHz US vs 920-923MHz Asia)
2. LoRaWAN network: Different NS configuration, ADR settings
3. Environmental: Higher humidity affecting RF propagation or condensation
4. Power supply: Different wall voltage (120V vs 230V) if using adapters

FIRST STEP:
Ask for device logs from Singapore. Check:
- RF link margin (SNR from network server)
- Error codes, if any
- Power cycling frequency

Without data, I'd check RF regulatory compliance first - most likely culprit."
*/

/*
QUESTION 42: "Your code passes all tests but crashes in production. Why?"

ANSWER:
"Test environment differs from production in:
1. TIMING: Tests may run slower (debugger) or faster (optimized builds)
2. ENVIRONMENT: Temperature, voltage, RF interference
3. DATA: Production data is messier, longer, more varied
4. SCALE: 1000x more events expose rare race conditions
5. DURATION: Tests run minutes, production runs months (memory leaks)

DIAGNOSIS:
1. Add production telemetry (last operations before crash)
2. Compare build options (optimization, asserts enabled?)
3. Run extended 'soak tests' - days of operation under load
4. Review assumptions: 'this never happens' often happens in production"
*/

/*
QUESTION 43: "You notice your team's code quality declining. What do you do?"

ANSWER:
"DIAGNOSE FIRST:
1. Talk to team members: Are they overloaded? Unclear on standards?
   Facing pressure to ship fast?
2. Look at data: Bug rate trending? Code review comments increasing?
3. Identify patterns: Certain areas worse? Certain times (end of sprint)?

THEN ADDRESS:
1. If workload: Advocate for realistic schedules, help prioritize
2. If unclear standards: Establish coding guidelines, automate linting
3. If skill gap: Pair programming, targeted training
4. If pressure from above: Quantify tech debt cost, present to management

MY APPROACH AT SPAN.IO:
Noticed test coverage dropping. Team was skipping tests to hit deadlines.
Proposed: mandatory 80% coverage for new code, enforced in CI. Initially
slowed us down 10%, but bugs caught in CI vs production saved time overall."
*/

/*
QUESTION 44: "You have to choose between React and Flutter for a mobile app. Neither
team has experience with either. How do you decide?"

ANSWER:
"DECISION CRITERIA:
1. Target platforms: Both cross-platform. Flutter has better iOS/Android parity.
2. Performance needs: Flutter's compiled code is faster for heavy UIs.
3. Existing codebase: If we have JavaScript/TypeScript skills, React Native.
   If Dart or greenfield, Flutter is fine.
4. Ecosystem: What libraries do we need? Check npm vs pub.dev.
5. Long-term: Flutter is newer but growing. React Native is mature.

PROCESS:
1. Timebox 2 days: Build 'hello world' in both
2. Involve team: Which felt more natural?
3. Prototype key feature: Our hardest screen in both
4. Decide based on concrete experience, not theory

DEFAULT IF TRULY TIE:
Choose the one with larger hiring pool in our market - we'll need to scale team."
*/

/*
QUESTION 45: "If you could automate one thing about your workflow, what would it be?"

ANSWER:
"Documentation from code changes.

THE PAIN:
Every time I change an API or behavior, I have to manually update:
- Header file comments
- README examples
- Confluence/wiki pages
- Changelog

THE AUTOMATION:
1. Write well-structured comments in code (already doing this)
2. Tool extracts and generates Markdown docs (Doxygen, Sphinx)
3. CI validates doc-code consistency (fail build if out of sync)
4. Changelog auto-generates from commit messages (conventional commits)

REAL IMPACT:
At ePlant, I estimated 10% of my time was documentation sync. Automating
this saves an hour a day, reduces errors, and keeps docs always current."
*/

/*******************************************************************************
 * SECTION 5: CLOSING QUESTIONS (Questions 46-50)
 * Your questions for them - don't skip this!
 ******************************************************************************/

// =============================================================================
// QUESTION 46: Questions for the Interviewer
// =============================================================================
/*
INTERVIEWER: "Do you have any questions for me?"

SMART QUESTIONS TO ASK:

1. ABOUT THE TEAM:
   "What's the team structure? How many firmware engineers, and how do you
   collaborate with hardware and software teams?"

   WHY: Reveals collaboration style, team size, your potential role

2. ABOUT THE WORK:
   "What would my first 90-day project look like? What would success look like?"

   WHY: Sets expectations, reveals priorities, shows you're thinking ahead

3. ABOUT CHALLENGES:
   "What's the hardest technical problem the team is currently facing?"

   WHY: Shows you want to contribute, reveals real work vs. job posting fluff

4. ABOUT GROWTH:
   "How does the team approach professional development? Any examples of engineers
   who've grown here?"

   WHY: Reveals investment in people, career path potential

5. ABOUT PROCESS:
   "What's your typical development cycle? How do you balance shipping fast
   vs. quality?"

   WHY: Reveals culture - are they move-fast-break-things or methodical?

QUESTIONS TO AVOID:
- Salary/benefits in first round (wait for HR)
- 'What does the company do?' (you should already know)
- 'Will I have to work weekends?' (sounds lazy)

HOW MANY: Prepare 5, ask 2-3 based on time and conversation flow.
*/

// =============================================================================
// QUESTION 47: Salary Negotiation
// =============================================================================
/*
INTERVIEWER: "What are your salary expectations?"

APPROACH 1 - DEFER (preferred in first round):
"I'm flexible depending on the total package and opportunity. Can you share
the range budgeted for this role?"

APPROACH 2 - RANGE (if pressed):
"Based on my research for embedded firmware roles in [city] with [X] years
of experience, I'm looking in the [Y-Z] range. But I'm open to discussion
based on the complete opportunity."

APPROACH 3 - VALUE (if you have leverage):
"In my current role, I'm at [X]. Given the scope of this role and the impact
I can bring, I'm targeting [X+20%] or higher."

PRINCIPLES:
1. Never give a single number - always a range
2. Research market rates: Glassdoor, levels.fyi, LinkedIn salary
3. Total comp matters: base, bonus, equity, benefits, learning budget
4. Don't negotiate in first round - get the offer first

IF THEY PUSH:
"I'd prefer to learn more about the role and team before discussing numbers.
Can we revisit this after the technical rounds?"

This is a business negotiation, not a confrontation. Both sides want a deal."
*/

// =============================================================================
// QUESTION 48: Why This Company?
// =============================================================================
/*
INTERVIEWER: "Why do you want to work here?"

GOOD ANSWER STRUCTURE:

1. MISSION/PRODUCT:
   "I'm excited about [specific product]. [Personal connection to problem space].
   The impact of [what the product does] resonates with me."

2. TEAM/CULTURE:
   "From my research and this conversation, the team seems [specific observation:
   technically strong, collaborative, etc.]. That's the environment I thrive in."

3. TECHNICAL CHALLENGE:
   "The work on [specific technology they use] is exactly what I want to deepen.
   I've done [related work], and this role would let me take it further."

4. GROWTH:
   "I see potential to grow toward [technical lead, architecture, etc.] here,
   which aligns with my career goals."

EXAMPLE FOR AN IoT COMPANY:
"I've spent 3 years building connected devices, but mostly in controlled
environments. Your products operate in harsh conditions - oil fields, mines.
That's a whole new level of reliability challenge. I want to learn how you
solve those problems. Plus, the mission of improving worker safety through
technology is meaningful to me - it's not just another app."

AVOID:
- "I need a job" (desperation)
- "You have good benefits" (transactional)
- "I want to learn" without giving back (one-sided)"
*/

// =============================================================================
// QUESTION 49: Strengths and Weaknesses
// =============================================================================
/*
INTERVIEWER: "What are your greatest strengths and weaknesses?"

STRENGTH - BE SPECIFIC:
"My greatest strength is debugging complex system issues. I have a systematic
process - isolate, instrument, hypothesize, test - that's helped me solve
problems others gave up on. The EMI issue in the crane controller, the frame
counter wrap at ePlant - both were 'impossible' bugs that I methodically traced
to root cause.

This comes from curiosity and stubbornness. When something fails, I need to
know WHY, not just what workaround makes it go away."

WEAKNESS - BE HONEST, SHOW GROWTH:
"I sometimes get absorbed in technical details and lose sight of schedule. I
can spend a day optimizing something that doesn't matter to the product.

I've learned to set time limits. Before diving into optimization, I ask: 'Is
this on the critical path? What's the business impact?' If the answer is
unclear, I check with my lead before spending time. I've also started using
task trackers to make sure I'm addressing priorities, not just interesting
problems."

BAD WEAKNESS ANSWERS (avoid):
- "I work too hard" (not a real weakness)
- "I'm a perfectionist" (cliché)
- "I'm bad with people" (red flag for team role)

Good weaknesses are real but not disqualifying, and you're actively managing them."
*/

// =============================================================================
// QUESTION 50: Final Closing
// =============================================================================
/*
INTERVIEWER: "Is there anything else you'd like to add before we wrap up?"

STRONG CLOSING:
"Thank you for your time today. I want to reinforce that I'm genuinely excited
about this opportunity.

The problems you're solving - [specific challenge they mentioned] - align
directly with my experience in [relevant work]. I've done [specific relevant
thing] and I'm confident I can contribute quickly.

I'm looking for a role where I can have technical impact while growing toward
leadership. From our conversation, this seems like a great fit.

What are the next steps in the process?"

THIS CLOSING:
1. Shows enthusiasm (not desperate, not aloof)
2. Reinforces fit with specific examples
3. Expresses career alignment
4. Asks for next steps (shows you're serious)

AFTER THE INTERVIEW:
- Send thank-you email within 24 hours
- Reference something specific from conversation
- Keep it short (3-4 sentences)

Example:
"Thank you for discussing the role today. The challenge of [X] you described
is exactly the kind of problem I enjoy solving. I'm excited about the
possibility of joining the team. Please let me know if you need any additional
information from me."
*/

/*******************************************************************************
 * SUMMARY: INTERVIEW SUCCESS CHECKLIST
 *
 * BEFORE:
 * □ Research company, product, recent news
 * □ Review your resume - know every line cold
 * □ Prepare 3 project stories at 30s/2min/5min depth
 * □ Prepare 3-5 questions to ask
 * □ Test your audio/video if remote
 *
 * DURING:
 * □ Listen carefully - answer the ASKED question
 * □ Use STAR for behavioral questions
 * □ Think aloud for technical questions
 * □ Say "I don't know, but here's how I'd find out" when stuck
 * □ Show enthusiasm without being excessive
 *
 * TECHNICAL QUESTIONS:
 * □ Clarify requirements before diving in
 * □ State assumptions explicitly
 * □ Walk through trade-offs
 * □ Start with high-level, go deep when asked
 *
 * AFTER:
 * □ Send thank-you email within 24 hours
 * □ Note what went well and what to improve
 * □ Follow up if no response within stated timeline
 *
 * GOOD LUCK! You've got this.
 ******************************************************************************/
