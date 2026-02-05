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
#include <BMP388_DEV.h>
#include <sensorfusion.h>

  const double GRAVITY = 9.80665;
  craftState vehicle;
  Adafruit_LIS3MDL magnetometer;
  BMP388_DEV bmp388;

void craftState::initalizeSensors()
{
  bmp388.begin();                                 // Default initialisation, place the BMP388 into SLEEP_MODE
  bmp388.setTimeStandby(TIME_STANDBY_1280MS);     // Set the standby time to 1.3 seconds
  bmp388.startNormalConversion();

  if (!IMU.begin())
  {
    Serial.println("IMU failed!");
  }
  else
  {
    Serial.println("IMU OK");
  }

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

void craftState::readBarometerData()
{
  float t, p, a;

  if (bmp388.getMeasurements(t, p, a))
  {
    vehicle.temperature_C = t;
    vehicle.pressure_Pa   = p;
    vehicle.altitude_m    = a;
  }

}

//GAM = Gyro, Accel, Magno
void craftState::readGAM()
{
  float ax, ay, az;
  float gx, gy, gz;

  if (IMU.accelerationAvailable()) 
  {
    IMU.readAcceleration(ax, ay, az);
  }
  if (IMU.gyroscopeAvailable()) 
  {
    IMU.readGyroscope(gx, gy, gz);
  }

  vehicle.accel_g.x = ax;
  vehicle.accel_g.y = ay;
  vehicle.accel_g.z = az;

  vehicle.gyro_dps.x = gx;
  vehicle.gyro_dps.y = gy;
  vehicle.gyro_dps.z = gz;

  sensors_event_t magEvent;
  magnetometer.getEvent(&magEvent);

  vehicle.mag_uT.x = magEvent.magnetic.x;
  vehicle.mag_uT.y = magEvent.magnetic.y;
  vehicle.mag_uT.z = magEvent.magnetic.z;
}

void craftState::duringFlightOrientationTracking(double dt)
{
    // Integrate to get angles, LSOX Sensor gives rotation velocity
    vehicle.gyroAngle_deg.x += vehicle.gyro_dps.x * dt;
    vehicle.gyroAngle_deg.y += vehicle.gyro_dps.y * dt;
    vehicle.gyroAngle_deg.z += vehicle.gyro_dps.z * dt;
}

void craftState::duringFlightPositionalTracking(double dt)
{
  vehicle.deadReckoning_mps2.x += (vehicle.accel_g.x * GRAVITY * dt);
  vehicle.deadReckoning_mps2.y += (vehicle.accel_g.y * GRAVITY * dt);
  vehicle.deadReckoning_mps2.z += (vehicle.accel_g.z * GRAVITY * dt);
}

void craftState::preFlightOrientationTracking(double dt)
{
  const float alpha = 0.50;
  //Corect pitch/roll with IMU while static
  vehicle.gyroAngle_deg.x = alpha * vehicle.gyroAngle_deg.x + (1 - alpha) * (vehicle.pitch * 180.0 / PI);
  vehicle.gyroAngle_deg.y = alpha * vehicle.gyroAngle_deg.y + (1 - alpha) * (vehicle.roll * 180.0 / PI);

  //Correct yaw with fusion Magnometer
  vehicle.yaw = atan2(vehicle.mag_uT.y, vehicle.mag_uT.x) * (180.0 / PI); // degrees
  if (vehicle.yaw < 0) vehicle.yaw += 360.0; // Normalize to 0-360

  vehicle.gyroAngle_deg.z = alpha * vehicle.gyroAngle_deg.z + (1 - alpha) * vehicle.yaw;;
}

void craftState::preFlightPositionalTracking(double dt)
{
  vehicle.deadReckoning_mps2.x += vehicle.gyroAngle_deg.x / dt; 
  vehicle.deadReckoning_mps2.y += vehicle.gyroAngle_deg.y / dt; 
  vehicle.deadReckoning_mps2.z += vehicle.gyroAngle_deg.z / dt; 

  vehicle.computeGravity(); //Gives Gravity IMU Offset

  vehicle.deadReckoning_mps2.x = (vehicle.accel_g.x * 9.81) - vehicle.gravity_mps2.x;
  vehicle.deadReckoning_mps2.y = (vehicle.accel_g.y * 9.81) - vehicle.gravity_mps2.y;
  vehicle.deadReckoning_mps2.z = (vehicle.accel_g.z * 9.81) - vehicle.gravity_mps2.z;
}
void craftState::computeGravity()
    {
        // Compute pitch and roll from accelerometer
        pitch = atan2(accel_g.x, sqrt(accel_g.y*accel_g.y + accel_g.z*accel_g.z));
        roll  = atan2(accel_g.y, sqrt(accel_g.x*accel_g.x + accel_g.z*accel_g.z));
        
        // Gravity vector in sensor frame (m/s²)
        gravity_mps2.x = 9.81 * sin(pitch);
        gravity_mps2.y = 9.81 * sin(roll);
        gravity_mps2.z = sqrt(9.81*9.81 - gravity_mps2.x*gravity_mps2.x - gravity_mps2.y*gravity_mps2.y);
    }