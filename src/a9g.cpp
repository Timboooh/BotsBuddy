#include "a9g.h"

#include <Arduino.h>

#define RESPONSE_BUF_CMD_SIZE 16
#define RESPONSE_BUF_ARGS_SIZE 64
namespace A9G
{
    struct Response
    {
        char cmd[RESPONSE_BUF_CMD_SIZE];
        char args[RESPONSE_BUF_ARGS_SIZE];
        int args_size;
    };

    bool is_initialized = false;

    void sendCommand(const char *fmt, ...)
    {
        char s[256];
        va_list arg;

        va_start(arg, fmt);
        vsprintf(s, fmt, arg);

        Serial2.print(s);
        Serial2.println();

        Serial.println("Sent AT command:");
        Serial.print("\t ");
        Serial.print(s);
        Serial.println();

        va_end(arg);

        delay(5);
    }

    void parseCommand(struct Response response)
    {
        Serial.println("Got response: ");
        Serial.print("  cmd:\t ");
        Serial.println(response.cmd);
        Serial.print("  args:\t ");
        Serial.println(response.args);
    }

    void setup()
    {
        // The A9G chip is connected to UART2 of the microcontroller.
        // And uses a baudrate of 115200
        Serial2.begin(115200);

        reset();
    }

    void post_setup()
    {
        Serial.println("A9G Ready");

        gps_enable(true);
        status_indication_mode(1);

        gsm_init();
        sim_update();

        is_initialized = true;
    }

    long lastUpdate = 0;

    void update()
    {

        // Ask the time every 5 seconds
        long now = millis();
        if (is_initialized && now - lastUpdate > 5000)
        {
            lastUpdate = now;

            gps_update();
        }

        // Parse the UART data coming back from the A9g
        while (Serial2.available())
        {
            // Start with reading entire lines at once
            // If haven´t received the full string yet, we wait until Serial2.getTimeout() has been reached.
            // This defaults to 1000ms.
            String rx = Serial2.readStringUntil('\n');
            rx.trim(); // get rid of messy spaces or carriage returns

            // Serial.println("A9G: " + rx);

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

    void reset() { sendCommand("AT+RST=1"); }
    void status_indication_mode(int mode) { sendCommand("AT+GPNT=%d", mode); }
    void gps_enable(bool enable) { sendCommand("AT+GPS=%d", enable); }
    void gps_update() { sendCommand("AT+LOCATION=2"); }
    void time_update() { sendCommand("AT+CCLK?"); }
    void gsm_init()
    {
        sendCommand("AT+CMGF=1");
        sendCommand("AT+CSMP=17,167,0,0");
    }
    void sms_send(const char *phonenumber, const char *message)
    {
        sendCommand("AT+CMGS=\"%s\"", phonenumber);
        sendCommand("%s%c", message, 26);
    }
    void call_send(const char *phonenumber) { sendCommand("ATD%s", phonenumber); }
    void sim_update() { sendCommand("AT+CPIN?"); }

} // namespace A9G
