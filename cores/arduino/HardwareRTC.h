#ifndef _GD_HARDWARERTC_H_
#define _GD_HARDWARERTC_H_

#include "rtc.h"
extern class HWRTC rtc;
typedef void(*RTCCallback_t)(void);

class HWRTC
{
    public:
        HWRTC(void);                                                  //RTC construct
        void setUTCTime(UTCTimeStruct *utcTime);                      //set UTC time
        void getUTCTime(UTCTimeStruct *utcTime);                      //get UTC time from base time
        void setSecTime(uint32_t secTime);                            //set second time from base time
        uint32_t getSecTime(void);                                    //get second time from base time
        void setAlarmTime(uint32_t offset, ALARM_OFFSET_FORMAT mode); //set alarm clock time base time
        void attachInterrupt(RTCCallback_t callback, INT_MODE mode);  //attach RTC interrupt
        void detachInterrupt(INT_MODE mode);                          //detach RTC interrupt
        void interruptHandler(INT_MODE mode);
    private:
        UTCTimeStruct UTCTime;//time base
        RTCCallback_t callback[3] = {0};
};

#endif
