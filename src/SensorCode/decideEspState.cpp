
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
float EspState::readTemp(int totalMeasurements)
{
  tempValue = 0;
  tempSum = 0;
  for (int i = 0; i<totalMeasurements; i++)
  {
   tempValue = bme.readTemperature();
   tempSum += tempValue;
  }
  tempAverage = round(tempSum/totalMeasurements);
  return tempAverage;
}

float EspState::readHumidity(int totalMeasurements)
{
  humidityValue = 0;
  humiditySum = 0;
  for (int i = 0; i<totalMeasurements; i++)
  {
    humidityValue = bme.readHumidity();
    humiditySum += humidityValue;
  }
  humidityAverage = humiditySum/totalMeasurements;
  return humidityAverage;
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
  if(tempAverage < 0)
  {
    stateVariable = 1;
  }

  // ESP in too warm state
  else if(tempAverage > 12)
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

// Declaring private variable for the ESP-class
EspState::EspState()
{
  // Temp
    float tempValue = 0;
    int tempAverage = 0;
    float tempSum = 0;
    // Humidity
    float humidityValue = 0;
    float humiditySum = 0;
    float humidityAverage = 0;
    // System state
    int stateVariable = 0;
    unsigned status;
}
