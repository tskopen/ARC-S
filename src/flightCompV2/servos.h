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
  int direction;

  void init_servo() 
  {
    servo.setPeriodHertz(50);
    servo.attach(pin);
    servo.writeMicroseconds(offset); // center on init
  }

  void writeDeltaUs(int deltaUs) 
  {
    int pulse = offset + deltaUs;
    pulse = constrain(pulse, 500, 2500);
    servo.writeMicroseconds(pulse);
  }
};

//UTIL
inline int clampServoUs(int us);
void loadOffsetsFromEEPROM();
void readSerialAndUpdateServo();
void processCommand(String cmd);
void ControlServo(String name, float angle);
float mapGyroToServo(float angle, float minAngle, float maxAngle);
float clamp(float value, float minValue, float maxValue);


// CONTROL LOOP

const float KP_PITCH = 0.9f;
const float KP_YAW   = 0.9f;
const float KP_ROLL  = 0.5f;

const float SERVO_LIMIT_DEG = 60.0f;   // mechanical authority

void flightControlUpdate();
void pitchYawControlLoop();
void rollControlLoop();
void applyServoMix();

extern const int NUM_SERVOS;
extern AxisServo servos[];
#endif