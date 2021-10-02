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

#ifndef _TIMER_H_
#define _TIMER_H_

#include "gd32xxyy.h"
#include "PinNames.h"
#include "PeripheralPins.h"

/* ############# Timer interrupt definition ############# */
#if defined(TIMER0) && !defined(TIMER0_IRQn)
    #if defined(GD32F30x)
#if defined(GD32F30X_CL) || defined(GD32F30X_XD)
        #define TIMER0_IRQn TIMER0_UP_TIMER9_IRQn
        #define TIMER0_IRQHandler TIMER0_UP_TIMER9_IRQHandler
#else
        #define TIMER0_IRQn TIMER0_UP_IRQn
        #define TIMER0_IRQHandler TIMER0_UP_IRQHandler
#endif
    #endif
#endif

#if defined(TIMER7) && !defined(TIMER7_IRQn)
    #if defined(GD32F30x)
#if defined(GD32F30X_CL) || defined(GD32F30X_XD)
        #define TIMER7_IRQn TIMER7_UP_TIMER12_IRQn
        #define TIMER7_IRQHandler TIMER7_UP_TIMER12_IRQHandler
#else
        #define TIMER7_IRQn TIMER7_UP_IRQn
        #define TIMER7_IRQHandler TIMER7_UP_IRQHandler
#endif
    #endif
#endif

#if defined(GD32F30X_CL) || defined(GD32F30X_XD)
#if defined(TIMER8) && !defined(TIMER8_IRQn)
    #if defined(GD32F30x)
        #define TIMER8_IRQn TIMER0_BRK_TIMER8_IRQn
        #define TIMER8_IRQHandler TIMER0_BRK_TIMER8_IRQHandler
    #endif
#endif

#if defined(TIMER9) && !defined(TIMER9_IRQn)
    #if defined(GD32F30x)
        #define TIMER9_IRQn TIMER0_UP_TIMER9_IRQn
        #define TIMER9_IRQHandler TIMER0_UP_TIMER9_IRQHandler
    #endif
#endif

#if defined(TIMER10) && !defined(TIMER10_IRQn)
    #if defined(GD32F30x)
        #define TIMER10_IRQn TIMER0_TRG_CMT_TIMER10_IRQn
        #define TIMER10_IRQHandler TIMER0_TRG_CMT_TIMER10_IRQHandler
    #endif
#endif

#if defined(TIMER11) && !defined(TIMER11_IRQn)
    #if defined(GD32F30x)
        #define TIMER11_IRQn TIMER7_BRK_TIMER11_IRQn
        #define TIMER11_IRQHandler TIMER7_BRK_TIMER11_IRQHandler
    #endif
#endif

#if defined(TIMER12) && !defined(TIMER12_IRQn)
    #if defined(GD32F30x)
        #define TIMER12_IRQn TIMER7_UP_TIMER12_IRQn
        #define TIMER12_IRQHandler TIMER7_UP_TIMER12_IRQHandler
    #endif
#endif

#if defined(TIMER13) && !defined(TIMER13_IRQn)
    #if defined(GD32F30x)
        #define TIMER13_IRQn TIMER7_TRG_CMT_TIMER13_IRQn
        #define TIMER13_IRQHandler TIMER7_TRG_CMT_TIMER13_IRQHandler
    #endif
#endif
#endif

typedef void(*callBack_t)(uint32_t instance, uint8_t channel);

enum captureMode {
    RISING_EDGE,
    FALLING_EDGE,
    BOTH_EDGE
};

enum timeFormat {
    FORMAT_TICK,
    FORMAT_US,
    FORMAT_MS,
    FORMAT_S,
    FORMAT_HZ
};

typedef struct {
    uint32_t timer;
    uint8_t channel;
} pwmDevice_t;

typedef struct {
    uint32_t period;
    uint32_t cycle;
    enum timeFormat format;
} pwmPeriodCycle_t;

typedef struct {
    uint32_t time;
    enum timeFormat format;
} timerPeriod_t;

typedef struct timerhandle {
    void (*init)(uint32_t instance, timerPeriod_t *imerPeriod);
    void (*start)(uint32_t instance);
    void (*stop)(uint32_t instance);
    void (*setPeriodTime)(uint32_t instance, timerPeriod_t *timerPeriod);
    void (*refresh)(uint32_t instance);
    void (*enableUpdateIT)(uint32_t instance);
    void (*enableCaptureIT)(uint32_t instance, uint8_t channel);
    void (*disableUpdateIT)(uint32_t instance);
    void (*disableCaptureIT)(uint32_t instance, uint8_t channel);
    void (*interruptHandle)(uint32_t instance);
    callBack_t captureInterruptHandle[4];
} timerhandle_t;

typedef struct pwmhandle {
    void (*init)(pwmDevice_t *pwmDevice, pwmPeriodCycle_t *pwmPeriodCycle);
    void (*start)(pwmDevice_t *pwmDevice);
    void (*stop)(pwmDevice_t *pwmDevice);
    void (*setPeriodCycle)(pwmDevice_t *pwmDevice, pwmPeriodCycle_t *pwmPeriodCycle);
    void (*writeCycleValue)(pwmDevice_t *pwmDevice, pwmPeriodCycle_t *pwmPeriodCycle);
    void (*enablePWMIT)(pwmDevice_t *pwmDevice);
    void (*disablePWMIT)(pwmDevice_t *pwmDevice);
    void (*interruptHandle)(uint32_t instance, uint8_t channel);
} pwmhandle_t;

#ifdef __cplusplus
extern "C"
{
#endif

pwmDevice_t getTimerDeviceFromPinname(PinName
                                      instance);                              //get timer device from pinname
uint32_t getPWMIndex(pwmDevice_t
                     instance);                                           //get pwm index
uint32_t getTimerIndex(uint32_t
                       instance);                                            //get timer index
void timer_clock_enable(uint32_t
                        instance);                                           //enable timer clock
void timer_clock_disable(uint32_t
                         instance);                                          //disable timer clock

void Timer_init(uint32_t instance,
                timerPeriod_t *imerPeriod);                        //initialize timer
void Timer_start(uint32_t instance);                                                  //start timer
void Timer_stop(uint32_t instance);                                                   //stop timer
void Timer_setPeriodTime(uint32_t instance,
                         timerPeriod_t *timerPeriod);              //set timer period
void Timer_rfresh(uint32_t
                  instance);                                                 //update some registers to restart counters
void Timer_enableUpdateIT(uint32_t
                          instance);                                         //enable timer update interrupt
void Timer_disableUpdateIT(uint32_t
                           instance);                                        //disable timer update interrupt
void Timer_updateHandle(uint32_t
                        instance);                                           //timer update interrupt handler
void Timer_enableCaptureIT(uint32_t instance,
                           uint8_t channel);                       //enable timer channel capture interrupt
void Timer_disableCaptureIT(uint32_t instance,
                            uint8_t channel);                      //disable timer channel capture interrupt
void Timer_captureHandle(uint32_t timer,
                         uint8_t channel);                            //timer capture interrupt handler

void PWM_init(pwmDevice_t *pwmDevice,
              pwmPeriodCycle_t *pwmPeriodCycle);              //initialize pwm
void PWM_start(pwmDevice_t
               *pwmDevice);                                               //start pwm output
void PWM_stop(pwmDevice_t
              *pwmDevice);                                                //stop pwm output
void PWM_setPeriodCycle(pwmDevice_t *pwmDevice,
                        pwmPeriodCycle_t *pwmPeriodCycle);    //set pwm period and cycle
void PWM_writeCyclevalue(pwmDevice_t *pwmDevice,
                         pwmPeriodCycle_t *pwmPeriodCycle);   //set pwm  cycle
void PWM_enablePWMIT(pwmDevice_t
                     *pwmDevice);                                         //enable pwm interrupt
void PWM_disablePWMIT(pwmDevice_t
                      *pwmDevice);                                        //disable pwm interrupt
void PWM_irqHandle(uint32_t instance,
                   uint8_t channel);                               //pwm capture/compare interrupt handler

uint32_t  getTimerClkFrequency(uint32_t
                               instance);                                    //get timer clock frequency
IRQn_Type getTimerUpIrq(uint32_t
                        tim);                                                //get timer update IRQn
IRQn_Type getTimerCCIrq(uint32_t
                        tim);                                                //get timer capture/compare IRQn


#ifdef __cplusplus
}
#endif

#endif /* _TIMER_H_ */
