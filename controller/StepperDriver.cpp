#ifndef STEPPERDRIVER_CPP
#define STEPPERDRIVER_CPP

/**
 * StepperDriver.cpp
 */

#include "StepperDriver.h"

StepperDriver::StepperDriver(){drive_state_ = HIGH;}

/**
 * Sets up the digital out pins.
 * @param input_dir    Digital pin defining the directional input
 * @param input_drive  Digital pin defining the pulsing input
 */
void StepperDriver::begin(uint8_t input_dir, uint8_t input_drive){
    input_dir_ = input_dir;
    input_drive_ = input_drive;
    pinMode(input_dir, OUTPUT);
    pinMode(input_drive, OUTPUT);
}

/**
 * Checks to see if the Stepper motor needs to be pulsed, if not, will do nothing.
 */
void StepperDriver::update(){
    uint32_t time_ = micros();
    if(((time_ - time_of_last_step_) >= time_delay_) && !is_finished()){
        // Internally step the motor if the time has changed enough since the previous step,
        // and if there are steps left to complete.
        internal_step();
    }
}

/**
 * Adds the number of steps specified in the direction specified if the stepper is free to move.
 * @param steps       Number of steps to move
 * @param dir         Direction to move in, -1 or 1
 * @param time_delay  Time delay between pulses, increase for slower speed
 * @return false if stepper is not free, true if steps have been added to the queue.
 */
bool StepperDriver::stepp(uint16_t steps, int8_t dir, uint16_t time_delay){
    if(!is_finished()) return false;
    digitalWrite(input_dir_, ((dir >= 1) ? 1 : 0));
    time_delay_ = time_delay;
    steps_to_complete_ = steps;
    return true;
}

/**
 * Steps the motor by pulsing the drive pin (changing from HIGH to LOW and from LOW to HIGH).
 */
void StepperDriver::internal_step(){
    // HIGH (1) --> LOW (0), LOW(0) --> HIGH (1)
    drive_state_ = !drive_state_;
    digitalWrite(input_drive_, drive_state_ );
    steps_to_complete_--;
    time_of_last_step_ = micros();
}

/**
 * Checks to see if the stepper has steps left to complete.
 * @return true if stepper is finished with its movement, false if stepper has steps left to complete.
 */
bool StepperDriver::is_finished(){
    return (steps_to_complete_ <= 0);
}

/**
 * Pulls both pins low to prevent any movement in the stepper due to floating pins.
 */
void StepperDriver::halt(){
    digitalWrite(input_dir_, LOW);
    digitalWrite(input_drive_, LOW);
}

#endif
