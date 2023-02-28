#include "TemperatureSensor.h"
#include "RTCModule.h"
#include <iostream>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "sdcard.h"
#include "ftoa.h"

using namespace std;

OneWire oneWire(TEMP_PIN);
DallasTemperature DS18B20(&oneWire);

TemperatureSensor::TemperatureSensor()
{
}

void TemperatureSensor::setupTemperatureSensor(RTCModule* r1)
{
  r1_ = r1; //Passing RTCModule object
  Serial.begin(115200);
  DS18B20.begin();
}

void TemperatureSensor::receiveTemperatureSensorData()
{
  DS18B20.requestTemperatures(); // Sending command to receive temperatures
  tempC = DS18B20.getTempCByIndex(0); // Reading the temperature in °C
}

void TemperatureSensor::saveTemperatureSensorData()
{
  char buffer [1000];
  std::string hi = r1_->getFileName(); //Requesting filename
  const char* filename = hi.c_str(); //Converting string to const char*
  appendFile(SD, filename, "Temperature: "); 
  appendFile(SD, filename, ftoa(tempC,buffer,10));
  appendFile(SD, filename, "\n\n");
}