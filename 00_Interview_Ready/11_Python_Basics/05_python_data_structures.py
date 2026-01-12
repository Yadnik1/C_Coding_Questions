"""
============================================================================
        PYTHON DATA STRUCTURES - INTERVIEW ESSENTIALS
============================================================================

Everything an embedded engineer needs to know about Python data structures.
Focus on what interviewers actually ask.

============================================================================
                    SECTION 1: LISTS (Most Important!)
============================================================================

Lists are Python's arrays - you'll use them constantly.
"""

# Creating lists
empty_list = []
numbers = [1, 2, 3, 4, 5]
mixed = [1, "hello", 3.14, True]  # Can mix types (but don't in practice)
sensor_readings = [23.5, 24.1, 23.8, 24.0, 23.9]

# Accessing elements (0-indexed like C arrays)
print("=== List Basics ===")
print(f"First element: {sensor_readings[0]}")    # 23.5
print(f"Last element: {sensor_readings[-1]}")    # 23.9 (negative indexing!)
print(f"Second last: {sensor_readings[-2]}")     # 24.0

# Slicing [start:end:step] - end is exclusive
print(f"First 3: {sensor_readings[0:3]}")        # [23.5, 24.1, 23.8]
print(f"First 3: {sensor_readings[:3]}")         # Same, omit start
print(f"Last 2: {sensor_readings[-2:]}")         # [24.0, 23.9]
print(f"Every 2nd: {sensor_readings[::2]}")      # [23.5, 23.8, 23.9]
print(f"Reversed: {sensor_readings[::-1]}")      # Reverse the list

# Modifying lists
sensor_readings[0] = 23.6                        # Change element
sensor_readings.append(24.2)                     # Add to end
sensor_readings.insert(0, 23.0)                  # Insert at position
sensor_readings.extend([24.3, 24.4])             # Add multiple
popped = sensor_readings.pop()                   # Remove and return last
sensor_readings.remove(23.0)                     # Remove first occurrence
print(f"After modifications: {sensor_readings}")

# List operations
nums = [3, 1, 4, 1, 5, 9, 2, 6]
print(f"\nLength: {len(nums)}")
print(f"Sum: {sum(nums)}")
print(f"Min: {min(nums)}, Max: {max(nums)}")
print(f"Count of 1s: {nums.count(1)}")
print(f"Index of 5: {nums.index(5)}")
print(f"Is 4 in list: {4 in nums}")

# Sorting
nums_sorted = sorted(nums)                       # Returns new list
print(f"Sorted (new): {nums_sorted}")
nums.sort()                                      # Sorts in place
print(f"Sorted (in place): {nums}")
nums.sort(reverse=True)                          # Descending
print(f"Sorted descending: {nums}")

# List comprehension (VERY IMPORTANT - interviewers love this)
print("\n=== List Comprehension ===")
squares = [x**2 for x in range(10)]
print(f"Squares: {squares}")

evens = [x for x in range(20) if x % 2 == 0]
print(f"Evens: {evens}")

# Convert ADC readings to voltage
adc_values = [1024, 2048, 3072, 4095]
voltages = [adc * 3.3 / 4095 for adc in adc_values]
print(f"Voltages: {[f'{v:.2f}V' for v in voltages]}")

# Filter readings above threshold
readings = [23.5, 26.0, 24.1, 28.5, 23.8, 30.0]
high_temps = [r for r in readings if r > 25]
print(f"High temps (>25): {high_temps}")

# Nested list comprehension
matrix = [[i*3 + j for j in range(3)] for i in range(3)]
print(f"Matrix: {matrix}")


"""
============================================================================
                    SECTION 2: TUPLES (Immutable Lists)
============================================================================
"""

print("\n=== Tuples ===")

# Tuples are immutable (can't change after creation)
point = (10, 20)
rgb = (255, 128, 64)
device_info = ("ESP32", "1.2.3", 115200)

# Accessing (same as lists)
print(f"x={point[0]}, y={point[1]}")

# Unpacking (very useful!)
x, y = point
print(f"Unpacked: x={x}, y={y}")

name, version, baud = device_info
print(f"Device: {name} v{version}")

# Tuple in function returns
def get_min_max(data):
    return min(data), max(data)  # Returns tuple

minimum, maximum = get_min_max([3, 1, 4, 1, 5, 9])
print(f"Min={minimum}, Max={maximum}")

# Swap using tuples
a, b = 5, 10
a, b = b, a  # Swap!
print(f"After swap: a={a}, b={b}")

# Named tuples (like C structs)
from collections import namedtuple

SensorReading = namedtuple('SensorReading', ['timestamp', 'temp', 'humidity'])
reading = SensorReading(timestamp=1000, temp=25.5, humidity=60)
print(f"Named tuple: {reading.temp}°C at time {reading.timestamp}")


"""
============================================================================
                    SECTION 3: DICTIONARIES (Key-Value)
============================================================================
"""

print("\n=== Dictionaries ===")

# Creating dictionaries
empty_dict = {}
device_config = {
    "port": "/dev/ttyUSB0",
    "baud": 115200,
    "timeout": 5.0,
    "parity": "none"
}

# Accessing
print(f"Port: {device_config['port']}")
print(f"Baud: {device_config.get('baud')}")
print(f"Missing key: {device_config.get('missing', 'default')}")  # With default

# Modifying
device_config["baud"] = 9600                     # Update
device_config["flow_control"] = "none"           # Add new
del device_config["parity"]                      # Delete

# Check if key exists
if "port" in device_config:
    print("Port is configured")

# Iteration
print("\nIteration:")
for key in device_config:
    print(f"  {key}: {device_config[key]}")

for key, value in device_config.items():
    print(f"  {key} = {value}")

# Dict comprehension
adc_channels = {f"CH{i}": i * 100 for i in range(4)}
print(f"ADC channels: {adc_channels}")

# Nested dictionaries (common in config files)
system_config = {
    "wifi": {
        "ssid": "MyNetwork",
        "password": "secret123"
    },
    "mqtt": {
        "broker": "mqtt.example.com",
        "port": 8883
    }
}
print(f"WiFi SSID: {system_config['wifi']['ssid']}")

# Default dict (auto-creates missing keys)
from collections import defaultdict

error_counts = defaultdict(int)  # Default value is 0
error_counts["timeout"] += 1
error_counts["timeout"] += 1
error_counts["checksum"] += 1
print(f"Error counts: {dict(error_counts)}")

# Counter (count occurrences)
from collections import Counter

error_log = ["timeout", "timeout", "checksum", "timeout", "crc_error"]
counts = Counter(error_log)
print(f"Error counter: {counts}")
print(f"Most common: {counts.most_common(2)}")


"""
============================================================================
                    SECTION 4: SETS (Unique Elements)
============================================================================
"""

print("\n=== Sets ===")

# Sets contain unique elements only
device_ids = {1, 2, 3, 4, 5}
more_ids = {4, 5, 6, 7, 8}

# Set operations
print(f"Union: {device_ids | more_ids}")           # All unique
print(f"Intersection: {device_ids & more_ids}")    # Common
print(f"Difference: {device_ids - more_ids}")      # In first, not second
print(f"Symmetric diff: {device_ids ^ more_ids}")  # In one but not both

# Remove duplicates from list
readings_with_dups = [1, 2, 2, 3, 3, 3, 4]
unique_readings = list(set(readings_with_dups))
print(f"Unique: {unique_readings}")

# Check membership (O(1) - fast!)
allowed_commands = {"read", "write", "status", "reset"}
cmd = "read"
if cmd in allowed_commands:
    print(f"Command '{cmd}' is allowed")


"""
============================================================================
            SECTION 5: STRINGS (Text Processing)
============================================================================
"""

print("\n=== Strings ===")

# Strings are immutable sequences
message = "Hello, Device!"
print(f"Length: {len(message)}")
print(f"Upper: {message.upper()}")
print(f"Lower: {message.lower()}")

# Slicing (same as lists)
print(f"First 5: {message[:5]}")
print(f"Last 7: {message[-7:]}")

# Searching
print(f"Find 'Device': {message.find('Device')}")  # Returns index or -1
print(f"Contains 'Device': {'Device' in message}")

# Splitting and joining
log_line = "2024-01-15,25.5,60,OK"
parts = log_line.split(',')
print(f"Split: {parts}")

data = ["25.5", "60", "OK"]
joined = ','.join(data)
print(f"Joined: {joined}")

# Strip whitespace
dirty = "  some data  \n"
clean = dirty.strip()
print(f"Stripped: '{clean}'")

# Replace
cmd = "AT+VERSION\r\n"
clean_cmd = cmd.replace('\r\n', '')
print(f"Cleaned: '{clean_cmd}'")

# Format strings (f-strings - use these!)
temp = 25.5
humidity = 60
print(f"Temp: {temp:.1f}°C, Humidity: {humidity}%")

# Hex formatting
value = 255
print(f"Decimal: {value}, Hex: {value:#04x}, Binary: {value:#010b}")

# Pad strings
device_id = "123"
padded = device_id.zfill(6)  # Pad with zeros
print(f"Padded ID: {padded}")


"""
============================================================================
                SECTION 6: COMMON INTERVIEW PATTERNS
============================================================================
"""

print("\n" + "="*60)
print("COMMON INTERVIEW PATTERNS")
print("="*60)

# Pattern 1: Two pointers (find pair with sum)
def find_pair_with_sum(arr, target):
    """Find two numbers that add up to target"""
    seen = set()
    for num in arr:
        complement = target - num
        if complement in seen:
            return (complement, num)
        seen.add(num)
    return None

nums = [2, 7, 11, 15]
print(f"Pattern 1 - Pair with sum 9: {find_pair_with_sum(nums, 9)}")


# Pattern 2: Frequency count
def most_frequent(arr):
    """Find most frequent element"""
    counts = {}
    for x in arr:
        counts[x] = counts.get(x, 0) + 1
    return max(counts, key=counts.get)

errors = ["timeout", "timeout", "crc", "timeout", "crc"]
print(f"Pattern 2 - Most frequent: {most_frequent(errors)}")


# Pattern 3: Sliding window (max sum of k elements)
def max_sum_subarray(arr, k):
    """Find max sum of k consecutive elements"""
    if len(arr) < k:
        return None

    window_sum = sum(arr[:k])
    max_sum = window_sum

    for i in range(k, len(arr)):
        window_sum += arr[i] - arr[i-k]
        max_sum = max(max_sum, window_sum)

    return max_sum

readings = [1, 4, 2, 10, 2, 3, 1, 0, 20]
print(f"Pattern 3 - Max sum of 3: {max_sum_subarray(readings, 3)}")


# Pattern 4: Remove duplicates preserving order
def remove_duplicates_ordered(arr):
    """Remove duplicates while preserving order"""
    seen = set()
    result = []
    for x in arr:
        if x not in seen:
            seen.add(x)
            result.append(x)
    return result

items = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3]
print(f"Pattern 4 - Remove dups: {remove_duplicates_ordered(items)}")


# Pattern 5: Merge sorted lists
def merge_sorted(list1, list2):
    """Merge two sorted lists"""
    result = []
    i = j = 0

    while i < len(list1) and j < len(list2):
        if list1[i] <= list2[j]:
            result.append(list1[i])
            i += 1
        else:
            result.append(list2[j])
            j += 1

    result.extend(list1[i:])
    result.extend(list2[j:])
    return result

a = [1, 3, 5, 7]
b = [2, 4, 6, 8]
print(f"Pattern 5 - Merged: {merge_sorted(a, b)}")


# Pattern 6: Group by key
def group_by_status(items):
    """Group items by status"""
    groups = defaultdict(list)
    for item in items:
        groups[item['status']].append(item['id'])
    return dict(groups)

devices = [
    {"id": "D1", "status": "online"},
    {"id": "D2", "status": "offline"},
    {"id": "D3", "status": "online"},
    {"id": "D4", "status": "error"}
]
print(f"Pattern 6 - Grouped: {group_by_status(devices)}")


# Pattern 7: Flatten nested list
def flatten(nested):
    """Flatten nested list"""
    result = []
    for item in nested:
        if isinstance(item, list):
            result.extend(flatten(item))
        else:
            result.append(item)
    return result

nested = [1, [2, 3], [4, [5, 6]], 7]
print(f"Pattern 7 - Flattened: {flatten(nested)}")


# Pattern 8: Rotate array
def rotate_left(arr, k):
    """Rotate array left by k positions"""
    k = k % len(arr)
    return arr[k:] + arr[:k]

def rotate_right(arr, k):
    """Rotate array right by k positions"""
    k = k % len(arr)
    return arr[-k:] + arr[:-k]

original = [1, 2, 3, 4, 5]
print(f"Pattern 8 - Rotate left 2: {rotate_left(original, 2)}")
print(f"Pattern 8 - Rotate right 2: {rotate_right(original, 2)}")


"""
============================================================================
                    SECTION 7: EMBEDDED-SPECIFIC EXAMPLES
============================================================================
"""

print("\n=== Embedded-Specific Examples ===")

# Example 1: Parse sensor log into structured data
def parse_sensor_log(log_lines):
    """Convert log lines to list of dicts"""
    readings = []
    for line in log_lines:
        if "temp=" in line:
            parts = line.split()
            timestamp = parts[0]
            temp = float(parts[1].split('=')[1].rstrip('C'))
            readings.append({"time": timestamp, "temp": temp})
    return readings

log = ["08:00 temp=25.5C", "08:01 temp=25.8C", "08:02 temp=26.0C"]
print(f"Parsed log: {parse_sensor_log(log)}")


# Example 2: Ring buffer using list
class RingBuffer:
    def __init__(self, size):
        self.buffer = [None] * size
        self.size = size
        self.index = 0
        self.count = 0

    def push(self, value):
        self.buffer[self.index] = value
        self.index = (self.index + 1) % self.size
        self.count = min(self.count + 1, self.size)

    def get_all(self):
        if self.count < self.size:
            return self.buffer[:self.count]
        # Return in correct order
        return self.buffer[self.index:] + self.buffer[:self.index]

rb = RingBuffer(5)
for i in range(7):
    rb.push(i)
print(f"Ring buffer: {rb.get_all()}")


# Example 3: Device registry
class DeviceRegistry:
    def __init__(self):
        self.devices = {}

    def register(self, device_id, port, status="offline"):
        self.devices[device_id] = {"port": port, "status": status}

    def update_status(self, device_id, status):
        if device_id in self.devices:
            self.devices[device_id]["status"] = status

    def get_online(self):
        return [did for did, info in self.devices.items()
                if info["status"] == "online"]

registry = DeviceRegistry()
registry.register("DEV01", "/dev/ttyUSB0")
registry.register("DEV02", "/dev/ttyUSB1")
registry.update_status("DEV01", "online")
print(f"Online devices: {registry.get_online()}")


"""
============================================================================
                        INTERVIEW Q&A
============================================================================

Q1: "Difference between list and tuple?"
A1:
   List: Mutable (can change), use [] syntax
   Tuple: Immutable (can't change), use () syntax
   Use tuple for: Fixed data, dict keys, function returns
   Use list for: Data that changes, collections

Q2: "Difference between list and dict?"
A2:
   List: Ordered by index, O(n) search
   Dict: Key-value pairs, O(1) search by key
   Use list when: Order matters, numeric indexing
   Use dict when: Need fast lookup by key

Q3: "How to remove duplicates from a list?"
A3:
   Method 1: list(set(my_list))  # Doesn't preserve order
   Method 2: list(dict.fromkeys(my_list))  # Preserves order (Python 3.7+)
   Method 3: Use loop with seen set (for complex objects)

Q4: "Time complexity of list operations?"
A4:
   append(): O(1)
   insert(0): O(n)
   pop(): O(1)
   pop(0): O(n)
   index(): O(n)
   in (membership): O(n)
   sort(): O(n log n)

Q5: "Time complexity of dict operations?"
A5:
   get/set: O(1) average
   delete: O(1) average
   in (membership): O(1) average
   iteration: O(n)

Q6: "How does Python list differ from C array?"
A6:
   Python list: Dynamic size, can hold mixed types, bounds checked
   C array: Fixed size, single type, no bounds checking
   Python list is more like C's dynamic array or vector

Q7: "What's a defaultdict?"
A7:
   Dict that auto-creates missing keys with default value
   defaultdict(int) -> default 0
   defaultdict(list) -> default []
   Useful for counting, grouping

============================================================================
                    QUICK REFERENCE
============================================================================

# List
my_list.append(x)       # Add to end
my_list.insert(i, x)    # Insert at index
my_list.pop()           # Remove last
my_list.remove(x)       # Remove first x
my_list.sort()          # Sort in place
sorted(my_list)         # Return sorted copy
len(my_list)            # Length
x in my_list            # Check membership

# Dict
my_dict[key] = value    # Set
my_dict.get(key, default)  # Get with default
del my_dict[key]        # Delete
key in my_dict          # Check key exists
my_dict.keys()          # All keys
my_dict.values()        # All values
my_dict.items()         # Key-value pairs

# Set
my_set.add(x)           # Add element
my_set.remove(x)        # Remove (error if missing)
my_set.discard(x)       # Remove (no error)
set1 | set2             # Union
set1 & set2             # Intersection
set1 - set2             # Difference

# String
s.split(',')            # Split by delimiter
','.join(list)          # Join with delimiter
s.strip()               # Remove whitespace
s.replace(old, new)     # Replace
s.startswith('x')       # Check prefix
s.endswith('x')         # Check suffix
f"{var:.2f}"            # Format string

============================================================================
"""

if __name__ == '__main__':
    print("\nPython data structures practice complete!")
