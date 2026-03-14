/*
* Program to control ARC-S Flash Chip
 */

#include <Arduino.h>
#include <SPI.h>
#include "SPIFFS.h"
#include <FS.h>

#define FLASH_CS    36
#define FLASH_CLK   35
#define FLASH_MISO  37
#define FLASH_MOSI  34

/*
void setupFlash()
{
    pinMode(FLASH_CS, OUTPUT);
    digitalWrite(FLASH_CS, HIGH);

    spiFlash.begin(FLASH_CLK, FLASH_MISO, FLASH_MOSI, FLASH_CS);

    Serial.println("External flash SPI bus ready");

  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  Serial.println("SPIFFS ready");

  File file = SPIFFS.open("/events.csv", FILE_WRITE);

    if(!file){
        Serial.println("Failed to open file");
        return;
    }

    file.println("time,roll,pitch,yaw,altitude,comments");

    file.close();
}

void logData(float roll, float pitch, float yaw, float alt)
{
    File file = SPIFFS.open("/events.csv", FILE_APPEND);

    if(!file){
        Serial.println("Append failed");
        return;
    }

    file.printf("%lu,%.2f,%.2f,%.2f,%.2f\n",
        millis(), roll, pitch, yaw, alt);

    file.close();
}*/