/*
* cpp file ARC-S telemetry printouts
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 27, 2025
 * Edited:    February 28, 2025
 */
#include "Telemetry.h"
#include "servos.h"
extern craftState vehicle;

void Telemetry::serialIMU() 
{
    // CSV format: ACC_X,ACC_Y,ACC_Z
    Serial.print(vehicle.accel_g.x, 3); Serial.print(",");
    Serial.print(vehicle.accel_g.y, 3); Serial.print(",");
    Serial.println(vehicle.accel_g.z, 3);
    Serial.flush(); // ensure buffer is sent
}

void Telemetry::serialIMUDeadReckoning() 
{
    // CSV format: DR_X,DR_Y,DR_Z
    Serial.print(vehicle.deadReckoningPosition_m.x, 3); Serial.print(",");
    Serial.print(vehicle.deadReckoningPosition_m.y, 3); Serial.print(",");
    Serial.println(vehicle.deadReckoningPosition_m.z, 3);
    Serial.flush();
}

void Telemetry::serialGyro() 
{
    // CSV format: GYRO_X,GYRO_Y,GYRO_Z
    Serial.print(vehicle.gyro_dps.x, 2); Serial.print(",");
    Serial.print(vehicle.gyro_dps.y, 2); Serial.print(",");
    Serial.println(vehicle.gyro_dps.z, 2);
    Serial.flush();
}

void Telemetry::serialGyroDeadReckoning() 
{
    // CSV format: GP_X,GP_Y,GP_Z
    Serial.print(vehicle.gyroAngle_deg.x, 2); Serial.print(",");
    Serial.print(vehicle.gyroAngle_deg.y, 2); Serial.print(",");
    Serial.println(vehicle.gyroAngle_deg.z, 2);
    Serial.flush();
}

void Telemetry::serialMagnometer() 
{
    // CSV format: MAG_X,MAG_Y,MAG_Z
    Serial.print(vehicle.mag_uT.x, 2); Serial.print(",");
    Serial.print(vehicle.mag_uT.y, 2); Serial.print(",");
    Serial.println(vehicle.mag_uT.z, 2);
    Serial.flush();
}

void Telemetry::serialBarometer() 
{
    // CSV format: TEMP_C,PRESSURE_HPA,ALT_M
    Serial.print(vehicle.temperature_C, 2); Serial.print(",");
    Serial.print(vehicle.pressure_Pa, 2); Serial.print(",");
    Serial.println(vehicle.altitude_m, 2);
    Serial.flush();
}

void Telemetry::serialServoOutputs() 
{
    // CSV format (deg): PITCH+Y,PITCH-Y,YAW+X,YAW-X
    // Uses the last commanded servo mix captured in applyServoMix()
    Serial.print(lastServoCommand.posY, 2); Serial.print(",");   // PITCH+Y
    Serial.print(lastServoCommand.negY, 2); Serial.print(",");   // PITCH-Y
    Serial.print(lastServoCommand.posX, 2); Serial.print(",");   // YAW+X
    Serial.print(lastServoCommand.negX, 2); Serial.println();    // YAW-X

    Serial.flush();
}