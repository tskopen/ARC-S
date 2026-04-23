/*
 * Program to combine all ARC-S Sensors into positional tracking/calculations.
 *
 * Name:      Thor Kopenkoskey
 * Created:   November 17, 2025
 * Edited:    Feb 26, 2025
 */

#pragma once
#ifndef SENSORFUSION_H
#define SENSORFUSION_H
#include <Arduino_LSM6DSOX.h>
#include <Adafruit_LIS3MDL.h>
#include <BMP388_DEV.h>

#define LSM6DSOX_ADDR 0x6B 
#define LIS3MDL_ADDR 0x1E

struct Vec3 
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    void zero();
};

class craftState 
{
public:
    // Raw sensors
    Vec3 accel_g;
    Vec3 gyro_dps;
    Vec3 mag_uT;

    // Derived state
    Vec3 velocity_mps;
    
    Vec3 gyroAngle_deg;
    Vec3 gyro_bias_dps;

    Vec3 gravity_mps2;

    //Derived position
    Vec3 deadReckoningPosition_m;

    double pitch = 0.0;
    double roll = 0.0;
    double yaw = 0.0;

    float temperature_C;
    float pressure_Pa;
    float altitude_m;

    bool launch = false;
    bool apogee = false;
    bool landed = false;

    double expectedTimeToApogee = 6;
    double expectedTimeTolanding = 6;


    void initalizeSensors();
    void readGAM();
    void duringFlightOrientationTracking(double dt);
    void duringFlightPositionalTracking(double dt);
    void preFlightOrientationTracking(double dt);
    void preFlightPositionalTracking(double dt);
    void readBarometerData();
    void computeGravityWhileStationary();
    void calibrateGyroBias();
    bool hasLanded(float altitude, float verticalSpeed);

};

// Global constants and objects declared as extern
extern const double GRAVITY;
extern craftState vehicle;
extern Adafruit_LIS3MDL magnetometer;
extern BMP388_DEV bmp388;

// Utility
double getDeltaTime();
#endif // SENSORFUSION_H