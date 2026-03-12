/*
 * Program to control ARC-S SD Card logging
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 11, 2025
 * Edited:    January 15, 2025
 */
#include "sdcard.h"
extern Telemetry telemetry;


#define SDCardCS    18
#define SDCardCLK   35
#define SDCardMISO  12
#define SDCardMOSI  16

File dataFile;
const int chipSelect = 10;  // CS pin for SD module

void initializeSDCard() {
 
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("SD card initialization failed!");
  }
  else
  {
  Serial.println("SD card ready.");
  }
  // Try creating or opening the file
  dataFile = SD.open("log.csv", FILE_WRITE);

  if (dataFile) 
  {
    // If the file is empty, write the header
    if (dataFile.size() == 0) {
      dataFile.println("Timestamp_ms,"
    "Accel_X_g,Accel_Y_g,Accel_Z_g,"
    "Gyro_X_dps,Gyro_Y_dps,Gyro_Z_dps,"
    "GyroAngle_X_deg,GyroAngle_Y_deg,GyroAngle_Z_deg,"
    "DeadReckon_X_m,DeadReckon_Y_m,DeadReckon_Z_m,"
    "Mag_X_uT,Mag_Y_uT,Mag_Z_uT,"
    "Temp_C,Pressure_Pa,Altitude_m,"
    "Servo_PosY_deg,Servo_NegY_deg,Servo_PosX_deg,Servo_NegX_deg");
      Serial.println("Header written to log.csv");
    }
    dataFile.close();
  } 
  else 
  {
    Serial.println("Error opening log.csv!");
  }
}

void writeToSDCard() 
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  // Log data every 1 seconds
  if (currentMillis - previousMillis >= 1000) 
  {
    previousMillis = currentMillis;

    float timeSec = currentMillis / 1000.0;

    // Open the file to append new data
    dataFile = SD.open("log.csv", FILE_WRITE);

    if (dataFile) 
    {
      telemetry.logAllSensors(dataFile);
      dataFile.close();
    } 
    else 
    {
      Serial.println("Error opening log.csv!");
    }
  }
}