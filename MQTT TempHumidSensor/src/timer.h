/*--------------------------------------------------------*/
/*                                                        */
/*      This program is apart of the IELET2001 Project    */
/*                                                        */
/*  The code is written by:                               */
/*   -  Adam                                              */
/*   -                                                    */
/*--------------------------------------------------------*/
/*                                                        */
/* This program creates the timer-class and declares      */
/*  functions and variables.                              */
/*--------------------------------------------------------*/

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

  Timer();

private:
  unsigned long timeout;
};
