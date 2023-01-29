#include <Arduino.h>

#include "button.hpp"

const int buttonPin = 8; 

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