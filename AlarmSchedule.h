/**
*  AlarmSchedule.h
*
*  Schedule Alarms at specific time or interval
*
*  Alarm Interupt Scheduler for DS3132/31 RTC
*
* Version: 1.01
* Date: 28-02-2020
*
* This library is licensed under Creative Commons Attribution-ShareAlike 3.0
*/

#ifndef AlarmSchedule_h
#define AlarmSchedule_h

#include "Arduino.h"

typedef void (*isrFunc_t)();



class AlarmSchedule
{

  public:
    void setAlarm(ALARM_TYPES_t alarmType, uint8_t alarmMatchSec, uint8_t alarmMatchMin, uint8_t alarmMatchHour, uint8_t alarmMatchDay,  uint8_t alarmInterval);
    void setRepeatAlarm(ALARM_TYPES_t alarmType, uint8_t alarmInterval);
    virtual void setAlarmInterupt(ALARM_TYPES_t alarmType, byte seconds, byte minutes, byte hours, byte daydate);
    virtual uint8_t minute();
    virtual uint8_t second();
  private:

  protected:

};

class AlarmScheduleRTC3232 : public AlarmSchedule
{
  public:
    void setAlarmInterupt(ALARM_TYPES_t alarmType, byte seconds, byte minutes, byte hours, byte daydate);
    static void attachISRInterrupt(uint8_t interruptNum, isrFunc_t isrFunc, int mode);
  private:

  protected:
    DS3232RTC rtc;
    uint8_t rtcInterruptPin = 2;
    uint8_t minute();
    uint8_t second();
};


#endif
