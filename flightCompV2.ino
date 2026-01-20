
#include <sensorfusion.h>
#include <servos.h>
#include "flash.h"
#include "sdcard.h"
#include <BMP388_DEV.h>
extern BMP388_DEV bmp388;
#define SerialPort Serial
#define I2C_SDA    9
#define I2C_SCL    46

// I2C Bus Recovery Function - call this if ERROR 259 occurs
void recoverI2CBus() {
  Serial.println("I2C Bus Recovery: Resetting I2C driver...");
  
  // End current I2C session
  Wire.end();
  delay(10);
  
  // Manual bus recovery
  pinMode(I2C_SCL, OUTPUT);
  pinMode(I2C_SDA, INPUT_PULLUP);
  
  // Clock recovery sequence
  for (int i = 0; i < 10; i++) {
    digitalWrite(I2C_SCL, LOW);
    delayMicroseconds(10);
    digitalWrite(I2C_SCL, HIGH);
    delayMicroseconds(10);
  }
  
  // Ensure SDA is released
  pinMode(I2C_SDA, INPUT_PULLUP);
  delay(10);
  
  // Reinitialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);
  Wire.setTimeout(1000);
  delay(50);
  
  Serial.println("I2C Bus Recovery: Complete");
}

void setup()
{
  launch = false;

  delay(300);
  Serial.begin(115200);
  delay(2000);
  Serial.println("Serial OK");



  // I2C Bus Recovery: Clear any stuck bus state BEFORE initializing Wire
  // Proper I2C bus recovery sequence - must be done before Wire.begin()
  pinMode(I2C_SCL, OUTPUT);
  pinMode(I2C_SDA, INPUT_PULLUP);
  
  // Clock recovery sequence - pulse SCL to release stuck devices
  // This releases any device holding SDA low
  for (int i = 0; i < 10; i++) 
  {
    digitalWrite(I2C_SCL, LOW);
    delayMicroseconds(10);
    digitalWrite(I2C_SCL, HIGH);
    delayMicroseconds(10);
  }
  
  // Ensure SDA is released (should be high with pullup)
  pinMode(I2C_SDA, INPUT_PULLUP);
  delayMicroseconds(50);
  

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);  
  Wire.setTimeout(1000);  
  

  delay(100);  
  
  Wire.beginTransmission(0x00);  // General call address (should always NACK, but tests bus)
  uint8_t error = Wire.endTransmission();
  if (error != 0 && error != 2) 
  {  // 2 = NACK (expected), other errors indicate problems
    Serial.print("WARNING: I2C bus test returned error code: ");
    Serial.println(error);
  }

  SPI.begin();   // DO NOT start a transaction here

  // =====================
  // IMU
  // =====================
  if (!IMU.begin())
  {
    Serial.println("IMU failed!");
  }
  else
  {
    Serial.println("IMU OK");
  }

  // =====================
  // Magnetometer (I2C)
  // =====================
  if (!magnetometer.begin_I2C(LIS3MDL_ADDR, &Wire))
  {
    Serial.println("Magnetometer failed!");
  }
  else
  {
    Serial.println("Mag OK");
  }
  
  
  if (!bmp388.begin(BMP388_I2C_ALT_ADDR)) 
  {   // try ALT first
    Serial.println("BMP388 not found at 0x77, trying 0x76...");
    if (!bmp388.begin(BMP388_I2C_ADDR)) 
    {
      Serial.println("BMP388 FAILED");
    } 
    else 
    {
      Serial.println("BMP388 OK @ 0x76");
    }
  } 
  else 
  {
    Serial.println("BMP388 OK @ 0x77");
  }

  bmp388.setTimeStandby(TIME_STANDBY_80MS);
  bmp388.setPresOversampling(OVERSAMPLING_X4);
  bmp388.setTempOversampling(OVERSAMPLING_X4);
  bmp388.startNormalConversion();

  for (int i = 0; i < NUM_SERVOS; i++)
  {
    servos[i].init_servo();
  }

}

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 100;


void loop() 
{


  readRawSensorData();
  pitchYawControlLoop();
  if (gyroAccelerationZAxis > 90) {
    rollControlLoop();
  }


  if (!launch) {

    preFlightPositionalTracking();
    preFlightOrientationTracking();

    if (gForceAccelerationZAxis > 1.5) {
      launch = true;
    }
  }

  if (launch) {
    duringFlightPositionalTracking();
    duringFlightOrientationTracking();
  }


  unsigned long currentTime = millis();

  // Only run every 100 ms
  if (currentTime - lastSendTime >= sendInterval) {
    lastSendTime = currentTime;
// IMU Accelerometer (g-force)
  Serial.print("ACC_X:"); Serial.print(gForceAccelerationXAxis); Serial.print('\t');
  Serial.print("ACC_Y:"); Serial.print(gForceAccelerationYAxis); Serial.print('\t');
  Serial.print("ACC_Z:"); Serial.println(gForceAccelerationZAxis);

  // Dead Reckoning
  Serial.print("DR_X:"); Serial.print(deadReckoningXAxis); Serial.print('\t');
  Serial.print("DR_Y:"); Serial.print(deadReckoningYAxis); Serial.print('\t');
  Serial.print("DR_Z:"); Serial.println(deadReckoningZAxis);

  // Magnetometer
  Serial.print("MX:"); Serial.print(magneticXAxis); Serial.print('\t');
  Serial.print("MY:"); Serial.print(magneticYAxis); Serial.print('\t');
  Serial.print("MZ:"); Serial.println(magneticZAxis);

  // Gyroscope
  Serial.print("GX:"); Serial.print(gyroAccelerationXAxis); Serial.print('\t');
  Serial.print("GY:"); Serial.print(gyroAccelerationYAxis); Serial.print('\t');
  Serial.print("GZ:"); Serial.println(gyroAccelerationZAxis);

  // Gyroscope Position
  Serial.print("GP_X:"); Serial.print(gyroPositionXAxis); Serial.print('\t');
  Serial.print("GP_Y:"); Serial.print(gyroPositionYAxis); Serial.print('\t');
  Serial.print("GP_Z:"); Serial.println(gyroPositionZAxis);

  Serial.println(); // Blank line between samples
  }
}