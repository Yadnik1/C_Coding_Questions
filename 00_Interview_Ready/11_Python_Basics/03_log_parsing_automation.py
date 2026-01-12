"""
============================================================================
            LOG PARSING & AUTOMATION FOR EMBEDDED ENGINEERS
============================================================================

Real embedded work involves LOTS of log parsing:
- Finding errors in device logs
- Extracting timing data
- Analyzing test results
- Monitoring production devices

============================================================================
                    PATTERN 1: BASIC LOG PARSING
============================================================================
"""

import re
from datetime import datetime
from collections import defaultdict

# Sample embedded device log
SAMPLE_LOG = """
2024-01-15 08:00:01.123 [INFO] System boot, firmware v1.2.3
2024-01-15 08:00:01.456 [INFO] WiFi connecting to "Office_Network"
2024-01-15 08:00:03.789 [INFO] WiFi connected, IP: 192.168.1.100
2024-01-15 08:00:04.012 [INFO] MQTT connecting to broker.example.com:8883
2024-01-15 08:00:05.234 [ERROR] MQTT connection failed: timeout
2024-01-15 08:00:06.456 [INFO] MQTT retry attempt 1
2024-01-15 08:00:08.678 [INFO] MQTT connected
2024-01-15 08:00:10.000 [INFO] Sensor init: temp=25.3C, humidity=45%
2024-01-15 08:01:00.000 [INFO] Sensor reading: temp=25.5C, humidity=46%
2024-01-15 08:02:00.000 [WARN] Low battery: 15%
2024-01-15 08:03:00.000 [ERROR] Sensor timeout, retrying
2024-01-15 08:03:01.000 [INFO] Sensor recovered
2024-01-15 08:04:00.000 [INFO] Sensor reading: temp=25.8C, humidity=44%
2024-01-15 08:05:00.000 [ERROR] WiFi disconnected
2024-01-15 08:05:05.000 [INFO] WiFi reconnecting
2024-01-15 08:05:10.000 [INFO] WiFi connected, IP: 192.168.1.100
"""

def parse_log_line(line):
    """
    Parse log line into components.
    Format: YYYY-MM-DD HH:MM:SS.mmm [LEVEL] message
    """
    pattern = r'(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3}) \[(\w+)\] (.+)'
    match = re.match(pattern, line)

    if match:
        timestamp_str, level, message = match.groups()
        timestamp = datetime.strptime(timestamp_str, '%Y-%m-%d %H:%M:%S.%f')
        return {
            'timestamp': timestamp,
            'level': level,
            'message': message
        }
    return None


def count_by_level(log_text):
    """Count log entries by level"""
    counts = defaultdict(int)
    for line in log_text.strip().split('\n'):
        parsed = parse_log_line(line)
        if parsed:
            counts[parsed['level']] += 1
    return dict(counts)


def find_errors(log_text):
    """Extract all error messages"""
    errors = []
    for line in log_text.strip().split('\n'):
        parsed = parse_log_line(line)
        if parsed and parsed['level'] == 'ERROR':
            errors.append(parsed)
    return errors


def extract_temperatures(log_text):
    """Extract temperature readings from log"""
    temps = []
    pattern = r'temp=(\d+\.?\d*)C'

    for line in log_text.strip().split('\n'):
        match = re.search(pattern, line)
        if match:
            parsed = parse_log_line(line)
            if parsed:
                temps.append({
                    'timestamp': parsed['timestamp'],
                    'temp': float(match.group(1))
                })
    return temps


print("=== Log Parsing Examples ===")
print(f"\nCounts by level: {count_by_level(SAMPLE_LOG)}")

print("\nErrors found:")
for err in find_errors(SAMPLE_LOG):
    print(f"  {err['timestamp']}: {err['message']}")

print("\nTemperatures:")
for t in extract_temperatures(SAMPLE_LOG):
    print(f"  {t['timestamp'].strftime('%H:%M:%S')}: {t['temp']}°C")


"""
============================================================================
                PATTERN 2: REGEX PATTERNS FOR EMBEDDED LOGS
============================================================================
"""

# Common patterns for embedded logs
PATTERNS = {
    # MAC address
    'mac': r'([0-9A-Fa-f]{2}[:-]){5}[0-9A-Fa-f]{2}',

    # IP address
    'ip': r'\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}',

    # Hex value
    'hex': r'0x[0-9A-Fa-f]+',

    # Firmware version (semver)
    'version': r'v?\d+\.\d+\.\d+',

    # Memory address
    'memaddr': r'0x[0-9A-Fa-f]{8}',

    # Percentage
    'percent': r'\d+%',

    # Timestamp (various formats)
    'timestamp_iso': r'\d{4}-\d{2}-\d{2}[T ]\d{2}:\d{2}:\d{2}',
    'timestamp_unix': r'\d{10}',

    # Float with unit
    'temp_c': r'-?\d+\.?\d*\s*[°]?C',
    'voltage': r'\d+\.?\d*\s*[mV]?V',
    'current': r'\d+\.?\d*\s*[muμ]?A',
}

def find_all_matches(text, pattern_name):
    """Find all matches for a named pattern"""
    if pattern_name not in PATTERNS:
        return []
    return re.findall(PATTERNS[pattern_name], text)


test_log = "Device MAC: AA:BB:CC:DD:EE:FF connected to 192.168.1.1, version v2.0.1, temp 25.5C"
print("\n=== Regex Pattern Matching ===")
print(f"MACs: {find_all_matches(test_log, 'mac')}")
print(f"IPs: {find_all_matches(test_log, 'ip')}")
print(f"Versions: {find_all_matches(test_log, 'version')}")


"""
============================================================================
            PATTERN 3: BOOT TIME ANALYSIS
============================================================================
"""

BOOT_LOG = """
[0.000] Boot start
[0.010] CPU init
[0.050] Memory test
[0.200] Flash init
[0.250] GPIO config
[0.300] SPI init
[0.400] WiFi module init
[1.500] WiFi connecting
[3.200] WiFi connected
[3.300] MQTT init
[3.800] MQTT connected
[3.850] App start
[4.000] Boot complete
"""

def analyze_boot_time(log_text):
    """
    Analyze boot time from log.
    Returns: List of (component, start_time, duration)
    """
    pattern = r'\[(\d+\.?\d*)\] (.+)'
    events = []

    for line in log_text.strip().split('\n'):
        match = re.match(pattern, line)
        if match:
            time_sec = float(match.group(1))
            event = match.group(2)
            events.append((time_sec, event))

    # Calculate durations
    results = []
    for i, (time, event) in enumerate(events):
        if i < len(events) - 1:
            duration = events[i+1][0] - time
            results.append({
                'event': event,
                'start': time,
                'duration': duration
            })

    return results


print("\n=== Boot Time Analysis ===")
boot_data = analyze_boot_time(BOOT_LOG)
total = 0
for item in boot_data:
    bar = '#' * int(item['duration'] * 20)  # Scale for display
    print(f"{item['event']:20} {item['start']:6.3f}s  +{item['duration']:.3f}s  {bar}")
    total += item['duration']

print(f"\nTotal boot time: {total:.3f}s")

# Find slowest component
slowest = max(boot_data, key=lambda x: x['duration'])
print(f"Slowest: {slowest['event']} ({slowest['duration']:.3f}s)")


"""
============================================================================
                PATTERN 4: CRASH DUMP ANALYSIS
============================================================================
"""

CRASH_DUMP = """
*** CRASH DUMP ***
Exception: HardFault
PC: 0x08012345
LR: 0x08012340
SP: 0x20001234
R0: 0x00000000
R1: 0x00000001
R2: 0xDEADBEEF
R3: 0x00000003

Stack trace:
  0x08012345 sensor_read+0x15
  0x08011234 main_loop+0x44
  0x08010100 main+0x20

Fault address: 0x00000000 (NULL pointer dereference)
"""

def parse_crash_dump(dump_text):
    """Parse crash dump and extract useful info"""
    result = {
        'exception': None,
        'registers': {},
        'stack_trace': [],
        'fault_addr': None,
        'fault_type': None
    }

    # Extract exception type
    match = re.search(r'Exception: (\w+)', dump_text)
    if match:
        result['exception'] = match.group(1)

    # Extract registers
    reg_pattern = r'(PC|LR|SP|R\d+): (0x[0-9A-Fa-f]+)'
    for match in re.finditer(reg_pattern, dump_text):
        result['registers'][match.group(1)] = match.group(2)

    # Extract stack trace
    stack_pattern = r'(0x[0-9A-Fa-f]+) (\w+)\+0x([0-9A-Fa-f]+)'
    for match in re.finditer(stack_pattern, dump_text):
        result['stack_trace'].append({
            'address': match.group(1),
            'function': match.group(2),
            'offset': match.group(3)
        })

    # Extract fault info
    fault_match = re.search(r'Fault address: (0x[0-9A-Fa-f]+) \((.+)\)', dump_text)
    if fault_match:
        result['fault_addr'] = fault_match.group(1)
        result['fault_type'] = fault_match.group(2)

    return result


print("\n=== Crash Dump Analysis ===")
crash = parse_crash_dump(CRASH_DUMP)
print(f"Exception: {crash['exception']}")
print(f"Fault: {crash['fault_type']} at {crash['fault_addr']}")
print(f"PC: {crash['registers'].get('PC')}")
print("Stack trace:")
for frame in crash['stack_trace']:
    print(f"  {frame['address']} {frame['function']}+{frame['offset']}")


"""
============================================================================
                PATTERN 5: TEST RESULT AGGREGATION
============================================================================
"""

TEST_RESULTS = """
[TEST] test_gpio_init: PASS (0.001s)
[TEST] test_gpio_write: PASS (0.002s)
[TEST] test_gpio_read: PASS (0.001s)
[TEST] test_spi_init: PASS (0.050s)
[TEST] test_spi_transfer: FAIL (0.100s) - Expected 0xAA, got 0x00
[TEST] test_uart_init: PASS (0.010s)
[TEST] test_uart_tx: PASS (0.020s)
[TEST] test_uart_rx: SKIP - No loopback connected
[TEST] test_wifi_connect: PASS (2.500s)
[TEST] test_wifi_disconnect: PASS (0.100s)
[TEST] test_mqtt_connect: FAIL (5.000s) - Connection timeout
"""

def parse_test_results(results_text):
    """Parse test results and generate summary"""
    pattern = r'\[TEST\] (\w+): (PASS|FAIL|SKIP)(?: \((\d+\.?\d*)s\))?(?: - (.+))?'

    tests = []
    for line in results_text.strip().split('\n'):
        match = re.match(pattern, line)
        if match:
            tests.append({
                'name': match.group(1),
                'status': match.group(2),
                'duration': float(match.group(3)) if match.group(3) else 0,
                'message': match.group(4)
            })

    # Summary
    summary = {
        'total': len(tests),
        'passed': sum(1 for t in tests if t['status'] == 'PASS'),
        'failed': sum(1 for t in tests if t['status'] == 'FAIL'),
        'skipped': sum(1 for t in tests if t['status'] == 'SKIP'),
        'total_time': sum(t['duration'] for t in tests),
        'failures': [t for t in tests if t['status'] == 'FAIL']
    }

    return tests, summary


print("\n=== Test Result Summary ===")
tests, summary = parse_test_results(TEST_RESULTS)
print(f"Total: {summary['total']}, Passed: {summary['passed']}, "
      f"Failed: {summary['failed']}, Skipped: {summary['skipped']}")
print(f"Total time: {summary['total_time']:.3f}s")

if summary['failures']:
    print("\nFailures:")
    for f in summary['failures']:
        print(f"  {f['name']}: {f['message']}")


"""
============================================================================
                PATTERN 6: PRODUCTION LOG MONITORING
============================================================================
"""

import json
from collections import Counter

# Simulated production telemetry (JSON format common for IoT)
TELEMETRY_DATA = [
    {"device_id": "DEV001", "event": "boot", "uptime": 0, "battery": 100},
    {"device_id": "DEV001", "event": "reading", "temp": 25.5, "battery": 99},
    {"device_id": "DEV002", "event": "boot", "uptime": 0, "battery": 100},
    {"device_id": "DEV001", "event": "reading", "temp": 25.8, "battery": 98},
    {"device_id": "DEV002", "event": "error", "code": "E001", "msg": "sensor_timeout"},
    {"device_id": "DEV001", "event": "reading", "temp": 26.0, "battery": 97},
    {"device_id": "DEV003", "event": "boot", "uptime": 0, "battery": 45},
    {"device_id": "DEV003", "event": "warning", "code": "W001", "msg": "low_battery"},
    {"device_id": "DEV002", "event": "reading", "temp": 24.0, "battery": 95},
]

def analyze_telemetry(data):
    """Analyze production telemetry data"""
    devices = defaultdict(lambda: {'events': [], 'errors': [], 'warnings': []})

    for entry in data:
        dev_id = entry['device_id']
        devices[dev_id]['events'].append(entry)

        if entry['event'] == 'error':
            devices[dev_id]['errors'].append(entry)
        elif entry['event'] == 'warning':
            devices[dev_id]['warnings'].append(entry)

    # Summary stats
    error_codes = Counter()
    for d in devices.values():
        for err in d['errors']:
            error_codes[err.get('code', 'unknown')] += 1

    return dict(devices), dict(error_codes)


print("\n=== Telemetry Analysis ===")
devices, error_counts = analyze_telemetry(TELEMETRY_DATA)
print(f"Active devices: {len(devices)}")
print(f"Error counts: {error_counts}")

for dev_id, info in devices.items():
    status = "OK"
    if info['errors']:
        status = "ERRORS"
    elif info['warnings']:
        status = "WARNINGS"
    print(f"  {dev_id}: {len(info['events'])} events, status: {status}")


"""
============================================================================
            PATTERN 7: GENERATING REPORTS
============================================================================
"""

def generate_html_report(tests, summary, output_file='report.html'):
    """Generate HTML test report"""
    html = f"""<!DOCTYPE html>
<html>
<head>
    <title>Test Report</title>
    <style>
        body {{ font-family: Arial, sans-serif; margin: 20px; }}
        .pass {{ color: green; }}
        .fail {{ color: red; }}
        .skip {{ color: orange; }}
        table {{ border-collapse: collapse; width: 100%; }}
        th, td {{ border: 1px solid #ddd; padding: 8px; text-align: left; }}
        th {{ background-color: #4CAF50; color: white; }}
    </style>
</head>
<body>
    <h1>Test Report</h1>
    <h2>Summary</h2>
    <p>Total: {summary['total']} |
       <span class="pass">Passed: {summary['passed']}</span> |
       <span class="fail">Failed: {summary['failed']}</span> |
       <span class="skip">Skipped: {summary['skipped']}</span></p>
    <p>Total time: {summary['total_time']:.3f}s</p>

    <h2>Details</h2>
    <table>
        <tr><th>Test</th><th>Status</th><th>Duration</th><th>Message</th></tr>
"""

    for test in tests:
        status_class = test['status'].lower()
        msg = test['message'] or ''
        html += f"""        <tr>
            <td>{test['name']}</td>
            <td class="{status_class}">{test['status']}</td>
            <td>{test['duration']:.3f}s</td>
            <td>{msg}</td>
        </tr>
"""

    html += """    </table>
</body>
</html>"""

    # In real code, write to file
    # with open(output_file, 'w') as f:
    #     f.write(html)

    print(f"\n=== HTML Report Generated ===")
    print(f"Would write to: {output_file}")
    print(f"Report size: {len(html)} bytes")


generate_html_report(tests, summary)


"""
============================================================================
                    CODING EXERCISES
============================================================================
"""

print("\n" + "="*60)
print("PRACTICE EXERCISES")
print("="*60)

# Exercise 1: Find average sensor reading
def average_sensor_reading(log_text, sensor_pattern):
    """Find average value for readings matching pattern"""
    values = []
    for match in re.finditer(sensor_pattern, log_text):
        values.append(float(match.group(1)))
    return sum(values) / len(values) if values else None

temp_avg = average_sensor_reading(SAMPLE_LOG, r'temp=(\d+\.?\d*)C')
print(f"Exercise 1 - Average temp: {temp_avg}°C")


# Exercise 2: Find time between events
def time_between_events(log_text, event1, event2):
    """Find time delta between two log events"""
    e1_time = e2_time = None

    for line in log_text.strip().split('\n'):
        parsed = parse_log_line(line)
        if parsed:
            if event1 in parsed['message'] and e1_time is None:
                e1_time = parsed['timestamp']
            if event2 in parsed['message'] and e1_time is not None:
                e2_time = parsed['timestamp']
                break

    if e1_time and e2_time:
        return (e2_time - e1_time).total_seconds()
    return None

boot_to_mqtt = time_between_events(SAMPLE_LOG, "System boot", "MQTT connected")
print(f"Exercise 2 - Boot to MQTT: {boot_to_mqtt}s")


# Exercise 3: Parse key-value log format
def parse_kv_log(line):
    """
    Parse key=value format: "event=reading temp=25.5 battery=99"
    Return: dict
    """
    result = {}
    pattern = r'(\w+)=([^\s]+)'
    for match in re.finditer(pattern, line):
        key = match.group(1)
        value = match.group(2)
        # Try to convert to number
        try:
            if '.' in value:
                value = float(value)
            else:
                value = int(value)
        except ValueError:
            pass
        result[key] = value
    return result

kv_line = "event=reading temp=25.5 battery=99 status=ok"
print(f"Exercise 3 - KV parse: {parse_kv_log(kv_line)}")


"""
============================================================================
                        INTERVIEW Q&A
============================================================================

Q1: "Parse this 10GB log file efficiently"
A1:
   - Read line by line (don't load entire file into memory)
   - Use generators for streaming
   - Process in chunks if parallel processing needed
   - Example:
     with open('huge.log', 'r') as f:
         for line in f:  # Reads one line at a time
             process(line)

Q2: "How would you find memory leaks from device logs?"
A2:
   - Extract heap usage over time from logs
   - Plot free memory vs time
   - Look for steady decrease without recovery
   - Track allocation patterns around events
   - Check for warnings like "allocation failed"

Q3: "Automate daily log analysis for 1000 devices"
A3:
   1. Collect logs via MQTT/HTTP to central server
   2. Parse each device's logs for errors/warnings
   3. Aggregate stats (error counts, boot times, battery)
   4. Generate daily report email
   5. Alert on anomalies (sudden increase in errors)
   6. Use cron job to run daily

============================================================================
"""

if __name__ == '__main__':
    print("\nLog parsing practice complete!")
