#include <Arduino.h>
#include "esp_adc_cal.h"

int i = 0;
int j = 0;
int nodes = 7;

int ADCChannels[7] = {32,25,26,27,14,12,13};
int measuredADCValue[7][7];
int measuredVoltage[7][7];

void setup()
{
  Serial.begin(115200);
}

void loop() 
{
  for(i; i<7;i++) //Kolonner
  {
    j = i + 1;
    for(j; j<nodes;j++) //Node
    {
      /*Serial.print("Node\n");
      Serial.print(i);
      Serial.print(j);
      Serial.print("\n");*/
      measuredADCValue[i][j] = (analogRead(ADCChannels[i]) - analogRead(ADCChannels[j]));
      measuredVoltage[i][j] = (analogReadMilliVolts(ADCChannels[i]) - analogReadMilliVolts(ADCChannels[j]));
      Serial.print(measuredVoltage[i][j]);
      Serial.print("       --------------->       ");
      Serial.print(i);
      Serial.print(j);
      Serial.print("\n");
      delay(1000);
    }
  }
  /*
  ADC_VALUE = analogRead(Analog_channel_pin);
  Serial.print("ADC VALUE = ");
  Serial.println(ADC_VALUE);
  delay(1000);
  voltage_value = (ADC_V0pALUE * 3.3 ) / (4095);
  Serial.print("Voltage = ");
  Serial.print(voltage_value,3); 
  Serial.print("volts");
  delay(1000);
  */
}