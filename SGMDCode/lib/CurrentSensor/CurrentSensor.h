#pragma once

 //Definér pin/pins
 
 using namespace std;

class CurrentSensor
{
public:
    CurrentSensor();
    void setupCurrentSensor();
	void receiveCurrentSensorData();
    static void saveCurrentSensorData(double current);
    double getCurrent();
private:
    double current = 0;
};