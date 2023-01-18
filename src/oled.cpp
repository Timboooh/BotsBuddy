
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "oled.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned long previousMillis = 0;
unsigned long interval = 1000;


const int buttonPin = 4;

int buttonState = 0;

namespace OLED
{
    void setup() {
        pinMode(LED_BUILTIN, OUTPUT);
        pinMode(buttonPin, INPUT);

        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
            Serial.println(F("SSD1306 allocation failed"));
            for (;;);
        }
        delay(2000);
        display.clearDisplay();
        display.setTextSize(6);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.println("GO");
        display.display();
    }



    void startCountdown(){
        if(counter > 0){
            digitalWrite(LED_BUILTIN, LOW);
            display.clearDisplay();
            display.setCursor(5, 5);
            counter--;
            display.println(counter);
            display.display();
            Serial.println(counter);
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }
}