#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiUdp.h>
#include "UbidotsEsp32Mqtt.h"
#include "NTPClient.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "Tobias sin iPhone";
const char* password = "Tobias123";

const char* UBIDOTS_TOKEN = "BBFF-2OYFDrW6Ts35uxYLMaOyNgkfW1ZOKw";
const char* device_sub = "esp32";
const char* VARIABLE_LABEL_TEMP = "temperatur";
const char* VARIABLE_LABEL_HUMIDITY = "fuktighet";

const char* device_pub = "";
const char* variable_pub = "";

unsigned long previous_time = 0;

const int button = 18;

String formattedDate = "n/a";
String lastTemp = "n/a";
String lastHum = "n/a";

Ubidots ubidots(UBIDOTS_TOKEN);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void write(String s) {
  display.print(s);
  display.display();
}

void callback(char* topic, byte* message, unsigned int length) {

  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  display.clearDisplay();
  display.setCursor(0, 10);
  Serial.println(topic);

  display.println(formattedDate);

  if (topic == "temperatur") {
    lastTemp = msg;
  }
  if (topic == "fuktighet") {
    lastHum = msg;
  }
  
  display.println("temp: " + lastTemp + "C");
  display.println("fukt: " + lastHum + "%");
  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(button, INPUT);

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
  write("connected");
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(device_sub, VARIABLE_LABEL_TEMP);
  ubidots.subscribeLastValue(device_sub, VARIABLE_LABEL_HUMIDITY);

  timeClient.begin();
  timeClient.setTimeOffset(3600);
  display.clearDisplay();
  display.setCursor(0, 10);
  write("ready");
}

void loop() {

  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(formattedDate);
  display.println("temp: " + lastTemp + "C");
  display.println("fukt: " + lastHum + "%");
  display.display();
  delay(1000);

  if (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  formattedDate = timeClient.getFormattedDate();
  
  if (!ubidots.connected()) {
    ubidots.reconnect();
    ubidots.subscribeLastValue(device_sub, VARIABLE_LABEL_TEMP);
    ubidots.subscribeLastValue(device_sub, VARIABLE_LABEL_HUMIDITY);
  }
  ubidots.loop();

  while (digitalRead(button) == HIGH) {
    while (digitalRead(button) == HIGH);
    //request readings
    delay(100);
  }
}
