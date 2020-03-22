#ifndef __MOTORDRIVER_H__
#define __MOTORDRIVER_H__

/**
 * MotorDriver.h
 * A wrapper to work with the DRV8871 Motor Driver from TI.
 */
 
#include "Arduino.h"

class MotorDriver {
private:
    int input1_;
    int input2_;

public:
    // Sets up the digital out pins.
    void begin(int input1, int input2);

    // Power is taken in the range from -255 to 255 (dual 8-bit).
    void setPower(int power);
};

#endif
