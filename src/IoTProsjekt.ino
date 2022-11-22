#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <math.h>
#include "UbidotsEsp32Mqtt.h"
#include "timer.h"
#include "decideEspState.h"


const char *UBIDOTS_TOKEN = "BBFF-2OYFDrW6Ts35uxYLMaOyNgkfW1ZOKw";  // Ubidots TOKEN
const char *WIFI_SSID = "PF14";      // Wi-Fi SSID
const char *WIFI_PASS = "ZdFiBppx";      // Wi-Fi password
const char *DEVICE_LABEL = "esp32";   // Device label to which data will be published
const char *VARIABLE_LABEL_TEMP = "temperatur"; // Variable label to which temperature  will be published
const char *VARIABLE_LABEL_HUMIDITY = "fuktighet"; // Variable label to which humidity will be published
const char *DEVICE_LABEL_ALARM = "esp32-alarm";
const char *VARIABLE_LABEL_TEMPALARM = "temperatur-alarm";

Ubidots ubidots(UBIDOTS_TOKEN); 

// System states
const int S_IDLE = 0;
const int S_TEMP_TOO_LOW = 1;
const int S_TEMP_TOO_HIGH = 2;

// Starting system state
int currentState = S_IDLE;
// Previous system state
int previousState; 

// Defining variable which uses header-files.
Timer myTimer;
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
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  myTimer.bmeStart(1000);
  myTimer.publishStart(10000);
  myTimer.alarmStart(20000);
}


void loop()
{ 
 ubidots.loop();
 int totalMeasurements = 50;
 switch(currentState)
 {

  case S_IDLE:
  
    if(myTimer.bmeHasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp(totalMeasurements);
      humidity = espState.readHumidity(totalMeasurements);
      changeStateTo(espState.newState());
      if(myTimer.publishHasExpired())
       {
         publishVariables();
       }
       if((myTimer.alarmTimerExpired()) && (previousState != S_IDLE))
       {
       publishVariableAlarm();
       }
    }

  break;

  case S_TEMP_TOO_LOW:
  
    if(myTimer.bmeHasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp(totalMeasurements);
      humidity = espState.readHumidity(totalMeasurements);
      changeStateTo(espState.newState());
      if(myTimer.publishHasExpired())
      {
        publishVariables();
      }
      if(myTimer.alarmTimerExpired())
      {
        publishVariableAlarm();
      }
    }

  break;

  case S_TEMP_TOO_HIGH:
  
    if(myTimer.bmeHasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp(totalMeasurements);
      humidity = espState.readHumidity(totalMeasurements);
      changeStateTo(espState.newState());
      if(myTimer.publishHasExpired())
      {
        publishVariables();
      }
      if(myTimer.alarmTimerExpired())
      {
        publishVariableAlarm();
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

// Function publishing variables to ubidots when timer is completed
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
  myTimer.publishStart(30000); // Timer to reduce publishments to ubidots.
}

void publishVariableAlarm()
{
 int mottakerVariabel;
 if(currentState == 0)
 {
  mottakerVariabel = 6; // 6: reciever turns off alarm.
  previousState = S_IDLE;
 }
 else if(currentState == 1)
 {
 mottakerVariabel = 10; // 10: reciever turns on alarm and displays system state.
 previousState = S_TEMP_TOO_LOW;
 }
 else
 {
  mottakerVariabel = 11; // 11: reciever turns on alarm and displays system state.
  previousState = S_TEMP_TOO_HIGH;
 }
 ubidots.add(VARIABLE_LABEL_TEMPALARM, mottakerVariabel); 
 ubidots.publish(DEVICE_LABEL_ALARM);
 myTimer.alarmStart(60000); // Timer to reduce amount of transmissions to alarm
}

// Function changing esp state, instantly publishes variables when state is changed to a new one. 
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
  myTimer.alarmStart(5000);
  }
  myTimer.bmeStart(5000); // Timer to reduce bme total measurements.
}
