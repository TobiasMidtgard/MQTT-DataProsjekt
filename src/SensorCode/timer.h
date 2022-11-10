#pragma once

#define timer_h

#include "common.h"

// Class declaration of the Timer-Class.
class Timer
{
  public:
    bool bmeHasExpired();
    bool publishHasExpired();
    bool alarmTimerExpired();
    void bmeStart(int timeout);
    void publishStart(int timeout);
    void alarmStart(int timeout);
    Timer();
   private:
    unsigned long bmeTimeout;
    unsigned long publishTimeout;
    unsigned long alarmTimeout;
};
