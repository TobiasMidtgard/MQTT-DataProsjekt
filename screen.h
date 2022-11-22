#ifndef screen_H
#define screen_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void ScreenSetup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
}

void Screen_kalt() {
  display.println("Kjoleskapet er for kalt");
  display.display();
  delay(3000);
}

void Screen_varmt() {
  display.println("Kjoleskapet er ");
  display.print("for varmt");
  display.display();
  delay(3000);
}
void Screen_door() {
  display.println("Kjoleskapet er opent");
  display.display();
  delay(3000);
}
void clear_screen() {
  display.clearDisplay();
  delay(2000);
  display.display();
  display.setCursor(0, 10);
  display.clearDisplay();
  delay(2000);
  display.display();
  display.setCursor(0, 10);
  display.clearDisplay();
  delay(2000);
  display.display();
}

#endif
