/*
Copyright (c) 2016 SparkFun Electronics
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <SPI.h>
#include <ICM_20948.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiUdp.h>
#include "UbidotsEsp32Mqtt.h"
#include "NTPClient.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define USE_SPI       // Uncomment this to use SPI
#define SPI_PORT SPI // Your desired SPI port.       Used only when "USE_SPI" is defined
#define CS_PIN 5     // Which pin you connect CS to. Used only when "USE_SPI" is defined
#define WIRE_PORT Wire

ICM_20948_SPI myICM;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "wifiName";
const char* password = "password";

const char* UBIDOTS_TOKEN = "BBFF-2OYFDrW6Ts35uxYLMaOyNgkfW1ZOKw";
const char* DEVICE_LABEL = "esp32";
const char* DEVICE_LABEL_ALARM = "esp32-alarm";
const char* VARIABLE_LABEL_TEMP = "temperatur";
const char* VARIABLE_LABEL_HUMIDITY = "fuktighet";
const char* VARIABLE_LABEL_DOOR = "door-alarm";

unsigned long door_timer = 0;

bool opened = false;

String formattedDate = "n/a"; //shows this before any data has been received
String lastTemp = "n/a";
String lastHum = "n/a";

Ubidots ubidots(UBIDOTS_TOKEN);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void write(String s) { //function for easy writing to display
  display.print(s);
  display.display();
}

void callback(char* topic, byte* message, unsigned int length) {
  formattedDate = timeClient.getFormattedDate(); //the date is only updated when new data is collected

  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  display.clearDisplay();
  display.setCursor(0, 10);

  display.println(formattedDate);

  if (msg.toInt() > 100) { //if the value is above '100' it is probably temperature instead of humidity
    lastHum = msg;
  } else {
    lastTemp = String(msg.toFloat() - 200.0); //removes the artificially added 200 which was added to differentiate between temperature and humidity
  }

  display.println("temp: " + lastTemp + "C");
  display.println("fukt: " + lastHum + "%");
  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  SPI_PORT.begin();

  bool initialized = false;
  while (!initialized)
  {

    // Initialize the ICM-20948
    // If the DMP is enabled, .begin performs a minimal startup. We need to configure the sample mode etc. manually.

    myICM.begin(CS_PIN, SPI_PORT);

    Serial.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Trying again..."));
      delay(500);
    }
    else
    {
      initialized = true;
    }
  }

  bool success = true; // Use success to show if the DMP configuration was successful

  // Initialize the DMP. initializeDMP is a weak function. You can overwrite it if you want to e.g. to change the sample rate
  success &= (myICM.initializeDMP() == ICM_20948_Stat_Ok);
  success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED) == ICM_20948_Stat_Ok);
  success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Cpass, 54) == ICM_20948_Stat_Ok);        // Set to 1Hz
  success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Cpass_Calibr, 54) == ICM_20948_Stat_Ok); // Set to 1Hz

  // Enable the FIFO
  success &= (myICM.enableFIFO() == ICM_20948_Stat_Ok);

  // Enable the DMP
  success &= (myICM.enableDMP() == ICM_20948_Stat_Ok);

  // Reset DMP
  success &= (myICM.resetDMP() == ICM_20948_Stat_Ok);

  // Reset FIFO
  success &= (myICM.resetFIFO() == ICM_20948_Stat_Ok);

  // Check success
  if (!success) {
    while (1); // Do nothing more
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.setTextColor(WHITE);
  display.clearDisplay();

  write("connecting to wifi");
  ubidots.connectToWifi(ssid, password);
  display.println("");
  write("connected");
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_TEMP);
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_HUMIDITY);

  timeClient.begin();
  timeClient.setTimeOffset(3600);
  display.clearDisplay();
  display.setCursor(0, 10);
  write("ready");
}

void loop() {

  // Read any DMP data waiting in the FIFO
  // Note:
  //    readDMPdataFromFIFO will return ICM_20948_Stat_FIFONoDataAvail if no data is available.
  //    If data is available, readDMPdataFromFIFO will attempt to read _one_ frame of DMP data.
  //    readDMPdataFromFIFO will return ICM_20948_Stat_FIFOIncompleteData if a frame was present but was incomplete
  //    readDMPdataFromFIFO will return ICM_20948_Stat_Ok if a valid frame was read.
  //    readDMPdataFromFIFO will return ICM_20948_Stat_FIFOMoreDataAvail if a valid frame was read _and_ the FIFO contains more (unread) data.
  icm_20948_DMP_data_t data;
  myICM.readDMPdataFromFIFO(&data);

  if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) // Was valid data available?
  {
    if ((data.header & DMP_header_bitmap_Compass) > 0) { // Check for Compass
      float x = (float)data.Compass.Data.X; // Extract the compass data
      float y = (float)data.Compass.Data.Y;
      float z = (float)data.Compass.Data.Z;

      static float z_base = z; // the first measurment is what the angle values are relative to

      if (abs(z_base - z) < 10) { //if the angle gives a value lower than 10 the door is most likely closed
        if (opened) {
          ubidots.add(VARIABLE_LABEL_DOOR, 6); //'6' is a signal meaning that the door is closed and the alarm should stop
          ubidots.publish(DEVICE_LABEL_ALARM);
        }
        opened = false;
      }

      if (abs(z_base - z) > 20  && !opened) { //a value higher than 20 means the door is most likely open
        if ((millis() - door_timer) > 30000) { //if door is open and it has gone 30s
          ubidots.add(VARIABLE_LABEL_DOOR, 5); //'5' is the signal that the door has been opened
          ubidots.publish(DEVICE_LABEL_ALARM);
          opened = true;
        }
      }
      else {
        door_timer = millis();
      }
    }
  }
  if (myICM.status != ICM_20948_Stat_FIFOMoreDataAvail) { // If more data is available then we should read it right away - and not delay
    delay(10);
  }

  if (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  if (!ubidots.connected()) { //reconnects to ubidots on lost connection
    ubidots.reconnect();
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_TEMP);
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL_HUMIDITY);
  }
  ubidots.loop();
}
