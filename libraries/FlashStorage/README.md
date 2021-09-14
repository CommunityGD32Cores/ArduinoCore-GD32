# FlashStorage

This is a library that implements a way to read and write data from and to
flash, from within the running program.

## Design decisions

There are different ways to do that, with different trade-offs. The goal during
the design of this library was simplicity, and to fulfill the particular goals
of the Kaleidoscope firmware. One of those goals is to make it easy to work with
the storage, even when firmware is flashed frequently.

For this reason, the storage defaults to being at the end of the flash, for
which we can reserve space externally, and prevent accidentally overwriting it
with program code. This way, when flashing new firmware, the storage area is not
destroyed.

It is still possible to have multiple storage areas, as `FlashStorage` allows
specifying both a size and an end address, but care must be taken to not overlap
with program code.

## FlashAsEEPROM

For practical reasons, the library includes a `FlashAsEEPROM.h` header, a class
that implements an Arduino-esque EEPROM emulation layer on top of
`FlashStorage`. The header does not export an `EEPROM` instance object, that's
what the `EEPROM` library does. A separate library, for practical and
convenience reasons.
