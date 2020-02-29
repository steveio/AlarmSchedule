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
* This library is licensed under Creative Commons Attribution-ShareAlike 3.0
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

    switch(alarmType)
    {
      case ALM1_EVERY_SECOND :
        setAlarmInterupt(ALM1_EVERY_SECOND , 0, 0, 0, 0);
        break;

      case ALM1_MATCH_SECONDS :
        if (alarmMatchSec >= 1)
        {
          nextIntervalSec = alarmMatchSec;
        } else {
          //Serial.println("Secs: ");
          //Serial.println(second());
          i = second() + alarmInterval;
          nextIntervalSec = (i < 60) ? (i) : 0 + (i - 60) ;
          //Serial.println("NextInterval: ");
          //Serial.println(nextIntervalSec);

        }
        break;

      case ALM1_MATCH_MINUTES :
        if (alarmMatchMin >= 1)
        {
          nextIntervalSec = alarmMatchSec;
          nextIntervalMin = alarmMatchMin;
        } else {
          i = (minute() * 60) + alarmInterval;
          nextIntervalMin = ((i / 60) < 60) ? (i) : 0 + (i - 60) ;
        }

      case ALM1_MATCH_HOURS :
        if (alarmMatchHour >= 1)
        {
          nextIntervalSec = alarmMatchSec;
          nextIntervalMin = alarmMatchMin;
          nextIntervalHour = alarmMatchHour;
        }
        break;

      case ALM1_MATCH_DAY :
        if (alarmMatchDay >= 1)
        {
          nextIntervalSec = alarmMatchSec;
          nextIntervalMin = alarmMatchMin;
          nextIntervalHour = alarmMatchHour;
          nextIntervalDay = alarmMatchDay;
        }
        break;

      case ALM1_MATCH_DATE :
        // @todo
        break;
    }

    setAlarmInterupt(alarmType , nextIntervalSec, nextIntervalMin, nextIntervalHour, nextIntervalDay);

}

/**
 * Schedule a repeat alarm at alarmInterval (secs|mins)
 * @param ALARM_TYPES_t ALM1_MATCH_SECONDS | ALM1_MATCH_MINUTES
 * @param int (secs|mins) interval to next alarm
 */
void AlarmSchedule::setRepeatAlarm(ALARM_TYPES_t alarmType, uint8_t alarmInterval)
{
    Serial.println("setRepeatAlarm()");

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
  rtc.setAlarm(alarmType , seconds, minutes, hours,daydate);
}

/**
* attachInterupt() does not appear to work within class method (static | non static)
* Interupt fires repeatedly, perhaps Interupt lock? gets corrupted when method returns
* Tried with typedef void (*isrFunc_t)();  declared in global scope and void (*isrFunc)()
*/
void AlarmScheduleRTC3232::attachISRInterrupt(uint8_t interruptNum, isrFunc_t isrFunc, int mode)
{
  Serial.println("attachISRInterrupt()");
  Serial.println(interruptNum);
  Serial.println(mode);

  //pinMode( interruptNum, INPUT_PULLUP );
  //attachInterrupt(digitalPinToInterrupt(interruptNum), (*isrFunc), mode);
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
