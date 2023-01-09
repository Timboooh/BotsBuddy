#include <Arduino.h>
#include "contacts.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Hello world!");
  
  CONTACTS::setup();
}

void loop() {
  Serial.println(CONTACTS::getNextContact());
  delay(5000);

}