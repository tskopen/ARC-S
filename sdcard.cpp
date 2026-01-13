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
void logAllData() 
{
  char buffer[256];

  unsigned long t = millis();

  snprintf(buffer, sizeof(buffer),
    "%lu,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
    t,

    // Accel (g)
    gForceAccelerationXAxis,
    gForceAccelerationYAxis,
    gForceAccelerationZAxis,

    // Gyro (rad/s)
    gyroAccelerationXAxis,
    gyroAccelerationYAxis,
    gyroAccelerationZAxis,

    // Dead reckoning
    deadReckoningXAxis,
    deadReckoningYAxis,
    deadReckoningZAxis,

    // Magnetometer
    magneticXAxis,
    magneticYAxis,
    magneticZAxis
  );

  dataLogSDCard("flight.csv", buffer);
  Serial.println(buffer);   // optional
}