#include <Wire.h>
#include <SPI.h>
#include "RTClib.h"
#include <sdcard.h>
 
RTC_DS1307 rtc;

void setup () {
  Serial.begin(115200);

  if (! rtc.begin()) { //Checking if RTC available
    Serial.println("Couldn't find RTC");
    while(1);
  }

  if(!SD.begin()){ //Checking if RTC available
    Serial.println("Card Mount Failed");
    while(1);
  }

  rtc.adjust(DateTime(F(__DATE__),F(__TIME__))); //Sets the time for the RTC. Should only be called once!!
}
 
void loop () {
  char buffer [1000];
  DateTime now = rtc.now();
     
  std::string name1="/data-", name2="-", month=itoa(now.month(),buffer,10), day=itoa(now.day(),buffer,10), year=itoa(now.year(),buffer,10), hour=itoa(now.hour(),buffer,10), minute=itoa(now.minute(),buffer,10), second=itoa(now.second(),buffer,10), name3=".txt"; //Creating a string for fullstring
  std::string filename = name1+month+name2+day+name2+year+name2+hour+name2+minute+name2+second+name3; //Putting the strings together
  const char *fullstring = filename.c_str(); //Converting from string to const char* in order to append file
  Serial.println(fullstring);

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
  delay(10000);
}