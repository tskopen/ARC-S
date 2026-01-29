/*
<<<<<<< Updated upstream
<<<<<<< Updated upstream
* Header file ARC-S telemetry printouts
=======
* cpp file ARC-S telemetry printouts
>>>>>>> Stashed changes
=======
* cpp file ARC-S telemetry printouts
>>>>>>> Stashed changes
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 27, 2025
 * Edited:    January 27, 2025
 */
<<<<<<< Updated upstream
<<<<<<< Updated upstream

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
=======
=======
>>>>>>> Stashed changes
#include "Telemetry.h"
extern craftState vehicle;

// -------------------- Telemetry.cpp --------------------

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
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
