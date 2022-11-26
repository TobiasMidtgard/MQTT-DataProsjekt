#pragma once

#define timer_h

// Class declaration of the Timer-Class.
class Timer
{
public:
  // Methods:

  /**
   * @brief Starts a timer.
   *
   * @param timeout starts a timer for how often measurments or publishment should be made.
   * @return
   */
  
  void start(int timer);

  /**
   * @brief Checks if the timer has run out.
   *
   * @return If the timer has expired.
   */
  bool hasExpired();


private:
  unsigned long timeout;
};
