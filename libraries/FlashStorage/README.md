# FlashStorage

This is a library that implements a way to read and write data from and to
flash, from within the running program.

## Design decisions

There are different ways to do that, with different trade-offs. The goal during
the design of this library was simplicity, and to fulfill the particular goals
of the Kaleidoscope firmware.

As such, the storage in this implementation is always at the end of Flash. This
allows us to reserve space for it externally, and prevent ever overwriting it,
so when we flash new firmware, the storage area is not destroyed. As a
consequence, we can only have one storage area, but its size can be changed at
compile time. On the other hand, all of this allows the code to be fairly simple
and straightforward.

## FlashAsEEPROM

For practical reasons, the library includes a `FlashAsEEPROM.h` header, a class
that implements an Arduino-esque EEPROM emulation layer on top of
`FlashStorage`. The header does not export an `EEPROM` instance object, that's
what the `EEPROM` library does. A separate library, for practical and
convenience reasons.
