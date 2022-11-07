#ifndef BME280sense_H
#define BME280sense_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/*********
  Complete project details at https://randomnerdtutorials.com  
*********/



/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/
float Temp;
float Pressure;
float Humidity;

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void printValues() {
  Temp = bme.readTemperature();
  Pressure = bme.readPressure() / 1000;
  Humidity = bme.readHumidity();
}
/*
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
  */

  #endif
