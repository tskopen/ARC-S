/*
* Header file ARC-S telemetry printouts
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 27, 2025
 * Edited:    January 27, 2025
 */

#include "Telemetry.h"
using namespace Telemetry;

void serialIMU()
{
 Serial.print("ACC_X:"); Serial.print(accel_g.x); Serial.print('\t');
 Serial.print("ACC_Y:"); Serial.print(accel_g.y); Serial.print('\t');
 Serial.print("ACC_Z:"); Serial.println(accel_g.z);
}

void serialIMUDeadReckoning()
{
 Serial.print("DR_X:"); Serial.print(deadReckoning_mps2.x); Serial.print('\t');
 Serial.print("DR_Y:"); Serial.print(deadReckoning_mps2.y); Serial.print('\t');
 Serial.print("DR_Z:"); Serial.println(deadReckoning_mps2.z);
}
void serialGyro()
{
 Serial.print("GX:"); Serial.print(gyro_dps.x); Serial.print('\t');
 Serial.print("GY:"); Serial.print(gyro_dps.y); Serial.print('\t');
 Serial.print("GZ:"); Serial.println(gyro_dps.z);
}
void serialGyroDeadReckoning()
{
 Serial.print("GP_X:"); Serial.print(gyroAngle_deg.x); Serial.print('\t');
 Serial.print("GP_Y:"); Serial.print(gyroAngle_deg.y); Serial.print('\t');
 Serial.print("GP_Z:"); Serial.println(gyroAngle_deg.z);
}
void serialMagnometer()
{
 Serial.print("MX:"); Serial.print(mag_uT.x); Serial.print('\t');
 Serial.print("MY:"); Serial.print(mag_uT.y); Serial.print('\t');
 Serial.print("MZ:"); Serial.println(mag_uT.z);
}
void serialBarometer()
{
 if (bmp388.getMeasurements(temperature, pressure, altitude))    // Check if the measurement is complete
 {
  Serial.print(temperature);
  Serial.print(F("*C   "));
  Serial.print(pressure);
  Serial.print(F("hPa   "));
  Serial.print(altitude);
  Serial.println(F("m"));
 }
}
void serialServoOutputs()
{
 Serial.print(gyroAngle_deg.x, 2);      // Pitch +Y
 Serial.print(",");
 Serial.print(-gyroAngle_deg.x, 2);     // Pitch -Y
 Serial.print(",");
 Serial.print(-gyroAngle_deg.y, 2);     // Yaw +X
 Serial.print(",");
 Serial.print(gyroAngle_deg.y, 2);      // Yaw -X
 Serial.print(",");
 Serial.println(gyroAngle_deg.z / 90, 2); // Roll +X/-X
}