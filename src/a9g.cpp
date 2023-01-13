#include "a9g.h"

#include <Arduino.h>

#define RESPONSE_BUF_CMD_SIZE 8
#define RESPONSE_BUF_ARGS_SIZE 64
namespace A9G
{
    struct Response
    {
        char cmd[RESPONSE_BUF_CMD_SIZE];
        char args[RESPONSE_BUF_ARGS_SIZE];
        int args_size;
    };

    void sendCommand(const String &command)
    {
        if (Serial2.availableForWrite())
        {
            Serial2.println("AT+" + command);
        }
    }

    void parseCommand(struct Response response)
    {
        Serial.println("Got response!");
        Serial.println(response.cmd);
        Serial.println(response.args);
    }

    void setup()
    {
        // The A9G chip is connected to UART2 of the microcontroller.
        // And uses a baudrate of 115200
        Serial2.begin(115200);
    }

    long lastUpdate = 0;

    void update()
    {   

        // Ask the time every 5 seconds
        long now = millis();
        if (now - lastUpdate > 5000)
        {
            lastUpdate = now;

            sendCommand("CCLK?");
        }
        
        while (Serial2.available())
        {
            // Start with reading entire lines at once
            // If haven´t received the full string yet, we wait until Serial2.getTimeout() has been reached. 
            // This defaults to 1000ms. 
            String rx = Serial2.readStringUntil('\n');
            rx.trim(); // get rid of messy spaces or carriage returns

            // Every command response starts with: '+'. 
            // And separates the command and return arguments with ": "
            // Example: +CCLK: "23/01/13,12:17:55+01"
            if (rx.charAt(0) == '+')
            {   
                struct Response response;

                // Find index of the command / argument separator
                int _divLocation = rx.indexOf(':');

                // Parse the full received string into the command and args
                rx.toCharArray(response.cmd, min(_divLocation, RESPONSE_BUF_CMD_SIZE), 1); // Get the cmd
                rx.toCharArray(response.args, RESPONSE_BUF_ARGS_SIZE, _divLocation + 2); // Get the args

                // Pass the response to the parser so we can actually do something with it.
                parseCommand(response);
            }
        }
    }
} // namespace A9G
