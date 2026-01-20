#include <Arduino_LSM6DSOX.h>
#include <Adafruit_LIS3MDL.h>

#define SerialPort Serial
#define I2C2_SDA    21
#define I2C2_SCL    22

#define LSM6DSOX_ADDR 0x6A 
#define LIS3MDL_ADDR 0x1C 

extern Adafruit_LIS3MDL magnetometer;

// Declare all global variables as extern here
extern float distanceXAxis;
extern float distanceYAxis;
extern float distanceZAxis;

extern float accelerationXAxis;
extern float accelerationYAxis;
extern float accelerationZAxis;

extern float gyroPositionXAxis;
extern float gyroPositionYAxis;
extern float gyroPositionZAxis;

extern float gyroAccelerationXAxis;
extern float gyroAccelerationYAxis;
extern float gyroAccelerationZAxis;

extern float magneticXAxis;
extern float magneticYAxis;
extern float magneticZAxis;

extern float gForceAccelerationXAxis;
extern float gForceAccelerationYAxis;
extern float gForceAccelerationZAxis;

extern float MPSAccelerationXAxis;
extern float MPSAccelerationYAxis;
extern float MPSAccelerationZAxis;

extern float deadReckoningXAxis;
extern float deadReckoningYAxis;
extern float deadReckoningZAxis;

extern float pitch;
extern float roll;

extern float gyroIMUCorrectionXAxis;
extern float gyroIMUCorrectionYAxis;
extern float gyroIMUCorrectionZAxis;

extern float gravityX;
extern float gravityY;
extern float gravityZ;

extern bool launch;
extern float yawMagnetometer;

// Function declarations
void readRawSensorData();
void duringFlightOrientationTracking();
void preFlightOrientationTracking();
void duringFlightPositionalTracking();
void preFlightPositionalTracking();
