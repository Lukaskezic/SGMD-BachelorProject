#include "CurrentSensor.h"
#include <Arduino.h>
#include <Adafruit_INA219.h>
#include "sdcard.h"
#include "ftoa.h"
#include "SD.h"
#include "FS.h"

using namespace std;

Adafruit_INA219 ina219(0x40);

CurrentSensor::CurrentSensor()
{

}

void CurrentSensor::setupCurrentSensor()
{
  Serial.begin(115200);
  ina219.begin();
  ina219.setCalibration_16V_400mA();
  delay(10000); //delay to make sure current has been injected.
  current = 0;
}

void CurrentSensor::receiveCurrentSensorData()
{
  for(int i = 0; i<100;i++)
  {
    current+=ina219.getCurrent_mA();
  }
  current = current/100; //average value
  current = current/1000; //mA to A
  double resistor_factor = 2.0/0.1;
  current = current/resistor_factor; //Because a 2.2ohms shunt resistor is used instead of 0.1ohms on INA219 Current Sensor
  double noisefloor = 0.000002971; //Average of measured noisefloor
  current = current - noisefloor; //Removing noise floor
}

void CurrentSensor::saveCurrentSensorData(double current)
{
  char buffer [1000];
  appendFile(SD, "/data.txt", ftoa(current,buffer,10));
  appendFile(SD, "/data.txt", "\n");
  Serial.print(current);
}

double CurrentSensor::getCurrent()
{
    return current;
}