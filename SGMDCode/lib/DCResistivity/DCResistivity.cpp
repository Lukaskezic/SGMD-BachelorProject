#include "DCResistivity.h"
#include "CurrentSensor.h"
#include "NodeVoltage.h"
#include "RTCModule.h"
#include <iostream>
#include <string>
#include <ftoa.h>
#include <sdcard.h>

using namespace std;

DCResistivity::DCResistivity()
{
}

void DCResistivity::setupDCResistivity(CurrentSensor* i1, NodeVoltage* v1, RTCModule* r1)
{
  i1_ = i1;
  v1_ = v1;
  r1_ = r1;
}

void DCResistivity::calculateDCResistivity()
{
  int i = 0;
  int j = 0;
  
  for(i; i<nodes;i++) //Kolonner
    {
      j = i + 1;
      for(j; j<nodes;j++) //Node
      {
        double voltage = v1_->getNodeVoltage(i,j);
        double current = i1_->getCurrent();
        double Resistance = voltage/current;
        int n = i+1;
        int m = nodes-(j);
        double a = 0.20; //Distance between electrodes
        double k_factor = 2*3.14159*n*(n+1)*m*(m+1)*((a)/(n*(n+1)+m*(m+1)));
        Resistivity = k_factor*Resistance;
        saveDCResistivityData(i,j,Resistivity);
      }
    }
}

void DCResistivity::saveDCResistivityData(int i, int j, float Resistivity)
{
  char buffer [1000];
  std::string hi = r1_->getFileName();
  const char* filename = hi.c_str();
  appendFile(SD, filename, "\nResistivity: \n\n");
  appendFile(SD, filename, itoa(i,buffer,10));
  appendFile(SD, filename, "->");
  appendFile(SD, filename, itoa(j,buffer,10));
  appendFile(SD, filename, "\n");
  appendFile(SD, filename, ftoa(Resistivity,buffer,10));
  appendFile(SD, filename, "\n");
}