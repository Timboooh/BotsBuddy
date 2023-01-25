#include <Arduino.h>

#include "button.hpp"

const int buttonPin = 2; // D4 on esp8266

bool buttonState = false;
namespace BUTTON
{
    void setup()
    {
        pinMode(buttonPin, INPUT_PULLUP);
    }

    bool update()
    {
        buttonState = digitalRead(buttonPin);
        Serial.print("button");
        Serial.println(!buttonState);
        return (!buttonState);
    }
}