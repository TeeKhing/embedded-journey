#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Simulated ATmega328P GPIO Port B registers
 * Real addresses: DDRB=0x24, PORTB=0x25, PINB=0x23
 * Here modelled as unions for simulation purposes
 */

typedef union {
    struct {
        uint8_t pin0 : 1;
        uint8_t pin1 : 1;
        uint8_t pin2 : 1;
        uint8_t pin3 : 1;
        uint8_t pin4 : 1;
        uint8_t pin5 : 1;   // Arduino pin 13
        uint8_t pin6 : 1;
        uint8_t pin7 : 1;
    } bits;
    uint8_t byte;
} GpioReg;

static GpioReg DDRB     = {.byte = 0x00};
static GpioReg PORTB    = {.byte = 0x00};
static GpioReg PINB     = {.byte = 0x00};

#define LED_PIN     5
#define BTN_PIN     2

void gpio_init(void) {
    DDRB.bits.pin5  = 1;    // LED pin as output
    DDRB.bits.pin2  = 0;    // Button pin as input
    PORTB.bits.pin2 = 1;    // Enable pull-up on button pin
}

void led_on(void)       { PORTB.bits.pin5 = 1; }
void led_off(void)      { PORTB.bits.pin5 = 0; }
void led_toggle(void)   { PORTB.bits.pin5 ^= 1; }

bool button_is_pressed(void) {
    return !PINB.bits.pin2;     // active low-pressed = LOW = 0
}

void simulate_button_press(void)    { PINB.bits.pin2 = 0; }
void simulate_button_release(void)  { PINB.bits.pin2 = 1; }

void print_register_state(void) {
    printf("DDRB    byte=0x%02X | pin5(LED dir)=%u  pin2(BTN dir)=%u\n", 
        DDRB.byte,
        DDRB.bits.pin5,
        DDRB.bits.pin2);

    printf("PORTB   byte=0x%02X | pin5(LED out)=%u  pin2(BTN pullup)=%u\n",
        PORTB.byte,
        PORTB.bits.pin5,
        PORTB.bits.pin2);

    printf("PINB    byte=0x%02X | pin5(LED in)=%u   pin2(BTN in)=%u\n\n",
        PINB.byte,
        PINB.bits.pin5,
        PINB.bits.pin2);
}

int main(void) {
    printf("=== Initial State ===\n");
    print_register_state();

    printf("=== After gpio_init() ===\n");
    gpio_init();
    print_register_state();

    printf("=== LED Operations ===\n");
    led_on();
    printf("led_on():       PORTB=0x%02X (pin5=%u)\n",
            PORTB.byte, PORTB.bits.pin5);

    led_off();
    printf("led_off():      PORTB=0x%02X (pin5=%u)\n",
            PORTB.byte, PORTB.bits.pin5);

    led_toggle();
    printf("led_toggle():   PORTB=0x%02X (pin5=%u)\n",
            PORTB.byte, PORTB.bits.pin5);

    led_toggle();
    printf("led_toggle():   PORTB=0x%02X (pin5=%u)\n\n",
            PORTB.byte, PORTB.bits.pin5);

    printf("=== Button Simulation ===\n");
    simulate_button_release();
    printf("Button released:    pressed=%u\n", button_is_pressed());

    simulate_button_press();
    printf("Button pressed:     pressed=%u\n", button_is_pressed());

    return 0;
}