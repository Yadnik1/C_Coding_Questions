/*
 * ============================================================================
 * PROBLEM: Protocol Parser State Machine
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: HIGH (Embedded)
 *
 * Parse UART/serial commands using state machine.
 * Format: "$CMD,DATA*CS\n"
 *
 * ============================================================================
 * EXAMPLE: "$TEMP,25.5*A3\n"
 * ============================================================================
 *
 *   CMD = "TEMP"
 *   DATA = "25.5"
 *   CS = "A3" (checksum)
 *
 * ============================================================================
 * STATE DIAGRAM:
 * ============================================================================
 *
 *   WAIT_START ──'$'──► READ_CMD ──','──► READ_DATA ──'*'──► READ_CS ──'\n'──► COMPLETE
 *        │                 │                  │                 │
 *        │                 │                  │                 │
 *        └─────────────────┴──────────────────┴─────────────────┴───► ERROR
 *                              (invalid input)
 *
 * ============================================================================
 * USE CASE: Parsing GPS NMEA sentences, AT commands, sensor protocols
 * ============================================================================
 *
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    PARSE_WAIT_START,
    PARSE_READ_CMD,
    PARSE_READ_DATA,
    PARSE_READ_CS,
    PARSE_COMPLETE,
    PARSE_ERROR
} ParseState;

typedef struct {
    ParseState state;
    char cmd[16];
    char data[64];
    char checksum[4];
    int cmdIdx;
    int dataIdx;
    int csIdx;
} ProtocolParser;

void parser_init(ProtocolParser* p) {
    p->state = PARSE_WAIT_START;
    p->cmdIdx = 0;
    p->dataIdx = 0;
    p->csIdx = 0;
    p->cmd[0] = '\0';
    p->data[0] = '\0';
    p->checksum[0] = '\0';
}

void parser_processByte(ProtocolParser* p, char c) {
    switch (p->state) {
        case PARSE_WAIT_START:
            if (c == '$') {
                p->state = PARSE_READ_CMD;
                p->cmdIdx = 0;
                p->dataIdx = 0;
                p->csIdx = 0;
            }
            break;

        case PARSE_READ_CMD:
            if (c == ',') {
                p->cmd[p->cmdIdx] = '\0';
                p->state = PARSE_READ_DATA;
            } else if (p->cmdIdx < 15) {
                p->cmd[p->cmdIdx++] = c;
            } else {
                p->state = PARSE_ERROR;
            }
            break;

        case PARSE_READ_DATA:
            if (c == '*') {
                p->data[p->dataIdx] = '\0';
                p->state = PARSE_READ_CS;
            } else if (p->dataIdx < 63) {
                p->data[p->dataIdx++] = c;
            } else {
                p->state = PARSE_ERROR;
            }
            break;

        case PARSE_READ_CS:
            if (c == '\n' || c == '\r') {
                p->checksum[p->csIdx] = '\0';
                p->state = PARSE_COMPLETE;
            } else if (p->csIdx < 3) {
                p->checksum[p->csIdx++] = c;
            } else {
                p->state = PARSE_ERROR;
            }
            break;

        case PARSE_COMPLETE:
        case PARSE_ERROR:
            // Terminal states
            break;
    }
}

bool parser_isComplete(ProtocolParser* p) {
    return p->state == PARSE_COMPLETE;
}

bool parser_isError(ProtocolParser* p) {
    return p->state == PARSE_ERROR;
}

const char* stateToString(ParseState state) {
    switch (state) {
        case PARSE_WAIT_START: return "WAIT_START";
        case PARSE_READ_CMD:   return "READ_CMD";
        case PARSE_READ_DATA:  return "READ_DATA";
        case PARSE_READ_CS:    return "READ_CS";
        case PARSE_COMPLETE:   return "COMPLETE";
        case PARSE_ERROR:      return "ERROR";
        default:               return "UNKNOWN";
    }
}

int main() {
    printf("=== Protocol Parser State Machine ===\n\n");

    // Test 1: Valid message
    printf("1. Parsing: \"$TEMP,25.5*A3\\n\"\n");
    ProtocolParser parser;
    parser_init(&parser);

    const char* msg1 = "$TEMP,25.5*A3\n";
    for (int i = 0; msg1[i] != '\0'; i++) {
        parser_processByte(&parser, msg1[i]);
    }

    if (parser_isComplete(&parser)) {
        printf("   Status: SUCCESS\n");
        printf("   CMD:  %s\n", parser.cmd);
        printf("   DATA: %s\n", parser.data);
        printf("   CS:   %s\n\n", parser.checksum);
    }

    // Test 2: Another valid message
    printf("2. Parsing: \"$GPS,37.7749,-122.4194*FF\\n\"\n");
    parser_init(&parser);

    const char* msg2 = "$GPS,37.7749,-122.4194*FF\n";
    for (int i = 0; msg2[i] != '\0'; i++) {
        parser_processByte(&parser, msg2[i]);
    }

    if (parser_isComplete(&parser)) {
        printf("   Status: SUCCESS\n");
        printf("   CMD:  %s\n", parser.cmd);
        printf("   DATA: %s\n", parser.data);
        printf("   CS:   %s\n\n", parser.checksum);
    }

    // Test 3: Invalid (no start)
    printf("3. Parsing: \"INVALID\\n\" (no $ start)\n");
    parser_init(&parser);

    const char* msg3 = "INVALID\n";
    for (int i = 0; msg3[i] != '\0'; i++) {
        parser_processByte(&parser, msg3[i]);
    }

    printf("   Status: %s\n\n", stateToString(parser.state));

    printf("=== Use Cases ===\n");
    printf("- NMEA GPS sentences\n");
    printf("- AT modem commands\n");
    printf("- Custom sensor protocols\n");
    printf("- UART command parsing\n");

    return 0;
}
