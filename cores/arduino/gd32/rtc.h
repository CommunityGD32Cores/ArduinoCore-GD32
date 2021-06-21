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

#ifndef _GD_RTC_H_
#define _GD_RTC_H_

#include "gd32f30x_rtc.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define IsLeapYear(yr) (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))
#define YearLength(yr)  ((uint16_t)(IsLeapYear(yr) ? 366 : 365))
#define SECONDS_PER_MINUTE    60UL
#define SECONDS_PER_HOUR      3600UL
#define SECONDS_PER_DAY       86400UL

typedef struct {
    uint16_t year;    // 1970+
    uint8_t month;    // 1-12
    uint8_t day;      // 1-31
    uint8_t hour;     // 0-23
    uint8_t minutes;  // 0-59
    uint8_t seconds;  // 0-59
} UTCTimeStruct;

typedef enum {
    RTC_ALARM_S,
    RTC_ALARM_M,
    RTC_ALARM_H
} ALARM_OFFSET_FORMAT;

typedef enum {
    INT_SECOND_MODE,
    INT_ALARM_MODE,
    INT_OVERFLOW_MODE
} INT_MODE;

void rtc_init(void);                                            //rtc init
void rtc_setUTCTime(UTCTimeStruct *utcTime);                    //rtc set UTC time
void rtc_setSecTime(uint32_t secTime);                          //rtc set second time
uint32_t rtc_getSecTime(void);                                  //rtc get second time
void rtc_getUTCTime(UTCTimeStruct *utcTime);                    //rtc get UTC time
void rtc_setAlarmTime(uint32_t alarmTime);                      //rtc set alarm time
void rtc_attachInterrupt(INT_MODE mode);                        //rtc attach interrupt
void rtc_detachInterrupt(INT_MODE mode);                        //rtc detach interrupt
extern void RTC_Handler(INT_MODE mode);                         //rtc irq handler

uint8_t monthLength(uint8_t lpyr, uint8_t mon);                 //get month length
uint32_t mkTimtoStamp(UTCTimeStruct *utcTime);                  //make utcTime to second counts

#ifdef __cplusplus
}
#endif


#endif
