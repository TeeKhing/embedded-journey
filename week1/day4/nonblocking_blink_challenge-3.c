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
#define SOS_SEQUENCE_LENGTH (sizeof(sos_sequence) / sizeof(sos_sequence[0]))

typedef enum {
    RATE_SLOW   = 500,
    RATE_MEDIUM = 200,
    RATE_FAST   = 100,
    RATE_OFF,
    RATE_SOS
} BlinkRate;

typedef struct {
  bool      is_on;
  uint16_t  duration;
} SOSStep;

volatile uint32_t ms_ticks   = 0;
static   uint32_t last_blink = 0;
static   uint32_t last_btn   = 0;
static   uint8_t  sos_step   = 0;
static   uint32_t step_start = 0;
static   bool     btn_prev   = false;
static   BlinkRate blink_rate = RATE_SLOW;

static const SOSStep sos_sequence[] = {
    { true, 100 },
    { false, 100 },
    { true, 100 },
    { false, 100 },
    { true, 100 },
    { false, 300 },

    {true, 300},
    {false, 100},
    {true, 300},
    {false, 100},
    {true, 300},
    {false, 300},

    { true, 100 },
    { false, 100 },
    { true, 100 },
    { false, 100 },
    { true, 100 },
    { false, 700 }
};


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
        case RATE_OFF:    blink_rate = RATE_SOS;    break;
        case RATE_SOS:    blink_rate = RATE_SLOW;   break;
    }
}

int main(void) {
    gpio_init();
    timer0_init();

    while (1) {
        uint32_t now = millis_get();

        if (blink_rate == RATE_OFF) {
            CLEAR_BIT(PORTB, LED_PIN);
        } else if (blink_rate == RATE_SOS) {
          if (now - step_start >= sos_sequence[sos_step].duration){
              sos_step = (sos_step + 1) % SOS_SEQUENCE_LENGTH;
              step_start = now;
          }

          if (sos_sequence[sos_step].is_on) {
              SET_BIT(PORTB, LED_PIN);
          } else {
            CLEAR_BIT(PORTB, LED_PIN);
          }
        } else if (now - last_blink >= (uint32_t) blink_rate) {
            led_toggle();
            last_blink = now;
        }

        if (now - last_btn >= 20) {
            bool btn_now = button_is_pressed();
            uint32_t press_start, held_time;
            if (btn_now && !btn_prev) {
                press_start = now;
            } else if (!btn_now && btn_prev) {
                held_time = now - press_start;

                if (held_time >= 500) {
                    blink_rate = RATE_SLOW;
                } else {
                    advance_blink_rate();
                }
            }
            btn_prev = btn_now;
            last_btn = now;
        }
    }

    return 0;
}