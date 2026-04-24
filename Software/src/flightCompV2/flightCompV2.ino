/*
 * Program to combine all ARC-S Files into a flight profile for the ARC-S 2026 Fair
 *
 * Name:      Thor Kopenkoskey
 * Created:   November 17, 2025
 * Edited:    Feb 26, 2025
 */

#include "telemetry.h"
#include <sensorfusion.h>
#include <servos.h>
#include "flash.h"
#include "sdcard.h"
#include "PID.h"

Telemetry telemetry;

#define SerialPort Serial
#define I2C_SDA 9
#define I2C_SCL 46

extern craftState vehicle;

void setup() 
{
  delay(1000);
  Serial.setRxBufferSize(512);  // before begin(); reduces blocking when host is slow or disconnected
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Serial OK");

  EEPROM.begin(EEPROM_SIZE);

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(50000);
  Wire.setTimeout(200);

  delay(100);

  Wire.beginTransmission(0x00);  

  uint8_t error = Wire.endTransmission();
  if (error != 0 && error != 2) 
  {  // 2 = NACK (expected), other errors indicate problems
    Serial.print("WARNING: I2C bus test returned error code: ");
    Serial.println(error);
  }

  vehicle.initalizeSensors();
  
  for (int i = 0; i < NUM_SERVOS; i++) 
  {
    servos[i].init_servo();
  }

  loadOffsetsFromEEPROM();  // restore trims on startup
  Serial.println("Servo system ready.");

  initializeSDCard();

  //Calibrate sensors
  vehicle.readGAM();
  vehicle.calibrateGyroBias();
  initRollController();
  initYawController();
  initPitchController();
  setupFlash();

}

const uint32_t LOOP_US = 1000; // 1 kHz
const uint32_t PRINT = 100; // 0.1 kHz

static uint32_t lastLoop = 0;
static uint32_t lastLoopPrint = 0;

uint32_t setupFinish = micros();

bool run_once = false;

void loop() 
{

  uint8_t error = Wire.endTransmission();
  if (error != 0 && error != 2) 
  {  // 2 = NACK (expected), other errors indicate problems
    Serial.print("WARNING: I2C bus test returned error code: ");
    Serial.println(error);
  }

    uint32_t now = micros();
    if (now - lastLoop < LOOP_US) return;

    float dt = (now - lastLoop) * 1e-6;
    lastLoop = now;
  //There are errors with serial prints outside of a timed loop such as this... outputting serial to fast can result in errors and hanging the ESP32 controller
  //BE VERY CAREFUL WHEN ADDING MORE SERIAL FUNCTIONS ON THE SAME TIMER AND IN SYNC
  if (now - lastLoopPrint > PRINT)
  {
    telemetry.serialServoOutputs();
    lastLoopPrint = now;
  }

  readSerialAndUpdateServo();

  vehicle.readGAM();

  //calibrate after 1 second of looping for warmup and calibration
  static bool gyroCalibrated = false;

  // Run once after 1 second, this allows sensor to warm up
  if (!gyroCalibrated && (now - setupFinish >= 1000))
  {
      vehicle.calibrateGyroBias();
      gyroCalibrated = true;
  }

  if (vehicle.launch == false) 
  {
    vehicle.preFlightPositionalTracking(dt);
    vehicle.preFlightOrientationTracking(dt);

    if (vehicle.gyro_dps.z > 1.5) 
    {
      vehicle.launch = true;
    }
  }

  if (vehicle.launch == true) 
  {
    
    if (bool run_once = false)
    {
      uint32_t since_launch = micros();
      startLogging();
      
      run_once = true;
    }

    vehicle.duringFlightPositionalTracking(dt);
    vehicle.duringFlightOrientationTracking(dt);

    logData(vehicle.roll, vehicle.pitch, vehicle.yaw, vehicle.altitude_m);
  }

  if (vehicle.landed == true) 
  {
    closeFlash();
  }


    flightControlUpdate(dt);

    delay(50);
}

 