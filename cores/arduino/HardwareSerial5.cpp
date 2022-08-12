#include "Arduino.h"
#include "HardwareSerial.h"

// Place each serial in its own .cpp file so that the linker cleans it up
// if the user sketch doesn't refer to it.
// otherwise we pay the RAM for *all* serial objects.

#if defined(HAVE_HWSERIAL5)
HardwareSerial Serial5(RX4, TX4, 4);
#endif