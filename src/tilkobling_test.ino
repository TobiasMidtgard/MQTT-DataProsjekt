/******************************************
 *
 * This example works for both Industrial and STEM users.
 *
 * Developed by Jose Garcia, https://github.com/jotathebest/
 *
 * ****************************************/

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsEsp32Mqtt.h"


/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-HG0PEHUbQb2Y4KAikNE4Z7DVgx5mSK";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "Endre iPhone";      // Put here your Wi-Fi SSID  Get-2G-35DF81
const char *WIFI_PASS = "12345678";      // Put here your Wi-Fi password 7PX8GG8DRP
const char *DEVICE_LABEL = "ESP32Endre";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "C"; // Put here your Variable label to which data  will be published
const char *VARIABLE_LABEL2 = "H";
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


void setup()
{
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE);

  timer = millis();
}

void loop()
{
  Milli_test = millis();
  // put your main code here, to run repeatedly:
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  if ((Milli_test - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    float value = analogRead(analogPin);
    if (!ubidots.connected())
    {
    ubidots.reconnect();
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE); // Insert the dataSource and Variable's Labels
    }

    value2 = analogRead(analogPin2);
    value2 = value2/4095 * 100; 
    ubidots.add(VARIABLE_LABEL, C); // Insert your variable Labels and the value to be sent
    ubidots.add(VARIABLE_LABEL2, value2);
    ubidots.publish(DEVICE_LABEL);  
    
    timer = millis();
  }
  ubidots.loop();
  if ((Milli_test - My_millis) > 1000) 
  {
    C = C + 1;
    H = H + 3;
    My_millis = millis();
    motatint = motat.toInt();
    if (motatint > 0) {
      Serial.println(motat);
      Serial.println(motatint);
      motatint = 0;
      motat = "";
    }

    if (C == 15) {
      C = 0;
      H = 15;
    }
  }
}
