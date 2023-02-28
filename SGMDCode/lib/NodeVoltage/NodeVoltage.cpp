#include "NodeVoltage.h"
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include "sdcard.h"
#include "ftoa.h"
#include "SD.h"
#include "FS.h"

using namespace std;

Adafruit_ADS1115 ads1115;

NodeVoltage::NodeVoltage()
{

}

void NodeVoltage::setupNodeVoltage()
{
  ads1115.begin();
  ads1115.setDataRate(128);
  Serial.begin(115200);
}

void NodeVoltage::receiveNodeVoltage()
{
  int i = 0;
  int j = 0;
  
  for(i; i<nodes;i++) //Start electrodes
  {
    j = i + 1;
    for(j; j<nodes;j++) //Electrodes
    {
      for(int a = 0; a<100;a++)
      {
      adc += (ads1115.readADC_SingleEnded(ADCChannels[i])-ads1115.readADC_SingleEnded(ADCChannels[j])); //Calculating voltage difference between electrodes
      }
      adc = adc/100; //Taking average of ADC values
      voltage = (adc * 0.1875)/1000; //converting ADC value to voltage
      measuredVoltage[i][j] = voltage; //Passing voltage value to array
      adc = 0; //Resetting ADC value
    }
  }
}

void NodeVoltage::saveNodeVoltageData(int i, int j, double voltage0)
{
  char buffer [1000];
  appendFile(SD, "/data.txt", itoa(i,buffer,10));
  appendFile(SD, "/data.txt", "->");
  appendFile(SD, "/data.txt", itoa(j,buffer,10));
  appendFile(SD, "/data.txt", "\n");
  appendFile(SD, "/data.txt", ftoa(voltage0,buffer,10));
  appendFile(SD, "/data.txt", "\n");
}

double NodeVoltage::getNodeVoltage(int a, int b) //Function used to get measured voltage difference between given electrodes
{
  return measuredVoltage[a][b]; //returning voltage difference between given electrodes
}