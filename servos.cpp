/*
 * Program to control ARC-S Servo's
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 25, 2025
 * Edited:    November 17, 2025
 */

#include "servos.h"
#include "sensorfusion.h"
#include <EEPROM.h>



int initialOffset = 1500;
String inputString = "";

AxisServo servos[] = 
{
  {"+Y", Servo(), 3, initialOffset},
  {"-X", Servo(), 4, initialOffset},
  {"-Y", Servo(), 5, initialOffset},
  {"+X", Servo(), 6, initialOffset}
};



const int NUM_SERVOS = sizeof(servos) / sizeof(servos[0]);


void writeServoOffsetEEPROM(int servoIndex)
{
  int addr = servoIndex * 2;                  // 2 bytes per servo
  uint16_t val = servos[servoIndex].offset;  // current offset
  EEPROM.write(addr, val >> 8);              // high byte
  EEPROM.write(addr + 1, val & 0xFF);        // low byte
  EEPROM.commit();                             // save immediately
}


void loadOffsetsFromEEPROM() 
{
  for (int i = 0; i < NUM_SERVOS; i++) 
  {
    int addr = EEPROM_START + i * 2;
    uint16_t val = (EEPROM.read(addr) << 8) | EEPROM.read(addr + 1);
    if (val != 0xFFFF) 
    { // ignore empty EEPROM
      servos[i].offset = val;
      servos[i].servo.writeMicroseconds(val); // center servo
      Serial.print("Loaded ");
      Serial.print(servos[i].name);
      Serial.print(" offset: ");
      Serial.println(val);
    }
  }
}


// ---- Function: Process Command ----
void processCommand(String cmd) 
{
  cmd.trim();  // Remove whitespace
  if (cmd.length() == 0) 
  {
    return;
  }

  int spaceIndex = cmd.indexOf(' ');
  if (spaceIndex == -1) 
  {
    Serial.println("Invalid command. Use: ServoName Offset");
    return;
  }

  String servoName = cmd.substring(0, spaceIndex);
  String offsetStr = cmd.substring(spaceIndex + 1);
  int newOffset = offsetStr.toInt();

  bool found = false;
  for (int i = 0; i < NUM_SERVOS; i++) 
  {
    if (servos[i].name == servoName) 
    {
      servos[i].offset = newOffset;  // Update offset
      servos[i].servo.writeMicroseconds(newOffset);  // Move servo
      Serial.print("Updated ");
      Serial.print(servoName);
      Serial.print(" to offset ");
      Serial.println(newOffset);
      writeServoOffsetEEPROM(i);                    // Save to EEPROM
      found = true;
      break;
    }
  }
}


void readSerialAndUpdateServo() 
{
  while (Serial.available() > 0) 
  {
    char c = Serial.read();
    if (c == '\n') 
    {    // End of input
      processCommand(inputString);
      inputString = "";               // Clear for next command
    } 
    else 
    {
      inputString += c;
    }
  }
}

void ControlServo(String name, float angle) 
{
  for (int i = 0; i < NUM_SERVOS; i++) 
  {
    if (servos[i].name == name) 
    {
      int deltaUs = mapGyroToServo(angle, -90, 90);
      servos[i].writeDeltaUs(deltaUs);
      return;
    }
  }
}


float mapGyroToServo(float angle, float minAngle, float maxAngle) 
{
  angle = constrain(angle, minAngle, maxAngle);

  return map(angle * 100,
             minAngle * 100, maxAngle * 100,
             -500, 500); // ±500 µs authority
}


void pitchYawControlLoop() 
{
  // X-axis rotation controlled by servos on Y-axis
  ControlServo("+Y", gyroAngle_deg.x);
  ControlServo("-Y", -1 * gyroAngle_deg.x);

  // Y-axis rotation controlled by servos on X-axis
  ControlServo("+X", -1 * gyroAngle_deg.y);
  ControlServo("-X", gyroAngle_deg.x);



}
void rollControlLoop() 
{
  // X-axis rotation controlled by servos on Y-axis
  //ControlServo("+Y", gyroPositionZAxis);
  //ControlServo("-Y", -1 * gyroPositionZAxis);
  ControlServo("+X", gyroAngle_deg.z / 90); //90 DPS authority limitor
  ControlServo("-X", gyroAngle_deg.z / 90);

}
