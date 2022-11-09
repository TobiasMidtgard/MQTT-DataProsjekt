#pragma once

#define timer_h

#include "common.h"

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
