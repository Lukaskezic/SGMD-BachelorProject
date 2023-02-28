#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219(0x40);

float shuntvoltage = 0;
float busvoltage = 0;
double current_mA = 0;
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
  //shuntvoltage = ina219.getShuntVoltage_mV();
  //busvoltage = ina219.getBusVoltage_V();
  current_mA=ina219.getCurrent_mA();
  //current_mA = current_mA/100;
  //power_mW = ina219.getPower_mW();

  Serial.println(current_mA,10);
  current_mA = 0;
}