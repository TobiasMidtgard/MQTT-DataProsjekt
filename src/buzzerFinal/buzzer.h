#ifndef buzzer_H
#define buzzer_H

long unsigned buzzer_millis; // buzzer timer to make breaks in the buzzer
// setting PWM properties
int freq = 2000; 
int channel = 0;  
int resolution = 8;

void BuzzerSetup(int buzzer) { // setup for the buzzer
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(buzzer, channel);
}
int BuzzerActive(long MM,int button) { // starts the buzzing and changes the frequency, also checks if the butten is pressed or timer is out
  ledcWriteTone(channel, freq);
  freq = freq + 50;
  if (freq > 1200) {
    freq = 800;
  }
  if (MM - buzzer_millis > 30000) {
    ledcWriteTone(channel, 0);
    buzzer_millis = millis();
    return 2;
  }
  else if (button == LOW) {
    ledcWriteTone(channel, 0);
    buzzer_millis = millis();
    return 6;
  }
  return 1;
}

int Buzzercooldown(long MM,int button) { // function for when the alarm is on but the buzzer is on cooldown to have breaks in the alarm, still checks if the butten is pressed
  if (button == LOW) {
    buzzer_millis = millis();
    return 6;
  }
  else if (MM - buzzer_millis > 10000) {
    buzzer_millis = millis();
    return 1; 
  }
  return 2;
}
void buzzer_off() { // turns the buzzer off
  ledcWriteTone(channel, 0);
}
#endif
