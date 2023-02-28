#include "BluetoothSerial.h"
#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_ADS1015.h> // Search for Adafruit 1X15

Adafruit_ADS1115 ads1115;	// Construct an ads1115

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 35;
const int PIN2 = 34;

// variable for storing the potentiometer value
int potValue = 0;
double Voltage0 = 0.0, adc0 = 0.0;

void setup() {
  Serial.begin(115200);
  ads1115.begin();  // Initialize ads1115
  pinMode(PIN2,INPUT);
}

void loop() {
    Serial.println("1");
    if(digitalRead(PIN2) == HIGH)
    {
        while(1)
        {
            adc0 = ads1115.readADC_SingleEnded(0);
            Voltage0 = (adc0 * 0.1875)/1000;
            Serial.println(Voltage0, 7); // 7 = number of meaningful digits
        }
    }
}