#include "BluetoothSerial.h"
#include "Wire.h"
//#include "Adafruit_ADS1X15.h" // Search for Adafruit 1X15
#include "FS.h"
#include "SD.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// incase of MD5 error write this in CMD: python -m esptool --chip auto --port COM3 write_flash_status --non-volatile 0

//BluetoothSerial SerialBT;
//Adafruit_ADS1115 ads1115;	// Construct an ads1115
//Adafruit_ADS1115 ads(0x48)

unsigned long myTime;
unsigned long microseconds;
unsigned int sampling_period_us;
int SAMPLES = 1000;
int SAMPLING_FREQUENCY = 50;
double dataArray[1000];
int PIN = 26;
int PIN2 = 2;
int potPin = 35;

char dataStr[100] = "";
int i = 0;
int j = 0;
int k = 0;
// variable for storing the external adc
double Voltage0 = 0.0;

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

#define MAX_PRECISION	(10)
static const double rounders[MAX_PRECISION + 1] =
{
	0.5,				// 0
	0.05,				// 1
	0.005,				// 2
	0.0005,				// 3
	0.00005,			// 4
	0.000005,			// 5
	0.0000005,			// 6
	0.00000005,			// 7
	0.000000005,		// 8
	0.0000000005,		// 9
	0.00000000005		// 10
};

char * ftoa(double f, char * buf, int precision)
{
	char * ptr = buf;
	char * p = ptr;
	char * p1;
	char c;
	long intPart;

	// check precision bounds
	if (precision > MAX_PRECISION)
		precision = MAX_PRECISION;

	// sign stuff
	if (f < 0)
	{
		f = -f;
		*ptr++ = '-';
	}

	if (precision < 0)  // negative precision == automatic precision guess
	{
		if (f < 1.0) precision = 6;
		else if (f < 10.0) precision = 5;
		else if (f < 100.0) precision = 4;
		else if (f < 1000.0) precision = 3;
		else if (f < 10000.0) precision = 2;
		else if (f < 100000.0) precision = 1;
		else precision = 0;
	}

	// round value according the precision
	if (precision)
		f += rounders[precision];

	// integer part...
	intPart = f;
	f -= intPart;

	if (!intPart)
		*ptr++ = '0';
	else
	{
		// save start pointer
		p = ptr;

		// convert (reverse order)
		while (intPart)
		{
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}

		// save end pos
		p1 = p;

		// reverse result
		while (p > ptr)
		{
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}

		// restore end pos
		ptr = p1;
	}

	// decimal part
	if (precision)
	{
		// place decimal point
		*ptr++ = '.';

		// convert
		while (precision--)
		{
			f *= 10.0;
			c = f;
			*ptr++ = '0' + c;
			f -= c;
		}
	}

	// terminating zero
	*ptr = 0;

	return buf;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN, OUTPUT);
  analogReadResolution(12); //resolution between 9-12
  analogSetAttenuation(ADC_11db); //Attenuation (the range of measurement) between 0db - 11db (the lower the more precise, but lower max voltage)
  //SerialBT.begin("ESP32test2"); //Bluetooth device name
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
  digitalWrite(PIN, LOW); //gpio pin right above grnd on left side
  digitalWrite(PIN2, LOW);
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
  //ads1115.begin();  // Initialize ads1115   
 }

 

void loop() {
  dataStr[0] = 0; //Clean out string
  if (digitalRead(PIN2) == HIGH) //Read GPIO 12
  {
    char buffer [1000];
    for(i; i<SAMPLES; i++)
    {
      microseconds = micros(); //Overflows after around 70 minutes!

      //int16_t adc0;
 
      //adc0 = ads1115.readADC_SingleEnded(0);

      //Voltage0 = (adc0 * 0.1875)/1000;

      Voltage0 = analogReadMilliVolts(potPin);


      //SerialBT.print("AIN0: ");
      //Serial.println(Voltage0, 7); // 7 = number of meaningful digits
      //SerialBT.println("V");

      dataArray[i] = Voltage0;

      if(k == 0)
      {
        while(dataArray[i] != Voltage0){}
        digitalWrite(PIN, HIGH);
        k = 1;
      }

      while(micros() < (microseconds + sampling_period_us)){
      }
    }
    for(j;j<SAMPLES;j++)
    {
        appendFile(SD, "/data.txt", ftoa(dataArray[j],buffer,10));
        appendFile(SD, "/data.txt", "\n");
    }
 }
}