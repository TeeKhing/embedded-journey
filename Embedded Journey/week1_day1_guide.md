# WEEK 1, DAY 1 — Full Learning Guide
## C Programming Refresher for Embedded Systems

**Estimated time: 4–5 hours**
**Tools needed: VS Code, GCC (compiler), Git, GitHub account, Wokwi (browser)**

---

## PART 1 — TOOL SETUP (60 minutes)

This part is purely administrative but foundational. A broken toolchain
wastes more time than almost anything else. Do this carefully once.

---

### 1A. Install VS Code

Go to https://code.visualstudio.com and download for your OS.

After installing, open it and install these extensions (Ctrl+Shift+X):
- **C/C++** (by Microsoft) — syntax highlighting, IntelliSense, debugging
- **PlatformIO IDE** — embedded build system (takes a few minutes to install)
- **GitLens** — enhanced Git integration

---

### 1B. Install GCC (C Compiler)

You need a C compiler on your computer to run plain C programs
(not just embedded code). This is separate from PlatformIO.

**Windows:**
1. Download MSYS2 from https://www.msys2.org
2. Install it, then open the MSYS2 terminal and run:
   pacman -S mingw-w64-x86_64-gcc
3. Add C:\msys64\mingw64\bin to your Windows PATH

**Ubuntu/Debian Linux:**
   sudo apt update && sudo apt install gcc build-essential

**macOS:**
   xcode-select --install

Verify installation by opening a terminal and typing:
   gcc --version

You should see something like: gcc (GCC) 13.2.0

---

### 1C. Install Git

Download from https://git-scm.com

After installing, open a terminal and configure your identity:

   git config --global user.name "Your Name"
   git config --global user.email "youremail@example.com"

---

### 1D. Create Your GitHub Repository

1. Go to https://github.com and create a free account if you don't have one
2. Click the "+" icon → "New repository"
3. Name it: embedded-journey
4. Set it to Public (this is your portfolio — it needs to be visible)
5. Check "Add a README file"
6. Click "Create repository"

Now clone it to your computer:

   git clone https://github.com/YOUR_USERNAME/embedded-journey.git
   cd embedded-journey

Create your folder structure:

   mkdir -p week1/day1
   mkdir notes

This folder structure will hold all your code for the 26 weeks.
Every single program you write goes into this repository.

---

### 1E. Set Up Wokwi

Go to https://wokwi.com — no installation needed, runs in the browser.

- Create a free account
- Click "New Project" → select "Arduino Uno" to verify it works
- Bookmark it

Wokwi is your simulated hardware lab for the first several weeks.

---

## PART 2 — WHY C FOR EMBEDDED SYSTEMS (20 minutes)

Before writing a single line of code, understand why you are learning C
and not Python or JavaScript.

### The Core Reason: C Gives You Direct Hardware Access

When you write Python, you are separated from the hardware by multiple
layers: Python interpreter → operating system → hardware drivers → hardware.
Each layer adds overhead and removes control.

When you write C for a microcontroller, you are one step from the metal:
   Your C code → compiler → machine code → runs directly on CPU

There is no operating system in the middle on a bare-metal microcontroller.
There is no garbage collector cleaning up your memory. There is no virtual
machine interpreting your code. You write instructions, the CPU executes
them — period.

### What This Means in Practice

A blink-an-LED program in bare-metal C might look like this
(you will understand every part of this by Week 2):

   PORTB |= (1 << PB5);   // set pin 13 HIGH

This single line writes a 1 to bit 5 of the PORTB register — a physical
memory address that is wired directly to pin 13 on the Arduino. There is
no function call overhead, no driver translation, no OS system call.
It takes exactly 1 or 2 clock cycles.

The equivalent Arduino function:

   digitalWrite(13, HIGH);

This calls a function that reads a lookup table, checks if the pin is
valid, computes the register and bit mask, then writes to it. It takes
~50 clock cycles. On a 16MHz Arduino, that's 3 microseconds of overhead
per call — meaningless for an LED, catastrophic for timing-sensitive
motor control or communication protocols.

This is why C. Not because it is easy. Because nothing else gives you
this level of control at this level of performance.

---

## PART 3 — POINTERS (60 minutes)

Pointers are the most important concept in embedded C. They are also
the concept that breaks the most beginners. Read this section slowly.

---

### What Is a Pointer?

Every variable you declare in C occupies memory. Memory is a long array
of bytes, and each byte has an address — a number that identifies its
location. Think of RAM as a street, and addresses as house numbers.

A pointer is a variable that stores a memory address.
That's the entire definition. Nothing more.

   int temperature = 25;

When you write this, the compiler:
1. Allocates 4 bytes of memory somewhere (say, at address 0x20000100)
2. Stores the value 25 at that address
3. Associates the name "temperature" with that address

   int *ptr = &temperature;

The & operator means "give me the address of this variable."
So ptr now holds the value 0x20000100 — the address where temperature lives.
The * in the declaration means "this variable holds an address, not a value."

   printf("%d\n", *ptr);   // prints 25

The * operator here means "go to the address stored in ptr and read what's
there." This is called dereferencing. It reads the value at the address.

---

### Visual Diagram of Memory

Imagine RAM like this:

   Address     | Value  | Name
   ------------|--------|------------------
   0x20000100  |   25   | temperature
   0x20000104  | 0x20000100 | ptr (stores the address!)
   0x20000108  |   ?    | (something else)

ptr is at address 0x20000104.
ptr's value is 0x20000100 (the address of temperature).
*ptr reads the value at 0x20000100, which is 25.

---

### Why Pointers Matter in Embedded Systems

**Reason 1: Accessing hardware registers**

In embedded systems, hardware peripherals (GPIO, UART, timers) are
controlled by writing to specific memory addresses. The only way to do
this in C is with pointers.

   #define PORTB  (*(volatile uint8_t *)0x25)

This line says: "treat address 0x25 as a pointer to a byte, and dereference
it immediately." Writing to PORTB writes to the physical GPIO register.
You will use patterns exactly like this starting Week 2.

**Reason 2: Passing large data without copying**

If you have a 256-byte sensor data array, passing it to a function copies
all 256 bytes onto the stack. Passing a pointer to it copies 4 bytes (just
the address). In memory-constrained microcontrollers, this difference matters.

**Reason 3: Dynamic data structures**

Linked lists, queues, buffers — all use pointers to chain together
blocks of memory. The circular buffer you'll build today uses this.

---

### Pointer Code — Study These Programs

**Program 1: Basic pointer operations**

Save this as week1/day1/pointers_basic.c

```c
#include <stdio.h>

int main(void) {
    int temperature = 25;
    int humidity = 60;

    int *ptr;

    ptr = &temperature;
    printf("Address of temperature: %p\n", (void *)ptr);
    printf("Value at that address:  %d\n", *ptr);

    ptr = &humidity;
    printf("Address of humidity:    %p\n", (void *)ptr);
    printf("Value at that address:  %d\n", *ptr);

    *ptr = 75;
    printf("humidity after *ptr=75: %d\n", humidity);

    return 0;
}
```

Compile and run:
   gcc pointers_basic.c -o pointers_basic
   ./pointers_basic

WHAT TO OBSERVE: The address printed for temperature and humidity will
differ by 4 bytes (because int is 4 bytes). When you write *ptr = 75,
you are directly modifying humidity through its address — and the
printf confirms humidity changed even though you never wrote "humidity = 75".

---

**Program 2: Pointers and functions — passing by reference**

```c
#include <stdio.h>

void celsius_to_fahrenheit(float *celsius, float *result) {
    *result = (*celsius * 9.0f / 5.0f) + 32.0f;
}

int main(void) {
    float temp_c = 100.0f;
    float temp_f = 0.0f;

    celsius_to_fahrenheit(&temp_c, &temp_f);

    printf("%.1f C = %.1f F\n", temp_c, temp_f);

    return 0;
}
```

WHAT TO OBSERVE: celsius_to_fahrenheit takes no return value (void).
Instead it writes its result directly into temp_f through a pointer.
This is a fundamental embedded pattern — functions modify data in-place
rather than returning copies. It is more memory efficient.

---

**Program 3: Pointer arithmetic (critical for arrays and buffers)**

```c
#include <stdio.h>

int main(void) {
    int sensor_readings[5] = {10, 20, 30, 40, 50};
    int *ptr = sensor_readings;

    printf("ptr points to first element: %d\n", *ptr);

    ptr++;
    printf("After ptr++, value is: %d\n", *ptr);

    ptr += 2;
    printf("After ptr+=2, value is: %d\n", *ptr);

    printf("\nIterating array with pointer:\n");
    ptr = sensor_readings;
    for (int i = 0; i < 5; i++) {
        printf("  sensor_readings[%d] = %d  (address: %p)\n",
               i, *ptr, (void *)ptr);
        ptr++;
    }

    return 0;
}
```

WHAT TO OBSERVE: ptr++ does not add 1 to the address. It adds sizeof(int)
bytes (4 bytes on most systems). This is pointer arithmetic. The pointer
always knows the size of what it points to and jumps by that many bytes.
This is why you must always know your data types in embedded systems.

---

## PART 4 — ARRAYS (30 minutes)

Arrays in C are pointers in disguise. This is a crucial insight.

### Arrays Are Contiguous Blocks of Memory

When you declare:

   int readings[8];

C allocates 8 × 4 = 32 bytes of contiguous memory and gives you the
name "readings" to refer to the start address of that block.

readings is equivalent to &readings[0].
readings[3] is equivalent to *(readings + 3).

These two lines do identical things:

   int val1 = readings[3];
   int val2 = *(readings + 3);

---

**Array Code — Study These Programs**

**Program 4: Arrays and their memory layout**

```c
#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint8_t sensor_buffer[8] = {0xAA, 0xBB, 0x01, 0x02,
                                 0x03, 0x04, 0x05, 0xFF};

    printf("Array name as address: %p\n", (void *)sensor_buffer);
    printf("Address of [0]:        %p\n", (void *)&sensor_buffer[0]);

    printf("\nBuffer contents:\n");
    for (int i = 0; i < 8; i++) {
        printf("  [%d] addr=%p  value=0x%02X\n",
               i, (void *)&sensor_buffer[i], sensor_buffer[i]);
    }

    return 0;
}
```

Note: uint8_t requires #include <stdint.h>. Add it at the top.
You will use uint8_t constantly in embedded code — it guarantees an
8-bit unsigned integer regardless of the platform.

WHAT TO OBSERVE: Every address differs by exactly 1 byte because
uint8_t is 1 byte wide. Compare this to the int array in Program 3
where addresses differed by 4 bytes.

---

**Program 5: Passing arrays to functions**

```c
#include <stdio.h>
#include <stdint.h>

void compute_average(uint16_t *data, uint8_t length, uint16_t *result) {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < length; i++) {
        sum += data[i];
    }
    *result = (uint16_t)(sum / length);
}

int main(void) {
    uint16_t adc_readings[6] = {1023, 987, 1010, 990, 1005, 1001};
    uint16_t average = 0;

    compute_average(adc_readings, 6, &average);

    printf("Average ADC reading: %u\n", average);

    return 0;
}
```

WHAT TO OBSERVE: compute_average receives the array as a pointer.
It does not know or care how large the array is — that is why length
is passed separately. This is a universal embedded pattern. Arrays
always travel with their length.

---

## PART 5 — STRUCTS (30 minutes)

A struct groups related variables under one name. In embedded systems,
structs represent hardware register layouts, sensor data packets,
configuration blocks, and message frames.

### Why Structs Matter in Embedded

Instead of managing separate variables:

   float temperature;
   float humidity;
   uint32_t timestamp;
   uint8_t sensor_id;

You group them into a meaningful unit:

   typedef struct {
       float temperature;
       float humidity;
       uint32_t timestamp;
       uint8_t sensor_id;
   } SensorReading;

Now you can pass one SensorReading to a function instead of four
separate arguments. You can have an array of SensorReading. You can
write it to flash or transmit it over UART as a single block of bytes.

---

**Struct Code — Study These Programs**

**Program 6: Basic struct usage**

```c
#include <stdio.h>
#include <stdint.h>

typedef struct {
    float    temperature;
    float    humidity;
    uint32_t timestamp_ms;
    uint8_t  sensor_id;
} SensorReading;

void print_reading(const SensorReading *reading) {
    printf("Sensor ID  : %u\n",   reading->sensor_id);
    printf("Timestamp  : %u ms\n", reading->timestamp_ms);
    printf("Temperature: %.2f C\n", reading->temperature);
    printf("Humidity   : %.2f %%\n", reading->humidity);
}

int main(void) {
    SensorReading r1 = {
        .temperature  = 28.5f,
        .humidity     = 65.3f,
        .timestamp_ms = 5000,
        .sensor_id    = 1
    };

    SensorReading r2 = {
        .temperature  = 31.2f,
        .humidity     = 70.1f,
        .timestamp_ms = 10000,
        .sensor_id    = 2
    };

    printf("=== Reading 1 ===\n");
    print_reading(&r1);

    printf("\n=== Reading 2 ===\n");
    print_reading(&r2);

    printf("\nSize of SensorReading struct: %zu bytes\n", sizeof(SensorReading));

    return 0;
}
```

WHAT TO OBSERVE:
- The -> operator accesses struct members through a pointer.
  reading->sensor_id is identical to (*reading).sensor_id
- We pass const SensorReading *reading — a pointer to avoid copying
  the whole struct, plus const to promise we won't modify it
- The designated initializer syntax (.temperature = 28.5f) is
  preferred in embedded code for clarity and order-independence

---

**Program 7: Array of structs — a sensor log**

```c
#include <stdio.h>
#include <stdint.h>

typedef struct {
    float    temperature;
    uint32_t timestamp_ms;
} TempLog;

void find_max_temp(const TempLog *log, uint8_t count,
                   float *max_temp, uint32_t *max_time) {
    *max_temp = log[0].temperature;
    *max_time = log[0].timestamp_ms;

    for (uint8_t i = 1; i < count; i++) {
        if (log[i].temperature > *max_temp) {
            *max_temp = log[i].temperature;
            *max_time = log[i].timestamp_ms;
        }
    }
}

int main(void) {
    TempLog readings[5] = {
        {.temperature = 27.1f, .timestamp_ms = 1000},
        {.temperature = 29.5f, .timestamp_ms = 2000},
        {.temperature = 35.8f, .timestamp_ms = 3000},
        {.temperature = 34.2f, .timestamp_ms = 4000},
        {.temperature = 30.0f, .timestamp_ms = 5000},
    };

    float    peak_temp;
    uint32_t peak_time;

    find_max_temp(readings, 5, &peak_temp, &peak_time);

    printf("Peak temperature: %.1f C at %u ms\n", peak_temp, peak_time);

    return 0;
}
```

---

## PART 6 — MEMORY LAYOUT: STACK vs HEAP (40 minutes)

This is one of the most important concepts in embedded systems because
microcontrollers have very limited RAM — sometimes as little as 2KB.
Understanding where your data lives is not optional.

---

### The Four Memory Regions

When your C program runs, memory is divided into distinct regions:

```
HIGH ADDRESS
+------------------+
|      STACK       |  ← grows downward
|                  |     local variables, function arguments,
|        ↓         |     return addresses
|                  |
|   (free space)   |
|                  |
|        ↑         |
|                  |
|      HEAP        |  ← grows upward (malloc/free)
+------------------+
|  .bss section    |  global/static variables (zero-initialized)
+------------------+
|  .data section   |  global/static variables (non-zero init)
+------------------+
|  .text section   |  your compiled machine code (read-only)
+------------------+
LOW ADDRESS
```

---

### The Stack — Automatic, Fast, Dangerous if Overflowed

The stack is where local variables live. It is managed automatically.
When you call a function, C pushes a "stack frame" — space for all the
function's local variables. When the function returns, that frame is
popped and the memory is instantly reclaimed.

```c
void read_sensor(void) {
    float temperature;     // lives on the stack
    uint8_t retry_count;   // lives on the stack
    char buffer[64];       // 64 bytes on the stack!

    // when this function returns, all 3 variables are GONE
}
```

**The critical rule for embedded:** The stack has a fixed, limited size.
On an Arduino Uno, the entire RAM is 2KB. On an STM32F401, you configure
stack size in the linker script (often 1–4KB). If you declare too many
large local variables (especially arrays) or recurse too deeply,
you overflow the stack and your system crashes with no error message.
It just misbehaves or resets. This is one of the nastiest bugs in
embedded development.

**What to never do in embedded code:**

```c
void dangerous_function(void) {
    uint8_t huge_buffer[1024];   // 1KB on the stack — may overflow!
    float matrix[32][32];        // 4KB on the stack — will overflow!
}
```

**What to do instead:**

```c
static uint8_t huge_buffer[1024];   // goes to .bss, allocated at compile time

void safe_function(void) {
    // stack frame is now tiny
}
```

---

### The Heap — Flexible, Dangerous, Often Avoided in Embedded

The heap is memory you request at runtime using malloc() and release
with free(). It is flexible because you can allocate exactly as much
as you need when you need it.

```c
#include <stdlib.h>

uint8_t *buffer = malloc(256);   // allocate 256 bytes from heap
if (buffer == NULL) {
    // malloc failed — heap is full!
    error_handler();
}

// ... use buffer ...

free(buffer);   // return memory to heap
```

**Why embedded engineers often avoid dynamic allocation:**

1. **Fragmentation**: After many malloc/free cycles, the heap becomes
   fragmented — many small holes that cannot satisfy large allocations.
   Predictable on a desktop with GB of RAM. Fatal on a 32KB RAM chip.

2. **Non-determinism**: malloc() does not guarantee how long it takes.
   Real-time systems need guaranteed response times.

3. **No error recovery**: If malloc fails at runtime, what do you do?
   On a standalone embedded system with no user, there is often no
   good answer.

**The embedded rule**: prefer static allocation. Know your memory
needs at compile time. Use fixed-size buffers.

---

### The .bss and .data Sections — Global and Static Variables

```c
uint32_t packet_count = 0;       // .bss — zero-initialized global
uint8_t device_id = 0xAB;        // .data — non-zero global

void counter_function(void) {
    static uint16_t call_count = 0;   // .bss — persists between calls!
    call_count++;
    printf("Called %u times\n", call_count);
}
```

The static keyword on a local variable is a powerful embedded pattern.
It persists between function calls (unlike normal locals) but is only
visible inside that function (unlike globals).

---

**Memory Layout Code — Study This Program**

```c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t global_counter = 0;          // .data section
uint8_t  receive_buffer[64];          // .bss section (auto zero-filled)

void demonstrate_stack_and_static(void) {
    uint8_t local_var = 100;          // stack — gone when function returns
    static uint8_t persistent = 0;   // .bss — survives function return

    persistent++;

    printf("  local_var address:     %p (stack)\n",    (void *)&local_var);
    printf("  persistent address:    %p (.bss)\n",     (void *)&persistent);
    printf("  persistent value:      %u\n", persistent);
}

int main(void) {
    printf("=== Memory Section Addresses ===\n");
    printf("global_counter:  %p (.data)\n", (void *)&global_counter);
    printf("receive_buffer:  %p (.bss)\n",  (void *)receive_buffer);

    printf("\n=== Stack vs Static ===\n");
    demonstrate_stack_and_static();
    demonstrate_stack_and_static();
    demonstrate_stack_and_static();

    printf("\n=== Heap Allocation ===\n");
    uint8_t *heap_buf = (uint8_t *)malloc(128);
    if (heap_buf == NULL) {
        printf("malloc failed!\n");
        return 1;
    }
    printf("heap_buf address: %p (heap)\n", (void *)heap_buf);
    free(heap_buf);
    heap_buf = NULL;   // always NULL after free to prevent use-after-free

    printf("\n=== Sizes ===\n");
    printf("sizeof(uint8_t):  %zu\n", sizeof(uint8_t));
    printf("sizeof(uint16_t): %zu\n", sizeof(uint16_t));
    printf("sizeof(uint32_t): %zu\n", sizeof(uint32_t));
    printf("sizeof(float):    %zu\n", sizeof(float));
    printf("sizeof(double):   %zu\n", sizeof(double));

    return 0;
}
```

WHAT TO OBSERVE:
- Call demonstrate_stack_and_static() 3 times — watch "persistent"
  increment each call even though it's a local variable. That's what
  static does.
- The heap address will be numerically higher than stack addresses
  on most systems — this confirms the memory map diagram above.
- Note that double is 8 bytes. Embedded systems almost always use float
  (4 bytes) because many microcontrollers lack a double-precision FPU.

---

## PART 7 — EXERCISE: THE CIRCULAR BUFFER (60 minutes)

This is your first real embedded data structure. Read the full
explanation before writing a single line of code.

---

### What Is a Circular Buffer?

A circular buffer (also called a ring buffer) is a fixed-size array
that wraps around. When you reach the end, you start writing at the
beginning again — overwriting the oldest data.

It solves a critical embedded problem: a sensor produces data continuously
but your main program can only process it periodically. The buffer sits
in between, holding recent readings. You never allocate new memory.
The buffer size is fixed at compile time.

### Visualization

Imagine a 6-slot buffer holding temperature readings (in °C):

```
Initial state (empty):
  [  ][  ][  ][  ][  ][  ]
   0   1   2   3   4   5
   ↑
  head=0, tail=0, count=0

After writing 25, 27, 30:
  [25][27][30][  ][  ][  ]
   0   1   2   3   4   5
   ↑           ↑
  head=0      tail=3, count=3

After reading once (reads 25):
  [25][27][30][  ][  ][  ]
   0   1   2   3   4   5
       ↑       ↑
      head=1  tail=3, count=2

After writing 28, 29, 31, 33 (buffer becomes full, wraps around):
  [33][27][30][28][29][31]
   0   1   2   3   4   5
       ↑   ↑
      tail  head=2, count=6
  (oldest data is at index 2 = 30)
```

The head tracks where the NEXT READ will happen.
The tail tracks where the NEXT WRITE will happen.
When tail reaches the end of the array, it wraps back to 0.

---

### The Implementation — Build This Yourself

Read the complete code below. Understand every line. Then close this
document and write it yourself from memory. Compare afterward.

```c
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_CAPACITY  8

typedef struct {
    float    data[BUFFER_CAPACITY];
    uint8_t  head;
    uint8_t  tail;
    uint8_t  count;
} CircularBuffer;

void cb_init(CircularBuffer *cb) {
    cb->head  = 0;
    cb->tail  = 0;
    cb->count = 0;
}

bool cb_is_empty(const CircularBuffer *cb) {
    return cb->count == 0;
}

bool cb_is_full(const CircularBuffer *cb) {
    return cb->count == BUFFER_CAPACITY;
}

bool cb_write(CircularBuffer *cb, float value) {
    if (cb_is_full(cb)) {
        return false;
    }

    cb->data[cb->tail] = value;
    cb->tail = (cb->tail + 1) % BUFFER_CAPACITY;
    cb->count++;
    return true;
}

bool cb_read(CircularBuffer *cb, float *value) {
    if (cb_is_empty(cb)) {
        return false;
    }

    *value = cb->data[cb->head];
    cb->head = (cb->head + 1) % BUFFER_CAPACITY;
    cb->count--;
    return true;
}

void cb_print_state(const CircularBuffer *cb) {
    printf("Buffer [head=%u tail=%u count=%u/%u]: ",
           cb->head, cb->tail, cb->count, BUFFER_CAPACITY);
    for (uint8_t i = 0; i < BUFFER_CAPACITY; i++) {
        if (cb->data[i] != 0.0f) {
            printf("[%.1f]", cb->data[i]);
        } else {
            printf("[    ]");
        }
    }
    printf("\n");
}

int main(void) {
    CircularBuffer sensor_buf;
    cb_init(&sensor_buf);

    float simulated_readings[] = {24.1f, 25.5f, 26.0f,
                                   27.3f, 28.1f, 27.9f};

    printf("=== Writing sensor readings ===\n");
    for (uint8_t i = 0; i < 6; i++) {
        bool ok = cb_write(&sensor_buf, simulated_readings[i]);
        printf("Write %.1f: %s\n", simulated_readings[i],
               ok ? "OK" : "FULL");
        cb_print_state(&sensor_buf);
    }

    printf("\n=== Reading 3 values ===\n");
    for (uint8_t i = 0; i < 3; i++) {
        float val;
        bool ok = cb_read(&sensor_buf, &val);
        if (ok) {
            printf("Read: %.1f\n", val);
        }
        cb_print_state(&sensor_buf);
    }

    printf("\n=== Writing 5 more (tests wrap-around) ===\n");
    float more[] = {30.0f, 31.5f, 29.8f, 28.4f, 27.0f};
    for (uint8_t i = 0; i < 5; i++) {
        bool ok = cb_write(&sensor_buf, more[i]);
        printf("Write %.1f: %s\n", more[i], ok ? "OK" : "FULL");
        cb_print_state(&sensor_buf);
    }

    printf("\n=== Draining entire buffer ===\n");
    while (!cb_is_empty(&sensor_buf)) {
        float val;
        cb_read(&sensor_buf, &val);
        printf("Drained: %.1f\n", val);
    }
    printf("Buffer empty: %s\n", cb_is_empty(&sensor_buf) ? "YES" : "NO");

    return 0;
}
```

---

### Line-by-Line Explanation of Key Parts

**The modulo operation (%) — the engine of the circular buffer:**

```c
cb->tail = (cb->tail + 1) % BUFFER_CAPACITY;
```

When tail is 7 (the last slot) and BUFFER_CAPACITY is 8:
   (7 + 1) % 8 = 8 % 8 = 0

The tail wraps back to index 0. This is the entire mechanism of the
"circular" behavior. No if-statement needed. Modulo handles it.

**Why return bool instead of void:**

```c
bool cb_write(CircularBuffer *cb, float value) {
    if (cb_is_full(cb)) {
        return false;   // caller knows write failed
    }
    ...
    return true;
}
```

In embedded systems, you cannot crash when something goes wrong —
there is no user to see an error message, and no OS to restart your
program. Instead, functions report success or failure via return values,
and the caller decides what to do. This pattern appears throughout
production embedded code.

**Passing the struct by pointer (not by value):**

```c
void cb_init(CircularBuffer *cb) { ... }
```

If we passed CircularBuffer by value (without *), C would copy the
entire struct onto the stack at every function call. The struct contains
an 8-element float array = 32 bytes, plus 3 uint8_t = 3 bytes = 35 bytes
copied on every call. With a pointer, only 4 bytes (the address) is
copied. In embedded code with 2KB of stack, this matters enormously.

---

### How to Compile and Run

Save the circular buffer program as week1/day1/circular_buffer.c, then:

   gcc circular_buffer.c -o circular_buffer
   ./circular_buffer

---

## PART 8 — GIT COMMIT (15 minutes)

Every piece of code you wrote today gets committed. This is non-negotiable.

Navigate to your embedded-journey folder:

   cd embedded-journey
   cd week1/day1

Make sure your files are here:
   pointers_basic.c
   circular_buffer.c

Add, commit, and push:

   git add .
   git commit -m "Week 1 Day 1: pointers, memory layout, circular buffer"
   git push origin main

Open GitHub in your browser and verify your files are there.

A good commit message describes WHAT you did and ideally WHY.
Not: "added stuff"
Good: "Week 1 Day 1: pointers, memory layout, circular buffer exercise"

---

## PART 9 — DAY 1 REVIEW QUESTIONS

Answer these without looking at the material. Write your answers in
notes/week1.md in your repository.

1. What does the & operator return? What does the * operator do when
   placed in front of a pointer variable?

2. What is the difference between these two declarations?
      int *ptr;
      int val;
   Which one can you use to indirectly modify another variable?

3. In pointer arithmetic, if ptr points to a uint32_t and you do ptr++,
   by how many bytes does the address change?

4. What does the % (modulo) operator return when given 9 % 8?

5. What memory section do these variables live in?
      a) A local variable inside a function
      b) A global variable initialized to zero
      c) A variable declared with the static keyword inside a function
      d) Memory returned by malloc()

6. Why does the circular buffer return bool from cb_write() instead of void?

7. If BUFFER_CAPACITY is 8 and tail is currently 7, what is
   (tail + 1) % BUFFER_CAPACITY?

8. What is the purpose of the count field in the CircularBuffer struct?
   Could you implement the buffer without it? What would be harder?

---

## WHAT YOU ACCOMPLISHED TODAY

By the end of Day 1 you have:
- A working development environment (VS Code, GCC, Git)
- A GitHub portfolio repository that will grow for 26 weeks
- Deep understanding of pointers and why embedded engineers need them
- Understanding of C memory layout (stack, heap, .bss, .data)
- A working circular buffer — a real embedded data structure used in
  UART drivers, ADC sample buffers, audio pipelines, and CAN bus stacks

Day 2 begins bit manipulation — the skill that lets you talk to hardware
registers. You will use everything from today as the foundation.
