/*
 * Program to combine all ARC-S Sensors into positional tracking/calculations.
 *
 * Name:      Thor Kopenkoskey
 * Created:   July 1, 2025
 * Edited:    Jan 25, 2025
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_LIS3MDL.h>
#include <Arduino_LSM6DSOX.h>
#include <cmath>


  Adafruit_LIS3MDL magnetometer;

//Gyro and Accel are dead reckoning so are set to 0... Magno + GPS should give reference if needed... IMU should aim to be start level for now
  float distanceXAxis = 0;
  float distanceYAxis = 0;
  float distanceZAxis = 0;


  float accelerationXAxis = 0;
  float accelerationYAxis = 0;
  float accelerationZAxis = 0;

  float gyroPositionXAxis = 0;
  float gyroPositionYAxis = 0;
  float gyroPositionZAxis = 0;

  float gyroAccelerationXAxis = 0;
  float gyroAccelerationYAxis = 0;
  float gyroAccelerationZAxis = 0;

  float magneticXAxis = 0;
  float magneticYAxis = 0;
  float magneticZAxis = 0;

  float gForceAccelerationXAxis = 0;
  float gForceAccelerationYAxis = 0;
  float gForceAccelerationZAxis = 0;

  float MPSAccelerationXAxis = 0;
  float MPSAccelerationYAxis = 0;
  float MPSAccelerationZAxis = 0;

  float deadReckoningXAxis = 0;
  float deadReckoningYAxis = 0;
  float deadReckoningZAxis = 0;

  float pitch = 0;
  float roll = 0;

  float gyroIMUCorrectionXAxis = 0;
  float gyroIMUCorrectionYAxis = 0;
  float gyroIMUCorrectionZAxis = 0;

  float gravityX = 0;
  float gravityY = 0;
  float gravityZ = 0;

  bool launch = false;
  float yawMagnetometer = 0;




void readRawSensorData(){
  if (IMU.accelerationAvailable()) 
  {
    IMU.readAcceleration(gForceAccelerationXAxis, gForceAccelerationYAxis, gForceAccelerationZAxis);
  }
  if (IMU.gyroscopeAvailable()) 
  {
    IMU.readGyroscope(gyroAccelerationXAxis, gyroAccelerationYAxis, gyroAccelerationZAxis);
  }

  sensors_event_t magEvent;
  magnetometer.getEvent(&magEvent);

  magneticXAxis = magEvent.magnetic.x;
  magneticYAxis = magEvent.magnetic.y;
  magneticZAxis = magEvent.magnetic.z;
}

void duringFlightOrientationTracking(unsigned long dt)
{

    // Integrate to get angles
    gyroPositionXAxis += gyroAccelerationXAxis * dt;
    gyroPositionYAxis += gyroAccelerationYAxis * dt;
    gyroPositionZAxis += gyroAccelerationZAxis * dt;
}


void preFlightOrientationTracking(unsigned long dt)
{
  const float alpha = 0.50;
// purely Gyro,
   gyroPositionXAxis += gForceAccelerationXAxis / 104;
   gyroPositionYAxis += gForceAccelerationYAxis / 104;
   gyroPositionZAxis += gForceAccelerationZAxis / 104;

//Corect pitch/roll with IMU while static
   gyroPositionXAxis = alpha * gyroPositionXAxis + (1 - alpha) * (pitch * 180.0 / PI);
   gyroPositionYAxis = alpha * gyroPositionYAxis + (1 - alpha) * (roll * 180.0 / PI);

//Correct yaw with fusiong Magnometer
  yawMagnetometer = atan2(magneticYAxis, magneticXAxis) * (180.0 / PI); // degrees
  if (yawMagnetometer < 0) yawMagnetometer += 360.0; // Normalize to 0-360

   gyroPositionZAxis = alpha * gyroPositionZAxis + (1 - alpha) * yawMagnetometer;;

}

void duringFlightPositionalTracking(unsigned long dt)
{
  deadReckoningXAxis += (gForceAccelerationXAxis * 9.81 / dt); // 9.81 g to m/s2 gravity, 104 sensor hz
  deadReckoningYAxis += (gForceAccelerationYAxis * 9.81 / dt);
  deadReckoningZAxis += (gForceAccelerationZAxis * 9.81 / dt);
}

void preFlightPositionalTracking(unsigned long dt)
{
  deadReckoningXAxis += gyroIMUCorrectionXAxis / dt; // 9.81 g to m/s2 gravity, 104 sensor hz
  deadReckoningYAxis += gyroIMUCorrectionYAxis / dt; // 9.81 g to m/s2 gravity, 104 sensor hz
  deadReckoningZAxis += gyroIMUCorrectionZAxis / dt; // 9.81 g to m/s2 gravity, 104 sensor hz

  pitch = atan2(gForceAccelerationXAxis, sqrt(gForceAccelerationYAxis * gForceAccelerationYAxis + gForceAccelerationZAxis * gForceAccelerationZAxis));
  roll  = atan2(gForceAccelerationYAxis, sqrt(gForceAccelerationXAxis * gForceAccelerationXAxis + gForceAccelerationZAxis * gForceAccelerationZAxis));

  gravityX = 9.81 * sin(pitch);
  gravityY = 9.81 * sin(roll);
  gravityZ = sqrt(9.81*9.81 - gravityX*gravityX - gravityY*gravityY); // assumes level

  deadReckoningXAxis = (gForceAccelerationXAxis * 9.81) - gravityX;
  deadReckoningYAxis = (gForceAccelerationYAxis * 9.81) - gravityY;
  deadReckoningZAxis = (gForceAccelerationZAxis * 9.81) - gravityZ;
}