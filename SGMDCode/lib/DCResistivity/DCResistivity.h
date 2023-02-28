#pragma once
#include "NodeVoltage.h"
#include "CurrentSensor.h"
#include "RTCModule.h"

using namespace std;

class DCResistivity
{
public:
    DCResistivity();
    void setupDCResistivity(CurrentSensor* i1,NodeVoltage* v1,RTCModule* r1);
	void calculateDCResistivity();
    void saveDCResistivityData(int i, int j, float Resistivity);
private:
    double Resistivity;
    //CurrentSensor* i1_;
    CurrentSensor* i1_;
    NodeVoltage* v1_;
    RTCModule* r1_;
    int nodes = 4;
};