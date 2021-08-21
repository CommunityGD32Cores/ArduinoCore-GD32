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

/*
  In the definitions for GD32F30X_{H,X}D, this is 'Alarm', but for the
  CL variant, it’s 'ALARM'. The core uses the upper case version. I
  have no good solution other than getting the official firmware
  library changed. -bjc (2021-Aug-20)
*/
#if defined(GD32F30X_HD) || defined (GD32F30X_XD)
#define RTC_ALARM_IRQn RTC_Alarm_IRQn
#endif

/*!
    \brief      rtc init
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_init(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(RTC_IRQn, 2, 0);
    nvic_irq_enable(RTC_ALARM_IRQn, 2, 0);
    /* enable PMU and BKPI clocks */
    rcu_periph_clock_enable(RCU_BKPI);
    rcu_periph_clock_enable(RCU_PMU);
    /* allow access to BKP domain */
    pmu_backup_write_enable();
    /* reset backup domain */
    bkp_deinit();
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

    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    /* set RTC prescaler: set RTC period to 1s */
    rtc_prescaler_set(32767);
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
}

/*!
    \brief      rtc set UTC time
    \param[in]  utcTime: point to UTC format time
    \param[out] none
    \retval     none
*/
void rtc_setUTCTime(UTCTimeStruct *utcTime)
{
    uint32_t secTime = mkTimtoStamp(utcTime) - SECONDS_PER_HOUR * 8;
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    /* change the current time */
    rtc_counter_set(secTime);
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
}

/*!
    \brief      rtc get UTC time
    \param[in]  utcTime: point to UTC format time
    \param[out] none
    \retval     none
*/
void rtc_getUTCTime(UTCTimeStruct *utcTime)
{
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
}

/*!
    \brief      rtc set second time
    \param[in]  secTime: second counts
    \param[out] none
    \retval     none
*/
void rtc_setSecTime(uint32_t secTime)
{
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    /* change the current time */
    rtc_counter_set(secTime);
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();

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
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    /* change the current time */
    rtc_alarm_config(alarmTime);
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
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
        case INT_SECOND_MODE:
            interrupt = RTC_INT_SECOND;
            break;
        case INT_ALARM_MODE:
            interrupt = RTC_INT_ALARM;
            exti_init(EXTI_17, EXTI_INTERRUPT, EXTI_TRIG_RISING);
            break;
        case INT_OVERFLOW_MODE:
            interrupt = RTC_INT_OVERFLOW;
            break;
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
        case INT_SECOND_MODE:
            interrupt = RTC_INT_SECOND;
            break;
        case INT_ALARM_MODE:
            interrupt = RTC_INT_ALARM;
            break;
        case INT_OVERFLOW_MODE:
            interrupt = RTC_INT_OVERFLOW;
            break;
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
    if(rtc_flag_get(RTC_FLAG_SECOND) != RESET) {
        rtc_flag_clear(RTC_FLAG_SECOND);
        RTC_Handler(INT_SECOND_MODE);
    }
    if(rtc_flag_get(RTC_FLAG_OVERFLOW) != RESET) {
        rtc_flag_clear(RTC_FLAG_OVERFLOW);
        RTC_Handler(INT_OVERFLOW_MODE);
    }
}

/*!
    \brief      rtc alarm irq handler
    \param[in]  mode
    \param[out] none
    \retval     none
*/
void RTC_Alarm_IRQHandler(void)
{
    if(rtc_flag_get(RTC_FLAG_ALARM) != RESET) {
        rtc_flag_clear(RTC_FLAG_ALARM);
        exti_flag_clear(EXTI_17);
        RTC_Handler(INT_ALARM_MODE);
    }
}

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
