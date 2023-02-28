/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

using namespace std;

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 35;
int i = 0;
int j = 0;

unsigned long myTime;
unsigned long microseconds;
unsigned int sampling_period_us;
int SAMPLES = 1000;
int SAMPLING_FREQUENCY = 50;
char dataArray[1000];

char dataStr[100] = "";

// variable for storing the potentiometer value
double potValue = 0;

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

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

void setup(){

    Serial.begin(115200);
    delay(1000);
    analogReadResolution(12);
    sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
    digitalWrite(12, LOW);
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }

    //createDir(SD, "/mydir");
    writeFile(SD, "/data.csv", "Hello");
    readFile(SD, "/data.csv");
}

void loop(){
    dataStr[0] = 0; //clean out string
    // Reading potentiometer value
    //if(digitalRead(12) == HIGH)
    //{

        char buffer [1000];
        for(i; i<SAMPLES; i++)
        {
            microseconds = micros();    //Overflows after around 70 minutes!

            potValue = analogReadMilliVolts(potPin);
            dataArray[i] = potValue;

            while(micros() < (microseconds + sampling_period_us)){
            }
        }

        for(j;j<SAMPLES;j++)
        {
            appendFile(SD, "/data.csv", sprintf(dataArray[j],"%f",);
            appendFile(SD, "/data.csv", "\n");
            
        }
    //}
}