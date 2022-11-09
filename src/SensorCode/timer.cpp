
/*--------------------------------------------------------*/
/*                                                        */
/*      This program is apart of the IELET2001 Project    */
/*                                                        */
/*  The code is written by:                               */
/*   -  Adam                                              */
/*   -                                                    */
/*--------------------------------------------------------*/
/*                                                        */
/* This code check which state the program should be in   */
/* depending on the temperature the sensor has messured.  */
/*--------------------------------------------------------*/

#include "timer.h"
#include "common.h"

// Timer-Classfunction to start a timer.
void Timer::start(int timeout)
{
  nextTimeout = millis() + timeout;
}

// Timer-Classfunction to check if a started timer is expired.
bool Timer::hasExpired()
{
  bool timerExpired = (millis() > nextTimeout);
  return timerExpired;
}

// Declaring private variable for the Timer-class
Timer::Timer()
{
  unsigned long nextTimeout = 0;
}
