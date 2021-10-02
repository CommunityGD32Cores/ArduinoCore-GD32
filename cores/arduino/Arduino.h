/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 LeafLabs LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/


#ifndef Arduino_h
#define Arduino_h

#include "api/ArduinoAPI.h"

#if defined(__cplusplus)
using namespace arduino;
#endif

#include "gd32xxyy.h"
#include "gd32/pins_arduino.h"
#include "gd32/PeripheralPins.h"


#ifdef __cplusplus
/* include outside of extern C block, this is basically a C++ library */
#include "pwm.h"

extern "C" {
#endif /* __cplusplus */
#include "gd32/systick.h"
#include "analog.h"
#include "wiring_analog_extra.h"
#include "gd32/gpio_interrupt.h"
#include "gd32/timer.h"
#include "gd32/rtc.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */
#ifdef __cplusplus



void init(void);
void setup();
void loop();

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef __cplusplus
}
#endif // __cplusplus

#include "variant.h"
#include "HardwareSerial.h"

#endif
#endif
