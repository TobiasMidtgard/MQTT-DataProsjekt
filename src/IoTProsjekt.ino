/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  See the LICENSE file for details.
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "UbidotsEsp32Mqtt.h"
#include "timer.h"
#include "decideEspState.h"


#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

const char *UBIDOTS_TOKEN = "BBFF-2OYFDrW6Ts35uxYLMaOyNgkfW1ZOKw";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "Adam sin iPhone";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "adam0115";      // Put here your Wi-Fi password

const char *DEVICE_LABEL = "esp32";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL_TEMP = "temperatur"; // Put here your Variable label to which data  will be published


Ubidots ubidots(UBIDOTS_TOKEN);

// System states
const int S_IDLE = 0;
const int S_TEMP_TOO_LOW = 1;
const int S_TEMP_TOO_HIGH = 2;

// Starting system state
int currentState = S_IDLE;

// Defining variable which uses header-files.
Timer myTimer;
EspState espState;

float temperature;

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
    Serial.begin(115200);
    while(!Serial);    // time to get serial running
    Serial.println(F("BME280 test"));

    //unsigned status;

    espState.initialiseBme();
    // default settings
    /*status = bme.begin(0x76);  
    
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }*/
    
    Serial.println("-- Default Test --");
    

    Serial.println();

  ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();

  
}


void loop() { 
    switch(currentState)
    {

    case S_IDLE:
    {
      if(myTimer.hasExpired())
      {
      seccureUbidotsConnection();
      temperature = espState.readTemp();
      changeStateTo(espState.newState());
      }
    }

    break;

    case S_TEMP_TOO_LOW:
    {
      if(myTimer.hasExpired())
      {
      seccureUbidotsConnection();
      temperature = espState.readTemp();
      changeStateTo(espState.newState());
      }
    }

    break;

    case S_TEMP_TOO_HIGH:
    {
      if(myTimer.hasExpired())
      {
      seccureUbidotsConnection();
      temperature = espState.readTemp();
      changeStateTo(espState.newState());
      }
    }

    break;
    
    
    }
}

// Function seccuring connection between esp32 and ubidots
void seccureUbidotsConnection()
{
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
}



void changeStateTo(int state)
{
  int stateCheck = currentState;
  Serial.print("State changed from ");
  Serial.print(currentState);
  currentState = state;
  Serial.print(" to ");
  Serial.print(state);
  
  if(stateCheck != currentState)
  {
    ubidots.add(VARIABLE_LABEL_TEMP, temperature); // Inserting variable-label and temperaturevalue to ubidots
    ubidots.publish(DEVICE_LABEL); // Inserting which device on ubidots it should be published to
    ubidots.loop();
  }
  myTimer.start(5000);
}
