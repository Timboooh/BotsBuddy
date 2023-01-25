#include <Arduino.h>

#include "buzzer.hpp"

const int buzzerPin = 23;

namespace BUZZER
{
    void setup()
    {
        pinMode(buzzerPin, OUTPUT);
    }

    bool update()
    {
        long now = millis();
        if(millis()%1000 > 500){
            digitalWrite(buzzerPin, HIGH);
        }
        else {
            digitalWrite(buzzerPin, LOW);
        }
    }
}