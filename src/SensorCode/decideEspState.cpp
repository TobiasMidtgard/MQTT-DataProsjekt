
/*--------------------------------------------------------*/
/*                                                        */
/*      This program is apart of the IELET2001 Project    */
/*                                                        */
/*  The code is written by:                               */
/*   -  Adam                                              */
/*   -                                                    */
/*--------------------------------------------------------*/
/*                                                        */
/* This code check which state the program should be in   */
/* depending on the temperature the sensor has messured.  */
/*--------------------------------------------------------*/

// Calls for the program`s headerfile and the common file
#include <Arduino.h>
#include "decideEspState.h"

// Function that reads the temperature from the bme sensor
float EspState::readTemp()
{
  tempValue = bme.readTemperature();
  return tempValue;
}

// a function to initialise the bme sensor and prints information to the seriial monitor.
void EspState::initialiseBme()
{
  status = bme.begin(0x76);

  if (!status)
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x");
    Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1)
      delay(10);
  }
}

// Changes the state of the program, depending on the temperature of the sensor.
int EspState::newState()
{
  // ESP in too cold state
  if (tempValue < 0 and tempValue > -20)
  {
    stateVariable = 1;
  }

  // ESP in too warm state
  else if (tempValue > 6 and tempValue < 40)
  {
    stateVariable = 2;
  }

  // ESP in alright state
  else
  {
    stateVariable = 0;
  }
  return stateVariable;
}
