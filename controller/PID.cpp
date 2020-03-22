#ifndef HARDWARELIBRARIES_PID_CPP
#define HARDWARELIBRARIES_PID_CPP

/**
 * PID.cpp
 * SEE: PID.h
 */

#include "PID.h"

PID::PID(){}

/**
 * Constructor
 * @param kP         Proportional constant
 * @param kI         Integral Constant
 * @param kD         Derivative Constant
 * @param min_power  Minimum power to output from the controller
 * @param max_power  Maximum power to output from the controller
 */
PID::PID(float kP, float kI, float kD, int16_t min_power, int16_t max_power):
    kP_(kP),
    kI_(kI),
    kD_(kD),
    min_power_(min_power),
    max_power_(max_power) {
        old_time_ = micros();
    }

/**
 * Sets the constants outside of the constructor.
 * @param kP  Proportional constant
 * @param kI  Integral constant
 * @param kD  Derivative constsnt
 */
void PID::set_constants(float kP, float kI, float kD){
    kP_ = kP;
    kI_ = kI;
    kD_ = kD;
}

/**
 * Iterates once through the PID controller and returns the correctional power.
 * This needs to be called within a closed loop where the parameter error is constantly updated.
 * @param error  Error to pass into the PID controller
 * @return Correctional power determined by the controller
 */
int16_t PID::step(float error){
    uint32_t new_time = micros();
    uint8_t dt = new_time - old_time_;
    old_time_ = new_time;

    // Riemann Sum
    integral_ += error * dt;

    // Slope Formula
    derivative_ = (error - prev_error_) / dt;
    prev_error_ = error;

    // Adding the three terms
    int16_t power = error * kP_ + integral_ * kI_ + derivative_ * kD_;

    // Capping the power at max_power
    power = (power >  max_power_) ?  max_power_ : power;
    power = (power < -max_power_) ? -max_power_ : power;
    return power;
} 

#endif
