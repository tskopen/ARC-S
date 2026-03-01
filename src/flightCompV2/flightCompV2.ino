/*
 * Program to combine all ARC-S Files into a flight profile for the ARC-S 2026 Fair
 *
 * Name:      Thor Kopenkoskey
 * Created:   November 17, 2025
 * Edited:    Feb 26, 2025
 */

#include "telemetry.h"
#include <sensorfusion.h>
#include <servos.h>
#include "flash.h"
#include "sdcard.h"
#include "PID.h"
Telemetry telemetry;

#define SerialPort Serial
#define I2C_SDA 9
#define I2C_SCL 46

extern craftState vehicle;

void setup() 
{
  delay(300);
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Serial OK");

  EEPROM.begin(EEPROM_SIZE);

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(50000);
  Wire.setTimeout(100);

  delay(100);

  Wire.beginTransmission(0x00);  

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

  loadOffsetsFromEEPROM();  // restore trims on startup
  Serial.println("Servo system ready.");

  //Calibrate sensors
  vehicle.readGAM();
  vehicle.calibrateGyroBias();

}

const uint32_t LOOP_US = 1000; // 1 kHz
static uint32_t lastLoop = 0;
uint32_t setupFinish = micros();
void loop() 
{

  uint8_t error = Wire.endTransmission();
  if (error != 0 && error != 2) 
  {  // 2 = NACK (expected), other errors indicate problems
    Serial.print("WARNING: I2C bus test returned error code: ");
    Serial.println(error);
  }

    uint32_t now = micros();
    if (now - lastLoop < LOOP_US) return;

    float dt = (now - lastLoop) * 1e-6;
    lastLoop = now;
  
  readSerialAndUpdateServo();

  vehicle.readGAM();

  //calibrate after 1 second of looping for warmup and calibration
  static bool gyroCalibrated = false;

  // Run once after 1 second, this allows sensor to warm up
  if (!gyroCalibrated && (now - setupFinish >= 1000))
  {
      vehicle.calibrateGyroBias();
      gyroCalibrated = true;
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

    flightControlUpdate();

    telemetry.serialGyro();
    delay(25);
}

 