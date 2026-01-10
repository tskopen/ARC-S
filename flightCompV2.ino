#include <sensorfusion.h>
#include <servos.h>
#include "flash.h"
#include "FlightLoraRadio.cpp"



void setup() {
  Serial.begin(115200);

  Wire.begin(I2C2_SDA, I2C2_SCL);
  Wire.setClock(100000);

  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); 


  pinMode(lora5MOSI, OUTPUT);


  pinMode(lora7MISO, INPUT);


  pinMode(lora6SCK,OUTPUT);


  pinMode(lora8CSN,OUTPUT);


  IMU.begin();
  magnetometer.begin_I2C(LIS3MDL_ADDR, &Wire);
  launch = false;

  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].init_servo();
  }

  setupFlash();

}
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10; 

void loop(){
  readRawSensorData(); 
  pitchYawControlLoop();
  if (gyroAccelerationZAxis > 90){
    rollControlLoop();
  }


  if (!launch){
    
  preFlightPositionalTracking();
  preFlightOrientationTracking();

    if (gForceAccelerationZAxis > 1.5)
    {
      launch = true;
    }
  }

  if (launch){
  duringFlightPositionalTracking();
  duringFlightOrientationTracking();
  }
  
/*
  Serial.print(gForceAccelerationXAxis); Serial.print('\t');
  Serial.print(gForceAccelerationYAxis); Serial.print('\t');
  Serial.print(gForceAccelerationZAxis); Serial.print('\t');

  Serial.print(deadReckoningXAxis); Serial.print('\t');
  Serial.print(deadReckoningYAxis); Serial.print('\t');
      Serial.print(deadReckoningZAxis);

*/
  unsigned long currentTime = millis();

  // Only run every 100 ms
  if (currentTime - lastSendTime >= sendInterval) {
  lastSendTime = currentTime;
  Serial.print("MX:");
  Serial.println(magneticXAxis);
  Serial.print("MY:");
  Serial.println(magneticYAxis);
  Serial.print("MZ:");
  Serial.println(magneticZAxis);

  Serial.print("GX:");
  Serial.println(gyroAccelerationXAxis);
  Serial.print("GY:");
  Serial.println(gyroAccelerationYAxis);
  Serial.print("GZ:");
  Serial.println(gyroAccelerationZAxis);

/*
  Serial.print(gyroAccelerationXAxis); Serial.print('\t');
  Serial.print(gyroAccelerationYAxis); Serial.print('\t');
  Serial.print(gyroAccelerationZAxis); Serial.print('\t');


  Serial.print(gyroPositionXAxis); Serial.print('\t');
  Serial.print(gyroPositionYAxis); Serial.print('\t');
  Serial.print(gyroPositionZAxis); Serial.println('\t');

  Serial.print(magneticXAxis); Serial.print('\t');
  Serial.print(magneticYAxis); Serial.print('\t');
  Serial.println(magneticZAxis); 
*/
}
}