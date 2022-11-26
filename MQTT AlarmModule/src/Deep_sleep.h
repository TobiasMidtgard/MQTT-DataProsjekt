
#ifndef Deep_sleep_H
#define Deep_sleep_H

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  300  // amount of time the program will sleep in seconds

void deepSleep() { // setsup the timer and starts the sleep
    esp_sleep_enable_timer_wakeup(uS_TO_S_FACTOR * TIME_TO_SLEEP);
    esp_deep_sleep_start();
}

void hibernate() {  // truns off parts of the esp32 to save power
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH,   ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL,         ESP_PD_OPTION_OFF);
    
    deepSleep();
}

#endif