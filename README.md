# UTMS9_a+cm

## What Is This?

UTMS9_a+cm is a lightweight real-time kernel and Software Development Kit (SDK) for ARM Cortex-M microcontrollers. It is not a complete operating system. It is a base layer for motion control systems.

You use this project to build firmware for CNC machines, 3D printers, laser engravers, and other machines that need precise motor control.

## Goal

The goal of UTMS9_a+cm is to give developers a fast, reliable, and small kernel. It must be easy to build a complete firmware on top of it. The kernel handles real-time tasks. The SDK gives you ready-to-use blocks for machine control.

You do not write a kernel from zero. You take UTMS9_a+cm, add your machine configuration, and build your firmware.

## How To Use

### Step 1: Build the Kernel

Clone this repository and build the kernel with UMK:

```bash
git clone https://github.com/user12ms4c/utms9_a_cm
cd utms9_a_cm
umk build
```

You get `utms9.bin`. You can flash this file to your board.

### Step 2: Configure Your Machine

Create a configuration file in `machine/your_machine/`. Define your pins, axes, steps per millimeter, and max speed.

### Step 3: Build Your Firmware

Write your application code. Use the SDK functions to control axes, parse G-code, and manage motion. Link your code with the kernel. Build your final firmware with UMK.

### Step 4: Send G-code

Connect to your board with a UART adapter. Send G-code commands:

```text
G1 X10 Y20 F1200
```

The kernel parses the command, plans the trajectory, and generates STEP pulses.

## Features

- Preemptive priority-based scheduler with time blocking.
- Hardware timer and DMA support for STEP pulse generation.
- G-code parser with basic G0/G1 support.
- Trajectory planner with Look-ahead algorithm.
- Motion profile generator for acceleration and deceleration.
- Flexible axis abstraction for CNC and 3D printers.
- UART driver for logging and G-code input.
- Minimal footprint. No standard library. No dynamic allocation in the core.

## Architecture

The project has three levels:

### Kernel

The kernel provides basic primitives:
- Task scheduler.
- Context switch.
- Critical sections.
- System timer (SysTick).

### HAL (Hardware Abstraction Layer)

The HAL provides hardware drivers:
- GPIO.
- UART.
- Hardware timers with DMA.

### SDK (Software Development Kit)

The SDK provides machine-level abstractions:
- Axis management.
- G-code parser.
- Trajectory planner.
- Motion profile generator.
- Kinematics.

## Supported Hardware

The current target is STM32F407 (ARM Cortex-M4F).

Supported peripherals:
- TIM5 for X-axis STEP pulses.
- TIM2 for Y-axis STEP pulses.
- TIM3 for Z-axis STEP pulses.
- TIM4 for E-axis STEP pulses.
- USART2 for G-code input and logging.

## Build Dependencies

You need the following tools:
- arm-none-eabi-gcc
- arm-none-eabi-as
- arm-none-eabi-ld
- arm-none-eabi-objcopy
- [UMK](https://github.com/user12msd4c/umk) build system

Install UMK from the official repository:

```bash
git clone https://github.com/user12msd4c/umk
cd umk
gcc -O3 -Wall -Wextra -o umk umk.c
sudo cp umk /usr/local/bin/
```

## License

MIT License

