/*!
    \file    gd32f1x0_opa.h
    \brief   definitions for the OPA

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

#ifndef GD32F1X0_OPA_H
#define GD32F1X0_OPA_H

#include "gd32f1x0.h"

/* OPAx(x=0,1,2) definitions */
#define OPA                     OPA_BASE
#define OPA0                    0U
#define OPA1                    1U
#define OPA2                    2U

/* registers definitions */
#define OPA_CTL                 REG32(OPA + 0x00000000U)     /*!< OPA control register */
#define OPA_BT                  REG32(OPA + 0x00000004U)     /*!< OPA bias trimming register for normal mode */
#define OPA_LPBT                REG32(OPA + 0x00000008U)     /*!< OPA bias trimming register for low power mode */

/* bits definitions */
/* OPA_CLT */
#define OPA_CTL_OPA0PD          BIT(0)                 /*!< OPA0 power down */
#define OPA_CTL_T3OPA0          BIT(1)                 /*!< T3 switch enable for OPA0 */
#define OPA_CTL_S1OPA0          BIT(2)                 /*!< S1 switch enable for OPA0 */
#define OPA_CTL_S2OPA0          BIT(3)                 /*!< S2 switch enable for OPA0 */
#define OPA_CTL_S3OPA0          BIT(4)                 /*!< S3 switch enable for OPA0 */
#define OPA_CTL_OPA0CAL_L       BIT(5)                 /*!< OPA1 offset calibration for P diff */
#define OPA_CTL_OPA0CAL_H       BIT(6)                 /*!< OPA1 offset calibration for N diff */
#define OPA_CTL_OPA0LPM         BIT(7)                 /*!< OPA0 low power mode */
#define OPA_CTL_OPA1PD          BIT(8)                 /*!< OPA1 power down */
#define OPA_CTL_T3OPA1          BIT(9)                 /*!< T3 switch enable for OPA1 */
#define OPA_CTL_S1OPA1          BIT(10)                /*!< S1 switch enable for OPA1 */
#define OPA_CTL_S2OPA1          BIT(11)                /*!< S2 switch enable for OPA1 */
#define OPA_CTL_S3OPA1          BIT(12)                /*!< S3 switch enable for OPA1 */
#define OPA_CTL_OPA1CAL_L       BIT(13)                /*!< OPA1 offset calibration for P diff */
#define OPA_CTL_OPA1CAL_H       BIT(14)                /*!< OPA1 offset calibration for N diff */
#define OPA_CTL_OPA1LPM         BIT(15)                /*!< OPA1 low power mode */
#define OPA_CTL_OPA2PD          BIT(16)                /*!< OPA2 power down */
#define OPA_CTL_T3OPA2          BIT(17)                /*!< T3 switch enable for OPA2 */
#define OPA_CTL_S1OPA2          BIT(18)                /*!< S1 switch enable for OPA2 */
#define OPA_CTL_S2OPA2          BIT(19)                /*!< S2 switch enable for OPA2 */
#define OPA_CTL_S3OPA2          BIT(20)                /*!< S3 switch enable for OPA2 */
#define OPA_CTL_OPA2CAL_L       BIT(21)                /*!< OPA2 offset calibration for P diff */
#define OPA_CTL_OPA2CAL_H       BIT(22)                /*!< OPA2 offset calibration for N diff */
#define OPA_CTL_OPA2LPM         BIT(23)                /*!< OPA2 low power mode */
#define OPA_CTL_S4OPA1          BIT(27)                /*!< S4 switch enable for OPA2 */
#define OPA_CTL_OPA_RANGE       BIT(28)                /*!< Power supply range */
#define OPA_CTL_OPA0CALOUT      BIT(29)                /*!< OPA0 calibration output */
#define OPA_CTL_OPA1CALOUT      BIT(30)                /*!< OPA1 calibration output */
#define OPA_CTL_OPA2CALOUT      BIT(31)                /*!< OPA2 calibration output */

/* OPA_BT */ 
#define OPA_BT_OA0_TRIM_LOW     BITS(0,4)              /*!< OPA0, normal mode 5-bit bias trim value for PMOS pairs */
#define OPA_BT_OA0_TRIM_HIGH    BITS(5,9)              /*!< OPA0, normal mode 5-bit bias trim value for NMOS pairs */
#define OPA_BT_OA1_TRIM_LOW     BITS(10,14)            /*!< OPA1, normal mode 5-bit bias trim value for PMOS pairs */
#define OPA_BT_OA1_TRIM_HIGH    BITS(15,19)            /*!< OPA1, normal mode 5-bit bias trim value for NMOS pairs */
#define OPA_BT_OA2_TRIM_LOW     BITS(20,24)            /*!< OPA2, normal mode 5-bit bias trim value for PMOS pairs*/
#define OPA_BT_OA2_TRIM_HIGH    BITS(25,29)            /*!< OPA2, normal mode 5-bit bias trim value for NMOS pairs */
#define OPA_BT_OT_USER          BIT(31)                /*!< OPA trimming mode */

/* OPA_LPBT */
#define OPA_LPBT_OA0_TRIM_LOW   BITS(0,4)              /*!< OPA0, low-power mode 5-bit bias trim value for PMOS pairs */
#define OPA_LPBT_OA0_TRIM_HIGH  BITS(5,9)              /*!< OPA0, low-power mode 5-bit bias trim value for NMOS pairs */
#define OPA_LPBT_OA1_TRIM_LOW   BITS(10,14)            /*!< OPA1, low-power mode 5-bit bias trim value for PMOS pairs */
#define OPA_LPBT_OA1_TRIM_HIGH  BITS(15,19)            /*!< OPA1, low-power mode 5-bit bias trim value for NMOS pairs */
#define OPA_LPBT_OA2_TRIM_LOW   BITS(20,24)            /*!< OPA2, low-power mode 5-bit bias trim value for PMOS pairs */
#define OPA_LPBT_OA2_TRIM_HIGH  BITS(25,29)            /*!< OPA2, low-power mode 5-bit bias trim value for NMOS pairs */

/* constants definitions */
/* opa switch definitions */
#define OPA_T3OPA0              OPA_CTL_T3OPA0          /*!< T3 switch enable for OPA0 */
#define OPA_S1OPA0              OPA_CTL_S1OPA0          /*!< S1 switch enable for OPA0 */
#define OPA_S2OPA0              OPA_CTL_S2OPA0          /*!< S2 switch enable for OPA0 */
#define OPA_S3OPA0              OPA_CTL_S3OPA0          /*!< S3 switch enable for OPA0 */
#define OPA_T3OPA1              OPA_CTL_T3OPA1          /*!< T3 switch enable for OPA1 */
#define OPA_S1OPA1              OPA_CTL_S1OPA1          /*!< S1 switch enable for OPA1 */
#define OPA_S2OPA1              OPA_CTL_S2OPA1          /*!< S2 switch enable for OPA1 */
#define OPA_S3OPA1              OPA_CTL_S3OPA1          /*!< S3 switch enable for OPA1 */
#define OPA_S4OPA1              OPA_CTL_S4OPA1          /*!< S4 switch enable for OPA1 */
#define OPA_T3OPA2              OPA_CTL_T3OPA2          /*!< T3 switch enable for OPA2 */
#define OPA_S1OPA2              OPA_CTL_S1OPA2          /*!< S1 switch enable for OPA2 */
#define OPA_S2OPA2              OPA_CTL_S2OPA2          /*!< S2 switch enable for OPA2 */
#define OPA_S3OPA2              OPA_CTL_S3OPA2          /*!< S3 switch enable for OPA2 */

/* opa trimming mode */
#define OPA_BT_TRIM_FACTORY     ((uint32_t)0x00000000U) /*!< factory trimming */
#define OPA_BT_TRIM_USER        OPA_BT_OT_USER          /*!< user trimming */

/* opa input */
#define OPA_INPUT_N             ((uint32_t)0x00000040U) /*!< NMOS input */
#define OPA_INPUT_P             ((uint32_t)0x00000020U) /*!< PMOS input */

/* opa power range */
#define OPA_POWRANGE_LOW        ((uint32_t)0x00000000U) /*!< low power range is selected (VDDA is lower than 3.3V) */
#define OPA_POWRANGE_HIGH       OPA_CTL_OPA_RANGE       /*!< high power range is selected (VDDA is higher than 3.3V) */

/* function declarations */
/* initialization functions */
/* reset OPA */
void opa_deinit(void);
/* enable OPA */
void opa_enable(uint32_t opa_periph);
/* disable OPA */
void opa_disable(uint32_t opa_periph);
/* enable OPA switch */
void opa_switch_enable(uint32_t opax_swy);
/* disable OPA switch */
void opa_switch_disable(uint32_t opax_swy);

/* function configuration */
/* enable OPA low_power mode */
void opa_low_power_enable(uint32_t opa_periph);
/* disable OPA low_power mode */
void opa_low_power_disable(uint32_t opa_periph);
/* set OPA power range */
void opa_power_range_config(uint32_t powerrange); 
/* set OPA bias trimming mode */
void opa_trim_mode_set(uint32_t opa_trimmode);
/* set OPA bias trimming value normal mode */
void opa_trim_value_config(uint32_t opa_periph,uint32_t opa_input,uint32_t opa_trimvalue);
/* set OPA bias trimming value low power mode */
void opa_trim_value_lp_config(uint32_t opa_periph,uint32_t opa_input,uint32_t opa_trimvalue);

/* flag & interrupt functions */
/* get OPA calibration flag */
FlagStatus opa_cal_out_get(uint32_t opa_periph);

#endif /* GD32F1X0_OPA_H */

#endif /* GD32F170_190 */
