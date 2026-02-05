/*
* cpp file ARC-S telemetry printouts
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 27, 2025
 * Edited:    February 27, 2025
 */

#include "Telemetry.h"
extern craftState vehicle;

void Telemetry::serialIMU() 
{
    // CSV format: ACC_X,ACC_Y,ACC_Z
    Serial.print(vehicle.accel_g.x, 3); Serial.print(",");
    Serial.print(vehicle.accel_g.y, 3); Serial.print(",");
    Serial.println(vehicle.accel_g.z, 3);
}

void Telemetry::serialIMUDeadReckoning() 
{
    // CSV format: DR_X,DR_Y,DR_Z
    Serial.print(vehicle.deadReckoning_mps2.x, 3); Serial.print(",");
    Serial.print(vehicle.deadReckoning_mps2.y, 3); Serial.print(",");
    Serial.println(vehicle.deadReckoning_mps2.z, 3);
}

void Telemetry::serialGyro() 
{
    // CSV format: GYRO_X,GYRO_Y,GYRO_Z
    Serial.print(vehicle.gyro_dps.x, 2); Serial.print(",");
    Serial.print(vehicle.gyro_dps.y, 2); Serial.print(",");
    Serial.println(vehicle.gyro_dps.z, 2);
}

void Telemetry::serialGyroDeadReckoning() 
{
    // CSV format: GP_X,GP_Y,GP_Z
    Serial.print(vehicle.gyroAngle_deg.x, 2); Serial.print(",");
    Serial.print(vehicle.gyroAngle_deg.y, 2); Serial.print(",");
    Serial.println(vehicle.gyroAngle_deg.z, 2);
}

void Telemetry::serialMagnometer() 
{
    // CSV format: MAG_X,MAG_Y,MAG_Z
    Serial.print(vehicle.mag_uT.x, 2); Serial.print(",");
    Serial.print(vehicle.mag_uT.y, 2); Serial.print(",");
    Serial.println(vehicle.mag_uT.z, 2);
}

void Telemetry::serialBarometer() 
{
    // CSV format: TEMP_C,PRESSURE_HPA,ALT_M
    Serial.print(vehicle.temperature_C, 2); Serial.print(",");
    Serial.print(vehicle.pressure_Pa, 2); Serial.print(",");
    Serial.println(vehicle.altitude_m, 2);
}

void Telemetry::serialServoOutputs() 
{
    // CSV format: PITCH+Y,PITCH-Y,YAW+X,YAW-X,ROLL
    Serial.print(vehicle.gyroAngle_deg.x, 2); Serial.print(",");
    Serial.print(-vehicle.gyroAngle_deg.x, 2); Serial.print(",");
    Serial.print(-vehicle.gyroAngle_deg.y, 2); Serial.print(",");
    Serial.print(vehicle.gyroAngle_deg.y, 2); Serial.print(",");
    Serial.println(vehicle.gyroAngle_deg.z / 90, 2);
}
