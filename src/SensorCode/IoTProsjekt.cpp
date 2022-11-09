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

// Includes all the relevant libraies and header files.
#include "common.h"
#include "timer.h"
#include "decideEspState.h"

// Initialization of the functions.
void callback(char *topic, byte *payload, unsigned int length);
void seccureUbidotsConnection();
void changeStateTo(int state);

// define the valuee of the variable SEALEVELPRESSURE_HPA to 1013.25
#define SEALEVELPRESSURE_HPA (1013.25)

// Defines the values to connect to an internet connection
// Put your Ubidots TOKEN here
static const char *UBIDOTS_TOKEN = "BBFF-HG0PEHUbQb2Y4KAikNE4Z7DVgx5mSK";
// Put your Wi-Fi SSID (Name) here
static const char *WIFI_SSID = "Endre iPhone";
// Put your Wi-Fi password here
static const char *WIFI_PASS = "12345678";
// Put your Device label here to which data will be published
static const char *DEVICE_LABEL = "ESP32Endre";
// Put your Variable labels here  to which data  will be published
static const char *VARIABLE_LABEL = "C";
static const char *VARIABLE_LABEL2 = "H";
static const char *VARIABLE_LABEL_SUBSCRIBE = "relay";
static const char *VARIABLE_LABEL_TEMP = "temperatur";

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

// Global variable containing temperature and humidity measurement
float temperature;
float humidity;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ; // time to get serial running
  Serial.println(F("BME280 test"));

  espState.initialiseBme(); // Connecting bme-sensor to esp32

  Serial.println("-- Default Test --");
  Serial.println();

  ubidots.setDebug(true);
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
}

void loop()
{
  switch (currentState)
  {

  case S_IDLE:

    if (myTimer.bmeHasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp();
      humidity = espState.readHumidity();
      changeStateTo(espState.newState());
      if (myTimer.publishHasExpired())
      {
        publishVariables();
      }
    }

    break;

  case S_TEMP_TOO_LOW:

    if (myTimer.bmeHasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp();
      humidity = espState.readHumidity();
      changeStateTo(espState.newState());
      if (myTimer.publishHasExpired())
      {
        publishVariables();
      }
    }

    break;

  case S_TEMP_TOO_HIGH:

    if (myTimer.bmeHasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp();
      humidity = espState.readHumidity();
      changeStateTo(espState.newState());
      if (myTimer.publishHasExpired())
      {
        publishVariables();
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
  ubidots.add(VARIABLE_LABEL_TEMP, temperature);  // Inserting variable-label and temperaturevalue to ubidots
  ubidots.add(VARIABLE_LABEL_HUMIDITY, humidity); // Inserting humidityvalue
  ubidots.publish(DEVICE_LABEL);                  // Inserting which device on ubidots it should be published to
  ubidots.loop();
  Serial.print("Temperature: ");
  Serial.print(temperature);
}

// Function changing esp state, instantly publishes variables when state is changed to a new one.
void changeStateTo(int state)
{
  if (currentState != state)
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

    ubidots.add(VARIABLE_LABEL_TEMP, temperature);  // Inserting variable-label and temperaturevalue to ubidots
    ubidots.add(VARIABLE_LABEL_HUMIDITY, humidity); // Inserting humidityvalue
    ubidots.publish(DEVICE_LABEL);                  // Inserting which device on ubidots it should be published to
    ubidots.loop();
  }
  myTimer.bmeStart(5000); // Timer to reduce bme total measurements.
  myTimer.publishStart(10000);
}
