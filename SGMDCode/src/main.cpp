#include "DCResistivity.h"
#include "RTCModule.h"
#include "TemperatureSensor.h"
#include "CurrentSensor.h"
#include "NodeVoltage.h"
#include <Arduino.h>

using namespace std;

#define transistor_pin 13

enum programState // Definition of states in state machine
{
  receiveRTCModule,
  receiveTemperatureSensor,
  receiveCurrentSensor,
  receiveNodeVoltage,
  calculateDCResistivity,
  ESP32Sleep,
};

programState state = receiveRTCModule; // start state

void programStateSwitch(programState &state);

RTCModule         *       r1    = new RTCModule();
TemperatureSensor *       t1    = new TemperatureSensor();
CurrentSensor     *       i1    = new CurrentSensor();
NodeVoltage       *       v1    = new NodeVoltage();
DCResistivity     *       c1    = new DCResistivity();

void setup()
{
  pinMode(transistor_pin,OUTPUT);
}

void loop()
{
  programStateSwitch(state);
}

//State maskine
void programStateSwitch(programState &state)
{
  switch (state)
  {
  case receiveRTCModule:
  {
    digitalWrite(transistor_pin,HIGH);
    r1->setupRTCModule();
    r1->receiveRTCModuleData();
    state = receiveTemperatureSensor;
  }
  break;
  
  case receiveTemperatureSensor:
  {
    t1->setupTemperatureSensor(r1);
    t1->receiveTemperatureSensorData();
    t1->saveTemperatureSensorData();
    state = receiveCurrentSensor;
  }
  break;
  
  case receiveCurrentSensor:
  {
    i1->setupCurrentSensor();
    i1->receiveCurrentSensorData();
    state = receiveNodeVoltage;
  }
  break;
  
  case receiveNodeVoltage:
  {
    v1->setupNodeVoltage();
    v1->receiveNodeVoltage();
    state = calculateDCResistivity;
  }
  break;
  
  case calculateDCResistivity:
  {
    c1->setupDCResistivity(i1,v1,r1);
    c1->calculateDCResistivity();
    state = ESP32Sleep;
  }
  break;

  case ESP32Sleep:
  {
    digitalWrite(transistor_pin,LOW);
    state = receiveRTCModule;
    esp_sleep_enable_timer_wakeup(10000000); //configuring timer wakeup 10 sek
    esp_deep_sleep_start(); //setting ESP32 to sleep mode
  }
  break;

  default:
  {
    // nothing
  }
  break;
  }
}