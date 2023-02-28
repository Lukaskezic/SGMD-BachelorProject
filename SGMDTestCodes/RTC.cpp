#include <Wire.h>
#include <SPI.h>
#include "RTClib.h"
#include "FS.h"
#include "SD.h"

uint64_t SleepTime = 6; //in hours
uint64_t convertTous = 1000000;

int s = 0;

RTC_DS1307 rtc;

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void SaveTimeStamp(){
  char buffer [1000];
  DateTime now = rtc.now();
  appendFile(SD, "/data.txt", itoa(now.year(),buffer,10));
  appendFile(SD, "/data.txt", "/");
  appendFile(SD, "/data.txt", itoa(now.month(),buffer,10));
  appendFile(SD, "/data.txt", "/");
  appendFile(SD, "/data.txt", itoa(now.day(),buffer,10));
  appendFile(SD, "/data.txt", " ");
  appendFile(SD, "/data.txt", itoa(now.hour(),buffer,10));
  appendFile(SD, "/data.txt", ":");
  appendFile(SD, "/data.txt", itoa(now.minute(),buffer,10));
  appendFile(SD, "/data.txt", ":");
  appendFile(SD, "/data.txt", itoa(now.second(),buffer,10));
  appendFile(SD, "/data.txt", "\n\n");
}

void setup () {
 
  Serial.begin(115200);
  Wire.begin(21,22);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if(!SD.begin()){
    Serial.println("Card Mount Failed");
    return;
  }
  
  SleepTime = SleepTime*60*60; //convering sleeptime from hours to seconds
}
 
void loop () {
    Serial.println("WOKE UP!! :)");
    SaveTimeStamp();
    esp_sleep_enable_timer_wakeup(5000000); //configuring timer wakeup
    esp_deep_sleep_start(); //setting ESP32 to sleep mode
}