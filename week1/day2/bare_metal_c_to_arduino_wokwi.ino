#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN     5   // PB5 = Arduino pin 13
#define BUTTON_PIN  2   // PD2 = Arduino pin 2

void gpio_init(void) {
  DDRB |=   (1 << LED_PIN);     // set PB5 as output
  DDRD &=  ~(1 << BUTTON_PIN);  // set PD2 as input
  PORTD |=  (1 << BUTTON_PIN);  // enable pull-up on PD2
}

uint8_t button_is_pressed(void) {
  return !(PIND & (1 << BUTTON_PIN));   // active low (pull-up)
}

void led_on(void)     { PORTB |=    (1 << LED_PIN); }
void led_off(void)    { PORTB &=   ~(1 << LED_PIN); }
void led_toggle(void) { PORTB ^=    (1 << LED_PIN); }

int main(void)  {
  gpio_init();

  while (1) {
    if (button_is_pressed()) {
      led_on();
    } else {
      led_off();
    }

    _delay_ms(10);    // simple debounce
  }

  return 0;
}