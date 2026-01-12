"""
============================================================================
        TESTING AND TOOLS FOR EMBEDDED ENGINEERS
============================================================================

Python for:
- Unit testing firmware helper functions
- Hardware-in-loop testing
- Test automation frameworks
- Factory test scripts

============================================================================
                    SECTION 1: UNIT TESTING WITH PYTEST
============================================================================

Install: pip install pytest

Run tests: pytest filename.py -v
"""

# Code under test (would normally be in separate file)
def calculate_crc8(data):
    """Calculate CRC-8"""
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


def parse_sensor_packet(packet):
    """Parse sensor data packet"""
    if len(packet) < 4:
        return None

    sync = packet[0]
    if sync != 0xAA:
        return None

    return {
        'sync': sync,
        'length': packet[1],
        'cmd': packet[2],
        'data': packet[3:-1],
        'crc': packet[-1]
    }


def adc_to_voltage(adc_value, vref=3.3, bits=12):
    """Convert ADC reading to voltage"""
    max_val = (1 << bits) - 1
    return (adc_value / max_val) * vref


# ============================================================================
# TEST CASES (pytest will find these automatically)
# ============================================================================

def test_crc8_empty():
    """CRC of empty data should be 0"""
    assert calculate_crc8([]) == 0


def test_crc8_single_byte():
    """CRC of single byte"""
    result = calculate_crc8([0x01])
    assert result == 0x07  # Known value


def test_crc8_multiple_bytes():
    """CRC of multiple bytes"""
    result = calculate_crc8([0x01, 0x02, 0x03])
    assert isinstance(result, int)
    assert 0 <= result <= 255


def test_parse_valid_packet():
    """Parse valid packet"""
    packet = [0xAA, 0x03, 0x01, 0x10, 0x20, 0x55]
    result = parse_sensor_packet(packet)

    assert result is not None
    assert result['sync'] == 0xAA
    assert result['cmd'] == 0x01
    assert result['data'] == [0x10, 0x20]


def test_parse_invalid_sync():
    """Invalid sync byte returns None"""
    packet = [0xBB, 0x03, 0x01, 0x10, 0x20, 0x55]
    assert parse_sensor_packet(packet) is None


def test_parse_short_packet():
    """Packet too short returns None"""
    packet = [0xAA, 0x01]
    assert parse_sensor_packet(packet) is None


def test_adc_zero():
    """ADC value 0 should give 0V"""
    assert adc_to_voltage(0) == 0.0


def test_adc_max():
    """ADC max value should give vref"""
    result = adc_to_voltage(4095, vref=3.3, bits=12)
    assert abs(result - 3.3) < 0.001


def test_adc_midpoint():
    """ADC midpoint should give half vref"""
    result = adc_to_voltage(2048, vref=3.3, bits=12)
    assert abs(result - 1.65) < 0.01


# Parametrized tests (multiple inputs, same test)
import pytest

@pytest.mark.parametrize("adc,expected", [
    (0, 0.0),
    (2048, 1.65),
    (4095, 3.3),
])
def test_adc_values(adc, expected):
    """Test multiple ADC values"""
    result = adc_to_voltage(adc, vref=3.3, bits=12)
    assert abs(result - expected) < 0.01


"""
============================================================================
                SECTION 2: TEST FIXTURES
============================================================================

Fixtures set up test resources (serial ports, mock devices, etc.)
"""

@pytest.fixture
def mock_device():
    """Create mock device for testing"""
    class MockDevice:
        def __init__(self):
            self.connected = False
            self.commands_sent = []

        def connect(self):
            self.connected = True

        def send(self, cmd):
            self.commands_sent.append(cmd)
            return "OK"

        def disconnect(self):
            self.connected = False

    device = MockDevice()
    device.connect()
    yield device  # Test runs here
    device.disconnect()  # Cleanup after test


def test_device_command(mock_device):
    """Test sending command to mock device"""
    response = mock_device.send("AT+VERSION")
    assert response == "OK"
    assert "AT+VERSION" in mock_device.commands_sent


@pytest.fixture
def sample_packet():
    """Provide sample packet for tests"""
    return bytes([0xAA, 0x03, 0x01, 0x10, 0x20, 0x55])


def test_packet_length(sample_packet):
    """Test using fixture"""
    assert len(sample_packet) == 6


"""
============================================================================
            SECTION 3: HARDWARE TEST FRAMEWORK
============================================================================
"""

from enum import Enum
from dataclasses import dataclass
from typing import List, Optional
import time

class TestResult(Enum):
    PASS = "PASS"
    FAIL = "FAIL"
    SKIP = "SKIP"
    ERROR = "ERROR"


@dataclass
class TestCase:
    name: str
    result: TestResult = TestResult.SKIP
    duration: float = 0.0
    message: str = ""


class HardwareTestRunner:
    """Framework for running hardware tests"""

    def __init__(self, device_port: str):
        self.device_port = device_port
        self.tests: List[TestCase] = []
        self.setup_func = None
        self.teardown_func = None

    def setup(self, func):
        """Decorator to register setup function"""
        self.setup_func = func
        return func

    def teardown(self, func):
        """Decorator to register teardown function"""
        self.teardown_func = func
        return func

    def test(self, name: str):
        """Decorator to register a test"""
        def decorator(func):
            def wrapper():
                tc = TestCase(name=name)
                start = time.time()

                try:
                    result = func()
                    tc.result = TestResult.PASS if result else TestResult.FAIL
                except AssertionError as e:
                    tc.result = TestResult.FAIL
                    tc.message = str(e)
                except Exception as e:
                    tc.result = TestResult.ERROR
                    tc.message = str(e)

                tc.duration = time.time() - start
                self.tests.append(tc)
                return tc

            wrapper._test_name = name
            wrapper._is_test = True
            return wrapper
        return decorator

    def run_all(self):
        """Run all registered tests"""
        print(f"\n{'='*60}")
        print(f"Running tests on {self.device_port}")
        print('='*60)

        # Setup
        if self.setup_func:
            print("Running setup...")
            self.setup_func()

        # Find and run all test methods
        for name in dir(self):
            attr = getattr(self, name)
            if callable(attr) and hasattr(attr, '_is_test'):
                print(f"\nRunning: {attr._test_name}... ", end="")
                result = attr()
                print(f"{result.result.value} ({result.duration:.3f}s)")
                if result.message:
                    print(f"  Message: {result.message}")

        # Teardown
        if self.teardown_func:
            print("\nRunning teardown...")
            self.teardown_func()

        self.print_summary()

    def print_summary(self):
        """Print test summary"""
        passed = sum(1 for t in self.tests if t.result == TestResult.PASS)
        failed = sum(1 for t in self.tests if t.result == TestResult.FAIL)
        errors = sum(1 for t in self.tests if t.result == TestResult.ERROR)

        print(f"\n{'='*60}")
        print(f"SUMMARY: {len(self.tests)} tests")
        print(f"  Passed: {passed}")
        print(f"  Failed: {failed}")
        print(f"  Errors: {errors}")
        print('='*60)


# Example usage of the test framework
class DeviceTests(HardwareTestRunner):
    """Test suite for our device"""

    def __init__(self, port):
        super().__init__(port)
        self.device = None

    @HardwareTestRunner.setup
    def setup(self):
        print(f"  Connecting to device on {self.device_port}")
        # self.device = serial.Serial(self.device_port, 115200)
        self.device = "MockDevice"

    @HardwareTestRunner.teardown
    def teardown(self):
        print("  Disconnecting device")
        # self.device.close()
        self.device = None

    @HardwareTestRunner.test("Check device responds to AT")
    def test_at_command(self):
        # In real code: send AT, check response
        return True  # Mock pass

    @HardwareTestRunner.test("Check firmware version")
    def test_version(self):
        # In real code: AT+VERSION, parse response
        version = "1.2.3"
        assert version.startswith("1."), f"Unexpected version: {version}"
        return True

    @HardwareTestRunner.test("GPIO test - LED toggle")
    def test_gpio(self):
        # In real code: toggle GPIO, verify with multimeter/scope
        return True

    @HardwareTestRunner.test("Read temperature sensor")
    def test_temp_sensor(self):
        temp = 25.5  # Mock reading
        assert 10 < temp < 50, f"Temperature out of range: {temp}"
        return True

    @HardwareTestRunner.test("WiFi connection")
    def test_wifi(self):
        # This one will fail for demo
        raise AssertionError("WiFi not configured")


# Run tests (comment out if running with pytest)
# runner = DeviceTests("/dev/ttyUSB0")
# runner.run_all()


"""
============================================================================
                SECTION 4: FACTORY TEST SCRIPT
============================================================================
"""

import json
from datetime import datetime

class FactoryTest:
    """Complete factory test for production line"""

    def __init__(self, port, station_id):
        self.port = port
        self.station_id = station_id
        self.results = {
            'timestamp': None,
            'station_id': station_id,
            'device_id': None,
            'firmware_version': None,
            'tests': [],
            'overall_pass': False
        }

    def log(self, message):
        """Log to console with timestamp"""
        ts = datetime.now().strftime('%H:%M:%S.%f')[:-3]
        print(f"[{ts}] {message}")

    def run_test(self, name, test_func, *args, **kwargs):
        """Run a single test and record result"""
        self.log(f"Running: {name}")
        start = time.time()

        try:
            result = test_func(*args, **kwargs)
            passed = result is True or result is None
            message = ""
        except Exception as e:
            passed = False
            message = str(e)
            result = None

        duration = time.time() - start

        test_result = {
            'name': name,
            'passed': passed,
            'duration': duration,
            'message': message,
            'value': result if result not in [True, False, None] else None
        }

        self.results['tests'].append(test_result)
        status = "PASS" if passed else "FAIL"
        self.log(f"  Result: {status} ({duration:.3f}s)")

        return passed

    def test_communication(self):
        """Test basic serial communication"""
        # Send AT, expect OK
        return True  # Mock

    def test_read_device_id(self):
        """Read and store device ID"""
        device_id = "DEV123456"  # Mock
        self.results['device_id'] = device_id
        return device_id

    def test_firmware_version(self):
        """Check firmware version"""
        version = "1.2.3"  # Mock
        self.results['firmware_version'] = version
        # Could add version check here
        return version

    def test_gpio_outputs(self):
        """Test all GPIO outputs"""
        # Toggle each GPIO, verify with test fixture
        return True

    def test_gpio_inputs(self):
        """Test all GPIO inputs"""
        # Read inputs, verify expected values
        return True

    def test_adc_calibration(self):
        """Verify ADC calibration"""
        # Read known voltage, compare to expected
        expected = 1650  # 1.65V in mV
        actual = 1648    # Mock reading
        error = abs(actual - expected)
        if error > 10:  # 10mV tolerance
            raise AssertionError(f"ADC error: {error}mV")
        return actual

    def test_sensors(self):
        """Test all onboard sensors"""
        # Read temp, humidity, etc.
        return True

    def test_wifi_module(self):
        """Test WiFi hardware"""
        # Initialize, scan, verify response
        return True

    def test_flash_storage(self):
        """Test flash read/write"""
        # Write test pattern, read back, verify
        return True

    def run_all(self):
        """Run complete factory test suite"""
        self.results['timestamp'] = datetime.now().isoformat()

        self.log("="*50)
        self.log(f"FACTORY TEST - Station {self.station_id}")
        self.log("="*50)

        # Run all tests
        all_passed = True
        all_passed &= self.run_test("Communication", self.test_communication)
        all_passed &= self.run_test("Device ID", self.test_read_device_id)
        all_passed &= self.run_test("Firmware Version", self.test_firmware_version)
        all_passed &= self.run_test("GPIO Outputs", self.test_gpio_outputs)
        all_passed &= self.run_test("GPIO Inputs", self.test_gpio_inputs)
        all_passed &= self.run_test("ADC Calibration", self.test_adc_calibration)
        all_passed &= self.run_test("Sensors", self.test_sensors)
        all_passed &= self.run_test("WiFi Module", self.test_wifi_module)
        all_passed &= self.run_test("Flash Storage", self.test_flash_storage)

        self.results['overall_pass'] = all_passed

        # Print summary
        self.log("="*50)
        if all_passed:
            self.log("OVERALL: PASS - Device ready for shipping")
        else:
            self.log("OVERALL: FAIL - Device needs review")
            failed = [t['name'] for t in self.results['tests'] if not t['passed']]
            self.log(f"Failed tests: {', '.join(failed)}")
        self.log("="*50)

        return self.results

    def save_results(self, filename):
        """Save results to JSON file"""
        with open(filename, 'w') as f:
            json.dump(self.results, f, indent=2)
        self.log(f"Results saved to {filename}")


# Demo factory test
print("\n" + "="*60)
print("FACTORY TEST DEMO")
print("="*60)
factory_test = FactoryTest("/dev/ttyUSB0", station_id="STA01")
results = factory_test.run_all()


"""
============================================================================
            SECTION 5: SUBPROCESS FOR EXTERNAL TOOLS
============================================================================
"""

import subprocess
import shutil

def run_flash_tool(hex_file, port, timeout=60):
    """Flash firmware using external tool (e.g., esptool, stlink)"""

    # Check tool exists
    # tool = shutil.which("esptool.py")
    # if not tool:
    #     raise FileNotFoundError("esptool.py not found")

    # Mock command for demo
    cmd = ["echo", f"Flashing {hex_file} to {port}"]

    # Real command would be:
    # cmd = ["esptool.py", "--port", port, "--baud", "921600",
    #        "write_flash", "0x0", hex_file]

    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=timeout
        )

        if result.returncode == 0:
            print(f"Flash successful: {result.stdout.strip()}")
            return True
        else:
            print(f"Flash failed: {result.stderr}")
            return False

    except subprocess.TimeoutExpired:
        print("Flash timed out")
        return False
    except Exception as e:
        print(f"Flash error: {e}")
        return False


def run_build(project_dir, target="all"):
    """Run make and capture output"""
    cmd = ["echo", f"Building {target} in {project_dir}"]  # Mock
    # Real: cmd = ["make", "-C", project_dir, target]

    result = subprocess.run(cmd, capture_output=True, text=True)

    return {
        'success': result.returncode == 0,
        'stdout': result.stdout,
        'stderr': result.stderr
    }


print("\n=== External Tool Demo ===")
run_flash_tool("firmware.hex", "/dev/ttyUSB0")
build_result = run_build("/path/to/project")
print(f"Build success: {build_result['success']}")


"""
============================================================================
                        INTERVIEW Q&A
============================================================================

Q1: "How would you automate testing for 100 devices?"
A1:
   1. Fixture with multiple serial ports or USB hubs
   2. Scan for devices: list_ports.comports()
   3. Parallel testing with threading/multiprocessing
   4. Each device gets dedicated test thread
   5. Aggregate results to central database/CSV
   6. Generate pass/fail report with device IDs

Q2: "How do you test firmware without modifying it?"
A2:
   - Black box testing via serial interface
   - GPIO observation (logic analyzer, scope)
   - Current measurement for power states
   - RF testing for wireless (spectrum analyzer)
   - Environmental testing (temp chamber)

Q3: "Device fails randomly in production test. Debug approach?"
A3:
   1. Log all test parameters when failure occurs
   2. Check for patterns (time of day, station, operator)
   3. Review test fixture (loose connections, noise)
   4. Add retries to identify flaky vs real failures
   5. Statistical analysis of failure rate per test
   6. Check if specific batch of components affected

Q4: "How would you implement continuous integration for firmware?"
A4:
   1. Git trigger on push
   2. Build firmware in Docker container
   3. Run unit tests (pytest for Python parts)
   4. Flash to test hardware (HIL testing)
   5. Run integration tests
   6. Generate build artifacts
   7. Report results to Slack/email

============================================================================
                    PYTEST COMMAND REFERENCE
============================================================================

pytest                      # Run all tests
pytest -v                   # Verbose output
pytest test_file.py         # Run specific file
pytest -k "test_name"       # Run tests matching name
pytest --tb=short           # Shorter traceback
pytest -x                   # Stop on first failure
pytest --pdb                # Drop to debugger on failure
pytest --cov=module         # Coverage report (needs pytest-cov)
pytest -n 4                 # Parallel (needs pytest-xdist)

============================================================================
"""

if __name__ == '__main__':
    # Run pytest on this file
    print("\nTo run tests: pytest 04_testing_and_tools.py -v")
