/*!
    \file    gd32f1x0_fmc.c
    \brief   FMC driver

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

#include "gd32f1x0_fmc.h"

/* return the FMC state */
static fmc_state_enum fmc_state_get(void);
/* check FMC ready or not */
static fmc_state_enum fmc_ready_wait(uint32_t timeout);

/* FMC main memory programming functions */

/*!
    \brief      unlock the main FMC operation
                it is better to used in pairs with fmc_lock
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_unlock(void)
{
    if((RESET != (FMC_CTL & FMC_CTL_LK))){
        /* write the FMC key */
        FMC_KEY = UNLOCK_KEY0;
        FMC_KEY = UNLOCK_KEY1;
    }
}

/*!
    \brief      lock the main FMC operation
                it is better to used in pairs with fmc_unlock after an operation
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_lock(void)
{
    /* set the LK bit*/
    FMC_CTL |= FMC_CTL_LK;
}

/*!
    \brief      set the wait state counter value
    \param[in]  wscnt: wait state counter value
                only one parameter can be selected which is shown as below:
      \arg        WS_WSCNT_0: 0 wait state added
      \arg        WS_WSCNT_1: 1 wait state added
      \arg        WS_WSCNT_2: 2 wait state added
    \param[out] none
    \retval     none
*/
void fmc_wscnt_set(uint8_t wscnt)
{
    uint32_t reg;

    reg = FMC_WS;
    /* set the wait state counter value */
    reg &= ~FMC_WS_WSCNT;
    FMC_WS = (reg | wscnt);
}

/*!
    \brief      fmc wait state enable
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_wait_state_enable(void)
{
    /* set the WSEN bit in register FMC_WSEN */
    FMC_WSEN |= FMC_WSEN_WSEN;
}

/*!
    \brief      fmc wait state disable
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_wait_state_disable(void)
{
    /* reset the WSEN bit in register FMC_WSEN */
    FMC_WSEN &= ~FMC_WSEN_WSEN;
}

/*!
    \brief      FMC erase page
    \param[in]  page_address: target page address
    \param[out] none
    \retval     state of FMC
      \arg        FMC_READY: the operation has been completed
      \arg        FMC_WPERR: erase/program protection error
      \arg        FMC_TOERR: timeout error
*/
fmc_state_enum fmc_page_erase(uint32_t page_address)
{
    fmc_state_enum fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == fmc_state){
        /* start page erase */
        FMC_CTL |= FMC_CTL_PER;
        FMC_ADDR = page_address;
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

        /* reset the PER bit */
        FMC_CTL &= ~FMC_CTL_PER;
    }

    /* return the FMC state  */
    return fmc_state;
}

/*!
    \brief      FMC erase whole chip
    \param[in]  none
    \param[out] none
    \retval     state of FMC
      \arg        FMC_READY: the operation has been completed
      \arg        FMC_WPERR: erase/program protection error
      \arg        FMC_TOERR: timeout error
*/
fmc_state_enum fmc_mass_erase(void)
{
    fmc_state_enum fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == fmc_state){ 
        /* start chip erase */
        FMC_CTL |= FMC_CTL_MER; 
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

        /* reset the MER bit */
        FMC_CTL &= ~FMC_CTL_MER;
    }

    /* return the fmc state  */
    return fmc_state;
}

/*!
    \brief      FMC program a word at the corresponding address
    \param[in]  address: address to program
    \param[in]  data: word to program
    \param[out] none
    \retval     state of FMC
      \arg        FMC_READY: the operation has been completed
      \arg        FMC_PGERR: program error
      \arg        FMC_WPERR: erase/program protection error
      \arg        FMC_TOERR: timeout error
*/
fmc_state_enum fmc_word_program(uint32_t address, uint32_t data)
{
    fmc_state_enum fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == fmc_state){ 
        /* set the PG bit to start program */
        FMC_CTL |= FMC_CTL_PG;
        REG32(address) = data;

        /* wait for the FMC ready */
        fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

        /* reset the PG bit */
        FMC_CTL &= ~FMC_CTL_PG;
    }

    /* return the FMC state */
    return fmc_state;
}

/*!
    \brief      FMC program a half word at the corresponding address
    \param[in]  address: address to program
    \param[in]  data: word to program
    \param[out] none
    \retval     state of FMC
      \arg        FMC_READY: the operation has been completed
      \arg        FMC_PGERR: program error
      \arg        FMC_WPERR: erase/program protection error
      \arg        FMC_TOERR: timeout error
*/
fmc_state_enum fmc_halfword_program(uint32_t address, uint16_t data)
{
    fmc_state_enum fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == fmc_state){
        /* set the PG bit to start program */
        FMC_CTL |= FMC_CTL_PG;
        REG16(address) = data;

        /* wait for the FMC ready */
        fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

        /* reset the PG bit */
        FMC_CTL &= ~FMC_CTL_PG;
    }

    /* return the FMC state */
    return fmc_state;
}

#ifdef GD32F170_190
/*!
    \brief      FMC program a word at the corresponding address without erasing
    \param[in]  address: address to program
    \param[in]  data: word to program
    \param[out] none
    \retval     state of FMC
      \arg        FMC_READY: the operation has been completed
      \arg        FMC_WPERR: erase/program protection error
      \arg        FMC_TOERR: timeout error
*/
fmc_state_enum fmc_word_reprogram(uint32_t address, uint32_t data)
{
    fmc_state_enum fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
    FMC_WSEN |= FMC_WSEN_BPEN;

    if(FMC_READY == fmc_state){
        /* set the PG bit to start program */
        FMC_CTL |= FMC_CTL_PG;
        REG32(address) = data;

        /* wait for the FMC ready */
        fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

        /* reset the PG bit */
        FMC_CTL &= ~FMC_CTL_PG;
    }

    /* return the FMC state */
    return fmc_state;
}
#endif /* GD32F170_190 */

/* FMC option bytes programming functions */

/*!
    \brief      unlock the option byte operation
                it is better to used in pairs with ob_lock
    \param[in]  none
    \param[out] none
    \retval     none
*/
void ob_unlock(void)
{
    if(RESET == (FMC_CTL & FMC_CTL_OBWEN)){
        /* write the FMC key */
        FMC_OBKEY = UNLOCK_KEY0;
        FMC_OBKEY = UNLOCK_KEY1;
    }
}

/*!
    \brief      lock the option byte operation
                it is better to used in pairs with ob_unlock after an operation
    \param[in]  none
    \param[out] none
    \retval     none
*/
void ob_lock(void)
{
    /* reset the OBWE bit */
    FMC_CTL &= ~FMC_CTL_OBWEN;
}

/*!
    \brief      reload the option byte and generate a system reset
    \param[in]  none
    \param[out] none
    \retval     none
*/
void ob_reset(void)
{
    /* set the OBRLD bit */
    FMC_CTL |= FMC_CTL_OBRLD;
}

/*!
    \brief      erase the option byte
                programmer must ensure FMC & option byte are both unlocked before calling this function
    \param[in]  none
    \param[out] none
    \retval     state of FMC
      \arg        FMC_READY: the operation has been completed
      \arg        FMC_PGERR: program error
      \arg        FMC_TOERR: timeout error
      \arg        FMC_OB_HSPC: option byte security protection code high
*/
fmc_state_enum ob_erase(void)
{
    uint8_t fmc_spc;
    uint32_t fmc_plevel = ob_obstat_plevel_get();
    fmc_state_enum fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

    /* get the original option byte security protection code */
    if(OB_OBSTAT_PLEVEL_NO == fmc_plevel){
        fmc_spc = FMC_NSPC;
    }else if(OB_OBSTAT_PLEVEL_LOW == fmc_plevel){
        fmc_spc = FMC_LSPC;
    }else{
        fmc_spc   = FMC_HSPC;
        fmc_state = FMC_OB_HSPC;
    }

    if(FMC_READY == fmc_state){
        /* start erase the option byte */
        FMC_CTL |= FMC_CTL_OBER;
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

        if(FMC_READY == fmc_state){
            /* reset the OBER bit and enable the option bytes programming */
            FMC_CTL &= ~FMC_CTL_OBER;
            FMC_CTL |= FMC_CTL_OBPG;

            /* restore the last get option byte security protection code */
            OB_SPC = fmc_spc;

            /* wait for the FMC ready */
            fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

            /* reset the OBPG bit */
            FMC_CTL &= ~FMC_CTL_OBPG;
        }else{
            /* reset the OBER bit */
            FMC_CTL &= ~FMC_CTL_OBER;
        }
    }

    /* return the FMC state */
    return fmc_state;
}

/*!
    \brief      enable option byte write protection (OB_WP)
    \param[in]  ob_wp: specify sector to be write protected
                one or more parameters can be selected which are shown as below:
      \arg        OB_WP_NONE: disable all write protection
      \arg        OB_WP_x(x=0..15): write protect specify sector
      \arg        OB_WP_ALL: write protect all sector
    \param[out] none
    \retval     state of FMC
      \arg        FMC_READY: the operation has been completed
      \arg        FMC_PGERR: program error
      \arg        FMC_TOERR: timeout error
      \arg        FMC_OB_HSPC: option byte security protection code high
*/

fmc_state_enum ob_write_protection_enable(uint16_t ob_wp)
{
    uint8_t i;
    uint8_t op_byte[6];
    fmc_state_enum fmc_state;
    uint32_t fmc_plevel = ob_obstat_plevel_get();
    if(OB_OBSTAT_PLEVEL_HIGH == fmc_plevel){
        return FMC_OB_HSPC;
    }
    fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
    for(i = 0U; i < 6U; i++){
        op_byte[i] = OP_BYTE(i);
    }
    op_byte[4] = ~((uint8_t)(ob_wp & OB_LWP));
    op_byte[5] = ~((uint8_t)((ob_wp & OB_HWP) >> 8));

    if(FMC_READY == fmc_state){
        /* start erase the option byte */
        FMC_CTL |= FMC_CTL_OBER;
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

        if(FMC_READY == fmc_state){
            /* reset the OBER bit and enable the option bytes programming */
            FMC_CTL &= ~FMC_CTL_OBER;
            FMC_CTL |= FMC_CTL_OBPG;

            for(i = 0U; i < 6U; i++){
                OP_BYTE(i) = op_byte[i];
                /* wait for the FMC ready */
                fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
                if(FMC_READY != fmc_state){
                    break;
                }
            }

            /* reset the OBPG bit */
            FMC_CTL &= ~FMC_CTL_OBPG;
        }else{
            /* reset the OBER bit */
            FMC_CTL &= ~FMC_CTL_OBER;
        }
    }

    /* return the FMC state */
    return fmc_state;
}

/*!
    \brief      configure security protection
    \param[in]  ob_spc: specify security protection code
                only one parameter can be selected which is shown as below:
      \arg        FMC_NSPC: no security protection
      \arg        FMC_LSPC: low security protection
      \arg        FMC_HSPC: high security protection
    \param[out] none
    \retval     state of FMC
      \arg        FMC_READY: the operation has been completed
      \arg        FMC_PGERR: program error
      \arg        FMC_TOERR: timeout error
      \arg        FMC_OB_HSPC: option byte security protection code high
*/
fmc_state_enum ob_security_protection_config(uint8_t ob_spc)
{
    uint8_t i;
    uint8_t op_byte[6];
    fmc_state_enum fmc_state;
    uint32_t fmc_plevel = ob_obstat_plevel_get();
    if(OB_OBSTAT_PLEVEL_HIGH == fmc_plevel){
        return FMC_OB_HSPC;
    }
    fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
    for(i = 0U; i < 6U; i++){
        op_byte[i] = OP_BYTE(i);
    }
    op_byte[0] = ob_spc;

    if(FMC_READY == fmc_state){
        /* start erase the option byte */
        FMC_CTL |= FMC_CTL_OBER;
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

        if(FMC_READY == fmc_state){
            /* reset the OBER bit and enable the option bytes programming */
            FMC_CTL &= ~FMC_CTL_OBER;
            FMC_CTL |= FMC_CTL_OBPG;

            for(i = 0U; i < 6U; i++){
                OP_BYTE(i) = op_byte[i];
                /* wait for the FMC ready */
                fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
                if(FMC_READY != fmc_state){
                    break;
                }
            }

            /* reset the OBPG bit */
            FMC_CTL &= ~FMC_CTL_OBPG;
        }else{
            /* reset the OBER bit */
            FMC_CTL &= ~FMC_CTL_OBER;
        }
    }

    /* return the FMC state */
    return fmc_state;
}

/*!
    \brief      program the FMC user option byte
                this function can only clear the corresponding bits to be 0 rather than 1.
                the function ob_erase is used to set all the bits to be 1.
    \param[in]  ob_user: user option byte
                one or more parameters (bitwise AND) can be selected which are shown as below:
      \arg        OB_FWDGT_HW: hardware free watchdog timer
      \arg        OB_DEEPSLEEP_RST: generate a reset instead of entering deepsleep mode
      \arg        OB_STDBY_RST: generate a reset instead of entering standby mode
      \arg        OB_BOOT1_SET_1: BOOT1 bit is 1
      \arg        OB_VDDA_DISABLE: disable VDDA monitor
      \arg        OB_SRAM_PARITY_ENABLE: enable sram parity check
      \arg        OB_USER_RSET: reset option byte user byte
    \param[out] none
    \retval     state of FMC
      \arg        FMC_READY: the operation has been completed
      \arg        FMC_PGERR: program error
      \arg        FMC_TOERR: timeout error
      \arg        FMC_OB_HSPC: option byte security protection code high
*/
fmc_state_enum ob_user_write(uint8_t ob_user)
{
    uint8_t i;
    uint8_t op_byte[6];
    fmc_state_enum fmc_state;
    uint32_t fmc_plevel = ob_obstat_plevel_get();
    if(OB_OBSTAT_PLEVEL_HIGH == fmc_plevel){
        return FMC_OB_HSPC;
    }
    fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
    for(i = 0U; i < 6U; i++){
        op_byte[i] = OP_BYTE(i);
    }
    op_byte[1] = (ob_user | OB_USER_MASK);

    if(FMC_READY == fmc_state){
        /* start erase the option byte */
        FMC_CTL |= FMC_CTL_OBER;
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

        if(FMC_READY == fmc_state){
            /* reset the OBER bit and enable the option bytes programming */
            FMC_CTL &= ~FMC_CTL_OBER;
            FMC_CTL |= FMC_CTL_OBPG;

            for(i = 0U; i < 6U; i++){
                OP_BYTE(i) = op_byte[i];
                /* wait for the FMC ready */
                fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
                if(FMC_READY != fmc_state){
                    break;
                }
            }

            /* reset the OBPG bit */
            FMC_CTL &= ~FMC_CTL_OBPG;
        }else{
            /* reset the OBER bit */
            FMC_CTL &= ~FMC_CTL_OBER;
        }
    }

    /* return the FMC state */
    return fmc_state;
}

/*!
    \brief      program the FMC data option byte
    \param[in]  data: the byte to be programmed
    \param[out] none
    \retval     state of FMC
      \arg        FMC_READY: the operation has been completed
      \arg        FMC_PGERR: program error
      \arg        FMC_TOERR: timeout error
      \arg        FMC_OB_HSPC: option byte security protection code high
*/
fmc_state_enum ob_data_program(uint16_t ob_data)
{
    uint8_t i;
    uint8_t op_byte[6];
    fmc_state_enum fmc_state;
    uint32_t fmc_plevel = ob_obstat_plevel_get();
    if(OB_OBSTAT_PLEVEL_HIGH == fmc_plevel){
        return FMC_OB_HSPC;
    }
    fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
    for(i = 0U; i < 6U; i++){
        op_byte[i] = OP_BYTE(i);
    }
    op_byte[2] = (uint8_t)(ob_data & OB_LDATA);
    op_byte[3] = (uint8_t)((ob_data & OB_HDATA) >> 8);

    if(FMC_READY == fmc_state){
        /* start erase the option byte */
        FMC_CTL |= FMC_CTL_OBER;
        FMC_CTL |= FMC_CTL_START;

        /* wait for the FMC ready */
        fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

        if(FMC_READY == fmc_state){
            /* reset the OBER bit and enable the option bytes programming */
            FMC_CTL &= ~FMC_CTL_OBER;
            FMC_CTL |= FMC_CTL_OBPG;

            for(i = 0U; i < 6U; i++){
                OP_BYTE(i) = op_byte[i];
                /* wait for the FMC ready */
                fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
                if(FMC_READY != fmc_state){
                    break;
                }
            }

            /* reset the OBPG bit */
            FMC_CTL &= ~FMC_CTL_OBPG;
        }else{
            /* reset the OBER bit */
            FMC_CTL &= ~FMC_CTL_OBER;
        }
    }

    /* return the FMC state */
    return fmc_state;
}

/*!
    \brief      get OB_USER in register FMC_OBSTAT
    \param[in]  none
    \param[out] none
    \retval     ob_user
*/
uint8_t ob_user_get(void)
{
    return (uint8_t)(FMC_OBSTAT >> 8U);
}

/*!
    \brief      get OB_DATA in register FMC_OBSTAT
    \param[in]  none
    \param[out] none
    \retval     ob_data
*/
uint16_t ob_data_get(void)
{
    return (uint16_t)(FMC_OBSTAT >> 16U);
}

/*!
    \brief      get the FMC option byte write protection (OB_WP) in register FMC_WP
    \param[in]  none
    \param[out] none
    \retval     OB_WP
*/
uint16_t ob_write_protection_get(void)
{
    return (uint16_t)(FMC_WP);
}

/*!
    \brief      get the value of FMC option byte security protection level (PLEVEL) in FMC_OBSTAT register
    \param[in]  none
    \param[out] none
    \retval     the value of PLEVEL
*/
uint32_t ob_obstat_plevel_get(void)
{
    return (FMC_OBSTAT & FMC_OBSTAT_PLVL);
}

/*!
    \brief      get FMC flag state
    \param[in]  flag: check FMC flag
                only one parameter can be selected which is shown as below:
      \arg        FMC_FLAG_BUSY: FMC busy flag
      \arg        FMC_FLAG_PGERR: FMC programming error flag
      \arg        FMC_FLAG_WPERR: FMC write protection error flag
      \arg        FMC_FLAG_END: FMC end of programming flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus fmc_flag_get(uint32_t flag)
{
    FlagStatus status = RESET;

    if(FMC_STAT & flag){
        status = SET;
    }
    /* return the state of corresponding FMC flag */
    return status;
}

/*!
    \brief      clear the FMC pending flag
    \param[in]  flag: clear FMC flag
                one or more parameters can be selected which are shown as below:
      \arg        FMC_FLAG_PGERR: FMC programming error flag
      \arg        FMC_FLAG_WPERR: FMC write protection error flag
      \arg        FMC_FLAG_END: fmc end of programming flag
    \param[out] none
    \retval     none
*/
void fmc_flag_clear(uint32_t flag)
{
    /* clear the flags */
    FMC_STAT = flag;
}

/* FMC interrupts and flags management functions */
/*!
    \brief      enable FMC interrupt
    \param[in]  interrupt: the FMC interrupt source
                one or more parameters can be selected which are shown as below:
      \arg        FMC_INT_END: FMC end of operation interrupt
      \arg        FMC_INT_ERR: FMC error interrupt
    \param[out] none
    \retval     none
*/
void fmc_interrupt_enable(uint32_t interrupt)
{
    FMC_CTL |= interrupt;
}

/*!
    \brief      disable FMC interrupt
    \param[in]  interrupt: the FMC interrupt source
                one or more parameters can be selected which are shown as below:
      \arg        FMC_INT_END: FMC end of operation interrupt
      \arg        FMC_INT_ERR: FMC error interrupt
    \param[out] none
    \retval     none
*/
void fmc_interrupt_disable(uint32_t interrupt)
{
    FMC_CTL &= ~(uint32_t)interrupt;
}

/*!
    \brief      get FMC interrupt flag state
    \param[in]  flag: FMC interrupt flags, refer to fmc_interrupt_flag_enum
                only one parameter can be selected which is shown as below:
      \arg        FMC_INT_FLAG_PGERR: FMC operation error interrupt flag
      \arg        FMC_INT_FLAG_WPERR: FMC erase/program protection error interrupt flag
      \arg        FMC_INT_FLAG_END: FMC end of operation interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus fmc_interrupt_flag_get(uint32_t flag)
{
    FlagStatus status = RESET;

    if(FMC_STAT & flag){
        status = SET;
    }
    /* return the state of corresponding FMC flag */
    return status;
}

/*!
    \brief      clear the FMC pending interrupt flag
    \param[in]  flag: FMC interrupt flags, refer to can_interrupt_flag_enum
                one or more parameters can be selected which are shown as below:
      \arg        FMC_INT_FLAG_PGERR: FMC operation error interrupt flag
      \arg        FMC_INT_FLAG_WPERR: FMC erase/program protection error interrupt flag
      \arg        FMC_INT_FLAG_END: FMC end of operation interrupt flag
    \param[out] none
    \retval     none
*/
void fmc_interrupt_flag_clear(uint32_t flag)
{
    /* clear the flags */
    FMC_STAT = flag;
}

/*!
    \brief      get the FMC state
    \param[in]  none
    \param[out] none
    \retval     fmc_state
*/
static fmc_state_enum fmc_state_get(void)
{
    fmc_state_enum fmc_state = FMC_READY;
  
    if((uint32_t)0x00U != (FMC_STAT & FMC_STAT_BUSY)){
        fmc_state = FMC_BUSY;
    }else{
        if((uint32_t)0x00U != (FMC_STAT & FMC_STAT_WPERR)){
            fmc_state = FMC_WPERR;
        }else{
            if((uint32_t)0x00U != (FMC_STAT & FMC_STAT_PGERR)){
                fmc_state = FMC_PGERR;
            }
        }
    }
    /* return the FMC state */
    return fmc_state;
}

/*!
    \brief      check whether FMC is ready or not
    \param[in]  timeout: timeout count
    \param[out] none
    \retval     fmc_state
*/
static fmc_state_enum fmc_ready_wait(uint32_t timeout)
{
    fmc_state_enum fmc_state = FMC_BUSY;

    /* wait for FMC ready */
    do{
        /* get FMC state */
        fmc_state = fmc_state_get();
        timeout--;
    }while((FMC_BUSY == fmc_state) && (0U != timeout));
  
    if(FMC_BUSY == fmc_state){
        fmc_state = FMC_TOERR;
    }
    /* return the FMC state */
    return fmc_state;
}
