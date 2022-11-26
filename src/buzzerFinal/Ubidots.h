#ifndef Ubidots_H
#define Ubidots_H

#include "UbidotsEsp32Mqtt.h" // includes the ubidots librery


const char *UBIDOTS_TOKEN = "BBFF-tiXYhoORGOAzVHV09jkY8lPcImrlDk";  // Put here your Ubidots TOKEN 
const char *WIFI_SSID = "Endre iPhone";      // Put here your Wi-Fi SSID  Get-2G-35DF81    PF14
const char *WIFI_PASS = "12345678";      // Put here your Wi-Fi password 7PX8GG8DRP  ZdFiBppx
const char *DEVICE_LABEL = "esp32-alarm";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL_SUBSCRIBE = "temperatur-alarm";
const char *VARIABLE_LABEL_SUBSCRIBE2 = "door-alarm";
String motat;
int temp_alarm; // state veribles for what alarms should be on
int door_alarm;
String topic_lable;

const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds

unsigned long timer;
Ubidots ubidots(UBIDOTS_TOKEN); // declering ubidots token to the ubidots library



void callback(char *topic, byte *payload, unsigned int length)  // function for resiving the data for ubidots
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    Serial.println();
    motat = motat + (char)payload[i]; // a string with just the number being resived 
    topic_lable = String(topic);
  }
  temp_alarm = motat.toInt(); // turns the resived number to a int from string 
  if ((temp_alarm < 1) && (topic_lable.length() > 40)) { // checks if the number resived is over 0 and what topic it is
    temp_alarm = 6;
  }
  else if ((temp_alarm < 1) && (topic_lable.length() > 30)) {
    door_alarm = 6;
  }
  else if (topic_lable.length() > 40) {
    temp_alarm = 11;
  }
  else if (topic_lable.length() > 30) {
    door_alarm = 5;
  }
}

void Ubidots_connect(long(MM)) {  // function for connecting to ubidots and checking for messages
  
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

void UbidotsSetup() {  // function for the ubidots setup
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE);
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE2);
}

#endif
