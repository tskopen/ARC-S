/*
* Program to control ARC-S Flash Chip
 */

#include <Arduino.h>
#include <SPI.h>

#define FLASH_CS    36
#define FLASH_CLK   35
#define FLASH_MISO  37
#define FLASH_MOSI  34

SPIClass spiFlash(HSPI);

void setupFlash()
{
    pinMode(FLASH_CS, OUTPUT);
    digitalWrite(FLASH_CS, HIGH);

    spiFlash.begin(FLASH_CLK, FLASH_MISO, FLASH_MOSI, FLASH_CS);

    Serial.println("External flash SPI bus ready");
}