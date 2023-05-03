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

#include "HardwareTimer.h"
#include "pins_arduino.h"
#define TIMERNUMS   17

HardwareTimer *hardwaretimerObj[TIMERNUMS] = {NULL};

/*!
    \brief      HardwareTimer object construct
    \param[in]  instance: TIMERx(x=0..13)
    \param[out] none
    \retval     none
*/
HardwareTimer::HardwareTimer(uint32_t instance)
{
    uint32_t index = getTimerIndex(instance);
    hardwaretimerObj[index] = this;
    this->timerDevice = instance;
    this->updateCallback = NULL;
    this->isTimerActive = false;
    this->timerPeriod.time = 1;
    this->timerPeriod.format = FORMAT_MS;
    timerHandle.init(timerDevice, &timerPeriod);
}

/*!
    \brief      start timer
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardwareTimer::start(void)
{
    timerHandle.start(timerDevice);
    this->isTimerActive = true;
}

/*!
    \brief      stop timer
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardwareTimer::stop(void)
{
    timerHandle.stop(timerDevice);
    this->isTimerActive = false;
}

/*!
    \brief      set prescaler
    \param[in]  prescaler: prescaler value
    \param[out] none
    \retval     none
*/
void HardwareTimer::setPrescaler(uint16_t prescaler)
{
    timer_prescaler_config(timerDevice, prescaler - 1, TIMER_PSC_RELOAD_NOW);
}

/*!
    \brief      set counter
    \param[in]  count: counter value
    \param[out] none
    \retval     none
*/
void HardwareTimer::setCounter(uint16_t count)
{
    timer_counter_value_config(timerDevice, count - 1);
}

/*!
    \brief      set rpetition value
    \param[in]  repetition: rpetition value
    \param[out] none
    \retval     none
*/
void HardwareTimer::setrRpetitionValue(uint16_t repetition)
{
    timer_repetition_value_config(timerDevice, repetition - 1);
}

/*!
    \brief      set timer period
    \param[in]  time: period time
    \param[in]  format: time format
    \param[out] none
    \retval     none
*/
void HardwareTimer::setPeriodTime(uint32_t time, enum timeFormat format)
{
    this->timerPeriod.time = time;
    this->timerPeriod.format = format;
    timerHandle.setPeriodTime(timerDevice, &timerPeriod);
}

/*!
    \brief      set timer period with the inital format
    \param[in]  value: period time
    \param[out] none
    \retval     none
*/
void HardwareTimer::setReloadValue(uint32_t value)
{
    timer_autoreload_value_config(timerDevice, value - 1);
}

/*!
    \brief      update some registers to restart counters
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardwareTimer::refresh(void)
{
    timerHandle.refresh(timerDevice);
}

/*!
    \brief      attach callback for period interrupt
    \param[in]  callback: callback function
    \param[out] none
    \retval     none
*/
void HardwareTimer::attachInterrupt(timerCallback_t callback, uint8_t channel)
{
    if (channel < 4) {
        this->captureCallbacks[channel] = callback;
        timerHandle.enableCaptureIT(timerDevice, channel);
    } else if (0xFF == channel) {
        this->updateCallback = callback;
        timerHandle.enableUpdateIT(timerDevice);
    }
}

/*!
    \brief      detach callback for period interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardwareTimer::detachInterrupt(uint8_t channel)
{
    if (channel < 4) {
        this->captureCallbacks[channel] = NULL;
        timerHandle.disableCaptureIT(timerDevice, channel);
    } else if (0xFF == channel) {
        this->updateCallback = NULL;
        timerHandle.disableUpdateIT(timerDevice);
    }
}

void HardwareTimer::setCaptureMode(uint32_t ulpin, uint8_t channel, captureMode mode)
{
    timer_ic_parameter_struct timer_icinitpara;

    PinName pinname = DIGITAL_TO_PINNAME(ulpin);
    uint32_t function = pinmap_find_function(pinname, PinMap_PWM);
    uint32_t remap  = GD_PIN_REMAP_GET(function);
    uint32_t port = gpio_port[GD_PORT_GET(pinname)];
    uint32_t pin = gpio_pin[GD_PIN_GET(pinname)];
    gpio_clock_enable(GD_PORT_GET(pinname));
#if defined(GD32F30x)
    rcu_periph_clock_enable(RCU_AF);
    gpio_init(port, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, pin);
    if (0 != remap) {
        gpio_pin_remap_config(GD_GPIO_REMAP[remap], ENABLE);
    }
#elif defined(GD32F3x0) || defined(GD32F1x0)
    /* !!TODO!! */
#endif

    switch (mode) {
        case RISING_EDGE:
            timer_icinitpara.icpolarity = TIMER_IC_POLARITY_RISING;
            break;
        case FALLING_EDGE:
            timer_icinitpara.icpolarity = TIMER_IC_POLARITY_FALLING;
            break;
        case BOTH_EDGE:
            timer_icinitpara.icpolarity = TIMER_IC_POLARITY_BOTH_EDGE;
            break;
        default:
            timer_icinitpara.icpolarity = TIMER_IC_POLARITY_RISING;
            break;
    }
    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
    timer_icinitpara.icfilter    = 0x0;
    timer_input_capture_config(timerDevice, channel, &timer_icinitpara);
    timer_auto_reload_shadow_enable(timerDevice);
}

uint32_t HardwareTimer::getCaptureValue(uint8_t channel)
{
    return timer_channel_capture_value_register_read(timerDevice, channel);
}
/*!
    \brief      get timer clock frequency
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint32_t HardwareTimer::getTimerClkFre(void)
{
    return getTimerClkFrequency(timerDevice);
}

/*!
    \brief      period callback handler
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardwareTimer::periodCallback(void)
{
    if (NULL != this->updateCallback) {
        this->updateCallback();
    }
}

void HardwareTimer::captureCallback(uint8_t channel)
{
    if (NULL != this->captureCallbacks[channel]) {
        this->captureCallbacks[channel]();
    }
}

extern "C"
{
    /*!
        \brief      timer update interrupt handler
        \param[in]  timer: TIMERx(x=0..13)
        \param[out] none
        \retval     none
    */
    void Timer_updateHandle(uint32_t timer)
    {
        uint32_t index = getTimerIndex(timer);
        if (hardwaretimerObj[index]) {
            hardwaretimerObj[index]->periodCallback();
        }
    }
    /*!
        \brief      timer capture interrupt handler
        \param[in]  timer: TIMERx(x=0..13)
        \param[in]  channel: TIMERx(x=0..3)
        \param[out] none
        \retval     none
    */
    void Timer_captureHandle(uint32_t timer, uint8_t channel)
    {
        uint32_t index =  getTimerIndex(timer);
        if (hardwaretimerObj[index]) {
            hardwaretimerObj[index]->captureCallback(channel);
        }
    }
}

