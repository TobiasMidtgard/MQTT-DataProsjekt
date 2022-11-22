#include "Ubidots.h"
#include "buzzer.h"
#include "screen.h"

int warning;  

const int buzzer = 4;
const int button = 5;
int button_read;
int buzzer_start;
int readint;


long unsigned int Main_Millis;
long unsigned My_millis;


void setup()
{
  ScreenSetup();

  BuzzerSetup(buzzer);
  pinMode(button, INPUT_PULLUP);

  Serial.begin(115200);

  UbidotsSetup();
  timer = millis();
}

void loop()
{
  Main_Millis = millis();
  
  Ubidots_Send_data(Main_Millis);
  
  if ((Main_Millis - My_millis) > 1000) 
  {
    My_millis = millis();
    readint = motat.toInt();
    motat = "";
    button_read = digitalRead(button);
    Serial.println(motatint);
    if (readint > 0) {
      Serial.println(motat);
      motatint = readint;
      Serial.println(motatint);
      display.setCursor(0, 10);
      Serial.println("motat");
    }
    switch(motatint) {
      case 6: {
        motatint = 0;
        buzzer_off();
        clear_screen();
        break;
      }
      case 11: {
        Screen_varmt();
        buzzer_millis = millis();
        motatint = 1;
        break;
      }
      case 10: {
        Screen_kalt();
        buzzer_millis = millis();
        motatint = 1;
        break;
      }
      case 1: {
        motatint = BuzzerActive(Main_Millis,button_read);
        break;
      }
      case 2: {
        motatint = Buzzercooldown(Main_Millis,button_read);
        break;
      }
      case 5: {
        Screen_door();
        buzzer_millis = millis();
        motatint = 1;
        break;
      }
    }
  }
}
