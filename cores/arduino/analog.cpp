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

#include "Arduino.h"
#include "analog.h"
#include "pwm.h"

#if defined(DAC0) && defined(DAC1)
#define DAC_NUMS  2
#elif defined(DAC) || defined(DAC0)
#define DAC_NUMS  1
#else 
#define DAC_NUMS  0
#endif

#define PWM_NUMS  40

#if defined(GD32F30x) 
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
    #define ADC_NUMS  3
#else
    #define ADC_NUMS  2
#endif
#elif defined(GD32F3x0)
    #define ADC_NUMS  1
#elif defined(GD32F1x0)
    #define ADC_NUMS  1
#endif

#if DAC_NUMS != 0
analog_t DAC_[DAC_NUMS] = {0};
#endif
analog_t ADC_[ADC_NUMS] = {0};

// dac write value
void set_dac_value(PinName pinname, uint16_t value)
{
#if DAC_NUMS != 0
    uint32_t dac_periph = pinmap_peripheral(pinname, PinMap_DAC);
    uint8_t index = get_dac_index(dac_periph);
    if(!DAC_[index].isactive) {
        pinmap_pinout(pinname, PinMap_DAC);
        rcu_periph_clock_enable(RCU_DAC);
        dac_deinit();
#if (defined(GD32F1x0) && defined(GD32F170_190)) || defined(GD32F30x)
        dac_trigger_disable(dac_periph);
        #if defined(GD32F30x)
        dac_wave_mode_config(dac_periph, DAC_WAVE_DISABLE);
        #endif
        dac_output_buffer_enable(dac_periph);
        dac_enable(dac_periph);
        dac_data_set(dac_periph, DAC_ALIGN_12B_R, value);
#elif defined(GD32F1x0) && !defined(GD32F170_190)
        dac0_trigger_disable();
        dac0_output_buffer_enable();
        dac0_enable();
        dac0_data_set(DAC_ALIGN_12B_R, value);
#elif defined(GD32F3x0) 
        /* only has 1 DAC at maximum, no need for a parameter... */
        dac_trigger_disable();
        dac_wave_mode_config(DAC_WAVE_DISABLE);
        dac_output_buffer_enable();
        dac_enable();
        dac_data_set(DAC_ALIGN_12B_R, value);
#endif
        DAC_[index].isactive = true;
    } else {
        //set dac value
#if defined(GD32F30x) || (defined(GD32F1x0) && defined(GD32F170_190))
        dac_data_set(dac_periph, DAC_ALIGN_12B_R, value);
#elif defined(GD32F10x) && !defined(GD32F170_190)
        dac0_data_set(DAC_ALIGN_12B_R, value);
#elif defined(GD32F3x0) 
        dac_data_set(DAC_ALIGN_12B_R, value);
#endif
    }
#endif
}

//pwm set value
void set_pwm_value(pin_size_t ulPin, uint32_t value)
{
    uint16_t ulvalue = 1000 * value / 65535;
    PWM pwm(ulPin);
    pwm.setPeriodCycle(1000, ulvalue, FORMAT_US);
    pwm.start();
}

//get adc value
uint16_t get_adc_value(PinName pinname)
{
    uint16_t value;
    uint32_t adc_periph = pinmap_peripheral(pinname, PinMap_ADC);
    uint8_t index = get_dac_index(adc_periph);
    uint8_t channel = get_adc_channel(pinname);
    if(!ADC_[index].isactive) {
        pinmap_pinout(pinname, PinMap_ADC);
        adc_clock_enable(adc_periph);

#if defined(GD32F30x)
        rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
        adc_mode_config(ADC_MODE_FREE);
        adc_resolution_config(adc_periph, ADC_RESOLUTION_12B);
        adc_data_alignment_config(adc_periph, ADC_DATAALIGN_RIGHT);
        adc_channel_length_config(adc_periph, ADC_REGULAR_CHANNEL, 1U);
#elif defined(GD32F3x0) || defined(GD32F1x0)
        rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);
        adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE);
        #if defined(GD32F3x0) || defined(GD32F170_190)
        adc_resolution_config(ADC_RESOLUTION_12B);
        #endif
        adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
        adc_channel_length_config(ADC_REGULAR_CHANNEL, 1U);
#endif
#ifdef GD32F30x
        adc_external_trigger_source_config(adc_periph, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
#elif defined(GD32VF103) /* what?! Code for a RISC-V MCU here? */
        adc_external_trigger_source_config(adc_periph, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
#elif defined(GD32F3x0) || defined(GD32F1x0)
        adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
#endif
#if defined(GD32F30x)
        adc_external_trigger_config(adc_periph, ADC_REGULAR_CHANNEL, ENABLE);
        adc_enable(adc_periph);
        delay(1U);
        adc_calibration_enable(adc_periph);
#elif defined(GD32F3x0) || defined(GD32F1x0)
        adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);
        adc_enable();
        delay(1U);
        adc_calibration_enable();
#endif
        ADC_[index].isactive = true;
    }
#if defined(GD32F30x)
    adc_regular_channel_config(adc_periph, 0U, channel, ADC_SAMPLETIME_7POINT5);
    adc_software_trigger_enable(adc_periph, ADC_REGULAR_CHANNEL);
    while(!adc_flag_get(adc_periph, ADC_FLAG_EOC));
    adc_flag_clear(adc_periph, ADC_FLAG_EOC);
    value = adc_regular_data_read(adc_periph);
#elif defined(GD32F3x0) || defined(GD32F1x0)
    adc_regular_channel_config(0U, channel, ADC_SAMPLETIME_7POINT5);
    adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
    while(!adc_flag_get(ADC_FLAG_EOC));
    adc_flag_clear(ADC_FLAG_EOC);
    value = adc_regular_data_read();
#endif
    return value;
}

//get adc index value
uint8_t get_adc_index(uint32_t instance)
{
    uint8_t index;
    switch(instance) {
#ifdef ADC
        case ADC:
            index = 0;
            break;
#endif
#ifdef ADC0
        case ADC0:
            index = 0;
            break;
#endif
#ifdef ADC1
        case ADC1:
            index = 1;
            break;
#endif
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
        case ADC2:
            index = 2;
            break;
#endif
        default:
            index = 0;
            break;
    }
    return index;
}

// get dac index value
uint8_t get_dac_index(uint32_t instance)
{
    uint8_t index;
    switch(instance) {
#ifdef DAC /* GD32F350 series */
        case DAC:
            index = 0;
            break;
#endif
#ifdef DAC0
        case DAC0:
            index = 0;
            break;
#endif
#ifdef DAC1
        case DAC1:
            index = 1;
            break;
#endif
        default:
            index = 0;
            break;
    }
    return index;
}

//get adc channel
uint8_t get_adc_channel(PinName pinname)
{
    uint32_t function = pinmap_function(pinname, PinMap_ADC);
    uint32_t channel = GD_PIN_CHANNEL_GET(function);
    uint32_t gd_channel = 0;
    switch(channel) {
        case 0:
            gd_channel = ADC_CHANNEL_0;
            break;
        case 1:
            gd_channel = ADC_CHANNEL_1;
            break;
        case 2:
            gd_channel = ADC_CHANNEL_2;
            break;
        case 3:
            gd_channel = ADC_CHANNEL_3;
            break;
        case 4:
            gd_channel = ADC_CHANNEL_4;
            break;
        case 5:
            gd_channel = ADC_CHANNEL_5;
            break;
        case 6:
            gd_channel = ADC_CHANNEL_6;
            break;
        case 7:
            gd_channel = ADC_CHANNEL_7;
            break;
        case 8:
            gd_channel = ADC_CHANNEL_8;
            break;
        case 9:
            gd_channel = ADC_CHANNEL_9;
            break;
        case 10:
            gd_channel = ADC_CHANNEL_10;
            break;
        case 11:
            gd_channel = ADC_CHANNEL_11;
            break;
        case 12:
            gd_channel = ADC_CHANNEL_12;
            break;
        case 13:
            gd_channel = ADC_CHANNEL_13;
            break;
        case 14:
            gd_channel = ADC_CHANNEL_14;
            break;
        case 15:
            gd_channel = ADC_CHANNEL_15;
            break;
        case 16:
            gd_channel = ADC_CHANNEL_16;
            break;
        case 17:
            gd_channel = ADC_CHANNEL_17;
            break;
        default:
            gd_channel = 0xFF;
            break;
    }
    return gd_channel;
}

//adc clock enable
void adc_clock_enable(uint32_t instance)
{
    rcu_periph_enum temp;
    switch(instance) {
#if defined(GD32F30x)
        case ADC0:
            temp = RCU_ADC0;
            break;
        case ADC1:
            temp = RCU_ADC1;
            break;
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))
        case ADC2:
            temp = RCU_ADC2;
            break;
#endif
#elif defined(GD32F3x0) || defined(GD32F1x0)
        case ADC:
            temp = RCU_ADC;
            break;
#endif
        default:
            break;
    }
    rcu_periph_clock_enable(temp);
}
