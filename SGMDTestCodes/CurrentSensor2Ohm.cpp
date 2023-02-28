#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219(0x40);

double current = 0;

void setup() {

  ina219.begin();
  Serial.begin(115200);
  ina219.setCalibration_16V_400mA(); // set measurement range to 16V, 400mA
}

void loop() {
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
  Serial.println(current,10);
  current = 0;
}