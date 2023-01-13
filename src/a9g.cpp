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
        while (!Serial2.availableForWrite())
        { /* wait until we can write*/
        }
        Serial2.println("AT+" + command);
        Serial2.flush();
        delay(5); // tiny delay to allow the A9G to process the command
    }

    template <typename T>
    String getString(T arg)
    {
        if (std::is_same(arg, bool)::value) {
            return "bool";
        } else if (std::is_same(arg, int)::value) {
            return "int";
        } else if (std::is_same(arg, String)::value) {
            return "String";
        } 
        return "";
    }

    template <typename T, typename U, typename V, typename W>
    void sendCommand_2(const String &command, T arg1, U arg2, V arg3)
    {
        Serial.printf("arg1: %s\n", getString(arg1));
        Serial.printf("arg2: %s\n", getString(arg2));
        Serial.printf("arg3: %s\n", getString(arg3));
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

        sendCommand_2("yo", "a", 1, false);

        sendCommand("RST=1");
    }

    void post_setup()
    {
        Serial.println("A9G Ready");
        sendCommand("GPS=1");
        sendCommand("GPNT=1");
    }

    long lastUpdate = 0;

    void update()
    {

        // Ask the time every 5 seconds
        long now = millis();
        if (now - lastUpdate > 5000)
        {
            lastUpdate = now;

            sendCommand("GPS?");
        }

        while (Serial2.available())
        {
            // Start with reading entire lines at once
            // If haven´t received the full string yet, we wait until Serial2.getTimeout() has been reached.
            // This defaults to 1000ms.
            String rx = Serial2.readStringUntil('\n');
            rx.trim(); // get rid of messy spaces or carriage returns

            // Some settings need to be set after the A9G has fully initialized.
            // We wait until the A9G reports ready and call the post_setup
            if (rx == "READY")
                post_setup();

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
                rx.toCharArray(response.args, RESPONSE_BUF_ARGS_SIZE, _divLocation + 2);   // Get the args

                // Pass the response to the parser so we can actually do something with it.
                parseCommand(response);
            }
        }
    }
} // namespace A9G
