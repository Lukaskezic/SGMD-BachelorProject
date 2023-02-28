#include "RTCModule.h"

#pragma once

#define TEMP_PIN 12 //Definér pin/pins

class TemperatureSensor
{
public:
    TemperatureSensor();
    void setupTemperatureSensor(RTCModule* r1);
	void receiveTemperatureSensorData();
    void saveTemperatureSensorData();
private:
    RTCModule* r1_;
    float tempC;
};