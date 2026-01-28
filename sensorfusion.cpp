/*
 * Program to combine all ARC-S Sensors into positional tracking/calculations.
 *
 * Name:      Thor Kopenkoskey
 * Created:   July 1, 2025
 * Edited:    Jan 27, 2025
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_LIS3MDL.h>
#include <Arduino_LSM6DSOX.h>
#include <cmath>


  Adafruit_LIS3MDL magnetometer;
  BMP388_DEV bmp388;
using namespace craftState;
void initalizeSensors()
{
  bmp388.begin();                                 // Default initialisation, place the BMP388 into SLEEP_MODE
  bmp388.setTimeStandby(TIME_STANDBY_1280MS);     // Set the standby time to 1.3 seconds
  bmp388.startNormalConversion();

  // =====================
  // IMU
  // =====================
  if (!IMU.begin())
  {
    Serial.println("IMU failed!");
  }
  else
  {
    Serial.println("IMU OK");
  }
  // Magnetometer (I2C)
  // =====================

  if (!magnetometer.begin_I2C(LIS3MDL_ADDR, &Wire))
  {
    Serial.println("Magnetometer failed!");
  }
  else
  {
    Serial.println("Mag OK");
  }
}

double getDeltaTime()
{
  static unsigned long lastTime = 0;         // remembers last call
  unsigned long currentTime = micros();      // current time in microseconds

  double dt = (currentTime - lastTime) / 1e6; // convert to seconds, sensors report MS and DPS

  lastTime = currentTime;

  return dt;
}

//GAM = Gyro, Accel, Magno
void readGAM(){
  if (IMU.accelerationAvailable()) 
  {
    IMU.readAcceleration(accel_g.x, accel_g.y, accel_g.z);
  }
  if (IMU.gyroscopeAvailable()) 
  {
    IMU.readGyroscope(gyro_dps.x, gyro_dps.y, gyro_dps.z);
  }

  sensors_event_t magEvent;
  magnetometer.getEvent(&magEvent);

  mag_uT.x = magEvent.magnetic.x;
  mag_uT.y = magEvent.magnetic.y;
  mag_uT.z = magEvent.magnetic.z;
}

void duringFlightOrientationTracking(double dt)
{
    // Integrate to get angles, LSOX Sensor gives rotation velocity
    gyroAngle_deg.x += gyro_dps.x * dt;
    gyroAngle_deg.y += gyro_dps.y * dt;
    gyroAngle_deg.z += gyro_dps.z * dt;
}




void duringFlightPositionalTracking(double dt)
{
  deadReckoning_mps2.x += (accel_g.x * GRAVITY * dt);
  deadReckoning_mps2.y += (accel_g.y * GRAVITY * dt);
  deadReckoning_mps2.z += (accel_g.z * GRAVITY * dt);
}

void preFlightOrientationTracking(double dt)
{
  const float alpha = 0.50;
  //Corect pitch/roll with IMU while static
  gyroAngle_deg.x = alpha * gyroPositionXAxis + (1 - alpha) * (pitch * 180.0 / PI);
  gyroAngle_deg.y = alpha * gyroPositionYAxis + (1 - alpha) * (roll * 180.0 / PI);

  //Correct yaw with fusiong Magnometer
  yawMagnetometer = atan2(magneticYAxis, magneticXAxis) * (180.0 / PI); // degrees
  if (yawMagnetometer < 0) yawMagnetometer += 360.0; // Normalize to 0-360

  gyroAngle_deg.z = alpha * gyroPositionZAxis + (1 - alpha) * yawMagnetometer;;

}

void preFlightPositionalTracking(double dt)
{
  deadReckoning_mps2.x += gyroAngle_deg.x / dt; // 9.81 g to m/s2 gravity, 104 sensor hz
  deadReckoning_mps2.y += gyroAngle_deg.y / dt; // 9.81 g to m/s2 gravity, 104 sensor hz
  deadReckoning_mps2.z += gyroAngle_deg.z / dt; // 9.81 g to m/s2 gravity, 104 sensor hz

  pitch = atan2(accel_g.x, sqrt(accel_g.y * accel_g.y + accel_g.z * accel_g.z));
  roll  = atan2(accel_g.y, sqrt(accel_g.x * accel_g.x + accel_g.z * accel_g.z));

  gravityX = 9.81 * sin(pitch);
  gravityY = 9.81 * sin(roll);
  gravityZ = sqrt(9.81*9.81 - gravityX*gravityX - gravityY*gravityY); // assumes level

  deadReckoning_mps2.x = (accel_g.x * 9.81) - gravityX;
  deadReckoning_mps2.y = (accel_g.y * 9.81) - gravityY;
  deadReckoning_mps2.z = (accel_g.z * 9.81) - gravityZ;
}