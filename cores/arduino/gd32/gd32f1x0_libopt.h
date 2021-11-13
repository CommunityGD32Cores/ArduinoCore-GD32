/*!
    \file    gd32f1x0_libopt.h
    \brief   library optional for gd32f1x0

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

#ifndef GD32F1X0_LIBOPT_H
#define GD32F1X0_LIBOPT_H

#include "gd32f1x0_adc.h"
#include "gd32f1x0_cec.h"
#include "gd32f1x0_crc.h"
#include "gd32f1x0_cmp.h"
#include "gd32f1x0_dac.h"
#include "gd32f1x0_dbg.h"
#include "gd32f1x0_dma.h"
#include "gd32f1x0_exti.h"
#include "gd32f1x0_fmc.h"
#include "gd32f1x0_gpio.h"
#include "gd32f1x0_syscfg.h"
#include "gd32f1x0_i2c.h"
#include "gd32f1x0_fwdgt.h"
#include "gd32f1x0_pmu.h"
#include "gd32f1x0_rcu.h"
#include "gd32f1x0_rtc.h"
#include "gd32f1x0_spi.h"
#include "gd32f1x0_timer.h"
#include "gd32f1x0_usart.h"
#include "gd32f1x0_wwdgt.h"
#include "gd32f1x0_misc.h"
#include "gd32f1x0_tsi.h"
#ifdef GD32F170_190
#include "gd32f1x0_slcd.h"
#include "gd32f1x0_opa.h"
#include "gd32f1x0_ivref.h"
#include "gd32f1x0_can.h"
#endif /* GD32F170_190 */

#endif /* GD32F1X0_LIBOPT_H */
