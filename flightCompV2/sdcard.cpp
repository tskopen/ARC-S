#include "sdcard.h"
#include <SPI.h>
#include <SD.h>
Preferences preferences;

#define SDCardCS    18;
#define SDCardCLK   35;
#define SDCardMISO  12;
#define SDCardMOSI  16;
void setupSDCard() 
  {
    SPI.begin(SDCardCLK, SDCardMISO, SDCardMOSI, SDCardCS);
    //Pins are IO#


    pinMode(SDCardMOSI, OUTPUT);


    pinMode(SDCardMISO, INPUT);


    pinMode(SDCardCLK, OUTPUT);


    pinMode(SDCardCS, OUTPUT);

    SD.begin(SDCardCS)


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