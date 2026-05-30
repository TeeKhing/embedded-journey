# WEEK 1, DAY 2 — Full Learning Guide
## Bit Manipulation, `volatile`, `const`, and `static` in Embedded C

**Estimated time: 4–5 hours**
**Tools: VS Code, GCC, Wokwi (Arduino Uno simulation)**
**Save all code to: week1/day2/ in your embedded-journey repo**

---

## WHY THIS DAY MATTERS MORE THAN ALMOST ANY OTHER

Before touching a single line of code today, understand why bit
manipulation is not just a "nice to know" skill in embedded systems —
it is the primary language through which your software speaks to hardware.

Every peripheral on a microcontroller — GPIO, UART, SPI, I2C, ADC,
timers — is controlled by writing specific values to specific bits
inside hardware registers. A register is just a fixed-size variable
(usually 8, 16, or 32 bits wide) mapped to a hardware address.

Here is a real example. On an ATmega328P (the Arduino Uno's chip),
to make pin 13 an output, you write a 1 to bit 5 of the DDRB register:

    DDRB = DDRB | (1 << 5);

To then set pin 13 HIGH:

    PORTB = PORTB | (1 << 5);

To set it LOW:

    PORTB = PORTB & ~(1 << 5);

Every one of those operations is pure bit manipulation. You will write
lines exactly like these starting Week 2. Today you build the foundation
so that syntax is completely transparent to you by then.

---

## PART 1 — NUMBER SYSTEMS (30 minutes)

You need to be comfortable in three number systems simultaneously:
decimal (base 10), binary (base 2), and hexadecimal (base 16).
Embedded code lives in all three at once.

---

### Binary — Base 2

Binary uses only two digits: 0 and 1. Each digit is called a bit.
A group of 8 bits is called a byte.

The positional value of each bit doubles as you move left:

```
Bit position:  7     6     5     4     3     2     1     0
Place value:  128    64    32    16     8     4     2     1

Example byte:   1     0     1     1     0     0     1     0
```

To convert to decimal: multiply each bit by its place value and sum:

    1×128 + 0×64 + 1×32 + 1×16 + 0×8 + 0×4 + 1×2 + 0×1
    = 128 + 32 + 16 + 2
    = 178

In C, binary literals use the 0b prefix:

    uint8_t value = 0b10110010;   // same as 178 in decimal

---

### Hexadecimal — Base 16

Hex uses 16 digits: 0–9 and A–F, where A=10, B=11, C=12, D=13,
E=14, F=15.

Why hex? Because one hex digit maps to exactly 4 bits.
Two hex digits represent exactly one byte.
This makes hex a compact, readable way to express binary values.

```
Binary:   1011  0010
Hex:        B     2    →  0xB2
```

Conversion table (memorise the 0–15 range):

```
Decimal  Binary   Hex
  0      0000      0
  1      0001      1
  2      0010      2
  3      0011      3
  4      0100      4
  5      0101      5
  6      0110      6
  7      0111      7
  8      1000      8
  9      1001      9
 10      1010      A
 11      1011      B
 12      1100      C
 13      1101      D
 14      1110      E
 15      1111      F
```

In C, hex literals use the 0x prefix:

    uint8_t reg = 0xB2;    // same as 0b10110010, same as 178

Hardware register addresses in embedded code are almost always
expressed in hex:

    #define PORTB  (*(volatile uint8_t *)0x25)
    #define GPIOA_ODR  (*(volatile uint32_t *)0x40020014)

---

### Practice — Convert These Before Continuing

Work these out by hand. No calculator.

1. Binary to decimal:   0b00101101
2. Binary to hex:       0b11001010
3. Hex to binary:       0x3F
4. Hex to decimal:      0xA8
5. Decimal to binary:   47

Answers:
1. 45
2. 0xCA
3. 0b00111111
4. 168
5. 0b00101111

If any of those took more than 30 seconds, spend more time on the
conversion table. Fluency here makes everything downstream faster.

---

## PART 2 — BITWISE OPERATORS (90 minutes)

There are six bitwise operators in C. You must know all six perfectly.

```
&    AND
|    OR
^    XOR
~    NOT (bitwise complement)
<<   Left shift
>>   Right shift
```

Each one operates on individual bits, independently, across the
entire width of the value. Think of them as six different tools,
each doing a different job on bits.

---

### The AND Operator: `&`

**Rule:** Output bit is 1 only if BOTH input bits are 1.

```
0 & 0 = 0
0 & 1 = 0
1 & 0 = 0
1 & 1 = 1
```

**Full byte example:**

```
  0b10110111    (0xB7)
& 0b00001111    (0x0F)
= 0b00000111    (0x07)
```

Work through each column top to bottom:
- Bit 7: 1 & 0 = 0
- Bit 6: 0 & 0 = 0
- Bit 5: 1 & 0 = 0
- Bit 4: 1 & 0 = 0
- Bit 3: 0 & 1 = 0
- Bit 2: 1 & 1 = 1
- Bit 1: 1 & 1 = 1
- Bit 0: 1 & 1 = 1

**Embedded use case: READING / CLEARING specific bits (masking)**

AND with a "mask" — a value where the bits you care about are 1,
everything else is 0. The AND operation:
- Preserves bits where the mask has 1
- Forces bits to 0 where the mask has 0

This is called masking or clearing bits.

Real embedded example — check if bit 5 of a status register is set:

```c
uint8_t status = UCSR0A;              // read UART status register
if (status & (1 << 5)) {             // is bit 5 set?
    // UART transmit buffer is empty — safe to send data
}
```

The mask `(1 << 5)` is `0b00100000`. AND-ing with it preserves only
bit 5 and zeros everything else. The if-condition is true only when
bit 5 was 1.

---

### The OR Operator: `|`

**Rule:** Output bit is 1 if EITHER input bit is 1.

```
0 | 0 = 0
0 | 1 = 1
1 | 0 = 1
1 | 1 = 1
```

**Full byte example:**

```
  0b10110000    (0xB0)
| 0b00001101    (0x0D)
= 0b10111101    (0xBD)
```

**Embedded use case: SETTING specific bits**

OR with a mask where the bits you want to SET are 1. The OR operation:
- Forces bits to 1 where the mask has 1
- Preserves all other bits unchanged

Real embedded example — set pin 5 as output (write 1 to DDRB bit 5):

```c
DDRB = DDRB | (1 << 5);
```

The mask `(1 << 5)` is `0b00100000`.
This sets bit 5 to 1 without disturbing bits 0–4 and 6–7.

The shorthand form — you will see this constantly:

```c
DDRB |= (1 << 5);   // identical to above
```

---

### The XOR Operator: `^`

**Rule:** Output bit is 1 if inputs are DIFFERENT. 0 if they are the same.

```
0 ^ 0 = 0
0 ^ 1 = 1
1 ^ 0 = 1
1 ^ 1 = 0
```

**Full byte example:**

```
  0b10110110
^ 0b00100100
= 0b10010010
```

**Embedded use case: TOGGLING specific bits**

XOR with a mask where the bits you want to toggle are 1. The XOR:
- Flips bits where the mask has 1 (0→1, 1→0)
- Preserves all other bits unchanged

Real embedded example — toggle an LED on pin 5:

```c
PORTB ^= (1 << 5);   // if pin was HIGH, now LOW; if LOW, now HIGH
```

This is the cleanest way to blink an LED in a timer interrupt —
one line, no if-statement, no state variable needed.

---

### The NOT Operator: `~`

**Rule:** Inverts every single bit. 0 becomes 1, 1 becomes 0.

```
~0b10110010 = 0b01001101
~0xFF       = 0x00
~0x00       = 0xFF
```

NOT is a unary operator — it operates on one value, not two.

**Embedded use case: CLEARING specific bits**

NOT is almost always combined with AND. The pattern is:

```c
register &= ~(1 << n);   // clear bit n
```

Breaking it down:

```
(1 << 5)    = 0b00100000    (only bit 5 is 1)
~(1 << 5)   = 0b11011111    (only bit 5 is 0, everything else 1)
reg & 0b11011111            (clears bit 5, preserves all others)
```

Real embedded example — set pin 5 LOW (clear bit 5 of PORTB):

```c
PORTB &= ~(1 << 5);
```

This is the canonical embedded pattern for clearing a single bit
without disturbing any other bits. Memorise it.

---

### Left Shift Operator: `<<`

**Rule:** Shift all bits left by N positions. Zeros fill from the right.
Bits shifted past the left edge are discarded.

```
0b00000001 << 1  =  0b00000010   (1 shifted left = 2)
0b00000001 << 3  =  0b00001000   (1 shifted left 3 = 8)
0b00000001 << 5  =  0b00100000   (1 shifted left 5 = 32)
```

Left shifting by N is mathematically equivalent to multiplying by 2^N.

**Critical embedded use: building bit masks**

This is why you constantly see `(1 << n)` in embedded code — it is
the universal way to say "I want a mask with only bit n set."

```c
(1 << 0) = 0b00000001   // bit 0 mask
(1 << 1) = 0b00000010   // bit 1 mask
(1 << 2) = 0b00000100   // bit 2 mask
(1 << 3) = 0b00001000   // bit 3 mask
(1 << 4) = 0b00010000   // bit 4 mask
(1 << 5) = 0b00100000   // bit 5 mask
(1 << 6) = 0b01000000   // bit 6 mask
(1 << 7) = 0b10000000   // bit 7 mask
```

Instead of writing cryptic magic numbers like `0x20` or `32`, you
write `(1 << 5)` and any engineer instantly knows you mean bit 5.
Readability is not optional in embedded code that others maintain.

---

### Right Shift Operator: `>>`

**Rule:** Shift all bits right by N positions. For unsigned types,
zeros fill from the left.

```
0b10000000 >> 1  =  0b01000000
0b10000000 >> 3  =  0b00010000
0b00101100 >> 2  =  0b00001011
```

Right shifting by N is equivalent to dividing by 2^N (for unsigned).

**Embedded use case: extracting multi-bit fields**

Hardware registers often pack multiple fields into one register.
For example, a 16-bit ADC result register where bits 15–4 hold the
12-bit result and bits 3–0 are status flags:

```c
uint16_t raw = ADC_RESULT_REG;         // read full register
uint16_t adc_value = (raw >> 4);       // shift right 4 to get result
uint8_t  flags     = (raw & 0x0F);     // AND to isolate lower 4 bits
```

---

## PART 3 — THE THREE CANONICAL EMBEDDED PATTERNS (30 minutes)

Every register manipulation in embedded C reduces to one of these three
patterns. Learn them until they are reflexive.

---

### Pattern 1: Set a bit (force to 1)

```c
register |= (1 << bit_position);
```

Example: set bit 3

```
register = 0b10100010
mask     = 0b00001000   (1 << 3)
result   = 0b10101010   (bit 3 is now 1, nothing else changed)
```

---

### Pattern 2: Clear a bit (force to 0)

```c
register &= ~(1 << bit_position);
```

Example: clear bit 5

```
register = 0b10100010
mask     = 0b00100000   (1 << 5)
~mask    = 0b11011111
result   = 0b10000010   (bit 5 is now 0, nothing else changed)
```

---

### Pattern 3: Toggle a bit (flip current state)

```c
register ^= (1 << bit_position);
```

Example: toggle bit 1

```
register = 0b10100010
mask     = 0b00000010   (1 << 1)
result   = 0b10100000   (bit 1 flipped from 1 to 0)

run again:
register = 0b10100000
mask     = 0b00000010
result   = 0b10100010   (bit 1 flipped back to 1)
```

---

### Bonus Pattern: Check if a bit is set

```c
if (register & (1 << bit_position)) {
    // bit is 1
}
```

The AND isolates just that bit. If it was 1, the result is non-zero
(truthy in C). If it was 0, the result is zero (falsy).

---

## PART 4 — CODE: BIT MANIPULATION PROGRAMS

---

### Program 1: All six operators demonstrated

Save as week1/day2/bitwise_operators.c

```c
#include <stdio.h>
#include <stdint.h>

void print_binary(uint8_t value) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i == 4) printf(" ");
    }
    printf("  (0x%02X = %u)", value, value);
}

int main(void) {
    uint8_t a = 0b10110110;
    uint8_t b = 0b01101011;

    printf("a = "); print_binary(a); printf("\n");
    printf("b = "); print_binary(b); printf("\n\n");

    printf("a & b  = "); print_binary(a & b); printf("  (AND)\n");
    printf("a | b  = "); print_binary(a | b); printf("  (OR)\n");
    printf("a ^ b  = "); print_binary(a ^ b); printf("  (XOR)\n");
    printf("~a     = "); print_binary(~a);    printf("  (NOT a)\n");
    printf("a << 2 = "); print_binary(a << 2); printf("  (left shift 2)\n");
    printf("a >> 2 = "); print_binary(a >> 2); printf("  (right shift 2)\n");

    return 0;
}
```

WHAT TO OBSERVE:
- Work out each result by hand before running the program
- Verify your manual answers against the output
- Notice how `a << 2` drops the two leftmost bits and fills zeros on the right
- Notice how `~a` flips every single bit

The print_binary function is itself a bit manipulation exercise.
Trace through it: `(value >> i) & 1` shifts bit i down to position 0,
then ANDs with 1 to isolate just that bit. The result is always 0 or 1.
This is how you extract and print individual bits.

---

### Program 2: The three canonical patterns in action

Save as week1/day2/register_patterns.c

```c
#include <stdio.h>
#include <stdint.h>

void print_binary(uint8_t value) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf(" (0x%02X)", value);
}

int main(void) {
    uint8_t reg = 0b10100010;

    printf("Initial register:  "); print_binary(reg); printf("\n\n");

    reg |= (1 << 3);
    printf("After SET bit 3:   "); print_binary(reg); printf("\n");

    reg &= ~(1 << 5);
    printf("After CLEAR bit 5: "); print_binary(reg); printf("\n");

    reg ^= (1 << 1);
    printf("After TOGGLE bit 1:"); print_binary(reg); printf("\n");
    reg ^= (1 << 1);
    printf("Toggle bit 1 again:"); print_binary(reg); printf("\n\n");

    printf("Checking individual bits:\n");
    for (int i = 7; i >= 0; i--) {
        printf("  Bit %d is %s\n", i,
               (reg & (1 << i)) ? "SET (1)" : "CLEAR (0)");
    }

    return 0;
}
```

WHAT TO OBSERVE:
- Trace each operation manually before running
- Confirm that setting bit 3 only changed bit 3
- Confirm clearing bit 5 only changed bit 5
- The bit-check loop at the bottom shows how to inspect every bit
  systematically — a pattern used in hardware diagnostic routines

---

### Program 3: Simulating a GPIO register

This is the most important program today. It models exactly what
happens inside a microcontroller when you manipulate GPIO pins.

Save as week1/day2/gpio_simulation.c

```c
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Simulated ATmega328P GPIO registers for PORTB
 *
 * DDRB  - Data Direction Register B
 *         Bit = 0 means INPUT
 *         Bit = 1 means OUTPUT
 *
 * PORTB - Port B Data Register
 *         For OUTPUT pins: bit = 0 means LOW (0V)
 *                          bit = 1 means HIGH (5V)
 *         For INPUT pins:  bit = 1 enables internal pull-up
 *
 * PINB  - Port B Input Pins Register (read-only on real hardware)
 *         Reflects actual voltage on each pin
 */

static uint8_t DDRB  = 0x00;   // all inputs by default
static uint8_t PORTB = 0x00;   // all low
static uint8_t PINB  = 0x00;   // simulated input state

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
    printf("  DDRB  = ");
    for (int i = 7; i >= 0; i--) printf("%d", (DDRB  >> i) & 1);
    printf("  (1=output, 0=input)\n");

    printf("  PORTB = ");
    for (int i = 7; i >= 0; i--) printf("%d", (PORTB >> i) & 1);
    printf("  (output state)\n");

    printf("  PINB  = ");
    for (int i = 7; i >= 0; i--) printf("%d", (PINB  >> i) & 1);
    printf("  (actual pin voltage)\n");
}

int main(void) {
    printf("=== Initial State ===\n");
    print_register_state();

    printf("\n=== Configure: pin 5 = output (LED), pin 2 = input (button) ===\n");
    gpio_set_output(5);
    gpio_set_input(2);
    print_register_state();

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
    printf("Button pressed:   pin 2 reads %s\n",
           gpio_read(2) ? "HIGH" : "LOW");

    simulate_input_low(2);
    printf("Button released:  pin 2 reads %s\n",
           gpio_read(2) ? "HIGH" : "LOW");

    print_register_state();

    return 0;
}
```

WHAT TO OBSERVE:
- Every function body is a single bit manipulation line
- gpio_set_output sets a bit. gpio_set_input clears a bit.
  gpio_write_high sets. gpio_write_low clears. gpio_toggle XORs.
  These are the exact same patterns from Part 3.
- On a real ATmega328P microcontroller, your Week 2 code will write
  to the ACTUAL DDRB and PORTB registers at their real hardware
  addresses. The logic is identical to what you just wrote.
- Notice that DDRB and PORTB are separate concerns:
  DDRB controls DIRECTION (input or output)
  PORTB controls OUTPUT STATE (high or low)
  A common beginner mistake is writing to PORTB before setting DDRB.

---

### Program 4: Ten bit-twiddling exercises

This is your drill set. Write these yourself — do not skip them.

Save as week1/day2/bit_exercises.c

```c
#include <stdio.h>
#include <stdint.h>

void print_binary(uint8_t v) {
    for (int i = 7; i >= 0; i--) printf("%d", (v >> i) & 1);
    printf(" (0x%02X)", v);
}

int main(void) {
    uint8_t reg;

    /* Exercise 1: Set bit 3 of 0b10100000 */
    reg = 0b10100000;
    reg |= (1 << 3);
    printf("Ex1 Set bit 3:    "); print_binary(reg);
    printf(" (expected: 10101000)\n");

    /* Exercise 2: Clear bit 7 of 0b11110000 */
    reg = 0b11110000;
    reg &= ~(1 << 7);
    printf("Ex2 Clear bit 7:  "); print_binary(reg);
    printf(" (expected: 01110000)\n");

    /* Exercise 3: Toggle bit 4 of 0b00001111 */
    reg = 0b00001111;
    reg ^= (1 << 4);
    printf("Ex3 Toggle bit 4: "); print_binary(reg);
    printf(" (expected: 00011111)\n");

    /* Exercise 4: Set bits 0 and 7 simultaneously */
    reg = 0b01000110;
    reg |= (1 << 0) | (1 << 7);
    printf("Ex4 Set 0 and 7:  "); print_binary(reg);
    printf(" (expected: 11000111)\n");

    /* Exercise 5: Clear bits 2 and 5 simultaneously */
    reg = 0b11111111;
    reg &= ~((1 << 2) | (1 << 5));
    printf("Ex5 Clear 2,5:    "); print_binary(reg);
    printf(" (expected: 11011011)\n");

    /* Exercise 6: Check if bit 6 is set — print result */
    reg = 0b11000000;
    printf("Ex6 Bit 6 set?    %s (expected: YES)\n",
           (reg & (1 << 6)) ? "YES" : "NO");

    /* Exercise 7: Extract the lower nibble (bits 3:0) of 0xAB */
    reg = 0xAB;
    uint8_t lower = reg & 0x0F;
    printf("Ex7 Lower nibble: "); print_binary(lower);
    printf(" (expected: 00001011)\n");

    /* Exercise 8: Extract the upper nibble (bits 7:4) of 0xAB, shift down */
    reg = 0xAB;
    uint8_t upper = (reg >> 4) & 0x0F;
    printf("Ex8 Upper nibble: "); print_binary(upper);
    printf(" (expected: 00001010)\n");

    /* Exercise 9: Swap the upper and lower nibbles of 0xAB */
    reg = 0xAB;
    uint8_t swapped = ((reg & 0x0F) << 4) | ((reg >> 4) & 0x0F);
    printf("Ex9 Nibble swap:  "); print_binary(swapped);
    printf(" (expected: 10111010)\n");

    /* Exercise 10: Count the number of set bits in a byte (population count) */
    reg = 0b10110101;
    uint8_t count = 0;
    uint8_t temp = reg;
    while (temp) {
        count += temp & 1;
        temp >>= 1;
    }
    printf("Ex10 Set bits in "); print_binary(reg);
    printf(":\n     Count = %u (expected: 5)\n", count);

    return 0;
}
```

Work through each exercise manually with pencil and paper first.
Write out the binary, do the operation bit by bit, then run the
program and see if you were right.

Exercises 9 and 10 are deliberately harder.
For exercise 9, think: how do you isolate each nibble separately,
reposition it, then combine them?
For exercise 10, trace the loop: what does `temp & 1` give you
each iteration? What does `temp >>= 1` do to the remaining bits?

---

## PART 5 — `volatile`, `const`, AND `static` (45 minutes)

These three keywords appear in almost every embedded C file you will
ever read. Each one solves a specific problem that only exists because
you are programming hardware, not a desktop application.

---

### `volatile` — The Most Important Embedded Keyword

**The problem it solves:**

Modern C compilers are optimising compilers. When the compiler sees
code like this:

```c
uint8_t status = UART_STATUS_REG;
while (status == 0) {
    status = UART_STATUS_REG;   // keep reading until non-zero
}
```

The compiler sees that status is being read from the same address in
a loop. It "optimises" by assuming: "the value at that address cannot
change between reads (nothing in my code changes it), so I'll just
keep the value in a CPU register and skip the memory read entirely."

The compiled code becomes:

```c
uint8_t status = UART_STATUS_REG;
while (status == 0) {
    // UART_STATUS_REG read is eliminated — compiler cached the value!
}
```

This is an infinite loop. The program never detects the UART event.

Why is the compiler wrong? Because hardware changes that memory
address from the *outside* — the UART peripheral writes to its own
status register independently of your CPU. The compiler has no way
to know this.

**The solution:**

```c
volatile uint8_t status = UART_STATUS_REG;
```

`volatile` is a promise to the compiler: "this value can change at
any time from outside your view — never cache it, never skip a read,
always go to the actual memory address."

**Rule: every hardware register access must use volatile.**

```c
#define PORTB  (*(volatile uint8_t *)0x25)
```

**The second use case — ISR shared variables:**

Variables shared between an interrupt service routine and main code
must be volatile. The interrupt fires asynchronously — the compiler
cannot see this happening. Without volatile, it may cache the
variable's value and never see the ISR's update.

```c
volatile bool button_pressed = false;   // shared with ISR

ISR(INT0_vect) {
    button_pressed = true;   // ISR sets the flag
}

int main(void) {
    while (!button_pressed) {
        // without volatile, compiler may cache button_pressed
        // and loop forever even after ISR sets it to true
    }
    // respond to button
}
```

**The rule in one sentence:** Any variable whose value can change
outside the normal execution flow of your program — hardware register,
ISR-modified variable, DMA-modified buffer — must be declared volatile.

---

### `const` — Protecting Values from Accidental Modification

`const` tells the compiler: this value must not be modified after
initialisation. Any attempt to modify it is a compile-time error.

**Basic usage:**

```c
const float PI = 3.14159f;
const uint8_t MAX_RETRIES = 5;

PI = 3.0f;          // COMPILE ERROR — cannot modify const
MAX_RETRIES = 10;   // COMPILE ERROR
```

**The embedded use case — const pointers in function parameters:**

When you pass a pointer to a function, you give that function the
power to modify your data. Sometimes that is intentional.
Sometimes it is dangerous. `const` enforces the intention:

```c
void transmit_packet(const uint8_t *data, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        uart_send_byte(data[i]);
        data[i] = 0;    // COMPILE ERROR — data is const, cannot write through it
    }
}
```

The const in the parameter says: "this function is allowed to READ
through this pointer but not WRITE through it." Any accidental write
is caught at compile time, not at runtime. In embedded systems where
a bug does not crash a program gracefully but corrupts hardware state,
catching errors at compile time is enormously valuable.

**`const` data in flash memory (important for microcontrollers):**

On microcontrollers, RAM is precious (often 2–32KB) but flash is
larger (often 32–512KB). Constant data — lookup tables, font bitmaps,
string messages — can be stored in flash instead of copied to RAM.

```c
// AVR specific — PROGMEM stores in flash, not RAM
const uint8_t SINE_TABLE[256] PROGMEM = {
    0, 3, 6, 9, 12, ...
};
```

A 256-byte lookup table stored in flash saves 256 bytes of RAM —
potentially 12% of an Arduino Uno's total RAM. This matters.

---

### `static` — Three Different Jobs, One Keyword

`static` is context-dependent. It means different things in different
locations. This confuses beginners. Learn all three uses.

**Use 1: Static local variable — persists between function calls**

```c
uint32_t get_next_id(void) {
    static uint32_t counter = 0;   // initialised ONCE, lives in .bss
    counter++;
    return counter;
}

int main(void) {
    printf("%u\n", get_next_id());   // prints 1
    printf("%u\n", get_next_id());   // prints 2
    printf("%u\n", get_next_id());   // prints 3
}
```

Without static, counter would reset to 0 every call. With static,
it retains its value. The variable lives in the .bss section (as you
learnt in Day 1) rather than the stack. It is initialised once at
program start.

Embedded use case: millisecond tick counters, state machines,
debounce timers — any function that needs to remember something
between calls without polluting global scope.

**Use 2: Static global variable — file-private, hidden from linker**

```c
// uart.c
static uint8_t tx_buffer[64];       // only visible inside uart.c
static uint8_t tx_head = 0;
static uint8_t tx_tail = 0;

void uart_init(void) { ... }        // visible externally
void uart_send(uint8_t byte) { ... } // visible externally
```

Without static, tx_buffer is a global that any other .c file could
accidentally access or name-clash with. With static, it is completely
hidden from the rest of the program. This is the embedded equivalent
of private class members in C++. Every internal variable in a module
that outsiders should not touch gets static.

**Use 3: Static function — file-private function**

```c
// uart.c
static void flush_tx_buffer(void) {   // internal helper, not callable externally
    tx_head = tx_tail = 0;
}

void uart_reset(void) {
    flush_tx_buffer();   // called internally — fine
}
```

Other .c files cannot call flush_tx_buffer. It is implementation
detail, not public interface.

---

## PART 6 — PUTTING IT TOGETHER IN WOKWI (45 minutes)

Now simulate an Arduino Uno on Wokwi with real bit manipulation.

**Steps to set up the simulation:**

1. Open https://wokwi.com
2. Click "New Project" → "Arduino Uno"
3. In the diagram editor, add:
   - An LED connected to pin 13 (built-in, already there)
   - A push button connected to pin 2 with a 10kΩ pull-up to 5V
4. Click the code editor and replace everything with the code below

**Wokwi Sketch — Bare-Metal Register Control (no digitalWrite)**

```cpp
#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN    5   // PB5 = Arduino pin 13
#define BUTTON_PIN 2   // PD2 = Arduino pin 2

void gpio_init(void) {
    DDRB |=  (1 << LED_PIN);     // set PB5 as output
    DDRD &= ~(1 << BUTTON_PIN);  // set PD2 as input
    PORTD |= (1 << BUTTON_PIN);  // enable pull-up on PD2
}

uint8_t button_is_pressed(void) {
    return !(PIND & (1 << BUTTON_PIN));  // active low (pull-up)
}

void led_on(void)     { PORTB |=  (1 << LED_PIN); }
void led_off(void)    { PORTB &= ~(1 << LED_PIN); }
void led_toggle(void) { PORTB ^=  (1 << LED_PIN); }

int main(void) {
    gpio_init();

    while (1) {
        if (button_is_pressed()) {
            led_on();
        } else {
            led_off();
        }

        _delay_ms(10);   // simple debounce
    }

    return 0;
}
```

WHAT TO OBSERVE:
- Every function body is a single bit manipulation line
- The button uses active-low logic — when pressed, the pin reads 0
  not 1 (because the pull-up holds it HIGH when released, and pressing
  connects it to GND). The ! operator inverts this.
- Compare this to Arduino's `pinMode()` and `digitalWrite()` —
  your code is doing exactly what those functions do, minus the
  overhead and abstraction

**Modify the sketch:**

1. Change it so the button toggles the LED (press once = on,
   press again = off) instead of holding to light
2. Change it so the LED blinks fast when the button is held,
   and blinks slow when it is not

These modifications require you to introduce state — a variable
that remembers whether the LED is currently on or off. Use static.

---

## PART 7 — `volatile` CODE DEMONSTRATION (20 minutes)

This program demonstrates why volatile matters. You cannot easily
demonstrate compiler optimisation removal on a desktop C program
(the effect requires embedded context), but this simulation shows
the correct usage pattern:

Save as week1/day2/volatile_demo.c

```c
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Simulating two hardware status registers.
 * In real embedded code these would be at hardware addresses:
 * volatile uint8_t *STATUS_REG = (volatile uint8_t *)0x40013800;
 */
volatile uint8_t UART_STATUS  = 0x00;
volatile bool    DATA_READY   = false;

void simulate_hardware_update(void) {
    UART_STATUS = 0x20;   // hardware sets bit 5 = TX ready
    DATA_READY  = true;
}

void wait_for_tx_ready(void) {
    /*
     * Without volatile on UART_STATUS, an optimising compiler might
     * read it once, cache it in a register, and loop forever.
     * With volatile it re-reads from memory every iteration.
     */
    while (!(UART_STATUS & (1 << 5))) {
        /* waiting for TX ready bit */
    }
    printf("TX ready — safe to send data\n");
}

int main(void) {
    printf("Simulating hardware register access with volatile\n\n");

    printf("UART_STATUS before: 0x%02X\n", UART_STATUS);
    printf("DATA_READY before:  %s\n\n", DATA_READY ? "true" : "false");

    simulate_hardware_update();

    printf("UART_STATUS after:  0x%02X\n", UART_STATUS);
    printf("DATA_READY after:   %s\n\n", DATA_READY ? "true" : "false");

    wait_for_tx_ready();

    printf("\n=== Demonstrating static local variable ===\n");

    for (int i = 0; i < 5; i++) {
        static uint32_t call_count = 0;
        static bool     first_call = true;

        call_count++;

        if (first_call) {
            printf("First call detected!\n");
            first_call = false;
        }

        printf("Loop iteration %d, total calls so far: %u\n", i, call_count);
    }

    return 0;
}
```

---

## PART 8 — GIT COMMIT (10 minutes)

    cd embedded-journey/week1/day2
    git add .
    git commit -m "Week 1 Day 2: bitwise operators, register patterns, volatile/const/static"
    git push origin main

---

## PART 9 — DAY 2 REVIEW QUESTIONS

Write answers in notes/week1.md — add a Day 2 section.

1. What is the result of `0b11001100 & 0b10101010`? Work it out in
   binary, then convert to hex.

2. What single operation sets bit 6 of a register without
   disturbing any other bit? Write it as a C expression.

3. What single operation clears bit 2 of a register without
   disturbing any other bit? Write it as a C expression.

4. What does `(1 << 4)` evaluate to in decimal? In binary?

5. A function receives a `const uint8_t *buffer` parameter.
   Can it read buffer[3]? Can it write buffer[3] = 0xFF?
   Why or why not?

6. You have a variable `uint8_t flag` shared between an ISR and
   main(). What keyword must you add to its declaration and why?

7. A function is called 100 times. Inside it, there is a
   `static uint16_t count = 0; count++;` declaration.
   What is the value of count after the 100th call?

8. In button_is_pressed() from the Wokwi sketch:
       return !(PIND & (1 << BUTTON_PIN));
   Explain every operator and symbol in this single line.

---

## WHAT YOU ACCOMPLISHED TODAY

- Full command of all 6 bitwise operators — AND, OR, XOR, NOT,
  left shift, right shift
- The three canonical register manipulation patterns memorised
- A simulated GPIO system built entirely from bit operations
- Real register-level Arduino code running on Wokwi
- Deep understanding of volatile (hardware registers, ISR sharing),
  const (write protection, flash storage), and static (persistence,
  encapsulation)

Day 3 covers enums, #define, fixed-width types, and modelling
hardware registers as C structs with bitfields — the last piece
before you write real hardware code in Week 2.
