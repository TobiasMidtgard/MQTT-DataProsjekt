/*--------------------------------------------------------*/
/*                                                        */
/*      This program is apart of the IELET2001 Project    */
/*                                                        */
/*  The code is written by:                               */
/*   -  Adam                                              */
/*   -                                                    */
/*--------------------------------------------------------*/
/*                                                        */
/* This program creates the timer-class functions meaning.*/
/*   */
/*--------------------------------------------------------*/

#include <Arduino.h>
#include "timer.h"

// Timer-Classfunction to start a timer.
void Timer::start(int timer)
{
  timeout = millis() + timer;
}

// Timer-Classfunction to check if a started timer is expired.
bool Timer::hasExpired()
{
  bool timerExpired = (millis() > timeout);
  return timerExpired;
}

// Declaring private variable for the Timer-class
Timer::Timer()
{
  unsigned long timeout = 0;
}
