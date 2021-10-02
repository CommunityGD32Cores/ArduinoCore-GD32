/*!
    \file    gd32f1x0_slcd.c
    \brief   SLCD driver

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

#ifdef GD32F170_190
#include "gd32f1x0_slcd.h"

/*!
    \brief      SLCD reset 
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_deinit(void)
{
    rcu_periph_reset_enable(RCU_SLCDRST);
    rcu_periph_reset_disable(RCU_SLCDRST);
}

/*!
    \brief      enable SLCD interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_enable(void)
{
    SLCD_CTL |= SLCD_CTL_SLCDON;
}

/*!
    \brief      disable SLCD interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_disable(void)
{
    SLCD_CTL &= ~(SLCD_CTL_SLCDON);
}

/*!
    \brief      SLCD bias voltage select
    \param[in]  bias_voltage: the SLCD voltage bias
                only one parameter can be selected which is shown as below:
      \arg        SLCD_BIAS_1_4: 1/4 voltage bias
      \arg        SLCD_BIAS_1_2: 1/2 voltage bias
      \arg        SLCD_BIAS_1_3: 1/3 voltage bias
    \param[out] none
    \retval     none
*/
void slcd_bias_voltage_select(uint32_t bias_voltage)
{
    SLCD_CTL &= (uint32_t)~(SLCD_CTL_BIAS);
    SLCD_CTL |= (uint32_t)bias_voltage;
}

/*!
    \brief      SLCD duty cycle select
    \param[in]  duty: duty select
                only one parameter can be selected which is shown as below:
      \arg        SLCD_DUTY_STATIC: static duty
      \arg        SLCD_DUTY_1_2: 1/2 duty
      \arg        SLCD_DUTY_1_3: 1/3 duty
      \arg        SLCD_DUTY_1_4: 1/4 duty
      \arg        SLCD_DUTY_1_6: 1/6 duty
      \arg        SLCD_DUTY_1_8: 1/8 duty
    \param[out] none
    \retval     none
*/
void slcd_duty_select(uint32_t duty)
{
    SLCD_CTL &= (uint32_t)~(SLCD_CTL_DUTY);
    SLCD_CTL |= (uint32_t)duty;
}

/*!
    \brief      config the prescaler and the divider of SLCD clock
                fSLCD = finclk/(pow(2, PRE)* DIV)
    \param[in]  prescaler: the prescaler factor
                only one parameter can be selected which is shown as below:
      \arg        SLCD_PRESCALER_1: PRE = 0
      \arg        SLCD_PRESCALER_2: PRE = 1
      \arg        SLCD_PRESCALER_4: PRE = 2
      \arg        SLCD_PRESCALER_8: PRE = 3
      \arg        SLCD_PRESCALER_16: PRE = 4
      \arg        SLCD_PRESCALER_32: PRE = 5
      \arg        SLCD_PRESCALER_64: PRE = 6
      \arg        SLCD_PRESCALER_128: PRE = 7
      \arg        SLCD_PRESCALER_256: PRE = 8
      \arg        SLCD_PRESCALER_512: PRE = 9
      \arg        SLCD_PRESCALER_1024: PRE = 10
      \arg        SLCD_PRESCALER_2048: PRE = 11
      \arg        SLCD_PRESCALER_4096: PRE = 12
      \arg        SLCD_PRESCALER_8192: PRE = 13
      \arg        SLCD_PRESCALER_16384: PRE = 14
      \arg        SLCD_PRESCALER_32768: PRE = 15
    \param[in]  divider: the divider factor
                only one parameter can be selected which is shown as below:
      \arg        SLCD_DIVIDER_x: x= 16..31, DIV = 16..31
    \param[out] none
    \retval     none
*/
void slcd_clock_config(uint32_t prescaler,uint32_t divider)
{
    uint32_t cfg;
    
    /* config the prescaler and the divider */
    cfg = SLCD_CFG;
    cfg &= (uint32_t)~(SLCD_CFG_PSC | SLCD_CFG_DIV);
    cfg |= (uint32_t)(prescaler | divider);
    SLCD_CFG = cfg;
}

/*!
    \brief      SLCD blink mode config
    \param[in]  mode: blink mode
                only one parameter can be selected which is shown as below:
      \arg        SLCD_BLINKMODE_OFF: blink disabled
      \arg        SLCD_BLINKMODE_SEG0_COM0: blink enabled on SEG[0], COM[0]
      \arg        SLCD_BLINKMODE_SEG0_ALLCOM: blink enabled on SEG[0], all COM
      \arg        SLCD_BLINKMODE_ALLSEG_ALLCOM: blink enabled on all SEG and all COM
    \param[in]  blink_divider: the divider factor
                only one parameter can be selected which is shown as below:
      \arg        SLCD_BLINK_FREQUENCY_DIV8: blink frequency = fSLCD/8
      \arg        SLCD_BLINK_FREQUENCY_DIV16: blink frequency = fSLCD/16
      \arg        SLCD_BLINK_FREQUENCY_DIV32: blink frequency = fSLCD/32
      \arg        SLCD_BLINK_FREQUENCY_DIV64: blink frequency = fSLCD/64
      \arg        SLCD_BLINK_FREQUENCY_DIV128: blink frequency = fSLCD/128
      \arg        SLCD_BLINK_FREQUENCY_DIV256: blink frequency = fSLCD/256
      \arg        SLCD_BLINK_FREQUENCY_DIV512: blink frequency = fSLCD/512
      \arg        SLCD_BLINK_FREQUENCY_DIV1024: blink frequency = fSLCD/1024
    \param[out] none
    \retval     none
*/
void slcd_blink_mode_config(uint32_t mode,uint32_t blink_divider)
{
    SLCD_CFG &= (uint32_t)~(SLCD_CFG_BLKMOD | SLCD_CFG_BLKDIV);
    SLCD_CFG |= (uint32_t)(mode | blink_divider);
}

/*!
    \brief      SLCD contrast ratio config
    \param[in]  contrast_ratio: specify the VSLCD voltage, when chosing the internal voltage source
                only one parameter can be selected which is shown as below:
      \arg        SLCD_CONTRAST_LEVEL_0: maximum SLCD Voltage = 2.60V
      \arg        SLCD_CONTRAST_LEVEL_1: maximum SLCD Voltage = 2.73V
      \arg        SLCD_CONTRAST_LEVEL_2: maximum SLCD Voltage = 2.86V
      \arg        SLCD_CONTRAST_LEVEL_3: maximum SLCD Voltage = 2.99V
      \arg        SLCD_CONTRAST_LEVEL_4: maximum SLCD Voltage = 3.12V
      \arg        SLCD_CONTRAST_LEVEL_5: maximum SLCD Voltage = 3.25V
      \arg        SLCD_CONTRAST_LEVEL_6: maximum SLCD Voltage = 3.38V
      \arg        SLCD_CONTRAST_LEVEL_7: maximum SLCD Voltage = 3.51V
    \param[out] none
    \retval     none
*/
void slcd_contrast_ratio_config(uint32_t contrast_ratio)
{
    uint32_t cfg;
    
    /* config the contrast ratio */
    cfg = SLCD_CFG;
    cfg &= (uint32_t)~(SLCD_CFG_CONR);
    cfg |= (uint32_t)contrast_ratio;
    SLCD_CFG = cfg;
}

/*!
    \brief      SLCD dead time duration config
    \param[in]  dead_time: configure the length of the dead time between frames
                only one parameter can be selected which is shown as below:
      \arg        SLCD_DEADTIME_PERIOD_0: no dead time
      \arg        SLCD_DEADTIME_PERIOD_1: 1 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_2: 2 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_3: 3 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_4: 4 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_5: 5 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_6: 6 phase inserted between couple of frame
      \arg        SLCD_DEADTIME_PERIOD_7: 7 phase inserted between couple of frame
    \param[out] none
    \retval     none
*/
void slcd_dead_time_config(uint32_t dead_time)
{
    uint32_t cfg;
    
    /* config dead time duration */
    cfg = SLCD_CFG;
    cfg &= (uint32_t)~(SLCD_CFG_DTD);
    cfg |= (uint32_t)dead_time;
    SLCD_CFG = cfg;
}

/*!
    \brief      SLCD pulse on duration config
    \param[in]  duration: define the pulse duration in terms of PSC pulses
                only one parameter can be selected which is shown as below:
      \arg        SLCD_PULSEON_DURATION_0: pulse on duration = 0
      \arg        SLCD_PULSEON_DURATION_1: pulse on duration = 1*1/fPRE
      \arg        SLCD_PULSEON_DURATION_2: pulse on duration = 2*1/fPRE
      \arg        SLCD_PULSEON_DURATION_3: pulse on duration = 3*1/fPRE
      \arg        SLCD_PULSEON_DURATION_4: pulse on duration = 4*1/fPRE
      \arg        SLCD_PULSEON_DURATION_5: pulse on duration = 5*1/fPRE
      \arg        SLCD_PULSEON_DURATION_6: pulse on duration = 6*1/fPRE
      \arg        SLCD_PULSEON_DURATION_7: pulse on duration = 7*1/fPRE
    \param[out] none
    \retval     none
*/
void slcd_pulse_on_duration_config(uint32_t duration)
{
    uint32_t cfg;
    
    /* config pulse on duration */
    cfg = SLCD_CFG;
    cfg &= (uint32_t)~(SLCD_CFG_PULSE);
    cfg |= (uint32_t)duration;
    SLCD_CFG = cfg; 
}

/*!
    \brief      SLCD common/segment pad select
    \param[in]  NewValue: ENABLE or DISABLE
                only one parameter can be selected which is shown as below:
      \arg        ENABLE: LCD_COM[7:4] pad select LCD_SEG[31:28]
      \arg        DISABLE: LCD_COM[7:4] pad select LCD_COM[7:4]
    \param[out] none
    \retval     none
*/
void slcd_com_seg_remap(ControlStatus newvalue)
{
    if( ENABLE == newvalue ){
        SLCD_CTL |= SLCD_CTL_COMS;
    }else{
        SLCD_CTL &= ~(SLCD_CTL_COMS);
    }
}

/*!
    \brief      SLCD voltage source select
    \param[in]  voltage_source: the SLCD voltage source
                only one parameter can be selected which is shown as below:
      \arg        SLCD_VOLTAGE_INTERNAL: internal source
      \arg        SLCD_VOLTAGE_EXTERNAL: external source (VSLCD pin)
    \param[out] none
    \retval     none
*/
void slcd_voltage_source_select(uint8_t voltage_source)
{
    if( SLCD_VOLTAGE_EXTERNAL == voltage_source ){
        SLCD_CTL |= SLCD_CTL_VSRC;
    }else{
        SLCD_CTL &= ~(SLCD_CTL_VSRC);
    }
}

/*!
    \brief      enable or disable permanent high drive
    \param[in]  newvalue: ENABLE or DISABLE
                only one parameter can be selected which is shown as below:
    \arg          ENBALE: enable permanent high drive
    \arg          DISBALE: disable permanent high drive
    \param[out] none
    \retval     none
*/
void slcd_high_drive_config(ControlStatus newvalue)
{
    if( ENABLE == newvalue ){
        SLCD_CFG |= SLCD_CFG_HDEN;
    }else{
        SLCD_CFG &= ~(SLCD_CFG_HDEN);
    }
}

/*!
    \brief      write slcd display data registers 
    \param[in]  register_number: refer to slcd_data_register_enum
                only one parameter can be selected which is shown as below:
      \arg        SLCD_DATA_REG0: SLCD_DATA register 0
      \arg        SLCD_DATA_REG1: SLCD_DATA Register 1
      \arg        SLCD_DATA_REG2: SLCD_DATA register 2
      \arg        SLCD_DATA_REG3: SLCD_DATA Register 3
      \arg        SLCD_DATA_REG4: SLCD_DATA register 4
      \arg        SLCD_DATA_REG5: SLCD_DATA Register 5
      \arg        SLCD_DATA_REG6: SLCD_DATA register 6
      \arg        SLCD_DATA_REG7: SLCD_DATA Register 7
    \param[in]  data: the data write to the register
    \param[out] none
    \retval     none
*/
void slcd_data_register_write(slcd_data_register_enum register_number, uint32_t data)
{
    /* wtite data word to DATA register */
    SLCD_DATA0_7((uint32_t)register_number) = data; 
}

/*!
    \brief      SLCD data update request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_data_update_request(void)
{
    SLCD_STAT |= SLCD_STAT_UPRF;
}


/*!
    \brief      the SLCD interrupt config
    \param[in]  slcd_interrupt: interrupt source
                only one parameter can be selected which is shown as below:
      \arg        SLCD_INT_SOF: start of frame interrupt
      \arg        SLCD_INT_UPD: SLCD update done interrupt
    \param[in]  newvalue: ENABLE or DISABLE
                only one parameter can be selected which is shown as below:
    \arg          ENBALE: enable interrupt
    \arg          DISBALE: disable interrupt
    \param[out] none
    \retval     none
*/
void slcd_interrupt_config(uint8_t slcd_interrupt,ControlStatus newvalue)
{
    /* ENABLE or DISABLE the interrupt */
    if(newvalue){
        /* select the interrupt source */
        switch(slcd_interrupt){
        case SLCD_INT_SOF:
            SLCD_CFG |= (uint32_t) SLCD_CFG_SOFIE;
            break;
        case SLCD_INT_UPD:
            SLCD_CFG |= (uint32_t) SLCD_CFG_UPDIE;
            break;
        default:
            break;
        }
    }else{
        switch(slcd_interrupt){
        /* select the interrupt source */
        case SLCD_INT_SOF:
            SLCD_CFG &= (uint32_t)~(SLCD_CFG_SOFIE);
            break;
        case SLCD_INT_UPD:
            SLCD_CFG &= (uint32_t)~(SLCD_CFG_UPDIE);
            break;
        default:
            break;
        }
    }
}

/*!
    \brief      get the SLCD status flag
    \param[in]  slcd_flag: the SLCD status flag
                only one parameter can be selected which is shown as below:
      \arg        SLCD_FLAG_ON: SLCD controller on flag
      \arg        SLCD_FLAG_SOF: start of frame flag
      \arg        SLCD_FLAG_UPR: SLCD data update request flag
      \arg        SLCD_FLAG_UPD: update LCDS data done flag
      \arg        SLCD_FLAG_VRDY: SLCD voltage ready flag
      \arg        SLCD_FLAG_SYN: SLCD CFG register synchronization flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus slcd_flag_get(uint8_t slcd_flag)
{
    if(SLCD_STAT & slcd_flag){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    \brief      clear the SLCD flag
    \param[in]  slcd_flag: the SLCD status flag bits
                only one parameter can be selected which is shown as below:
      \arg        SLCD_FLAG_SOF: start of frame flag
      \arg        SLCD_FLAG_UPD: update LCDS data done flag
    \param[out] none
    \retval     none
*/
void slcd_flag_clear(uint8_t slcd_flag)
{
    /* check the interrupt bits */
    switch(slcd_flag){
    case SLCD_FLAG_SOF:
        SLCD_STATC |= SLCD_STATC_SOFC;
        break;
    case SLCD_FLAG_UPD:
        SLCD_STATC |= SLCD_STATC_UPDC;
        break;
    default:
        break;
    }
}

/*!
    \brief      get the SLCD interrupt flag
    \param[in]  slcd_interrupt: the SLCD interrupt flag
                only one parameter can be selected which is shown as below:
      \arg        SLCD_INT_FLAG_SOF: start of frame interrupt
      \arg        SLCD_INT_FLAG_UPD: SLCD update done interrupt
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus slcd_interrupt_flag_get(uint8_t slcd_interrupt)
{
    FlagStatus interrupt_flag = RESET;
    uint32_t state;
    /* check the interrupt bits */
    switch(slcd_interrupt){
    case SLCD_INT_FLAG_SOF:
        state = SLCD_STAT & SLCD_FLAG_SOF;
        if((SLCD_CFG & SLCD_CFG_SOFIE) && state){
            interrupt_flag = SET;
        }
        break;
    case SLCD_INT_FLAG_UPD:
        state = SLCD_STAT & SLCD_FLAG_UPD;
        if((SLCD_CFG & SLCD_CFG_UPDIE) && state){
            interrupt_flag = SET;
        }
        break;
    default:
        break;
    }
    return interrupt_flag;
}

/*!
    \brief      clear the SLCD interrupt flag
    \param[in]  slcd_interrupt: the SLCD interrupt bits
                only one parameter can be selected which is shown as below:
      \arg        SLCD_INT_FLAG_SOF: start of frame interrupt
      \arg        SLCD_INT_FLAG_UPD: SLCD update done interrupt
    \param[out] none
    \retval     none
*/
void slcd_interrupt_flag_clear(uint8_t slcd_interrupt)
{
    /* check the interrupt bits */
    switch(slcd_interrupt){
    case SLCD_INT_FLAG_SOF:
        SLCD_STATC |= SLCD_STATC_SOFC;
        break;
    case SLCD_INT_FLAG_UPD:
        SLCD_STATC |= SLCD_STATC_UPDC;
        break;
    default:
        break;
    }
}

#endif /* GD32F170_190 */
