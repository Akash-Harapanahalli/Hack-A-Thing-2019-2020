#ifndef __ENCODER_H__
#define __ENCODER_H__

/**
 * Encoder.h
 */

#include <FreqMeasureMulti.h>

class Encoder {
private:
    uint8_t  input;
    int32_t  position;          // position in teeth
    int16_t  speed;             // speed in teeth / s
    uint16_t toneWheelTeeth;

}



#endif