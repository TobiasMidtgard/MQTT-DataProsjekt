
/*--------------------------------------------------------*/
/*                                                        */
/*      This program is apart of the IELET2001 Project    */
/*                                                        */
/*  The code is written by:                               */
/*   -  Adam                                              */
/*   -                                                    */
/*--------------------------------------------------------*/
/*                                                        */
/* This program keeps the time and chekcs if    */
/*   */
/*--------------------------------------------------------*/

#include "timer.h"

// Timer-Classfunction to start a timer.
void Timer::bmeStart(int timeout)
 {
  bmeTimeout = millis() + timeout;
 }

void Timer::publishStart(int timeout)
{
  publishTimeout = millis() + timeout;
}

void Timer::alarmStart(int timeout)
{
  alarmTimeout = millis() + timeout;
}

// Timer-Classfunction to check if a started timer is expired.
bool Timer::bmeHasExpired()
{
  bool timerExpired = (millis() > bmeTimeout);
  return timerExpired;
}

bool Timer::publishHasExpired()
{
  bool timerExpired = (millis() > publishTimeout);
  return timerExpired;
}

bool Timer::alarmTimerExpired()
{
  bool timerExpired = (millis() > alarmTimeout);
  return timerExpired;
}

// Declaring private variable for the Timer-class
Timer::Timer()
{
  unsigned long bmeTimeout = 0;
  unsigned long publishTimeout = 0;
  unsigned long alarmTimeout = 0;
}
