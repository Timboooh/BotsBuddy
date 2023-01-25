#ifndef A9G_H
#define A9G_H

#include <Arduino.h>

namespace A9G
{
    void setup();
    void update();

    /**
     * true if gps has a sattelite fix
     *
     * @return true gps has fix
     * @return false gps has no fix
     */
    bool gps_getHasfix();

    /**
     * true if a9g has reported readyness
    */
    bool is_init();

    /**
     * Get the url of the current location
     * Gives last known location if no fix is available
     *
     * @return String google maps url of last known location
     */
    String gps_getLocationURL();

    /**
     * Soft reset the A9G chip
     */
    void reset();

    /**
     * Set the indication mode of the status LEDs
     * Modes:
     *      0: Flash when unavailable, off when available
     *      1: Solid when unavailable, off when available
     */
    void status_indication_mode(int mode);

    /**
     * Update the on/off status of the GPS
     */
    void gps_status();

    /**
     * Enable or disable the GPS
     * @param enable Whether to enable or disable the GPS
     */
    void gps_enable(bool enable);

    /**
     * Update the location every n seconds
     *
     * @param interval_sec time in seconds between every update
     */
    void gps_update_interval(int interval_sec);

    /**
     * Update the local clock based on the GSM time from the A9g.
     */
    void time_update();

    /**
     * Send a SMS message to a phonenumber
     * @param phonenumber The phonenumber to send the SMS message to
     * @param message The message to send
     */
    void sms_send(const char *phonenumber, String message);

    /**
     * Start a call to a phonenumber
     * @param phonenumber The phonenumber to start the call with
     */
    void call_send(const char *phonenumber);

    /**
     * Initialize the GSM to be able to send SMS messages and make calls
     */
    void gsm_init();

    /**
     * Update the status of the SIM card
     */
    void sim_update();
} // namespace A9G

#endif