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

#include "HardwareRTC.h"

HWRTC rtc;

/*!
    \brief      HWRTC object construct
    \param[in]  none
    \param[out] none
    \retval     none
*/
HWRTC::HWRTC(void)
{
    rtc_init();
    this->UTCTime = {
        .year    = 1970,
        .month   = 1,
        .day     = 1,
        .hour    = 0,
        .minutes = 0,
        .seconds = 0
    };
    this->callback[0] = NULL;
    this->callback[1] = NULL;
    this->callback[2] = NULL;
}

/*!
    \brief      get UTC time
    \param[in]  utcTime: point to UTC format time
    \param[out] none
    \retval     none
*/
void HWRTC::getUTCTime(UTCTimeStruct *utcTime)
{
    utcTime->year    = UTCTime.year;
    utcTime->month   = UTCTime.month;
    utcTime->day     = UTCTime.day;
    rtc_getUTCTime(utcTime);
}

/*!
    \brief      set UTC time
    \param[in]  utcTime: point to UTC format time
    \param[out] none
    \retval     none
*/
void HWRTC::setUTCTime(UTCTimeStruct *utcTime)
{
    rtc_setUTCTime(utcTime);
}


/*!
    \brief      get second time
    \param[in]  none
    \param[out] none
    \retval     second counts
*/
uint32_t HWRTC::getSecTime(void)
{
    return rtc_getSecTime();
}

/*!
    \brief      set second counts
    \param[in]  utcTime: point to UTC format time
    \param[out] none
    \retval     none
*/
void HWRTC::setSecTime(uint32_t secTime)
{
    rtc_setSecTime(secTime);
}

/*!
    \brief      set alarm time
    \param[in]  offset: alarm time base
    \param[in]  mode: alarm time format
    \param[out] none
    \retval     none
*/
void HWRTC::setAlarmTime(uint32_t offset, ALARM_OFFSET_FORMAT mode)
{
    uint32_t seconds = 0;
    switch(mode) {
        case RTC_ALARM_S:
            seconds = offset;
            break;
        case RTC_ALARM_M:
            seconds = offset * SECONDS_PER_MINUTE;
            break;
        case RTC_ALARM_H:
            seconds = offset * SECONDS_PER_HOUR;
            break;
    }
    rtc_setAlarmTime(rtc_getSecTime() + seconds);
}

/*!
    \brief      attach interrupt
    \param[in]  callback: callback function
    \param[in]  mode: interrupt mode
    \param[out] none
    \retval     none
*/
void HWRTC::attachInterrupt(RTCCallback_t callback, INT_MODE mode)
{
    switch(mode) {
        case INT_SECOND_MODE:
            this->callback[0] = callback;
            rtc_attachInterrupt((INT_MODE)0);
            break;
        case INT_ALARM_MODE:
            this->callback[1] = callback;
            rtc_attachInterrupt((INT_MODE)1);
            break;
        case INT_OVERFLOW_MODE:
            this->callback[2] = callback;
            rtc_attachInterrupt((INT_MODE)2);
            break;
    }
}

/*!
    \brief      detach interrupt
    \param[in]  mode: interrupt mode
    \param[out] none
    \retval     none
*/
void HWRTC::detachInterrupt(INT_MODE mode)
{
    switch(mode) {
        case INT_SECOND_MODE:
            rtc_detachInterrupt((INT_MODE)0);
            this->callback[0] = NULL;
            break;
        case INT_ALARM_MODE:
            rtc_detachInterrupt((INT_MODE)1);
            this->callback[1] = NULL;
            break;
        case INT_OVERFLOW_MODE:
            rtc_detachInterrupt((INT_MODE)2);
            this->callback[2] = NULL;
            break;
    }
}

/*!
    \brief      interrupt handler
    \param[in]  mode: interrupt mode
    \param[out] none
    \retval     none
*/
void HWRTC::interruptHandler(INT_MODE mode)
{
    uint8_t i;
    switch(mode) {
        case INT_SECOND_MODE:
            if(NULL != this->callback[0]) {
                this->callback[0]();
            }
            break;
        case INT_ALARM_MODE:
            if(NULL != this->callback[1]) {
                this->callback[1]();
            }
            break;
        case INT_OVERFLOW_MODE:
            if(NULL != this->callback[2]) {
                this->callback[2]();
            }
            break;
        default:
            break;
    }
}


extern "C"
{

    /*!
        \brief      rtc handler
        \param[in]  mode: interrupt mode
        \param[out] none
        \retval     none
    */
    void RTC_Handler(INT_MODE mode)
    {
        rtc.interruptHandler(mode);
    }
}
