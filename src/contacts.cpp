#include "contacts.h"

#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>

namespace CONTACTS
{
    DynamicJsonDocument doc(1024);
    JsonArray contacts;

    void setup()
    {
        if (!SD.begin(5))
        {
            Serial.println("Card Mount Failed");
            return;
        }

        uint8_t cardType = SD.cardType();

        if (cardType == CARD_NONE)
        {
            Serial.println("No SD card attached");
            return;
        }

        File file = SD.open("/settings.json");
        if (!file)
        {
            Serial.println("Failed to open file for reading");
            return;
        }

        deserializeJson(doc, file);
        contacts = doc["contacts"].as<JsonArray>();

        file.close();
    }

    const char *getContact(int index)
    {
        return contacts[index].as<const char *>();
    }

    int getContactCount(){
        return contacts.size();
    }

    int currentContact = 0;
    const char *getNextContact() {
        if (currentContact >= contacts.size())
            return "112";
        auto contact = getContact(currentContact);
        currentContact++;
        return contact;
    }
} // namespace CONTACTS
