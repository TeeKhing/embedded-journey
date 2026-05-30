#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LED_OFF,
    LED_ON,
    LED_BLINKING
} LedState;

typedef enum {
    CMD_TURN_ON,
    CMD_TURN_OFF,
    CMD_BLINK,
    CMD_INVALID
} LedCommand;

const char *state_to_string(LedState state) {
    switch (state) {
        case LED_OFF:       return "OFF";
        case LED_ON:        return "ON";
        case LED_BLINKING:  return "BLINKING";
        default:            return "UNKNOWN";
    }
}

LedState process_command(LedState current, LedCommand cmd) {
    switch (current) {
        case LED_OFF:
            if (cmd == CMD_TURN_ON) return LED_ON;
            if (cmd == CMD_BLINK)   return LED_BLINKING;
            break;
        
        case LED_ON:
        if (cmd == CMD_TURN_OFF)    return LED_OFF;
        if (cmd == CMD_BLINK)       return LED_BLINKING;
            break;

        case LED_BLINKING:
        if (cmd == CMD_TURN_ON)     return LED_ON;
        if (cmd == CMD_TURN_OFF)    return LED_OFF;
        break;
    }
    return current;
}

int main(void) {
    LedState state = LED_OFF;

    printf("Initial state: %s\n\n", state_to_string(state));

    LedCommand commands[] = {
        CMD_TURN_ON,
        CMD_BLINK,
        CMD_TURN_OFF,
        CMD_TURN_ON,
    };

    const char *cmd_names[] = {
        "CMD_TURN_ON",
        "CMD_TURN_OFF",
        "CMD_BLINK",
        "CMD_INVALID"
    };

    for (uint8_t i = 0; i < 4; i++) {
        LedState previous = state;
        state = process_command(state, commands[i]);
        printf("Command: %-15s | %s -> %s\n",
                cmd_names[commands[i]],
                state_to_string(previous),
                state_to_string(state));
    }

    return 0;
}