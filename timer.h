#ifndef TIMER_H
#define TIMER_H
#include <arduino.h>

// Class declaration of the Timer-Class.
class Timer
{
  public:
    bool hasExpired();
    void start(int timeout);
    Timer();
   private:
    unsigned long nextTimeout;
};

#endif
