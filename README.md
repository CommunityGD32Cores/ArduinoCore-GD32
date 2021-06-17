# GD32 Arduino Core (New) 

This is a Arduino core is based off of the original GigaDevice core that was provided by the company in early June 2021 (see https://github.com/CommunityGD32Cores/GD32Core/)

It is currently a work in progress, but believed to be functional on GigaDevice's mBed boards.

The intention is to further develop that original core in an open-source, community-driven manner.

# Media

A GD32F303CC chip (placed on a bluepill PCB) runs its first blinky code with the new Arduino core!

![first blinky](https://github.com/CommunityGD32Cores/GD32Core-New/raw/main/docs/first_blinky_gd32f303cc.mp4)

# Using this core

Currently, development of this core is being done using PlatformIO. It uses the custom PlatformIO platform https://github.com/maxgerhardt/platform-gd32. 

Using PlatformIO is already possible to very easily edit code in the IDE and even live-debug a chip (with e.g. an ST-Link)

Example projects for this platform for the SPL framework and this Arduino core are currently hosted at https://github.com/maxgerhardt/gd32-pio-projects, but are subject to be moved soon.

# Current state

The [gd32-arduino-blinky](https://github.com/maxgerhardt/gd32-pio-projects/tree/main/gd32-arduino-blinky) project compiles for the `gd32f307_mbed` board. 

```
CONFIGURATION: https://docs.platformio.org/page/boards/gd32/gd32f307_mbed.html
PLATFORM: GD GD32 (1.0.0) > GD32F307VG-mbed (48k RAM. 256k Flash)
HARDWARE: GD32F307VGT6 120MHz, 96KB RAM, 1MB Flash
DEBUG: Current (stlink) External (blackmagic, cmsis-dap, jlink, stlink)
PACKAGES:
 - framework-arduinogd32 4.20000.210603 (2.0.0)
 - toolchain-gccarmnoneeabi 1.90201.191206 (9.2.1)
[...]
Checking size .pio\build\gd32f307_mbed\firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [          ]   0.2% (used 164 bytes from 98304 bytes)
Flash: [          ]   0.6% (used 6128 bytes from 1048576 bytes)
Building .pio\build\gd32f307_mbed\firmware.bin
============= [SUCCESS] Took 7.38 seconds =============
```

Not yet testable since I do not have a GD32F307VG-mbed board, will be developing on a GD32F303CC instead.

# Development log

*18.06.2021:*
* Add `package.json` for PlatformIO compatibility
* Add `tools\platformio\platformio-build.py` PlatformIO builder script
* gives a successful build for the gd32f307_mbed board

# ToDo / thoughts

* [ ] add general variant for GD32F303CC
* [ ] check the working state of currently implemented code (GPIO, PWM, I2C, SPI, UART, USB, ...)
    * [ ] note down what works and what doesn't
    * [ ] prioritize implementation and implement it
* [ ] USB bootloader?
* [ ] write good documentation on how to add new variants and how the build process works
* [ ] release Arduino-IDE compatible package