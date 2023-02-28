#include <Arduino.h>

using namespace std;

void setup() {

}

void loop() {
    esp_sleep_enable_timer_wakeup(10000000); //configuring timer wakeup
    esp_deep_sleep_start(); //setting ESP32 to sleep mode
}