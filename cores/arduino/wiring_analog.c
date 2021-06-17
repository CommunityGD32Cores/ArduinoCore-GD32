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

static uint32_t analogOut_resolution = DAC_ALIGN_12B_R;

#define MAX_ADC_RESOLUTION 12
static uint32_t set_Resolution = ADC_RESOLUTION;
static uint32_t analogIn_resolution =
#if ADC_RESOLUTION > MAX_ADC_RESOLUTION
    MAX_ADC_RESOLUTION
#else
#ifdef ADC_RESOLUTION_12B
#if ADC_RESOLUTION <= 6 && defined(ADC_RESOLUTION_6B)
    6
#elif ADC_RESOLUTION <= 8 && defined(ADC_RESOLUTION_8B)
    8
#elif ADC_RESOLUTION <= 10 && defined(ADC_RESOLUTION_10B)
    10
#elif ADC_RESOLUTION <= 12
    12
#endif
#else  /* ADC_RESOLUTION_12B */
    12
#endif /* ADC_RESOLUTION_12B */
#endif /* ADC_RESOLUTION > MAX_ADC_RESOLUTION */
    ;

void analogReadResolution(int res)
{
    if((res > 0) && (res <= 32)) {
        set_Resolution = res;
        analogIn_resolution = set_Resolution;
        if(set_Resolution > MAX_ADC_RESOLUTION) {
            analogIn_resolution = MAX_ADC_RESOLUTION;
        } else {
#ifdef ADC_RESOLUTION_12B
#ifdef ADC_RESOLUTION_6B
            if(analogIn_resolution <= 6) {
                analogIn_resolution = 6;
            } else
#endif
#ifdef ADC_RESOLUTION_8B
                if(analogIn_resolution <= 8) {
                    analogIn_resolution = 8;
                } else
#endif
#ifdef ADC_RESOLUTION_10B
                    if(analogIn_resolution <= 10) {
                        analogIn_resolution = 10;
                    } else
#endif
                        if(analogIn_resolution <= 12) {
                            analogIn_resolution = 12;
                        }
#else
            analogIn_resolution = 12;
#endif
        }
    }
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


void analogReference(enum analogReferenceType ulMode)
{

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
        value = adc_read_value(p, analogIn_resolution);
        value = mapResolution(value, analogIn_resolution, set_Resolution);
    }
    return value;
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// variant.cpp file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint32_t ulPin, uint32_t ulValue)
{
    PinName pinname = DIGITAL_TO_PINNAME(ulPin);
    dac_write_value(pinname, ulValue, analogOut_resolution);
}

void analogWriteResolution(int res)
{
    switch(res) {
        case 8:
            analogOut_resolution = DAC_ALIGN_8B_R;
            break;
        case 12:
            analogOut_resolution = DAC_ALIGN_12B_R;
            break;
        default:
            analogOut_resolution = DAC_ALIGN_12B_R;
    }
}

#ifdef __cplusplus
}
#endif
