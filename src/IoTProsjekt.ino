/*--------------------------------------------------------*/
/*                                                        */
/*      This program is apart of the IELET2001 Project    */
/*                                                        */
/*  The code is written by:                               */
/*   -  Adam                                              */
/*   -                                                    */
/*--------------------------------------------------------*/
/*                                                        */
/* This is the main file and is responsable for reading   */
/* the temperature, establishing a connection and sending */
/* information to ubidots.                                */
/*--------------------------------------------------------*/

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <math.h>
#include "UbidotsEsp32Mqtt.h"
#include "timer.h"
#include "decideEspState.h"


const char *UBIDOTS_TOKEN = "";  // Ubidots TOKEN
const char *WIFI_SSID = "";      // Wi-Fi SSID
const char *WIFI_PASS = "";      // Wi-Fi password
const char *DEVICE_LABEL = "esp32";   // Device label to which data will be published
const char *DEVICE_LABEL_ALARM = "esp32-alarm";
const char *VARIABLE_LABEL_TEMP = "temperatur"; // Variable label to which temperature  will be published
const char *VARIABLE_LABEL_HUMIDITY = "fuktighet"; // Variable label to which humidity will be published
const char *VARIABLE_LABEL_TEMPALARM = "temperatur-alarm";

//Declaring ubidots-token to ubidots-library
Ubidots ubidots(UBIDOTS_TOKEN); 

// System states
const int S_IDLE = 0;
const int S_TEMP_TOO_LOW = 1;
const int S_TEMP_TOO_HIGH = 2;

// Starting system state
int currentState = S_IDLE;
// Previous system state
int previousState; 

// Defining instances of classes
Timer bmeTimer;
Timer publishTimer;
Timer alarmTimer;
EspState espState;

// Global variable containing temperature and humidity measurement
float temperature;
float humidity;

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
  ubidots.setup();
  ubidots.reconnect();
  bmeTimer.start(1000); // 1 second timer for bme-sensor
  publishTimer.start(5000); // 5 seconds timer for publishment to screen  
}


void loop()
{ 
 ubidots.loop();
 int totalMeasurements = 50; // Average measurement of 50 measurements.
 switch(currentState) 
 {

  case S_IDLE:
  
    if(bmeTimer.hasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp(totalMeasurements);
      humidity = espState.readHumidity(totalMeasurements);
      changeStateTo(espState.newState());
      if(publishTimer.hasExpired())
       {
         publishVariables();
       }
       if((alarmTimer.hasExpired()) && (previousState != S_IDLE))
       {
       publishVariableAlarm();
       }
    }

  break;

  case S_TEMP_TOO_LOW:
  
    if(bmeTimer.hasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp(totalMeasurements);
      humidity = espState.readHumidity(totalMeasurements);
      changeStateTo(espState.newState());
      if(publishTimer.hasExpired())
      {
        publishVariables();
      }
      if((alarmTimer.hasExpired()) && (previousState != S_TEMP_TOO_LOW))
      {
        publishVariableAlarm();
      }
    }

  break;

  case S_TEMP_TOO_HIGH:
  
    if(bmeTimer.hasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp(totalMeasurements);
      humidity = espState.readHumidity(totalMeasurements);
      changeStateTo(espState.newState());
      if(publishTimer.hasExpired())
      {
        publishVariables();
      }
      if((alarmTimer.hasExpired()) && (previousState != S_TEMP_TOO_HIGH))
      {
        publishVariableAlarm();
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

// Function used for publishing variables to ubidots, which transfers the variables to the screen.
void publishVariables() 
{
  ubidots.add(VARIABLE_LABEL_TEMP, temperature); // Inserting variable-label and temperaturevalue to ubidots
  ubidots.add(VARIABLE_LABEL_HUMIDITY, humidity); // Inserting humidityvalue
  ubidots.publish(DEVICE_LABEL); // Inserting which device on ubidots it should be published to
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  publishTimer.start(5000); // Timer to reduce publishments to ubidots.
}

// Function used for publishing variables to ubidots, which transfers the variables to alarmsystem.
void publishVariableAlarm()
{
 int mottakerVariabel;
 if(currentState == 0)
 {
  mottakerVariabel = 0; // 0: reciever turns off alarm.
  previousState = S_IDLE;
 }
 else if(currentState == 1)
 {
 mottakerVariabel = 1; // 1: reciever turns on alarm and displays system state.
 previousState = S_TEMP_TOO_LOW;
 }
 else
 {
  mottakerVariabel = 1; // 1: reciever turns on alarm and displays system state.
  previousState = S_TEMP_TOO_HIGH;
 }
 ubidots.add(VARIABLE_LABEL_TEMPALARM, mottakerVariabel); 
 ubidots.publish(DEVICE_LABEL_ALARM);
 alarmTimer.start(15000); // Timer to reduce amount of transmissions to alarm
}

// Function changing esp state, instantly publishes variables to screen when state is changed to a new one. 
void changeStateTo(int state)
{
  if(currentState != state)
  {
  Serial.print("State changed from ");
  Serial.print(currentState);
  currentState = state;
  Serial.print(" to ");
  Serial.print(state);
  Serial.println();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  ubidots.add(VARIABLE_LABEL_TEMP, temperature); // Inserting variable-label and temperaturevalue to ubidots
  ubidots.add(VARIABLE_LABEL_HUMIDITY, humidity); // Inserting humidityvalue
  ubidots.publish(DEVICE_LABEL); // Inserting which device on ubidots it should be published to
  alarmTimer.start(15000);
  }
  bmeTimer.start(5000); // Timer to reduce bme total measurements.
}
