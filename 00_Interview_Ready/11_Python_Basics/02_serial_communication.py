"""
============================================================================
            SERIAL COMMUNICATION IN PYTHON (DEEP DIVE)
============================================================================

This is THE most important Python skill for embedded engineers.
Everything else is secondary to this.

Install: pip install pyserial

============================================================================
                    BASIC SERIAL OPERATIONS
============================================================================
"""

import time

# NOTE: Most code here uses MockSerial for practice
# Replace with real `import serial` and `serial.Serial` for actual hardware

class MockSerial:
    """Mock serial for practice without hardware"""
    def __init__(self, port, baudrate=115200, timeout=1.0, **kwargs):
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout
        self._rx_buffer = b''
        self._is_open = True
        print(f"[Mock] Opened {port} @ {baudrate}")

    def write(self, data):
        print(f"[Mock TX] {data}")
        # Simulate responses
        if b'AT\r\n' in data:
            self._rx_buffer = b'OK\r\n'
        elif b'AT+VERSION' in data:
            self._rx_buffer = b'+VERSION: 1.2.3\r\nOK\r\n'
        elif b'AT+CSQ' in data:
            self._rx_buffer = b'+CSQ: 18,0\r\nOK\r\n'
        elif b'READ_TEMP' in data:
            self._rx_buffer = b'TEMP:25.5\r\n'
        else:
            self._rx_buffer = b'ERROR\r\n'
        return len(data)

    def read(self, size=1):
        data = self._rx_buffer[:size]
        self._rx_buffer = self._rx_buffer[size:]
        return data

    def readline(self):
        if b'\n' in self._rx_buffer:
            idx = self._rx_buffer.index(b'\n') + 1
            line = self._rx_buffer[:idx]
            self._rx_buffer = self._rx_buffer[idx:]
            return line
        data = self._rx_buffer
        self._rx_buffer = b''
        return data

    def read_all(self):
        data = self._rx_buffer
        self._rx_buffer = b''
        return data

    @property
    def in_waiting(self):
        return len(self._rx_buffer)

    def close(self):
        self._is_open = False
        print(f"[Mock] Closed {self.port}")

    def flush(self):
        pass

    def reset_input_buffer(self):
        self._rx_buffer = b''

    def reset_output_buffer(self):
        pass

# Use MockSerial for examples (replace with serial.Serial for real hardware)
Serial = MockSerial


"""
============================================================================
                    PATTERN 1: SIMPLE COMMAND-RESPONSE
============================================================================
"""

def send_at_command(ser, command, timeout=2.0):
    """
    Send AT command and wait for response.
    Returns: Response string or None on timeout
    """
    # Clear any pending data
    ser.reset_input_buffer()

    # Send command
    cmd = f"{command}\r\n".encode()
    ser.write(cmd)

    # Wait for response
    response_lines = []
    start_time = time.time()

    while time.time() - start_time < timeout:
        if ser.in_waiting > 0:
            line = ser.readline().decode().strip()
            if line:
                response_lines.append(line)
                # Check for terminator
                if line in ['OK', 'ERROR', 'FAIL']:
                    break
        time.sleep(0.01)

    return response_lines if response_lines else None


# Test it
print("=== Pattern 1: Simple AT Commands ===")
ser = Serial('/dev/ttyUSB0', 115200, timeout=1.0)
print(f"AT response: {send_at_command(ser, 'AT')}")
print(f"VERSION: {send_at_command(ser, 'AT+VERSION')}")
ser.close()


"""
============================================================================
                PATTERN 2: BINARY PROTOCOL
============================================================================

Many embedded devices use binary packets, not text AT commands.
"""

def build_packet(cmd, payload):
    """
    Build binary packet: [SYNC][LEN][CMD][PAYLOAD][CRC]
    """
    SYNC = 0xAA
    length = len(payload) + 1  # cmd + payload

    packet = bytearray()
    packet.append(SYNC)
    packet.append(length)
    packet.append(cmd)
    packet.extend(payload)

    # CRC: XOR of all bytes except sync
    crc = 0
    for b in packet[1:]:
        crc ^= b
    packet.append(crc)

    return bytes(packet)


def parse_packet(data):
    """
    Parse binary packet, validate CRC.
    Returns: (cmd, payload) or None if invalid
    """
    if len(data) < 4:
        return None

    if data[0] != 0xAA:
        return None  # Bad sync

    length = data[1]
    if len(data) < length + 3:  # sync + len + data + crc
        return None

    # Verify CRC
    crc = 0
    for b in data[1:-1]:
        crc ^= b

    if crc != data[-1]:
        return None  # CRC mismatch

    cmd = data[2]
    payload = data[3:-1]

    return (cmd, payload)


def send_binary_command(ser, cmd, payload, timeout=2.0):
    """Send binary packet and receive response"""
    packet = build_packet(cmd, payload)
    print(f"TX: {packet.hex()}")
    ser.write(packet)

    # Wait for response (in real code, read until sync found)
    time.sleep(0.1)  # Give device time to respond
    response = ser.read_all()

    if response:
        print(f"RX: {response.hex()}")
        return parse_packet(response)
    return None


print("\n=== Pattern 2: Binary Protocol ===")
packet = build_packet(0x01, [0x10, 0x20])
print(f"Built packet: {packet.hex()}")
parsed = parse_packet(packet)
print(f"Parsed: cmd={parsed[0]:#x}, payload={list(parsed[1])}")


"""
============================================================================
                PATTERN 3: STREAMING DATA
============================================================================

For continuous sensor data, logs, etc.
"""

class DataStreamReader:
    """Read continuous data stream from serial port"""

    def __init__(self, port, baudrate=115200):
        self.ser = Serial(port, baudrate, timeout=0.1)
        self.running = False
        self.buffer = b''

    def start(self, callback):
        """
        Start reading data stream.
        callback(data): Called for each complete packet/line
        """
        self.running = True
        print("Starting data stream reader...")

        # Simulate some incoming data
        self.ser._rx_buffer = b'TEMP:25.5\r\nTEMP:25.6\r\nTEMP:25.4\r\n'

        while self.running:
            # Read available data
            chunk = self.ser.read(64)
            if chunk:
                self.buffer += chunk

            # Process complete lines
            while b'\n' in self.buffer:
                line, self.buffer = self.buffer.split(b'\n', 1)
                line = line.strip()
                if line:
                    callback(line)

            # In real code, this would loop forever
            # Breaking here for demo
            if not self.ser._rx_buffer and not self.buffer:
                break

        print("Stream reader stopped")

    def stop(self):
        self.running = False
        self.ser.close()


def handle_sensor_data(data):
    """Callback for each data packet"""
    line = data.decode()
    if line.startswith('TEMP:'):
        temp = float(line.split(':')[1])
        print(f"  Temperature: {temp}°C")


print("\n=== Pattern 3: Streaming Data ===")
reader = DataStreamReader('/dev/ttyUSB0')
reader.start(handle_sensor_data)
reader.stop()


"""
============================================================================
            PATTERN 4: STATE MACHINE FOR COMPLEX PROTOCOLS
============================================================================
"""

from enum import Enum

class RxState(Enum):
    WAIT_SYNC = 0
    WAIT_LENGTH = 1
    WAIT_DATA = 2
    WAIT_CRC = 3

class PacketReceiver:
    """State machine for receiving binary packets"""

    def __init__(self):
        self.state = RxState.WAIT_SYNC
        self.length = 0
        self.data = bytearray()
        self.packets = []

    def feed(self, byte):
        """Feed one byte to the state machine"""

        if self.state == RxState.WAIT_SYNC:
            if byte == 0xAA:
                self.data = bytearray([byte])
                self.state = RxState.WAIT_LENGTH

        elif self.state == RxState.WAIT_LENGTH:
            self.length = byte
            self.data.append(byte)
            self.state = RxState.WAIT_DATA

        elif self.state == RxState.WAIT_DATA:
            self.data.append(byte)
            if len(self.data) == self.length + 2:  # sync + len + data
                self.state = RxState.WAIT_CRC

        elif self.state == RxState.WAIT_CRC:
            self.data.append(byte)
            # Validate and store packet
            crc = 0
            for b in self.data[1:-1]:
                crc ^= b
            if crc == byte:
                self.packets.append(bytes(self.data))
            self.state = RxState.WAIT_SYNC

    def get_packets(self):
        """Get and clear received packets"""
        pkts = self.packets
        self.packets = []
        return pkts


print("\n=== Pattern 4: Packet State Machine ===")
receiver = PacketReceiver()

# Simulate noisy stream with valid packet embedded
stream = bytes([0x00, 0xFF, 0xAA, 0x02, 0x01, 0x55, 0x56, 0x00, 0xAA, 0x01, 0x02, 0x03])
for byte in stream:
    receiver.feed(byte)

for pkt in receiver.get_packets():
    print(f"  Valid packet: {pkt.hex()}")


"""
============================================================================
                PATTERN 5: RETRIES AND ERROR HANDLING
============================================================================
"""

def reliable_command(ser, command, max_retries=3, timeout=2.0):
    """
    Send command with retries on failure.
    Returns: Response or raises exception after max retries
    """
    last_error = None

    for attempt in range(max_retries):
        try:
            ser.reset_input_buffer()
            ser.write(f"{command}\r\n".encode())

            # Wait for response
            start = time.time()
            while time.time() - start < timeout:
                if ser.in_waiting > 0:
                    response = ser.readline().decode().strip()
                    if response == 'OK':
                        return response
                    elif response == 'ERROR':
                        raise ValueError("Device returned ERROR")
                time.sleep(0.01)

            raise TimeoutError("No response")

        except (TimeoutError, ValueError) as e:
            last_error = e
            print(f"  Attempt {attempt + 1} failed: {e}")
            time.sleep(0.5 * (attempt + 1))  # Backoff

    raise last_error


print("\n=== Pattern 5: Reliable Command ===")
ser = Serial('/dev/ttyUSB0', 115200)
try:
    result = reliable_command(ser, 'AT')
    print(f"Command succeeded: {result}")
except Exception as e:
    print(f"Command failed after retries: {e}")
ser.close()


"""
============================================================================
                PATTERN 6: DEVICE CLASS WITH CONTEXT MANAGER
============================================================================
"""

class Device:
    """Clean device interface with automatic cleanup"""

    def __init__(self, port, baudrate=115200):
        self.port = port
        self.baudrate = baudrate
        self.ser = None

    def __enter__(self):
        """Called when entering 'with' block"""
        self.ser = Serial(self.port, self.baudrate, timeout=1.0)
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        """Called when exiting 'with' block (even on exception)"""
        if self.ser:
            self.ser.close()
        return False  # Don't suppress exceptions

    def get_version(self):
        self.ser.write(b'AT+VERSION\r\n')
        time.sleep(0.1)
        return self.ser.read_all().decode()

    def read_temperature(self):
        self.ser.write(b'READ_TEMP\r\n')
        time.sleep(0.1)
        response = self.ser.readline().decode()
        if response.startswith('TEMP:'):
            return float(response.split(':')[1])
        return None


print("\n=== Pattern 6: Context Manager ===")
with Device('/dev/ttyUSB0') as dev:
    print(f"Version: {dev.get_version()}")
    print(f"Temperature: {dev.read_temperature()}°C")
# Serial port automatically closed here, even if exception occurred


"""
============================================================================
                    PRACTICE EXERCISES
============================================================================
"""

print("\n" + "="*60)
print("PRACTICE EXERCISES")
print("="*60)

# Exercise 1: Parse GPS NMEA sentence
def parse_gps_gprmc(sentence):
    """
    Parse GPRMC sentence: $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
    Return: {'time': '123519', 'lat': 48.1173, 'lon': 11.5167, 'speed': 22.4} or None
    """
    try:
        if not sentence.startswith('$GPRMC'):
            return None

        # Remove checksum
        sentence = sentence.split('*')[0]
        parts = sentence.split(',')

        time_str = parts[1]
        lat = float(parts[3][:2]) + float(parts[3][2:]) / 60
        if parts[4] == 'S':
            lat = -lat
        lon = float(parts[5][:3]) + float(parts[5][3:]) / 60
        if parts[6] == 'W':
            lon = -lon
        speed = float(parts[7]) if parts[7] else 0

        return {'time': time_str, 'lat': round(lat, 4), 'lon': round(lon, 4), 'speed': speed}
    except (IndexError, ValueError):
        return None

gps = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A"
print(f"Exercise 1 - GPS: {parse_gps_gprmc(gps)}")


# Exercise 2: Implement SLIP encoding (RFC 1055)
def slip_encode(data):
    """
    SLIP encode data:
    END (0xC0) -> ESC (0xDB) + ESC_END (0xDC)
    ESC (0xDB) -> ESC (0xDB) + ESC_ESC (0xDD)
    Add END at start and end
    """
    END = 0xC0
    ESC = 0xDB
    ESC_END = 0xDC
    ESC_ESC = 0xDD

    encoded = bytearray([END])
    for byte in data:
        if byte == END:
            encoded.extend([ESC, ESC_END])
        elif byte == ESC:
            encoded.extend([ESC, ESC_ESC])
        else:
            encoded.append(byte)
    encoded.append(END)

    return bytes(encoded)

test_data = bytes([0x01, 0xC0, 0x02, 0xDB, 0x03])
print(f"Exercise 2 - SLIP: {slip_encode(test_data).hex()}")


# Exercise 3: Calculate Modbus CRC-16
def modbus_crc16(data):
    """Calculate Modbus CRC-16"""
    crc = 0xFFFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 0x0001:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc

print(f"Exercise 3 - Modbus CRC: {modbus_crc16([0x01, 0x03, 0x00, 0x00, 0x00, 0x0A]):#06x}")


"""
============================================================================
                    INTERVIEW Q&A - SERIAL COMMUNICATION
============================================================================

Q1: "What's the difference between read() and readline()?"
A1:
   read(n): Returns exactly n bytes (or less if timeout)
   readline(): Returns bytes until \n found (or timeout)
   read_all(): Returns all available bytes in buffer

Q2: "How do you handle partial packets in binary protocol?"
A2:
   Use state machine (like PacketReceiver above):
   - Buffer incoming bytes
   - Track state: waiting for sync, length, data, crc
   - Only emit complete, validated packets
   - Handle sync loss by resetting state

Q3: "Serial port works in terminal but not in Python. Why?"
A3:
   1. Port still open in terminal (only one app can use it)
   2. Wrong permissions (need dialout group on Linux)
   3. Wrong port name (check /dev/tty* or COM*)
   4. Different baud rate or settings
   5. Missing DTR/RTS handling (some devices need these)

Q4: "How would you test serial code without hardware?"
A4:
   1. Mock class (like MockSerial above)
   2. Virtual serial port pair (socat, com0com)
   3. Hardware loopback (TX->RX wire)
   4. Unit test packet parsing separately

Q5: "Device responds but data is garbage. Debug steps?"
A5:
   1. Verify baud rate matches device
   2. Check data bits, parity, stop bits (8N1 most common)
   3. Voltage levels (TTL 3.3V vs 5V vs RS232 ±12V)
   4. TX/RX swapped? (crossover needed)
   5. Check with oscilloscope/logic analyzer

============================================================================
"""

if __name__ == '__main__':
    print("\nSerial communication practice complete!")
    print("Replace MockSerial with serial.Serial for real hardware")
