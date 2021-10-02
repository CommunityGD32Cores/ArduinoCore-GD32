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

#include "rtc.h"
#include <time.h>

/*
  In the definitions for GD32F30X_{H,X}D, this is 'Alarm', but for the
  CL variant, it’s 'ALARM'. The core uses the upper case version. I
  have no good solution other than getting the official firmware
  library changed. -bjc (2021-Aug-20)
*/
#if defined(GD32F30X_HD) || defined (GD32F30X_XD)
#define RTC_ALARM_IRQn RTC_Alarm_IRQn
#endif

#if defined(GD32F3x0)
/* 
 * wrapper functions for microcontrollers that have an RTC
 * with an asychronous and synchronous prescaler (A,S)
*/
void rtc_prescaler_set(uint32_t prescaler) {
    //ignore incoming prescalar for now -- prescale to 1 per sec
    (void) prescaler;
    ErrStatus error_status = ERROR;
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* values good for LXTAL clock source (32kHz) */
    uint16_t prescaler_s = 0xFF;
    uint16_t prescaler_a = 0x7F;

    error_status = rtc_init_mode_enter();
    RTC_PSC = (uint32_t)(PSC_FACTOR_A(prescaler_a)| \
                            PSC_FACTOR_S(prescaler_s));
    rtc_init_mode_exit();
    error_status = rtc_register_sync_wait();
    RTC_WPK = RTC_LOCK_KEY;
}

/* get RTC counter by getting Unix time and converting to timestamp */
uint32_t rtc_counter_get() {
    UTCTimeStruct utcTime;
    rtc_getUTCTime(&utcTime);
    struct tm ts;
    ts.tm_hour = utcTime.hour;
    ts.tm_min = utcTime.minutes;
    ts.tm_sec = utcTime.seconds;
    ts.tm_year = utcTime.year;
    ts.tm_mon = utcTime.month;
    ts.tm_mday = utcTime.day;
    time_t unix_time = mktime(&ts);
    return (uint32_t) unix_time;
}
#endif

/*!
    \brief      rtc init
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_Init(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(RTC_IRQn, 2, 0);
    #if defined(GD32F30x)
    nvic_irq_enable(RTC_ALARM_IRQn, 2, 0);
    /* enable PMU and BKPI clocks */
    rcu_periph_clock_enable(RCU_BKPI);
    #endif
    rcu_periph_clock_enable(RCU_PMU);
    /* allow access to BKP domain */
    pmu_backup_write_enable();
    /* reset backup domain */
    #if defined(GD32F30x)
    bkp_deinit();
    #elif defined(GD32F3x0)
    rcu_bkp_reset_enable();
    rcu_bkp_reset_disable();
    #endif
    /* enable LXTAL */
    rcu_osci_on(RCU_LXTAL);
    /* wait till LXTAL is ready */
    rcu_osci_stab_wait(RCU_LXTAL);
    /* select RCU_LXTAL as RTC clock source */
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
    /* enable RTC Clock */
    rcu_periph_clock_enable(RCU_RTC);
    /* wait for RTC registers synchronization */
    rtc_register_sync_wait();

#if defined(GD32F30x)
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
#endif
    /* set RTC prescaler: set RTC period to 1s */
    rtc_prescaler_set(32767);
#if defined(GD32F30x)
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
#endif
}

/*!
    \brief      rtc set UTC time
    \param[in]  utcTime: point to UTC format time
    \param[out] none
    \retval     none
*/
void rtc_setUTCTime(UTCTimeStruct *utcTime)
{
#if defined(GD32F30x)
    uint32_t secTime = mkTimtoStamp(utcTime) - SECONDS_PER_HOUR * 8;
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    /* change the current time */
    rtc_counter_set(secTime);
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
#elif defined(GD32F3x0)
    rtc_parameter_struct curr_date;
    rtc_current_time_get(&curr_date);
    curr_date.rtc_hour = utcTime->hour;
    curr_date.rtc_minute = utcTime->minutes;
    curr_date.rtc_second = utcTime->seconds;
    curr_date.rtc_year = utcTime->year - 2000;
    curr_date.rtc_month = utcTime->month;
    curr_date.rtc_date = utcTime->day;
    rtc_init(&curr_date);
#endif
}

/*!
    \brief      rtc get UTC time
    \param[in]  utcTime: point to UTC format time
    \param[out] none
    \retval     none
*/
void rtc_getUTCTime(UTCTimeStruct *utcTime)
{
#if defined(GD32F30x)
    uint32_t timestamp = rtc_getSecTime() + SECONDS_PER_HOUR * 8;
    uint32_t day = timestamp % SECONDS_PER_DAY;  //seconds less then one day

    utcTime->hour    = day / 3600;
    utcTime->minutes = (day % 3600) / 60;
    utcTime->seconds = day % 60;

    {
        uint32_t numDays = timestamp / SECONDS_PER_DAY;

        while(numDays >= YearLength(utcTime->year)) {
            numDays -= YearLength(utcTime->year);
            utcTime->year++;
        }

        while(numDays >= monthLength(IsLeapYear(utcTime->year), utcTime->month)) {
            numDays -= monthLength(IsLeapYear(utcTime->year), utcTime->month);
            utcTime->month++;
        }
        utcTime->day += numDays;
    }
#elif defined(GD32F3x0)
    rtc_parameter_struct curr_date;
    rtc_current_time_get(&curr_date);
    utcTime->hour = curr_date.rtc_hour;
    utcTime->minutes = curr_date.rtc_minute;
    utcTime->seconds = curr_date.rtc_second;
    utcTime->year = 2000 + curr_date.rtc_year;
    utcTime->month = curr_date.rtc_month;
    utcTime->day = curr_date.rtc_date;
#endif
}

/*!
    \brief      rtc set second time
    \param[in]  secTime: second counts
    \param[out] none
    \retval     none
*/
void rtc_setSecTime(uint32_t secTime)
{
#if defined(GD32F30x)
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    /* change the current time */
    rtc_counter_set(secTime);
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
#elif defined(GD32F3x0)
    /* convert total seconds into date and set as UTC time */
    time_t t = (time_t) secTime;
    struct tm ts = *localtime(&t);
    UTCTimeStruct utcTime;
    utcTime.hour = ts.tm_hour;
    utcTime.minutes = ts.tm_min;
    utcTime.seconds = ts.tm_sec;
    utcTime.year = ts.tm_year;
    utcTime.month = ts.tm_mon;
    utcTime.day = ts.tm_mday;
    rtc_setUTCTime(&utcTime);
#endif
}

/*!
    \brief      rtc get second time
    \param[in]  none
    \param[out] none
    \retval     second counts
*/
uint32_t rtc_getSecTime(void)
{
    return rtc_counter_get();
}

/*!
    \brief      rtc set alarm time
    \param[in]  alarmTime: alarm time
    \param[out] none
    \retval     second counts
*/
void rtc_setAlarmTime(uint32_t alarmTime)
{
#if defined(GD32F30x)
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    /* change the current time */
    rtc_alarm_config(alarmTime);
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
#elif defined(GD32F3x0)
    rtc_alarm_struct rtc_alarm_time;
    time_t t = (time_t) alarmTime;
    struct tm ts = *localtime(&t);
    rtc_alarm_time.rtc_alarm_mask = RTC_ALARM_DATE_MASK|RTC_ALARM_HOUR_MASK|RTC_ALARM_MINUTE_MASK|RTC_ALARM_SECOND_MASK;
    rtc_alarm_time.rtc_weekday_or_date = RTC_ALARM_DATE_SELECTED;
    rtc_alarm_time.rtc_alarm_day = ts.tm_mday;
    if(ts.tm_hour <= 12) {
        rtc_alarm_time.rtc_am_pm = RTC_AM;
        rtc_alarm_time.rtc_alarm_hour = ts.tm_hour;
    } else {
        rtc_alarm_time.rtc_am_pm = RTC_PM;
        rtc_alarm_time.rtc_alarm_hour = 12 - ts.tm_hour;
    }
    rtc_alarm_time.rtc_alarm_minute = ts.tm_min;
    rtc_alarm_time.rtc_alarm_second = ts.tm_sec;
    rtc_alarm_time.rtc_weekday_or_date = ts.tm_sec;

    rtc_alarm_config(&rtc_alarm_time);
    rtc_alarm_enable();
#endif
}

/*!
    \brief      rtc attach interrupt
    \param[in]  mode: interrupt mode
    \param[out] none
    \retval     none
*/
void rtc_attachInterrupt(INT_MODE mode)
{
    uint32_t interrupt = 0;
    switch(mode) {
#if defined(GD32F30x)
        case INT_SECOND_MODE:
            interrupt = RTC_INT_SECOND;
            break;
#endif
        case INT_ALARM_MODE:
            interrupt = RTC_INT_ALARM;
            exti_init(EXTI_17, EXTI_INTERRUPT, EXTI_TRIG_RISING);
            break;
#if defined(GD32F30x)
        case INT_OVERFLOW_MODE:
            interrupt = RTC_INT_OVERFLOW;
            break;
#endif
    }
    rtc_interrupt_enable(interrupt);
}

/*!
    \brief      rtc detach interrupt
    \param[in]  mode: interrupt mode
    \param[out] none
    \retval     none
*/
void rtc_detachInterrupt(INT_MODE mode)
{
    uint32_t interrupt = 0;
    switch(mode) {
#if defined(GD32F30x)
        case INT_SECOND_MODE:
            interrupt = RTC_INT_SECOND;
            break;
#endif
        case INT_ALARM_MODE:
            interrupt = RTC_INT_ALARM;
            break;
#if defined(GD32F30x)
        case INT_OVERFLOW_MODE:
            interrupt = RTC_INT_OVERFLOW;
            break;
#endif
    }
    rtc_interrupt_disable(interrupt);
}

/*!
    \brief      rtc irq handler
    \param[in]  mode
    \param[out] none
    \retval     none
*/
void RTC_IRQHandler(void)
{
#if defined(GD32F30x)
    if(rtc_flag_get(RTC_FLAG_SECOND) != RESET) {
        rtc_flag_clear(RTC_FLAG_SECOND);
        RTC_Handler(INT_SECOND_MODE);
    }
    if(rtc_flag_get(RTC_FLAG_OVERFLOW) != RESET) {
        rtc_flag_clear(RTC_FLAG_OVERFLOW);
        RTC_Handler(INT_OVERFLOW_MODE);
    }
#elif defined(GD32F3x0)
    if(rtc_flag_get(RTC_FLAG_ALARM0) != RESET){
        rtc_flag_clear(RTC_FLAG_ALARM0);
        exti_flag_clear(EXTI_17);
        RTC_Handler(INT_ALARM_MODE);
    } 
#endif
}

/*!
    \brief      rtc alarm irq handler
    \param[in]  mode
    \param[out] none
    \retval     none
*/
#if defined(GD32F30x)
void RTC_Alarm_IRQHandler(void)
{
    if(rtc_flag_get(RTC_FLAG_ALARM) != RESET) {
        rtc_flag_clear(RTC_FLAG_ALARM);
        exti_flag_clear(EXTI_17);
        RTC_Handler(INT_ALARM_MODE);
    }
}
#endif

/*!
    \brief      get month length
    \param[in]  lpyr: is leap year
    \param[in]  mon: month
    \param[out] none
    \retval     month lenth
*/
uint8_t monthLength(uint8_t lpyr, uint8_t mon)
{
    uint8_t days = 30;

    if(mon == 2) {  // feb
        days = (28 + lpyr);
    } else {
        if(mon > 7) {  // aug-dec
            mon--;
        }

        if(mon & 1) {
            days = 31;
        }
    }

    return (days);
}

/*!
    \brief      make utcTime to second counts
    \param[in]  alarmTime: alarm time
    \param[out] none
    \retval     second counts
*/
uint32_t mkTimtoStamp(UTCTimeStruct *utcTime)
{
    uint16_t year = utcTime->year;
    uint8_t mon = utcTime->month;
    uint8_t day = utcTime->day;
    uint32_t numDays = 0;
    uint32_t timestamp = 0;
    while(1969 != --year) {
        if(IsLeapYear(year)) {
            numDays += 366;
        } else {
            numDays += 365;
        }
    }
    while(0 != --mon) {
        numDays += monthLength(IsLeapYear(utcTime->year), mon);
    }
    numDays = numDays + day - 1;
    timestamp = numDays * SECONDS_PER_DAY + (utcTime->hour * 3600 + utcTime->minutes * 60 +
                                             utcTime->seconds);
    return timestamp;
}
