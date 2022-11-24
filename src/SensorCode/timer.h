#pragma once

#define timer_h

// Class declaration of the Timer-Class.
class Timer
{
public:
  // Methods:

  /**
   * @brief Starts a timer for the BME sensor for how often the sensor should take measurements.
   *
   * @param timeout starts a timer for how often measurments should be made.
   * @return
   *
   *
   * @brief Starts a timer that controls how often data should be published to main server.
   *
   * @param timeout starts a timer for how often data should be published.
   * @return
   *
   *
   * @brief A timer after the alarm that cotrols when the microcontroller can send data.
   *
   * @param timeout starts a timer for how often data should be published.
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
