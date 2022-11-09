#pragma once

#define timer_h

#include "common.h"

// Class declaration of the Timer-Class.
class Timer
{
  public:
    bool bmeHasExpired();
    bool publishHasExpired();
    void bmeStart(int timeout);
    void publishStart(int timeout);
    Timer();
   private:
    unsigned long bmeNextTimeout;
    unsigned long publishNextTimeout;
};
