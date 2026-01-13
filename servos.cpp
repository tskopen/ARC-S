#include "servos.h"
#include "sensorfusion.h"
#include <EEPROM.h>

AxisServo servos[] = 
{
  {"+Y", Servo(), 3},
  {"-X", Servo(), 4},
  {"-Y", Servo(), 5},
  {"+X", Servo(), 6}
};


struct ServoOffsets 
{
  float posY;
  float negX;
  float negY;
  float posX;
};

const int EEPROM_START_ADDR = 0; // starting EEPROM address


const int NUM_SERVOS = sizeof(servos) / sizeof(servos[0]);

void ControlServo(String name, float angle) 
{
  for (int i = 0; i < NUM_SERVOS; i++) 
  {
    if (servos[i].name == name) {
      float servoAngle = mapGyroToServo(angle, -90, 90);  // Map to servo range
      servos[i].writeAngle(servoAngle);        // Use mapped value
      return;
    }
  }
}

float mapGyroToServo(float angle, float minAngle, float maxAngle) 
{
  // Clamp angle
  if (angle < minAngle) angle = minAngle;
  if (angle > maxAngle) angle = maxAngle;
  // Map from [-90, 90] to [0, 180]
  return map(angle * 100, minAngle * 100, maxAngle * 100, 0, 180);
}

void pitchYawControlLoop() 
{
  // X-axis rotation controlled by servos on Y-axis
  ControlServo("+Y", gyroPositionXAxis);
  ControlServo("-Y", -1 * gyroPositionXAxis);

  // Y-axis rotation controlled by servos on X-axis
  ControlServo("+X", -1 * gyroPositionYAxis);
  ControlServo("-X", gyroPositionYAxis);
}
void rollControlLoop() 
{
  // X-axis rotation controlled by servos on Y-axis
  //ControlServo("+Y", gyroPositionZAxis);

  ControlServo("+X", gyroAccelerationZAxis / 90); //90 DPS authority limitor
  ControlServo("-X",  gyroAccelerationZAxis / 90);
}



void EEPROM_writeFloat(int address, float value) 
{
  byte *p = (byte*)(void*)&value;
  for (int i = 0; i < 4; i++) 
  {
    EEPROM.write(address + i, *(p + i));
  }
}

float EEPROM_readFloat(int address) 
{
  float value = 0.0;
  byte *p = (byte*)(void*)&value;
  for (int i = 0; i < 4; i++) 
  {
    *(p + i) = EEPROM.read(address + i);
  }
  return value;
}

void saveServoOffsets(const ServoOffsets &offsets) 
{
  EEPROM_writeFloat(EEPROM_START_ADDR + 0, offsets.posY);
  EEPROM_writeFloat(EEPROM_START_ADDR + 4, offsets.negX);
  EEPROM_writeFloat(EEPROM_START_ADDR + 8, offsets.negY);
  EEPROM_writeFloat(EEPROM_START_ADDR + 12, offsets.posX);
}

ServoOffsets loadServoOffsets() 
{
  ServoOffsets offsets;
  offsets.posY = EEPROM_readFloat(EEPROM_START_ADDR + 0);
  offsets.negX = EEPROM_readFloat(EEPROM_START_ADDR + 4);
  offsets.negY = EEPROM_readFloat(EEPROM_START_ADDR + 8);
  offsets.posX = EEPROM_readFloat(EEPROM_START_ADDR + 12);
  return offsets;
}
