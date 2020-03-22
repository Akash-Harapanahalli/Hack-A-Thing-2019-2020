#ifndef STEPPERDRIVER_H
#define STEPPERDRIVER_H

/**
 * StepperDriver.h
 * A class to interface directly with the DRV8825 Stepper Driver.
 */

#include "Arduino.h"

class StepperDriver {
private:
    uint8_t  input_dir_;
    uint8_t  input_drive_;
    uint16_t steps_to_complete_;
    uint16_t time_delay_;
    uint32_t time_of_last_step_;
    bool     drive_state_;

    // Steps the motor by pulsing the drive pin (changing from HIGH to LOW and from LOW to HIGH).
    void internal_step();

public:
    StepperDriver();

    // Sets up the digital out pins.
    void begin(uint8_t input_dir, uint8_t input_drive);

    // Checks to see if the Stepper motor needs to be pulsed, if not, will do nothing.
    void update();

    // Adds the number of steps specified in the direction specified if the stepper is free to move.
    bool stepp(uint16_t steps, int8_t dir, uint16_t time_delay);
    
    // Checks to see if the stepper has steps left to complete.
    bool is_finished();

    // Pulls both pins low to prevent any movement in the stepper due to floating pins.
    void halt();
};


#endif
