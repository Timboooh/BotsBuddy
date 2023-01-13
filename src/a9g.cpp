#include "a9g.h"

#include <Arduino.h>

namespace A9G
{
    long lastUpdate = 0;

    void setup()
    {
        Serial2.begin(115200);

        Serial.print("Serial timeout: ");
        Serial.println(Serial2.getTimeout());
    }

    void update()
    {
        long now = millis();
        if (now - lastUpdate > 5000)
        {
            lastUpdate = now;

            Serial2.println("AT+CCLK?");
        }


        while (Serial2.available())
        {   
            String rx = Serial2.readStringUntil('\n');
                   
            if (rx[0] == '+'){
                Serial.println("Got response!");
                Serial.println(rx);
            }

        }
        
    }
} // namespace A9G
