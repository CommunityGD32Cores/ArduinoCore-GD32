#include "Wire.h"

#if defined(HAVE_I2C2)
TwoWire Wire2(SDA2, SCL2, 2);
#endif
