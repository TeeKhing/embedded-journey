# 6-Month Roadmap: Embedded Systems → Edge Computing
### From Complete Beginner to Field-Ready Engineer

---

## Overview & Philosophy

This roadmap is built on one principle: **build things, break things, understand why**.
Passive learning (watching videos, reading docs) accounts for ~30% of your time. The remaining 70% is hands-on — writing C, flashing firmware, reading datasheets, debugging hardware, and deploying real inference on real devices.

The trajectory moves deliberately:
> **Bare-metal C** → **Microcontroller Peripherals** → **Linux on Hardware** → **RTOS** → **IoT Networking** → **Edge AI/ML Inference** → **Production Edge Architecture**

**Daily time allocation (4–5 hours):**
| Block | Duration | Purpose |
|---|---|---|
| Concept Block | 60–75 min | Reading, videos, datasheets |
| Lab Block | 90–120 min | Hands-on implementation |
| Problem/Debug Block | 60 min | Exercises, bug fixing, Q&A |
| Review & Documentation | 30–45 min | Notes, Git commits, reflection |

---

## Required Hardware & Tools

### Hardware (acquire by Week 1)
- **Arduino Uno R3** — beginner-friendly entry point
- **STM32 Nucleo-F401RE** — professional-grade ARM Cortex-M4
- **Raspberry Pi 4 (4GB)** — Linux embedded platform
- **ESP32 DevKit** — Wi-Fi/BT, great for edge AI
- **Breadboard, jumper wires, resistors, LEDs, push buttons**
- **DHT22** (temp/humidity), **HC-SR04** (ultrasonic), **MPU-6050** (IMU)
- **I2C 0.96" OLED display**, **16x2 LCD**
- **Logic Analyzer (optional but highly recommended)** — Saleae clone works fine
- **USB-to-TTL UART adapter**

### Software
- **VS Code** + PlatformIO extension (primary IDE)
- **STM32CubeIDE** (STM32-specific)
- **Arduino IDE** (early weeks only)
- **Git** + GitHub account (day one — every line of code gets committed)
- **minicom / PuTTY** (serial terminal)
- **Wireshark** (networking analysis)
- **Edge Impulse Studio** (edge ML — Phase 3)
- **draw.io** (architecture diagrams — document everything)

---

## PHASE 1 — Embedded Foundations (Months 1–2, Weeks 1–8)

> Goal: Write confident bare-metal C, understand digital/analog I/O, communicate over UART/SPI/I2C, and read datasheets without fear.

---

### WEEK 1 — C Programming Refresher for Embedded

**Monday (Day 1)**
- [ ] Install all tools: VS Code, PlatformIO, Git, Arduino IDE
- [ ] Configure GitHub repo: `embedded-journey` — all future code lives here
- [ ] Review: pointers, arrays, structs, memory layout (stack vs heap)
- [ ] Exercise: Write a C program that simulates a circular buffer for sensor data
- [ ] Read: "Why C for Embedded?" — search embedded.fm podcast archives

**Tuesday (Day 2)**
- [ ] Study: bit manipulation — AND, OR, XOR, NOT, shifts
- [ ] Lab: Write 10 bit-twiddling exercises by hand (set bit 3, clear bit 5, toggle bit 7 of a register)
- [ ] Study: `volatile` keyword, `const`, `static` in embedded context
- [ ] Commit all exercises to GitHub with clear commit messages

**Wednesday (Day 3)**
- [ ] Study: Fixed-width integer types (`uint8_t`, `uint16_t`, `uint32_t`) — why they matter
- [ ] Study: Enums and #define for hardware register maps
- [ ] Lab: Model a hypothetical GPIO register in C using structs and bitfields
- [ ] Read: Chapters 1–3 of "Making Embedded Systems" by Elecia White (PDF/print)

**Thursday (Day 4)**
- [ ] Study: Memory sections — `.text`, `.bss`, `.data`, stack, heap
- [ ] Lab: Flash your first Arduino sketch — blink an LED (non-trivial goal: understand every line)
- [ ] Dig into Arduino's `digitalWrite` source — find what register it actually writes
- [ ] Document findings in a `notes/week1.md` file in your repo

**Friday (Day 5)**
- [ ] Review all week's material — quiz yourself on bit manipulation without looking at notes
- [ ] Refactor your circular buffer to use fixed-width types and proper register-style comments
- [ ] Write `README.md` for your repo — describe what you're building and why
- [ ] Draft LinkedIn post (see topics below)

**LinkedIn Post Topic — Week 1:**
*"I just started a 6-month journey from complete beginner to Edge Computing engineer. Day 1: I traced Arduino's `digitalWrite()` to its actual hardware register. This is what learning embedded systems feels like — and I'm documenting every step."*

---

### WEEK 2 — Microcontroller Architecture & GPIO

**Monday**
- [ ] Study: CPU, ALU, registers, program counter, memory-mapped I/O — draw a block diagram
- [ ] Read: STM32F401 datasheet introduction and memory map (pages 1–40) — yes, read a datasheet
- [ ] Understand: Harvard vs Von Neumann architecture, why embedded uses Harvard

**Tuesday**
- [ ] Study: GPIO — input mode, output mode, pull-up/pull-down resistors, open-drain
- [ ] Lab: Wire an LED and push button to Arduino; write bare-metal AVR register code (no `digitalWrite`)
- [ ] Measure voltage on GPIO pin with a multimeter — verify your code with hardware

**Wednesday**
- [ ] Study: Debouncing — hardware vs software, why it matters
- [ ] Lab: Implement software debounce on your button; observe behavior on serial monitor
- [ ] Study: Interrupts vs polling — conceptual understanding (implementation next week)

**Thursday**
- [ ] Lab: Move to STM32 Nucleo — blink onboard LED using STM32CubeIDE (HAL library)
- [ ] Compare: Arduino AVR GPIO code vs STM32 HAL GPIO code — what abstractions exist?
- [ ] Study: Clock trees on STM32 — why do you need to enable peripheral clocks?

**Friday**
- [ ] Lab: STM32 GPIO — read button, control LED, no HAL (use direct register access)
- [ ] Write up a comparison doc: `Arduino AVR vs STM32 bare-metal` in your notes folder
- [ ] Review & commit everything

**LinkedIn Post Topic — Week 2:**
*"Most people learn GPIO as 'make an LED blink.' I dug deeper: I read a 1,000-page datasheet to understand what memory address my code actually writes to. Embedded systems = reading the manual, and I'm learning to love it."*

---

### WEEK 3 — Timers, Interrupts & PWM

**Monday**
- [ ] Study: Hardware timers — prescalers, compare registers, overflow interrupts
- [ ] Study: Interrupt Service Routines (ISR) — NVIC on Cortex-M, interrupt priority
- [ ] Read: Chapter 5 of "Making Embedded Systems" (timers and interrupts)

**Tuesday**
- [ ] Lab: Implement a 1-second timer interrupt on Arduino (using Timer1 directly, no `delay()`)
- [ ] Rule to internalize: **Never use `delay()` in production embedded code again**
- [ ] Study: ISR best practices — keep them short, use flags, volatile variables

**Wednesday**
- [ ] Study: PWM — duty cycle, frequency, applications (motor speed, LED brightness)
- [ ] Lab: Generate PWM on Arduino pin — fade an LED smoothly using a timer
- [ ] Lab: Read PWM signal with oscilloscope function in logic analyzer (if available)

**Thursday**
- [ ] Lab: STM32 — configure TIM2 for PWM output using CubeMX + HAL
- [ ] Compare PWM configuration complexity: Arduino vs STM32 (document in notes)
- [ ] Lab: Implement an external interrupt on STM32 button — toggle LED on falling edge

**Friday**
- [ ] Lab: Combine — use timer interrupt every 10ms, implement a software PWM manually on STM32
- [ ] Exercise: Why is `volatile` mandatory for variables shared between ISR and main loop?
- [ ] Write the answer as a blog-style note in your repo

**LinkedIn Post Topic — Week 3:**
*"I deleted `delay()` from my vocabulary this week. Here's why every professional embedded engineer avoids it — and what hardware timers + interrupts give you instead. [short explainer thread]"*

---

### WEEK 4 — UART & Serial Communication

**Monday**
- [ ] Study: UART protocol — baud rate, start bit, data bits, parity, stop bits
- [ ] Study: Serial vs parallel communication — why serial dominates embedded
- [ ] Read: UART section in STM32F401 reference manual (skim for structure, not full depth yet)

**Tuesday**
- [ ] Lab: Send formatted sensor data over UART from Arduino to PC terminal
- [ ] Lab: Implement your own `printf`-style function using `USART_Transmit` on STM32
- [ ] Capture UART frames with logic analyzer — visually verify baud rate and bit timing

**Wednesday**
- [ ] Study: Ring buffer for UART receive — why you need one (avoid data loss)
- [ ] Lab: Implement interrupt-driven UART receive with a ring buffer on STM32
- [ ] Exercise: What happens if your ISR takes longer than one UART byte period?

**Thursday**
- [ ] Lab: Two-device UART communication — Arduino TX → STM32 RX
- [ ] Protocol design: define a simple packet format (`[START][CMD][LEN][DATA][CRC]`)
- [ ] Lab: Implement the packet parser on both sides

**Friday**
- [ ] Lab: Add UART-based debug console to your STM32 project — accept single-char commands
- [ ] Document your packet protocol spec in a `protocols/uart_spec.md` file
- [ ] Review interrupt-driven UART implementation — refactor for clarity

**LinkedIn Post Topic — Week 4:**
*"UART is the heartbeat of embedded communication. This week I built a custom packet protocol from scratch — start byte, command, length, data, CRC checksum. Writing protocols you actually understand is a different level of learning."*

---

### WEEK 5 — SPI & I2C Protocols

**Monday**
- [ ] Study: SPI — MOSI, MISO, SCLK, CS; 4-wire full-duplex; clock polarity/phase (CPOL/CPHA)
- [ ] Study: I2C — SDA/SCL, 7-bit addressing, ACK/NACK, clock stretching, multi-master

**Tuesday**
- [ ] Lab: Interface OLED display over I2C on Arduino — understand every line of the driver
- [ ] Capture I2C frames with logic analyzer — identify address, data, ACK bits
- [ ] Lab: I2C scan — write a program that scans all 128 addresses and prints found devices

**Wednesday**
- [ ] Lab: Read MPU-6050 IMU over I2C on STM32 without HAL I2C driver — write your own
- [ ] Study: I2C pull-up resistor calculation — why does it matter?
- [ ] Document your I2C driver with register addresses as named constants

**Thursday**
- [ ] Study: SPI vs I2C — when to use which (speed, wiring, addressing)
- [ ] Lab: Interface an SPI flash memory chip (W25Q32 or similar) — write/read a byte
- [ ] Observe SPI timing on logic analyzer — compare to I2C trace from Tuesday

**Friday**
- [ ] Lab: Create a unified sensor abstraction — `sensor_read()` works the same regardless of I2C or SPI underneath
- [ ] Exercise: Read the DHT22 datasheet and implement its custom 1-wire protocol manually
- [ ] Commit, document, review

**LinkedIn Post Topic — Week 5:**
*"I wrote an I2C driver from scratch this week — no libraries, just me and the STM32 reference manual. The moment it worked and I saw the ACK bit on my logic analyzer was one of the most satisfying moments in 5 weeks of learning."*

---

### WEEK 6 — ADC/DAC, Power & Debugging

**Monday**
- [ ] Study: ADC — resolution, sampling rate, reference voltage, Nyquist theorem
- [ ] Study: Quantization error, signal-to-noise ratio, oversampling for better resolution

**Tuesday**
- [ ] Lab: STM32 ADC — read a potentiometer value, map to LED brightness via PWM
- [ ] Lab: Configure ADC in DMA mode (no CPU polling) — understand DMA transfers
- [ ] Lab: Read temperature from internal STM32 temperature sensor via ADC

**Wednesday**
- [ ] Study: Power consumption basics — active, sleep, deep sleep modes on Cortex-M
- [ ] Lab: Measure current consumption in normal vs sleep mode using a multimeter
- [ ] Study: Battery-powered design considerations — brown-out detection, voltage regulation

**Thursday**
- [ ] Study: Debugging embedded systems — GDB, SWD/JTAG, printf debugging, blink codes
- [ ] Lab: Use GDB with OpenOCD to set breakpoints and inspect registers on STM32
- [ ] Lab: Implement a hardware fault handler that blinks an error code in Morse

**Friday**
- [ ] Lab: Build a data acquisition system — read 3 sensors (temp, light, potentiometer) via ADC, transmit over UART every 500ms
- [ ] Use DMA for ADC, interrupt-driven UART transmit — no polling anywhere
- [ ] Commit, write `notes/week6_debugging.md`

**LinkedIn Post Topic — Week 6:**
*"Debugging embedded code without GDB is like debugging blind. This week I set up SWD debugging, stepped through my firmware instruction by instruction, and watched my stack pointer in real time. Embedded development has incredible tooling when you learn it."*

---

### WEEK 7 — Q1 MILESTONE PROJECT BUILD WEEK

> **Q1 Milestone Project: Digital Environmental Monitor**
> Build a standalone embedded device that reads temperature and humidity (DHT22), ambient light (LDR via ADC), and displays live readings on an I2C OLED display. Blinks an alert LED when temperature exceeds a threshold. All peripherals driven by interrupt/timer — zero polling, zero `delay()`.

**Monday**
- [ ] Write project requirements doc — inputs, outputs, constraints, success criteria
- [ ] Draw block diagram: MCU ↔ DHT22 ↔ OLED ↔ LED ↔ ADC input
- [ ] Scaffold codebase: `main.c`, `dht22.c/.h`, `oled.c/.h`, `adc.c/.h`, `uart_debug.c/.h`

**Tuesday**
- [ ] Implement DHT22 driver (custom 1-wire protocol using timer-based bit timing)
- [ ] Test in isolation — print readings over UART before touching display

**Wednesday**
- [ ] Implement OLED display driver (I2C, SSD1306 chip)
- [ ] Design a clean screen layout — temperature line, humidity line, light level bar
- [ ] Integrate ADC light sensor reading

**Thursday**
- [ ] Implement threshold alarm — configure as timer-based periodic check, not polling
- [ ] Add UART debug console — `t` prints temp, `h` prints humidity, `r` resets alarm
- [ ] Full integration test — all subsystems running simultaneously

**Friday**
- [ ] Stress test, edge case handling (sensor disconnect, out-of-range values)
- [ ] Write `README.md` for project: what it does, how to build, wiring diagram, demo
- [ ] Film a 60-second demo video — post to LinkedIn

**LinkedIn Post Topic — Week 7:**
*"Q1 milestone complete: a standalone environmental monitor in bare-metal C. No Arduino libraries. Custom I2C, custom ADC, interrupt-driven everything. Here's what I learned building my first real embedded project — [thread]"*

---

### WEEK 8 — Code Quality, Version Control & Embedded Best Practices

**Monday**
- [ ] Study: MISRA-C guidelines (key rules) — why safety-critical code has style rules
- [ ] Study: Static analysis tools — `cppcheck`, `PC-lint` — install and run on your Q1 project
- [ ] Refactor Q1 project based on static analysis findings

**Tuesday**
- [ ] Study: Modular C architecture — header guards, opaque types, hardware abstraction layers
- [ ] Refactor Q1 project to have a clean HAL: `bsp.c/.h` separates hardware from logic
- [ ] Study: Linker scripts — what they do, stack/heap sizing, memory regions

**Wednesday**
- [ ] Study: Git workflow for embedded — branching strategy, meaningful commits, tagging releases
- [ ] Practice: Create `v1.0.0` tag for Q1 project, write a clean release description
- [ ] Study: Semantic versioning — why firmware versions matter in production

**Thursday**
- [ ] Study: Documentation practices — Doxygen comments, README quality
- [ ] Add Doxygen-style comments to all functions in Q1 project
- [ ] Generate HTML documentation from Doxygen

**Friday**
- [ ] Study: Unit testing for embedded — unity test framework
- [ ] Write 3 unit tests for your circular buffer from Week 1 using Unity
- [ ] Reflect: what habits from this week would you have wanted in Week 1?

**LinkedIn Post Topic — Week 8:**
*"Nobody teaches this in tutorials: how to write embedded code that professionals won't cringe at. MISRA-C, static analysis, proper HALs, Doxygen docs. 2 months in and I'm learning what clean firmware actually looks like."*

---

## PHASE 2 — Intermediate Embedded: Linux, RTOS & Networking (Months 3–4, Weeks 9–17)

> Goal: Run Linux on embedded hardware, understand FreeRTOS task scheduling, implement real networking protocols, and build a production-quality multi-sensor system.

---

### WEEK 9 — Linux on Embedded Hardware

**Monday**
- [ ] Study: Why Linux on embedded? Kernel, drivers, user space, device tree
- [ ] Set up Raspberry Pi 4 with Raspberry Pi OS — learn the boot sequence
- [ ] Study: `dmesg`, `lsmod`, `/proc`, `/sys` — the Linux embedded interface

**Tuesday**
- [ ] Study: Device Tree — what it is, why ARM Linux needs it, basic syntax
- [ ] Lab: Read `/sys/class/gpio` — control GPIO from Linux user space
- [ ] Lab: Write a Python script that blinks an LED via sysfs GPIO

**Wednesday**
- [ ] Study: Linux I2C from user space — `/dev/i2c-X`, `i2cdetect`, `i2cget/set`
- [ ] Lab: Read MPU-6050 from Raspberry Pi user space using `smbus2` Python library
- [ ] Lab: Read DHT22 from Raspberry Pi — use a C program via `/dev/gpiomem`

**Thursday**
- [ ] Study: Cross-compilation — build ARM code on x86 host
- [ ] Lab: Set up an ARM cross-compiler toolchain on your PC; cross-compile a "hello world" C program, deploy to Raspberry Pi via SCP
- [ ] Study: `systemd` — write a service file so your sensor program runs on boot

**Friday**
- [ ] Lab: Write a Linux C application that reads 3 sensors and writes timestamped CSV to SD card
- [ ] Set up SSH key authentication — no more password prompts
- [ ] Create systemd service; test auto-start on reboot

**LinkedIn Post Topic — Week 9:**
*"Month 3 begins with Linux on a Raspberry Pi. The jump from bare-metal microcontrollers to Linux embedded is huge — but the fundamentals I built in Months 1–2 make the kernel's GPIO subsystem make total sense now."*

---

### WEEK 10 — Linux Systems Programming

**Monday**
- [ ] Study: POSIX threads (pthreads) — `pthread_create`, mutexes, condition variables
- [ ] Study: Processes vs threads in embedded Linux — when to use each

**Tuesday**
- [ ] Lab: Write a multithreaded sensor application — one thread reads sensors, one writes to disk, one serves data over a named pipe
- [ ] Debug: race condition — introduce one deliberately, observe the crash, fix with mutex

**Wednesday**
- [ ] Study: Memory management in Linux — `mmap`, virtual memory, avoiding fragmentation
- [ ] Study: Real-time Linux — `PREEMPT_RT` patch, priority inversion, priority inheritance

**Thursday**
- [ ] Study: Linux IPC — pipes, FIFOs, Unix domain sockets, message queues
- [ ] Lab: Sensor reader process sends data to a display process via Unix socket

**Friday**
- [ ] Lab: Implement a watchdog — use Linux `/dev/watchdog` to restart your process on hang
- [ ] Study: Power management on Raspberry Pi — `cpufreq`, CPU governor settings
- [ ] Commit, document, review

**LinkedIn Post Topic — Week 10:**
*"I deliberately introduced a race condition in my embedded Linux code this week. Then I watched it crash. Then I fixed it with a mutex. Learning to break things intentionally is one of the most underrated ways to understand systems programming."*

---

### WEEK 11 — FreeRTOS Fundamentals

**Monday**
- [ ] Study: RTOS concepts — tasks, scheduler, context switching, priority
- [ ] Study: When do you need an RTOS? (vs superloop vs Linux)
- [ ] Read: FreeRTOS documentation — "Getting Started," "Tasks and Co-routines"

**Tuesday**
- [ ] Lab: Set up FreeRTOS on STM32 via STM32CubeMX (CMSIS-RTOS2 wrapper)
- [ ] Lab: Create 3 tasks — LED blink, UART print, ADC read — each at different priorities
- [ ] Observe task switching with logic analyzer (toggle GPIO at task start/end)

**Wednesday**
- [ ] Study: Queues — inter-task communication without shared memory
- [ ] Lab: Producer-consumer with queues — sensor task produces, display task consumes
- [ ] Study: Semaphores — binary, counting, mutex — when to use which

**Thursday**
- [ ] Lab: Implement a semaphore-protected UART driver — only one task can transmit at a time
- [ ] Study: Task notifications — lightweight alternative to semaphores
- [ ] Lab: Use task notification to wake display task when new sensor data arrives

**Friday**
- [ ] Study: Stack overflow detection in FreeRTOS — configCHECK_FOR_STACK_OVERFLOW
- [ ] Lab: Measure stack high-water mark for each task; size appropriately
- [ ] Study: Tick hooks and idle hooks — where to put low-priority housekeeping work

**LinkedIn Post Topic — Week 11:**
*"FreeRTOS changed how I think about concurrency. A queue between tasks is cleaner than any mutex I've written. This week's insight: in RTOS design, how you *structure* your tasks matters more than how fast your CPU runs."*

---

### WEEK 12 — Advanced FreeRTOS & Power Management

**Monday**
- [ ] Study: Software timers in FreeRTOS — periodic callbacks without a dedicated task
- [ ] Study: Event groups — synchronizing multiple tasks on multiple events

**Tuesday**
- [ ] Lab: Implement a state machine using FreeRTOS event groups (IDLE → MEASURING → TRANSMITTING → SLEEPING)
- [ ] Lab: Use FreeRTOS software timers for periodic sensor reads

**Wednesday**
- [ ] Study: Tickless idle mode — FreeRTOS low-power operation
- [ ] Lab: Implement tickless idle on STM32; measure power reduction in sleep intervals
- [ ] Study: Stream buffers and message buffers — for UART/DMA integration

**Thursday**
- [ ] Lab: DMA-driven UART receive feeding a FreeRTOS stream buffer — zero-copy UART receive pipeline
- [ ] Benchmark: CPU usage with polling vs DMA+RTOS — use `vTaskGetRunTimeStats`

**Friday**
- [ ] Lab: Full integration — FreeRTOS system with: sensor task (I2C, 100ms), display task (OLED, 200ms), UART command task (event-driven), power manager task (sleep when idle)
- [ ] Commit everything, write architecture doc with diagram

**LinkedIn Post Topic — Week 12:**
*"Power management in embedded systems is an art. This week I implemented FreeRTOS tickless idle mode and measured a 60% reduction in average current consumption. For battery-powered edge devices, this is the difference between months and years of runtime."*

---

### WEEK 13 — Q2 MILESTONE PROJECT BUILD WEEK

> **Q2 Milestone Project: FreeRTOS Multi-Sensor Data Logger**
> An STM32-based system running FreeRTOS with 4 concurrent tasks: IMU data collection (I2C, 50Hz), temperature logging (DHT22, 1Hz), UART command interface (interactive), and SD card writer (buffered). All inter-task communication via queues. Tickless idle when no activity. Produces timestamped CSV data logs.

**Monday**
- [ ] Requirements + architecture doc, block diagram with task boundaries and queue layout
- [ ] Scaffold: define all task functions, queue sizes, stack sizes before writing any logic

**Tuesday**
- [ ] Implement sensor tasks — IMU + DHT22, posting to data queue
- [ ] Implement SD card writer task — consumes queue, writes 512-byte buffered CSV

**Wednesday**
- [ ] Implement UART command interface task — `start`, `stop`, `status`, `dump` commands
- [ ] Integrate state machine — system only collects when in ACTIVE state

**Thursday**
- [ ] Implement tickless idle; profile with GPIO toggles + logic analyzer
- [ ] Add watchdog timer — system auto-resets if any task starves

**Friday**
- [ ] Full integration test, stress test (run 1 hour, verify log integrity)
- [ ] Write README with architecture diagram, demo instructions, wiring diagram
- [ ] Post project to LinkedIn with demo video

**LinkedIn Post Topic — Week 13:**
*"Q2 milestone: a production-quality FreeRTOS data logger. 4 tasks, 3 queues, SD card logging, interactive UART shell, hardware watchdog. 4 months ago I didn't know what a microcontroller was. [Project breakdown thread]"*

---

### WEEK 14 — Networking Fundamentals for Embedded

**Monday**
- [ ] Study: OSI model — understand layers 1–4 deeply; layers 5–7 at application level
- [ ] Study: TCP vs UDP — when each applies in embedded/edge scenarios
- [ ] Study: IP addressing, subnets, routing basics — enough for edge device architecture

**Tuesday**
- [ ] Study: MQTT protocol — broker, topics, QoS levels, retain, will messages
- [ ] Lab: Set up Mosquitto MQTT broker on Raspberry Pi
- [ ] Lab: Python MQTT publisher/subscriber — publish sensor data, subscribe on another terminal

**Wednesday**
- [ ] Lab: ESP32 — connect to WiFi, publish DHT22 readings to MQTT broker every 5 seconds
- [ ] Study: MQTT QoS 0 vs 1 vs 2 — test message delivery guarantees under packet loss

**Thursday**
- [ ] Study: HTTP/HTTPS REST APIs — GET/POST, JSON, authentication headers
- [ ] Lab: ESP32 — POST sensor JSON to a local Flask server on Raspberry Pi
- [ ] Study: CoAP protocol — the UDP alternative to HTTP for constrained devices

**Friday**
- [ ] Study: WebSockets — persistent connections for real-time dashboards
- [ ] Lab: Raspberry Pi Flask server with WebSocket — live sensor graph in browser
- [ ] Review: MQTT vs HTTP vs CoAP — document when to use each

**LinkedIn Post Topic — Week 14:**
*"MQTT is the language of IoT. I connected an ESP32 to a Raspberry Pi broker today and watched sensor data flow in real time. Understanding *why* MQTT uses a publish-subscribe model (vs REST polling) is fundamental to scalable edge architecture."*

---

### WEEK 15 — Embedded Linux Networking & Security

**Monday**
- [ ] Study: TLS/SSL basics — certificates, handshake, symmetric vs asymmetric encryption
- [ ] Lab: Enable TLS on Mosquitto MQTT broker (self-signed certificates)
- [ ] Lab: Connect ESP32 to TLS-enabled broker — observe certificate verification

**Tuesday**
- [ ] Study: Secure boot concepts — chain of trust, bootloader signing
- [ ] Study: Hardware security modules (HSM) and TPM basics
- [ ] Lab: Store MQTT credentials in ESP32 NVS (Non-Volatile Storage) — never hardcode secrets

**Wednesday**
- [ ] Study: OTA firmware updates — requirements, risks, rollback mechanisms
- [ ] Lab: Implement OTA on ESP32 using Arduino OTA or ESP-IDF OTA partition scheme
- [ ] Exercise: What happens if power fails mid-OTA? Design a recovery strategy

**Thursday**
- [ ] Study: VPN basics for edge devices — WireGuard for embedded
- [ ] Lab: Set up WireGuard on Raspberry Pi — connect securely without exposing MQTT to internet
- [ ] Study: firewall rules — `iptables` for embedded Linux

**Friday**
- [ ] Study: Common IoT vulnerabilities — default credentials, unencrypted comms, no OTA
- [ ] Audit your Week 14 ESP32 project against OWASP IoT Top 10
- [ ] Document all security fixes made

**LinkedIn Post Topic — Week 15:**
*"Security is not optional in edge computing. This week I audited my own IoT project against OWASP IoT Top 10 and found 3 vulnerabilities I'd introduced without thinking. Hardcoded credentials, unencrypted MQTT, no certificate verification. Fixed all three — here's how."*

---

### WEEK 16 — Data Serialization, Storage & Time

**Monday**
- [ ] Study: JSON vs MessagePack vs Protobuf vs CBOR — tradeoffs for constrained devices
- [ ] Lab: Benchmark serialization size and parse time: JSON vs MessagePack on ESP32

**Tuesday**
- [ ] Study: Time synchronization — NTP, PTP, RTC modules, why timestamps matter in edge data
- [ ] Lab: Add RTC (DS3231 I2C) to STM32 project — log data with accurate timestamps
- [ ] Lab: NTP sync on Raspberry Pi — synchronize system time, script to check drift

**Wednesday**
- [ ] Study: Databases for embedded Linux — SQLite, InfluxDB (time-series)
- [ ] Lab: Store sensor data in SQLite on Raspberry Pi — write, query, aggregate
- [ ] Study: Data retention policies — ring buffer on disk, rollover after N days

**Thursday**
- [ ] Study: InfluxDB + Grafana stack — the industry standard for IoT time-series visualization
- [ ] Lab: Install InfluxDB + Grafana on Raspberry Pi; ingest MQTT data, build a live dashboard

**Friday**
- [ ] Lab: Connect everything — ESP32 → MQTT → Raspberry Pi → InfluxDB → Grafana dashboard
- [ ] Commit full stack setup with `docker-compose.yml` or install scripts
- [ ] Document architecture with diagram

**LinkedIn Post Topic — Week 16:**
*"InfluxDB + Grafana on a Raspberry Pi. My ESP32 now streams sensor data into a beautiful real-time dashboard. This is what end-to-end IoT looks like before you involve a cloud provider — and it runs on a $35 computer."*

---

### WEEK 17 — Q3 MILESTONE PROJECT BUILD WEEK

> **Q3 Milestone Project: Raspberry Pi MQTT IoT Hub with Live Dashboard**
> A multi-device IoT system: 2× ESP32 nodes (different sensors) publish to a Mosquitto broker on Raspberry Pi. TLS-encrypted MQTT. InfluxDB stores time-series data. Grafana displays live dashboard with alerts. OTA update mechanism for ESP32 nodes. All running as systemd services.

**Monday–Tuesday** — ESP32 node firmware (TLS MQTT + OTA + sensor reading)
**Wednesday** — Raspberry Pi broker + InfluxDB pipeline + Telegraf MQTT consumer
**Thursday** — Grafana dashboard + alerting rules + data retention policy
**Friday** — Integration, stress test, README, GitHub release, LinkedIn post

**LinkedIn Post Topic — Week 17:**
*"Q3 milestone — a complete IoT stack from sensor to dashboard. ESP32 nodes, TLS-encrypted MQTT, InfluxDB, Grafana alerts, OTA updates. This is a production IoT architecture. Building it on hardware you own, from scratch, is the best way to truly understand it."*

---

## PHASE 3 — Edge Computing (Months 5–6, Weeks 18–26)

> Goal: Deploy real-time ML inference on microcontrollers and SBCs, understand edge architecture patterns, integrate with cloud platforms, and build a capstone project suitable for a portfolio.

---

### WEEK 18 — Introduction to Edge Computing & Edge AI

**Monday**
- [ ] Study: What is edge computing? Latency, bandwidth, privacy, reliability arguments
- [ ] Study: Edge vs Cloud vs Fog continuum — use cases for each tier
- [ ] Study: Edge AI landscape — TinyML, TensorFlow Lite Micro, Edge Impulse, ONNX Runtime

**Tuesday**
- [ ] Study: Machine learning basics — classification, regression, neural networks (conceptual)
- [ ] Study: Model compression techniques — quantization, pruning, knowledge distillation
- [ ] Lab: Run a pre-trained TensorFlow Lite image classification model on Raspberry Pi

**Wednesday**
- [ ] Study: What makes a model "edge-ready"? — FLOPS, memory, latency, accuracy tradeoffs
- [ ] Study: Edge AI hardware — MCU, DSP, NPU, GPU differences
- [ ] Read: Google's "On-Device Machine Learning" overview documentation

**Thursday**
- [ ] Lab: Set up Edge Impulse Studio account — explore interface, dataset types
- [ ] Lab: Record 3 sensor motion classes with MPU-6050 on Arduino Nano 33 BLE Sense or ESP32
- [ ] Study: Feature engineering for time-series sensor data — FFT, spectral features

**Friday**
- [ ] Lab: Train your first Edge Impulse model — gesture classification (3 classes)
- [ ] Study: Confusion matrix, precision, recall, F1 — evaluate your model
- [ ] Document: What was your model's accuracy? What would improve it?

**LinkedIn Post Topic — Week 18:**
*"Week 18: first steps into Edge AI. I trained a gesture classifier on an ESP32 today — the model lives entirely on the microcontroller, runs inference in under 5ms, and works completely offline. The edge computing revolution is real and it runs on $5 chips."*

---

### WEEK 19 — TensorFlow Lite Micro & Quantization

**Monday**
- [ ] Study: TensorFlow Lite Micro architecture — interpreter, ops, arena, model flatbuffer
- [ ] Study: INT8 quantization — how float32 weights map to int8, accuracy vs size tradeoffs

**Tuesday**
- [ ] Lab: Convert a TensorFlow Keras model (MNIST or simple audio) to TensorFlow Lite
- [ ] Lab: Apply post-training quantization — compare model size: float32 vs float16 vs INT8
- [ ] Lab: Run TFLite inference on Raspberry Pi, measure latency

**Wednesday**
- [ ] Lab: Deploy TFLite Micro on ESP32 — run keyword spotting model ("yes"/"no")
- [ ] Measure: inference time, RAM usage, flash usage on ESP32
- [ ] Study: Tensor Arena sizing — how to calculate minimum arena size

**Thursday**
- [ ] Study: Operator support in TFLite Micro — not all TF ops are available
- [ ] Lab: Attempt to use an unsupported layer — observe the error, understand the constraint
- [ ] Study: Custom ops in TFLite Micro — when and how to implement them

**Friday**
- [ ] Lab: Optimize inference pipeline — batch reads, double buffering audio/sensor input
- [ ] Benchmark: CPU-only vs SIMD-accelerated inference on Raspberry Pi (using NEON intrinsics)
- [ ] Document: inference pipeline architecture diagram

**LinkedIn Post Topic — Week 19:**
*"INT8 quantization: I took a 4MB TensorFlow model and compressed it to 380KB with less than 1% accuracy loss. Then I ran it on an ESP32 with 320KB of RAM. Understanding quantization is foundational for anyone serious about Edge AI."*

---

### WEEK 20 — Edge Impulse Deep Dive & Custom Pipelines

**Monday**
- [ ] Study: Edge Impulse signal processing blocks — MFCC, spectrogram, raw, spectral analysis
- [ ] Lab: Build an anomaly detection model using IMU data — collect "normal" and "anomalous" motion

**Tuesday**
- [ ] Lab: Export Edge Impulse model as Arduino library — deploy to ESP32
- [ ] Lab: Measure inference latency; tune DSP block parameters for accuracy vs speed

**Wednesday**
- [ ] Study: Continuous inference vs triggered inference — duty cycling for power efficiency
- [ ] Lab: Implement a sliding-window inference pipeline — infer on last 1s of data every 250ms

**Thursday**
- [ ] Lab: Build a vibration anomaly detector — mount ESP32 to a motor, train on normal vibration, detect fault
- [ ] Study: Production deployment considerations — confidence thresholds, false positive handling

**Friday**
- [ ] Lab: ONNX Runtime on Raspberry Pi — run a scikit-learn model converted to ONNX
- [ ] Compare: Edge Impulse vs TFLite Micro vs ONNX RT — document use cases for each
- [ ] Commit anomaly detector project

**LinkedIn Post Topic — Week 20:**
*"I put an AI model on a motor today. The ESP32 learned what 'normal' vibration feels like, then detected a simulated bearing fault in real time. Industrial predictive maintenance — running on a $4 chip. This is why I chose Edge Computing."*

---

### WEEK 21 — Cloud Edge Integration (AWS IoT / Azure IoT Edge)

**Monday**
- [ ] Study: AWS IoT Core architecture — device shadow, MQTT bridge, Greengrass
- [ ] Study: Azure IoT Edge — modules, container runtime, route configuration
- [ ] Study: When to use cloud edge vs on-premise edge — latency, data sovereignty, cost

**Tuesday**
- [ ] Lab: Connect Raspberry Pi to AWS IoT Core via MQTT (X.509 certificate auth)
- [ ] Lab: Use device shadow — update ESP32 LED state from AWS console

**Wednesday**
- [ ] Lab: AWS IoT Greengrass on Raspberry Pi — deploy a Lambda function that runs locally
- [ ] Study: Greengrass ML inference component — deploy TFLite model via Greengrass

**Thursday**
- [ ] Study: Azure IoT Hub + IoT Edge — module deployment via container
- [ ] Lab: Deploy a custom Python module to Azure IoT Edge on Raspberry Pi
- [ ] Study: IoT Edge routing — filter sensor data at the edge, send only anomalies to cloud

**Friday**
- [ ] Study: Data at rest and in transit — encryption requirements for cloud-connected edge devices
- [ ] Lab: Implement cloud-to-edge OTA using AWS IoT Jobs
- [ ] Document: Architecture diagram comparing AWS Greengrass vs Azure IoT Edge

**LinkedIn Post Topic — Week 21:**
*"Connected my Raspberry Pi to AWS IoT Core today. Device shadow, MQTT over TLS, X.509 certificates, OTA jobs. The cloud and the edge speak the same protocols — the difference is where computation and decisions happen. Understanding both is non-negotiable."*

---

### WEEK 22 — Edge Computing Architecture Patterns

**Monday**
- [ ] Study: Edge computing design patterns — gateway, filter, aggregator, federated learning
- [ ] Study: CAP theorem applied to edge — consistency, availability, partition tolerance at the edge

**Tuesday**
- [ ] Study: Containerization at the edge — Docker on ARM, minimal base images, resource constraints
- [ ] Lab: Dockerize your MQTT-InfluxDB-Grafana stack on Raspberry Pi
- [ ] Study: Kubernetes at the edge — k3s (lightweight Kubernetes) for edge clusters

**Wednesday**
- [ ] Study: Edge orchestration — balancing local autonomy with central management
- [ ] Study: Digital twin concepts — modeling physical assets in software
- [ ] Lab: Create a simple digital twin for a motor (state: running/stopped/fault, metrics: RPM, temp)

**Thursday**
- [ ] Study: Edge data pipelines — Apache Kafka, Node-RED, Apache NiFi at the edge
- [ ] Lab: Set up Node-RED on Raspberry Pi — build a visual data pipeline: MQTT → transform → InfluxDB
- [ ] Study: Event-driven architectures vs polling at edge scale

**Friday**
- [ ] Study: Multi-access edge computing (MEC) — 5G edge architecture
- [ ] Read: ETSI MEC architecture overview (skim, focus on concepts)
- [ ] Write a 1-page architecture proposal for a hypothetical industrial IoT system

**LinkedIn Post Topic — Week 22:**
*"Edge computing isn't just 'put a Raspberry Pi in a factory.' This week I studied real architecture patterns: how to filter data at the edge, aggregate before cloud upload, and handle partition tolerance when connectivity drops. Systems thinking is the skill that separates engineers from tinkerers."*

---

### WEEK 23 — Performance Optimization & Profiling at the Edge

**Monday**
- [ ] Study: Profiling embedded Linux applications — `perf`, `gprof`, `valgrind --tool=callgrind`
- [ ] Lab: Profile your IoT hub application — find the bottleneck

**Tuesday**
- [ ] Study: CPU affinity on multi-core (Raspberry Pi 4 has 4 cores) — pin real-time tasks to cores
- [ ] Lab: Use `taskset` to pin inference thread to core 3, leaving cores 0–2 for OS
- [ ] Measure: inference latency improvement with CPU affinity

**Wednesday**
- [ ] Study: Memory optimization — object pools, arena allocators, avoiding heap fragmentation
- [ ] Lab: Replace all `malloc` in a C program with a fixed-size pool allocator
- [ ] Study: Cache-friendly data structures — struct of arrays vs array of structs

**Thursday**
- [ ] Study: GPU/NPU acceleration on edge devices — NVIDIA Jetson Nano, Google Coral
- [ ] Study: OpenCL and CUDA basics (conceptual) — what hardware acceleration gives you
- [ ] Lab: Run TFLite with GPU delegate on Raspberry Pi; compare latency to CPU

**Friday**
- [ ] Study: Benchmarking methodology — warm-up, statistical significance, variance
- [ ] Lab: Write a proper benchmark suite for your inference pipeline — mean, p50, p95, p99 latency
- [ ] Document results as a performance report

**LinkedIn Post Topic — Week 23:**
*"p99 latency matters in edge computing. A system that's fast 99% of the time but stalls for 500ms on the 100th inference is unusable for real-time control. This week I learned to benchmark properly — not just averages, but percentiles. Engineers measure, they don't guess."*

---

### WEEK 24 — MLOps for Edge: Model Lifecycle & Monitoring

**Monday**
- [ ] Study: MLOps concepts — model versioning, CI/CD for ML, data drift, concept drift
- [ ] Study: Model monitoring at the edge — how do you know when to retrain?

**Tuesday**
- [ ] Lab: Implement model versioning — store model hash in firmware, report via MQTT telemetry
- [ ] Study: Active learning loops — edge device flags uncertain predictions for human labeling

**Wednesday**
- [ ] Study: Federated learning concepts — training on-device, no raw data leaves the edge
- [ ] Study: Privacy-preserving machine learning — differential privacy, secure aggregation (conceptual)

**Thursday**
- [ ] Lab: Build a drift detector — running average of inference confidence; alert when confidence drops
- [ ] Lab: Implement model A/B testing via device shadow — deploy model v2 to 50% of devices

**Friday**
- [ ] Study: Responsible edge AI — bias, fairness, explainability at inference time
- [ ] Write a 1-page MLOps runbook for your ESP32 anomaly detector: how to update, monitor, and rollback the model
- [ ] Commit runbook to repo

**LinkedIn Post Topic — Week 24:**
*"Deploying an AI model to 1000 edge devices is easy. Maintaining it, monitoring it, retraining it when the world changes — that's the hard part. This week I built a confidence drift detector. If my ESP32's model starts making uncertain predictions, I know before customers notice."*

---

### WEEK 25 — Capstone Project Build (Part 1)

> **Q4 Capstone Project: Intelligent Edge Monitoring System**
> A multi-node edge computing system: 2× ESP32 nodes continuously run anomaly detection inference on vibration data (TFLite Micro + Edge Impulse). A Raspberry Pi gateway aggregates results, runs a second-stage classification model, stores time-series data in InfluxDB, and forwards anomaly events to AWS IoT Core. Grafana dashboard shows real-time node health, inference confidence, and historical anomaly events. OTA model updates via AWS IoT Jobs.

**Monday**
- [ ] Architecture document — all components, data flows, failure modes, recovery strategies
- [ ] Hardware setup — mount ESP32s, connect IMUs, establish power supply design

**Tuesday**
- [ ] ESP32 firmware — TFLite Micro inference + MQTT publish + OTA partition
- [ ] Calibration routine — collect 10 minutes of normal operation data

**Wednesday**
- [ ] Train final Edge Impulse model — target: >95% accuracy on 3 classes (normal, minor fault, critical fault)
- [ ] Deploy to both ESP32 nodes — measure inference latency on each

**Thursday**
- [ ] Raspberry Pi gateway — MQTT subscriber, second-stage logic, InfluxDB ingestion
- [ ] AWS IoT Core integration — forward anomaly events as IoT events

**Friday**
- [ ] Grafana dashboard — node map, confidence trends, anomaly timeline, alert rules
- [ ] Integration test — simulate fault condition, verify alert chain end-to-end

**LinkedIn Post Topic — Week 25:**
*"Final project week begins. I'm building an end-to-end intelligent edge monitoring system — from sensor to cloud, with on-device AI inference. Everything from the past 6 months goes into this one build. The engineering is real. The stakes feel real. And I'm almost there."*

---

### WEEK 26 — Capstone Completion, Portfolio Polish & Career Readiness

**Monday**
- [ ] Stress test capstone — 48-hour continuous run, check for memory leaks, crashes, data loss
- [ ] Implement all remaining failure recovery paths

**Tuesday**
- [ ] Write comprehensive README — problem statement, architecture diagram, hardware BOM, setup guide, demo instructions
- [ ] Film 3-minute demo video — narrate the architecture and live inference

**Wednesday**
- [ ] Polish all 4 milestone projects — consistent README format, clean code, proper tags
- [ ] Create GitHub portfolio profile README — brief bio, project showcase, skills

**Thursday**
- [ ] Write a technical blog post on Medium or Dev.to — 1,000 words on one technical topic you mastered
- [ ] Update LinkedIn profile — headline, about section, featured projects, skills
- [ ] Request recommendations from anyone who has reviewed your work (communities, mentors)

**Friday**
- [ ] Apply to 3 embedded/edge engineer roles — entry-level, internship, or freelance
- [ ] Join communities: Embedded.fm Discord, r/embedded, EEVblog forums, Edge Impulse Discord
- [ ] Celebrate. Document what you would do differently.

**LinkedIn Post Topic — Week 26:**
*"6 months ago I wrote my first blink program. Today I deployed an AI model to an edge device that detects equipment faults in real time and alerts the cloud. Here's everything I built, everything I learned, and what I'm doing next. [Full 6-month breakdown thread]"*

---

## MILESTONE PROJECTS SUMMARY

### Q1 Milestone — Week 7: Digital Environmental Monitor
**Platform:** STM32 Nucleo-F401RE
**Skills demonstrated:** Bare-metal C, I2C driver (custom), ADC with DMA, timer interrupts, hardware abstraction layer
**Deliverable:** GitHub repo with clean C code, wiring diagram, Doxygen docs, demo video

### Q2 Milestone — Week 13: FreeRTOS Multi-Sensor Data Logger
**Platform:** STM32 + SD card module
**Skills demonstrated:** FreeRTOS task design, inter-task queues, DMA-UART pipeline, tickless idle, hardware watchdog
**Deliverable:** Architected repo with task diagram, unit tests, 1-hour stress test log file

### Q3 Milestone — Week 17: Raspberry Pi MQTT IoT Hub
**Platform:** Raspberry Pi 4 + 2× ESP32 nodes
**Skills demonstrated:** Linux systems programming, MQTT with TLS, InfluxDB + Grafana, OTA updates, systemd services
**Deliverable:** Dockerized stack, ESP32 firmware repo, live dashboard screenshot, architecture diagram

### Q4 Capstone — Week 26: Intelligent Edge Monitoring System
**Platform:** ESP32 (×2) + Raspberry Pi + AWS IoT Core
**Skills demonstrated:** TFLite Micro inference, Edge Impulse MLOps, cloud-edge integration, model OTA, full-stack dashboard, performance benchmarking
**Deliverable:** Fully documented multi-repo project, 3-minute demo video, technical blog post

---

## PROFESSIONAL BEST PRACTICES

These are habits consistently associated with successful engineers in the field, backed by industry survey data and career research.

### Code Quality & Engineering Rigor
- **Read the datasheet first.** Not the tutorial, not Stack Overflow — the datasheet. Engineers who read primary sources debug 3× faster.
- **Never poll when you can interrupt.** Never interrupt when you can DMA. Performance optimization is a mindset, not a task.
- **Every function has a single responsibility.** Functions longer than 40 lines are a design smell in embedded C.
- **Version everything on day one.** Git is not optional. Tag every working release. Your future self will thank you.
- **Write the worst case first.** What happens on power failure? On sensor disconnect? On I2C bus lockup? Production embedded systems must handle every failure mode.
- **Test on hardware, not just simulation.** Logic analyzers and oscilloscopes are the embedded engineer's debugger. Learn to read waveforms.

### Documentation Culture
- **Document what you decided, not just what you built.** Architecture Decision Records (ADRs) — one paragraph per major technical choice explaining why you chose it over alternatives.
- **Every hardware project has a wiring diagram.** `draw.io` is free. No excuse.
- **Your README is your first interview.** If a recruiter opens your repo and can't understand what it does in 30 seconds, you've already lost.

### Community & Continuous Learning
- **Listen to Embedded.fm and The Amp Hour podcasts** — the closest thing to having a senior mentor on demand.
- **Contribute to open-source embedded projects** — submitting even one bug fix to FreeRTOS, Zephyr RTOS, or TFLite Micro puts you ahead of 95% of applicants.
- **Engage with the community before you need a job.** Answering questions on forums, sharing projects, writing blog posts — these create inbound opportunities.
- **Attend (or watch) CppCon, EmbeddedWorld, and tinyML Summit recordings** — these are the conferences where the field's direction is set.

### Career & Portfolio
- **Specificity beats breadth in portfolios.** One deeply understood project is worth ten superficial ones. Know every line of your Q4 capstone.
- **GitHub contribution graph matters.** Consistent green squares signal discipline. Code every weekday, even if just a small fix or improvement.
- **Write publicly.** A technical blog post explaining one concept clearly is visible proof of both knowledge and communication skill — a rare combination in engineering.
- **Apply early.** Job-readiness is not a destination. Apply at the end of Month 4 while still building. Interviews are learning opportunities.
- **Target seed-stage and Series A startups.** They give junior engineers the most ownership, the broadest scope, and the fastest career growth. Large companies hire specialists; startups need generalists who can ship.

### Edge Computing Specific
- **Think in tradeoffs, not solutions.** Every edge decision — local vs cloud inference, battery vs accuracy, latency vs bandwidth — is a tradeoff with no universal right answer. Engineers who articulate tradeoffs clearly are the ones who get hired.
- **Security is not a feature, it is a requirement.** Every device you ship has an attack surface. Know the OWASP IoT Top 10 and audit your own work against it.
- **Measure before you optimize.** Profile before you assume. The bottleneck is never where you think it is.
- **Design for the update.** A device in the field that can't receive OTA updates is a ticking time bomb. Build OTA in from day one.

---

## RESOURCE INDEX

### Books
- *Making Embedded Systems* — Elecia White (O'Reilly) — the definitive beginner text
- *Programming Embedded Systems* — Michael Barr & Anthony Massa — systems programming depth
- *Real-Time C++* — Christopher Kormanyos — for Cortex-M C++ development
- *Designing Embedded Hardware* — John Catsoulis — hardware fundamentals

### Online Courses
- **Fastai Practical Deep Learning for Coders** — accessible ML foundation
- **Edge Impulse Coursera courses** — official edge ML curriculum
- **Udemy: Embedded Systems Bare Metal Programming in C** — STM32 focused

### Key Documentation
- STM32F4 Reference Manual (RM0368) — your primary STM32 source
- FreeRTOS Mastering the FreeRTOS Kernel (free PDF from freertos.org)
- ESP-IDF Programming Guide — docs.espressif.com
- TensorFlow Lite Micro documentation — tensorflow.org/lite/microcontrollers

### Communities
- **Embedded.fm Discord** — active professional community
- **r/embedded** — breadth of experience levels, good for questions
- **Edge Impulse Forum** — edge ML specific
- **EEVblog Forums** — hardware and electronics depth
- **Zephyr Project Slack** — RTOS for production IoT (worth learning after Month 6)

---

*Roadmap Version 1.0 | 26 Weeks | Weekdays Only | 4–5 Hours/Day*
*Total estimated study hours: ~575–650 hours*
