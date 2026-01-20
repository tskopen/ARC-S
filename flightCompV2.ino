
#include <sensorfusion.h>
#include <servos.h>
#include "flash.h"
#include "sdcard.h"
#include <BMP388_DEV.h>                           

#define SerialPort Serial
#define I2C_SDA    9
#define I2C_SCL    46
float temperature, pressure, altitude;            // Create the temperature, pressure and altitude variables
BMP388_DEV bmp388; 


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

  bmp388.begin();                                 // Default initialisation, place the BMP388 into SLEEP_MODE 
  bmp388.setTimeStandby(TIME_STANDBY_1280MS);     // Set the standby time to 1.3 seconds
  bmp388.startNormalConversion();  

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
  
  
  


  for (int i = 0; i < NUM_SERVOS; i++)
  {
    servos[i].init_servo();
  }

}

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 100;


unsigned long lastTime = 0;
float dt = 0;

void loop() 
{
    unsigned long currentTime = micros();          // microseconds
    dt = (currentTime - lastTime) / 1000000.0;    // convert to seconds
    lastTime = currentTime;

  readRawSensorData();
  pitchYawControlLoop();
  if (gyroAccelerationZAxis > 90) {
    rollControlLoop();
  }


  if (!launch) {

    preFlightPositionalTracking(dt);
    preFlightOrientationTracking(dt);

    if (gForceAccelerationZAxis > 1.5) 
    {
      launch = true;
    }
  }

  if (launch) 
  {
    duringFlightPositionalTracking(dt);
    duringFlightOrientationTracking(dt);
  }



  // Only run every 100 ms
  if (currentTime - lastSendTime >= sendInterval) {
    lastSendTime = currentTime;

  if (bmp388.getMeasurements(temperature, pressure, altitude))    // Check if the measurement is complete
  {
    Serial.print(temperature);                    // Display the results    
    Serial.print(F("*C   "));
    Serial.print(pressure);    
    Serial.print(F("hPa   "));
    Serial.print(altitude);
    Serial.println(F("m"));  
  }

/*Servos controls
Serial.print(gyroPositionXAxis, 2);      // Pitch +Y
Serial.print(",");
Serial.print(-gyroPositionXAxis, 2);     // Pitch -Y
Serial.print(",");
Serial.print(-gyroPositionYAxis, 2);     // Yaw +X
Serial.print(",");
Serial.print(gyroPositionYAxis, 2);      // Yaw -X
Serial.print(",");
Serial.println(gyroAccelerationZAxis / 90, 2); // Roll +X/-X
*/

  /*
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
*/
  Serial.println(); // Blank line between samples
  }
}