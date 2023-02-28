#include "BluetoothSerial.h"
#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_ADS1015.h> // Search for Adafruit 1X15

Adafruit_ADS1115 ads1115;	// Construct an ads1115

// variable for storing the potentiometer value
int potValue = 0;
double Voltage0 = 0.0, Voltage1 = 0.0, adc0 = 0.0, adc1 = 0.0, diff = 0.0;

void setup() {
  Serial.begin(115200);
  ads1115.begin();  // Initialize ads1115
}

void loop() {
    adc0 = ads1115.readADC_SingleEnded(0);
    //adc1 = ads1115.readADC_SingleEnded(1);
    Voltage0 = (adc0 * 0.1875)/1000;
    //Voltage1 = (adc1 * 0.1875)/1000;
    //diff = Voltage0-Voltage1;
    Serial.println(Voltage0, 7); // 7 = number of meaningful digits
}