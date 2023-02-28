#include "iostream"
#include <Arduino.h>
#include <Adafruit_ADS1015.h>
using namespace std;

Adafruit_ADS1115 ads1115;	// Construct an ads1115

char dataStr[100] = "";
int i = 0;

unsigned long myTime;
unsigned long microseconds;
unsigned int sampling_period_us;
int SAMPLES = 1000; 
int SAMPLING_FREQUENCY = 50; 
double dataArray[1000];
double dataArray2[1000];

int PIN = 34; // Pin for testing ESP32 ADC

// variable for storing the external adc
double Voltage0 = 0.0;
double Voltage1 = 0.0;

// Variables to store read data
double total, total1 = 0.0;
double avg, avg1 = 0.0;

// Setup function to set different parameters for both ESP32 and ADS1115
void setup() {
  Serial.begin(115200);
  analogReadResolution(12); //resolution between 9-12
  analogSetAttenuation(ADC_11db); //Attenuation (the range of measurement) between 0db - 11db (the lower the more precise, but lower max voltage)
  //SerialBT.begin("ESP32test2"); //Bluetooth device name (only used in bluetooth test)
  ads1115.begin();  // Initialize ads1115   
 }

void loop() {
      double adc0, adc1;

    for(i;i<100;i++)
    {
      
      adc0 = ads1115.readADC_SingleEnded(0);
      adc1 = ads1115.readADC_SingleEnded(1);
      Voltage0 = (adc0 * 0.1875)/1000;
      Voltage1 = (adc1 * 0.1875)/1000;
      total += Voltage0;
      total1 += Voltage1;
    }
      
      avg = (total/100);
      avg1 = (total1/100);
      Serial.print("ADS1115: ");
      Serial.println("Vin");
      Serial.println(avg, 10); // 7 = number of meaningful digits
      Serial.println();
      Serial.println("VOUT");
      Serial.println(avg1, 10); // 7 = number of meaningful digits
      Serial.println();
      Serial.println("DIFFERENCE");
      Serial.println((avg-avg1), 10); // 7 = number of meaningful digits
      Serial.println("-------------------------------------------");
      total = 0.0;
      total1 = 0.0;
      i = 0;
      delay(10000);
}