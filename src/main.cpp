#include <Arduino.h>

#include "accel.hpp"
#include "oled.hpp"
#include "a9g.h"
#include "button.hpp"
#include "buzzer.hpp"

long lastUpdate = 0;

int state = 0;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    BUZZER::setup();
    for (size_t i = 0; i < 100; i++)
    {
        Serial.println("Hello, world!");
        delay(100);
    }
    

    OLED::setup();
    ACCEL::setup();
    A9G::setup();
    BUTTON::setup();
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

        if (BUTTON::get_state()) trigger = true;

        if (trigger) {
            counter = 21;
            state = 2;
            BUZZER::set(true);
        }

        break;
    }
    case 2:
    {
        if (trigger)
            OLED::update();
        if (counter <= 0) {
            trigger = false;
            state = 3;
            BUZZER::set(false);

        }
        if (BUTTON::get_state()) {
            counter = 0;
            state = 1;
            trigger = false;
            BUZZER::set(false);
        }
        BUZZER::update();
        break;
    }
    case 3:
    {
        String locationURL = A9G::gps_getLocationURL();
        String message = "EMERGENCY ALERT MESSAGE\r\nThis person has been in an collision.\r\n" + locationURL;
        
        A9G::sms_send("+31623379877", message);
        A9G::call_send("+31623379877");

        state = 1;
        break;
    }
    default:
    {
        break;
    }
    }
    delay(10);
}
