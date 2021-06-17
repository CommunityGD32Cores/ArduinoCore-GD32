# GD32 Arduino Core (New) 

This is a Arduino core is based off of the original GigaDevice core that was provided by the company in early June 2021 (see https://github.com/CommunityGD32Cores/GD32Core/)

It is currently a work in progress, but believed to be functional on GigaDevice's mBed boards.

The intention is to further develop that original core in an open-source, community-driven manner.

# Media

Pictures of boards running this Arduino core soon!

# Using this core

Currently, development of this core is being done using PlatformIO. It uses the custom PlatformIO platform https://github.com/maxgerhardt/platform-gd32. 

Using PlatformIO is already possible to very easily edit code in the IDE and even live-debug a chip (with e.g. an ST-Link)

Example projects for this platform for the SPL framework and this Arduino core are currently hosted at https://github.com/maxgerhardt/gd32-pio-projects, but are subject to be moved soon.

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