#ifndef __MOTORDRIVER_CPP__
#define __MOTORDRIVER_CPP__

/**
 * MotorDriver.cpp
 * SEE: MotorDriver.h
 */

#include "MotorDriver.h"

/**
 * Sets up the digital out pins.
 * @param input1 First input pin
 * @param input2 Second input pin
 */
void MotorDriver::begin(int input1, int input2){
    input1_ = input1;
    input2_ = input2;
    pinMode(input1_, OUTPUT);
    pinMode(input2_, OUTPUT);
}

/**
 * Power is taken in the range from -255 to 255 (dual 8-bit)
 * @param power Output power for motors in the range from -255 to 255.
 */
void MotorDriver::setPower(int power){
    // Powering the motors is based on a potential difference between the in1 and in2 leads on the driver.
    // When in1 is LOW, the motor is powered "forward" and when in2 is LOW, the motor is powered "backward".
    if(power > 0){
        analogWrite(input1_, LOW);
        // Cap the output power to 255
        (power >= 255) ? analogWrite(input2_, 255) : analogWrite(input2_, power);
    } else if(power < 0){
        // Cap the output power to 255
        (abs(power) >= 255) ? analogWrite(input1_, 255) : analogWrite(input1_, abs(power));
        analogWrite(input2_, LOW);
    } else {
        analogWrite(input1_, LOW);
        analogWrite(input2_, LOW);
    }
}

#endif
