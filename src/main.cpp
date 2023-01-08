#include <Arduino.h>
#include "accel.hpp"

long lastUpdate = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ACCEL::setup();
}

void loop() {
    long now = millis();
    if (now - lastUpdate > 2000)
    {
        lastUpdate = now;
        ACCEL::update();
    }
}