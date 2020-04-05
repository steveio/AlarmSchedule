/**
*  AlarmSchedule.cpp
*
*  Schedule Alarms at specific time or interval
*
*  Alarm Interupt Scheduler for DS3132/31 RTC
*
* Version: 1.01
* Date: 28-02-2020
*
*
*/


#if defined (__AVR__)
  #include <DS3232RTC.h>
  #include <Time.h>
  #include <DateTime.h>
#elif defined (__arm__)
#else
	#error "Processor architecture is not supported."
#endif


#include "AlarmSchedule.h"


void AlarmSchedule::setAlarm(ALARM_TYPES_t alarmType, uint8_t alarmMatchSec, uint8_t alarmMatchMin, uint8_t alarmMatchHour, uint8_t alarmMatchDay, uint8_t alarmInterval)
{
    uint8_t i, nextIntervalSec, nextIntervalMin, nextIntervalHour, nextIntervalDay = 0;

    _alarmType = alarmType;
    _alarmMatchSec = alarmMatchSec;
    _alarmMatchMin = alarmMatchMin;
    _alarmMatchHour = alarmMatchHour;
    _alarmMatchDay = alarmMatchDay;
    _alarmInterval = alarmInterval;

    switch(alarmType)
    {
      case ALM1_EVERY_SECOND :
        setAlarmInterupt(ALM1_EVERY_SECOND , 0, 0, 0, 0);
        break;

      case ALM1_MATCH_SECONDS :
        if (_alarmInterval == 0)
        {
          nextIntervalSec = _alarmMatchSec;
        } else {
          i = second() + _alarmInterval;
          nextIntervalSec = (i < 60) ? (i) : 0 + (i - 60) ;
        }
        break;

      case ALM1_MATCH_MINUTES :
        if (_alarmInterval == 0)
        {
          nextIntervalSec = _alarmMatchSec;
          nextIntervalMin = _alarmMatchMin;
        } else {
          i = (minute() * 60) + _alarmInterval;
          nextIntervalMin = ((i / 60) < 60) ? (i) : 0 + (i - 60) ;
        }

      case ALM1_MATCH_HOURS :
        if (_alarmMatchHour >= 1)
        {
          nextIntervalSec = _alarmMatchSec;
          nextIntervalMin = _alarmMatchMin;
          nextIntervalHour = _alarmMatchHour;
        }
        break;

      case ALM1_MATCH_DAY :
        if (_alarmMatchDay >= 1)
        {
          nextIntervalSec = _alarmMatchSec;
          nextIntervalMin = _alarmMatchMin;
          nextIntervalHour = _alarmMatchHour;
          nextIntervalDay = _alarmMatchDay;
        }
        break;

      case ALM1_MATCH_DATE :
        // @todo
        break;
    }

    setAlarmInterupt(_alarmType , nextIntervalSec, nextIntervalMin, nextIntervalHour, nextIntervalDay);

}

/**
 * Schedule a repeat alarm at alarmInterval (secs|mins)
 * @param ALARM_TYPES_t ALM1_MATCH_SECONDS | ALM1_MATCH_MINUTES
 * @param int (secs|mins) interval to next alarm
 */
void AlarmSchedule::setRepeatAlarm(ALARM_TYPES_t alarmType, uint8_t alarmInterval)
{

    uint8_t min, sec, hour, day, i = 0;

    switch(alarmType)
    {
      case ALM1_MATCH_SECONDS :
        i = second() + alarmInterval;
        sec = (i < 60) ? (i) : 0 + (i - 60) ;
        break;

      case ALM1_MATCH_MINUTES :
        i = (minute() * 60) + alarmInterval;
        min = ((i / 60) < 60) ? (i) : 0 + (i - 60) ;
        break;
    }

    setAlarmInterupt(alarmType , sec, min, hour, day);

}

/**
 * Set DS3132/3232 Alarm Interupt
 *
 * @param byte seconds 00-59
 * @param byte minutes 00-59
 * @param byte hours 1-12 + AM/PM | 00-23
 * @param byte daydate 1-7
 */
void AlarmScheduleRTC3232::setAlarmInterupt(ALARM_TYPES_t alarmType, byte seconds, byte minutes, byte hours, byte daydate)
{
  rtc.squareWave(SQWAVE_NONE);                  // Turns OFF 1Hz Frequency mode on RTC pin SQW
  rtc.alarm( ALARM_1 );
  rtc.alarmInterrupt(ALARM_2, false);
  rtc.alarmInterrupt(ALARM_1, true);            // Turns on Alarm_1
  rtc.setAlarm(alarmType, seconds, minutes, hours, daydate);
}

/**
* attachInterupt() - @toso
*/
void AlarmScheduleRTC3232::attachISRInterrupt(uint8_t interruptNum, isrFunc_t isrFunc, int mode)
{
  Serial.println("attachISRInterrupt()");
  Serial.println(interruptNum);
  Serial.println(mode);
  Serial.println(digitalPinToInterrupt(interruptNum));

  //pinMode( interruptNum, INPUT_PULLUP );
  // attempt to call function in global scope
  //attachInterrupt(digitalPinToInterrupt(interruptNum), (*isrFunc), mode);

  // define IRQ handler as class method
  //attachInterrupt(digitalPinToInterrupt(interruptNum), irqHandler0, mode);
}

uint8_t AlarmScheduleRTC3232::minute()
{
  DateTime dt = rtc.get();
  return dt.minute();
}

uint8_t AlarmScheduleRTC3232::second()
{
  DateTime dt = rtc.get();
  return dt.second();
}

void AlarmScheduleRTC3232::irqHandler0()
{
  Serial.println("irqHandler0");
}
