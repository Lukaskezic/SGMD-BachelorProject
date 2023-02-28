#include "BluetoothSerial.h"
#include "Wire.h"
#include "Adafruit_ADS1X15.h" // Search for Adafruit 1X15

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
Adafruit_ADS1115 ads1115;	// Construct an ads1115

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 35;

// variable for storing the potentiometer value
int potValue = 0;
float Voltage0 = 0.0, Voltage1 = 0.0, Voltage2 = 0.0, Voltage3 = 0.0;

void setup() {
  Serial.begin(9600);
  analogReadResolution(12); //resolution between 9-12
  analogSetAttenuation(ADC_11db); //Attenuation (the range of measurement) between 0db - 11db (the lower the more precise, but lower max voltage)
  //SerialBT.begin("ESP32test"); //Bluetooth device name
  ads1115.begin();  // Initialize ads1115
}

void loop() {
  // Reading potentiometer value
  //if (SerialBT.available()) //Bluetooth setup
  //{
    //potValue = analogReadMilliVolts(potPin); //function for esp32 adc
    //Serial.println(ReadVoltage(35),3); //read voltage of esp32 adc
    //Serial.println(analogRead(35)); //read esp32 adc
    //Serial.println(potValue); //print esp32 adc
    //SerialBT.println(potValue); //Bluetooth setup
    int16_t adc0, adc1, adc2, adc3;
 
    adc0 = ads1115.readADC_SingleEnded(0);
    adc1 = ads1115.readADC_SingleEnded(1);
    adc2 = ads1115.readADC_SingleEnded(2);
    adc3 = ads1115.readADC_SingleEnded(3);

    Voltage0 = (adc0 * 0.1875)/1000;
    Voltage1 = (adc1 * 0.1875)/1000;
    Voltage2 = (adc2 * 0.1875)/1000;
    Voltage3 = (adc3 * 0.1875)/1000;

    Serial.print("AIN0: ");
    Serial.print(Voltage0, 7); // 7 = number of meaningful digits
    Serial.println("V");

    Serial.print("AIN1: ");
    Serial.print(Voltage1, 7);
    Serial.println("V");

    Serial.print("AIN2: ");
    Serial.print(Voltage2, 7);
    Serial.println("V");

    Serial.print("AIN3: ");
    Serial.print(Voltage3, 7);
    Serial.println("V");
    Serial.println(" ");
 //}
delay(1000);
}