/*
 * Program to control ARC-S SD Card logging
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 11, 2025
 * Edited:    January 15, 2025
 */
#include "sdcard.h"
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SDCardCS    18
#define SDCardCLK   35
#define SDCardMISO  12
#define SDCardMOSI  16

void setupSDCard()
{
    // Initialize SPI bus for SD card
    SPI.begin(SDCardCLK, SDCardMISO, SDCardMOSI, SDCardCS);

    // Initialize SD card
    if (!SD.begin(SDCardCS))
    {
        Serial.println("ERROR: SD card initialization failed");
    }
    else
    {
        Serial.println("SD card ready");
    }
}

