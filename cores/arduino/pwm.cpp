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

#include "pwm.h"
#include "pins_arduino.h"
#define PWMNUMS   56
PWM *pwmObj[PWMNUMS] = {NULL};

/*!
    \brief      PWM object construct
    \param[in]  instance: PWMx(x=0..11)
    \param[out] none
    \retval     none
*/
PWM::PWM(uint32_t pin)
{
    PinName instance = DIGITAL_TO_PINNAME(pin);
    this->pwmCallback = NULL;
    this->pwmPeriodCycle = {9999, 4999, FORMAT_US};
    this->pwmDevice   = getTimerDeviceFromPinname(instance);
    this->ispwmActive = false;
    this->index = getPWMIndex(pwmDevice);
    pwmObj[index] = this;
    pinmap_pinout(instance, PinMap_PWM);
    pwmHandle.init(&pwmDevice, &pwmPeriodCycle);
}

/*!
    \brief      start pwm output
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PWM::start(void)
{
    pwmHandle.start(&pwmDevice);
    this->ispwmActive = true;
}

/*!
    \brief      stop pwm output
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PWM::stop(void)
{
    pwmHandle.stop(&pwmDevice);
    this->ispwmActive = false;
}

/*!
    \brief      set pwm period and cyclye
    \param[in]  time: period time
    \param[in]  cycle: cycle time
    \param[in]  format: time format
    \param[out] none
    \retval     none
*/
void PWM::setPeriodCycle(uint32_t time, uint16_t cycle, enum timeFormat format)
{
    this->pwmPeriodCycle.period = time;
    this->pwmPeriodCycle.cycle = cycle;
    this->pwmPeriodCycle.format = format;
    pwmHandle.setPeriodCycle(&pwmDevice, &pwmPeriodCycle);
}

/*!
    \brief      set pwm cycle time with the inital format
    \param[in]  value: period time
    \param[out] none
    \retval     none
*/
void PWM::writeCycleValue(uint32_t cycle, enum timeFormat format)
{
    this->pwmPeriodCycle.cycle = cycle;
    this->pwmPeriodCycle.format = format;
    pwmHandle.writeCycleValue(&pwmDevice, &pwmPeriodCycle);
}

/*!
    \brief      attach callback for capture/compare interrupt
    \param[in]  callback: callback function
    \param[out] none
    \retval     none
*/
void PWM::attachInterrupt(pwmCallback_t callback)
{
    this->pwmCallback = callback;
    pwmHandle.enablePWMIT(&pwmDevice);
}

/*!
    \brief      detach callback for capture/compare interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PWM::detachInterrupt(void)
{
    this->pwmCallback = NULL;
    pwmHandle.disablePWMIT(&pwmDevice);
}

/*!
    \brief      capture/compare callback handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PWM::captureCompareCallback(void)
{
    if(NULL != this->pwmCallback) {
        this->pwmCallback();
    }
}

extern "C"
{
    /*!
        \brief      pwm capture/compare interrupt handler
        \param[in]  timer: PWMx(x=0..11)
        \param[in]  channel: TIMER_CH_x(x=0..4)
        \param[out] none
        \retval     none
    */
    void PWM_irqHandle(uint32_t timer, uint8_t channel)
    {
        uint32_t index = 0;
        pwmDevice_t pwmDevice = {timer, channel};
        index = getPWMIndex(pwmDevice);
        if(pwmObj[index]) {
            pwmObj[index]->captureCompareCallback();
        }
    }
}

