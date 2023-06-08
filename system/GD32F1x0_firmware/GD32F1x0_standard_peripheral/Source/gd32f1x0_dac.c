/*!
    \file    gd32f1x0_dac.c
    \brief   DAC driver

    \version 2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    \version 2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    \version 2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2019-11-20, V3.2.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2020-09-21, V3.3.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2022-08-15, V3.4.0, firmware update for GD32F1x0(x=3,5)
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

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


#include "gd32f1x0_dac.h"

/*!
    \brief      deinit DAC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_deinit(void)
{
    rcu_periph_reset_enable(RCU_DACRST);
    rcu_periph_reset_disable(RCU_DACRST);
}

/*!
    \brief      enable DAC0
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_enable(void)
{
    DAC_CTL |= DAC_CTL_DEN0;
}

/*!
    \brief      disable DAC0
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_disable(void)
{
    DAC_CTL &= ~DAC_CTL_DEN0;
}

/*!
    \brief      enable DAC0 DMA
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_dma_enable(void)
{
    DAC_CTL |= DAC_CTL_DDMAEN0;
}

/*!
    \brief      disable DAC0 DMA
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_dma_disable(void)
{
    DAC_CTL &= ~DAC_CTL_DDMAEN0;
}

/*!
    \brief      enable DAC0 output buffer
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_output_buffer_enable(void)
{
    DAC_CTL &= ~DAC_CTL_DBOFF0;
}

/*!
    \brief      disable DAC0 output buffer
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_output_buffer_disable(void)
{
    DAC_CTL |= DAC_CTL_DBOFF0;
}

/*!
    \brief      enable DAC0 trigger
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_trigger_enable(void)
{
    DAC_CTL |= DAC_CTL_DTEN0;
}

/*!
    \brief      disable DAC0 trigger
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_trigger_disable(void)
{
    DAC_CTL &= ~DAC_CTL_DTEN0;
}

/*!
    \brief      enable DAC0 software trigger
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_software_trigger_enable(void)
{
    DAC_SWT |= DAC_SWT_SWTR0;
}

/*!
    \brief      disable DAC0 software trigger
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_software_trigger_disable(void)
{
    DAC_SWT &= ~DAC_SWT_SWTR0;
}

/*!
    \brief      enable DAC0 interrupt(DAC0 DMA underrun interrupt)
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_interrupt_enable(void)
{
    DAC_CTL |= DAC_CTL_DDUDRIE0;
}

/*!
    \brief      disable DAC0 interrupt(DAC0 DMA underrun interrupt)
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_interrupt_disable(void)
{
    DAC_CTL &= ~DAC_CTL_DDUDRIE0;
}

/*!
    \brief      set DAC0 tgigger source
    \param[in]  triggersource: external triggers of DAC
      \arg        DAC_TRIGGER_T1_TRGO: trigger source is TIMER1 TRGO
      \arg        DAC_TRIGGER_T2_TRGO: trigger source is TIMER2 TRGO
      \arg        DAC_TRIGGER_T5_TRGO: trigger source is TIMER5 TRGO
      \arg        DAC_TRIGGER_T14_TRGO: trigger source is TIMER14 TRGO
      \arg        DAC_TRIGGER_EXTI_IT9: trigger source is EXTI interrupt line 9
      \arg        DAC_TRIGGER_SOFTWARE: trigger source is software
    \param[out] none
    \retval     none
*/
void dac0_trigger_source_config(uint32_t triggersource)
{
    DAC_CTL &= ~DAC_CTL_DTSEL0;
    DAC_CTL |= triggersource;
}

/*!
    \brief      get the last data output value of DAC0
    \param[in]  none
    \param[out] none
    \retval     DAC output data
*/
uint16_t dac0_output_value_get(void)
{
    uint16_t data = 0U;
    data = (uint16_t)DAC0_DO;
    return data;
}

/*!
    \brief      get the specified DAC0 flag(DAC0 DMA underrun flag)
    \param[in]  none
    \param[out] none
    \retval     the state of dac bit(SET or RESET)
*/
FlagStatus dac0_flag_get(void)
{
    /* check the DMA underrun flag */
    if((uint8_t)RESET != (DAC_STAT & DAC_STAT_DDUDR0)){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    \brief      clear the specified DAC0 flag(DAC0 DMA underrun flag)
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_flag_clear(void)
{
    DAC_STAT &= ~DAC_STAT_DDUDR0;
}

/*!
    \brief      get the specified DAC0 interrupt flag(DAC0 DMA underrun interrupt flag)
    \param[in]  none
    \param[out] none
    \retval     the state of DAC interrupt flag(SET or RESET)
*/
FlagStatus dac0_interrupt_flag_get(void)
{
    uint32_t ddudr_flag = 0U, ddudrie_flag = 0U;
    ddudr_flag = (DAC_STAT & DAC_STAT_DDUDR0);
    ddudrie_flag = DAC_CTL & DAC_CTL_DDUDRIE0;
    /* check the DMA underrun flag and DAC DMA underrun interrupt enable flag */
    if((RESET != ddudr_flag) && (RESET != ddudrie_flag)){
         return SET;
    }else{
        return RESET;
    }
}

/*!
    \brief      clear the specified DAC0 interrupt flag(DAC0 DMA underrun interrupt flag)
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac0_interrupt_flag_clear(void)
{
    DAC_CTL &= ~DAC_CTL_DDUDRIE0;
}

/*!
    \brief      set DAC0 data holding register value
    \param[in]  dac_align
      \arg        DAC_ALIGN_8B_R: data right 8b alignment
      \arg        DAC_ALIGN_12B_R: data right 12b alignment
      \arg        DAC_ALIGN_12B_L: data left 12b alignment
    \param[in]  data: data to be loaded
    \param[out] none
    \retval     none
*/
void dac0_data_set(uint32_t dac_align, uint16_t data)
{
    switch(dac_align){
        /* data right 12b alignment */
        case DAC_ALIGN_12B_R:
            DAC0_R12DH = data;
            break;
        /* data left 12b alignment */
        case DAC_ALIGN_12B_L:
            DAC0_L12DH = data;
            break;
        /* data right 8b alignment */
        case DAC_ALIGN_8B_R:
            DAC0_R8DH = data;
            break;
        default:
            break;
    }
}
