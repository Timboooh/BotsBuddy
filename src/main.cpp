#include <Arduino.h>
#include "contacts.h"
#include "a9g.h"

void setup()
{
  delay(5000);
  Serial.begin(115200);
  Serial.println("Hello world!");
  
  A9G::setup();
}

void loop() {
  
  A9G::update();

void loop()
{
  // Serial.println(CONTACTS::getNextContact());
  A9G::update();
  delay(100);
