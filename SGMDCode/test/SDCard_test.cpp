#include "DCResistivity.h"
#include "RTCModule.h"
#include "TemperatureSensor.h"
#include "CurrentSensor.h"
#include "NodeVoltage.h"
#include <Arduino.h>
#include <SPI.h>
#include "iostream"
#include "sdcard.h"
#include <RTClib.h>

using namespace std;

enum programState // Definition af states i state machine
{
  receiveRTCModule,
  SDCardNameChange,
  ESP32Sleep
};

programState state = receiveRTCModule; // start state

void programStateSwitch(programState &state);

RTCModule         *       r1    = new RTCModule();

void setup()
{
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
    r1->receiveRTCModule();
    state = SDCardNameChange;
  }
  break;
  
  case SDCardNameChange:
  {
    RTCModule rtc1;
    RTC_DS1307 rtc;
    DateTime now = rtc.now();
    char buffer [1000];
    appendFile(SD, rtc1.getFileName(), itoa(now.year(),buffer,10));
  }

  case ESP32Sleep:
  {
    state = receiveRTCModule;
    esp_sleep_enable_timer_wakeup(5000000); //configuring timer wakeup
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