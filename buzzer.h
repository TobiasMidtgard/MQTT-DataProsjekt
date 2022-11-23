#ifndef buzzer_H
#define buzzer_H

long unsigned buzzer_millis;
int freq = 2000;
int channel = 0;
int resolution = 8;

void BuzzerSetup(int buzzer) {
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(buzzer, channel);
}
int BuzzerActive(long MM,int button) {
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

int Buzzercooldown(long MM,int button) {
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
void buzzer_off() {
  ledcWriteTone(channel, 0);
}
#endif
