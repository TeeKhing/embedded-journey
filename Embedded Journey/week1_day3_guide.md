# WEEK 1, DAY 3 — Full Learning Guide
## Enums, #define, Fixed-Width Types, and Hardware Register Modelling

**Estimated time: 4–5 hours**
**Tools: VS Code, GCC, Wokwi**
**Save all code to: week1/day3/ in your embedded-journey repo**

---

## WHY THIS DAY MATTERS

Day 1 gave you the data structures of embedded C — pointers, arrays, structs.
Day 2 gave you the language of hardware — bit manipulation, volatile, const, static.

Day 3 gives you the **vocabulary and organisation** of embedded C — the tools
that make your code readable, maintainable, and safe. Specifically:

- `#define` — naming constants and hardware addresses
- `enum` — naming states and options so your code reads like English
- Fixed-width integer types — guaranteeing exact sizes across platforms
- Bitfields inside structs — modelling hardware registers in C

By the end of today you will write a complete hardware register model
in C — the same technique used in every professional microcontroller
header file in the industry.

---

## PART 1 — `#define`: NAMING CONSTANTS AND ADDRESSES (45 minutes)

---

### What `#define` Does

`#define` is a preprocessor directive. Before your code is compiled,
the preprocessor runs through every line and performs text substitution
— replacing every occurrence of the defined name with its value.

```c
#define LED_PIN  5
```

After preprocessing, every occurrence of `LED_PIN` in your code
becomes the number `5`. The compiler never sees `LED_PIN` — it only
sees `5`.

This happens before compilation — it is purely textual replacement.
`LED_PIN` is not a variable. It occupies no memory. It has no address.
It is simply a name the preprocessor replaces with a value.

---

### Why Not Just Use the Number Directly?

Consider this code without `#define`:

```c
DDRB  |= (1 << 5);
PORTB |= (1 << 5);
PORTB &= ~(1 << 5);
```

Three questions:
- What is pin 5?
- What does setting it mean?
- Six months from now, will you remember what 5 represents?

Now with `#define`:

```c
#define LED_PIN  5

DDRB  |= (1 << LED_PIN);    // configure LED pin as output
PORTB |= (1 << LED_PIN);    // turn LED on
PORTB &= ~(1 << LED_PIN);   // turn LED off
```

The intent is immediately clear. And if the LED moves to pin 6,
you change one line — `#define LED_PIN 6` — instead of hunting
through every register operation in your code.

This is not optional in professional embedded code. Magic numbers
(raw numeric literals with no explanation) are considered a code
smell and are explicitly banned in most embedded style guides
including MISRA-C.

---

### `#define` for Hardware Addresses

You saw this in Day 2:

```c
#define PORTB  (*(volatile uint8_t *)0x25)
```

This is the same mechanism — text substitution. Every time the
compiler sees `PORTB`, the preprocessor has already replaced it
with the full hardware register access expression.

Professional microcontroller header files contain hundreds of these:

```c
#define TCCR1A  (*(volatile uint8_t *)0x80)   // Timer 1 Control Register A
#define TCCR1B  (*(volatile uint8_t *)0x81)   // Timer 1 Control Register B
#define TCNT1   (*(volatile uint16_t *)0x84)  // Timer 1 Counter Value
#define OCR1A   (*(volatile uint16_t *)0x88)  // Timer 1 Output Compare A
```

When you include `<avr/io.h>` in an Arduino sketch, you are including
a header file full of exactly these definitions. You have been using
them all along — now you know what they actually are.

---

### `#define` for Bit Positions

Bit positions inside registers also get named:

```c
// TCCR1B register bit positions
#define CS10    0   // Clock Select bit 0
#define CS11    1   // Clock Select bit 1
#define CS12    2   // Clock Select bit 2
#define WGM12   3   // Waveform Generation Mode bit 2
#define WGM13   4   // Waveform Generation Mode bit 3
#define ICES1   6   // Input Capture Edge Select
#define ICNC1   7   // Input Capture Noise Canceler
```

Now instead of:

```c
TCCR1B |= (1 << 3);   // what is bit 3?
```

You write:

```c
TCCR1B |= (1 << WGM12);  // set Waveform Generation Mode bit 2
```

Any engineer reading this code immediately knows what is happening.

---

### `#define` for Expressions and Macros

`#define` can define expressions, not just single values:

```c
#define SET_BIT(reg, bit)    ((reg) |=  (1 << (bit)))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~(1 << (bit)))
#define TOGGLE_BIT(reg, bit) ((reg) ^=  (1 << (bit)))
#define READ_BIT(reg, bit)   (((reg) >> (bit)) & 1)
```

These are function-like macros. They take parameters like functions
but are still pure text substitution — no function call overhead.

Usage:

```c
SET_BIT(PORTB, LED_PIN);     // expands to: PORTB |= (1 << LED_PIN)
CLEAR_BIT(PORTB, LED_PIN);   // expands to: PORTB &= ~(1 << LED_PIN)
```

Notice every parameter is wrapped in parentheses inside the macro.
This is critical. Without them, operator precedence can produce
unexpected results when complex expressions are passed as arguments.

---

### `#define` vs `const` — When to Use Which

```c
#define MAX_SENSORS   8         // preprocessor constant — no memory
const uint8_t MAX_SENSORS = 8; // typed variable — occupies RAM
```

For simple numeric constants in embedded systems, `#define` is
preferred because it occupies zero RAM. Every byte matters on a
microcontroller with 2KB of RAM.

Use `const` when you need type safety — the compiler checks that
you are not assigning a float to a uint8_t constant, for example.
Use `#define` for hardware addresses, pin numbers, and simple
numeric constants.

---

## PART 2 — FIXED-WIDTH INTEGER TYPES (30 minutes)

---

### The Problem With Basic C Types

In standard C, the size of basic types depends on the platform:

```c
int x;   // 2 bytes on some systems, 4 bytes on others, 8 bytes on some
```

On a desktop running 64-bit Linux, `int` is 4 bytes.
On an AVR microcontroller (Arduino Uno), `int` is 2 bytes.
On a 16-bit DSP, `int` is 2 bytes.

This means code written on your desktop may behave differently
when compiled for a microcontroller. A value that fits in an `int`
on your PC may overflow the same `int` on an AVR.

This is not acceptable in embedded systems where hardware register
sizes are fixed and specific.

---

### Fixed-Width Types — The Solution

The header `<stdint.h>` defines types with guaranteed sizes on
every platform:

```c
uint8_t    // unsigned,  exactly 8 bits  (0 to 255)
uint16_t   // unsigned, exactly 16 bits  (0 to 65535)
uint32_t   // unsigned, exactly 32 bits  (0 to 4294967295)
uint64_t   // unsigned, exactly 64 bits

int8_t     // signed,    exactly 8 bits  (-128 to 127)
int16_t    // signed,   exactly 16 bits  (-32768 to 32767)
int32_t    // signed,   exactly 32 bits  (-2147483648 to 2147483647)
int64_t    // signed,   exactly 64 bits
```

The naming convention is always:
`[u for unsigned][int][width in bits]_t`

The `_t` suffix is a C convention meaning "this is a type."

---

### When to Use Which Type

**`uint8_t`** — single byte values, GPIO register manipulation,
array indices on small MCUs, I2C/SPI data bytes, ASCII characters,
flags and status bytes. The most common type in embedded C.

**`uint16_t`** — ADC results (10–12 bit), timer counter values,
16-bit register pairs, array indices on larger systems.

**`uint32_t`** — millisecond timestamps (up to 49 days before
overflow), 32-bit peripheral registers on ARM Cortex-M, large
counters, memory addresses on 32-bit systems.

**`int8_t` / `int16_t` / `int32_t`** — signed values: temperature
readings that go below zero, IMU acceleration values, PID controller
error terms.

**The rule:** always choose the smallest type that fits your value
range. `uint8_t` for values 0–255, `uint16_t` for values 0–65535.
Using `uint32_t` for everything wastes RAM and slows down 8-bit MCUs.

---

### Additional Useful Types from `<stdint.h>`

```c
bool      // requires <stdbool.h> — true or false
size_t    // unsigned type for sizes and array indices — platform-native width
int_fast8_t   // fastest signed type at least 8 bits wide
```

`bool` you have already used in your circular buffer. `size_t` you
saw in `sizeof()` returns and `printf("%zu")`. Use `size_t` for
anything that represents a size or count of memory elements.

---

### Code — Demonstrating Type Sizes and Overflow

Save as week1/day3/fixed_width_types.c

```c
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main(void) {
    printf("=== Fixed-Width Type Sizes ===\n");
    printf("uint8_t:  %zu byte(s)\n", sizeof(uint8_t));
    printf("uint16_t: %zu byte(s)\n", sizeof(uint16_t));
    printf("uint32_t: %zu byte(s)\n", sizeof(uint32_t));
    printf("uint64_t: %zu byte(s)\n", sizeof(uint64_t));
    printf("int8_t:   %zu byte(s)\n", sizeof(int8_t));
    printf("bool:     %zu byte(s)\n", sizeof(bool));
    printf("int:      %zu byte(s)\n", sizeof(int));
    printf("long:     %zu byte(s)\n", sizeof(long));

    printf("\n=== Value Ranges ===\n");
    uint8_t  max_u8  = 255;
    uint16_t max_u16 = 65535;
    int8_t   max_s8  = 127;
    int8_t   min_s8  = -128;

    printf("uint8_t  max: %u\n",  max_u8);
    printf("uint16_t max: %u\n",  max_u16);
    printf("int8_t   max: %d\n",  max_s8);
    printf("int8_t   min: %d\n",  min_s8);

    printf("\n=== Overflow Demonstration ===\n");
    uint8_t counter = 254;
    printf("counter starts at: %u\n", counter);
    counter++;
    printf("after ++:          %u\n", counter);   // 255
    counter++;
    printf("after ++:          %u\n", counter);   // wraps to 0!
    counter++;
    printf("after ++:          %u\n", counter);   // 1

    printf("\n=== Signed Overflow ===\n");
    int8_t signed_val = 126;
    printf("signed_val starts at: %d\n", signed_val);
    signed_val++;
    printf("after ++:             %d\n", signed_val);   // 127
    signed_val++;
    printf("after ++:             %d\n", signed_val);   // wraps to -128!

    return 0;
}
```

WHAT TO OBSERVE:
- `int` and `long` sizes may differ from the fixed-width types
  depending on your system — this proves why fixed-width types exist
- The uint8_t overflow wraps from 255 back to 0 — this is defined
  behaviour for unsigned types in C. It is sometimes exploited
  deliberately in embedded code (timestamp counters, ring buffer
  indices on power-of-2 sized buffers)
- The int8_t overflow from 127 to -128 is technically undefined
  behaviour in C for signed types — another reason to use unsigned
  types wherever possible in embedded systems

---

## PART 3 — ENUMS: NAMING STATES AND OPTIONS (45 minutes)

---

### What an Enum Is

An enum (enumeration) is a named list of integer constants.
It gives meaningful names to a set of related values that represent
states, modes, options, or error codes.

Without enum:

```c
#define STATE_IDLE        0
#define STATE_MEASURING   1
#define STATE_TRANSMITTING 2
#define STATE_ERROR       3

uint8_t current_state = 0;

if (current_state == 1) {
    // measuring
}
```

With enum:

```c
typedef enum {
    STATE_IDLE,
    STATE_MEASURING,
    STATE_TRANSMITTING,
    STATE_ERROR
} SystemState;

SystemState current_state = STATE_IDLE;

if (current_state == STATE_MEASURING) {
    // immediately clear what this means
}
```

The enum version communicates intent. The `#define` version is
a collection of disconnected constants. The enum groups them
logically and the compiler knows they belong together.

---

### How Enum Values Work

By default, enum members are assigned integer values starting at 0
and incrementing by 1:

```c
typedef enum {
    STATE_IDLE,         // 0
    STATE_MEASURING,    // 1
    STATE_TRANSMITTING, // 2
    STATE_ERROR         // 3
} SystemState;
```

You can assign explicit values:

```c
typedef enum {
    ERROR_NONE    = 0,
    ERROR_TIMEOUT = 1,
    ERROR_CRC     = 2,
    ERROR_OVERRUN = 4,   // skipped 3 deliberately
    ERROR_UNKNOWN = 255
} ErrorCode;
```

You can assign the same value to multiple members (useful for aliases):

```c
typedef enum {
    DIRECTION_FORWARD  = 0,
    DIRECTION_BACKWARD = 1,
    DIRECTION_REVERSE  = 1,   // alias for backward
    DIRECTION_STOP     = 2
} MotorDirection;
```

---

### Enums in Embedded Systems — State Machines

The most important use of enums in embedded systems is state machines.
A state machine is a system that exists in one of a finite set of
states and transitions between them based on events.

Every embedded system is fundamentally a state machine:
- An LED controller: OFF → ON → BLINKING → OFF
- A UART receiver: IDLE → RECEIVING → COMPLETE → ERROR
- A sensor: POWERUP → CALIBRATING → MEASURING → SLEEPING

```c
typedef enum {
    SENSOR_POWERUP,
    SENSOR_CALIBRATING,
    SENSOR_MEASURING,
    SENSOR_SLEEPING,
    SENSOR_ERROR
} SensorState;

typedef enum {
    EVENT_NONE,
    EVENT_CALIBRATION_DONE,
    EVENT_READING_READY,
    EVENT_SLEEP_TIMEOUT,
    EVENT_FAULT_DETECTED
} SensorEvent;

SensorState handle_event(SensorState current, SensorEvent event) {
    switch (current) {
        case SENSOR_POWERUP:
            if (event == EVENT_CALIBRATION_DONE)
                return SENSOR_CALIBRATING;
            break;

        case SENSOR_CALIBRATING:
            if (event == EVENT_CALIBRATION_DONE)
                return SENSOR_MEASURING;
            if (event == EVENT_FAULT_DETECTED)
                return SENSOR_ERROR;
            break;

        case SENSOR_MEASURING:
            if (event == EVENT_SLEEP_TIMEOUT)
                return SENSOR_SLEEPING;
            if (event == EVENT_FAULT_DETECTED)
                return SENSOR_ERROR;
            break;

        case SENSOR_SLEEPING:
            if (event == EVENT_READING_READY)
                return SENSOR_MEASURING;
            break;

        default:
            return SENSOR_ERROR;
    }
    return current;
}
```

This pattern — enum for states, enum for events, switch statement
for transitions — is used in virtually every non-trivial embedded
system. You will build state machines starting Week 3.

---

### Enums vs `#define` — When to Use Which

Use `enum` when:
- Values are related and mutually exclusive (states, modes, errors)
- You want the compiler to enforce type checking
- You want a debugger to show the name instead of a raw number

Use `#define` when:
- Values are independent constants (pin numbers, buffer sizes)
- You need the value at preprocessor time (array size declarations)
- You need non-integer constants (float, string)

```c
// correct use of enum — related states
typedef enum { LED_OFF, LED_ON, LED_BLINKING } LedState;

// correct use of #define — independent constants
#define LED_PIN        5
#define BLINK_DELAY_MS 500
#define MAX_RETRIES    3
```

---

### Code — Enums and State Machines

Save as week1/day3/enum_state_machine.c

```c
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
        case LED_OFF:      return "OFF";
        case LED_ON:       return "ON";
        case LED_BLINKING: return "BLINKING";
        default:           return "UNKNOWN";
    }
}

LedState process_command(LedState current, LedCommand cmd) {
    switch (current) {
        case LED_OFF:
            if (cmd == CMD_TURN_ON) return LED_ON;
            if (cmd == CMD_BLINK)   return LED_BLINKING;
            break;

        case LED_ON:
            if (cmd == CMD_TURN_OFF) return LED_OFF;
            if (cmd == CMD_BLINK)    return LED_BLINKING;
            break;

        case LED_BLINKING:
            if (cmd == CMD_TURN_ON)  return LED_ON;
            if (cmd == CMD_TURN_OFF) return LED_OFF;
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
        CMD_TURN_OFF
    };

    const char *cmd_names[] = {
        "CMD_TURN_ON",
        "CMD_TURN_OFF",
        "CMD_BLINK",
        "CMD_INVALID"
    };

    for (uint8_t i = 0; i < 5; i++) {
        LedState previous = state;
        state = process_command(state, commands[i]);
        printf("Command: %-15s | %s → %s\n",
               cmd_names[commands[i]],
               state_to_string(previous),
               state_to_string(state));
    }

    return 0;
}
```

WHAT TO OBSERVE:
- The `state_to_string` function converts an enum value to a readable
  string — essential for debugging state machines over UART
- The switch statement inside `process_command` is the state machine
  — each case is a state, each if-block is a transition condition
- Invalid commands in a given state are silently ignored — the
  function returns `current` unchanged. This is safe default behaviour.
- The `%-15s` format specifier left-aligns the string in a 15-character
  field — producing clean columnar output

---

## PART 4 — HARDWARE REGISTER MODELLING WITH STRUCTS AND BITFIELDS (60 minutes)

This is the most advanced concept of Day 3 and the most directly
applicable to real embedded development.

---

### The Problem: Hardware Registers Pack Multiple Fields

A single hardware register often contains multiple independent fields.
Each field controls a different aspect of the peripheral.

Take the ATmega328P UART Control and Status Register A (UCSR0A):

```
Bit:    7      6      5      4      3      2      1      0
Name:  RXC0  TXC0  UDRE0  FE0   DOR0   UPE0   U2X0  MPCM0
```

Each bit or group of bits means something different:
- Bit 7 (RXC0): Receive Complete — hardware sets this when a byte arrives
- Bit 5 (UDRE0): Data Register Empty — hardware sets this when ready to send
- Bit 1 (U2X0): Double Speed mode — you set this to double UART speed

To check if a byte has been received:

```c
if (UCSR0A & (1 << 7)) { ... }   // cryptic
if (UCSR0A & (1 << RXC0)) { ... } // better — but RXC0 is just a number
```

What if you could access it like a struct member?

```c
if (uart_status.RXC0) { ... }   // completely readable
```

This is what bitfields give you.

---

### Bitfields — Allocating Specific Numbers of Bits to Members

A bitfield is a struct member with a specified bit width using the
colon syntax:

```c
typedef struct {
    uint8_t field_a : 3;   // field_a occupies exactly 3 bits
    uint8_t field_b : 4;   // field_b occupies exactly 4 bits
    uint8_t field_c : 1;   // field_c occupies exactly 1 bit
} PackedRegister;          // total: 3 + 4 + 1 = 8 bits = 1 byte
```

The `: N` after the member name tells the compiler to allocate only
N bits for that member. Multiple bitfield members pack together into
the underlying storage type.

Accessing bitfield members uses the same dot or arrow operator as
regular struct members:

```c
PackedRegister reg;
reg.field_a = 5;     // stores 5 in the 3-bit field (max value: 7)
reg.field_b = 12;    // stores 12 in the 4-bit field (max value: 15)
reg.field_c = 1;     // stores 1 in the 1-bit field  (max value: 1)
```

---

### Modelling a Real Register — UCSR0A

```c
typedef struct {
    uint8_t MPCM0 : 1;   // bit 0 — Multi-processor Communication Mode
    uint8_t U2X0  : 1;   // bit 1 — Double UART Speed
    uint8_t UPE0  : 1;   // bit 2 — Parity Error flag
    uint8_t DOR0  : 1;   // bit 3 — Data OverRun flag
    uint8_t FE0   : 1;   // bit 4 — Frame Error flag
    uint8_t UDRE0 : 1;   // bit 5 — Data Register Empty flag
    uint8_t TXC0  : 1;   // bit 6 — Transmit Complete flag
    uint8_t RXC0  : 1;   // bit 7 — Receive Complete flag
} UCSR0A_t;
```

Now you can model the register as a variable and access each bit
by name:

```c
volatile UCSR0A_t *uart_status = (UCSR0A_t *)0xC0;

if (uart_status->RXC0) {
    uint8_t received_byte = UDR0;   // read the received byte
}

if (uart_status->UDRE0) {
    UDR0 = 'A';   // safe to send — transmit buffer is empty
}

uart_status->U2X0 = 1;   // enable double speed mode
```

This is dramatically more readable than raw bit manipulation, and
the compiler generates identical machine code.

---

### The Union Trick — Access as Both Struct and Raw Byte

Professional embedded code often combines a bitfield struct with
a union so you can access the register either by named field or
as a raw byte — whichever is more convenient:

```c
typedef union {
    struct {
        uint8_t MPCM0 : 1;
        uint8_t U2X0  : 1;
        uint8_t UPE0  : 1;
        uint8_t DOR0  : 1;
        uint8_t FE0   : 1;
        uint8_t UDRE0 : 1;
        uint8_t TXC0  : 1;
        uint8_t RXC0  : 1;
    } bits;
    uint8_t byte;
} UCSR0A_t;
```

A union makes all its members share the same memory location.
So `bits` and `byte` are different views of the same 8 bits:

```c
UCSR0A_t status;

status.byte = 0b00100000;   // set UDRE0 directly as raw byte

if (status.bits.UDRE0) {    // check UDRE0 by name
    printf("TX ready\n");
}

printf("Raw register value: 0x%02X\n", status.byte);
```

This pattern — union of bitfield struct and raw integer — is used
in every professional MCU header file and HAL library.

---

### Code — Complete Register Model

Save as week1/day3/register_model.c

```c
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
        uint8_t pin5 : 1;   // Arduino pin 13 (LED)
        uint8_t pin6 : 1;
        uint8_t pin7 : 1;
    } bits;
    uint8_t byte;
} GpioReg;

static GpioReg DDRB  = {.byte = 0x00};
static GpioReg PORTB = {.byte = 0x00};
static GpioReg PINB  = {.byte = 0x00};

#define LED_PIN   5
#define BTN_PIN   2

void gpio_init(void) {
    DDRB.bits.pin5  = 1;   // LED pin as output
    DDRB.bits.pin2  = 0;   // Button pin as input
    PORTB.bits.pin2 = 1;   // Enable pull-up on button pin
}

void led_on(void)     { PORTB.bits.pin5 = 1; }
void led_off(void)    { PORTB.bits.pin5 = 0; }
void led_toggle(void) { PORTB.bits.pin5 ^= 1; }

bool button_is_pressed(void) {
    return !PINB.bits.pin2;   // active low — pressed = LOW = 0
}

void simulate_button_press(void)   { PINB.bits.pin2 = 0; }
void simulate_button_release(void) { PINB.bits.pin2 = 1; }

void print_register_state(void) {
    printf("DDRB  byte=0x%02X | pin5(LED dir)=%u  pin2(BTN dir)=%u\n",
           DDRB.byte,
           DDRB.bits.pin5,
           DDRB.bits.pin2);

    printf("PORTB byte=0x%02X | pin5(LED out)=%u  pin2(BTN pullup)=%u\n",
           PORTB.byte,
           PORTB.bits.pin5,
           PORTB.bits.pin2);

    printf("PINB  byte=0x%02X | pin5(LED in)=%u   pin2(BTN in)=%u\n\n",
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
    printf("led_on():     PORTB=0x%02X (pin5=%u)\n",
           PORTB.byte, PORTB.bits.pin5);

    led_off();
    printf("led_off():    PORTB=0x%02X (pin5=%u)\n",
           PORTB.byte, PORTB.bits.pin5);

    led_toggle();
    printf("led_toggle(): PORTB=0x%02X (pin5=%u)\n",
           PORTB.byte, PORTB.bits.pin5);

    led_toggle();
    printf("led_toggle(): PORTB=0x%02X (pin5=%u)\n\n",
           PORTB.byte, PORTB.bits.pin5);

    printf("=== Button Simulation ===\n");
    simulate_button_release();
    printf("Button released: pressed=%u\n", button_is_pressed());

    simulate_button_press();
    printf("Button pressed:  pressed=%u\n", button_is_pressed());

    return 0;
}
```

WHAT TO OBSERVE:
- The union lets you write `PORTB.bits.pin5 = 1` (named field access)
  and read `PORTB.byte` (full register value) — both work on the
  same 8 bits simultaneously
- `led_toggle()` uses `^= 1` directly on a single bitfield member
  instead of XOR-ing a full register — cleaner when you have named fields
- `print_register_state` prints both the raw byte value and individual
  named bits — this is how hardware debuggers display register state
- When `gpio_init` sets `DDRB.bits.pin5 = 1`, the DDRB byte changes
  from 0x00 to 0x20 — because pin5 is bit 5, and bit 5 set = 0x20.
  Verify this matches your Day 2 knowledge: `(1 << 5) = 0x20`

---

## PART 5 — PUTTING IT ALL TOGETHER: COMPLETE GPIO DRIVER (45 minutes)

Now combine everything from Days 1, 2, and 3 into a professional-
quality GPIO driver — the kind you would find in a real embedded
project.

Save as week1/day3/gpio_driver.c

```c
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * GPIO Driver — Simulated ATmega328P
 * Demonstrates: #define, enums, fixed-width types,
 *               bitfield register model, state machine
 */

#define MAX_PINS     8
#define LED_PIN      5
#define BUTTON_PIN   2

typedef enum {
    GPIO_INPUT  = 0,
    GPIO_OUTPUT = 1
} GpioDirection;

typedef enum {
    GPIO_LOW  = 0,
    GPIO_HIGH = 1
} GpioState;

typedef enum {
    GPIO_NO_PULL   = 0,
    GPIO_PULL_UP   = 1
} GpioPull;

typedef enum {
    GPIO_OK             = 0,
    GPIO_ERR_INVALID_PIN = 1,
    GPIO_ERR_NOT_OUTPUT  = 2,
    GPIO_ERR_NOT_INPUT   = 3
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

static PortReg DDRB  = {.byte = 0x00};
static PortReg PORTB = {.byte = 0x00};
static PortReg PINB  = {.byte = 0x00};

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
    if (!pin_valid(pin))              return GPIO_ERR_INVALID_PIN;
    if (!(DDRB.byte & (1 << pin)))   return GPIO_ERR_NOT_OUTPUT;
    if (state == GPIO_HIGH) {
        PORTB.byte |= (1 << pin);
    } else {
        PORTB.byte &= ~(1 << pin);
    }
    return GPIO_OK;
}

GpioError gpio_toggle(uint8_t pin) {
    if (!pin_valid(pin))            return GPIO_ERR_INVALID_PIN;
    if (!(DDRB.byte & (1 << pin))) return GPIO_ERR_NOT_OUTPUT;
    PORTB.byte ^= (1 << pin);
    return GPIO_OK;
}

GpioError gpio_read(uint8_t pin, GpioState *state) {
    if (!pin_valid(pin)) return GPIO_ERR_INVALID_PIN;
    if (DDRB.byte & (1 << pin)) return GPIO_ERR_NOT_INPUT;
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
        case GPIO_OK:              return "OK";
        case GPIO_ERR_INVALID_PIN: return "INVALID PIN";
        case GPIO_ERR_NOT_OUTPUT:  return "NOT AN OUTPUT";
        case GPIO_ERR_NOT_INPUT:   return "NOT AN INPUT";
        default:                   return "UNKNOWN ERROR";
    }
}

int main(void) {
    GpioError err;

    printf("=== GPIO Driver Test ===\n\n");

    err = gpio_set_direction(LED_PIN, GPIO_OUTPUT);
    printf("Set pin %u as OUTPUT: %s\n", LED_PIN, error_to_string(err));

    err = gpio_set_direction(BUTTON_PIN, GPIO_INPUT);
    printf("Set pin %u as INPUT:  %s\n", BUTTON_PIN, error_to_string(err));

    err = gpio_set_pull(BUTTON_PIN, GPIO_PULL_UP);
    printf("Enable pull-up pin %u: %s\n\n", BUTTON_PIN, error_to_string(err));

    printf("=== LED Control ===\n");
    err = gpio_write(LED_PIN, GPIO_HIGH);
    printf("Write HIGH to LED: %s | PORTB=0x%02X\n",
           error_to_string(err), PORTB.byte);

    err = gpio_write(LED_PIN, GPIO_LOW);
    printf("Write LOW to LED:  %s | PORTB=0x%02X\n",
           error_to_string(err), PORTB.byte);

    printf("\n=== Error Handling ===\n");
    err = gpio_write(9, GPIO_HIGH);
    printf("Write to pin 9 (invalid):     %s\n", error_to_string(err));

    err = gpio_write(BUTTON_PIN, GPIO_HIGH);
    printf("Write to input pin %u:         %s\n",
           BUTTON_PIN, error_to_string(err));

    printf("\n=== Button Read ===\n");
    GpioState btn_state;

    simulate_pin_input(BUTTON_PIN, GPIO_LOW);
    gpio_read(BUTTON_PIN, &btn_state);
    printf("Button (LOW/pressed):  %s\n",
           btn_state == GPIO_LOW ? "PRESSED" : "RELEASED");

    simulate_pin_input(BUTTON_PIN, GPIO_HIGH);
    gpio_read(BUTTON_PIN, &btn_state);
    printf("Button (HIGH/released): %s\n",
           btn_state == GPIO_LOW ? "PRESSED" : "RELEASED");

    return 0;
}
```

WHAT TO OBSERVE:
- Every function returns a `GpioError` enum — the caller always
  knows if an operation failed and exactly why
- `gpio_write` checks that the pin is configured as output before
  writing — attempting to write to an input pin returns an error
  instead of silently corrupting the register
- `gpio_read` checks that the pin is an input — reading from an
  output pin returns an error
- The `error_to_string` function converts error codes to readable
  strings — this is how you would display errors over UART on
  a real embedded system
- All internal variables (DDRB, PORTB, PINB, pin_valid) are `static`
  — they are private to this file. External code uses only the
  public functions. This is the hardware abstraction layer pattern.

---

## PART 6 — WOKWI CHALLENGE: UPGRADE THE DAY 2 SKETCH (30 minutes)

Open your Day 2 Wokwi project and upgrade the code to use:

1. An enum for LED states
2. `#define` macros for SET_BIT, CLEAR_BIT, TOGGLE_BIT
3. Proper fixed-width types everywhere

```cpp
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define LED_PIN     5
#define BUTTON_PIN  2

#define SET_BIT(reg, bit)    ((reg) |=  (1 << (bit)))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~(1 << (bit)))
#define TOGGLE_BIT(reg, bit) ((reg) ^=  (1 << (bit)))
#define READ_BIT(reg, bit)   (((reg) >> (bit)) & 1)

typedef enum {
    LED_OFF,
    LED_ON,
    LED_BLINKING
} LedState;

static LedState current_led_state = LED_OFF;

void gpio_init(void) {
    SET_BIT(DDRB, LED_PIN);
    CLEAR_BIT(DDRD, BUTTON_PIN);
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
```

**Modification Challenge:**

Extend the state machine so a single button press cycles through
three states:

```
LED_OFF → LED_ON → LED_BLINKING → LED_OFF → ...
```

- In `LED_BLINKING` state, the LED toggles every 200ms
- A button press always advances to the next state
- Use `static` for the debounce mechanism

---

## PART 7 — GIT COMMIT (10 minutes)

```cmd
cd C:\Users\USER\Desktop\KingTee\embedded-journey
git add .
git commit -m "Week 1 Day 3: enums, #define, fixed-width types, register modelling"
git push origin main
```

---

## PART 8 — DAY 3 REVIEW QUESTIONS

Write answers in notes/week1.md — add a Day 3 section.

1. What is the difference between `#define MAX 8` and
   `const uint8_t MAX = 8`? Which occupies RAM?

2. What is the value of each member in this enum?
   ```c
   typedef enum { A, B, C = 10, D, E } Letters;
   ```

3. Why must every parameter in a function-like macro be wrapped
   in parentheses?

4. What does the `: 1` mean in this struct member?
   ```c
   uint8_t RXC0 : 1;
   ```

5. What is a union and how does it differ from a struct?

6. After this code runs, what is the value of `reg.byte`?
   ```c
   typedef union {
       struct { uint8_t a:3; uint8_t b:5; } bits;
       uint8_t byte;
   } Reg;
   Reg reg = {.byte = 0};
   reg.bits.a = 5;
   ```

7. Why does the GPIO driver return `GpioError` from every function
   instead of `void`?

8. What does `static` on a function mean, and why are internal
   driver functions like `pin_valid` declared static?

---

## WHAT YOU ACCOMPLISHED TODAY

- `#define` for constants, hardware addresses, and function-like macros
- Fixed-width types — guaranteed sizes across all platforms
- Enums for states, error codes, and options — code that reads
  like English
- Bitfield structs and unions — professional hardware register models
- A complete GPIO driver combining all three days' concepts

This is the last day of pure C foundation. Day 4 begins actual
microcontroller programming — you will flash real code (in Wokwi)
and understand every register write that makes the hardware respond.
