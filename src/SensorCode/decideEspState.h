
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

// Include all of the libraries
#include "common.h"

// Class declaration of ESP32 states
class EspState
{
  // Public variables
public:
  float readTemp(int totalTempMeasurements);
  float readHumidity(int totalMeasurements);
  int newState();
  void initialiseBme();
  Adafruit_BME280 bme;

  // Private variables
  // Temp
  float tempValue = 0;
  float tempAverage = 0;
  float tempSum = 0;
  // Humidity
  float humidityValue = 0;
  float humiditySum = 0;
  float humidityAverage = 0;
  // System state
  int stateVariable = 0;
  unsigned status;
};
