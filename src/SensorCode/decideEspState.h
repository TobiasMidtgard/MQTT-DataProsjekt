
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
  float readTemp();
  int newState();
  void initialiseBme();
  Adafruit_BME280 bme;

  // Private variables
private:
  int stateVariable = 0;
  float tempValue = 0;
  unsigned status;
};
