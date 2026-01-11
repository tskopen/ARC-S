#include "flash.h"

Preferences preferences;

void setupSDCard() {
  SPI.begin(SDCardCLK, SDCardMISO, SDCardMOSI, SDCardCS);
  //Pins are IO#

  int SDCardCS = 18;
  int SDCardCLK = 35;
  int SDCardMISO = 12;
  int SDCardMOSI = 16;

  pinMode(SDCardMOSI, OUTPUT);


  pinMode(SDCardMISO, INPUT);


  pinMode(SDCardCLK, OUTPUT);


  pinMode(SDCardCS, OUTPUT);

}
void dataLogSDCard() {
  SPI.begin(SDCardCLK, SDCardMISO, SDCardMOSI, SDCardCS);
  //Pins are IO#

  int SDCardCS = 18;
  int SDCardCLK = 35;
  int SDCardMISO = 12;
  int SDCardMOSI = 16;

  pinMode(SDCardMOSI, OUTPUT);


  pinMode(SDCardMISO, INPUT);


  pinMode(SDCardCLK, OUTPUT);


  pinMode(SDCardCS, OUTPUT);

}