#include "Ubidots.h"  // includes librarys 
#include "buzzer.h"
#include "screen.h"
#include "Deep_sleep.h"

const int buzzer = 4; // pins where the buzzer and button is
const int button = 5;

int button_read;
int buzzer_start;

long unsigned int Main_Millis; 
long unsigned My_millis;


void setup()
{
  ScreenSetup();  // from screen.h

  BuzzerSetup(buzzer); // from buzzer.h
  pinMode(button, INPUT_PULLUP); // defines button as input

  Serial.begin(115200);  // starts serial monitor

  UbidotsSetup(); // from ubidots.h
}

void loop()
{
  Main_Millis = millis();
  if ((Main_Millis > 60000) && (temp_alarm < 1) && (door_alarm < 1)) {
    Serial.println("sleep");
    Serial.flush(); 
    deepSleep(); // from Deep_sleep.h
  }
  Ubidots_connect(Main_Millis); // from ubidots.h
  
  if ((Main_Millis - My_millis) > 1000) // timer to only run this code every 1 second
  {
    My_millis = millis(); // resets the timer
    motat = "";
    button_read = digitalRead(button); // reads the button state
    switch(door_alarm) { // switch case of the door alarm 
      case 6: {
        door_alarm = 0;
        buzzer_off(); // from buzzer.h
        clear_screen(); // from screen.h
        break;
      }
      case 5: {
        Screen_door();
        buzzer_millis = millis();
        door_alarm = 1;
        break;
      }
      case 1: {
        door_alarm = BuzzerActive(Main_Millis,button_read); // from buzzer.h
        break;
      }
      case 2: {
        door_alarm = Buzzercooldown(Main_Millis,button_read);
        break;
      } 
    }
    switch(temp_alarm) { // switch case for the temprature alarm
      case 6: {
        temp_alarm = 0;
        buzzer_off(); // from buzzer.h
        clear_screen(); // from screen.h
        break;
      }
      case 11: {
        Screen_varmt(); // from screen.h
        buzzer_millis = millis();
        temp_alarm = 1;
        break;
      }
      case 10: {
        Screen_cold(); // from screen.h
        buzzer_millis = millis();
        temp_alarm = 1;
        break;
      }
      case 1: {
        temp_alarm = BuzzerActive(Main_Millis,button_read); // from buzzer.h
        break;
      }
      case 2: {
        temp_alarm = Buzzercooldown(Main_Millis,button_read); // from buzzrer.h
        break;
      }
    }
  }
}