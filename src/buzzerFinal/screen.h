#ifndef screen_H //define the file as a header file
#define screen_H

#include <Wire.h>             // includes librearys
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // define the screen size

void ScreenSetup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));  // error 
    for(;;); // infenet loop
  }
  display.clearDisplay(); // clear the display on the screen
  display.setTextSize(1); // select's a text size
  display.setTextColor(WHITE); // select's a text color
  display.setCursor(0, 10);  // sets the place on the screen where you want to start when sending it data
}

void Screen_cold() {  // function for if the refrigerator is to cold 
  display.println("Kjoleskapet er for kalt");  // message on the screen
  display.display(); // sends the message to the screen
  delay(3000); //delay to give time between the screen truning on and the buzzer truning on
}

void Screen_varmt() { // function for if the refrigerator is to warm
  display.println("Kjoleskapet er ");   // message on the screen
  display.println("for varmt"); 
  display.display(); // sends the message to the screen
  delay(3000);
}
void Screen_door() { // function for the door being open sending a msg to the screen
  display.println("Kjoleskapet er opent");
  display.display();
  delay(3000);
}
void clear_screen() { // function for clearing the screen. it clears multiple times because the screen connection was unrelable and didnt always clear.
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
