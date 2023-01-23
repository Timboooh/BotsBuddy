#include <Arduino.h>
#include "accel.hpp"
#include "oled.hpp"

long lastUpdate = 0;
int counter = 21;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  OLED::setup();
  ACCEL::setup();
  Serial.println("setup done");
}

void loop() {
    long now = millis();
    if (trigger == true)
    {
        if(OLED::buttonPress() == false){
            if(now - lastUpdate > 1000){
                lastUpdate = now;
                OLED::startCountdown();
            }
        } else {
            counter = 21;
            trigger = false;
        }
    } else {
        ACCEL::update();
        Serial.println(trigger);   
    }
}   