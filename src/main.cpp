#include <Arduino.h>

#include "accel.hpp"
#include "oled.hpp"
#include "a9g.h"

long lastUpdate = 0;

int state = 0;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    OLED::setup();
    ACCEL::setup();
    A9G::setup();
    Serial.println("setup done");

}

void loop()
{
    long now = millis();

    if (now - lastUpdate > 500)
    {
        lastUpdate = now;
        Serial.printf("state: %d \r\n", state);
    }
    switch (state)
    {
    case 0:
    {
        A9G::update();
        if (A9G::is_init()){
            state = 1;
        }
        break;
    }
    case 1:
    {
        ACCEL::update();
        A9G::update();
        OLED::update();

        if (trigger) {
            counter = 21;
            state = 2;
        }

        break;
    }
    case 2:
    {
        if (trigger)
            OLED::update();
        if (counter <= 0) {
            state = 3;
        }
        break;
    }
    case 3:
    {
        String locationURL = A9G::gps_getLocationURL();
        String message = "EMERGENCY ALERT MESSAGE\r\nThis person has been in an collision.\r\n" + locationURL;
        
        A9G::sms_send("+31623379877", message);

        state = 1;
        break;
    }
    default:
    {
        break;
    }
    }

    // long now = millis();
    // if (trigger == true)
    // {
    //     if(OLED::buttonPress() == false){
    //         if(now - lastUpdate > 1000){
    //             lastUpdate = now;
    //             OLED::startCountdown();
    //         }
    //     } else {
    //         counter = 21;
    //         trigger = false;
    //     }
    // } else {
    //     ACCEL::update();
    //     Serial.println(trigger);
    // }
}
