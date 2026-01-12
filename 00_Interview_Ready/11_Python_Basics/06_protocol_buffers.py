"""
============================================================================
            PROTOCOL BUFFERS (PROTOBUF) FOR EMBEDDED ENGINEERS
============================================================================

WHAT ARE PROTOCOL BUFFERS?
- Binary serialization format created by Google
- Like JSON but smaller, faster, and strongly typed
- Define data structure once, generate code for any language
- Used for: Device-to-cloud communication, config files, inter-service messaging

WHY USE PROTOBUF IN EMBEDDED/IoT?
- Smaller than JSON (saves bandwidth, crucial for cellular/LoRa)
- Faster to parse (less CPU, important for MCUs)
- Schema enforced (catch errors at compile time)
- Backward compatible (add fields without breaking old devices)

PROTOBUF vs JSON vs CUSTOM BINARY:

| Feature          | JSON        | Protobuf    | Custom Binary |
|------------------|-------------|-------------|---------------|
| Size             | Large       | Small       | Smallest      |
| Human readable   | Yes         | No          | No            |
| Schema           | Optional    | Required    | Manual        |
| Parsing speed    | Slow        | Fast        | Fastest       |
| Language support | Universal   | Many        | Custom        |
| Versioning       | Hard        | Built-in    | Manual        |
| Debug ease       | Easy        | Medium      | Hard          |

WHEN TO USE WHAT:
- JSON: Debug, config files, REST APIs, small payloads
- Protobuf: Production device-cloud, large data, bandwidth-limited
- Custom binary: Extremely constrained MCUs, proprietary protocols

============================================================================
                    HOW PROTOBUF WORKS
============================================================================

1. Define schema in .proto file
2. Compile with `protoc` to generate code
3. Use generated code to serialize/deserialize

   .proto file                    Generated Code
   +--------------+    protoc     +-----------------+
   | message Foo  | ------------> | Python class    |
   |   int32 x    |               | C struct        |
   |   string y   |               | Go struct       |
   +--------------+               +-----------------+
                                         |
                                         v
                                  Binary wire format
                                  (compact, fast)

============================================================================
                    PROTOBUF SCHEMA (.proto FILE)
============================================================================

Here's what a .proto file looks like for an IoT sensor:
"""

PROTO_SCHEMA = '''
// sensor_data.proto
syntax = "proto3";

package iot;

// Sensor reading message
message SensorReading {
    uint32 device_id = 1;       // Field number 1
    uint32 timestamp = 2;       // Unix timestamp
    float temperature = 3;      // Celsius
    float humidity = 4;         // Percentage
    uint32 battery_mv = 5;      // Battery in millivolts

    // Nested message
    GPSLocation location = 6;

    // Enum for status
    DeviceStatus status = 7;

    // Repeated field (like array)
    repeated float adc_readings = 8;
}

message GPSLocation {
    float latitude = 1;
    float longitude = 2;
    float altitude = 3;
}

enum DeviceStatus {
    STATUS_UNKNOWN = 0;
    STATUS_OK = 1;
    STATUS_LOW_BATTERY = 2;
    STATUS_ERROR = 3;
}

// Request/Response for RPC
message ConfigRequest {
    uint32 device_id = 1;
}

message ConfigResponse {
    uint32 sample_rate_ms = 1;
    uint32 upload_interval_s = 2;
    bool wifi_enabled = 3;
    string mqtt_broker = 4;
}
'''

print("=== Protocol Buffer Schema Example ===")
print(PROTO_SCHEMA)

"""
============================================================================
                FIELD NUMBERS AND WIRE TYPES
============================================================================

Field numbers (1, 2, 3...) are crucial:
- Used in binary encoding (not field names!)
- NEVER change or reuse field numbers
- 1-15 use 1 byte (use for frequent fields)
- 16-2047 use 2 bytes

Wire types (how data is encoded):
- 0: Varint (int32, int64, uint32, uint64, bool, enum)
- 1: 64-bit (fixed64, sfixed64, double)
- 2: Length-delimited (string, bytes, nested messages, repeated)
- 5: 32-bit (fixed32, sfixed32, float)

Binary format: [field_number << 3 | wire_type] [data...]

============================================================================
                    PYTHON USAGE (with protobuf library)
============================================================================

Install: pip install protobuf
Compile: protoc --python_out=. sensor_data.proto
"""

# Since we can't actually compile .proto files here,
# let's simulate what the generated code would look like
# and how you'd use it

class MockSensorReading:
    """
    This simulates what protoc generates.
    Real generated code would be in sensor_data_pb2.py
    """
    def __init__(self):
        self.device_id = 0
        self.timestamp = 0
        self.temperature = 0.0
        self.humidity = 0.0
        self.battery_mv = 0
        self.status = 0
        self.adc_readings = []

    def SerializeToString(self):
        """Serialize to binary (simplified simulation)"""
        import struct
        # Real protobuf is more complex, this is just to show concept
        data = struct.pack('<IIffI',
                          self.device_id,
                          self.timestamp,
                          self.temperature,
                          self.humidity,
                          self.battery_mv)
        return data

    def ParseFromString(self, data):
        """Parse from binary (simplified simulation)"""
        import struct
        (self.device_id, self.timestamp, self.temperature,
         self.humidity, self.battery_mv) = struct.unpack('<IIffI', data[:20])

    def __repr__(self):
        return (f"SensorReading(device_id={self.device_id}, "
                f"temp={self.temperature}, humidity={self.humidity})")


# How you'd use real protobuf:
USAGE_EXAMPLE = '''
# Real usage with generated protobuf code:

from sensor_data_pb2 import SensorReading, DeviceStatus

# Create message
reading = SensorReading()
reading.device_id = 12345
reading.timestamp = 1705312800
reading.temperature = 25.5
reading.humidity = 60.0
reading.battery_mv = 3700
reading.status = DeviceStatus.STATUS_OK
reading.adc_readings.extend([1024, 2048, 3072])

# Serialize to bytes (for transmission)
binary_data = reading.SerializeToString()
print(f"Serialized size: {len(binary_data)} bytes")

# Deserialize (on receiving end)
received = SensorReading()
received.ParseFromString(binary_data)
print(f"Temperature: {received.temperature}")

# Convert to dict (for debugging)
from google.protobuf.json_format import MessageToDict
data_dict = MessageToDict(reading)
print(data_dict)
'''

print("\n=== Python Usage Example ===")
print(USAGE_EXAMPLE)

# Demo with our mock class
print("\n=== Mock Demo ===")
reading = MockSensorReading()
reading.device_id = 12345
reading.timestamp = 1705312800
reading.temperature = 25.5
reading.humidity = 60.0
reading.battery_mv = 3700

binary = reading.SerializeToString()
print(f"Serialized: {binary.hex()} ({len(binary)} bytes)")

received = MockSensorReading()
received.ParseFromString(binary)
print(f"Deserialized: {received}")


"""
============================================================================
                SIZE COMPARISON: JSON vs PROTOBUF
============================================================================
"""

import json

# Same data in JSON
json_data = {
    "device_id": 12345,
    "timestamp": 1705312800,
    "temperature": 25.5,
    "humidity": 60.0,
    "battery_mv": 3700
}

json_bytes = json.dumps(json_data).encode()
proto_bytes = reading.SerializeToString()

print("\n=== Size Comparison ===")
print(f"JSON size:     {len(json_bytes)} bytes")
print(f"Protobuf size: {len(proto_bytes)} bytes")
print(f"Savings:       {100 * (1 - len(proto_bytes)/len(json_bytes)):.1f}%")


"""
============================================================================
            PROTOBUF IN EMBEDDED C (Nanopb)
============================================================================

For MCUs, use Nanopb - a lightweight C implementation.
https://github.com/nanopb/nanopb

Nanopb features:
- No dynamic memory allocation (perfect for embedded)
- Small code size (~2-10 KB)
- Works with any C compiler
- Generates .c/.h from .proto files
"""

NANOPB_EXAMPLE = '''
// On ESP32/STM32 with Nanopb:

#include "sensor_data.pb.h"
#include <pb_encode.h>
#include <pb_decode.h>

// Encode sensor data
void send_sensor_reading(float temp, float humidity) {
    uint8_t buffer[128];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    SensorReading reading = SensorReading_init_zero;
    reading.device_id = DEVICE_ID;
    reading.timestamp = get_unix_time();
    reading.temperature = temp;
    reading.humidity = humidity;
    reading.battery_mv = read_battery_mv();
    reading.status = DeviceStatus_STATUS_OK;

    bool success = pb_encode(&stream, SensorReading_fields, &reading);

    if (success) {
        // Send buffer[0..stream.bytes_written] over MQTT/HTTP
        mqtt_publish("sensor/data", buffer, stream.bytes_written);
    }
}

// Decode config from server
void handle_config(uint8_t* data, size_t len) {
    pb_istream_t stream = pb_istream_from_buffer(data, len);

    ConfigResponse config = ConfigResponse_init_zero;
    bool success = pb_decode(&stream, ConfigResponse_fields, &config);

    if (success) {
        set_sample_rate(config.sample_rate_ms);
        set_upload_interval(config.upload_interval_s);
    }
}
'''

print("\n=== Nanopb (C) Example ===")
print(NANOPB_EXAMPLE)


"""
============================================================================
                    PROTOBUF BEST PRACTICES
============================================================================

1. FIELD NUMBERS:
   - Never change existing field numbers
   - Never reuse deleted field numbers
   - Use 1-15 for frequently used fields (1 byte encoding)

2. BACKWARD COMPATIBILITY:
   - Adding new fields: OK (old code ignores them)
   - Removing fields: Mark as reserved, don't delete
   - Changing field type: Generally NOT OK

3. DEFAULT VALUES:
   - Numbers default to 0
   - Strings default to ""
   - Booleans default to false
   - Default values are NOT transmitted (saves space)

4. OPTIONAL vs REQUIRED:
   - proto3: All fields are optional
   - Use wrapper types if you need to distinguish "not set" from "zero"

5. REPEATED FIELDS (arrays):
   - Use for lists of values
   - Packed encoding by default in proto3 (efficient)

============================================================================
                    SCHEMA EVOLUTION EXAMPLE
============================================================================
"""

SCHEMA_EVOLUTION = '''
// Version 1.0 - Original schema
message SensorReading {
    uint32 device_id = 1;
    float temperature = 2;
}

// Version 1.1 - Added humidity (backward compatible!)
message SensorReading {
    uint32 device_id = 1;
    float temperature = 2;
    float humidity = 3;        // NEW - old devices won't send this
}

// Version 1.2 - Added location (still backward compatible)
message SensorReading {
    uint32 device_id = 1;
    float temperature = 2;
    float humidity = 3;
    GPSLocation location = 4;  // NEW - optional nested message
}

// WRONG - Breaking change!
message SensorReading {
    uint32 device_id = 1;
    double temperature = 2;    // WRONG: Changed type from float to double!
}

// Correct way to deprecate a field:
message SensorReading {
    uint32 device_id = 1;
    reserved 2;                // Don't reuse field number 2
    reserved "temperature";    // Document the old name
    float temp_celsius = 3;    // New field with new number
}
'''

print("\n=== Schema Evolution ===")
print(SCHEMA_EVOLUTION)


"""
============================================================================
                PROTOBUF vs OTHER FORMATS
============================================================================

PROTOBUF vs MESSAGEPACK:
- Both are binary, compact
- Protobuf: Schema required, better tooling, more enterprise
- MessagePack: Schema-less (like binary JSON), simpler

PROTOBUF vs FLATBUFFERS:
- Both from Google, schema-based
- Protobuf: Requires parsing, smaller wire size
- FlatBuffers: Zero-copy access, faster reads, larger size

PROTOBUF vs CBOR:
- CBOR: Binary JSON, schema-less, IETF standard
- Often used in constrained IoT (CoAP + CBOR)
- Protobuf better when you control both ends

FOR IRONSITE/IoT:
- Device to cloud: Protobuf (bandwidth savings matter)
- Config files: JSON (human readable)
- Debug/logging: JSON
- Internal MCU comms: Custom binary or Protobuf (Nanopb)

============================================================================
                    PRACTICAL WORKFLOW
============================================================================

1. Define .proto file with your data structures

2. Generate code:
   protoc --python_out=. --c_out=. sensor_data.proto

   For Nanopb:
   nanopb_generator sensor_data.proto

3. Cloud/Server (Python):
   - Use generated sensor_data_pb2.py
   - Parse incoming device data
   - Send config responses

4. Device (C with Nanopb):
   - Use generated sensor_data.pb.h/c
   - Encode sensor readings
   - Decode config from server

5. Testing:
   - Use protoc --decode for debugging
   - Print as JSON for logging: MessageToJson()

============================================================================
                    INTERVIEW Q&A
============================================================================

Q1: "What are Protocol Buffers and why use them?"
A1:
   Binary serialization format by Google.
   Use when: Bandwidth matters (IoT/cellular), need schema enforcement,
   multi-language (device in C, server in Python).
   Benefits: 3-10x smaller than JSON, faster parsing, backward compatible.

Q2: "How does Protobuf achieve smaller size than JSON?"
A2:
   1. Binary encoding (not text)
   2. Field numbers instead of names (1 byte vs "temperature")
   3. Varints for small numbers
   4. No delimiters/quotes/whitespace
   5. Default values not transmitted

Q3: "How do you handle schema changes without breaking old devices?"
A3:
   1. Only ADD new fields (new field numbers)
   2. Never change existing field types
   3. Never reuse field numbers
   4. Mark removed fields as "reserved"
   5. Old devices ignore unknown fields automatically

Q4: "Protobuf vs JSON - when to use each?"
A4:
   Protobuf: Production data, bandwidth-limited, need schema
   JSON: Config files, debugging, REST APIs, human-readable needed
   Often use both: Protobuf for data, JSON for config

Q5: "How would you use Protobuf on an ESP32?"
A5:
   Use Nanopb library:
   1. Write .proto schema
   2. Generate C code with nanopb_generator
   3. Include generated .pb.h and .pb.c
   4. Use pb_encode() to serialize
   5. Use pb_decode() to deserialize
   No dynamic allocation, ~5KB code size

============================================================================
"""

# Simple implementation to understand the concept
print("\n=== Simple Protobuf-like Encoder (Educational) ===")

def encode_varint(value):
    """Encode integer as varint (protobuf style)"""
    result = []
    while value > 127:
        result.append((value & 0x7F) | 0x80)
        value >>= 7
    result.append(value)
    return bytes(result)

def decode_varint(data):
    """Decode varint from bytes"""
    result = 0
    shift = 0
    for byte in data:
        result |= (byte & 0x7F) << shift
        if not (byte & 0x80):
            break
        shift += 7
    return result

# Demo varint encoding
test_values = [1, 127, 128, 300, 16384]
print("Varint encoding (how protobuf stores integers):")
for val in test_values:
    encoded = encode_varint(val)
    print(f"  {val:5d} -> {encoded.hex()} ({len(encoded)} bytes)")


def simple_encode(device_id, temperature):
    """
    Simple protobuf-like encoding.
    Field 1 (device_id): varint
    Field 2 (temperature): fixed32 float
    """
    import struct

    result = bytearray()

    # Field 1: device_id (field_num=1, wire_type=0 for varint)
    # Tag = (field_num << 3) | wire_type = (1 << 3) | 0 = 8
    result.append(0x08)
    result.extend(encode_varint(device_id))

    # Field 2: temperature (field_num=2, wire_type=5 for fixed32)
    # Tag = (2 << 3) | 5 = 21 = 0x15
    result.append(0x15)
    result.extend(struct.pack('<f', temperature))

    return bytes(result)


encoded = simple_encode(12345, 25.5)
print(f"\nSimple encode(12345, 25.5) = {encoded.hex()}")
print(f"Size: {len(encoded)} bytes (vs ~35 bytes for JSON)")


if __name__ == '__main__':
    print("\n" + "="*60)
    print("Protocol Buffers crash course complete!")
    print("Key takeaway: Smaller, faster, schema-enforced serialization")
    print("="*60)
