#ifndef ENCODER_H
#define ENCODER_H

#include "Arduino.h"

class Encoder {
private:
    uint8_t  pin_;
    uint16_t  current_position_; // position in teeth

public:
    Encoder();

    void begin(uint8_t pin, void (*isr)());
    void isr_();
    uint16_t get_position();
};



#endif
