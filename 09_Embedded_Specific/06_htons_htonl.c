/*
 * ============================================================================
 * PROBLEM: Implement htons, htonl, ntohs, ntohl
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: HIGH (Embedded/Networking)
 *
 * Network byte order functions for converting between host and network order.
 *
 * ============================================================================
 * FUNCTIONS:
 * ============================================================================
 *
 * htons = Host TO Network Short (16-bit)
 * htonl = Host TO Network Long (32-bit)
 * ntohs = Network TO Host Short (16-bit)
 * ntohl = Network TO Host Long (32-bit)
 *
 * Network byte order = BIG-ENDIAN (always)
 * Host byte order = depends on processor (usually little-endian)
 *
 * ============================================================================
 * WHEN TO USE:
 * ============================================================================
 *
 * SENDING over network:
 *   port = htons(8080);      // Convert before send
 *   ip = htonl(ip_address);
 *
 * RECEIVING from network:
 *   port = ntohs(net_port);  // Convert after receive
 *   ip = ntohl(net_ip);
 *
 * ============================================================================
 * TIME: O(1) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why is htons and ntohs the same function?"
 * A1: Byte swapping is symmetric! Swapping twice returns original.
 *     If host is little-endian, both swap. If big-endian, both are no-op.
 *     Same code works both directions.
 *
 * -------------------------------------------------------------------------
 * Q2: "What happens on a big-endian system?"
 * A2: Network order IS big-endian, so hton*/ntoh* are no-ops (just return value).
 *     The isLittleEndian() check handles this - no swap needed on BE systems.
 *
 * -------------------------------------------------------------------------
 * Q3: "Why not just always swap?"
 * A3: That would break big-endian systems! On BE host, data is already in
 *     network order. Swapping would convert TO little-endian, which is wrong.
 *     Always check host endianness first.
 *
 * -------------------------------------------------------------------------
 * Q4: "What's the 64-bit version called?"
 * A4: No standard name. Some use htonll/ntohll. POSIX doesn't define them.
 *     Implement yourself or use htobe64/be64toh from <endian.h> (Linux).
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Check endianness
bool isLittleEndian(void) {
    uint16_t test = 0x0001;
    return *((uint8_t*)&test) == 0x01;
}

// Swap functions
uint16_t swap16(uint16_t value) {
    return (value >> 8) | (value << 8);
}

uint32_t swap32(uint32_t value) {
    return ((value >> 24) & 0x000000FF) |
           ((value >> 8)  & 0x0000FF00) |
           ((value << 8)  & 0x00FF0000) |
           ((value << 24) & 0xFF000000);
}

// Host to Network
uint16_t my_htons(uint16_t hostValue) {
    if (isLittleEndian()) {
        return swap16(hostValue);
    }
    return hostValue;
}

uint32_t my_htonl(uint32_t hostValue) {
    if (isLittleEndian()) {
        return swap32(hostValue);
    }
    return hostValue;
}

// Network to Host (same operation!)
uint16_t my_ntohs(uint16_t netValue) {
    return my_htons(netValue);
}

uint32_t my_ntohl(uint32_t netValue) {
    return my_htonl(netValue);
}

void printBytes(const char* label, void* ptr, size_t size) {
    uint8_t* bytes = (uint8_t*)ptr;
    printf("%s: [", label);
    for (size_t i = 0; i < size; i++) {
        printf("0x%02X", bytes[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    printf("=== htons / htonl / ntohs / ntohl ===\n\n");

    printf("System: %s\n\n", isLittleEndian() ? "Little-Endian" : "Big-Endian");

    // 16-bit example (port number)
    uint16_t hostPort = 8080;
    uint16_t netPort = my_htons(hostPort);

    printf("16-bit (Port):\n");
    printf("  Host value:    %d (0x%04X)\n", hostPort, hostPort);
    printf("  Network value: 0x%04X\n", netPort);
    printBytes("  Host bytes   ", &hostPort, 2);
    printBytes("  Network bytes", &netPort, 2);
    printf("\n");

    // 32-bit example (IP address)
    uint32_t hostIP = 0xC0A80001;  // 192.168.0.1
    uint32_t netIP = my_htonl(hostIP);

    printf("32-bit (IP Address 192.168.0.1):\n");
    printf("  Host value:    0x%08X\n", hostIP);
    printf("  Network value: 0x%08X\n", netIP);
    printBytes("  Host bytes   ", &hostIP, 4);
    printBytes("  Network bytes", &netIP, 4);
    printf("\n");

    // Round trip
    printf("Round trip test:\n");
    printf("  Original: %d\n", hostPort);
    printf("  htons:    0x%04X\n", my_htons(hostPort));
    printf("  ntohs:    %d\n", my_ntohs(my_htons(hostPort)));

    return 0;
}
