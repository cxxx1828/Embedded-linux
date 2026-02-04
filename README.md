# Embedded Linux Labs

This repository contains code and resources for lab assignments from the "Linux Operating System in Embedded Computers" course. The labs focus on Linux kernel development for Raspberry Pi, including custom modules, BusyBox integration, I2C devices, and device tree modifications.

## Overview
The course covers embedded Linux topics such as:
- Kernel configuration and compilation for Raspberry Pi.
- Developing character drivers with read/write/ioctl operations.
- Integrating BusyBox for minimal root filesystems.
- Working with I2C peripherals (e.g., Nunchuk controller) and LED control.
- Device tree (DTS) modifications for hardware integration.

Assignments are organized into branches for each lab:
- `driver`: Lab 1 - Simple character driver with buffer management and string processing.
- `busybox`: Lab 2 - BusyBox configuration, hello module with ioctl, and root filesystem setup.
- `zad3`: Lab 3 - Nunchuk LED driver, platform bus integration, and device tree updates.

## Setup and Requirements
- **Hardware**: Raspberry Pi (tested on models compatible with the specified kernel commit).
- **Tools**: QEMU for emulation (optional), cross-compiler (e.g., `arm-none-eabi-gcc`), Git.
- **Kernel Source**: Based on Raspberry Pi Linux kernel from commit `8e1110a580887f4b82303b9354c25d7e2ff5860e`.
- **Root Filesystem**: Use `nfsroot/` with unpacked `rootfs.tar.bz2` for NFS mounting.

### Installation Steps
1. Clone the repository:
