#include "sdcard.h"
#include <Arduino.h>  // must include this for pinMode, digitalWrite, HIGH, OUTPUT
#include <SPI.h>
#include <SD.h>
#include <Preferences.h> // if using Preferences

#define SDCardCS    18
#define SDCardCLK   35
#define SDCardMISO  12
#define SDCardMOSI  16

void setupSDCard() 
{
    // Initialize SPI bus for SD card
    SPI.begin(SDCardCLK, SDCardMISO, SDCardMOSI, SDCardCS);

    // Set pin modes
    pinMode(SDCardMOSI, OUTPUT);
    pinMode(SDCardMISO, INPUT);
    pinMode(SDCardCLK, OUTPUT);
    pinMode(SDCardCS, OUTPUT);

    // Initialize SD card
    if (!SD.begin(SDCardCS)) {
        Serial.println("ERROR: SD card initialization failed");
    } else {
        Serial.println("SD card ready");
    }
}

void dataLogSDCard(const char *logFile, const char *data) 
{
    File file = SD.open(logFile, FILE_APPEND);

    if (!file) {
        Serial.println("ERROR: Unable to open log file");
        return;
    }

    file.println(data);
    file.close();
}
