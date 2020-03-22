#ifndef HARDWARELIBRARIES_PID_H
#define HARDWARELIBRARIES_PID_H

#include "Arduino.h"

/**
 * PID.h
 * PID's are used for converting error into usable power output.
 * P - Proportion --> adding power as a proportion of the error
 * I - Integral   --> adding power as a proportion of the integral of error, leveling off any error present over longer periods of time.
 * D - Derivative --> removing power as a proportion of the derivative of error, preventing possible overshoots and restricting too large changes in error.
 */

class PID {
private:
    float kP_, kI_, kD_;
    uint32_t old_time_;
    float integral_;
    float derivative_, prev_error_;
    int16_t min_power_, max_power_;

public:
    PID();
    PID(float kP, float kI, float kD, int16_t min_power, int16_t max_power);

    // Sets the constants outside of the constructor.
    void set_constants(float kP, float kI, float kD);

    // Iterates once through the PID controller and returns the correctional power.
    // This needs to be called within a closed loop where the parameter error is constantly updated.
    int16_t step(float error);
};

#endif
