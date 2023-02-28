#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219(0x40);

double current = 0;
float power_mW = 0;
int i = 0;

void setup() {

  ina219.begin();
  Serial.begin(115200);
  // ina219.setCalibration_32V_2A();    // set measurement range to 32V, 2A  (do not exceed 26V!)
  // ina219.setCalibration_32V_1A();    // set measurement range to 32V, 1A  (do not exceed 26V!)
  ina219.setCalibration_16V_400mA(); // set measurement range to 16V, 400mA
}

void loop() {
  current=ina219.getCurrent_mA();
  Serial.println(current,10);
}