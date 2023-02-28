#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

BluetoothSerial SerialBT;

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 35;
int i = 0;

unsigned long myTime;
unsigned long microseconds;
unsigned int sampling_period_us;
int SAMPLES = 1000;
int SAMPLING_FREQUENCY = 50;

char dataStr[100] = "";

// variable for storing the potentiometer value
int potValue = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  analogReadResolution(12);
  SerialBT.begin("ESP32test4"); //Bluetooth device name
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
  digitalWrite(12, LOW);
}

void loop() {
  dataStr[0] = 0; //clean out string
  // Reading potentiometer value
  if(digitalRead(12) == HIGH)
  {
    for(i; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!

        potValue = analogReadMilliVolts(potPin);
        Serial.println(potValue);

        while(micros() < (microseconds + sampling_period_us)){
        }
    }
  }
}