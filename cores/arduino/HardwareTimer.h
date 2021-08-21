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

#ifndef HARDWARETIMER_H
#define HARDWARETIMER_H

#include "timer.h"

// Copied from 1.0.x firmware library for compatibility, but this is
// unspecified in the data sheet. Being able to use both edges is not
// described as supported behavior, and bit 4 is in a reserved section
// of the TIMERx_CHCTL2 register. -bjc (2021-Aug-20)
#ifndef TIMER_IC_POLARITY_BOTH_EDGE
#define TIMER_IC_POLARITY_BOTH_EDGE         ((uint16_t)0x000AU)                     /*!< input capture both edge(not for timer1..6) */
#endif


typedef void(*timerCallback_t)(void);

class HardwareTimer {
public:
    HardwareTimer(void) {};                                                   //default construct
    HardwareTimer(uint32_t instance);                                         //HardwareTimer construct
    void start(void);                                                         //start timer
    void stop(void);                                                          //stop timer
    void refresh(
        void);                                                       //update some registers to restart counters
    void setPrescaler(uint16_t prescaler);                                    //set prescaler
    void setCounter(uint16_t count);                                          //set counter
    void setrRpetitionValue(uint16_t repetition);                             //set rpetition value
    void setPeriodTime(uint32_t time, enum timeFormat format = FORMAT_MS);    //set timer period
    void setReloadValue(uint32_t
                        value);                                      //set timer period with the inital format
    void attachInterrupt(timerCallback_t callback,
                         uint8_t channel = 0xff);   //attach callback for period/capture interrupt
    void detachInterrupt(uint8_t channel =
                             0xff);                             //detach callback for period/capture interrupt
    void periodCallback(void);                                                //period callback handler
    void captureCallback(uint8_t channel);                                    //capture callback handler
    void setCaptureMode(uint32_t ulpin, uint8_t channel, captureMode mode);   //set timer capture mode
    uint32_t getCaptureValue(uint8_t
                             channel);                                //get timer channel capture value
    uint32_t getTimerClkFre(
        void);                                            //get timer clock frequency
private:
    uint32_t timerDevice;
    bool isTimerActive;
    timerPeriod_t timerPeriod;
    timerCallback_t updateCallback;
    timerCallback_t captureCallbacks[4] = {0};
};

extern timerhandle_t timerHandle;

#endif /* HARDWARETIMER_H */
