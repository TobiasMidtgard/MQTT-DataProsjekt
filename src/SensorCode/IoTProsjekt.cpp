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

//? Establishes a connection with ubidots?
Ubidots ubidots(UBIDOTS_TOKEN);

// System states
const int S_IDLE = 0;
const int S_TEMP_TOO_LOW = 1;
const int S_TEMP_TOO_HIGH = 2;

// Sets the starting state to idle
int currentState = S_IDLE;

// Defining variable which uses header-files.
Timer myTimer;
EspState espState;

float temperature;

// Checks if the message has arrived
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

// Changes the esp state dependent on the temperature of the sensor
void changeStateTo(int state)
{
  int stateCheck = currentState;
  Serial.print("State changed from ");
  Serial.print(currentState);
  currentState = state;
  Serial.print(" to ");
  Serial.print(state);
  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Push to ubidots when esp in a new state
  if (stateCheck != currentState)
  {
    // Inserting variable-label and temperaturevalue to ubidots
    ubidots.add(VARIABLE_LABEL_TEMP, temperature);
    // Inserting which device on ubidots it should be published to
    ubidots.publish(DEVICE_LABEL);
    ubidots.loop();
  }
  // Timer to reduce bme total measurements
  myTimer.start(5000);
}

void setup()
{
  Serial.begin(115200);
  // Waits to establish a serial connection
  while (!Serial)
    ;
  // After a serial connection is established print a confirmation message
  Serial.println(F("BME280 test"));

  // Connects the ESP to the ubidots server
  ubidots.setDebug(true);
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE);

  espState.initialiseBme(); // BME sensor startup

  Serial.println("-- Default Test --");
  Serial.println();
}

void loop()
{
  // switch case to change the state of the ESP
  switch (currentState)
  {

  // Everything is alright
  case S_IDLE:
  {
    if (myTimer.hasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp();
      changeStateTo(espState.newState());
    }
  }

  break;

  // Too low temperature
  case S_TEMP_TOO_LOW:
  {
    if (myTimer.hasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp();
      changeStateTo(espState.newState());
    }
  }

  break;

  // Too high temperature
  case S_TEMP_TOO_HIGH:
  {
    if (myTimer.hasExpired())
    {
      seccureUbidotsConnection();
      temperature = espState.readTemp();
      changeStateTo(espState.newState());
    }
  }

  break;
  }
}
