/*
 Copyright (c) 2011 Arduino.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Arduino.h"
#include "PinConfigured.h"
#include "analog.h"
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ANALOG_OUT_RESOLUTION 12

static uint32_t analogIn_resolution = 10;
static uint32_t analogOut_resolution = 8;

void analogReference(enum analogReferenceType ulMode)
{

}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to)
{
    if(from != to) {
        if(from > to) {
            value = (value < (uint32_t)(1 << (from - to))) ? 0 : ((value + 1) >> (from - to)) - 1;
        } else {
            if(value != 0) {
                value = ((value + 1) << (to - from)) - 1;
            }
        }
    }
    return value;
}

// Perform the read operation on the selected analog pin.
// the initialization of the analog PIN is done through this function
uint32_t analogRead(uint32_t ulPin)
{
    uint32_t value = 0;
    PinName p = DIGITAL_TO_PINNAME(ulPin);
    switch(ulPin) {
        case ADC_TEMP:
            p = ADC_TEMP;
            break;
        case ADC_VREF:
            p = ADC_VREF;
            break;
        default:
            break;
    }
    if(p != NC) {
        value = get_adc_value(p);
        value = mapResolution(value, 12, analogIn_resolution);
    }
    return value;
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// variant.cpp file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint32_t ulPin, uint32_t ulValue)
{
    uint32_t value;
    PinName pinname = DIGITAL_TO_PINNAME(ulPin);
    if(pin_in_pinmap(pinname, PinMap_DAC)) {
        value = mapResolution(ulValue, analogOut_resolution, MAX_ANALOG_OUT_RESOLUTION);
        set_dac_value(pinname, value);
    } else if(pin_in_pinmap(pinname, PinMap_PWM)) {
        value = mapResolution(ulValue, analogOut_resolution, 16);
        set_pwm_value(ulPin, value);
    } else {
        // Defaults to digital write
        pinMode(ulPin, OUTPUT);
        value = mapResolution(ulValue, analogOut_resolution, 8);
        if(value < 128) {
            digitalWrite(ulPin, LOW);
        } else {
            digitalWrite(ulPin, HIGH);
        }
    }
}

//analog input resolution
void analogReadResolution(int res)
{
    if((res > 0) && (res < 16)) {
        analogIn_resolution = res;
    } else {
        analogIn_resolution = 8;
    }
}

//analog output resolution
void analogWriteResolution(int res)
{
    if((res > 0) && (res < 16)) {
        analogOut_resolution = res;
    } else {
        analogOut_resolution = 8;
    }
}

#ifdef __cplusplus
}
#endif
