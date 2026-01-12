"""
============================================================================
        PYTHON BASICS FOR EMBEDDED/FIRMWARE ENGINEERS (4-HOUR CRASH COURSE)
============================================================================

WHY PYTHON FOR EMBEDDED ROLES?
- Test scripts and automation
- Serial communication with devices
- Log parsing and data analysis
- Factory provisioning tools
- CI/CD scripts
- Quick prototyping

THIS IS NOT: Python for web dev or data science
THIS IS: Python you'll use alongside C firmware work

============================================================================
                        SECTION 1: SYNTAX BASICS (30 min)
============================================================================
"""

# Variables - no type declaration needed
device_id = 0x1234
firmware_version = "1.2.3"
is_connected = True
temperature = 25.5

# Print formatting (you'll use this constantly)
print(f"Device {device_id:#06x} running v{firmware_version}")
print(f"Temperature: {temperature:.2f}°C")

# Lists (like arrays, but dynamic)
sensor_readings = [23.5, 24.1, 23.8, 24.0]
sensor_readings.append(24.2)  # Add to end
print(f"Readings: {sensor_readings}")
print(f"Average: {sum(sensor_readings)/len(sensor_readings):.2f}")

# Dictionaries (key-value pairs - used everywhere)
device_config = {
    "baud_rate": 115200,
    "parity": "none",
    "timeout": 5.0,
    "port": "/dev/ttyUSB0"
}
print(f"Baud: {device_config['baud_rate']}")

# Loops
for reading in sensor_readings:
    print(f"  Reading: {reading}")

for i, reading in enumerate(sensor_readings):
    print(f"  [{i}] = {reading}")

# Range (like for loop in C)
for i in range(5):       # 0, 1, 2, 3, 4
    print(i, end=" ")
print()

for i in range(2, 10, 2):  # 2, 4, 6, 8 (start, stop, step)
    print(i, end=" ")
print()

"""
============================================================================
                    SECTION 2: FUNCTIONS (20 min)
============================================================================
"""

def calculate_checksum(data):
    """Calculate simple XOR checksum"""
    checksum = 0
    for byte in data:
        checksum ^= byte
    return checksum

def parse_sensor_packet(packet):
    """Parse sensor data packet, return dict"""
    if len(packet) < 4:
        return None

    return {
        "header": packet[0],
        "sensor_id": packet[1],
        "value": (packet[2] << 8) | packet[3],
        "checksum": packet[-1] if len(packet) > 4 else None
    }

# Test it
test_data = [0xAA, 0x01, 0x00, 0xFF, 0x55]
print(f"Checksum: {calculate_checksum(test_data):#04x}")
print(f"Parsed: {parse_sensor_packet(test_data)}")

# Default arguments
def connect_device(port="/dev/ttyUSB0", baud=115200, timeout=5.0):
    print(f"Connecting to {port} at {baud} baud, timeout={timeout}s")
    return True

connect_device()  # Uses defaults
connect_device(port="COM3", baud=9600)  # Override some

"""
============================================================================
                SECTION 3: FILE OPERATIONS (30 min)
============================================================================

Critical for: Log parsing, config files, data export
"""

# Writing to file
def save_log(filename, data):
    with open(filename, 'w') as f:
        for line in data:
            f.write(f"{line}\n")

# Reading file
def read_log(filename):
    with open(filename, 'r') as f:
        return f.readlines()

# Reading binary file (firmware images, etc.)
def read_firmware_file(filename):
    with open(filename, 'rb') as f:
        return f.read()

# CSV handling (sensor data export)
import csv

def export_sensor_data(filename, readings):
    """Export sensor readings to CSV"""
    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['timestamp', 'sensor_id', 'value'])  # Header
        for reading in readings:
            writer.writerow(reading)

def import_sensor_data(filename):
    """Import sensor readings from CSV"""
    readings = []
    with open(filename, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            readings.append(row)
    return readings

# JSON handling (config files, API responses)
import json

config = {
    "device_name": "TempSensor_01",
    "sample_rate": 100,
    "calibration": {"offset": 0.5, "scale": 1.02}
}

# Save config
def save_config(filename, config):
    with open(filename, 'w') as f:
        json.dump(config, f, indent=2)

# Load config
def load_config(filename):
    with open(filename, 'r') as f:
        return json.load(f)

"""
============================================================================
            SECTION 4: SERIAL COMMUNICATION (45 min) - CRITICAL!
============================================================================

This is the #1 Python skill for embedded engineers.
Install: pip install pyserial
"""

# NOTE: This code won't run without a real serial port
# But this is exactly how you'd use it

SERIAL_EXAMPLE = '''
import serial
import time

# Open serial port
ser = serial.Serial(
    port='/dev/ttyUSB0',  # or 'COM3' on Windows
    baudrate=115200,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=1.0  # Read timeout in seconds
)

# Send data
ser.write(b'AT\\r\\n')  # Send AT command

# Read response
response = ser.readline()  # Read until newline or timeout
print(f"Response: {response}")

# Read specific number of bytes
data = ser.read(10)  # Read exactly 10 bytes

# Read all available
data = ser.read_all()

# Check if data available
if ser.in_waiting > 0:
    data = ser.read(ser.in_waiting)

# Close when done
ser.close()
'''

print("Serial communication example (see SERIAL_EXAMPLE string)")

# Mock serial class for practice
class MockSerial:
    """Mock serial port for testing without hardware"""
    def __init__(self, port, baudrate, timeout=1.0):
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout
        self.buffer = b''
        print(f"MockSerial: Opened {port} at {baudrate} baud")

    def write(self, data):
        print(f"MockSerial TX: {data}")
        # Simulate response
        if data == b'AT\r\n':
            self.buffer = b'OK\r\n'
        return len(data)

    def readline(self):
        response = self.buffer
        self.buffer = b''
        return response

    def close(self):
        print("MockSerial: Closed")

# Test with mock
mock_ser = MockSerial('/dev/ttyUSB0', 115200)
mock_ser.write(b'AT\r\n')
print(f"Response: {mock_ser.readline()}")
mock_ser.close()

"""
============================================================================
            SECTION 5: BYTE MANIPULATION (30 min) - CRITICAL!
============================================================================

Firmware deals with bytes. You must be comfortable with this.
"""

# Bytes vs bytearray
data = b'\x01\x02\x03\x04'  # Immutable bytes
data_mutable = bytearray([0x01, 0x02, 0x03, 0x04])  # Mutable

# Access individual bytes
print(f"First byte: {data[0]:#04x}")  # 0x01

# Modify (only bytearray)
data_mutable[0] = 0xFF
print(f"Modified: {data_mutable.hex()}")

# Convert between formats
hex_string = "DEADBEEF"
as_bytes = bytes.fromhex(hex_string)
back_to_hex = as_bytes.hex()
print(f"{hex_string} -> {as_bytes} -> {back_to_hex}")

# Pack/unpack binary data (like C structs)
import struct

# Pack: Python values -> bytes
# '<' = little endian, 'H' = uint16, 'I' = uint32, 'f' = float
packet = struct.pack('<HHf', 0x1234, 0x5678, 3.14159)
print(f"Packed: {packet.hex()}")

# Unpack: bytes -> Python values
header, sensor_id, value = struct.unpack('<HHf', packet)
print(f"Unpacked: header={header:#06x}, sensor={sensor_id:#06x}, value={value:.4f}")

# Common format characters:
# 'b' = int8,   'B' = uint8
# 'h' = int16,  'H' = uint16
# 'i' = int32,  'I' = uint32
# 'f' = float,  'd' = double
# '<' = little endian, '>' = big endian

def build_command_packet(cmd, data):
    """Build packet: [SYNC][LEN][CMD][DATA...][CRC]"""
    SYNC = 0xAA
    length = len(data) + 1  # cmd + data

    packet = bytearray()
    packet.append(SYNC)
    packet.append(length)
    packet.append(cmd)
    packet.extend(data)

    # Calculate CRC (simple XOR)
    crc = 0
    for b in packet[1:]:  # Skip sync
        crc ^= b
    packet.append(crc)

    return bytes(packet)

cmd_packet = build_command_packet(0x01, [0x10, 0x20, 0x30])
print(f"Command packet: {cmd_packet.hex()}")

"""
============================================================================
                SECTION 6: ERROR HANDLING (20 min)
============================================================================
"""

def safe_serial_read(port, baudrate):
    """Demonstrate proper error handling"""
    try:
        # This would be real serial code
        # ser = serial.Serial(port, baudrate, timeout=1.0)
        # data = ser.read(100)
        # ser.close()

        # Simulating potential errors
        if port == "INVALID":
            raise FileNotFoundError(f"Port {port} not found")

        return b"OK"

    except FileNotFoundError as e:
        print(f"Error: Port not found - {e}")
        return None
    except PermissionError:
        print(f"Error: Permission denied for {port}")
        return None
    except Exception as e:
        print(f"Unexpected error: {e}")
        return None
    finally:
        print("Cleanup: Ensuring port is closed")

safe_serial_read("/dev/ttyUSB0", 115200)
safe_serial_read("INVALID", 115200)

# Assert for debugging (disabled in production with -O flag)
def validate_packet(packet):
    assert len(packet) >= 4, "Packet too short"
    assert packet[0] == 0xAA, f"Invalid sync byte: {packet[0]:#x}"
    return True

"""
============================================================================
                SECTION 7: CLASSES (30 min)
============================================================================

For organizing device interfaces, test frameworks, etc.
"""

class SerialDevice:
    """Base class for serial-connected devices"""

    def __init__(self, port, baudrate=115200):
        self.port = port
        self.baudrate = baudrate
        self.connected = False
        # self.serial = None

    def connect(self):
        """Open serial connection"""
        print(f"Connecting to {self.port}...")
        # self.serial = serial.Serial(self.port, self.baudrate, timeout=1.0)
        self.connected = True
        return True

    def disconnect(self):
        """Close serial connection"""
        # if self.serial:
        #     self.serial.close()
        self.connected = False
        print("Disconnected")

    def send_command(self, cmd):
        """Send command and get response"""
        if not self.connected:
            raise RuntimeError("Not connected")
        print(f"TX: {cmd}")
        # self.serial.write(cmd.encode() + b'\r\n')
        # return self.serial.readline().decode().strip()
        return "OK"  # Mock response


class TemperatureSensor(SerialDevice):
    """Specific device class inheriting from SerialDevice"""

    def __init__(self, port):
        super().__init__(port, baudrate=9600)
        self.last_reading = None

    def read_temperature(self):
        """Read temperature from sensor"""
        response = self.send_command("READ_TEMP")
        # Parse response, e.g., "TEMP:25.5"
        # self.last_reading = float(response.split(':')[1])
        self.last_reading = 25.5  # Mock
        return self.last_reading

    def set_sample_rate(self, rate_hz):
        """Configure sample rate"""
        return self.send_command(f"SET_RATE:{rate_hz}")

# Usage
sensor = TemperatureSensor("/dev/ttyUSB0")
sensor.connect()
print(f"Temperature: {sensor.read_temperature()}°C")
sensor.disconnect()

"""
============================================================================
            SECTION 8: SUBPROCESS (20 min)
============================================================================

Running external commands (flash tools, build scripts, etc.)
"""

import subprocess

def flash_firmware(hex_file, port):
    """Flash firmware using external tool"""
    cmd = ["echo", "Flashing", hex_file, "to", port]  # Mock command
    # Real: cmd = ["esptool.py", "--port", port, "write_flash", "0x0", hex_file]

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode == 0:
        print(f"Flash successful: {result.stdout}")
        return True
    else:
        print(f"Flash failed: {result.stderr}")
        return False

flash_firmware("firmware.bin", "/dev/ttyUSB0")

def run_tests():
    """Run test suite and capture output"""
    result = subprocess.run(
        ["python", "-c", "print('Tests passed!')"],
        capture_output=True,
        text=True,
        timeout=30  # Timeout in seconds
    )
    return result.returncode == 0

"""
============================================================================
            SECTION 9: LOGGING (15 min)
============================================================================

Better than print() for real tools
"""

import logging

# Configure logging
logging.basicConfig(
    level=logging.DEBUG,
    format='%(asctime)s [%(levelname)s] %(message)s',
    datefmt='%H:%M:%S'
)

logger = logging.getLogger(__name__)

def device_test_with_logging():
    logger.info("Starting device test")
    logger.debug("Checking serial port...")

    # Simulate test
    logger.warning("Low battery detected")
    logger.error("Failed to read sensor")

    logger.info("Test complete")

# device_test_with_logging()  # Uncomment to see output

"""
============================================================================
            SECTION 10: THREADING (20 min)
============================================================================

For: Concurrent serial reads, background monitoring, timeouts
"""

import threading
import time

class DeviceMonitor:
    """Monitor device in background thread"""

    def __init__(self):
        self.running = False
        self.thread = None
        self.readings = []

    def start(self):
        """Start monitoring in background"""
        self.running = True
        self.thread = threading.Thread(target=self._monitor_loop)
        self.thread.start()
        print("Monitor started")

    def stop(self):
        """Stop monitoring"""
        self.running = False
        if self.thread:
            self.thread.join()
        print("Monitor stopped")

    def _monitor_loop(self):
        """Background monitoring loop"""
        while self.running:
            # Read from device
            reading = 25.0 + (time.time() % 1)  # Mock reading
            self.readings.append(reading)
            time.sleep(0.5)

    def get_readings(self):
        return self.readings.copy()

# Demo (brief)
monitor = DeviceMonitor()
monitor.start()
time.sleep(1.5)  # Let it run
monitor.stop()
print(f"Collected {len(monitor.readings)} readings")

"""
============================================================================
            SECTION 11: ARGPARSE (15 min)
============================================================================

For making proper CLI tools
"""

import argparse

def create_cli():
    """Create argument parser for device tool"""
    parser = argparse.ArgumentParser(description='Device Programming Tool')

    parser.add_argument('--port', '-p',
                        default='/dev/ttyUSB0',
                        help='Serial port (default: /dev/ttyUSB0)')

    parser.add_argument('--baud', '-b',
                        type=int, default=115200,
                        help='Baud rate (default: 115200)')

    parser.add_argument('--firmware', '-f',
                        required=False,
                        help='Firmware file to flash')

    parser.add_argument('--verbose', '-v',
                        action='store_true',
                        help='Enable verbose output')

    return parser

# Demo (would normally be in if __name__ == '__main__')
# parser = create_cli()
# args = parser.parse_args()
# print(f"Port: {args.port}, Baud: {args.baud}")

"""
============================================================================
                    CODING PRACTICE PROBLEMS
============================================================================
"""

print("\n" + "="*60)
print("PRACTICE PROBLEMS")
print("="*60)

# PROBLEM 1: Parse log file for errors
def count_errors_in_log(log_lines):
    """
    Count lines containing 'ERROR' or 'FAIL'
    Input: List of log lines
    Output: Count of error lines
    """
    count = 0
    for line in log_lines:
        if 'ERROR' in line.upper() or 'FAIL' in line.upper():
            count += 1
    return count

test_log = [
    "[INFO] Device started",
    "[ERROR] Sensor timeout",
    "[INFO] Retry succeeded",
    "[FAIL] Connection lost"
]
print(f"Problem 1: Errors found = {count_errors_in_log(test_log)}")


# PROBLEM 2: Calculate CRC-8
def crc8(data):
    """
    Calculate CRC-8 (polynomial 0x07)
    Input: bytes or list of integers
    Output: CRC value (0-255)
    """
    crc = 0
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 0x80:
                crc = (crc << 1) ^ 0x07
            else:
                crc <<= 1
            crc &= 0xFF
    return crc

print(f"Problem 2: CRC-8 = {crc8([0x01, 0x02, 0x03]):#04x}")


# PROBLEM 3: Parse AT command response
def parse_at_response(response):
    """
    Parse AT+CSQ response: "+CSQ: 18,0"
    Return: {'rssi': 18, 'ber': 0} or None if invalid
    """
    try:
        if not response.startswith('+CSQ:'):
            return None
        values = response.split(':')[1].strip().split(',')
        return {
            'rssi': int(values[0]),
            'ber': int(values[1])
        }
    except (IndexError, ValueError):
        return None

print(f"Problem 3: Parsed = {parse_at_response('+CSQ: 18,0')}")


# PROBLEM 4: Convert temperature (ADC to Celsius)
def adc_to_celsius(adc_value, vref=3.3, bits=12):
    """
    Convert ADC reading to temperature (10mV/°C sensor, 500mV at 0°C)
    Input: ADC value (0 to 2^bits-1)
    Output: Temperature in Celsius
    """
    voltage = (adc_value / ((1 << bits) - 1)) * vref
    temp_c = (voltage - 0.5) / 0.01  # 10mV per degree, 500mV offset
    return temp_c

print(f"Problem 4: ADC 1861 = {adc_to_celsius(1861):.1f}°C")


# PROBLEM 5: Find devices on multiple ports
def scan_ports(port_list):
    """
    Mock scanning serial ports for devices
    Return: List of ports that respond to "AT" command
    """
    active = []
    for port in port_list:
        # In real code: try opening and sending AT command
        if "USB" in port:  # Mock: USB ports have devices
            active.append(port)
    return active

ports = ["/dev/ttyUSB0", "/dev/ttyS0", "/dev/ttyUSB1", "/dev/ttyACM0"]
print(f"Problem 5: Active ports = {scan_ports(ports)}")


# PROBLEM 6: Merge firmware chunks
def merge_chunks(chunks):
    """
    Merge list of byte chunks into single firmware blob
    Input: List of bytes objects
    Output: Single merged bytes object
    """
    return b''.join(chunks)

chunks = [b'\x00\x01', b'\x02\x03', b'\x04\x05']
print(f"Problem 6: Merged = {merge_chunks(chunks).hex()}")


# PROBLEM 7: Rate limiting
class RateLimiter:
    """
    Allow at most N calls per second
    """
    def __init__(self, calls_per_second):
        self.interval = 1.0 / calls_per_second
        self.last_call = 0

    def wait(self):
        """Wait if needed to respect rate limit"""
        now = time.time()
        elapsed = now - self.last_call
        if elapsed < self.interval:
            time.sleep(self.interval - elapsed)
        self.last_call = time.time()

print("Problem 7: RateLimiter class implemented")


# PROBLEM 8: Timeout decorator
def timeout_after(seconds):
    """Decorator to add timeout to function (simplified)"""
    def decorator(func):
        def wrapper(*args, **kwargs):
            # In real code, use threading or signal
            # This is simplified for demo
            start = time.time()
            result = func(*args, **kwargs)
            elapsed = time.time() - start
            if elapsed > seconds:
                raise TimeoutError(f"Function took {elapsed:.2f}s, limit was {seconds}s")
            return result
        return wrapper
    return decorator

@timeout_after(5.0)
def slow_function():
    time.sleep(0.1)
    return "done"

print(f"Problem 8: {slow_function()}")


"""
============================================================================
                    INTERVIEW Q&A
============================================================================

Q1: "How would you automate flashing 100 devices in production?"
A1:
   1. Script to scan for connected devices (scan serial ports)
   2. Parallel flashing using threading (one thread per device)
   3. Log results to CSV (device ID, pass/fail, time)
   4. Verify each flash by reading back or running self-test
   5. Handle failures gracefully (retry, mark for review)

Q2: "Parse this log file and find all temperature readings above 50°C"
A2:
   with open('log.txt', 'r') as f:
       for line in f:
           if 'TEMP:' in line:
               temp = float(line.split('TEMP:')[1].split()[0])
               if temp > 50:
                   print(line.strip())

Q3: "How do you handle serial port timeouts?"
A3:
   - Set timeout when opening port: serial.Serial(..., timeout=1.0)
   - Check if read() returned empty bytes: if not data: handle_timeout()
   - Use try/except for serial.SerialTimeoutException
   - Implement retry logic with backoff
   - Log timeout events for debugging

Q4: "Difference between bytes and bytearray?"
A4:
   bytes: Immutable, use for received data, constants
   bytearray: Mutable, use when building packets byte-by-byte
   Both are sequences of integers 0-255

Q5: "How would you test a serial protocol without hardware?"
A5:
   1. Create mock serial class (like MockSerial above)
   2. Use virtual serial ports (socat on Linux, com0com on Windows)
   3. Dependency injection: pass serial object to functions
   4. Unit test packet building/parsing separately

============================================================================
                    QUICK REFERENCE CHEAT SHEET
============================================================================

# Hex formatting
f"{value:#04x}"      # 0x1a
f"{value:02X}"       # 1A

# Bytes
b'\x01\x02'          # Byte literal
bytes.fromhex("0102") # From hex string
data.hex()           # To hex string

# Struct pack/unpack
struct.pack('<HH', 1, 2)    # Little-endian 2x uint16
struct.unpack('<I', data)   # Little-endian uint32

# Serial (pyserial)
ser = serial.Serial(port, baud, timeout=1.0)
ser.write(b'data')
data = ser.read(n)
ser.readline()
ser.close()

# File
with open(f, 'r') as file:   # Text
with open(f, 'rb') as file:  # Binary
json.load(f) / json.dump(obj, f)
csv.reader(f) / csv.writer(f)

# Command line
subprocess.run(['cmd', 'arg'], capture_output=True)
argparse.ArgumentParser()

============================================================================
"""

if __name__ == '__main__':
    print("\n" + "="*60)
    print("Python crash course for embedded engineers complete!")
    print("Total time: ~4 hours if you practice each section")
    print("="*60)
