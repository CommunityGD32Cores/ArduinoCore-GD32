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

    Based on mbed-os/targets/TARGET_GigaDevice/TARGET_GD32F30X/analogin_api.c
    Based on mbed-os/targets/TARGET_GigaDevice/TARGET_GD32F30X/analogout_api.c
*/

#include "gd32_def.h"
#include "analog.h"
#include "gd32f30x.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct analogin_s analogin_t;
FlagStatus temperature_sample_flag = RESET;

/**
  * @brief  This function will set the DAC to the required value
  * @param  pin : the gpio pin to use
  * @param  value : the value to push on the dac output
  * @param  resolution : dac resolution to configure
  * @retval None
  */
void dac_write_value(PinName pin, uint32_t value, uint8_t resolution)
{
    dac_t dac_obj;
    uint32_t function;

    dac_obj.dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);
    if(NC == dac_obj.dac) {
        return;
    }

    function = pinmap_function(pin, PinMap_DAC);
    dac_obj.channel = GD_PIN_CHANNEL_GET(function);

    pinmap_pinout(pin, PinMap_DAC);

    /* save the pin for future use */
    dac_obj.pin = pin;

    dac_deinit();

    /* enable DAC clock */
    rcu_periph_clock_enable(RCU_DAC);

    /* configure DAC */
    dac_wave_mode_config(dac_obj.dac, DAC_WAVE_DISABLE);
    dac_trigger_disable(dac_obj.dac);
    dac_output_buffer_enable(dac_obj.dac);

    switch(resolution) {
        case DAC_ALIGN_8B_R:
            dac_data_set(dac_obj.dac, DAC_ALIGN_8B_R, (value & DEV_DAC_ACCURACY_12BIT));
            break;
        case DAC_ALIGN_12B_R:
            dac_data_set(dac_obj.dac, DAC_ALIGN_12B_R, (value & DEV_DAC_ACCURACY_8BIT));
            break;
    }

    dac_enable(dac_obj.dac);
}

/**
  * @brief  This function will stop the DAC
  * @param  pin : the gpio pin to use
  * @retval None
  */
void dac_stop(PinName pin)
{
    dac_t dac_obj;

    dac_obj.dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);
    if(NC == dac_obj.dac) {
        return;
    }

    dac_deinit();
    dac_disable(dac_obj.dac);
}

/**
 * @brief  software delay
 * @param  time : The time need to delay
 * @retval none
 */
static void _delay(uint16_t time)
{
    uint16_t i;
    for(i = 0; i < time; i++) {
    }
}

/**
 * @brief  Initialize the analogin peripheral, configures the pin used by analogin.
 * @param  obj : The analogin object to initialize
 * @param  pin : The analogin pin name
 * @param  resolution : adc resolution to configure
 * @retval intialization is succeed or not
 */
uint16_t analogin_init(analogin_t *obj, PinName pin, uint32_t resolution)
{
    uint32_t periph;
    uint32_t function = pinmap_function(pin, PinMap_ADC);

    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);

    if(obj->adc == NC) {
        return 0;
    }

    obj->channel = GD_PIN_CHANNEL_GET(function);

    periph = obj->adc;
    /* save the pin for future use */
    obj->pin = pin;

    /* ADC clock enable and pin number reset */
    switch(periph) {
        case ADC0:
            rcu_periph_clock_enable(RCU_ADC0);
            break;

        case ADC1:
            rcu_periph_clock_enable(RCU_ADC1);
            /* reset pin number */
            pin = (PinName)(pin & AND_NUMBER);
            break;
    }

    /* ADC clock cannot be greater than 40MHz */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);

    if((ADC_CHANNEL_16 == obj->channel)) {
        /* ADC Vrefint enable */
        adc_tempsensor_vrefint_enable();
        /* set temperature sample flag */
        temperature_sample_flag = SET;
    }
    if((ADC_CHANNEL_17 == obj->channel)) {
        /* ADC Vrefint enable */
        adc_tempsensor_vrefint_enable();
    }

    /* when pin >= ADC_TEMP, it indicates that the channel has no external pins */
    if(pin < ADC_TEMP) {
        pinmap_pinout(pin, PinMap_ADC);
    }

    /* ADC configuration */
    adc_special_function_config(obj->adc, ADC_SCAN_MODE, DISABLE);
    adc_special_function_config(obj->adc, ADC_CONTINUOUS_MODE, DISABLE);
    /* ADC trigger config */
    adc_external_trigger_source_config(obj->adc, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE);
    /* ADC data alignment config */
    adc_data_alignment_config(obj->adc, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(obj->adc, ADC_REGULAR_CHANNEL, 1);

    if(temperature_sample_flag == SET) {
        /* sample temperature needs more time */
        adc_regular_channel_config(obj->adc, 0, obj->channel, ADC_SAMPLETIME_239POINT5);
        /* clear temperature sample flag */
        temperature_sample_flag = RESET;
    } else {
        adc_regular_channel_config(obj->adc, 0, obj->channel, ADC_SAMPLETIME_28POINT5);
    }
    adc_external_trigger_config(obj->adc, ADC_REGULAR_CHANNEL, ENABLE);

    switch(resolution) {
        case 6:
            adc_resolution_config(obj->adc, ADC_RESOLUTION_6B);
            break;
        case 8:
            adc_resolution_config(obj->adc, ADC_RESOLUTION_8B);
            break;
        case 10:
            adc_resolution_config(obj->adc, ADC_RESOLUTION_10B);
            break;
        case 12:
            adc_resolution_config(obj->adc, ADC_RESOLUTION_12B);
            break;
    }

    /* ADC enable */
    adc_enable(obj->adc);
    /* wait for ADC to stabilize */
    _delay(500);
    adc_calibration_enable(obj->adc);

    return 1;
}

/**
 * @brief  Read the input voltage, represented as a hex in the range [0, 0xFFF]
 * @param  obj : The analogin object
 * @retval 16-bit value representing the current input voltage
 */
uint16_t analogin_read(analogin_t *obj)
{
    uint16_t reval;

    adc_flag_clear(obj->adc, ADC_FLAG_EOC);
    /* start Conversion */
    adc_software_trigger_enable(obj->adc, ADC_REGULAR_CHANNEL);
    /* wait for conversion to complete */
    while(SET != adc_flag_get(obj->adc, ADC_FLAG_EOC)) {
    }
    /* ADC actual accuracy is 12 bits */
    reval = adc_regular_data_read(obj->adc);

    return reval;
}

/**
 * @brief  Read the input voltage, represented as a hex in the range [0, 0xFFF]
 * @param  pin : The analogin pin name
 * @param  resolution : adc resolution to configure
 * @retval 16-bit value representing the current input voltage
 */
uint16_t adc_read_value(PinName pin, uint32_t resolution)
{
    __IO uint16_t adc_value = 0;
    analogin_t adc_obj;
    uint16_t ret = 0;

    ret = analogin_init(&adc_obj, pin, resolution);
    if(0 == ret) {
        return 0;
    }

    adc_value = analogin_read(&adc_obj);

    adc_disable(adc_obj.adc);

    adc_deinit(adc_obj.adc);

    return adc_value;
}


#ifdef __cplusplus
}
#endif
