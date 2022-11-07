#ifndef ESP_STATE_H
#define ESP_STATE_H

#include <arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Class declaration of ESP32 states

class EspState
{
  public:
    float readTemp();
    int newState();
    void initialiseBme();
    Adafruit_BME280 bme;
    
  private:
    int stateVariable = 0;
    float tempValue = 0;
    unsigned status;
};

#endif
