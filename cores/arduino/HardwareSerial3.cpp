#include "Arduino.h"
#include "HardwareSerial.h"

// Place each serial in its own .cpp file so that the linker cleans it up
// if the user sketch doesn't refer to it.
// otherwise we pay the RAM for *all* serial objects.

#if defined(HAVE_HWSERIAL3)
HardwareSerial Serial3(RX2, TX2, 2);
#endif