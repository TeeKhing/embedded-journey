#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define LED_PIN     5
#define BUTTON_PIN  2

#define SET_BIT(reg, bit)     ((reg)  |=  (1 << (bit)))
#define CLEAR_BIT(reg, bit)   ((reg)  &= ~(1 << (bit)))
#define TOGGLE_BIT(reg, bit)  ((reg)  ^=  (1 << (bit)))
#define READ_BIT(reg, bit)    (((reg)  >> (bit)) & 1)

typedef enum {
  LED_OFF,
  LED_ON,
  LED_BLINKING
} LedState;

static LedState current_led_state = LED_OFF;

void gpio_init(void) {
  SET_BIT(DDRB, LED_PIN);
  CLEAR_BIT(DDRB, BUTTON_PIN);
  SET_BIT(PORTD, BUTTON_PIN);
}

bool button_is_pressed(void) {
  return !READ_BIT(PIND, BUTTON_PIN);
}

void led_set_state(LedState state) {
  current_led_state = state;
  if (state == LED_ON) {
    SET_BIT(PORTB, LED_PIN);
  } else if (state == LED_OFF) {
    CLEAR_BIT(PORTB, LED_PIN);
  }
}

int main(void) {
  gpio_init();

  while (1) {
    if (button_is_pressed()) {
      led_set_state(LED_ON);
    } else {
      led_set_state(LED_OFF);
    }
    _delay_ms(10);
  }

  return 0;
}