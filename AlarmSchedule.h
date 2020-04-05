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

/*
 * Alarm Schedule Constants:
 *
ALM1_EVERY_SECOND = 0x0F,      // once a second
ALM1_MATCH_SECONDS = 0x0E,     // when seconds match
ALM1_MATCH_MINUTES = 0x0C,     // match minutes *and* seconds
ALM1_MATCH_HOURS = 0x08,       // match hours *and* minutes, seconds
ALM1_MATCH_DATE = 0x00,        // match date *and* hours, minutes, seconds
ALM1_MATCH_DAY = 0x10,         // match day *and* hours, minutes, seconds
ALM2_EVERY_MINUTE = 0x8E,
ALM2_MATCH_MINUTES = 0x8C,     // match minutes
ALM2_MATCH_HOURS = 0x88,       // match hours *and* minutes
ALM2_MATCH_DATE = 0x80,        // match date *and* hours, minutes
ALM2_MATCH_DAY = 0x90,         // match day *and* hours, minutes
*/



class AlarmSchedule
{

  public:
    void setAlarm(ALARM_TYPES_t alarmType, uint8_t alarmMatchSec, uint8_t alarmMatchMin, uint8_t alarmMatchHour, uint8_t alarmMatchDay,  uint8_t alarmInterval);
    void setRepeatAlarm(ALARM_TYPES_t alarmType, uint8_t alarmInterval);
    virtual void setAlarmInterupt(ALARM_TYPES_t alarmType, byte seconds, byte minutes, byte hours, byte daydate);

    uint8_t alarmType()          { return _alarmType; }
    uint8_t alarmMatchSec()      { return _alarmMatchSec; }
    uint8_t alarmMatchMin()      { return _alarmMatchMin; }
    uint8_t alarmMatchHour()     { return _alarmMatchHour; }
    uint8_t alarmMatchDay()      { return _alarmMatchDay; }
    uint8_t alarmInterval()      { return _alarmInterval; }

    virtual uint8_t minute();
    virtual uint8_t second();

  private:
    ALARM_TYPES_t _alarmType;
    uint8_t _alarmMatchSec = 0;  // 00-59
    uint8_t _alarmMatchMin = 0;  // 00-59
    uint8_t _alarmMatchHour = 0; // 00-23
    uint8_t _alarmMatchDay = 0;  // 1-7

    // Set repeat Alarm (ALM1_MATCH_SECONDS | ALM1_MATCH_MINUTES) every n secs/mins
    uint8_t _alarmInterval = 0;

  protected:
};

class AlarmScheduleRTC3232 : public AlarmSchedule
{
  public:
    void setAlarmInterupt(ALARM_TYPES_t alarmType, byte seconds, byte minutes, byte hours, byte daydate);
    static void attachISRInterrupt(uint8_t interruptNum, isrFunc_t isrFunc, int mode);
  private:
    static void irqHandler0();
  protected:
    DS3232RTC rtc;
    uint8_t rtcInterruptPin = 2;
    uint8_t minute();
    uint8_t second();
};


#endif
