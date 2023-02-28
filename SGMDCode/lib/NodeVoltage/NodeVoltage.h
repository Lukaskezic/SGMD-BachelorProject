#pragma once

using namespace std;

class NodeVoltage
{
public:
    NodeVoltage();
    void setupNodeVoltage();
	void receiveNodeVoltage();
    void saveNodeVoltageData(int i, int j, double voltage0);
    double getNodeVoltage(int a, int b);
private:
    double adc = 0;
    double voltage = 0;
    int nodes = 4;  
    int ADCChannels[4] = {0,1,2,3};
    double measuredVoltage[4][4];
};