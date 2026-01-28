/*
 * Program to combine all ARC-S Sensors into positional tracking/calculations.
 *
 * Name:      Thor Kopenkoskey
 * Created:   November 17, 2025
 * Edited:    Jan 28, 2025
 */

#include <Arduino_LSM6DSOX.h>
#include <Adafruit_LIS3MDL.h>



#define LSM6DSOX_ADDR 0x6B 
#define LIS3MDL_ADDR 0x1E
const double GRAVITY = 9.80665;
struct Vec3 {

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    void zero();
};

class craftState {
    public:
    // Raw sensors
    Vec3 accel_g;
    Vec3 gyro_dps;
    Vec3 mag_uT;

    // Derived state
    Vec3 deadReckoning_mps2;
    Vec3 gyroAngle_deg;
    Vec3 gravity_mps2;


    double pitch = 0.0;
    double roll = 0.0;
    double yaw = 0.0;

    float temperature;
    float pressure;
    float altitude;

    bool launch = false;
    bool apogee = false; //apogee can be cruising altitude for aircraft
    bool landed = false;

    void initalizeSensors();
    void readGAM();
    void duringFlightOrientationTracking(double dt);
    void duringFlightPositionalTracking(double dt);
    void preFlightOrientationTracking(double dt);
    void preFlightPositionalTracking(double dt);


    void computeGravity()
    {
        // Compute pitch and roll from accelerometer
        pitch = atan2(accel_g.x, sqrt(accel_g.y*accel_g.y + accel_g.z*accel_g.z));
        roll  = atan2(accel_g.y, sqrt(accel_g.x*accel_g.x + accel_g.z*accel_g.z));

        // Gravity vector in sensor frame (m/s²)
        gravity_mps2.x = 9.81 * sin(pitch);
        gravity_mps2.y = 9.81 * sin(roll);
        gravity_mps2.z = sqrt(9.81*9.81 - gravity_mps2.x*gravity_mps2.x - gravity_mps2.y*gravity_mps2.y);
    }

};
