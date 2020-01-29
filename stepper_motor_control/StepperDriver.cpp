#ifndef STEPPERDRIVER_CPP
#define STEPPERDRIVER_CPP

#include "StepperDriver.h"

StepperDriver::StepperDriver(){drive_state_ = HIGH;}

void StepperDriver::begin(uint8_t input_dir, uint8_t input_drive){
    input_dir_ = input_dir;
    input_drive_ = input_drive;
    pinMode(input_dir, OUTPUT);
    pinMode(input_drive, OUTPUT);
}

void StepperDriver::update(){
    uint32_t time_ = micros();
//    Serial.println(time_ - time_of_last_step_);
    if(((time_ - time_of_last_step_) >= time_delay_) && !is_finished()){
//        Serial.print ("hello");
//        Serial.println(time_ - time_of_last_step_);
        internal_step();
    }
}

bool StepperDriver::stepp(uint16_t steps, int8_t dir, uint16_t time_delay){
    if(!is_finished()) return false;
    digitalWrite(input_dir_, ((dir >= 1) ? 1 : 0));
    time_delay_ = time_delay;
    steps_to_complete_ = steps;
}

void StepperDriver::internal_step(){
    drive_state_ = !drive_state_;
    digitalWrite(input_drive_, drive_state_ );
    steps_to_complete_--;
    time_of_last_step_ = micros();
}

bool StepperDriver::is_finished(){
    return (steps_to_complete_ <= 0);
}

#endif
