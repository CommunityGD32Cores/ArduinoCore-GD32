# GD32 Arduino Core (New) 

This is a Arduino core is based off of the original GigaDevice core that was provided by the company in early June 2021 (see https://github.com/CommunityGD32Cores/GD32Core/)

It is currently a work in progress, but believed to be functional on GigaDevice's mBed boards.

The intention is to further develop that original core in an open-source, community-driven manner.

## Collaborating

Interested in collaborating? Join our dedicated Discord channel for this at https://discord.gg/59kf4JxsRM.

# Media

A GD32F303CC chip (placed on a bluepill PCB) runs its first blinky code with the new Arduino core!


https://user-images.githubusercontent.com/26485477/122486275-05609280-cfd9-11eb-8a90-dbb0f96968c9.mp4

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

Compilation now also works for the `genericGD32F303CC` board **and** works, as seen above
```
RAM:   [          ]   0.3% (used 164 bytes from 49152 bytes)
Flash: [          ]   2.3% (used 6104 bytes from 262144 bytes)
Building .pio\build\genericGD32F303CC\firmware.bin
=== [SUCCESS] Took 5.38 seconds ===
```

# Updates / History

_31.05.2021:_

Initial contact and thoughts about an Arduino core implementation from scratch for GD32 devices with @kemotz via Email.

_02.06.2021:_

Creation of the Github project https://github.com/maxgerhardt/gd32-arduino-core/ and a discord channel.

_10.06.2021:_

A custom dev board has been designed and is in production. The repo with the files for it is at https://github.com/kemotz/GD32F1x0-dev-brd. 

![board_preview](https://user-images.githubusercontent.com/26485477/122487075-a69c1880-cfda-11eb-9675-d4d582f6e6e4.png)


[@obra](https://github.com/obra) and [@algernon](https://github.com/algernon) join the project. 

GigaDevices is contact with a request for information on a potentially existing in-house developed Arduino core and code licensing questions.

*16.06.2021:*

GigaDeviecs confirms that there is a internally developed Arduino core, sends it over to use and approves of publishing it. The code is also BSD 3-clause licensed. 

The original files for this are found at https://github.com/CommunityGD32Cores/GD32Core.

The focus shifts from creating a new Arduino core from scratch to getting the retrieved one working and expanding upon it.

*18.06.2021:*

Arduino core:
* Add `package.json` for PlatformIO compatibility
* Add `tools\platformio\platformio-build.py` PlatformIO builder script
* gives a successful build for the gd32f307_mbed board
* gives a successful build for the genericGD32F303CC board (and gives a working blinky!)

# ToDo / thoughts

* [ ] add general variant for GD32F303CC
* [ ] check the working state of currently implemented code (GPIO, PWM, I2C, SPI, UART, USB, ...)
    * [ ] note down what works and what doesn't
    * [ ] prioritize implementation and implement it
* [ ] USB bootloader?
* [ ] write good documentation on how to add new variants and how the build process works
* [ ] release Arduino-IDE compatible package
