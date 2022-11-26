
/*--------------------------------------------------------*/
/*                                                        */
/*      This program is apart of the IELET2001 Project    */
/*                                                        */
/*  The code is written by:                               */
/*   -  Adam                                              */
/*   -                                                    */
/*--------------------------------------------------------*/
/*                                                        */
/* This is the headerfile of "decideEspState.cpp. It      */
/* contains variables and calls for relevant libraries    */
/*--------------------------------------------------------*/

// Prevents multiple #include for the same library
#pragma once

// Defines the name of the file
#define decideEspState_h

// Includes nesccessary libraries
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "UbidotsEsp32Mqtt.h"

// Class declaration of ESP32 states
class EspState
{
  // Public variables
public:
  // Methods:

  /**
   * @brief Reads the temperature from the BME sensor
   *
   * @return Current temperature
   */
  int readTemp(int totalMeasurements);

  /**
   * @brief Reads the humidity from the BME sensor
   *
   * @return Current humidity
   */
  float readHumidity(int totalMeasurements);

  /**
   * @brief Changes the state of the alarm
   *
   * @return Alarm state
   */
  int newState();

  /**
   * @brief Sets up the sensor and prints information about the connection to the serial port.
   *
   */
  void initialiseBme();

  // Renames the library class to bme
  Adafruit_BME280 bme;

  EspState();

  // Private variables
private:
  // Temp
  float tempValue;
  int tempAverage;
  float tempSum;
  // Humidity
  float humidityValue;
  float humiditySum;
  float humidityAverage;
  // System state
  int stateVariable;
  unsigned status;
};