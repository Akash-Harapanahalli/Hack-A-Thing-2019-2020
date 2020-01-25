#ifndef STEPPERDRIVER_H
#define STEPPERDRIVER_H

#include "Arduino.h"

class StepperDriver {
private:
    uint8_t  input_dir_;
    uint8_t  input_drive_;
    uint16_t steps_to_complete_;
    uint16_t time_delay_;
    uint32_t time_of_last_step_;
    bool     drive_state_;

    void internal_step();

public:
    StepperDriver();

    void begin(uint8_t input_dir, uint8_t input_drive);
    void update();
    bool is_finished();
    bool stepp(uint16_t steps, int8_t dir, uint16_t time_delay);

};


#endif
