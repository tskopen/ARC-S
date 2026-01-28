/*
* Header file ARC-S telemetry printouts
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 27, 2025
 * Edited:    January 27, 2025
 */

#include "Telemetry.h"
extern craftState vehicle;

void serialIMU()
{
 Serial.print("ACC_X:"); Serial.print(vehicle.accel_g.x); Serial.print('\t');
 Serial.print("ACC_Y:"); Serial.print(vehicle.accel_g.y); Serial.print('\t');
 Serial.print("ACC_Z:"); Serial.println(vehicle.accel_g.z);
}

void serialIMUDeadReckoning()
{
 Serial.print("DR_X:"); Serial.print(vehicle.deadReckoning_mps2.x); Serial.print('\t');
 Serial.print("DR_Y:"); Serial.print(vehicle.deadReckoning_mps2.y); Serial.print('\t');
 Serial.print("DR_Z:"); Serial.println(vehicle.deadReckoning_mps2.z);
}
void serialGyro()
{
 Serial.print("GX:"); Serial.print(vehicle.gyro_dps.x); Serial.print('\t');
 Serial.print("GY:"); Serial.print(vehicle.gyro_dps.y); Serial.print('\t');
 Serial.print("GZ:"); Serial.println(vehicle.gyro_dps.z);
}
void serialGyroDeadReckoning()
{
 Serial.print("GP_X:"); Serial.print(vehicle.gyroAngle_deg.x); Serial.print('\t');
 Serial.print("GP_Y:"); Serial.print(vehicle.gyroAngle_deg.y); Serial.print('\t');
 Serial.print("GP_Z:"); Serial.println(vehicle.gyroAngle_deg.z);
}
void serialMagnometer()
{
 Serial.print("MX:"); Serial.print(vehicle.mag_uT.x); Serial.print('\t');
 Serial.print("MY:"); Serial.print(vehicle.mag_uT.y); Serial.print('\t');
 Serial.print("MZ:"); Serial.println(vehicle.mag_uT.z);
}
void serialBarometer()
{
  Serial.print(vehicle.temperature_C);
  Serial.print(F("*C   "));
  Serial.print(vehicle.pressure_Pa);
  Serial.print(F("hPa   "));
  Serial.print(vehicle.altitude_m);
  Serial.println(F("m"));
}
void serialServoOutputs()
{
 Serial.print(vehicle.gyroAngle_deg.x, 2);      // Pitch +Y
 Serial.print(",");
 Serial.print(-vehicle.gyroAngle_deg.x, 2);     // Pitch -Y
 Serial.print(",");
 Serial.print(-vehicle.gyroAngle_deg.y, 2);     // Yaw +X
 Serial.print(",");
 Serial.print(vehicle.gyroAngle_deg.y, 2);      // Yaw -X
 Serial.print(",");
 Serial.println(vehicle.gyroAngle_deg.z / 90, 2); // Roll +X/-X
}