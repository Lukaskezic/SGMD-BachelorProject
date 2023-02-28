#include <iostream>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "sdcard.h"
#include "ftoa.h"

#define TEMP_PIN  12 // ESP32 pin GIOP21 connected to DS18B20 sensor's DQ pin

OneWire oneWire(TEMP_PIN);
DallasTemperature DS18B20(&oneWire);

float tempC; // temperature in Celsius

void setup() {
  Serial.begin(115200);
  DS18B20.begin();

  if(!SD.begin()){ //Checking if RTC available
  Serial.println("Card Mount Failed");
  while(1);
  }
}

void loop() {
  DS18B20.requestTemperatures(); // Sending command to receive temperatures
  tempC = DS18B20.getTempCByIndex(0); // Reading the temperature in °C
  char buffer [1000];
  //Saving data to data.txt file
  appendFile(SD, "/data.txt", "Temperature: ");
  appendFile(SD, "/data.txt", ftoa(tempC,buffer,10));
  appendFile(SD, "/data.txt", "\n\n");
}