#ifndef _WIRING_ANALOG_EXTRA_H
#define _WIRING_ANALOG_EXTRA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void analogReadResolution(int res);

void analogWriteResolution(int res);
void analogWriteFrequency(uint32_t freq_hz);

#ifdef __cplusplus
}
#endif

#endif /* _WIRING_ANALOG_EXTRA_H */