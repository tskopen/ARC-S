/*
 * Program to combine all ARC-S Files into a flight profile for the ARC-S 2026 Fair
 *
 * Name:      Thor Kopenkoskey
 * Created:   November 17, 2025
 * Edited:    Jan 25, 2025
 */

#include <sensorfusion.h>
#include <servos.h>
#include "flash.h"
#include "sdcard.h"

#define SerialPort Serial
#define I2C_SDA    9
#define I2C_SCL    46

extern craftState vehicle;

void setup()
{
  delay(300);
  Serial.begin(115200);
  delay(2000);
  Serial.println("Serial OK");

  EEPROM.begin(EEPROM_SIZE);

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);  
  Wire.setTimeout(1000);
  delay(100);
  Wire.beginTransmission(0x00);  // General call address (should always NACK, but tests bus)
  uint8_t error = Wire.endTransmission();
  if (error != 0 && error != 2) 
  {  // 2 = NACK (expected), other errors indicate problems
    Serial.print("WARNING: I2C bus test returned error code: ");
    Serial.println(error);
  }

  SPI.begin();

  vehicle.initalizeSensors();

  for (int i = 0; i < NUM_SERVOS; i++)
  {
    servos[i].init_servo();
  }

  loadOffsetsFromEEPROM(); // restore trims on startup
  Serial.println("Servo system ready.");

}


void loop() 
{
  double dt = getDeltaTime();
  unsigned long currentTime = micros();


  readSerialAndUpdateServo();

  vehicle.readGAM();
  vehicle.readBarometerData();
  pitchYawControlLoop();

  if (vehicle.gyro_dps.z > 90) 
  {
    rollControlLoop();
  }


  if (vehicle.launch == false)
  {
    vehicle.preFlightPositionalTracking(dt);
    vehicle.preFlightOrientationTracking(dt);

    if (vehicle.gyro_dps.z > 1.5) 
    {
      vehicle.launch = true;
    }

  }

  if (vehicle.launch == true)
  {

    vehicle.duringFlightPositionalTracking(dt);
    vehicle.duringFlightOrientationTracking(dt);

  }

  unsigned long lastSendTime = 0;
  const unsigned long sendInterval = 1000;
  // Time Defined loop (good for serial output monitors.
  if (currentTime - lastSendTime >= sendInterval) 
  {
    lastSendTime = currentTime;

    Serial.println(); // Blank line between samples
  }
}