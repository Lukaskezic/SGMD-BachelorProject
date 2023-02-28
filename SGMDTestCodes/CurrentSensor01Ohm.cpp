#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219(0x40);

float shuntvoltage = 0;
float busvoltage = 0;
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
  //shuntvoltage = ina219.getShuntVoltage_mV();
  //busvoltage = ina219.getBusVoltage_V();
  current=0;
  //for(int i = 0; i<100;i++)
  //{
  //  current_mA+=ina219.getCurrent_mA();
  //}
  //current_mA = current_mA/100;
  double noisefloor = -0.000000225;

  for(int i = 0;i<100;i++)
  {
    current+=ina219.getCurrent_mA();
  }
  //double noisefloor = -0.00001989;
  current = current/100; //Mean value
  current = current/1000; //mA to A
  double resistor_factor = 2.0/0.1;
  current = current/resistor_factor;
  current = current/20;
  current = current-noisefloor; //Subtracking noisefloor
  //current_mA = current_mA/100;
  //power_mW = ina219.getPower_mW();

  Serial.println(current,10); //Printing current
}