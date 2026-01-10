#include "servos.h"
#include "sensorfusion.h"


AxisServo servos[] = {
  {"+Y", Servo(), 12},
  {"-X", Servo(), 13},
  {"-Y", Servo(), 27},
  {"+X", Servo(), 14}
};

const int NUM_SERVOS = sizeof(servos) / sizeof(servos[0]);

void ControlServo(String name, float angle) {
  for (int i = 0; i < NUM_SERVOS; i++) {
    if (servos[i].name == name) {
      float servoAngle = mapGyroToServo(angle, -90, 90);  // Map to servo range
      servos[i].writeAngle(servoAngle);        // Use mapped value
      return;
    }
  }
}

float mapGyroToServo(float angle, float minAngle, float maxAngle) {
  // Clamp angle
  if (angle < minAngle) angle = minAngle;
  if (angle > maxAngle) angle = maxAngle;
  // Map from [-90, 90] to [0, 180]
  return map(angle * 100, minAngle * 100, maxAngle * 100, 0, 180);
}

void pitchYawControlLoop() {
  // X-axis rotation controlled by servos on Y-axis
  ControlServo("+Y", gyroPositionXAxis);
  ControlServo("-Y", -1 * gyroPositionXAxis);

  // Y-axis rotation controlled by servos on X-axis
  ControlServo("+X", -1 * gyroPositionYAxis);
  ControlServo("-X", gyroPositionYAxis);
}
void rollControlLoop() {
  // X-axis rotation controlled by servos on Y-axis
  //ControlServo("+Y", gyroPositionZAxis);
  //ControlServo("-Y", -1 * gyroPositionZAxis);

  ControlServo("+X", gyroAccelerationZAxis / 90); //90 DPS authority limitor
  ControlServo("-X", gyroAccelerationZAxis / 90);
}
