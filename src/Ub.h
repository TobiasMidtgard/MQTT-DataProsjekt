#ifndef Ubi_H
#define Ubi_H

#include <UbidotsEsp32Mqtt.h>



/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-HG0PEHUbQb2Y4KAikNE4Z7DVgx5mSK";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "Endre iPhone";      // Put here your Wi-Fi SSID  Get-2G-35DF81
const char *WIFI_PASS = "12345678";      // Put here your Wi-Fi password 7PX8GG8DRP
const char *DEVICE_LABEL = "ESP32Endre";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "C"; // Put here your Variable label to which data  will be published
const char *VARIABLE_LABEL2 = "H";
const char *VARIABLE_LABEL3 = "trykk";
const char *VARIABLE_LABEL_SUBSCRIBE = "relay";
String motat;
int motatint;

const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds

unsigned long timer;
uint8_t analogPin = 34; // Pin used to read data from GPIO34 ADC_CH6.
uint8_t analogPin2 = 35; // Pin used to read data from GPIO34 ADC_CH6.
float value2;
Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    //if ((int)payload[i] > 0){
    motat = motat + (char)payload[i];
    
  //  }
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/
long unsigned int Milli_test;
long unsigned My_millis;
int C;
int H;


#endif
