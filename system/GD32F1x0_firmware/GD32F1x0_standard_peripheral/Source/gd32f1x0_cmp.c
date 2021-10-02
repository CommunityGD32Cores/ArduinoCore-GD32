/*!
    \file    gd32f1x0_cmp.c
    \brief   CMP driver

    \version 2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    \version 2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    \version 2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2019-11-20, V3.2.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2020-09-21, V3.3.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

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
*/

#include "gd32f1x0_cmp.h"

/*!
    \brief      deinitialize comparator 
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_deinit(void)
{
    CMP_CS = ((uint32_t)0x00000000U);
}

/*!
    \brief      initialize comparator mode 
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[in]  operating_mode
      \arg        CMP_HIGHSPEED: high speed mode
      \arg        CMP_MIDDLESPEED: medium speed mode
      \arg        CMP_LOWSPEED: low speed mode
      \arg        CMP_VERYLOWSPEED: very-low speed mode
    \param[in]  inverting_input 
      \arg        CMP_1_4VREFINT: VREFINT *1/4 input
      \arg        CMP_1_2VREFINT: VREFINT *1/2 input
      \arg        CMP_3_4VREFINT: VREFINT *3/4 input
      \arg        CMP_VREFINT: VREFINT input
      \arg        CMP_DAC0: PA4 (DAC0) input
      \arg        CMP_PA5 or CMP_DAC1: PA5 input (for GD32F130_150 series) or PA5 (DAC1) input (for GD32F170_190 series)
      \arg        CMP_PA_0_2: PA0 input (CMP0) or PA2 input (CMP1)
    \param[in]  hysteresis
      \arg        CMP_HYSTERESIS_NO: output no hysteresis
      \arg        CMP_HYSTERESIS_LOW: output low hysteresis
      \arg        CMP_HYSTERESIS_MIDDLE: output middle hysteresis
      \arg        CMP_HYSTERESIS_HIGH: output high hysteresis
    \param[out] none
    \retval     none
*/
void cmp_mode_init(uint32_t cmp_periph, operating_mode_enum operating_mode, inverting_input_enum inverting_input, cmp_hysteresis_enum output_hysteresis)
{
    uint32_t CMPx_CS = 0;
    if(CMP0 == cmp_periph){
        /* initialize comparator 0 mode */
        CMPx_CS = CMP_CS;
        CMPx_CS &= ~(uint32_t)(CMP_CS_CMP0M | CMP_CS_CMP0MSEL | CMP_CS_CMP0HST ); 
        CMPx_CS |= CS_CMP0M(operating_mode) | CS_CMP0MSEL(inverting_input) | CS_CMP0HST(output_hysteresis);
        CMP_CS = CMPx_CS;
    }else{
        /* initialize comparator 1 mode */
        CMPx_CS = CMP_CS;
        CMPx_CS &= ~(uint32_t)(CMP_CS_CMP1M | CMP_CS_CMP1MSEL | CMP_CS_CMP1HST );
        CMPx_CS |= CS_CMP1M(operating_mode) | CS_CMP1MSEL(inverting_input) | CS_CMP1HST(output_hysteresis);
        CMP_CS = CMPx_CS;
    }
}

/*!
    \brief      initialize comparator output
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[in]  output_slection 
      \arg        CMP_OUTPUT_NONE: output no selection
      \arg        CMP_OUTPUT_TIMER0BKIN: TIMER 0 break input
      \arg        CMP_OUTPUT_TIMER0IC0: TIMER 0 channel0 input capture 
      \arg        CMP_OUTPUT_TIMER0OCPRECLR: TIMER 0 OCPRE_CLR input
      \arg        CMP_OUTPUT_TIMER1IC3: TIMER 1 channel3 input capture
      \arg        CMP_OUTPUT_TIMER1OCPRECLR: TIMER 1 OCPRE_CLR input
      \arg        CMP_OUTPUT_TIMER2IC0: TIMER 2 channel0 input capture
      \arg        CMP_OUTPUT_TIMER2OCPRECLR: TIMER 2 OCPRE_CLR input
    \param[in]  output_polarity 
      \arg        CMP_OUTPUT_POLARITY_INVERTED: output is inverted
      \arg        CMP_OUTPUT_POLARITY_NOINVERTED: output is not inverted
    \param[out] none
    \retval     none
*/
void cmp_output_init(uint32_t cmp_periph, cmp_output_enum output_slection, uint32_t output_polarity)
{
    uint32_t CMPx_CS = 0;
    if(CMP0 == cmp_periph){
        /* initialize comparator 0 output */
        CMPx_CS = CMP_CS;
        CMPx_CS &= ~(uint32_t)CMP_CS_CMP0OSEL;
        CMPx_CS |= CS_CMP0OSEL(output_slection);
        /* output polarity */
        if(CMP_OUTPUT_POLARITY_INVERTED == output_polarity){
            CMPx_CS |= CMP_CS_CMP0PL;
        }else{
            CMPx_CS &= ~CMP_CS_CMP0PL;
        }
        CMP_CS = CMPx_CS;
    }else if(CMP1 == cmp_periph){
        /* initialize comparator 1 output */
        CMPx_CS = CMP_CS;
        CMPx_CS &= ~(uint32_t)CMP_CS_CMP1OSEL;
        CMPx_CS |= CS_CMP1OSEL(output_slection);
        /* output polarity */
        if(CMP_OUTPUT_POLARITY_INVERTED == output_polarity){
            CMPx_CS |= CMP_CS_CMP1PL;
        }else{
            CMPx_CS &= ~CMP_CS_CMP1PL;
        }
        CMP_CS = CMPx_CS;
    }
}

/*!
    \brief      enable comparator 
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_enable(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        CMP_CS |= CMP_CS_CMP0EN;
    }else{
        CMP_CS |= CMP_CS_CMP1EN;
    }
}

/*!
    \brief      disable comparator 
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_disable(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        CMP_CS &= ~CMP_CS_CMP0EN;
    }else{
        CMP_CS &= ~CMP_CS_CMP1EN;
    }
}

/*!
    \brief      enable comparator switch
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_switch_enable(void)
{
    CMP_CS |= CMP_CS_CMP0SW;
}

/*!
    \brief      disable comparator switch
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_switch_disable(void)
{
    CMP_CS &= ~CMP_CS_CMP0SW;
}

/*!
    \brief      enable the window mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_window_enable(void)
{
    CMP_CS |= CMP_CS_WNDEN;
}

/*!
    \brief      disable the window mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_window_disable(void)
{
    CMP_CS &= ~CMP_CS_WNDEN;
}

/*!
    \brief      lock the comparator
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     none
*/
void cmp_lock_enable(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        /* lock CMP0 */
        CMP_CS |= CMP_CS_CMP0LK;
    }else{
        /* lock CMP1 */
        CMP_CS |= CMP_CS_CMP1LK;
    }
}

/*!
    \brief      get output level
    \param[in]  cmp_periph
      \arg        CMP0: comparator 0
      \arg        CMP1: comparator 1
    \param[out] none
    \retval     the output level
*/
uint32_t cmp_output_level_get(uint32_t cmp_periph)
{
    if(CMP0 == cmp_periph){
        /* get output level of CMP0 */
        if(CMP_CS & CMP_CS_CMP0O){
            return CMP_OUTPUTLEVEL_HIGH;
        }else{
            return CMP_OUTPUTLEVEL_LOW;
        }
    }else{
        /* get output level of CMP1 */
        if(CMP_CS & CMP_CS_CMP1O){
            return CMP_OUTPUTLEVEL_HIGH;
        }else{
            return CMP_OUTPUTLEVEL_LOW;
        }
    }
}
