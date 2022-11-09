
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
  bmeNextTimeout = millis() + timeout;
 }

void Timer::publishStart(int timeout)
{
  publishNextTimeout = millis() + timeout;
}

// Timer-Classfunction to check if a started timer is expired.
bool Timer::bmeHasExpired()
{
  bool timerExpired = (millis() > bmeNextTimeout);
  return timerExpired;
}

bool Timer::publishHasExpired()
{
  bool timerExpired = (millis() > publishNextTimeout);
  return timerExpired;
}

// Declaring private variable for the Timer-class
Timer::Timer()
{
  unsigned long bmeNextTimeout = 0;
  unsigned long publishNextTimeout = 0;
}
