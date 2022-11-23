#ifndef Ubidots_H
#define Ubidots_H

#include "UbidotsEsp32Mqtt.h"


const char *UBIDOTS_TOKEN = "BBFF-2OYFDrW6Ts35uxYLMaOyNgkfW1ZOKw";  // Put here your Ubidots TOKEN      BBFF-HG0PEHUbQb2Y4KAikNE4Z7DVgx5mSK    BBFF-2OYFDrW6Ts35uxYLMaOyNgkfW1ZOKw
const char *WIFI_SSID = "PF14";      // Put here your Wi-Fi SSID  Get-2G-35DF81  Endre iPhone
const char *WIFI_PASS = "ZdFiBppx";      // Put here your Wi-Fi password 7PX8GG8DRP
const char *DEVICE_LABEL = "esp32-alarm";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL_SUBSCRIBE = "temperatur-alarm";
const char *VARIABLE_LABEL_SUBSCRIBE2 = "door-alarm";
String motat;
int motatint;

const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds

unsigned long timer;
float value2;
Ubidots ubidots(UBIDOTS_TOKEN);



void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    motat = motat + (char)payload[i];
  }
  Serial.println();
}

void Ubidots_Send_data(long(MM)) {  
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  if ((MM - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    if (!ubidots.connected())
    {
    ubidots.reconnect();
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE); // Insert the dataSource and Variable's Labels
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE2);
    }

    timer = millis();
  }
  ubidots.loop();
}

void UbidotsSetup() {
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE);
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE2);
}

#endif
