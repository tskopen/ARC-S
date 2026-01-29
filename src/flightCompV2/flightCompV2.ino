/*
 * Program to combine all ARC-S Files into a flight profile for the ARC-S 2026 Fair
 *
 * Name:      Thor Kopenkoskey
 * Created:   November 17, 2025
 * Edited:    Jan 25, 2025
 */

<<<<<<< Updated upstream
=======
#include "telemetry.h"
>>>>>>> Stashed changes
#include <sensorfusion.h>
#include <servos.h>
#include "flash.h"
#include "sdcard.h"
<<<<<<< Updated upstream

#define SerialPort Serial
#define I2C_SDA    9
#define I2C_SCL    46

extern craftState vehicle;

void setup()
{
=======
Telemetry telemetry;

#define SerialPort Serial
#define I2C_SDA 9
#define I2C_SCL 46

extern craftState vehicle;

void setup() {
>>>>>>> Stashed changes
  delay(300);
  Serial.begin(115200);
  delay(2000);
  Serial.println("Serial OK");

  EEPROM.begin(EEPROM_SIZE);

  Wire.begin(I2C_SDA, I2C_SCL);
<<<<<<< Updated upstream
  Wire.setClock(100000);  
=======
  Wire.setClock(100000);
>>>>>>> Stashed changes
  Wire.setTimeout(1000);
  delay(100);
  Wire.beginTransmission(0x00);  // General call address (should always NACK, but tests bus)
  uint8_t error = Wire.endTransmission();
<<<<<<< Updated upstream
  if (error != 0 && error != 2) 
  {  // 2 = NACK (expected), other errors indicate problems
=======
  if (error != 0 && error != 2) {  // 2 = NACK (expected), other errors indicate problems
>>>>>>> Stashed changes
    Serial.print("WARNING: I2C bus test returned error code: ");
    Serial.println(error);
  }

  SPI.begin();

  vehicle.initalizeSensors();

<<<<<<< Updated upstream
  for (int i = 0; i < NUM_SERVOS; i++)
  {
    servos[i].init_servo();
  }

  loadOffsetsFromEEPROM(); // restore trims on startup
  Serial.println("Servo system ready.");

}


void loop() 
{
=======
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].init_servo();
  }

  loadOffsetsFromEEPROM();  // restore trims on startup
  Serial.println("Servo system ready.");
}


void loop() {
>>>>>>> Stashed changes
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


<<<<<<< Updated upstream
  if (vehicle.launch == false)
=======
  if (vehicle.launch == false) 
>>>>>>> Stashed changes
  {
    vehicle.preFlightPositionalTracking(dt);
    vehicle.preFlightOrientationTracking(dt);

    if (vehicle.gyro_dps.z > 1.5) 
    {
      vehicle.launch = true;
    }
<<<<<<< Updated upstream

  }

  if (vehicle.launch == true)
=======
  }

  if (vehicle.launch == true) 
>>>>>>> Stashed changes
  {

    vehicle.duringFlightPositionalTracking(dt);
    vehicle.duringFlightOrientationTracking(dt);
<<<<<<< Updated upstream

  }

  unsigned long lastSendTime = 0;
  const unsigned long sendInterval = 1000;
  // Time Defined loop (good for serial output monitors.
  if (currentTime - lastSendTime >= sendInterval) 
  {
    lastSendTime = currentTime;

    Serial.println(); // Blank line between samples
  }
=======
  }


    telemetry.serialIMU();
    telemetry.serialGyro();
    telemetry.serialBarometer();
    Serial.println();  // Blank line between samples
>>>>>>> Stashed changes
}