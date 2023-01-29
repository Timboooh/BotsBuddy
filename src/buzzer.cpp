#include <Arduino.h>

#include "buzzer.hpp"

const int buzzerPin = 2;

namespace BUZZER
{
    bool state = false;

    void setup()
    {
        pinMode(buzzerPin, OUTPUT);

        digitalWrite(buzzerPin, HIGH);
    }

    void update()
    {
        if (state)
        {
            long now = millis();
            if (millis() % 1000 > 750)
            {
                digitalWrite(buzzerPin, LOW);
            }
            else
            {
                digitalWrite(buzzerPin, HIGH);
            }
        }
        else
        {
            digitalWrite(buzzerPin, HIGH);
        }
    }

    void set(bool on)
    {
        state = on;
    }
}