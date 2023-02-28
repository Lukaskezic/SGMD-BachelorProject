//#include "BluetoothSerial.h"
#include "Wire.h"
//#include "Adafruit_ADS1X15.h" // Search for Adafruit 1X15
#include "FS.h"
#include "SD.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

// incase of MD5 error write this in CMD: python -m esptool --chip auto --port COM3 write_flash_status --non-volatile 0

//BluetoothSerial SerialBT;
//Adafruit_ADS1115 ads1115;	// Construct an ads1115
//Adafruit_ADS1115 ads(0x48)

unsigned long myTime;
unsigned long microseconds;
unsigned int sampling_period_us;
int SAMPLES = 1000;
int SAMPLING_FREQUENCY = 50;
double dataArray[1000];
int PIN = 26;
int PIN2 = 2;
int potPin = 35;

char dataStr[100] = "";
int i = 0;
int j = 0;
int k = 0;
// variable for storing the external adc
double Voltage0 = 0.0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN, OUTPUT);
  analogReadResolution(12); //resolution between 9-12
  analogSetAttenuation(ADC_11db); //Attenuation (the range of measurement) between 0db - 11db (the lower the more precise, but lower max voltage)
  //SerialBT.begin("ESP32test2"); //Bluetooth device name
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
  digitalWrite(PIN, LOW); //gpio pin right above grnd on left side
  digitalWrite(PIN2, LOW);
  //ads1115.begin();  // Initialize ads1115   
 }

void loop() {
  dataStr[0] = 0; //Clean out string
  if (digitalRead(PIN2) == HIGH) //Read GPIO 12
  {
    char buffer [1000];
    for(i; i<SAMPLES; i++)
    {
      microseconds = micros(); //Overflows after around 70 minutes!

      //int16_t adc0;
 
      //adc0 = ads1115.readADC_SingleEnded(0);

      //Voltage0 = (adc0 * 0.1875)/1000;

      Voltage0 = analogReadMilliVolts(potPin);

      //SerialBT.print("AIN0: ");
      Serial.println(Voltage0, 7); // 7 = number of meaningful digits
      if(k==0)
      {
         Serial.flush(); //Waits until our string has been recieved in the serial terminal
         digitalWrite(PIN, HIGH);
         k = 1;
      }

      //SerialBT.println("V");

      dataArray[i] = Voltage0;

      while(micros() < (microseconds + sampling_period_us)){
      }
    }
 }
}