#include "BME280sense.h"
#include "Ub.h"







void setup() {
  Serial.begin(115200);
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE);

  timer = millis();



  Serial.println(F("BME280 test"));

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
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
    if (!ubidots.connected())
    {
    ubidots.reconnect();
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE); // Insert the dataSource and Variable's Labels
    }

    value2 = analogRead(analogPin2);
    value2 = value2/4095 * 100; 
    ubidots.add(VARIABLE_LABEL, Temp); // Insert your variable Labels and the value to be sent
    ubidots.add(VARIABLE_LABEL2, Humidity);
    ubidots.add(VARIABLE_LABEL3, Pressure);
    ubidots.publish(DEVICE_LABEL);  
    
    timer = millis();
  }
  ubidots.loop();
  if (Milli_test - My_millis > 5000) {
    My_millis = millis();
    printValues();
  }
}
