Arduino library to schedule RTC alarms at specific times or repeating intervals

Alarm Interupt Scheduler for DS3132/31 RTC

Installation
--------------------------------------------------------------------------------

To install this library, just place this entire folder as a subfolder in your
Arduino/lib/targets/libraries folder.

Example Use
--------------------------------------------------------------------------------

#include <AlarmSchedule.h>

AlarmScheduleRTC3232 als;

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
ALARM_TYPES_t alarmType = ALM1_MATCH_SECONDS;

// Set Alarm at specific times
uint8_t alarmMatchSec = 0;  // 00-59
uint8_t alarmMatchMin = 0;  // 00-59
uint8_t alarmMatchHour = 0; // 00-23
uint8_t alarmMatchDay = 0;  // 1-7

// Or set repeat Alarm (ALM1_MATCH_SECONDS | ALM1_MATCH_MINUTES) in (secs|mins)
uint8_t alarmInterval = 20;

als.setAlarm( alarmType, alarmMatchSec, alarmMatchMin, alarmMatchHour, alarmMatchDay, alarmInterval);

als.setRepeatAlarm(alarmType, alarmInterval);
