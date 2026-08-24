#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/* ── Configuration ─────────────────────────────────────────── */
#define F_CPU   16000000UL
#define BAUD    115200UL
#define LED_PIN 5
#define BTN_PIN 2

/* ── Macros ────────────────────────────────────────────────── */
#define SET_BIT(r,b)    ((r) |=  (1<<(b)))
#define CLEAR_BIT(r,b)  ((r) &= ~(1<<(b)))
#define TOGGLE_BIT(r,b) ((r) ^=  (1<<(b)))
#define READ_BIT(r,b)   (((r)>>(b))&1)

/* ── System state ───────────────────────────────────────────── */
volatile uint32_t ms_ticks = 0;

typedef enum {
    SYS_IDLE,
    SYS_RUNNING,
    SYS_ERROR
} SystemState;

static SystemState sys_state  = SYS_IDLE;
static uint32_t    loop_count = 0;

/* ── Timer0 ISR ─────────────────────────────────────────────── */
ISR(TIMER0_COMPA_vect) {
    ms_ticks++;
}

/* ── Timer0 init ────────────────────────────────────────────── */
void timer0_init(void) {
    TCCR0A = 0; TCCR0B = 0; TCNT0 = 0;
    SET_BIT(TCCR0A, WGM01);
    SET_BIT(TCCR0B, CS01);
    SET_BIT(TCCR0B, CS00);
    OCR0A = 249;
    SET_BIT(TIMSK0, OCIE0A);
    sei();
}

/* ── GPIO init ──────────────────────────────────────────────── */
void gpio_init(void) {
    SET_BIT(DDRB, LED_PIN);
    CLEAR_BIT(DDRD, BTN_PIN);
    SET_BIT(PORTD, BTN_PIN);
}

/* ── UART init ──────────────────────────────────────────────── */
void uart_init(void) {
    uint16_t ubrr = (F_CPU / (8UL * BAUD)) - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);
    UCSR0A = (1 << U2X0);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/* ── UART transmit ──────────────────────────────────────────── */
void uart_send_byte(uint8_t byte) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = byte;
}

void uart_send_string(const char *str) {
    while (*str) uart_send_byte((uint8_t)*str++);
}

void uart_send_uint32(uint32_t val) {
    char    buf[11];
    uint8_t idx = 0;
    if (val == 0) { uart_send_byte('0'); return; }
    while (val > 0) { buf[idx++] = '0' + (val % 10); val /= 10; }
    while (idx > 0) uart_send_byte((uint8_t) buf[--idx]);
}

void uart_send_hex8(uint8_t val) {
    const char h[] = "0123456789ABCDEF";
    uart_send_byte('0'); uart_send_byte('x');
    uart_send_byte((uint8_t)h[(val >> 4) & 0xF]);
    uart_send_byte((uint8_t)h[val & 0xF]);
}

void uart_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'u': uart_send_uint32 (va_arg(args, uint32_t));  break;
                case 'x': uart_send_hex8 ((uint8_t) va_arg(args,int));    break;
                case 's': uart_send_string (va_arg(args, char *));        break;
                case 'c': uart_send_byte ((uint8_t) va_arg(args,int));    break;
                case '%': uart_send_byte ('%');                           break;
            }
        } else {
            uart_send_byte((uint8_t)*fmt);
        }
        fmt++;
    }
    va_end(args);
}

/* ── UART receive ───────────────────────────────────────────── */
uint8_t uart_available(void) {
    return (UCSR0A & (1 << RXC0)) ? 1 : 0;
}

uint8_t uart_read_byte(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

/* ── Millis ─────────────────────────────────────────────────── */
uint32_t millis_get(void) {
    uint32_t t;
    cli(); t = ms_ticks; sei();
    return t;
}

/* ── Command handler ────────────────────────────────────────── */
void handle_command(uint8_t cmd) {
    switch (cmd) {
        case '\r': return;   // ignore carriage return
        case '\n': return;   // ignore newline
        case 's':
            sys_state = SYS_RUNNING;
            uart_printf("[CMD] System started\r\n");
            break;
        case 'p':
            sys_state = SYS_IDLE;
            uart_printf("[CMD] System paused\r\n");
            break;
        case 'r':
            loop_count = 0;
            uart_printf("[CMD] Counter reset\r\n");
            break;
        case 'e':
            sys_state = SYS_ERROR;
            uart_printf("[CMD] Error state triggered\r\n");
            break;
        case '?':
            uart_printf("\r\n--- Commands ---\r\n");
            uart_printf("  s = start\r\n");
            uart_printf("  p = pause\r\n");
            uart_printf("  r = reset counter\r\n");
            uart_printf("  e = trigger error\r\n");
            uart_printf("  ? = this help\r\n\r\n");
            break;
        default:
            uart_printf("[ERR] Unknown command: %c\r\n", cmd);
    }
}

/* ── Main ───────────────────────────────────────────────────── */
int main(void) {
    gpio_init();
    timer0_init();
    uart_init();
    uart_printf("DDRB after init = %x\r\n", DDRB);

    uart_printf("\r\n=== UART Debug Console ===\r\n");
    uart_printf("Type ? for commands\r\n\r\n");

    uint32_t last_report = 0;
    uint32_t last_blink  = 0;

    while (1) {
        uint32_t now = millis_get();

        /* Receive and handle commands */
        if (uart_available()) {
            uint8_t cmd = uart_read_byte();

            if (cmd != '\r' && cmd != '\n') {
                uart_printf("[RX] Got: %c\r\n", cmd);
            }

            handle_command(cmd);
        }

        /* Report system status every 2 seconds */
        if (now - last_report >= 2000) {
            uart_printf("[STATUS] time=%u ms | state=%u | loops=%u | PORTB=%x\r\n",
                        now, (uint32_t)sys_state, loop_count, PORTB);
            last_report = now;
        }

        /* Blink LED based on state */
        uint32_t blink_interval = (sys_state == SYS_RUNNING) ? 200 :
                                  (sys_state == SYS_ERROR) ? 100 : 1000;

        if (now - last_blink >= blink_interval) {
            TOGGLE_BIT(PORTB, LED_PIN);
            last_blink = now;
        }

        /* Increment loop counter when running */
        if (sys_state == SYS_RUNNING) {
            loop_count++;
        }
    }

    return 0;
}