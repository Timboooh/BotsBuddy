#include <Arduino.h>

#include "button.hpp"

const int buttonPin = 4; // D4 on esp8266

namespace BUTTON
{
    bool lastState = false;

    void setup()
    {
        pinMode(buttonPin, INPUT_PULLUP);
    }

    bool get_state()
    {
        bool buttonState = !digitalRead(buttonPin);
        if (buttonState && !lastState) {
            lastState = buttonState;
            return true;
        }
        lastState = buttonState;
        return false;
    }
}