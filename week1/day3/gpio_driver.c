#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * GPIO Driver - Simulated ATmega328p
 * Demonstrates: #define, enums, fixed-width types,
 *               bitfield register model, state machine  
 */

#define MAX_PINS    8
#define LED_PIN     5
#define BUTTON_PIN  2

typedef enum {
    GPIO_INPUT  = 0,
    GPIO_OUTPUT = 1
} GpioDirection;

typedef enum {
    GPIO_LOW    = 0,
    GPIO_HIGH   = 1
} GpioState;

typedef enum {
    GPIO_NO_PULL    = 0,
    GPIO_PULL_UP    = 1,
} GpioPull;

typedef enum {
    GPIO_OK                 = 0,
    GPIO_ERR_INVALID_PIN    = 1,
    GPIO_ERR_NOT_OUTPUT     = 2,
    GPIO_ERR_NOT_INPUT      = 3
} GpioError;

typedef union {
    struct {
        uint8_t pin0 : 1;
        uint8_t pin1 : 1;
        uint8_t pin2 : 1;
        uint8_t pin3 : 1;
        uint8_t pin4 : 1;
        uint8_t pin5 : 1;
        uint8_t pin6 : 1;
        uint8_t pin7 : 1;
    } bits;
    uint8_t byte;
} PortReg;

static PortReg DDRB     = {.byte = 0x00};
static PortReg PORTB    = {.byte = 0x00};
static PortReg PINB     = {.byte = 0x00};

static bool pin_valid(uint8_t pin) {
    return pin < MAX_PINS;
}

GpioError gpio_set_direction(uint8_t pin, GpioDirection dir) {
    if (!pin_valid(pin)) return GPIO_ERR_INVALID_PIN;
    if (dir == GPIO_OUTPUT) {
        DDRB.byte |= (1 << pin);
    } else {
        DDRB.byte &= ~(1 << pin);
    }
    return GPIO_OK;
}

GpioError gpio_set_pull(uint8_t pin, GpioPull pull) {
    if (!pin_valid(pin)) return GPIO_ERR_INVALID_PIN;
    if (pull == GPIO_PULL_UP) {
        PORTB.byte |= (1 << pin);
    } else {
        PORTB.byte &= ~(1 << pin);
    }
    return GPIO_OK;
}

GpioError gpio_write(uint8_t pin, GpioState state) {
    if (!pin_valid(pin))            return GPIO_ERR_INVALID_PIN;
    if (!(DDRB.byte & (1 << pin)))  return GPIO_ERR_NOT_OUTPUT;
    if (state == GPIO_HIGH) {
        PORTB.byte |= (1 << pin);
    } else {
        PORTB.byte &= ~(1 << pin);
    }
    return GPIO_OK;
}

GpioError gpio_toggle(uint8_t pin) {
    if (!pin_valid(pin))            return GPIO_ERR_INVALID_PIN;
    if (!(DDRB.byte & (1<< pin)))   return GPIO_ERR_NOT_OUTPUT;
    PORTB.byte ^= (1 << pin);
    return GPIO_OK;
}

GpioError gpio_read(uint8_t pin, GpioState *state) {
    if (!pin_valid(pin))            return GPIO_ERR_INVALID_PIN;
    if (DDRB.byte & (1 << pin))     return GPIO_ERR_NOT_INPUT;
    *state = (PINB.byte & (1 << pin)) ? GPIO_HIGH : GPIO_LOW;
    return GPIO_OK;
}

void simulate_pin_input(uint8_t pin, GpioState state) {
    if (state == GPIO_HIGH) {
        PINB.byte |= (1 << pin);
    } else {
        PINB.byte &= ~(1 << pin);
    }
}

const char *error_to_string(GpioError err) {
    switch (err) {
        case GPIO_OK:               return "OK";
        case GPIO_ERR_INVALID_PIN:  return "INVALID PIN";
        case GPIO_ERR_NOT_OUTPUT:   return "NOT AN OUTPUT";
        case GPIO_ERR_NOT_INPUT:    return "NOT AN INPUT";
        default:                    return "UNKNOWN ERROR";
    }
}

int main(void) {
    GpioError err;

    printf("=== GPIO Driver Test ===\n\n");

    err = gpio_set_direction(LED_PIN, GPIO_OUTPUT);
    printf("Set pin %u as OUTPUT:   %s\n", LED_PIN, error_to_string(err));

    err = gpio_set_direction(BUTTON_PIN, GPIO_INPUT);
    printf("Set pin %u as INPUT:    %s\n", BUTTON_PIN, error_to_string(err));

    err = gpio_set_pull(BUTTON_PIN, GPIO_PULL_UP);
    printf("Enable pull-up pin %u:  %s\n\n", BUTTON_PIN, error_to_string(err));
    
    printf("=== LED Control ===\n");
    err = gpio_write(LED_PIN, GPIO_HIGH);
    printf("Write HIGH to LED:  %s | PORTB=0x%02X\n",
        error_to_string(err), PORTB.byte);

    err = gpio_write(LED_PIN, GPIO_LOW);
    printf("Write LOW to LED:   %s | PORTB=0x%02X\n",
        error_to_string(err), PORTB.byte);

    printf("\n=== Error Handling ===\n");
    err = gpio_write(9, GPIO_HIGH);
    printf("Write to pin 9 (invalid):   %s\n", error_to_string(err));

    err = gpio_write(BUTTON_PIN, GPIO_HIGH);
    printf("Write to input pin %u:      %s\n",
        BUTTON_PIN, error_to_string(err));

    printf("\n=== Button Read ===\n");
    GpioState btn_state;

    simulate_pin_input(BUTTON_PIN, GPIO_LOW);
    gpio_read(BUTTON_PIN, &btn_state);
    printf("Button (LOW/pressed):   %s\n",
        btn_state == GPIO_LOW ? "PRESSED" : "RELEASED");

    simulate_pin_input(BUTTON_PIN, GPIO_HIGH);
    gpio_read(BUTTON_PIN, &btn_state);
    printf("Button (HIGH/released): %s\n",
        btn_state == GPIO_LOW ? "PRESSED" : "RELEASED");

    return 0;
}