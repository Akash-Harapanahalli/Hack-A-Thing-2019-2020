#include "Encoder.h"

Encoder::Encoder(){}

void Encoder::begin(uint8_t pin, void (*isr)()){
    pin_ = pin;
    pinMode(pin_, INPUT);
    attachInterrupt(digitalPinToInterrupt(pin_), isr, FALLING);
}

void Encoder::isr_(){
    current_position_++;
}

uint16_t Encoder::get_position(){
    return current_position_;
}
