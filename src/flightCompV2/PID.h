/*
 * Header file for controling ARC-S Servo's
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 25, 2025
 * Edited:    November 17, 2025
 */
 
#ifndef PID_H
#define PID_H

#include <servos.h>
#include <sensorfusion.h>

struct PIDController
{
    double Kp;
    double Ki;
    double Kd;

    double integral;
    double prev_error;

    double integral_min;
    double integral_max;
};

void pid_init(PIDController *pid, double Kp, double Ki, double Kd, double integral_min, double integral_max);

double pid_update(PIDController *pid, double setpoint, double measurement, double dt);

#endif