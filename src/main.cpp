#include <Arduino.h>
#include "accel.hpp"
#include "oled.hpp"

long lastUpdate = 0;
int counter = 21;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //ACCEL::setup();
  OLED::setup();
}

void loop() {
    long now = millis();
    if (now - lastUpdate > 1000)
    {
        lastUpdate = now;
        //ACCEL::update();
        OLED::startCountdown();
    }
}