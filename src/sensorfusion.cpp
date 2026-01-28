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

  craftState vehicle;

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
    IMU.readAcceleration(vehicle.accel_g.x, vehicle.accel_g.y, vehicle.accel_g.z);
  }
  if (IMU.gyroscopeAvailable()) 
  {
    IMU.readGyroscope(vehicle.gyro_dps.x, vehicle.gyro_dps.y, vehicle.gyro_dps.z);
  }

  sensors_event_t magEvent;
  magnetometer.getEvent(&magEvent);

  vehicle.mag_uT.x = magEvent.magnetic.x;
  vehicle.mag_uT.y = magEvent.magnetic.y;
  vehicle.mag_uT.z = magEvent.magnetic.z;
}

void duringFlightOrientationTracking(double dt)
{
    // Integrate to get angles, LSOX Sensor gives rotation velocity
    vehicle.gyroAngle_deg.x += vehicle.gyro_dps.x * dt;
    vehicle.gyroAngle_deg.y += vehicle.gyro_dps.y * dt;
    vehicle.gyroAngle_deg.z += vehicle.gyro_dps.z * dt;
}




void duringFlightPositionalTracking(double dt)
{
  vehicle.deadReckoning_mps2.x += (vehicle.accel_g.x * GRAVITY * dt);
  vehicle.deadReckoning_mps2.y += (vehicle.accel_g.y * GRAVITY * dt);
  vehicle.deadReckoning_mps2.z += (vehicle.accel_g.z * GRAVITY * dt);
}

void preFlightOrientationTracking(double dt)
{
  const float alpha = 0.50;
  //Corect pitch/roll with IMU while static
  vehicle.gyroAngle_deg.x = alpha * vehicle.gyroAngle_deg.x + (1 - alpha) * (pitch * 180.0 / PI);
  vehicle.gyroAngle_deg.y = alpha * vehicle.gyroAngle_deg.y + (1 - alpha) * (roll * 180.0 / PI);

  //Correct yaw with fusiong Magnometer
  vehicle.yaw = atan2(vehicle.mag_uT.y, vehicle.mag_uT.x) * (180.0 / PI); // degrees
  if (vehicle.yaw < 0) vehicle.yaw += 360.0; // Normalize to 0-360

  vehicle.gyroAngle_deg.z = alpha * vehicle.gyroAngle_deg.z + (1 - alpha) * yawMagnetometer;;

}

void preFlightPositionalTracking(double dt)
{
  vehicle.deadReckoning_mps2.x += vehicle.gyroAngle_deg.x / dt; // 9.81 g to m/s2 gravity, 104 sensor hz
  vehicle.deadReckoning_mps2.y += vehicle.gyroAngle_deg.y / dt; // 9.81 g to m/s2 gravity, 104 sensor hz
  vehicle.deadReckoning_mps2.z += vehicle.gyroAngle_deg.z / dt; // 9.81 g to m/s2 gravity, 104 sensor hz

  computeGravity();

  vehicle.deadReckoning_mps2.x = (accel_g.x * 9.81) - gravityX;
  vehicle.deadReckoning_mps2.y = (accel_g.y * 9.81) - gravityY;
  vehicle.deadReckoning_mps2.z = (accel_g.z * 9.81) - gravityZ;
}