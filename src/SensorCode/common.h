
/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  This is a common file, it contains the libraries used for the rest of the program.  */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

// Prevents multiple #include for the same library
#pragma once
// Defines a name for the file to be called upon, in this case "Common.h"
#define common_h

// Inludes all of the necessary libraries for the program
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "UbidotsEsp32Mqtt.h"

// Defines which header files should be included
#include "timer.h"
#include "decideEspState.h"
