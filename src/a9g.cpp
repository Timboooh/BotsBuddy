#include "a9g.h"

#include <Arduino.h>

#define RESPONSE_BUF_CMD_SIZE 16
#define RESPONSE_BUF_ARGS_SIZE 256
namespace A9G
{
    struct Response
    {
        char cmd[RESPONSE_BUF_CMD_SIZE];
        char args[RESPONSE_BUF_ARGS_SIZE];
    };

    bool is_initialized = false;
    bool gps_hasfix = false;
    bool in_call = false;
    bool is_ringing = false;

    String location = "https://www.google.com/maps/place/51.9172,%204.4842";

    void sendCommand(const char *fmt, ...)
    {
        char s[256];
        va_list arg;

        va_start(arg, fmt);
        vsprintf(s, fmt, arg);

        Serial1.print(s);

        Serial1.println();

        Serial.println("Sent AT command:");
        Serial.print("\t ");
        Serial.print(s);
        Serial.println();

        va_end(arg);

        delay(50);
    }

    void parseCommand(struct Response response)
    {
        Serial.println("Got response: ");
        Serial.print("  cmd:\t ");
        Serial.println(response.cmd);
        Serial.print("  args:\t ");
        Serial.println(response.args);

        if (!strcmp(response.cmd, "GPSRD"))
        { // $GNGGA,000025.029,5133.4337,N,00431.0019,E,0,0,,6.2,M,47.2,M,,*5D
            String args = response.args;
            auto latitudeStr = args.substring(18, 20) + "%20" + args.substring(20, 27);
            auto longitudeStr = args.substring(30, 33) + "%20" + args.substring(33, 40);
            gps_hasfix = (args.charAt(43) != '0');

            location = "https://www.google.com/maps/place/" + latitudeStr + ",%20" + longitudeStr;
            Serial.println("Location: " + location);
            Serial.printf("Has Fix: %d\r\n\r\n", gps_hasfix);
        }

        if (!strcmp(response.cmd, "CIEV"))
        {
            if (!strcmp(response.args, " \"SOUNDER\",1"))
                Serial.println("A9G: Sounder 1");
            else if (!strcmp(response.args, " \"SOUNDER\",0"))
                Serial.println("A9G: Sounder 0");
            else if (!strcmp(response.args, " \"CALL\",1"))
                Serial.println("A9G: Call 1");
            else if (!strcmp(response.args, " \"CALL\",0"))
                Serial.println("A9G: Call 0");
        }
    }

    void setup()
    {
        // The A9G chip is connected to UART2 of the microcontroller.
        // And uses a baudrate of 115200
        Serial1.begin(115200, SERIAL_8N1, 2, 3);

        reset();
    }

    void post_setup()
    {
        Serial.println("A9G Ready");

        gps_enable(true);
        status_indication_mode(1);

        gsm_init();
        sim_update();

        gps_update_interval(10);

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
        }

        // Parse the UART data coming back from the A9g
        while (Serial1.available())
        {
            // Start with reading entire lines at once
            // If haven??t received the full string yet, we wait until Serial1.getTimeout() has been reached.
            // This defaults to 1000ms.
            String rx = Serial1.readStringUntil('\n');
            rx.trim(); // get rid of messy spaces or carriage returns

            // Serial.println("A9G: " + rx);

            // Some settings need to be set after the A9G has fully initialized.
            // We wait until the A9G reports ready and call the post_setup
            if (rx == "READY")
                post_setup();

            // Every command response starts with: '+'.
            // And separates the command and return arguments with ": "
            // Example: +CCLK: "23/01/13,12:17:55+01"
            if (rx.charAt(0) == '+' || rx.charAt(0) == '$')
            {
                struct Response response;

                // Find index of the command / argument separator
                int _divLocation = 0;
                if (rx.charAt(0) == '+')
                    _divLocation = rx.indexOf(':');
                else if (rx.charAt(0) == '$')
                    _divLocation = rx.indexOf(',');

                // Parse the full received string into the command and args
                rx.toCharArray(response.cmd, min(_divLocation, RESPONSE_BUF_CMD_SIZE), 1); // Get the cmd
                rx.toCharArray(response.args, RESPONSE_BUF_ARGS_SIZE, _divLocation + 1);   // Get the args

                // Pass the response to the parser so we can actually do something with it.
                parseCommand(response);
            }
        }
    }

    bool is_init() { return is_initialized; }
    bool gps_getHasfix() { return gps_hasfix; }
    String gps_getLocationURL() { return location; }

    void reset() { sendCommand("AT+RST=1"); }
    void status_indication_mode(int mode) { sendCommand("AT+GPNT=%d", mode); }
    void gps_enable(bool enable) { sendCommand("AT+GPS=%d", enable); }
    void gps_update_interval(int interval_sec) { sendCommand("AT+GPSRD=%d", interval_sec); }
    void time_update() { sendCommand("AT+CCLK?"); }
    void gsm_init()
    {
        sendCommand("AT+CMGF=1");
        sendCommand("AT+CSMP=17,167,0,0");
    }
    void sms_send(const char *phonenumber, String message)
    {
        Serial.println("SMS has been sent!");
        Serial.println(message);

        char messageBuf[256];
        message.toCharArray(messageBuf, 256);

        sendCommand("AT+CMGS=\"%s\"", phonenumber);
        sendCommand("%s%c", messageBuf, 26);
        Serial.println(messageBuf);
        delay(200);
    }
    void call_send(const char *phonenumber) { sendCommand("ATD%s", phonenumber); }
    void sim_update() { sendCommand("AT+CPIN?"); }

} // namespace A9G
