#ifndef _WIRING_DIGITAL_EXTRA_H
#define _WIRING_DIGITAL_EXTRA_H

#include "gd32/PinConfigured.h"
#include "gd32/PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

void digitalToggle(pin_size_t ulPin);

#ifdef __cplusplus
}
#endif

#endif /* _WIRING_DIGITAL_EXTRA_H */