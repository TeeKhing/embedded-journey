#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Simulated ATmega328P GPIO registers for PORTB
 * 
 * DDRB     -   Data Direction Register B
 *              Bit = 0 means INPUT
 *              Bit = 1 means OUTPUT
 * 
 * PORTB    -   Port B Data Register
 *              For OUTPUT pins: bit = 0 means LOW (0V)
 *                               bit = 1 means HIGH (5V)
 *              For INPUT pins:  bit = 1 enables internal pull-up
 * 
 * PINB     -   Port B Input Pins Register (read-only on real hardware)
 *              Reflects actual voltage on each pin
*/

static uint8_t DDRB  = 0x00;     // all inputs by default
static uint8_t PORTB = 0x00;     // all low
static uint8_t PINB  = 0x00;     // simulated input state

void gpio_set_output(uint8_t pin) {
    DDRB |= (1 << pin);
}

void gpio_set_input(uint8_t pin) {
    DDRB &= ~(1 << pin);
}

void gpio_write_high(uint8_t pin) {
    PORTB |= (1 << pin);
}

void gpio_write_low(uint8_t pin) {
    PORTB &= ~(1 << pin);
}

void gpio_toggle(uint8_t pin) {
    PORTB ^= (1 << pin);
}

bool gpio_read(uint8_t pin) {
    return (PINB & (1 << pin)) != 0;
}

void simulate_input_high(uint8_t pin) {
    PINB |= (1 << pin);
}

void simulate_input_low(uint8_t pin) {
    PINB &= ~(1 << pin);
}

void print_register_state(void) {
    printf("    DDRB    =   ");
    for (int i = 7; i >= 0; i--) printf("%d", (DDRB >> i) & 1);
    printf("    (1=output, 0=input)\n");

    printf("    PORTB   =   ");
    for (int i = 7; i >= 0; i--) printf("%d", (PORTB >> i) & 1);
    printf("    (output state)\n");

    printf("    PINB    =   ");
    for (int i = 7; i >= 0; i--) printf("%d", (PINB >> i) & 1);
    printf("    (actual pin voltage)\n");
}

int main(void) {
    printf("=== Initial State ===\n");
    print_register_state();

    printf("\n=== Configure: pin 5 = output (LED), pin 2 = input (button) ===\n");
    gpio_set_output(5);
    gpio_set_input(2);
    print_register_state;

    printf("\n=== Turn LED ON (set pin 5 HIGH) ===\n");
    gpio_write_high(5);
    print_register_state();

    printf("\n=== Turn LED OFF (set pin 5 LOW) ===\n");
    gpio_write_low(5);
    print_register_state();

    printf("\n=== Toggle LED three times ===\n");
    for (int i = 0; i < 3; i++) {
        gpio_toggle(5);
        printf("Toggle %d: pin 5 is %s\n", i + 1,
            (PORTB & (1 << 5)) ? "HIGH" : "LOW");
    }

    printf("\n=== Read button state (simulating physical button presses) ===\n");
    simulate_input_high(2);
    printf("Button pressed:     pin 2 reads %s\n", 
        gpio_read(2) ? "HIGH" : "LOW");

    simulate_input_low(2);
    printf("Button released:    pin 2 reads %s\n", 
        gpio_read(2) ? "HIGH" : "LOW");

    print_register_state();

    return 0;
}
