/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

    Based on mbed-os/hal/include/hal/analogin_api.c
    Based on mbed-os/hal/include/hal/analogout_api.c
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ANALOG_H
#define __ANALOG_H

/* Includes ------------------------------------------------------------------*/
#include "gd32_def.h"
#include "PeripheralPins.h"
#include "PeripheralNames.h"

#ifdef __cplusplus
extern "C" {
#endif

struct dac_s {
    DACName dac;
    PinName pin;
    uint32_t channel;
};

struct analogin_s {
    ADCName adc;
    PinName pin;
    uint8_t channel;
};

typedef struct dac_s dac_t;

#define DEV_DAC_ACCURACY_12BIT (0xFFF)
#define DEV_DAC_ACCURACY_8BIT (0xFF)

#define DEV_ADC_ACCURACY_12BIT 0xFFF
#define DEV_ADC_PRECISION_12TO16(val) ((val << 4) | ((val >> 8) & (uint16_t)0x000F))
#define AND_NUMBER (0xFF)

#ifndef GD_PIN_CHANNEL_GET
#define GD_PIN_CHANNEL_GET(X) ((X >> 11) & 0x1F)
#endif

/* Exported functions ------------------------------------------------------- */
void dac_write_value(PinName pin, uint32_t value, uint8_t resolution);
void dac_stop(PinName pin);
uint16_t adc_read_value(PinName pin, uint32_t resolution);

#ifdef __cplusplus
}
#endif

#endif /* __ANALOG_H */
