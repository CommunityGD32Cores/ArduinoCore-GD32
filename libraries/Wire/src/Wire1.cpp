#include "Wire.h"

#if defined(HAVE_I2C1)
TwoWire Wire1(SDA1, SCL1, 1);
#endif
