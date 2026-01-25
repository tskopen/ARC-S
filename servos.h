/*
 * Header file for controling ARC-S Servo's
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 25, 2025
 * Edited:    November 17, 2025
 */
 
#ifndef SERVOS_H
#define SERVOS_H

#include <ESP32Servo.h>
#include <EEPROM.h>

#define EEPROM_SIZE 64   // adjust if you add more data
#define EEPROM_START 0   // start address



struct AxisServo 
{
  String name;
  Servo servo;
  int pin;
  int offset; // center in microseconds

  void init_servo() 
  {
    servo.setPeriodHertz(50);
    servo.attach(pin);
    servo.writeMicroseconds(offset); // center on init
  }

  void writeDeltaUs(int deltaUs) 
  {
    int pulse = offset + deltaUs;
    pulse = constrain(pulse, 1500, 3000);
    servo.writeMicroseconds(pulse);
  }
};

void loadOffsetsFromEEPROM();
void readSerialAndUpdateServo();
void processCommand(String cmd);
void ControlServo(String name, float angle);
float mapGyroToServo(float angle, float minAngle, float maxAngle);
void pitchYawControlLoop();
void rollControlLoop();
extern const int NUM_SERVOS;
extern AxisServo servos[];



#endif