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
void MotorDriver::setup(int _input1, int _input2){
    this->input1 = _input1;
    this->input2 = _input2;
    pinMode(_input1, OUTPUT);
    pinMode(_input2, OUTPUT);
}

/**
 * Power is taken in the range from -255 to 255 (dual 8-bit)
 * Powering the motors is based on a potential difference between the in1 and in2 leads on the driver.
 * When in1 is LOW, the motor is powered "forward" and when in2 is LOW, the motor is powered "backward".
 */
void MotorDriver::setPower(int power){
    if(power > 0){
        analogWrite(this->input1, LOW);
        analogWrite(this->input2, power);
    } else if(power < 0){
        analogWrite(this->input1, abs(power));
        analogWrite(this->input2, LOW);
    } else {
        analogWrite(this->input1, LOW);
        analogWrite(this->input2, LOW);
    }
}

#endif
