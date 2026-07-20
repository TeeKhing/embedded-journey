#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

#define LED_PIN     5
#define BUTTON_PIN  2

#define SET_BIT(reg, bit)    ((reg) |=  (1 << (bit)))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~(1 << (bit)))
#define TOGGLE_BIT(reg, bit) ((reg) ^=  (1 << (bit)))
#define READ_BIT(reg, bit)   (((reg) >> (bit)) & 1)

typedef enum {
    RATE_SLOW   = 500,
    RATE_MEDIUM = 200,
    RATE_FAST   = 100,
    RATE_OFF
} BlinkRate;

volatile uint32_t ms_ticks   = 0;
static   uint32_t last_blink = 0;
static   uint32_t last_btn   = 0;
static   bool     btn_prev   = false;
static   BlinkRate blink_rate = RATE_SLOW;

ISR(TIMER0_COMPA_vect) {
    ms_ticks++;
}

void timer0_init(void) {
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0  = 0;
    SET_BIT(TCCR0A, WGM01);
    SET_BIT(TCCR0B, CS01);
    SET_BIT(TCCR0B, CS00);
    OCR0A = 249;
    SET_BIT(TIMSK0, OCIE0A);
    sei();
}

void gpio_init(void) {
    SET_BIT(DDRB, LED_PIN);
    CLEAR_BIT(DDRD, BUTTON_PIN);
    SET_BIT(PORTD, BUTTON_PIN);
}

uint32_t millis_get(void) {
    uint32_t ticks;
    cli();
    ticks = ms_ticks;
    sei();
    return ticks;
}

void led_toggle(void) {
    TOGGLE_BIT(PORTB, LED_PIN);
}

bool button_is_pressed(void) {
    return !READ_BIT(PIND, BUTTON_PIN);
}

void advance_blink_rate(void) {
    switch (blink_rate) {
        case RATE_SLOW:   blink_rate = RATE_MEDIUM; break;
        case RATE_MEDIUM: blink_rate = RATE_FAST;   break;
        case RATE_FAST:   blink_rate = RATE_OFF;    break;
        case RATE_OFF:    blink_rate = RATE_SLOW;   break;
    }
}

int main(void) {
    gpio_init();
    timer0_init();

    while (1) {
        uint32_t now = millis_get();

        if (blink_rate == RATE_OFF) {
            CLEAR_BIT(PORTB, LED_PIN);
        } else if (now - last_blink >= (uint32_t) blink_rate) {
            led_toggle();
            last_blink = now;
        }

        if (now - last_btn >= 20) {
            bool btn_now = button_is_pressed();
            if (btn_now && !btn_prev) {
                advance_blink_rate();
            }
            btn_prev = btn_now;
            last_btn = now;
        }
    }

    return 0;
}