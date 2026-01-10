#ifndef SERVOS_H
#define SERVOS_H

#include <ESP32Servo.h>

struct AxisServo {
  String name;           // Label (e.g., "+X")
  Servo servo;           // Servo object
  int pin;               // GPIO pin

  void init_servo() {
    servo.setPeriodHertz(50); // 50 Hz for servos
    servo.attach(pin);
  }

  void writeAngle(double angle) {
    servo.write(angle); // 0–180 degrees
  }

};

void ControlServo(String name, float angle);
float mapGyroToServo(float angle, float minAngle, float maxAngle);
void pitchYawControlLoop();
void rollControlLoop();
extern const int NUM_SERVOS;
extern AxisServo servos[];

#endif