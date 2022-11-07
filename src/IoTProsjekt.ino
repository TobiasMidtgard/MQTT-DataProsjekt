#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "UbidotsEsp32Mqtt.h"
#include "timer.h"
#include "decideEspState.h"

#define SEALEVELPRESSURE_HPA (1013.25)

const char *UBIDOTS_TOKEN = "BBFF-2OYFDrW6Ts35uxYLMaOyNgkfW1ZOKw";  // Ubidots TOKEN
const char *WIFI_SSID = "Adam sin iPhone";      // Wi-Fi SSID
const char *WIFI_PASS = "adam0115";      // Wi-Fi password
const char *DEVICE_LABEL = "esp32";   // Device label to which data will be published
const char *VARIABLE_LABEL_TEMP = "temperatur"; // Variable label to which data  will be published

Ubidots ubidots(UBIDOTS_TOKEN); 

// System states
const int S_IDLE = 0;
const int S_TEMP_TOO_LOW = 1;
const int S_TEMP_TOO_HIGH = 2;

// Starting system state
int currentState = S_IDLE;

// Defining variables which uses header-files.
Timer myTimer;
EspState espState;

// Global variable containing temperature measurement
float temperature;

void setup() 
{
  Serial.begin(115200);
  while(!Serial);    // time to get serial running
  Serial.println(F("BME280 test"));

  espState.initialiseBme(); // Connecting bme-sensor to esp32
    
  Serial.println("-- Default Test --");
  Serial.println();

  ubidots.setDebug(true); 
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();

  temperature = espState.readTemp();
  ubidots.add(VARIABLE_LABEL_TEMP, temperature); // Inserting variable-label and temperaturevalue to ubidots
  ubidots.publish(DEVICE_LABEL); // Inserting which device on ubidots it should be published to
  ubidots.loop();
  
}


void loop()
{ 
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
  Serial.println();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  if(stateCheck != currentState) // Push to ubidots when esp in a new state
  {
    ubidots.add(VARIABLE_LABEL_TEMP, temperature); // Inserting variable-label and temperaturevalue to ubidots
    ubidots.publish(DEVICE_LABEL); // Inserting which device on ubidots it should be published to
    ubidots.loop();
  }
  myTimer.start(5000); // Timer to reduce bme total measurements
}













