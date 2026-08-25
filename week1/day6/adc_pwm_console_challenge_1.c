#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/* ── Configuration ─────────────────────────────────────────── */
#define F_CPU    16000000UL
#define BAUD     115200UL
#define POT_CH   0       /* potentiometer on ADC0 = Arduino A0 */
#define PWM_PIN  3       /* OC2B = PD3 = Arduino pin 3         */

/* ── Macros ────────────────────────────────────────────────── */
#define SET_BIT(r,b)    ((r) |=  (1<<(b)))
#define CLEAR_BIT(r,b)  ((r) &= ~(1<<(b)))
#define TOGGLE_BIT(r,b) ((r) ^=  (1<<(b)))
#define READ_BIT(r,b)   (((r)>>(b))&1)

/* ── Global state ───────────────────────────────────────────── */
volatile uint32_t ms_ticks     = 0;
static   bool     auto_report  = true;
static   uint8_t  report_rate  = 10;   /* report every N x 100ms */

/* ── Timer0 ISR (1ms tick) ──────────────────────────────────── */
ISR(TIMER0_COMPA_vect) {
    ms_ticks++;
}

/* ── Timer0 init (1ms tick) ─────────────────────────────────── */
void timer0_init(void) {
    TCCR0A = 0; TCCR0B = 0; TCNT0 = 0;
    SET_BIT(TCCR0A, WGM01);
    SET_BIT(TCCR0B, CS01);
    SET_BIT(TCCR0B, CS00);
    OCR0A = 249;
    SET_BIT(TIMSK0, OCIE0A);
    sei();
}

/* ── ADC init ───────────────────────────────────────────────── */
void adc_init(void) {
    ADMUX  = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    DIDR0 |= (1 << ADC0D);
    
    /* DEBUG: Verify registers after init */
    uart_send_string("[INIT] ADMUX=");
    uart_send_hex8(ADMUX);
    uart_send_string("  ADCSRA=");
    uart_send_hex8(ADCSRA);
    uart_send_string("\r\n");
    
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
}

uint16_t adc_read(uint8_t ch) {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

uint16_t adc_read_oversampled(uint8_t ch, uint8_t extra_bits) {
    uint8_t  n   = (1 << (extra_bits * 2));
    uint32_t sum = 0;
    for (uint8_t i = 0; i < n; i++) sum += adc_read(ch);
    return (uint16_t)(sum >> extra_bits);
}

/* ── PWM init (Timer2, OC2B, pin 3) ────────────────────────── */
void pwm_init(void) {
    SET_BIT(DDRD, PWM_PIN);
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS22);
    OCR2B  = 0;
}

void pwm_set(uint8_t duty) {
    OCR2B = duty;
}

/* ── UART ───────────────────────────────────────────────────── */
void uart_init(void) {
    uint16_t ubrr = (F_CPU / (8UL * BAUD)) - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);
    UCSR0A = (1 << U2X0);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send_byte(uint8_t b) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = b;
}

void uart_send_string(const char *s) {
    while (*s) uart_send_byte((uint8_t)*s++);
}

void uart_send_uint16(uint16_t val) {
    char buf[6]; uint8_t idx = 0;
    if (!val) { uart_send_byte('0'); return; }
    while (val) { buf[idx++] = '0' + (val % 10); val /= 10; }
    while (idx) uart_send_byte((uint8_t)buf[--idx]);
}

void uart_send_fixed(uint16_t integer, uint16_t frac_mv) {
    /* Prints "integer.frac" e.g. uart_send_fixed(3, 142) -> "3.142" */
    uart_send_uint16(integer);
    uart_send_byte('.');
    if (frac_mv < 100) uart_send_byte('0');
    if (frac_mv < 10)  uart_send_byte('0');
    uart_send_uint16(frac_mv);
}

void uart_send_hex8(uint8_t v) {
    const char h[] = "0123456789ABCDEF";
    uart_send_byte('0'); uart_send_byte('x');
    uart_send_byte(h[(v>>4)&0xF]);
    uart_send_byte(h[v&0xF]);
}

uint8_t uart_available(void) {
    return (UCSR0A & (1 << RXC0)) ? 1 : 0;
}

uint8_t uart_read_byte(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

/* ── Millis ─────────────────────────────────────────────────── */
uint32_t millis_get(void) {
    uint32_t t; cli(); t = ms_ticks; sei(); return t;
}

/* ── ADC to voltage (integer math, no float) ─────────────────
 * Returns millivolts to avoid floating point on AVR.
 * ADC result 0-1023 maps to 0-5000mV
 * millivolts = (adc_result * 5000UL) / 1023
 */
uint16_t adc_to_mv(uint16_t adc_val) {
    return (uint16_t)((uint32_t)adc_val * 5000UL / 1023UL);
}

/* ── Map value from one range to another ─────────────────────
 * Maps val from [in_min, in_max] to [out_min, out_max]
 * Integer version -- no floating point
 */
uint16_t map_value(uint16_t val,
                   uint16_t in_min,  uint16_t in_max,
                   uint16_t out_min, uint16_t out_max) {
    return (uint16_t)(
        (uint32_t)(val - in_min) *
        (out_max - out_min) /
        (in_max - in_min) + out_min
    );
}

/* ── Command handler ────────────────────────────────────────── */
void handle_command(uint8_t cmd) {
    if (cmd == '\r' || cmd == '\n') return; 
    switch (cmd) {
        case 'a': {
            /* Single ADC reading */
            uint16_t raw = adc_read(POT_CH);
            uint16_t mv  = adc_to_mv(raw);
            uart_send_string("[ADC] raw=");
            uart_send_uint16(raw);
            uart_send_string("  voltage=");
            uart_send_fixed(mv / 1000, mv % 1000);
            uart_send_string("V\r\n");
            break;
        }
        case 'o': {
            /* Oversampled 12-bit reading */
            uint16_t precise = adc_read_oversampled(POT_CH, 2);
            uint16_t mv = (uint16_t)((uint32_t)precise * 5000UL / 4095UL);
            uart_send_string("[ADC] 12-bit raw=");
            uart_send_uint16(precise);
            uart_send_string("  voltage=");
            uart_send_fixed(mv / 1000, mv % 1000);
            uart_send_string("V\r\n");
            break;
        }
        case '+':
            if (report_rate > 1) report_rate--;
            uart_send_string("[RATE] faster\r\n");
            break;
        case '-':
            if (report_rate < 50) report_rate++;
            uart_send_string("[RATE] slower\r\n");
            break;
        case 'r':
            auto_report = !auto_report;
            uart_send_string(auto_report ? "[AUTO] on\r\n" : "[AUTO] off\r\n");
            break;
        case '?':
            uart_send_string("\r\n--- Commands ---\r\n");
            uart_send_string("  a = single ADC read\r\n");
            uart_send_string("  o = oversampled 12-bit read\r\n");
            uart_send_string("  + = faster reporting\r\n");
            uart_send_string("  - = slower reporting\r\n");
            uart_send_string("  r = toggle auto-report\r\n");
            uart_send_string("  ? = this help\r\n\r\n");
            break;
        default:
            uart_send_string("[ERR] unknown command\r\n");
    }
}

/* ── Main ───────────────────────────────────────────────────── */
int main(void) {
    timer0_init();
    adc_init();
    pwm_init();
    uart_init();

    uart_send_string("\r\n=== ADC + PWM Console ===\r\n");
    uart_send_string("Potentiometer on A0, LED on pin 3\r\n");
    uart_send_string("Type ? for commands\r\n\r\n");

    uint32_t last_report = 0;
    uint16_t readings[8];
    uint8_t read_index = 0;

    while (1) {
        uint32_t now = millis_get();

        /* Handle incoming commands */
        if (uart_available()) {
            handle_command(uart_read_byte());
        }

        /* Auto-report ADC and PWM */
        if (auto_report && (now - last_report >= (uint32_t)report_rate * 100UL)) {
            readings[read_index] = adc_read(POT_CH);
            read_index = (read_index + 1) % 8;

            uint32_t sum = 0;
            for (uint8_t i = 0; i < 8; i++) {
                sum += readings[i];
            }
            uint16_t raw = (uint16_t)(sum / 8);

            uint16_t mv   = adc_to_mv(raw);
            uint8_t  duty = (uint8_t)map_value(raw, 0, 1023, 0, 255);

            pwm_set(duty);

            uart_send_string("[SENSOR] raw=");
            uart_send_uint16(raw);
            uart_send_string("  voltage=");
            uart_send_fixed(mv / 1000, mv % 1000);
            uart_send_string("V  duty=");
            uart_send_uint16(duty);
            uart_send_string("/255\r\n");

            last_report = now;
        }
    }

    return 0;
}