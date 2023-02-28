#include "BluetoothSerial.h"
#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_ADS1015.h> // Search for Adafruit 1X15

Adafruit_ADS1115 ads1115;	// Construct an ads1115

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 35;
const int PIN2 = 2;
const int PIN = 26;

// variable for storing the potentiometer value
int potValue = 0;
int k = 0;
int a = 0;
double Voltage0 = 0.0, adc0 = 0.0;

void setup() {
Serial.begin(115200);
  ads1115.begin();  // Initialize ads1115
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW); //gpio pin right above grnd on left side
  digitalWrite(PIN2, LOW);
 }

void loop() {
  if (digitalRead(PIN2) == HIGH) //Read GPIO 12
  {
    char buffer [1000];
    while(digitalRead(PIN2) == HIGH) //samplerate på 128 i 20 sekunder.
    {
        adc0 = ads1115.readADC_SingleEnded(0);
        Voltage0 = (adc0 * 0.1875)/1000;
        if(k==0)
        {
            Serial.flush(); //Waits until our string has been recieved in the serial terminal
            digitalWrite(PIN, HIGH); //Sending trigger signal to analog discovery
            k = 1;
        }
        Serial.println(Voltage0, 7); // 7 = number of meaningful digits
    }
 }
}

