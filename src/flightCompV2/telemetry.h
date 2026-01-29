/*
 * Header file for controling ARC-S telemetry output settings
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 27, 2025
 * Edited:    January 27, 2025
 */

#pragma once
#include <Arduino.h>
#include <BMP388_DEV.h>
#include "sensorfusion.h"

class Telemetry {
public:
    void serialIMU();
    void serialIMUDeadReckoning();
    void serialGyro();
    void serialGyroDeadReckoning();
    void serialMagnometer();
    void serialBarometer();
    void serialServoOutputs();
};

