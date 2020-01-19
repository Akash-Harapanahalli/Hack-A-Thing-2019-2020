#ifndef __MOTORDRIVER_H__
#define __MOTORDRIVER_H__

/**
 * MotorDriver.h
 * A wrapper to work with the DRV8871 Motor Driver.
 */

class MotorDriver {
private:
    /**
     * The two leads that take input signal to power the motor.
     */
    int input1;
    int input2;

public:
    /**
     * Sets up the digital out pins and the PID if specified.
     */
    void setup(int _input1, int _input2);

    /**
     * Power is taken in the range from -255 to 255 (dual 8-bit)
     */
    void setPower(int power);

};

#endif
