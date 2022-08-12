#include "Arduino.h"
#include "HardwareSerial.h"

// Place each serial in its own .cpp file so that the linker cleans it up
// if the user sketch doesn't refer to it.
// otherwise we pay the RAM for *all* serial objects.

#if defined(HAVE_HWSERIAL2)
HardwareSerial Serial2(RX1, TX1, 1);
#endif