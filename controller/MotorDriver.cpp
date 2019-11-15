#ifndef __MOTORDRIVER_CPP__
#define __MOTORDRIVER_CPP__

/**
 * MotorDriver.cpp
 */

#include "MotorDriver.h"
#include <Arduino.h>

/**
 * Sets up the digital out pins and the PID if specified.
 */
void MotorDriver::setup(int _in1, int _in2){
    this->in1 = _in1;
    this->in2 = _in2;
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
}

/**
 * Power is taken in the range from -255 to 255 (dual 8-bit)
 * Powering the motors is based on a potential difference between the in1 and in2 leads on the driver.
 * When in1 is LOW, the motor is powered "forward" and when in2 is LOW, the motor is powered "backward".
 */
void MotorDriver::setPower(int power){
    if(power > 0){
        analogWrite(this->in1, LOW);
        analogWrite(this->in2, power);
    } else if(power < 0){
        analogWrite(this->in1, abs(power));
        analogWrite(this->in2, LOW);
    } else {
        analogWrite(this->in1, LOW);
        analogWrite(this->in2, LOW);
    }
}

#endif
