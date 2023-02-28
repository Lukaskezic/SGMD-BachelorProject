#include "RTCModule.h"
#include "iostream"
#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include "sdcard.h"
#include "ftoa.h"
#include <string.h>

RTC_DS1307 rtc;

RTCModule::RTCModule()
{
  
}

void RTCModule::setupRTCModule()
{
  Serial.begin(115200);
  delay(1000);

  if (! rtc.begin()) { //Checking if RTC available
    Serial.println("Couldn't find RTC");
    while(1);
  }

  if(!SD.begin()){ //Checking if RTC available
    Serial.println("Card Mount Failed");
    while(1);
  }
}

void RTCModule::receiveRTCModuleData()
{
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__))); //Sets the time for the RTC. Should only be called once!!

  saveRTCModuleData();
}

void RTCModule::saveRTCModuleData() //Function used to save RTC data on SD-card
{
  char buffer [1000];
  DateTime now = rtc.now(); //Reading RTC data containing the current date and time

  std::string name1="/data-", name2="-", month=itoa(now.month(),buffer,10), day=itoa(now.day(),buffer,10), year=itoa(now.year(),buffer,10), hour=itoa(now.hour(),buffer,10), minute=itoa(now.minute(),buffer,10), second=itoa(now.second(),buffer,10), name3=".txt"; //Creating a string for fullstring
  filename = name1+month+name2+day+name2+year+name2+hour+name2+minute+name2+second+name3; //Putting the strings together
  const char *fullstring = filename.c_str(); //Converting from string to const char* in order to append file
  appendFile(SD, fullstring, itoa(now.year(),buffer,10));
  appendFile(SD, fullstring, "/");
  appendFile(SD, fullstring, itoa(now.month(),buffer,10));
  appendFile(SD, fullstring, "/");
  appendFile(SD, fullstring, itoa(now.day(),buffer,10));
  appendFile(SD, fullstring, " ");
  appendFile(SD, fullstring, itoa(now.hour(),buffer,10));
  appendFile(SD, fullstring, ":");
  appendFile(SD, fullstring, itoa(now.minute(),buffer,10));
  appendFile(SD, fullstring, ":");
  appendFile(SD, fullstring, itoa(now.second(),buffer,10));
  appendFile(SD, fullstring, "\n\n");
}

std::string RTCModule::getFileName() //returning a string that contains the fullstring
{
  return filename;
}