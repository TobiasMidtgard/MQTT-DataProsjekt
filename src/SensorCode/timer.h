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
   */
  void bmeStart(int timeout);

  /**
   * @brief Starts a timer that controls how often data should be published to main server.
   *
   * @param timeout starts a timer for how often data should be published.
   * @return
   */
  void publishStart(int timeout);

  /**
   * @brief A timer after the alarm that cotrols when the microcontroller can send data.
   *
   * @param timeout starts a timer for how often data should be published.
   * @return
   */
  void alarmStart(int timeout);

  /**
   * @brief Checks if the timer has run out for the BME sensor.
   *
   * @return If the timer has expired.
   */
  bool bmeHasExpired();

  /**
   * @brief Checks if the publish timer has run out.
   *
   * @return If the timer has expired.
   */
  bool publishHasExpired();

  /**
   * @brief Checks if the alarm timer has run out.
   *
   * @return If the timer has expired.
   */
  bool alarmTimerExpired();

private:
  unsigned long SbmeTimeout;
  unsigned long publishTimeout;
  unsigned long alarmTimeout;
};
