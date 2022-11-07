#include "decideEspState.h"

float EspState::readTemp()
{
  tempValue = bme.readTemperature();
  return tempValue;
}

void EspState::initialiseBme()
{
  status = bme.begin(0x76);  
    
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
}

int EspState::newState()
{
  if(tempValue < 0 and tempValue > -20)
  {
    stateVariable = 1; // ESP in too cold state, temperature regulation needed
  }

  else if(tempValue > 6 and tempValue < 40)
  {
    stateVariable = 2; // ESP in too warm state, temperature regulation needed
  }

  else
  {
    stateVariable = 0; // ESP in idle state, no temperature regulation needed
  }
  return stateVariable;
}
